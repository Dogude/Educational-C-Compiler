#include "lexer.h"

struct Token Token;
struct FileReader FileReader;
IncludeStack* top = NULL;



/* may be switched to mmap */
void push_file(IncludeStack** top, char* file_name) {
	
	long long index = _ftelli64(FileReader.file);
	fclose(FileReader.file); // close previous file
	FILE *file = fopen(file_name, "rb");

	if (!file) {
		printf(COLOR_ERROR "Include File Not Found : %s\n" COLOR_RESET, file_name);
		exit_compiler();
	}

	FileReader.file = file;
		
	IncludeStack *node = malloc(sizeof(IncludeStack));
	
	if (!node) {
		printf("Not Enough Memory for Includes\n");
		exit_compiler();
	}

	memcpy(node->filename, file_name, Token.index);
	
	if (*top == NULL) {	
		node->prev = NULL;
		*top = node;
	} else {
		node->fpos = index;
		node->line = FileReader.line;
		node->last_line = FileReader.last_line;		
		node->prev = *top;
		*top = node;
	}
	
	FileReader.line = 1;
	FileReader.last_line = 0;
	FileReader.size = 0;
}

void pop_file(IncludeStack** top) {

	IncludeStack* temp = *top;
	*top = temp->prev;
	if (*top) {
		long long index = (*top)->fpos;
		FileReader.file = fopen((*top)->filename, "rb"); // open previous file
		FileReader.last_line = (*top)->last_line;
		FileReader.line = (*top)->line;	
		_fseeki64(FileReader.file, index, 0);
		FileReader.size = 0;
	}

	free(temp);
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

int is_identifier(int c) {
	return (c >= 'a' && c <= 'z') ||
		(c >= 'A' && c <= 'Z') ||
		c == '_';
}

int is_xdigit(int c) {
	return (c >= 'a' && c <= 'f') ||
		(c >= 'A' && c <= 'F') ||
		(c >= '0' && c <= '9');
}

// '\t' '\r' ' '
void skip_SRT() {

	while (peek() == ' ' || peek() == '\t' || peek() == '\r') {
		advance();
	}
}

void print_line() {
	
	size_t line = FileReader.last_line;
	size_t index = FileReader.pos - FileReader.last_line;
		
}

void exit_compiler() {
	





	free(DATA.data);
	free(eDATA.data);
	free(TEXT.data);
	free(Token.lexeme);
	free(FileReader.buffer);
	exit(1);
}

void lexer() {

	int c = peek();

	switch (c) {

	case ',':
		advance();
		Token.type == COMMA;
		break;

	case '(':
		advance();
		Token.type = OPEN_PAR;
		break;
	case ')':
		advance();
		Token.type = CLOSE_PAR;
		break;
	case '+':
		advance();
		if (peek() == '+') {
			advance(); // consume
			Token.type = PLUS_PLUS;
		}
		else {
			Token.type = PLUS;				
		}
		break;
	case '-':
		advance();
		if (peek() == '-') {
			advance(); // consume
			Token.type = MINUS_MINUS;
		}
		else if (peek() == '=') {
			advance();
			Token.type = MINUS_EQU;
		}
		else {
			Token.type = MINUS;
		}
		
		break;
	case '%':
		advance();
		if (peek() == '=') {
			advance();
			Token.type = MOD_EQU;
		}
		else {
			Token.type = MOD;
		}
		break;
	
	case '?':
		advance();
		Token.type = QUESTION;
		break;
	
	case '\\':
		advance();
		if (peek() == '\n') {
			advance();
			c = peek();
		}
		else {
			// lexer error
		}
		break;
	
	case '\r':
	case '\t':
	case ' ':
		advance();
		skip_SRT();
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
		if (peek() == '.') {
			advance(); // consume
			if (peek() == '.') {
				advance();
				Token.type = THREE_DOT;
			}
			else {

				exit_compiler();
			}
		} else {
			Token.type = DOT;
		}

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
		if (is_digit(c)) {
			number();
		}
		else if (is_identifier(c)) {
			str_literals();
		}
		else {

			exit_compiler();
		}		
		break;
	}
			
	if (c == EOF) {
			
	}

		
}

// adjust the .exe path and .c path
void _start(const char * source) {

	push_file(&top, source);
	Token.lexeme = malloc(LEXEME_SIZE);
	
	if (!Token.lexeme) {

		exit_compiler();
	}

	FileReader.buffer = malloc(CHUNK_SIZE);
	
	if (!FileReader.buffer) {

		exit_compiler();
	}
	
	alloc_pe();


	parser();	

}

int main(int argc , char *argv[]) {
	
	_start(argv[1]);
	
}#include "lexer.h"

struct Token Token;
struct FileReader FileReader;
IncludeStack* top = NULL;

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

int is_identifier(int c) {
	return (c >= 'a' && c <= 'z') ||
		(c >= 'A' && c <= 'Z') ||
		c == '_';
}

int is_xdigit(int c) {
	return (c >= 'a' && c <= 'f') ||
		(c >= 'A' && c <= 'F') ||
		(c >= '0' && c <= '9');
}

// '\t' '\r' ' '
void skip_SRT() {

	while (peek() == ' ' || peek() == '\t' || peek() == '\r') {
		advance();
	}
}

void print_line() {
	
	size_t line = FileReader.last_line;
	size_t index = FileReader.pos - FileReader.last_line;
		
}

void exit_compiler() {
	





	free(DATA.data);
	free(eDATA.data);
	free(TEXT.data);
	free(Token.lexeme);
	free(FileReader.buffer);
	exit(1);
}

void lexer() {

	int c = peek();

	switch (c) {

	case ',':
		advance();
		Token.type == COMMA;
		break;

	case '(':
		advance();
		Token.type = OPEN_PAR;
		break;
	case ')':
		advance();
		Token.type = CLOSE_PAR;
		break;
	case '+':
		advance();
		if (peek() == '+') {
			advance(); // consume
			Token.type = PLUS_PLUS;
		}
		else {
			Token.type = PLUS;				
		}
		break;
	case '-':
		advance();
		if (peek() == '-') {
			advance(); // consume
			Token.type = MINUS_MINUS;
		}
		else if (peek() == '=') {
			advance();
			Token.type = MINUS_EQU;
		}
		else {
			Token.type = MINUS;
		}
		
		break;
	case '%':
		advance();
		if (peek() == '=') {
			advance();
			Token.type = MOD_EQU;
		}
		else {
			Token.type = MOD;
		}
		break;
	
	case '?':
		advance();
		Token.type = QUESTION;
		break;
	
	case '\\':
		advance();
		if (peek() == '\n') {
			advance();
			c = peek();
		}
		else {
			// lexer error
		}
		break;
	
	case '\r':
	case '\t':
	case ' ':
		advance();
		skip_SRT();
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
		if (peek() == '.') {
			advance(); // consume
			if (peek() == '.') {
				advance();
				Token.type = THREE_DOT;
			}
			else {

				exit_compiler();
			}
		} else {
			Token.type = DOT;
		}

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
		if (is_digit(c)) {
			number();
		}
		else if (is_identifier(c)) {
			str_literals();
		}
		else {

			exit_compiler();
		}		
		break;
	}
			
	if (c == EOF) {
			
	}

		
}

// adjust the .exe path and .c path
void _start(const char * source) {

	push_file(&top, source);
	Token.lexeme = malloc(LEXEME_SIZE);
	
	if (!Token.lexeme) {

		exit_compiler();
	}

	FileReader.buffer = malloc(CHUNK_SIZE);
	
	if (!FileReader.buffer) {

		exit_compiler();
	}
	
	alloc_pe();


	parser();	

}

int main(int argc , char *argv[]) {
	
	_start(argv[1]);
	
}
