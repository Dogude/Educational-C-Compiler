#include <stdio.h>

// text segment
typedef struct {
    unsigned char* data;
    size_t size;
    size_t capacity;
} x64_Opcodes;

// data segment
typedef struct {
    unsigned char* data;
    size_t size;
    size_t capacity;
} data;


