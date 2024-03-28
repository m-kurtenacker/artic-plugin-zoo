#include <thorin/world.h>
#include <thorin/analyses/cfg.h>
#include <thorin/analyses/scope.h>

#include <iostream>

using namespace thorin;


int type_to_size(const Type * t) {
    if (false) {
        return 0;
    } else if (auto st = t->isa<PrimType>()) {
        return 12;
    } else if (auto st = t->isa<StructType>()) {
        int a = 0;
        for (int i = 0; i < st->num_ops(); i++) {
            a += type_to_size(st->op(i)->as<Type>());
        }
        return a;
    }
    THORIN_UNREACHABLE;
}


extern "C" {

//#[import(cc = "plugin", name = "serialize")] fn serialize[T]() -> (fn(T) -> &mut[u8]);
const Def * serialize(World* world, const App* app) {
#if 0
    //#[import(cc = "C", name = "anydsl_alloc")] fn runtime_alloc(_device: i32, _size: i64) -> &mut [i8];
    const MemType* mem_type_mem_t = world->mem_type();
    const PrimType* prim_type_1 = world->type_qs32();
    const PrimType* prim_type_2 = world->type_qs64();
    const PrimType* prim_type_3 = world->type_qs8();
    const IndefiniteArrayType* iarr_type_3_iarr_4 = world->indefinite_array_type(prim_type_3);
    const PtrType* ptr_type_3_iarr_4_p_5 = world->ptr_type(iarr_type_3_iarr_4);
    const FnType* fn_type_6 = world->fn_type({mem_type_mem_t, ptr_type_3_iarr_4_p_5});
    const FnType* fn_type_7 = world->fn_type({mem_type_mem_t, prim_type_1, prim_type_2, fn_type_6});
    Continuation* runtime_alloc = world->continuation(fn_type_7);
    runtime_alloc->set_name("anydsl_alloc");
    world->make_external(runtime_alloc);
    runtime_alloc->attributes().cc = CC::C;

    //fn(T) -> &mut[u8]
    Continuation * input_fn = world->continuation(app->op(2)->type()->op(1)->as<FnType>());
    Continuation * write_cont = world->continuation(fn_type_6);

    std::cerr << "Serializer needs " << type_to_size(input_fn->param(1)->type()) << " bytes of memory.\n";

    input_fn->jump(runtime_alloc, {input_fn->param(0),
                                       world->literal_qs32(0, {}),
                                       world->literal_qs64(sizeof(char) * type_to_size(input_fn->param(1)->type()), {}),
                                       write_cont});

    auto str_pointer = world->bitcast(world->ptr_type(world->indefinite_array_type(world->type_pu8())), write_cont->param(1));
    auto res_mem = world->store(write_cont->param(0), world->lea(str_pointer, world->literal_qs32(0, {}), {}), world->literal_qs8(65, {}));
    res_mem = world->store(res_mem, world->lea(str_pointer, world->literal_qs32(1, {}), {}), world->literal_qs8(0, {}));

    write_cont->jump(input_fn->param(2), {res_mem, str_pointer});

    return input_fn;
#endif

    const Type * S = app->op(2)->type()->op(1)->op(1)->as<Type>();
    std::cout << "Input type ";
    S->dump();

const MemType* mem_type_mem_t = world->mem_type();
const PrimType* prim_type_1 = world->type_qs32();
const PrimType* prim_type_2 = world->type_qs64();
const PrimType* prim_type_3 = world->type_qs8();
const IndefiniteArrayType* iarr_type_3_iarr_4 = world->indefinite_array_type(prim_type_3);
const PtrType* ptr_type_3_iarr_4_p_5 = world->ptr_type(iarr_type_3_iarr_4);
const FnType* fn_type_6 = world->fn_type({mem_type_mem_t, ptr_type_3_iarr_4_p_5});
const FnType* fn_type_7 = world->fn_type({mem_type_mem_t, prim_type_1, prim_type_2, fn_type_6});
const PrimType* prim_type_8 = world->type_pu8();
const PtrType* ptr_type_8_p_9 = world->ptr_type(prim_type_8);
const IndefiniteArrayType* iarr_type_8_iarr_10 = world->indefinite_array_type(prim_type_8);
const PtrType* ptr_type_8_iarr_10_p_11 = world->ptr_type(iarr_type_8_iarr_10);
const FnType* fn_type_12 = world->fn_type({mem_type_mem_t, prim_type_1});
const FnType* fn_type_13 = world->fn_type({mem_type_mem_t, ptr_type_8_p_9, ptr_type_8_iarr_10_p_11, prim_type_1, fn_type_12});
const PrimType* prim_type_14 = world->type_qf32();
const FnType* fn_type_15 = world->fn_type({mem_type_mem_t, ptr_type_8_p_9, ptr_type_8_iarr_10_p_11, prim_type_14, fn_type_12});

//S->set_op(0, prim_type_1);
//S->set_op(1, prim_type_14);

const FnType* fn_type_17 = world->fn_type({mem_type_mem_t, ptr_type_8_iarr_10_p_11});
const FnType* fn_type_18 = world->fn_type({mem_type_mem_t, S, fn_type_17});
const FnType* fn_type_19 = world->fn_type({mem_type_mem_t, ptr_type_3_iarr_4_p_5, fn_type_17});
const PrimType* prim_type_20 = world->type_bool();
const PrimType* prim_type_21 = world->type_qu32();
Continuation* anydsl_alloc = world->continuation(fn_type_7);
anydsl_alloc->set_name("anydsl_alloc");
anydsl_alloc->attributes().cc = CC::C;
world->make_external(anydsl_alloc);
Continuation* sprintf_i32 = world->continuation(fn_type_13);
sprintf_i32->set_name("sprintf_i32");
sprintf_i32->attributes().cc = CC::C;
world->make_external(sprintf_i32);
Continuation* sprintf_f32 = world->continuation(fn_type_15);
sprintf_f32->set_name("sprintf_f32");
sprintf_f32->attributes().cc = CC::C;
world->make_external(sprintf_f32);
Continuation* test = world->continuation(fn_type_18);
test->set_name("test");
test->attributes().cc = CC::C;
world->make_external(test);
Continuation* cont_cont_4 = world->continuation(fn_type_6);
Continuation* cont_cont_5 = world->continuation(fn_type_19);
Continuation* cont_cont_6 = world->continuation(fn_type_17);
Continuation* cont_cont_7 = world->continuation(fn_type_12);
Continuation* cont_cont_8 = world->continuation(fn_type_12);
const Def* const_0 = world->literal(prim_type_1->primtype_tag(), Box(0), {});
const Def* const_1 = world->literal(prim_type_2->primtype_tag(), Box(24), {});
const Def* bitcast_2 = world->bitcast(ptr_type_8_iarr_10_p_11, cont_cont_5->param(1));
const Def* const_3 = world->literal(prim_type_20->primtype_tag(), Box(true), {});
//Here there be filter
cont_cont_5->jump(cont_cont_5->param(2), {cont_cont_5->param(0), bitcast_2});
const Def* enter_6 = world->enter(cont_cont_6->param(0));
const Def* const_7 = world->literal(prim_type_21->primtype_tag(), Box(0), {});
const Def* extract_8 = world->extract(enter_6, const_7);
const Def* const_9 = world->literal(prim_type_21->primtype_tag(), Box(1), {});
const Def* extract_10 = world->extract(enter_6, const_9);
const Def* slot_11 = world->slot(prim_type_1, extract_10);
const Def* store_12 = world->store(extract_8, slot_11, const_0);
const Def* load_13 = world->load(store_12, slot_11);
const Def* extract_14 = world->extract(load_13, const_7);
const Def* load_15 = world->load(extract_14, slot_11);
const Def* extract_16 = world->extract(load_15, const_7);
const Def* extract_17 = world->extract(load_15, const_9);
const Def* lea_18 = world->lea(cont_cont_6->param(1), extract_17, {});
const Def* const_19 = world->literal(prim_type_8->primtype_tag(), Box(37), {});
const Def* const_20 = world->literal(prim_type_8->primtype_tag(), Box(100), {});
const Def* const_21 = world->literal(prim_type_8->primtype_tag(), Box(59), {});
const Def* const_22 = world->literal(prim_type_8->primtype_tag(), Box(0), {});
const Def* def_array_23 = world->definite_array(prim_type_8, {const_19, const_20, const_21, const_22});
const Def* global_24 = world->global(def_array_23, false);
const Def* bitcast_25 = world->bitcast(ptr_type_8_iarr_10_p_11, global_24);
const Def* extract_26 = world->extract(test->param(1), const_7);
const Def* extract_27 = world->extract(load_13, const_9);
const Def* aop_28 = world->arithop(ArithOpTag::ArithOp_add, extract_27, cont_cont_7->param(1));
const Def* store_29 = world->store(cont_cont_7->param(0), slot_11, aop_28);
const Def* load_30 = world->load(store_29, slot_11);
const Def* extract_31 = world->extract(load_30, const_7);
const Def* load_32 = world->load(extract_31, slot_11);
const Def* extract_33 = world->extract(load_32, const_7);
const Def* extract_34 = world->extract(load_32, const_9);
const Def* lea_35 = world->lea(cont_cont_6->param(1), extract_34, {});
const Def* const_36 = world->literal(prim_type_8->primtype_tag(), Box(102), {});
const Def* def_array_37 = world->definite_array(prim_type_8, {const_19, const_36, const_21, const_22});
const Def* global_38 = world->global(def_array_37, false);
const Def* bitcast_39 = world->bitcast(ptr_type_8_iarr_10_p_11, global_38);
const Def* extract_40 = world->extract(test->param(1), const_9);
const Def* extract_41 = world->extract(load_30, const_9);
const Def* aop_42 = world->arithop(ArithOpTag::ArithOp_add, extract_41, cont_cont_8->param(1));
const Def* store_43 = world->store(cont_cont_8->param(0), slot_11, aop_42);
cont_cont_8->jump(test->param(2), {store_43, cont_cont_6->param(1)});
cont_cont_7->jump(sprintf_f32, {extract_33, lea_35, bitcast_39, extract_40, cont_cont_8});
cont_cont_6->jump(sprintf_i32, {extract_16, lea_18, bitcast_25, extract_26, cont_cont_7});
cont_cont_4->jump(cont_cont_5, {cont_cont_4->param(0), cont_cont_4->param(1), cont_cont_6});
test->jump(anydsl_alloc, {test->param(0), const_0, const_1, cont_cont_4});

return test;
}


}
