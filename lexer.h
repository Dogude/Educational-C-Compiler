#pragma once
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>

void number();

void str_literals();
void char_literal();
void string();
void file_str();
unsigned int check_utf8();

void free_pe();
void alloc_pe();

#define CHUNK_SIZE (4096 * 2)
#define LEXEME_SIZE 1024
#define MAX_STR_LEN (1024 * 1024)
#define FILE_LEN 128

#define COLOR_ERROR     "\033[91m"  // red, critical
#define COLOR_SUCCESS   "\033[92m"  // green, approve
#define COLOR_INFO      "\033[94m"  // blue, knowledge
#define COLOR_PRIMARY    "\033[38;5;202m" // Primary, hint
#define COLOR_ACADEMIC  "\033[95m"       // Academic Purple
#define COLOR_RESET     "\033[0m"   

typedef struct Token {	
	
	union {
		size_t integral;
		double float2;
		float float1;
	};

	enum Type type;

} Token;

struct FileReader {
	FILE* file;
	unsigned char *buffer;
	size_t line;
	size_t last_line;
	size_t pos;
	size_t size;
	int eof;
};

typedef struct IncludeStack {
	char filename[FILE_LEN];
	size_t fpos;
	size_t line;
	size_t last_line;	
	struct IncludeStack* prev;
} IncludeStack;

enum LexemeState {

	FILE_MOD,
	STR_MOD	
};

typedef enum Type {

	/* literals */	
	IDENTIFIER,
	STR,
	CHAR32_STR,
	CHAR_LITERAL,
	CHAR32_LITERAL,
	CHAR16_LITERAL,
	UTF8_LITERAL,
	UTF8_STR,
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

	/* Operators */
	PLUS,
	PLUS_EQU,
	MINUS,
	MINUS_EQU,
	ARROW,
	ASSIGN,
	CMP,
	DOT,
	DIV,
	DIV_EQU,
	MUL,
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
	MOD,
	MOD_EQU,
	INVERT,
	NOT,
	LOGICAL_AND,
	LOGICAL_OR,
	NOT_CMP,
	GT,
	LT,
	GT_EQU,
	LT_EQU,
	OPEN_PAR,
	CLOSE_PAR,
	OPEN_BRACKET,
	CLOSET_BRACKET,
	THREE_DOT,
	COMMA,
	QUESTION,
	COLOMN,
	SIZEOF,
	_ALIGNOF,
	ALIGNOF,

	SEMICOLOMN,
	COMMA

} Type;

void exit_compiler();
int is_digit(int c);
int is_identifier_continue();
int is_xdigit(int c);
void print_line();
int peek();

extern struct Token Token;
extern struct FileReader FileReader;

#define advance() FileReader.pos++

