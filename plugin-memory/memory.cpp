#include <thorin/world.h>
#include <thorin/analyses/cfg.h>
#include <thorin/analyses/scope.h>

#include <iostream>

using namespace thorin;

extern "C" {

const Def* static_alloca (World* world, const App* app) {
    assert(app->num_args() == 3);
    const Def* size = app->arg(1);

    Continuation* y = world->continuation(world->fn_type({
                world->mem_type(),
                world->fn_type({
                        world->mem_type(),
                        world->ptr_type(world->indefinite_array_type(world->type_pu8()))
                        })
                }));

    const Def* mem = y->param(0);
    const Def* array;

    if (auto size_lit = size->isa<PrimLit>()) {
        u64 size = size_lit->value().get_u64();
        auto target_type = world->ptr_type(world->indefinite_array_type(world->type_pu8()));
        const Def* inner_array = world->global(world->bottom(world->definite_array_type(world->type_pu8(), size)));
        array = world->bitcast(target_type, inner_array);
    } else {
        const Def* inner_alloca = world->alloc(world->indefinite_array_type(world->type_pu8()), mem, size);
        mem = world->extract(inner_alloca, (int) 0);
        array = world->extract(inner_alloca, 1);
    }

    array->dump();

    y->jump(y->param(1), {mem, array});

    return y;
}

const Def* static_release (World* world, const App* app) {
    assert(app->num_args() == 3);
    const Def* data = app->arg(1);

    Continuation* y = world->continuation(world->fn_type({
                world->mem_type(),
                world->fn_type({
                        world->mem_type()
                        })
                }));

    const Def* mem = y->param(0);

    if (data->isa<Extract>()) {
        mem = world->release(mem, data);
    }

    y->jump(y->param(1), {mem});

    return y;
}

}
