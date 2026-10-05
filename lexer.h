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


typedef struct {
	FILE* file;
	unsigned char *buffer;
	size_t line;
	size_t last_line;
	size_t pos;
	size_t size;
	int eof;
} FileInfo;

typedef struct IncludeStack {
	char filename[FILE_LEN];
	size_t fpos;
	size_t line;
	size_t last_line;	
} IncludeStack;

typedef struct  {

	IncludeStack *head;
	size_t size;
	size_t capacity;
	FileInfo file;

} IncludeContext;

void parser(IncludeContext * inc);

typedef struct Token {	

	union {
		size_t c1;
		double c2;
		float c3;
		int precedence;
		unsigned int index;
	} info ;

	int type;

} Token;


enum LexemeState {

	FILE_MOD,
	STR_MOD	
};

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

void exit_compiler();
int is_digit(int c);
int is_identifier_continue();
int is_xdigit(int c);
void print_line();
int peek();

