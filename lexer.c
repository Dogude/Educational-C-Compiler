#include "common.h"

int is_identifier(IncludeContext * s);
void str_literals(IncludeContext * s , Token * t);
int is_digit(int c);

int peek(IncludeContext * s) {

	int len = s->head->len;
	int pos = s->head->fpos;
	return pos < len ? s->head->source[pos] : EOF;
}

void advance(IncludeContext * s) {

	s->head->fpos++;
}

void init_include(IncludeContext * c , const char * main_file) {

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


// fill token area that in parser
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
		else if (is_identifier(s)) {
			str_literals(s,t);
		}
		else {

			exit_compiler();
		}		
		break;
	}
	
	if (peek(s) == EOF) {
		

	}

}
















