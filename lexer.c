#include "common.h"

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

int peek(IncludeContext * s) {

	int len = s->head->len;
	int pos = s->head->fpos;
	return pos < len ? s->head->source[pos] : EOF;
}
	
unsigned int codepoint(IncludeContext * s) {

	unsigned int cp;
	int c = peek(s); 

	if ((c & 0x80) == 0x00) {	
		cp = c;
	}
	else if ((c & 0xE0) == 0xC0) {		
		int c2 = peek(s);
		advance(s);
		if (c2 == EOF) goto invalid_byte;
		cp = ((c & 0x1F) << 6) | (c2 & 0x3F);
	}	
	else if ((c & 0xF0) == 0xE0) {
		int c2 = peek(s);
		advance(s);
		int c3 = peek(s);
		advance(s);
		if (c2 == EOF || c3 == EOF) goto invalid_byte;
		cp = ((c & 0x0F) << 12) |
			((c2 & 0x3F) << 6) |
			(c3 & 0x3F);
	}
	else if ((c & 0xF8) == 0xF0) {
		int c2 = peek(s);
		advance(s);
		int c3 = peek(s);
		advance(s);	
		int c4 = peek(s);
		advance(s);
		if (c2 == EOF || c3 == EOF || c4 == EOF) goto invalid_byte;
		cp = ((c & 0x07) << 18) |
			((c2 & 0x3F) << 12) |
			((c3 & 0x3F) << 6) |
			(c4 & 0x3F);
	}

	return cp;

	invalid_byte:	
		printf(COLOR_ERROR "Invalid character in source code(%s)\n" COLOR_RESET ,"");
		// print_line();
		exit_compiler();
		
}

void advance(IncludeContext * s) {

	s->head->fpos++;

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
		if(/* overflow check */ 0) {

			exit_compiler();
		}

		int new_cap = c->size * INCLUDE_GROW;
		
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
	c->head[i].fpos = 0;
	c->head[i].last_line = 0;
	c->head[i].line = 0;
	c->head[i].len = size;

}

void pop_include(IncludeContext * c) {
	
	free(c->head->source); // release file char array
	c->size--;

}

void release_include(IncludeContext* c) {
	
	free(c->head);

}


// fill token area in parser
void lexer(IncludeContext * s , Token * t) {

	switch (peek(s)) {
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
	
	if (peek(s) == EOF) {
		

	}

}
















