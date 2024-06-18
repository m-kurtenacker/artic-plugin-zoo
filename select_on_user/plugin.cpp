#include <thorin/world.h>
#include <thorin/analyses/cfg.h>
#include <thorin/analyses/scope.h>

#include <thorin/transform/mangle.h>

#include <iostream>

using namespace thorin;

extern "C" {

const Def* select_option(World* world, const App* app) {
    int num_uses = 0;

    for (auto user : app->arg(4)->as<Continuation>()->param(1)->uses()) {
        if (auto app = user->isa<App>()) {
            num_uses += app->arg(1)->as<Continuation>()->param(1)->num_uses();
        } else {
            std::cerr << "Users should be app nodes only\n";
            num_uses += 666;
        }
    }

    bool build_ro = (num_uses == 0);
    bool build_rw = (!build_ro) && world->is_pe_done();

    if (build_ro) {

        auto child_cont = app->arg(2);
        auto child = child_cont->as<Continuation>();
        assert(child);

        const MemType* mem_t = world->mem_type();
        const PrimType* i32_t = world->type_qs32();

        Continuation* result = world->continuation(world->fn_type({mem_t, world->fn_type({mem_t, i32_t})}));

        result->jump(child, {result->param(0), app->arg(3), result->param(1)});

        return result;

    } else if (build_rw) {

        auto child_cont = app->arg(1);
        auto child = child_cont->as<Continuation>();
        assert(child);

        const MemType* mem_t = world->mem_type();
        const PrimType* i32_t = world->type_qs32();

        Continuation* result = world->continuation(world->fn_type({mem_t, world->fn_type({mem_t, i32_t})}));

        result->jump(child, {result->param(0), app->arg(3), result->param(1)});

        return result;

    } else {

        throw std::runtime_error("I don't know what to do!");

    }
}

}
