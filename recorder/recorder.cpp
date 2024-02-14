#include <thorin/world.h>

using namespace thorin;

extern "C" {

//#[import(cc = "plugin", name = "wrap_token")] fn wrap_token(_token: fn () -> i32, _expr: fn (i32) -> i32) -> ();
const Def* wrap_token(World* world, App* app) {
    assert(app->num_args() == 4);

    const Def* token_def = app->arg(1);
    const Def* apply_def = app->arg(2);

    const Param * token = token_def->as<Param>();
    const Continuation * apply = apply_def->as<Continuation>();

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
    Continuation * new_lam = world->continuation(world->fn_type({world->mem_type(), world->fn_type({world->mem_type(), world->type_qs32()})}));
    Continuation * inter = world->continuation(world->fn_type({world->mem_type(), world->type_qs32()}));
    new_lam->jump(original_lam, {new_lam->param(0), inter});
    inter->jump(apply, {inter->param(0), inter->param(1), new_lam->param(1)});

    generate_token_call->unset_op(2);
    generate_token_call->set_op(2, new_lam);

    return nullptr;
}

//#[import(cc = "plugin", name = "generate_token", depends = wrap_token)] fn generate_token(_expr: fn () -> i32) -> fn () -> i32;
const Def* generate_token(World* world, App* app) {
    //Plain passthrough. Simply used to guard the contained lambda.
    assert(app->num_args() == 3);

    return app->arg(1);
}

}
