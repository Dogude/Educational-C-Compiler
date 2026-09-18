#pragma once
// text segment read exec
struct TEXT {
    unsigned char* data;
    int len;
    int cap;
};

// data segment read write
struct DATA {
    unsigned char* data;
    int len;
    int cap;
};

// edata segment , read
struct eDATA {
    unsigned char* data;
    int len;
    int cap;
};

