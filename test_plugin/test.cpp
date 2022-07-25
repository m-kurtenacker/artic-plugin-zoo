#include <stdio.h>

#include "thorin/world.h"
#include "thorin/primop.h"

void * test_cpp (void * input) {
    int * testp = (int*)input;
    return testp;
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

