#include <stdio.h>

#include <thorin/world.h>
#include <thorin/analyses/cfg.h>
#include <thorin/analyses/scope.h>

using namespace thorin;

void replace_numbers(Def * input);
void replace_numbers(Continuation * input);

void replace_numbers(Def * input) {
    World& world = input->world();

    if (auto cont = input->isa<Continuation>())
        return replace_numbers(cont);

    if (auto param = input->isa<Param>())
        return;

    for (size_t i = 0, e = input->num_ops(); i < e; i++) {
        auto op = input->op(i);

        if (auto lit = op->isa<PrimLit>()) {
            if (lit->primtype_tag() == PrimType_qf64) {
                auto new_lit = world.literal_qf64(42, {});
                input->unset_op(i);
                input->set_op(i, new_lit);
            }
        } else {
            replace_numbers(const_cast<Def*>(op));
        }
    }
}

void replace_numbers(Continuation * input) {
    World& world = input->world();

    if (!input->has_body())
        return;

    App* app = const_cast<App*>(input->body());

    replace_numbers(const_cast<Continuation*>(app->callee()->as<Continuation>()));
    for (size_t i = 0, e = app->num_args(); i < e; i++) {
        auto arg = app->arg(i);

        if (auto lit = arg->isa<PrimLit>()) {
            if (lit->primtype_tag() == PrimType_qf64) {
                auto new_lit = world.literal_qf64(42, {});
                app->unset_op(i + 1);
                app->set_op(i + 1, new_lit);
            }
        } else {
            replace_numbers(const_cast<Def*>(arg));
        }
    }
}

void * test_cpp (void * input) {
    Continuation * test = (Continuation *)input;
    replace_numbers(test);
    return input;
}


extern "C" {

void init(void) {
    fprintf(stdout, "Hallo Welt!\n");
}

void * test(void * input) {
    fprintf(stdout, "Plugin executed!\n");
    return test_cpp(input);
}

}

