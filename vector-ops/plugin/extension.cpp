#include <thorin/world.h>

using namespace thorin;

extern "C" {

//#[import(cc = "plugin", name = "vector_zeroinitializer")] fn vector_zeroinitializer () -> simd[i32 * 4];
const Def* vector_zeroinitializer(World* world, App* app) {
    std::cerr << "zeroinit\n";

    assert(app->num_args() == 2);

    return world->vector({
            world->literal_qs32(0, {}),
            world->literal_qs32(0, {}),
            world->literal_qs32(0, {}),
            world->literal_qs32(0, {})
            });
}

//#[import(cc = "plugin", name = "vector_neq")] fn vector_neq (_n : simd[i32 * 4], _m : simd[i32 * 4]) -> simd[bool * 4];
const Def* vector_neq(World* world, App* app) {
    std::cerr << "NEq\n";

    assert(app->num_args() == 4);

    const Def* n_def = app->arg(1);
    const Def* m_def = app->arg(2);

    return world->cmp(Cmp_ne, n_def, m_def, {});
}

}
