#include <stdio.h>

#include <thorin/world.h>
#include <thorin/analyses/cfg.h>
#include <thorin/analyses/scope.h>

using namespace thorin;

void replace_numbers(Def * input, double target);
void replace_numbers(Continuation * input, double target);

void replace_numbers(Def * input, double target) {
    World& world = input->world();

    if (auto cont = input->isa<Continuation>())
        return replace_numbers(cont, target);

    if (auto param = input->isa<Param>())
        return;

    for (size_t i = 0, e = input->num_ops(); i < e; i++) {
        auto op = input->op(i);

        if (auto lit = op->isa<PrimLit>()) {
            if (lit->primtype_tag() == PrimType_qf64) {
                auto new_lit = world.literal_qf64(target, {});
                input->unset_op(i);
                input->set_op(i, new_lit);
            }
        } else {
            replace_numbers(const_cast<Def*>(op), target);
        }
    }
}

void replace_numbers(Continuation * input, double target) {
    World& world = input->world();

    if (!input->has_body())
        return;

    App* app = const_cast<App*>(input->body());

    if (app->callee()->isa<Continuation>())
        replace_numbers(const_cast<Continuation*>(app->callee()->as<Continuation>()), target);
    for (size_t i = 0, e = app->num_args(); i < e; i++) {
        auto arg = app->arg(i);

        if (auto lit = arg->isa<PrimLit>()) {
            if (lit->primtype_tag() == PrimType_qf64) {
                auto new_lit = world.literal_qf64(target, {});
                app->unset_op(i + 1);
                app->set_op(i + 1, new_lit);
            }
        } else {
            replace_numbers(const_cast<Def*>(arg), target);
        }
    }
}

void * test_cpp (void * input) {
    Continuation * test = (Continuation *)input;
    replace_numbers(test, 42);
    return input;
}

void * test_b_cpp (void * input, void * number) {
    Continuation * test = (Continuation *)input;
    PrimLit * lit = (PrimLit*)number;
    double value = lit->value().get_f64();
    replace_numbers(test, value);
    return input;
}

static double num_replacements;

void * test_c_cpp (void * input) {
    Continuation * test = (Continuation *)input;
    replace_numbers(test, num_replacements);
    num_replacements += 1;
    return input;
}

void * test_d_cpp (void * input) {
    const Def* test = (Def*) input;
    World& world = test->world();

    test = world.arithop(ArithOp_add, test, world.literal_qf64(1.0, {}));

    Continuation *a = world.continuation(
            world.fn_type({
                world.mem_type(),
                world.fn_type({world.mem_type(), world.type_qf64()})
            }));
    a->jump(a->param(1), {a->param(0), test});
    return a;
}


extern "C" {

void init(void) {
    fprintf(stdout, "Hallo Welt!\n");
    num_replacements = 0;
}

void * test(size_t input_c, void ** input_v) {
    fprintf(stdout, "Plugin executed!\n");
    assert(input_c == 1);
    return test_cpp(input_v[0]);
}

void * test_b(size_t input_c, void ** input_v) {
    fprintf(stdout, "Plugin executed!\n");
    assert(input_c == 2);
    return test_b_cpp(input_v[0], input_v[1]);
}

void * test_c(size_t input_c, void ** input_v) {
    fprintf(stdout, "Plugin executed!\n");
    assert(input_c == 1);
    return test_c_cpp(input_v[0]);
}

void * test_d(size_t input_c, void ** input_v) {
    fprintf(stdout, "Plugin executed!\n");
    assert(input_c == 1);
    return test_d_cpp(input_v[0]);
}

}
