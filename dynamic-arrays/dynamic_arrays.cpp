#include <thorin/world.h>

using namespace thorin;

extern "C" {

void * build_dynamic_array (World* world, App* app) {
    if (app->num_args() != 3)
        throw std::runtime_error("Wrong number of arguments for call to build_dynamic_array.");

    const Def* size = app->arg(1);
    const Def* return_fn = app->arg(2);

    Continuation* y = world->continuation(return_fn->type()->op(1)->as<FnType>());

    //Traverse the return type to figure out what the array element type is supposed to be.
    const Type* element_type = y->type()->op(1)->op(1)->op(0)->op(0)->as<Type>();

    auto size_lit = size->isa<PrimLit>();
    if (!size_lit)
        throw std::runtime_error("Dynamic arrays need to have a known size at this point.");

    s64 array_size = size_lit->value().get_s64();

    Array<const Def*> elems(array_size);

    auto array_type = world->definite_array_type(element_type, array_size);
    auto array_return_type = world->ptr_type(world->indefinite_array_type(element_type));

    const Def* mem = y->param(0);
    const Def* inner_return_fn = y->param(1);

    auto pair = world->enter(mem);
    mem = world->extract(pair, thorin::u32(0));
    auto frame = world->extract(pair, thorin::u32(1));

    auto slot = world->slot(array_type, frame);
    auto array = world->bitcast(array_return_type, slot);

    y->jump(inner_return_fn, {mem, array});

    return y;
}

}
