#include <thorin/world.h>
#include <thorin/analyses/cfg.h>
#include <thorin/analyses/scope.h>

#include <iostream>

using namespace thorin;

extern "C" {

const Def* with_shared (World* world, const App* app) {
    if (!world->is_pe_done())
        throw std::runtime_error("This can only be optimized once the rest of the PE is done.");

    //app->dump();
    //world->dump_scoped();
    assert(app->num_args() == 5);
    //const Def* in_mem = app->arg(0); //Not relevant
    const Def* size = app->arg(1);
    const Def* offset = app->arg(2);
    const Def* body = app->arg(3);
    const Def* target = app->arg(4);

    if (!body->isa<Continuation>()) {
        throw std::runtime_error("Cannot do anything useful here.");
    }

    auto new_child_offset = world->arithop_add(size, offset);

    Scope body_scope = Scope(const_cast<Continuation*>(body->as<Continuation>()));
    const Def* with_shared_continuation = app->callee();
    for (auto use : with_shared_continuation->uses()) {
        if (body_scope.contains(use)) {
            if (auto child_app = use->isa<App>()) { //XXX: This is ugly as blody hell, I should find a better way to implement this.
                auto child_app_nonconst = const_cast<App*>(child_app);
                //child_app_nonconst->op(3)->dump();
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
