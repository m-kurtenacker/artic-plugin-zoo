#include<stdio.h>

int sprintf_i32(char * str, char * format, int value) {
    return sprintf(str, format, value);
}

int sprintf_f32(char * str, char * format, float value) {
    return sprintf(str, format, value);
}
