#include "lexer.h"

#define VER "0.90.0"

extern void parser();
struct Token Token;
struct FileReader FileReader;
IncludeStack* top = NULL;

/* may be switched to mmap */
void push_file(IncludeStack** top, char* file_name) {
	
	if (top) {
		long long index = _ftelli64(FileReader.file);
		fclose(FileReader.file); // close previous file		
	}
	
	FILE* file = fopen(file_name, "rb");

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

int _peek() {
		
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

void invalid_byte() {

	printf(COLOR_ERROR "Invalid character in source code\n" COLOR_RESET);
	print_line();
	exit_compiler();

}

/*  real peek  */
int peek() {

	while (_peek() == '\\') {
		int next = (FileReader.pos + 1) < FileReader.size ? FileReader.buffer[FileReader.pos] : _peek();
		if (next == '\n') {
			FileReader.pos += 2;
		}
		else {
			break;
		}
	}

	int cp = 0;
	int c = _peek(0);
	if ((c & 0x80) == 0x00) {
		cp = c;
	}
	else if ((c & 0xE0) == 0xC0) {
		int c2 = _peek(1);
		if (c2 == EOF)
			invalid_byte();
		cp = ((c & 0x1F) << 6) | (c2 & 0x3F);
	}
	else if ((c & 0xF0) == 0xE0) {
		int c2 = _peek(1);
		int c3 = _peek(2);
		if (c2 == EOF || c3 == EOF)
			invalid_byte();
		cp = ((c & 0x0F) << 12) |
			((c2 & 0x3F) << 6) |
			(c3 & 0x3F);
	}
	else if ((c & 0xF8) == 0xF0) {
		int c2 = _peek(1);
		int c3 = _peek(2);
		int c4 = _peek(3);
		if (c2 == EOF || c3 == EOF || c4 == EOF)
			invalid_byte();
		cp = ((c & 0x07) << 18) |
			((c2 & 0x3F) << 12) |
			((c3 & 0x3F) << 6) |
			(c4 & 0x3F);
	}
	else {

		invalid_byte();

	}
	
	return cp;
}
	
int is_digit(int c) {
	return c >= '0' && c <= '9';
}

int is_identifier() {

	unsigned int cp = check_utf8();

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
	if (peek() >= '0' && peek() <= '9') return 1;
	return is_identifier();
}

int is_xdigit(int c) {
	return (c >= 'a' && c <= 'f') ||
		(c >= 'A' && c <= 'F') ||
		(c >= '0' && c <= '9');
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
void lexer(struct Token* t) {

	while (peek() == ' ' || peek() == '\t' || peek() == '\r') {
		advance();
	}
	
	switch (peek()) {

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

// adjust the .exe path and .c path
void _start(const char ** source, int len) {
	
	Token.lexeme = malloc(LEXEME_SIZE);	
	if (!Token.lexeme) {

		exit_compiler();
	}

	FileReader.buffer = malloc(CHUNK_SIZE);
	
	if (!FileReader.buffer) {

		exit_compiler();
	}
	
	alloc_pe();


	for (int i = 0; i < len; i++) {
		push_file(&top, source[i]);
		parser();
	}
		
}

struct Files {

	char** list;
	int len;

};

void construct_files() {



}

int main(int argc , char * argv[]) {
		
	if (argc == 2 && strcmp(argv[1], "*.c") == 0) {

		struct Files f = {0};
		construct_files(&f);
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
