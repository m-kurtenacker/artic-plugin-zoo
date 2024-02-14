#include <iostream>

#include <thorin/world.h>
#include <thorin/analyses/cfg.h>
#include <thorin/analyses/scope.h>

#include <thorin/transform/partial_evaluation.h>

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

extern "C" {

static double num_replacements;

void init(void) {
    std::cout << "Hallo Welt!\n";

    num_replacements = 0;
}

const Def* test(World* world, App* app) {
    std::cout << "Plugin test executed!\n";
    assert(app->num_args() == 3);
    
    Continuation * test = const_cast<Continuation*>(app->arg(1)->as<Continuation>());
    replace_numbers(test, 42);

    return test;
}

const Def* test_b(World* world, App* app) {
    std::cout << "Plugin test_b executed!\n";

    assert(app->num_args() == 4);

    Continuation * test = const_cast<Continuation*>(app->arg(1)->as<Continuation>());
    const PrimLit * lit = app->arg(2)->as<PrimLit>();
    double value = lit->value().get_f64();
    replace_numbers(test, value);

    return test;
}

const Def* test_c(World* world, App* app) {
    std::cout << "Plugin test_c executed!\n";
    assert(app->num_args() == 3);

    Continuation * test = const_cast<Continuation*>(app->arg(1)->as<Continuation>());
    replace_numbers(test, num_replacements);
    num_replacements += 1;

    return test;
}

const Def* test_d(World* world, App* app) {
    std::cout << "Plugin test_d executed!\n";

    assert(app->num_args() == 3);

    const Def* test = app->arg(1);

    test = world->arithop(ArithOp_add, test, world->literal_qf64(1.0, {}));

    Continuation *a = world->continuation(
            world->fn_type({
                world->mem_type(),
                world->fn_type({world->mem_type(), world->type_qf64()})
            }));
    a->jump(a->param(1), {a->param(0), test});

    return a;
}

const Def* test_inner(World* world, App* app) {
    std::cout << "Inner plugin executed!\n";

    assert(app->num_args() == 3);

    const Def* test = app->arg(1);

    std::cerr << "Analyzing ";
    test->dump();
    assert(test->isa<Continuation>());

    Continuation * cont = const_cast<Continuation*>(app->arg(1)->as<Continuation>());
    replace_numbers(cont, 24);

    return cont;
}

const Def* test_outer(World* world, App* app) {
    std::cout << "Outer plugin executed!\n";

    assert(app->num_args() == 3);

    const Def* test = app->arg(1);

    std::cerr << "Analyzing ";
    test->dump();
    assert(test->isa<Continuation>());

    Continuation * cont = const_cast<Continuation*>(app->arg(1)->as<Continuation>());
    replace_numbers(cont, 42);

    return cont;
}

}
