#include <thorin/world.h>

using namespace thorin;

std::string const_string_parse(const Def* namedef) {
    const Bitcast * bitcast = namedef->isa<Bitcast>();
    assert(bitcast);

    const Def * bitcast_inner = bitcast->from();
    assert(bitcast_inner);

    const Global * global = bitcast_inner->isa<Global>();
    assert(global && !global->is_mutable());

    const Def * array_def = global->init();
    assert(array_def);

    const DefiniteArray * array = array_def->as<DefiniteArray>();
    assert(array);

    return array->as_string();
}

extern "C" {

//#[import(cc = "plugin", name = "open_file")] fn open_file(_filename: &[u8]) -> u32;
const Def* open_file(World* world, App* app) {
    if (app->num_args() != 3)
        throw std::runtime_error("Wrong number of arguments for call to open_file.");

    const Def* filename_def = app->arg(1);
    std::string filename = const_string_parse(filename_def);

    FILE* f = fopen(filename.c_str(), "w+");

    return world->literal_pu64((long int) f, {});
}

//#[import(cc = "plugin", name = "emit_code", depends = open_file)] fn emit_code(_file: u32, _code : &[u8]) -> u32;
const Def* emit_code(World* world, App* app) {
    if (app->num_args() != 4)
        throw std::runtime_error("Wrong number of arguments for call to emit_code.");

    const Def* fileptr_def = app->arg(1);
    const Def* code_def = app->arg(2);

    auto filename_lit = fileptr_def->isa<PrimLit>();
    if (!filename_lit)
        throw std::runtime_error("Fileptr broken.");

    FILE* f = ((FILE*)filename_lit->value().get_u64());

    if (auto code_lit = code_def->isa<PrimLit>()) {
        s32 lit = code_lit->value().get_s32();
        fprintf(f, "%d", lit);
    } else {
        std::string code = const_string_parse(code_def);
        fprintf(f, "%s", code.c_str());
    }


    return fileptr_def;
}

//#[import(cc = "plugin", name = "close_file")] fn close_file(_file: u32]) -> ();
const Def* close_file(World* world, App* app) {
    if (app->num_args() != 3)
        throw std::runtime_error("Wrong number of arguments for call to close_file.");

    const Def* fileptr_def = app->arg(1);

    auto filename_lit = fileptr_def->isa<PrimLit>();
    if (!filename_lit)
        throw std::runtime_error("Fileptr broken.");

    FILE* f = ((FILE*)filename_lit->value().get_u64());

    fclose(f);

    return nullptr;
}

}
