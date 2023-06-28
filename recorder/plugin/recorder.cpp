#include <thorin/world.h>

using namespace thorin;

void * generate_token_cpp (void * somedef1) {
    //Plain passthrough. Simply used to guard the contained lambda.
    return somedef1;
}

void * wrap_token_cpp (void * somedef1, void * somedef2) {
    Def * token_def = (Def*) somedef1;
    Def * apply_def = (Def*) somedef2;

    World& world = token_def->world();

    Param * token = token_def->as<Param>();
    Continuation * apply = apply_def->as<Continuation>();

    //Find the original call to generate_token for the token that has been given.
    Continuation * return_continuation = token->continuation();
    App * generate_token_call = nullptr;
    for (auto use : return_continuation->uses()) {
        if (use->isa<App>()) {
            assert(!generate_token_call && "Multiple uses of the same lambda are not supported here.");
            generate_token_call = const_cast<App*>(use->as<App>());
        }
    }
    assert(generate_token_call);

    //Build a new lambda that passes the result of the original token to the new wrapper.
    auto * original_lam = generate_token_call->arg(1);
    Continuation * new_lam = world.continuation(world.fn_type({world.mem_type(), world.fn_type({world.mem_type(), world.type_qs32()})}));
    Continuation * inter = world.continuation(world.fn_type({world.mem_type(), world.type_qs32()}));
    new_lam->jump(original_lam, {new_lam->param(0), inter});
    inter->jump(apply, {inter->param(0), inter->param(1), new_lam->param(1)});

    generate_token_call->unset_op(2);
    generate_token_call->set_op(2, new_lam);

    return nullptr;
}


extern "C" {

//#[import(cc = "plugin", name = "wrap_token")] fn wrap_token(_token: fn () -> i32, _expr: fn (i32) -> i32) -> ();
void * wrap_token(size_t input_c, void ** input_v) {
    assert(input_c == 2);

    return wrap_token_cpp(input_v[0], input_v[1]);
}

//#[import(cc = "plugin", name = "generate_token", depends = wrap_token)] fn generate_token(_expr: fn () -> i32) -> fn () -> i32;
void * generate_token(size_t input_c, void ** input_v) {
    assert(input_c == 1);

    return generate_token_cpp(input_v[0]);
}

}
