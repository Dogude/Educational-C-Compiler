#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>

#define VER "0.90.0"

#define COLOR_ERROR     "\033[91m"  // red, critical
#define COLOR_SUCCESS   "\033[92m"  // green, approve
#define COLOR_INFO      "\033[94m"  // blue, knowledge
#define COLOR_PRIMARY    "\033[38;5;202m" // Primary, hint
#define COLOR_ACADEMIC  "\033[95m"       // Academic Purple
#define COLOR_RESET     "\033[0m"   

// allocations
#define TEXT (256*1024)
#define TEXT_GROW 2 // until 8 mb then linear grow(+2 mb)
#define DATA (256*1024)
#define DATA_GROW 2 // until 8mb then linear grow(+2 mb)
#define EAT 512

#define INCLUDE_INIT 32
#define INCLUDE_GROW 2

#define SYMBOL_VECTOR_INIT (8 * 1024)
#define SYMBOL_VECTOR_GROW 2

#define MACRO_VECTOR_INIT 16
#define MACRO_VECTOR_GROW 2

#define IDENTIFIER_POOL_INIT (256 * 1024)
#define IDENTIFIER_POOL_GROW 2

#define EXPR_STACK 256
#define EXPR_STACK_GROW 2
// allocations

enum Precedence {

    // L    
    PRECEDENCE_COMMA,
    // L

    // R
	PRECEDENCE_XOR_EQU = 1,
	PRECEDENCE_OR_EQU = 1,
	PRECEDENCE_AND_EQU = 1,
	PRECEDENCE_SHIFTR_EQU = 1,
    PRECEDENCE_SHIFTL_EQU = 1,
    PRECEDENCE_MUL_EQU = 1,
    PRECEDENCE_DIV_EQU = 1,
    PRECEDENCE_MOD_EQU = 1,
    PRECEDENCE_PLUS_EQU = 1, 
    PRECEDENCE_MINUS_EQU = 1,
    PRECEDENCE_ASSIGN = 1,
    PRECEDENCE_COLOMN = 6,
	PRECEDENCE_QUESTION = 6,
	// R

	// L
	PRECEDENCE_LOGICAL_OR = 7,
	PRECEDENCE_LOGICAL_AND = 8,
	PRECEDENCE_OR = 9,
	PRECEDENCE_XOR = 10,
	PRECEDENCE_AND = 11,
	PRECEDENCE_NOT_CMP = 12,
    PRECEDENCE_CMP = 12,
    PRECEDENCE_GT_EQU = 13,
    PRECEDENCE_GT = 13,
    PRECEDENCE_LT_EQU = 13,
	PRECEDENCE_LT = 13,  
    PRECEDENCE_SHIFTR = 14,	
	PRECEDENCE_SHIFTL = 14,
	PRECEDENCE_MINUS = 15,
    PRECEDENCE_PLUS = 15,
	PRECEDENCE_MOD = 16,
	PRECEDENCE_DIV = 16,
	PRECEDENCE_MUL = 16,	
	// L

	// R
	PRECEDENCE_ALIGNOF = 25,
	PRECEDENCE__ALIGNOF = 25,
	PRECEDENCE_SIZEOF = 25,	
	PRECEDENCE_INVERT = 25,		
	PRECEDENCE_NOT = 25,
	PRECEDENCE_UNARY_PLUS = 25,
	PRECEDENCE_UNARY_MINUS = 25,
	PRECEDENCE_DEREF = 25,
	PRECEDENCE_PRE_MINUS_MINUS = 25,
	PRECEDENCE_PRE_PLUS_PLUS = 25,
	// R
	
	// L
	PRECEDENCE_POST_MINUS_MINUS = 26,
	PRECEDENCE_POST_PLUS_PLUS = 26,
	PRECEDENCE_ARROW = 26,
	PRECEDENCE_DOT = 26,
	PRECEDENCE_OPEN_BRACKET =26 ,
	PRECEDENCE_CLOSET_BRACKET = 26,
	PRECEDENCE_OPEN_PAR = 26,
	PRECEDENCE_CLOSE_PAR = 26,
	// L

};

typedef enum Type {

	/* literals */	
	IDENTIFIER,
	STR,
	UTF8_STR,
	CHAR16_STR,	
	CHAR32_STR,
	CHAR_LITERAL,
	UTF8_LITERAL,	
	CHAR16_LITERAL,	
	CHAR32_LITERAL,
	ULL_LITERAL,
	LL_LITERAL,
	UL_LITERAL,
	L_LITERAL,
	U_LITERAL,
	INTEGER_LITERAL,
	DOUBLE_LITERAL,
	FLOAT_LITERAL,
	LONG_DOUBLE_LITERAL,
	
	/* PREPROCESSOR */
	DEFINE,
	INCLUDE, FILE_STR,
	IFNDEF,
	IFDEF,
	ELIF,
	ENDIF,
	ELIFDEF,
	ELIFNDEF,
	UNDEF,
	EMBED, EMBED_IF_EMPTY, EMBED_SUFFIX, EMBED_PREFIX,EMBED_LIMIT,
	LINE,
	ERROR,
	WARNING,
	PRAGMA,
	SHARP,
	CONCAT,
	DEFINED,
	NEW_LINE,

	/* other */
	EXTERN,
	STATIC,
	INLINE,
	ALIGNAS,
	TYPEDEF,
	CONSTEXPR,
	FALSE,
	NULLPTR,
	TRUE,
	REGISTER,
	RESTRICT,
	STATIC_ASSERT,
	THREAD_LOCAL,
	_STATIC_ASSERT,
	_THREAD_LOCAL,
	TYPEOF,
	TYPEOF_UNQUAL,
	VOLATILE,
	_ATOMIC,
	_ALIGNAS,
	_BOOL,

	/* type words */
	LONG,
	INT,
	AUTO,
	BOOL,
	UNSIGNED,
	STRUCT,
	ENUM,
	UNION,
	DOUBLE,
	FLOAT,	
	CHAR,
	SHORT,
	SIGNED,
	COMPLEX,
	IMAGINARY,
	CONST,
	VOID,
	GENERIC,
	NO_RETURN,
	
	/* instructions */
	WHILE,
	FOR,
	IF,
	ELSE,
	GOTO,
	BREAK,
	CASE,
	CONTINUE,
	DEFAULT,
	DO,
	RETURN,
	SWITCH,

	THREE_DOT,

	SEMICOLOMN,

	/* Operators */	
	PLUS_EQU,
	MINUS_EQU,
	ASSIGN,
	CMP,
	DIV_EQU,
	MUL_EQU,
	AND,
	AND_EQU,
	OR,
	OR_EQU,
	XOR,
	XOR_EQU,
	PLUS_PLUS,
	MINUS_MINUS,
	SHIFTR,
	SHIFTR_EQU,
	SHIFTL,
	SHIFTL_EQU,
	MOD_EQU,
	LOGICAL_AND,
	LOGICAL_OR,
	NOT_CMP,
	GT,
	LT,
	GT_EQU,
	LT_EQU,
	QUESTION,
	COLOMN,
	PLUS,
	MINUS,
	MOD,
	DIV,
	MUL,	
	ALIGNOF,
	_ALIGNOF,
	SIZEOF,	
	INVERT,		
	NOT,
	ARROW,
	DOT,
	OPEN_BRACKET,
	CLOSET_BRACKET,
	OPEN_PAR,
	CLOSE_PAR,

	COMMA,
	
	UNARY_PLUS,
	UNARY_MINUS,
	DEREF,
	PRE_MINUS_MINUS,
	PRE_PLUS_PLUS,
	POS_PLUS_PLUS,
	POST_MINUS_MINUS,
	
} Type;

// PE Management
typedef unsigned int UINT;
typedef unsigned short USHORT;
typedef unsigned char UCHAR;
typedef unsigned long long ULONG;
typedef unsigned short uint16_t;

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
// PE Management

typedef struct IncludeStack {
	unsigned char * source;
	int len;
	int fpos;
	int line; // count new lines
	int last_line;	// hold position of last new line 	
} IncludeStack;

typedef struct IncludeContext {
	IncludeStack *head;
	int size;
	int capacity;

	int str_or_file;

	// symbol name buffer
	unsigned char *identifier_pool;
	int len;
	int cap;

	// pe buffers
	unsigned char* text_data;
    int text_len;
    int text_cap; 

	unsigned char* r_data;
    int r_len;
    int r_cap; 

	unsigned char* e_data;
    int e_len;
    int e_cap; 
	// pe buffers

} IncludeContext;


// parser
typedef struct TokenContext {
	struct Token *head;
	int size;
	int capacity;		
} TokenContext;

typedef struct Symbol {
	
	union {
        TokenContext token ; // for C Macros
        int mem_offset;
        size_t c1;
        double c2;
        float c3;
    } info ; 

	int hash;	
	int pool_offset;
    int type;

} sym;

typedef struct SymbolContext {
    int size;
    int capacity;
    sym * head;
    int state; // parser state for all
} SymbolContext;


// lexer
typedef struct Token {	

	union {
		size_t c1;
		double c2;
		float c3;
		int precedence;
		int mem_offset; // for data segment variables, parser will copy this offset
		int sym_index; // requires for expr eval stack, parser fills that 
	} info ;

	int type;

} Token;

void print_line() {
	
	const char* filename = top->filename;
	size_t pos = top->fpos;
	printf(COLOR_PRIMARY "Error in file : %s , Position : %lld\n" COLOR_RESET, filename,pos);

	size_t line = FileReader.last_line;
	

	size_t index = FileReader.pos - FileReader.last_line;		

}

void exit_compiler() {

	while (top) {
		pop_file(&top);
	}

	free_pe();
	free(Token.lexeme);		
	free(FileReader.buffer);
	exit(1);
}

int hash_function(SymbolContext * s, const unsigned char *str) {
    unsigned int hash = 5381;
    int c;
    while ((c = *str++)) {
        hash = ((hash << 5) + hash) + c;
    }

    return hash % s->capacity; 
}
