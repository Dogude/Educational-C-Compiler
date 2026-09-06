#include "lexer.h"

extern void parser();

struct Token Token;
struct FileReader FileReader;

typedef struct IncludeStack {
	FILE* file;
	char* filename;
	size_t pos;
	size_t line;
	size_t last_line;
	struct IncludeStack* prev;
} IncludeStack;

void push_file(IncludeStack** top, char* file_name) {
			
	char* fname = malloc(strlen(file_name) + 1);
	if (!fname) {
		printf("Not Enough Memory for Includes\n");
		exit_compiler();
	}

	memcpy(fname, file_name, strlen(file_name) + 1);
		
	FILE *file = fopen(fname, "rb");

	if (!file) {
		printf("Include File Not Found : %s\n",fname);
		exit_compiler();
	}
	
	IncludeStack *node = malloc(sizeof(IncludeStack));
	
	if (!node) {
		printf("Not Enough Memory for Includes\n");
		exit_compiler();
	}
	
	node->file = file;
	FileReader.file = node->file;
	node->filename = fname;
	node->prev = NULL;
	
	if (*top == NULL) {		
		*top = node;
	} else {
		(*top)->pos = FileReader.pos;
		(*top)->line = FileReader.line;
		(*top)->last_line = FileReader.last_line;
		node->prev = *top;
		*top = node;
	}
	
	FileReader.pos = 0;
	FileReader.line = 1;
	FileReader.last_line = 0;

}

void pop_file(IncludeStack** top) {


}

int next_char() {
	if (FileReader.pos >= FileReader.size) {
		FileReader.size = fread(FileReader.buffer, 1, CHUNK_SIZE, FileReader.file);
		FileReader.pos = 0;
		if (FileReader.size == 0) {
			FileReader.eof = 1;
			return EOF;
		}
	}
	return FileReader.buffer[FileReader.pos++];
}

int peek() {
	
	if (FileReader.pos < FileReader.size ) {
		return FileReader.buffer[FileReader.pos];
	}
	else {
		FileReader.size = fread(FileReader.buffer, 1, CHUNK_SIZE, FileReader.file);
		FileReader.pos = 0;
		if (FileReader.size == 0) {
			FileReader.eof = 1;
			return EOF;
		}
		return FileReader.buffer[FileReader.pos];
	}				
}
	

int is_digit(int c) {
	return c >= '0' && c <= '9';
}

int is_alpha(int c) {
	return (c >= 'a' && c <= 'z') ||
		(c >= 'A' && c <= 'Z') ||
		c == '_';
}

int is_xdigit(int c) {
	return (c >= 'a' && c <= 'f') ||
		(c >= 'A' && c <= 'F') ||
		(c >= '0' && c <= '9');
}

void print_line() {
	
	size_t line = FileReader.last_line;
	size_t index = FileReader.pos - FileReader.last_line;
		
}

void exit_compiler() {


	exit(1);
}

void lexer(IncludeStack *top) {

	int c = peek();

	if (c > 127) {
			
		exit_compiler();
	}
	
	lexer_loop:	
	switch (c) {

	case '+':
		advance();
		if (peek() == '+') {
			advance(); // consume
			Token.type = PLUS_PLUS;
			parser();

		}
		else {
			Token.type = PLUS;
			parser();		
		}

		break;

	case '-':
		advance();
		if (peek() == '-') {
			advance(); // consume
			Token.type = MINUS_MINUS;
			parser();
		}
		else if (peek() == '=') {
			advance();
			Token.type = MINUS_EQU;
			parser();
		}
		else {
			Token.type = MINUS;
			parser();
		}
		
		break;

	case '%':
		advance();
		if (peek() == '=') {
			advance();
			Token.type = MOD_EQU;
			parser();
		}
		else {
			Token.type = MOD;
			parser();
		}

		break;

	case '?':
		advance();
		Token.type = QUESTION;
		parser();
		break;

	case '\r':
	case '\t':
	case ' ':
		advance();
		break;
	
	case '\n':
		FileReader.last_line = FileReader.pos;
		FileReader.line++;
		advance();
		break;
		
	case '#':
		advance();
		if (peek() == '#') {
			advance();
			Token.type = CONCAT;
			parser();
		}		
		Token.type = SHARP;
		parser();
		break;
	
	case '^':
		advance();
		if (peek() == '=') {
			advance(); // consume
			Token.type = XOR_EQU;
			parser();

		}
		else {
			Token.type = XOR;
			parser();

		}
		break;
	
	case '|':
		advance();
		if (peek() == '=') {
			advance(); // consume
			Token.type = OR_EQU;
			parser();

		}
		else {
			Token.type = OR;
			parser();

		}
		break;

	case '&':
		advance();
		if (peek() == '=') {
			advance(); // consume
			Token.type = AND_EQU;
			parser();

		}
		else {
			Token.type = AND;
			parser();

		}
		break;
	
	case '*':
		advance();
		if (peek() == '=') {
			advance(); // consume
			Token.type = MUL_EQU;
			parser();

		}
		else {
			Token.type = MUL;
			parser();

		}
		break;

	case '/':
		advance();
		if (peek() == '=') {
			advance(); // consume
			Token.type = DIV_EQU;
			parser();

		}
		else {
			Token.type = DIV;
			parser();

		}
		break;
	
	case '>':
		advance();
		if (peek() == '>') {
			advance(); // consume
			if (peek() == '=') {
				advance();
				Token.type = SHIFTR_EQU;
				parser();
			}
			else {
				Token.type = SHIFTR;
				parser();
			}
		}
		else if (peek() == '=') {
			advance(); // consume
			Token.type = GT_EQU;
			parser();

		}
		else {
			Token.type = GT;
			parser();

		}
		break;

	case '.':
		advance();
		if (peek() == '.') {
			advance(); // consume
			if (peek() == '.') {
				advance();
				Token.type = THREE_DOT;
				parser();
			}
			else {

				exit_compiler();
			}
		} else {
			Token.type = DOT;
			parser();
		}

		break;

	case '<':
		advance();
		if (peek() == '<') {
			advance(); // consume
			if (peek() == '=') {
				advance();
				Token.type = SHIFTL_EQU;
				parser();
			}
			else {
				Token.type = SHIFTL;
				parser();
			}
		}
		else if (peek() == '=') {
			advance(); // consume
			Token.type = LT_EQU;
			parser();

		}
		else {
			Token.type = LT;
			parser();

		}
		break;


	case '"':
		advance();
		string();
		parser();
		break;

	case '\'':
		advance();

		break;


	default:
		if (is_digit(c)) {
			number();
			parser();
		}
		else if (is_alpha(c)) {
			identifier();
			parser();
		}
		else {

			exit_compiler();
		}
		
		break;
	}
		
	c = peek();
	
	if (c == EOF) {
		

	}
	
	goto lexer_loop;

	
}

// adjust the .exe path and .c path
void lexer_start(const char * source) {
	
	IncludeStack* top = NULL;
	push_file(&top, source);	
	lexer(top);		
		
}

int main(int argc , char *argv[]) {
	
	lexer_start(argv[1]);


}
