#include <thorin/world.h>
#include <thorin/analyses/cfg.h>
#include <thorin/analyses/scope.h>

#include <thorin/transform/mangle.h>

#include <iostream>

using namespace thorin;

extern "C" {

const Def* nvvm_add_atomic_or_red_global(World* world, const App* app) {
    int num_uses = 0;

    for (auto user : app->arg(3)->as<Continuation>()->param(1)->uses()) {
        if (auto app = user->isa<App>()) {
            num_uses += app->arg(1)->as<Continuation>()->param(1)->num_uses();
        } else {
            std::cerr << "Users should be app nodes only\n";
            num_uses += 666;
        }
    }

    bool build_reduction = (num_uses == 0);
    bool build_atomic = (!build_reduction) && world->is_pe_done();

    if (build_reduction) {
        const Continuation* reduce = nullptr;
        //for (auto def : world->defs()) {
        for (auto [_, def] : world->externals()) {
            if (def->name() == "private_nvvm_reduce_add_global") {
                reduce = def->isa<Continuation>();
                //if (reduce && reduce->dead_) { reduce = nullptr; }
                if (reduce) break;
            }
        }
        assert(reduce);

        const MemType* mem_t = world->mem_type();
        const PrimType* i32_t = world->type_qs32();

        Continuation* result = world->continuation(world->fn_type({mem_t, world->fn_type({mem_t, i32_t})}));
        Continuation* return_cont = world->continuation(world->fn_type({mem_t}));

        result->jump(reduce, {app->arg(0), app->arg(1), app->arg(2), return_cont});
        return_cont->jump(result->param(1), {return_cont->param(0), world->literal_qs32(0, {})});

        return result;
    } else if (build_atomic) {
        const Continuation* reduce = nullptr;
        //for (auto def : world->defs()) {
        for (auto [_, def] : world->externals()) {
            if (def->name() == "private_nvvm_atomic_add_global") {
                reduce = def->isa<Continuation>();
                //if (reduce && reduce->dead_) { reduce = nullptr; }
                if (reduce) break;
            }
        }
        assert(reduce);

        const MemType* mem_t = world->mem_type();
        const PrimType* i32_t = world->type_qs32();

        Continuation* result = world->continuation(world->fn_type({mem_t, world->fn_type({mem_t, i32_t})}));
        Continuation* return_cont = world->continuation(world->fn_type({mem_t, i32_t}));

        result->jump(reduce, {app->arg(0), app->arg(1), app->arg(2), return_cont});
        return_cont->jump(result->param(1), {return_cont->param(0), return_cont->param(1)});

        return result;
    } else {
        throw std::runtime_error("I don't know what to do!");
    }
}

}
