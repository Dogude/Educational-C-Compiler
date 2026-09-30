#pragma once

// len for VirtualSize

// text segment read exec
struct TEXT {
    unsigned char* data;
    struct TEXT* next;
    int len;
    int cap; 
};

// data segment read write
struct DATA {
    unsigned char* data;
    struct DATA* next;
    int len;
    int cap;
};

// edata segment read
struct eDATA {
    unsigned char* data;
    struct eDATA* next;
    int len;
    int cap; 
};

void write_to_data(int val);
void write_to_text(int val);
void write_to_edata(int val);
void alloc_pe();
void free_pe();

void exit_compiler();
