#include <thorin/world.h>

using namespace thorin;

class MaskedPointer : public Def {
private:
    //const Def* pointer;
    //const Def* mask;
    const Def* rebuild(World& world, const Type* type, Defs defs) const override {
        return world.cse(new MaskedPointer(world, defs[0], defs[1]));
    }

public:
    //MaskedPointer(World& world, const Def* pointer, const Def* mask) : Def(world, Node_Tuple, {pointer, mask}, pointer->type(), {}), pointer(pointer), mask(mask) {}
    MaskedPointer(World& world, const Def* pointer, const Def* mask)
        : Def(world, Node_Tuple, pointer->type(), {pointer, mask}, {})
        {}

    friend class World;
};

extern "C" {

//#[import(cc = "plugin", name = "masked_ptr")] fn masked_ptr(_ptr : &simd[i32 * 4], _mask : simd[bool * 4]) -> &simd[i32 * 4];
const Def* masked_ptr(World* world, App* app) {
    std::cerr << "Masked Ptr\n";

    assert(app->num_args() == 4);

    const Def* ptr_def = app->arg(1);
    const Def* mask_def = app->arg(2);

    const Def* new_pointer = new MaskedPointer(*world, ptr_def, mask_def);

    return world->cse(new_pointer);
}

//#[import(cc = "plugin", name = "masked_load")] fn masked_load(_ptr : &simd[i32 * 4]) -> simd[i32 * 4];
const Def* masked_load(World* world, App* app) {
    std::cerr << "Masked Load\n";

    assert(app->num_args() == 3);

    const Def* ptr_def = app->arg(1)->as<MaskedPointer>();

    auto llvm_masked_load = world->continuation(world->fn_type({world->mem_type(),
                                                                world->ptr_type(world->type_qs32(4)),
                                                                world->type_qs32(),
                                                                world->type_bool(4),
                                                                world->type_qs32(4),

                                                                world->fn_type({world->mem_type(), world->type_qs32(4)})
                                                            }));
    llvm_masked_load->set_name("llvm.masked.load.v4i32.p0");
    world->make_external(llvm_masked_load);
    llvm_masked_load->attributes().cc = CC::Device;

    auto zero_vector = world->vector({
            world->literal_qs32(0, {}),
            world->literal_qs32(0, {}),
            world->literal_qs32(0, {}),
            world->literal_qs32(0, {})
            });

    auto int_cont = world->continuation(
            world->fn_type({
                            world->mem_type(),
                            world->fn_type({
                                    world->mem_type(),
                                    world->type_qs32(4)
                                    })
                            })
            );
    int_cont->jump(llvm_masked_load, {int_cont->param(0), ptr_def->op(0), world->literal_qs32(4, {}), ptr_def->op(1), zero_vector, int_cont->param(1)});

    return int_cont;
}

}
