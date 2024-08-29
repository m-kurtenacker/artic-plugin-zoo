#include <thorin/world.h>
#include <thorin/analyses/cfg.h>
#include <thorin/analyses/scope.h>

#include <thorin/transform/mangle.h>

#include <iostream>

using namespace thorin;

class CacheContinuation : public Continuation {
public:
    CacheContinuation(World& world, const FnType* type, const Attributes& attributes, Debug dbg, const Def* caching_object) : Continuation(world, type, attributes, dbg), caching_object(caching_object) {}
    Continuation* stub(Rewriter& rewriter, const Type* nty) const override {
        auto& nworld = rewriter.dst();
        auto npi = nty->isa<FnType>();

        auto new_caching_object = rewriter.instantiate(caching_object);

        CacheContinuation* ncontinuation = new CacheContinuation(nworld, npi, attributes(), debug(), new_caching_object);
        nworld.data_.defs_.emplace(ncontinuation);

        return ncontinuation;
    }

    const Def* caching_object;
};

extern "C" {

const Def* clear_cache(World* world, const App* app) {
    auto caching_continuation = app->arg(1)->as<CacheContinuation>();
    const Def* caching_object = caching_continuation->caching_object;

    Continuation* clear_cache = world->continuation(world->fn_type(
                {
                world->mem_type(),
                world->ptr_type(world->ptr_type(world->type_qs8())),
                world->fn_type({world->mem_type()})
                }
                ));
    clear_cache->set_name("clear_cache");
    world->make_external(clear_cache);
    clear_cache->attributes().cc = CC::C;

    Continuation* clear_function = world->continuation(world->fn_type({world->mem_type(), world->fn_type({world->mem_type()})}));
    Continuation* empty_return = world->continuation(world->fn_type({world->mem_type()}));

    clear_function->jump(clear_cache, {clear_function->param(0), caching_object, empty_return});

    empty_return->jump(clear_function->param(1), {empty_return->param(0)});

    return clear_function;
}

const Def* cache_compute(World* world, const App* app) {
    Continuation* function = const_cast<Continuation*>(app->arg(1)->as<Continuation>());

    const Def* lifted = lift(Scope(function), function, {function});

    //Allocate a new caching map into a global:
    const Def* caching_object = world->global(
            world->bitcast(
                world->ptr_type(world->type_qs8()),
                world->literal_qs32(0, {})
            )
    );

    CacheContinuation* caching_function = new CacheContinuation(*world, function->type(), Continuation::Attributes(), {}, caching_object);
    world->data_.defs_.emplace(caching_function);
    
    Continuation* return_from_cache = world->continuation(world->fn_type({world->mem_type(), world->type_qs32()}));

    //Get from cache if possible.
    Continuation* get_from_cache = world->continuation(world->fn_type(
                {
                world->mem_type(),
                world->type_qs32(),
                world->ptr_type(world->ptr_type(world->type_qs8())),
                world->fn_type({world->mem_type(), world->type_qs32()})
                }
                ));
    get_from_cache->set_name("get_from_cache");
    world->make_external(get_from_cache);
    get_from_cache->attributes().cc = CC::C;

    Continuation* update_cache = world->continuation(world->fn_type(
                {
                world->mem_type(),
                world->type_qs32(),
                world->type_qs32(),
                world->ptr_type(world->ptr_type(world->type_qs8())),
                world->fn_type({world->mem_type()})
                }
                ));
    update_cache->set_name("update_cache");
    world->make_external(update_cache);
    update_cache->attributes().cc = CC::C;

    Continuation* return_fixed = world->continuation(world->fn_type({world->mem_type()}));
    Continuation* return_dynamic = world->continuation(world->fn_type({world->mem_type()}));
    Continuation* return_capture = world->continuation(world->fn_type({world->mem_type(), world->type_qs32()}));
    Continuation* return_after_caching = world->continuation(world->fn_type({world->mem_type()}));

    auto ret = caching_function->param(2);
    auto key = caching_function->param(1);

    caching_function->jump(get_from_cache, {caching_function->param(0), key, caching_object, return_from_cache});

    const Def* condition = world->cmp(Cmp_eq, return_from_cache->param(1), world->literal_qs32(0, {}));
    return_from_cache->branch(return_from_cache->param(0), condition, return_dynamic, return_fixed);

    { /* return_fixed */
        auto value = return_from_cache->param(1);
        return_fixed->jump(ret, {return_fixed->param(0), value});
    }
    { /* return dynamic */
        return_dynamic->jump(lifted, {return_dynamic->param(0), key, return_capture, caching_function});
        auto value = return_capture->param(1);
        return_capture->jump(update_cache, {return_capture->param(0), key, value, caching_object, return_after_caching});
        return_after_caching->jump(ret, {return_after_caching->param(0), value});
    }

    return caching_function;
}

}
