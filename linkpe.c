#include "lexer.h"

#define FILE_ALIGN 0x200

struct section_bss {
    // name 8 bytes , ".bss\0\0\0\0"
    unsigned int VirtualSize;
    unsigned int VirtualAddress;
    unsigned int SizeOfRawData;
    unsigned int PointerToRawData;
    unsigned int PointerToRelocations;
    unsigned int PointerToLinenumbers;
    unsigned short NumberOfRelocations;
    unsigned short NumberOfLinenumbers;
    unsigned int Characteristics; // 0xC0000080
} section_bss;

struct section_text {
    // name 8 bytes , ".text\0\0\0\0"
    unsigned int VirtualSize;
    unsigned int VirtualAddress;
    unsigned int SizeOfRawData;
    unsigned int PointerToRawData;
    unsigned int PointerToRelocations;
    unsigned int PointerToLinenumbers;
    unsigned short NumberOfRelocations;
    unsigned short NumberOfLinenumbers;
    unsigned int Characteristics; // 
} section_text;

// section
struct section_data {
    // name 8 bytes , ".data\0\0\0"
    unsigned int VirtualSize;
    unsigned int VirtualAddress;
    unsigned int SizeOfRawData;
    unsigned int PointerToRawData;
    unsigned int PointerToRelocations;
    unsigned int PointerToLinenumbers;
    unsigned short NumberOfRelocations;
    unsigned short NumberOfLinenumbers;
    unsigned int Characteristics; // 
} section_data;

struct section_edata {
    // name 8 bytes , ".edata\0\0\0"
    unsigned int VirtualSize;
    unsigned int VirtualAddress;
    unsigned int SizeOfRawData;
    unsigned int PointerToRawData;
    unsigned int PointerToRelocations;
    unsigned int PointerToLinenumbers;
    unsigned short NumberOfRelocations;
    unsigned short NumberOfLinenumbers;
    unsigned int Characteristics; // 
} section_edata;

// cap for SizeOfRawData (alloc multiple of 0x200)
// len for VirtualSize

// text segment read exec
struct TEXT {   
    unsigned char* data;
    int len;
    int cap;
} TEXT;

// data segment read write
struct DATA {  
    unsigned char* data;
    int len;
    int cap;
} DATA;

// edata segment , read
struct eDATA {
    unsigned char* data;
    int len;
    int cap;
} eDATA;


void alloc_pe() {

    DATA.data = malloc(FILE_ALIGN);
    if (!DATA.data) {


        exit_compiler();
    }

    DATA.cap = FILE_ALIGN;
    
}

void write_to_data(int val) {

    if (DATA.len < DATA.cap) {

        DATA.data[DATA.len++] = val;

    }
    else {

        int new_cap = DATA.cap + FILE_ALIGN;

        void* temp = realloc(DATA.data, new_cap);
        if (!temp) {


            exit_compiler();
        }

        DATA.data = temp;
        DATA.cap = new_cap;
        DATA.data[DATA.len++] = val;

    }


}




