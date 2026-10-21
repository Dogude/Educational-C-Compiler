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

// PE Management
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

void exit_compiler();

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

void pop_include(IncludeContext * c);
void release_include(IncludeContext* c);

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

	unsigned int hash;	
	int pool_offset;
    int type;

} sym;

typedef struct SymbolContext {
    int size;
    int capacity;
    sym * head;
    int state; // parser state for all
} SymbolContext;

int hash_function(SymbolContext * s , const unsigned char *str);
void parser(IncludeContext *inc);
void pragma_directive(SymbolContext* c);
void embed_directive(SymbolContext* c);

enum ParserState {
    START,
    PREPROCESSOR,
    EMBED_DIRECTIVE,
    PRAGMA_DIRECTIVE,
    EXPR_STATE, OPERAND, OPERATOR
};

enum SymbolType {

    SYMBOL_CHAR = 0,
    SYMBOL_SHORT = 128,
    SYMBOL_INT = SYMBOL_SHORT + 128,
    SYMBOL_LONG =  SYMBOL_INT + 128,
    SYMBOL_LONG_LONG = SYMBOL_LONG + 128,
     
    SYMBOL_FLOAT = SYMBOL_LONG_LONG + 128,
    SYMBOL_DOUBLE = SYMBOL_FLOAT + 128,
    
};
// parser end


// lexer
typedef struct Token {	

	union {
		size_t c1;
		double c2;
		float c3;
		int precedence;
		int sym_index; // requires for expr eval stack, parser fills that 
	} info ;

	int type;

} Token;

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

void init_include(IncludeContext *c , const char *main_file);
int peek();

int is_digit(int c);
int is_identifier();
int is_identifier_continue();
// lexer end

int hash_function(SymbolContext * s, const unsigned char *str) {
    unsigned int hash = 5381;
    int c;
    while ((c = *str++)) {
        hash = ((hash << 5) + hash) + c;
    }

    return hash % s->capacity; 
}

void init_include(IncludeContext *c , const char *main_file) {

	IncludeStack *p = malloc(sizeof(IncludeStack) * INCLUDE_INIT);
	if (!p) {
		printf(COLOR_ERROR "Memory Error.\n" COLOR_RESET);
		exit_compiler();
	}

	c->head = p;
	c->size = 0;
	c->capacity = INCLUDE_INIT;

	FILE * file = fopen(main_file, "rb");	
	if(!file){
		printf(COLOR_ERROR "Can not open file : %s\n" COLOR_RESET , main_file);
		exit_compiler();
	}

	fseek(file, 0, SEEK_END);
	long size = ftell(file);

	unsigned char *source = malloc(size);

	if (!source) {
		printf(COLOR_ERROR "Memory Error.\n" COLOR_RESET);
		exit_compiler();
	}

	fread(source,1,size,file);
	fclose(file);

	c->head->source = source;
	c->head->fpos = 0;
	c->head->last_line = 0;
	c->head->line = 0;
	c->head->len = size;

}

void push_include(IncludeContext* c , const char * file) {
	
	FILE *f = fopen(file, "rb");
	if(!f){
		
		exit_compiler();
	}

	fseek(f, 0, SEEK_END);
	long size = ftell(f);	
	unsigned char *source = malloc(size);
	
	if(!source) {

		exit_compiler();
	}

	fread(source,1,size,f);
	fclose(f);

	if (c->size < c->capacity) {	
		c->size++;
	}
	else {
		size_t new_cap = c->size * INCLUDE_GROW;
		void* ptr = realloc(c->head, new_cap * sizeof(IncludeStack));
		if (!ptr) {
			printf(COLOR_ERROR "Memory Error.\n" COLOR_RESET);
			exit_compiler();
		}

		c->head = ptr;				
		c->size++;
		c->capacity = new_cap;
	}

	size_t i = c->size;
	c->head[i].source = source;	
	c->head->fpos = 0;
	c->head->last_line = 0;
	c->head->line = 0;
	c->head->len = size;

}

void pop_include(IncludeContext * c) {
	
	free(c->head->source); // release file char array
	c->size--;

}

void release_include(IncludeContext* c) {
	
	free(c->head);

}

void invalid_byte() {

	printf(COLOR_ERROR "Invalid character in source code\n" COLOR_RESET);
	// print_line();
	exit_compiler();

}

int peek(IncludeContext * s) {

	int len = s->head->len;
	int pos = s->head->fpos;
	return pos < len ? s->head->source[pos] : EOF;

}
	
void advance(IncludeContext * s) {

	int c = peek(s);

	if ((c & 0x80) == 0x00) {	
		cp = c;
	}
	else if ((c & 0xE0) == 0xC0) {
		int c2 = _peek();
		if (c2 == EOF)
			invalid_byte();
		cp = ((c & 0x1F) << 6) | (c2 & 0x3F);
	}
	else if ((c & 0xF0) == 0xE0) {
		int c2 = _peek();
		int c3 = _peek();
		if (c2 == EOF || c3 == EOF)
			invalid_byte();
		cp = ((c & 0x0F) << 12) |
			((c2 & 0x3F) << 6) |
			(c3 & 0x3F);
	}
	else if ((c & 0xF8) == 0xF0) {
		int c2 = _peek();
		if (c2 == EOF)
			invalid_byte();
		int c3 = _peek();
		if (c3 == EOF)
			invalid_byte();
		int c4 = _peek();
		if (c4 == EOF)
			invalid_byte();
		cp = ((c & 0x07) << 18) |
			((c2 & 0x3F) << 12) |
			((c3 & 0x3F) << 6) |
			(c4 & 0x3F);
	}
	else {

		invalid_byte();

	}


}

int is_digit(int c) {
	return c >= '0' && c <= '9';
}

int is_identifier() {

	unsigned int cp = peek();
	
	if ((cp >= 'a' && cp <= 'z') || (cp >= 'A' && cp <= 'Z') || cp == '_') {
		return 1;
	}
	
	if (cp < 0x80) return 0;

	if (cp >= 0x2600 && cp <= 0x27BF) return 0; // Genel Semboller, Dingbats (etc, heart U+2665)
	if (cp >= 0x1F000 && cp <= 0x1FFFF) return 0; // Emojiler (Gülen yüzler vb.)
	if (cp >= 0xD800 && cp <= 0xDFFF) return 0; // Surrogate pairs
	if (cp >= 0xFE00 && cp <= 0xFE0F) return 0; // Varyasyon Seçiciler (Kalbin 2. parçası U+FE0F buraya takılır)

	// 4. Geriye kalan üst aralıklar genelde uluslararası harflerdir (Çince, Kiril vb.)
	if (cp <= 0x10FFFF) {
		return 1;
	}

	return 0;
}

int is_identifier_continue() {
	int c = peek();
	if (c >= '0' && c <= '9') return 1;
	return is_identifier();
}

int is_xdigit(int c) {
	return (c >= 'a' && c <= 'f') ||
		(c >= 'A' && c <= 'F') ||
		(c >= '0' && c <= '9');
}

void check_keyword(IncludeContext * c , Token *t) {

	

}

unsigned int encode_utf16_val(unsigned int cp) {
	
	if (cp <= 0xFFFF) {	
		return cp;
	}
	else {
		unsigned int v = cp - 0x10000;
		uint16_t a = (uint16_t)(0xD800 + (v >> 10));
		uint16_t b = (uint16_t)(0xDC00 + (v & 0x3FF));
		unsigned int val = a | b << 16;
		return val;
	}
}

void name(IncludeContext * c , Token * t) {
		
	while (is_identifier_continue()) {
		if (ctx.len >= MAX_NAME_LEN - 1) {
			printf(COLOR_ACADEMIC "Identifier(Name) length can not exceed %d characters\n" COLOR_RESET,MAX_NAME_LEN);

			exit_compiler();
		}
		name_buf[ctx.len++] = peek();
		advance();
	}
		
	name_buf[ctx.len] = '\0';
	
	check_keyword(t);

}

// handle utf8 to data segment
void string(Token * t) {

	Token.index = 0;
	int c = peek();
	while (c != "\"" && Token.index < MAX_STR_LEN) {	
		write_to_data(c);
		Token.index++;
		advance();
		c = peek();	
	}
}
	
void file_str(Token * t) {
	
	Token.index = 0;
	int c = peek();
	
	while (c != '\n' && c != '"' && Token.index < FILE_LEN) {			
		Token.lexeme[Token.index++] = c;
		advance();
		c = peek();
		if (c == '\\') {
			Token.lexeme[Token.index] = c;
			advance();
			if (peek() == '\n')
				advance();
			else
				Token.index++;
		}
	}
	 
	if (c != '"') {

		exit_compiler();

	}
	
	advance();
	Token.lexeme[Token.index] = '\0';
	Token.type = FILE_STR;

}


void char32_literal(Token * t) {

	Token.index = 0;
	int c = peek();
	
}

void char32_string(Token * t) {


}

void char16_literal(Token * t) {


}

void char16_string(Token * t) {


}

void wide_string(Token * t) {
	
	
}


void char_literal(Token * t) {



}

void wide_char(Token * t) {



}

void u_literals(Token * t) {
	
	enum { INITAL , UTF8 };
	int u_state = INITAL; // base case
	
	while (1) {
		switch (u_state) {

		case INITAL:
			switch (peek()) {
			case '"':
				advance();
				char16_string();
				return;
			case '\'':
				advance();
				char16_literal();
				return;
			case '8':
				u_state = UTF8;
				break;
			default:
				Token.lexeme[Token.index++] = peek();
				advance();
				name();
				return;
			};
			break;

		case UTF8:
			switch (peek()) {
			case '"':
				advance();
				Token.type = UTF8_STR;
				string(); // both "" and u8"" are utf8 strings
				return;
			case '\'':
				Token.type = UTF8_LITERAL;
				char_literal(); // both '' and u8'' are utf8 char literals
				return;
			default:
				Token.lexeme[Token.index++] = peek();
				advance();
				name();
				return;
			};
			break;
		};

	} // end while

}

// may be continue as identifier
void str_literals(Token * t) {

	Token.index = 0;
		
	switch (peek()) {
	
	case 'L':
		Token.lexeme[Token.index++] = peek();
		advance();
		if (peek() == '"') {
			advance();
			wide_string();
		}
		else if (peek() == '\'') {
			advance();
			wide_char();
		}
		else {
			name();
		}
		break;
	
	case 'u':
		Token.lexeme[Token.index++] = peek();
		advance();
		u_literals();
		break;
	
	case 'U':
		Token.lexeme[Token.index++] = peek();
		advance();
		if (peek() == '"') {
			advance();
			char32_string();
		}
		else if (peek() == '\'') {
			advance();
			char32_literal();
		}
		else {
			name();
		}
		break;
	
	default:
		name();
		break;
	
	};
	
}

void check_double(Token * t) {

	errno = 0;
	double dval;
	float fval;

	switch (Token.type) {

	case DOUBLE_LITERAL:
		dval = strtod(Token.lexeme, NULL, 0);
		if (errno == ERANGE) {
			
			exit_compiler();
		}
		// write to data segment
		break;

	case FLOAT_LITERAL:
		fval = strtof(Token.lexeme, NULL, 0);
		if (errno == ERANGE) {

			exit_compiler();
		}
		// write to data segment

		break;

	};
	
}

int fnumber(Token * t) {


}

void check_integer(Token * t) {
	
	errno = 0;
	unsigned long long val = strtoull(Token.lexeme, NULL, 0);
	
	if (errno == ERANGE) {	
		exit_compiler();
	}	

	Token.number_size = 8;

	if (val <= 0xffffffff)
		Token.number_size = 4;
	if (val <= 0xffff)
		Token.number_size = 2;
	if (val <= 0xff)
		Token.number_size = 1;
		
	memcpy(&val,Token.number,Token.number_size);

}

void check_suffix(Token * t) {

	int c = peek();
	
	suffix:
	switch (Token.type) {
	case INTEGER_LITERAL:
		switch (c) {
		case 'l':
		case 'L': // 12L
			advance();
			Token.type = L;			
			break;
		case 'u':
		case 'U': //12u
			advance();
			Token.type = U;
			break;
		};
		break;
	
	case U: // 12uL
		switch (c) {
		case 'L':
		case 'l':
			advance();
			Token.type = UL;
			break;
		};
		break;
	
	case L: // 12lu
		switch (c) {
		case 'U':
		case 'u':
			advance();
			Token.type = UL;
			return;			
		case 'l':
		case 'L': // 12ll
			advance();
			Token.type = LL;
			break;	
		};
		break;

	case LL:
		switch (c) {
		case 'U': // 12llu
		case 'u':
			advance();
			Token.type = ULL;
			return;
		};
		break;

	case UL:
		switch (c) {
		case 'L': // 12ull
		case 'l':
			advance();
			Token.type = ULL;		
			return;
		};
		break;

	};

	c = peek();
	goto suffix;
	
}

void number_binary(Token * t) {

	int c = peek();
	
	while (c == '1' || c == '0') {
		Token.lexeme[Token.index++] = c;
		advance();
		if (peek() == '\'') {
			advance();
			if (c != '1' && c != '0') {
				c = 0x39; // '
				break;
			}
			else {
				Token.lexeme[Token.index++] = c;
				advance();
			}
		}
		c = peek();
	}
	
	if (c == '\'') {

		exit_compiler();
	}

	Token.lexeme[Token.index] = '\0';	
	Token.type = INTEGER;
	check_suffix();
	check_integer();

}

void number_hex(Token * t) {

	int c = peek();

	while (is_xdigit(c)) {
		Token.lexeme[Token.index++] = c;
		advance();
		if (peek() == '\'') {
			advance();
			if (!is_xdigit(peek())) {
				c = 0x39; // '
				break;
			}
			else {				
				Token.lexeme[Token.index++] = c;
				advance();
			}
		}
		c = peek();
	}

	if (c == '\'') {

		exit_compiler();
	}
	
	Token.lexeme[Token.index] = '\0';
	Token.type = INTEGER;
	check_suffix();
	check_integer();
	
}

void number_flat(Token * t) {

	int c = peek();

	while (is_digit(c)) {
		Token.lexeme[Token.index++] = c;
		advance();
		if (peek() == '\'') {
			advance();
			if (!is_digit(peek())) {
				c = 0x39; // '
				break;
			}
			else {
				Token.lexeme[Token.index++] = c;
				advance();
			}
		}
		c = peek();
	}

	if (c == '\'') {

		exit_compiler();
	}

	Token.lexeme[Token.index] = '\0';
	Token.type = INTEGER;
	check_suffix();
	check_integer();
}

void number_after_0(Token * t) {

	int c = peek();
	
	switch (c) {
	case 'x':
	case 'X':
		advance();
		Token.lexeme[Token.index++] = c;
		number_hex();
		break;
	case 'b':
	case 'B':
		advance();
		Token.lexeme[Token.index++] = c;
		number_binary();
		break;
	case '0':
		advance();
		Token.lexeme[Token.index++] = c;
		number_flat();
		break;
	case '.':
		advance();
		Token.lexeme[Token.index++] = c;
		fnumber();
		break;
	default:
		if (is_digit(c)) {
			advance();
			Token.lexeme[Token.index++] = c;
			number_octal();
		}
		break;
	};

}

void number_octal(Token * t) {

	int c = peek();

	while (c >= '0' && c <= '7') {
		Token.lexeme[Token.index++] = c;
		advance();
		if (peek() == '\'') {
			advance();
			if (c < '0' || c > '7') {
				c = 0x39; // '
				break;
			}
			else {
				Token.lexeme[Token.index++] = c;
				advance();
			}
		}
		c = peek();
	}

	if (c == '\'') {

		exit_compiler();
	}

	Token.lexeme[Token.index] = '\0';
	Token.type = INTEGER;
	check_suffix();
	check_integer();
}

void number(Token * t) {

	Token.index = 0;

	int c = peek();

	if (c == '0') {
		advance();
		Token.lexeme[Token.index++] = c;
		number_after_0();
	}

	else {
		advance();
		Token.lexeme[Token.index++] = c;
		number_flat();
	}

}
 
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

// fill token area in parser
void lexer(Token *t) {

	switch (peek()) {
	case ',':
		t->type == COMMA;
		break;
	case '(':
		t->type = OPEN_PAR;
		break;
	case ')':
		t->type = CLOSE_PAR;
		break;
	case '+':
		if (peek() == '+') {
			t->type = PLUS_PLUS;
			t->precedence = PRECEDENCE_PRE_MINUS_MINUS;
		}
		else {
			t->type = PLUS;
			t->precedence = PRECEDENCE_PLUS;					
		}
		break;
	case '-':
		advance();
		if (peek() == '-') {
			advance(); // consume
			t->type = MINUS_MINUS;
		}
		else if (peek() == '=') {
			advance();
			t->type = MINUS_EQU;
		}
		else {
			t->type = MINUS;
		}
		
		break;
	case '%':
		advance();
		if (peek() == '=') {
			advance();
			t->type = MOD_EQU;
		}
		else {
			t->type = MOD;
		}
		break;
	
	case '?':
		advance();
		t->type = QUESTION;
		break;		
	case '\n':
		FileReader.last_line = FileReader.pos;
		FileReader.line++;
		Token.type = NEW_LINE; /* token type for preprocessor parser */
		advance();
		break;	
	case '#':
		advance();
		if (peek() == '#') {
			advance();
			Token.type = CONCAT;
		}		
		Token.type = SHARP;
		break;	
	case '^':
		advance();
		if (peek() == '=') {
			advance(); // consume
			Token.type = XOR_EQU;
		}
		else {
			Token.type = XOR;
		}
		break;	
	case '|':
		advance();
		if (peek() == '=') {
			advance(); // consume
			Token.type = OR_EQU;
		}
		else {
			Token.type = OR;
		}
		break;
	case '&':
		advance();
		if (peek() == '=') {
			advance(); // consume
			Token.type = AND_EQU;
		}
		else {
			Token.type = AND;
		}
		break;	
	case '*':
		advance();
		if (peek() == '=') {
			advance(); // consume
			Token.type = MUL_EQU;
		}
		else {
			Token.type = MUL;
		}
		break;
	case '/':
		advance();
		if (peek() == '=') {
			advance(); // consume
			Token.type = DIV_EQU;
		}
		else {
			Token.type = DIV;
		}
		break;	
	case '>':
		advance();
		if (peek() == '>') {
			advance(); // consume
			if (peek() == '=') {
				advance();
				Token.type = SHIFTR_EQU;
			}
			else {
				Token.type = SHIFTR;
			}
		}
		else if (peek() == '=') {
			advance(); // consume
			Token.type = GT_EQU;
		}
		else {
			Token.type = GT;
		}
		break;
	
	case '.':
		advance();		
		if (peek() != '.') {
			Token.type = DOT;
			break;
		}
		advance();
		if (peek() != '.') {
			
			exit_compiler();			
		}
		advance();
		Token.type = THREE_DOT;
		break;
		
	case '<':
		advance();
		if (peek() == '<') {
			advance(); // consume
			if (peek() == '=') {
				advance();
				Token.type = SHIFTL_EQU;
			}
			else {
				Token.type = SHIFTL;
			}
		}
		else if (peek() == '=') {
			advance(); // consume
			Token.type = LT_EQU;
		}
		else {
			Token.type = LT;
		}
		break;
	
	case '"':
		advance();
		switch (Token.state) {
		case STR_MOD:
			string();
			break;
		case FILE_MOD:
			file_str();
			break;
		};		
		break;

	case '\'':
		advance();
		Token.type = CHAR_LITERAL;
		char_literal();	
		break;
	default:
		if (is_digit(peek())) {
			number();
		}
		else if (is_identifier()) {
			str_literals();
		}
		else {

			exit_compiler();
		}		
		break;
	}
	
	if (peek() == EOF) {
		

	}

}

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

void init_macro(TokenArray* c) {

	struct Token *p = malloc(sizeof(struct Token) * MacroVectorInit);
	
	if (!p) {
		printf(COLOR_ERROR "Memory Error.\n" COLOR_RESET);
		exit_compiler();
	}

	c->head = p;
	c->size = 0;
	c->capacity = MacroVectorInit;

}

void append_macro(TokenArray* c , struct Token * t) {
	
	if (c->size < c->capacity) {	
		unsigned int i = c->size;
		memcpy(&c->head[i], t, sizeof(struct Token));
		c->size++;
	}
	else {
		unsigned int new_cap = c->size * MacroVectorGrow;
		void* ptr = realloc(c->head, new_cap * sizeof(struct Token));
		if (!ptr) {
			printf(COLOR_ERROR "Memory Error.\n" COLOR_RESET);
			exit_compiler();
		}

		c->head = ptr;
		unsigned int i = c->size;
		memcpy(&c->head[i], t, sizeof(struct Token));
		c->size++;
		c->capacity = new_cap;
	}
		
}

void release_macro(TokenArray* c) {
	
	free(c->head);
}

void init_symbol(SymbolContext* c) {

	sym* p = malloc(sizeof(sym) * SymbolVectorInit);
	if (!p) {
		printf(COLOR_ERROR "Memory Error.\n" COLOR_RESET);
		exit_compiler();
	}

	c->head = p;
	c->size = 0;
	c->capacity = SymbolVectorInit;
	c->state = START;
}

void append_symbol(SymbolContext* c , sym * s) {
	
	if (c->size < c->capacity) {	
		size_t i = c->size;
		memcpy(&c->head[i], s, sizeof(sym));
		c->size++;
	}
	else {
		size_t new_cap = c->size * SymbolVectorGrow;
		void* ptr = realloc(c->head, new_cap * sizeof(sym));
		if (!ptr) {
			printf(COLOR_ERROR "Memory Error.\n" COLOR_RESET);
			exit_compiler();
		}

		c->head = ptr;
		size_t i = c->size;
		memcpy(&c->head[i], s, sizeof(sym));
		c->size++;
		c->capacity = new_cap;
	}
		
}

void release_symbols(SymbolContext* c) {
	
	free(c->head);
}


void preprocessor_expr_eval(SymbolContext* c) {
// handle #if #elif expressions	


}


void preprocessor_expr_parser(SymbolContext* c) {
// handle #if #elif expressions	
	

}


// generate x64 opcode
void expr_eval(TokenContext * s){


}

void push(TokenContext * s , Token * t){
	
	if(s->size < s->capacity) {
		s->head[s->size] = *t;
		s->size++;
	}  
	else {	
		struct Token *ptr = NULL;
		unsigned int new_capacity = s->size * 2;

		if(s->size == EXPR_STACK) {
			ptr = malloc(new_capacity * sizeof(struct Token));
			if(!ptr) {

				exit_compiler();
			}		
			memcpy(ptr, s->head, sizeof(Token) * EXPR_STACK);
		
		} else {

			ptr = realloc(s->head, new_capacity * sizeof(Token));
			if(!ptr){

				exit_compiler();
			}	
		
		}

		s->head = ptr;
		s->head[s->size] = *t;
		s->size++;
		s->capacity = new_capacity;
	
	}

}

struct Token * pop(TokenContext * s){


}

void expr_parser(SymbolContext* c) {
// handle C expressions	
	
	c->state = OPERAND ;
	struct Token token ;
	lexer(&token);
	struct Token a[EXPR_STACK]; // default limit (256) on cpu stack
	struct Token b[EXPR_STACK]; // default limit (256) on cpu stack
	TokenContext operator = {a,0,EXPR_STACK};
	TokenContext operand  = {b,0,EXPR_STACK};

	while(1) {

		lexer(&token);

		switch(c->state) {

			case OPERAND:
				switch(token.type){
					case PLUS_PLUS:
						token.type = PRE_PLUS_PLUS;
						token.info.precedence = PRECEDENCE_PRE_PLUS_PLUS;
						push(&operator,&token);
						break;	
					case MINUS:
						token.type = UNARY_MINUS;
						token.info.precedence = PRECEDENCE_UNARY_MINUS;
						push(&operator,&token);
						break;	
					case PLUS:
						token.type = UNARY_PLUS;
						token.info.precedence = PRECEDENCE_UNARY_PLUS;
						push(&operator,&token);
						break;		
					case IDENTIFIER:
						token.info.index = 123;
						
						c->state = OPERAND;
						break;

				}

				break;

			case OPERATOR:
				switch(token.type){
					
				}
			
				break;

		}
    

	}

	// final state of parser	
	// while not stack empty

	if(operator.capacity > EXPR_STACK) {
		free(operator.head);
	}

	if(operand.capacity > EXPR_STACK){
		free(operand.head);
	}

}

void start(SymbolContext * c) {

	switch (Token.type) {
	
	case SHARP:
		c->state = PREPROCESSOR;
		break;

	case TYPEDEF:
		break;

	case NEW_LINE:
		break;

	case STATIC:
		break;

	case INLINE:
		break;
		
	case ENUM:	
		break;
	
	case STRUCT:
		break;
	
	case CONST:
		break;
	case INT:
		break;
	case VOID:
		break;
	case DOUBLE:
		break;
	case FLOAT:
		BREAK;
	default:
		// error
		break;
	}

}

void preprocessor(SymbolContext* c) {

	switch (Token.type) {

	case DEFINE:
		break;
	case ELIFDEF:
		break;
	case ELIFNDEF:
		break;
	case INCLUDE:
		break;
	case EMBED:
		c->state = EMBED_DIRECTIVE;
		break;
	case IFDEF:
		break;
	case IF:
		break;
	case ELSE:
		break;
	case ELIF:
		break;
	case PRAGMA:
		c->state = PRAGMA_DIRECTIVE;
		break;

	}

}

/* a comma seperated unsigned char array for context */
void embed_directive(SymbolContext* c) {

	enum EmbedState {

		SEQUENCE,
		PARAMETER,
		PARAMETER_OPEN,
		PARAMETER_CLOSE,
		FINISH
	};

	int embed_state = SEQUENCE;
	
	FILE* file;

	Token.state = FILE_MOD; // switch to lexer mode to read file path in ""


	while (1) {

		lexer();

		switch (embed_state) {

		case SEQUENCE:
		
			switch (t.type) {

			case FILE_STR:
				embed_state = PARAMETER;
				Token.state = STR_MOD;
				break;
			default:
				// error case
				break;

			}
			break;

		case PARAMETER:
			switch (Token.type) {

			case EMBED_IF_EMPTY:
				break;

			case EMBED_LIMIT:
				break;

			case EMBED_PREFIX:
				break;

			case EMBED_SUFFIX:
				break;

			case NEW_LINE:
				return;

			default:
				// error case
				break;

			}
			break;

		case PARAMETER_OPEN:

			break;

		case PARAMETER_CLOSE:
			switch (Token.type) {
			case CLOSE_PAR:
				embed_state = FINISH;
				break;
			default:
				// error
				break;
			}
			break;

		case FINISH:
			switch (Token.type) {
			case NEW_LINE:
				return;
			default:
				// error
				break;
			}
			break;

		}

	}

}

void pragma_directive(SymbolContext* c) {

	
}

void parser(IncludeContext * inc) {

	SymbolContext parser_context = { 0 };
	init_vector(&parser_context);
	init_symbol();

	switch (parser_context.state) {

	case START:
		start(&parser_context);
		break;

	case PREPROCESSOR:
		preprocessor(&parser_context);
		break;

	case EMBED_DIRECTIVE:
		embed_directive(&parser_context);
		break;

	case PRAGMA_DIRECTIVE:
		pragma_directive(&parser_context);
		break;

	default:
		break;
	}	
}

int main(int argc , char * argv[]) {
		
	if (argc == 2 && strcmp(argv[1], "*.c") == 0) {

		struct Files f = {0};
		// construct_files(&f);
		_start(f.list,f.len);

	}
	else if(argc >= 2) {

		_start(argv,argc - 1);
	}
	else if (argc == 1) {
		printf(COLOR_ACADEMIC "educ %s - Educational C Compiler \n" COLOR_RESET , VER);
		printf(COLOR_PRIMARY "Usage : educ *.c -FLAG OR  educ file1.c file2.c file3.c ... -FLAG \n" COLOR_RESET);
		printf(COLOR_PRIMARY "Usage : -FLAG can be -shared[creates .dll] \n" COLOR_RESET);

	}
	else {
		printf(COLOR_ERROR "Invalid command line arguments\n" COLOR_RESET);
		printf(COLOR_PRIMARY "Usage : educ *.c  OR  educ file1.c file2.c file3.c ...  \n" COLOR_RESET);
	}

}