#include "sections.h"

#define FILE_ALIGN 0x200      // FileAlignment = 512 byte
#define MAX_ALIGN 0x7fffffff // 2GB

#define TEXT_SIZE 256*1024
#define DATA_SIZE 1024
#define EDATA_SIZE 512


#define COLOR_ERROR     "\033[91m"  // red, critical
#define COLOR_SUCCESS   "\033[92m"  // green, approve
#define COLOR_INFO      "\033[94m"  // blue, knowledge
#define COLOR_PRIMARY    "\033[38;5;202m" // Primary, hint
#define COLOR_ACADEMIC  "\033[95m"       // Academic Purple
#define COLOR_RESET     "\033[0m"   


unsigned char dos_header[64] = {
// PE (50 45 00 00) starts in 64th (0x40)
0x4D, 0x5A, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x40, 0x00, 0x00, 0x00

};

typedef unsigned int UINT;
typedef unsigned short USHORT;
typedef unsigned char UCHAR;
typedef unsigned long long ULONG;

#pragma pack(push, 1)
struct PE {

    // 32 bit Structure of PE 
    UINT Signature;
    
    // COFF Header
    USHORT Machine;
    USHORT NumberOfSections;
    UINT TimeDateStamp;
    UINT PointerToSymbolTable;
    UINT NumberOfSymbolTable;
    USHORT SizeOfOptionalHeader; // 0xF0 in PE+32 , 0xE0 in PE32
    USHORT Characteristics;
    
    // Optional header start
    // Standard COFF Fields
    USHORT Magic; // 0x10B
    UCHAR MajorLinkerVersion;
    UCHAR MinorLinkerVersion;
    UINT SizeOfCode;
    UINT SizeOfInitializedData;
    UINT SizeOfUninitializedData;
    UINT AddressOfEntryPoint; // will be offset of "main" function
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
    UINT SizeOfImage; // Important
    UINT SizeOfHeaders; // Important
    UINT CheckSum;
    USHORT Subsystem; // GUI or Console
    USHORT DllCharacteristics;
    UINT SizeOfStackReserve;  // switch to 64bit
    UINT SizeOfStackCommit;  // switch to 64bit
    UINT SizeOfHeapReserve;  // switch to 64bit
    UINT SizeOfHeapCommit; // switch to 64bit
    UINT LoaderFlags; // Zeros filled
    UINT NumberOfRvaAndSizes; // 0x10 


    // DataDirectories
    
    UINT ExportTable; // RVA    
    UINT SizeOfExportTable;

    UINT ImportTable;
    UINT SizeOfImportTable;

    UINT ResourceTable;
    UINT SizeOfResourceTable;

    UINT ExceptionTable;
    UINT SizeOfExceptionTable;

    UINT CertificateTable;
    UINT SizeOfCertificateTable;

    UINT BaseRelocationTable;
    UINT SizeOfBaseRelocationTable;

    UINT Debug;
    UINT SizeOfDebug;

    UINT ArchitectureData;
    UINT SizeOfArchitectureData;

    UINT GlobalPtr; UINT ZERO1;

    UINT TLSTable;
    UINT SizeOfTLSTable;

    UINT LoadConfigTable;
    UINT SizeOfLoadConfigTable; 

    UINT BoundImport;
    UINT SizeOfBoundImport;

    UINT ImportAddressTable;
    UINT SizeOfImportAddressTable;

    UINT DelayImportDescriptor;
    UINT SizeOfDelayImportDescriptor;

    UINT CLRRuntimeHeader;
    UINT SizeOfCLRRuntimeHeader;

    UINT ZERO2; UINT ZERO3;
    // Optional header end
};
#pragma pack(pop)

struct section {
   // name 8 bytes
    unsigned int VirtualSize;
    unsigned int VirtualAddress;
    unsigned int SizeOfRawData;
    unsigned int PointerToRawData;
    unsigned int PointerToRelocations;
    unsigned int PointerToLinenumbers;
    unsigned short NumberOfRelocations;
    unsigned short NumberOfLinenumbers;
    unsigned int Characteristics;
};

struct SectionBody {

    struct TEXT* root_t;
    struct DATA* root_d;
    struct eDATA* root_e;

} SectionBody;

void free_pe() {



}

void fill_pe() {




    exit_compiler();
}


void memory_error() {

    printf(COLOR_ERROR "Memory error\n" COLOR_RESET);
    exit_compiler();
}


void alloc_pe() {
    
    struct TEXT* text_segment = malloc(sizeof(struct TEXT));
    if (!text_segment) {
        memory_error();
    }
    
    struct DATA* data_segment = malloc(sizeof(struct DATA));
    if (!data_segment) {
        memory_error();
    }

    struct eDATA* edata_segment = malloc(sizeof(struct eDATA));
    if (!edata_segment) {
        memory_error();
    }

    text_segment->data = malloc(TEXT_SIZE);
    if (!text_segment->data) {
        memory_error();
    }
    
    data_segment->data = malloc(DATA_SIZE);
    if (!data_segment->data) {
        memory_error();
    }

    edata_segment->data = malloc(EDATA_SIZE);
    if (!edata_segment->data) {
        memory_error();
    }

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

void write_to_edata(int val) {

    if (eDATA.len < eDATA.cap) {

        eDATA.data[eDATA.len++] = val;

    }
    else {
        unsigned int new_cap = eDATA.cap + FILE_ALIGN;
        if (new_cap > MAX_ALIGN) {

            // single section can not exceed 2gb
            exit_compiler();
        }

        void* temp = realloc(eDATA.data, new_cap);
        if (!temp) {

            exit_compiler();
        }
        eDATA.data = temp;
        eDATA.cap = new_cap;
        eDATA.data[eDATA.len++] = val;
    }

}




