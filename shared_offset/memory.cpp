#include <thorin/world.h>
#include <thorin/analyses/cfg.h>
#include <thorin/analyses/scope.h>

#include <iostream>

using namespace thorin;

const Def* max(World* world, const Def* a, const Def* b) {
    const Def* condition = world->cmp_ge(a, b);
    return world->select(condition, a, b);
}

extern "C" {

const Def* get_max_offset (World* world, const App* app) {
    assert(app->num_args() == 3);

    const Def* body = app->arg(1);
    const Def* target = app->arg(2);

    const Def* with_shared_continuation = nullptr;
    for (auto cont : world->copy_continuations()) {
        if (cont->name() == "with_shared") {
            with_shared_continuation = cont;
            break;
        }
    }
    if (!with_shared_continuation) {
        auto offset_arg = target->as<Continuation>()->body()->arg(1);

        Continuation* y = world->continuation(target->type()->op(1)->as<FnType>());
        y->jump(body, {y->param(0), offset_arg, y->param(2)});
        return y;
    } else {
        App* target_app = const_cast<App*>(target->as<Continuation>()->body());
        auto offset_arg = target_app->arg(1);

        const Def* cuda_continuation = nullptr;
        for (auto cont : world->copy_continuations()) {
            if (cont->name() == "cuda") {
                cuda_continuation = cont;
                break;
            }
        }
        assert(cuda_continuation);

        //Find direct with_shared children.
        Scope body_scope = Scope(const_cast<Continuation*>(body->as<Continuation>()));
        for (auto use : with_shared_continuation->uses()) {
            if (body_scope.contains(use)) {
                if (auto child_app = use->isa<App>()) {
                    offset_arg = max(world, world->arithop_add(child_app->op(2), child_app->op(3)), offset_arg);
                }
            }
        }

        //Cuda bodies are not part of this scope, analyze them explicitly.
        for (auto use : cuda_continuation->uses()) {
            if (body_scope.contains(use)) {
                auto cuda_body = use->isa<App>()->op(5);
                Scope cuda_body_scope = Scope(const_cast<Continuation*>(cuda_body->as<Continuation>()));

                for (auto use : with_shared_continuation->uses()) {
                    if (cuda_body_scope.contains(use)) {
                        if (auto child_app = use->isa<App>()) {
                            offset_arg = max(world, world->arithop_add(child_app->op(2), child_app->op(3)), offset_arg);
                        }
                    }
                }
            }
        }

        if (offset_arg != target_app->arg(1)) {
            target_app->unset_op(2);
            target_app->set_op(2, offset_arg);
        }

        throw std::runtime_error("Not finished yet.");
    }
}

const Def* with_shared (World* world, const App* app) {
    if (!world->is_pe_done())
        throw std::runtime_error("This can only be optimized once the rest of the PE is done.");

    assert(app->num_args() == 5);
    const Def* size = app->arg(1);
    const Def* offset = app->arg(2);
    const Def* body = app->arg(3);
    const Def* target = app->arg(4);

    if (!body->isa<Continuation>())
        throw std::runtime_error("Cannot do anything useful here.");

    auto new_child_offset = world->arithop_add(size, offset);

    Scope body_scope = Scope(const_cast<Continuation*>(body->as<Continuation>()));
    const Def* with_shared_continuation = app->callee();
    for (auto use : with_shared_continuation->uses()) {
        if (body_scope.contains(use)) {
            if (auto child_app = use->isa<App>()) { //XXX: This is ugly as blody hell, I should find a better way to implement this.
                auto child_app_nonconst = const_cast<App*>(child_app);
                child_app_nonconst->unset_op(3);
                child_app_nonconst->set_op(3, new_child_offset);
            }
        }
    }

    Continuation* y = world->continuation(target->type()->op(1)->as<FnType>());

    const Def* mem = y->param(0);

    y->jump(body, {mem, offset, y->param(1)});

    return y;
}

}
