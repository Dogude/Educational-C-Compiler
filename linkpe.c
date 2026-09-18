#include "lexer.h"

#define FILE_ALIGN 0x200      // FileAlignment = 512 byte
#define MAX_ALIGN 0x7fffffff // 2  GB


unsigned char dos_header[64] = {
// Offset 0x00000000 to 0x0000003F
0x4D, 0x5A, 0x90, 0x00, 0x03, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00,
0xFF, 0xFF, 0x00, 0x00, 0xB8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0xF0, 0x00, 0x00, 0x00

};

typedef unsigned int UINT;
typedef unsigned short USHORT;
typedef unsigned char UCHAR;
typedef unsigned long long ULONG;


struct PE {
    
    // COFF Header
    UINT Signature;
    USHORT Machine;
    USHORT NumberOfSections;
    UINT TimeDateStamp;
    UINT PointerToSymbolTable;
    UINT NumberOfSymbolTable;
    USHORT SizeOfOptionalHeader;
    USHORT Characteristics;
    
    // Standard COFF Fields
    USHORT Magic;
    UCHAR MajorLinkerVersion;
    UCHAR MinorLinkerVersion;
    UINT SizeOfCode;
    UINT SizeOfInitializedData;
    UINT SizeOfUninitializedData;
    UINT AddressOfEntryPoint;
    UINT BaseOfCode;
    UINT BaseOfData;

    // Windows Specific Fields
    UINT ImageBase;  // switch to 64bit , delete BaseOfData
    UINT SectionAlignment;
    UINT FileAlignment;
    USHORT MajorOperatingSystemVersion;
    USHORT MinorOperatingSystemVersion;
    USHORT MajorImageVersion;
    USHORT MinorImageVersion;
    USHORT MajorSubsystemVersion;
    USHORT MinorSubsystemVersion;
    UINT Win32VersionValue; // zeros filled
    UINT SizeOfImage;
    UINT SizeOfHeaders;
    UINT CheckSum;
    USHORT Subsystem; // GUI or Console
    USHORT DllCharacteristics;
    UINT SizeOfStackReserve;  // switch to 64bit
    UINT SizeOfStackCommit;  // switch to 64bit
    UINT SizeOfHeapReserve;  // switch to 64bit
    UINT SizeOfHeapCommit; // switch to 64bit
    UINT LoaderFlags; // Zeros filled
    UINT NumberOfRvaAndSizes;




} PE;


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
    
    TEXT.data = malloc(FILE_ALIGN);
    if (!TEXT.data) {


        exit_compiler();
    }

    TEXT.cap = FILE_ALIGN;
    

    DATA.data = malloc(FILE_ALIGN);
    if (!DATA.data) {


        exit_compiler();
    }
    
    DATA.cap = FILE_ALIGN;


    eDATA.data = malloc(FILE_ALIGN);
    
    if (!eDATA.data) {


        exit_compiler();
    }

    eDATA.cap = FILE_ALIGN;
    
}

void write_to_data(int val) {

    if (DATA.len < DATA.cap) {

        DATA.data[DATA.len++] = val;

    }
    else {

        unsigned int new_cap = DATA.cap + FILE_ALIGN;
        if (new_cap > MAX_ALIGN) {

            // single section can not exceed 2gb
            exit_compiler();
        }
        
        void* temp = realloc(DATA.data, new_cap);
        if (!temp) {

            exit_compiler();
        }

        DATA.data = temp;
        DATA.cap = new_cap;
        DATA.data[DATA.len++] = val;
    }

}

void write_to_text(int val) {

    if (TEXT.len < TEXT.cap) {

        TEXT.data[TEXT.len++] = val;

    }
    else {

        unsigned int new_cap = TEXT.cap + FILE_ALIGN;
        if (new_cap > MAX_ALIGN) {

            // single section can not exceed 2gb
            exit_compiler();
        }

        void* temp = realloc(TEXT.data, new_cap);
        if (!temp) {

            exit_compiler();
        }

        TEXT.data = temp;
        TEXT.cap = new_cap;
        TEXT.data[TEXT.len++] = val;
    }

}




