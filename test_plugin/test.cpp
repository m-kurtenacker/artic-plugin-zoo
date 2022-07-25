#include <stdio.h>

#include "thorin/world.h"
#include "thorin/primop.h"

using namespace thorin;

void * test_cpp (void * input) {
    //Closure * testclosure = (thorin::Closure *)input;
    //return testclosure;
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

