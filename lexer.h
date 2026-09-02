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
	
	Identifier,
	
	string_literal,
	
	ull_Literal,
	ll_Literal,
	integer_literal,
	u_literal,
	l_literal,
	double_literal,
	float_literal,
	long_double_literal,
	
	Long,
	Int,
	Unsigned,
	Struct,
	Enum,
	Union,
	
	Plus,
	PlusEqual,
	Minus,
	MinusEqual,	
	
	SemiColomn

};

void exit_compiler();

int is_digit(int c);
int is_alpha(int c);
int is_xdigit(int c);

void print_line();
void number();
int next_char();
int peek();
void identifier();


extern struct Token Token;
extern struct FileReader FileReader;

#define advance() FileReader.pos++

