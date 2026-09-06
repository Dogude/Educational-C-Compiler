#pragma once
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>

#define CHUNK_SIZE 4096 
#define FILE_NAME_LEN 256
#define LEXEME_SIZE 1024

struct Token {
	enum Type type;
	size_t line;
	char lexeme[LEXEME_SIZE];
	unsigned char number[8]; 
	int number_size;
	int index; /* write to lexeme */
	int state; /* lexeme state */
};

struct FileReader {
	FILE* file;
	unsigned char buffer[CHUNK_SIZE];
	size_t line;
	size_t last_line;
	size_t pos;
	size_t size;
	int eof;
};

enum Type {

	/* literals */
	IDENTIFIER,
	STR,
	_CHAR,
	CHAR32_T,
	CHAR16_T,
	LSTR,
	ULL,
	LL,
	L,
	U,
	INTEGER,
	_DOUBLE,
	_FLOAT,
	_LONG_DOUBLE,
	
	/* PREPROCESSOR */
	DEFINE,
	IFNDEF,
	IFDEF,
	PRE_IF,
	ELIF,
	PRE_ELSE,
	ENDIF,
	ELIFDEF,
	ELIFNDEF,
	UNDEF,
	EMBED,
	LINE,
	ERROR,
	WARNING,
	PRAGMA,
	SHARP,
	CONCAT,
	DEFINED,

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
	ELSE,
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

	SEMICOLOMN

};

void exit_compiler();

int is_digit(int c);
int is_alpha(int c);
int is_xdigit(int c);

void print_line();
void number();
int peek();
void identifier();
void sharp_lex();
void string();


extern struct Token Token;
extern struct FileReader FileReader;

#define advance() FileReader.pos++

