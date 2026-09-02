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
			Token.type = 123;

		}
		else {
			Token.type = 12;

		}

	case '-':
		if (peek() == '-') {
			next_char(); // consume
			Token.type = 123;

		}
		else if (peek() == '=') {
			next_char(); // consume
			Token.type = 123;

		}
		else {
			Token.type = 12;

		}

	case '%':
		if (peek() == '=') {
			next_char(); // consume
			Token.type = 123;

		}
		else {
			Token.type = 12;

		}
	
	case '\n':
		break;

	case '?':

	case '^':
		if (peek() == '=') {
			next_char(); // consume
			Token.type = 123;

		}
		else {
			Token.type = 12;

		}

	case '"':
		string();
		break;
	case '\r':
		break;
	default:
		if (is_digit(c)) {
			number();
			
		}

		else if (is_alpha(c)) {
			identifier();			
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

	FileReader.last_line = FileReader.pos;
	FileReader.line++;
	
}




void include_lexeme(IncludeStack** top, int c) {
	
	static char file_name[FILE_NAME_LEN];
	
	size_t pos = FileReader.pos - 1;
	if (c == '#') {
		int i = 0;
		
		while (c = next_char(&FileReader), c == ' ' || c == '\t');
		
		while (i < FILE_NAME_LEN - 1 && c != ' ' && c != '\t' && c != '"' && c != '<')file_name[i++] = c, c = next_char(&FileReader);
		file_name[i] = '\0';		
		i = 0;
		if (strcmp(file_name, "include") == 0) {

			while (c == ' ' || c == '\t')c = next_char(&FileReader);

			if (c == '"') {

				while (c = next_char(&FileReader), file_name[i++] = c, i < FILE_NAME_LEN - 1 && c != '\n' && c != '"');
				file_name[i-1] = '\0'; // override "
				
				if (c != '"') {
					printf("Include Directive File Name Does Not End With \"\n");
					exit_compiler();
				}
					
				while (c = next_char(&FileReader), c == ' ' || c == '\t');

				if (c != '\n') {
					printf("Include Directive Does Not End With New Line\n");
					exit_compiler();
				}

				FileReader.line++;
				push_file(top, file_name, &FileReader);

				return;

			} else if (c == '<') {

				const char* stab = "include/";
				memcpy(file_name, stab, strlen(stab));

				while (c = next_char(&FileReader), file_name[i++] = c, i < FILE_NAME_LEN - 1 && c != '\n' && c != '>');
				file_name[i - 1] = '\0'; // override >

				if (c != '>') {
					printf("Include Directive File Name Does Not End With >\n");
					exit_compiler();
				}

				while (c = next_char(&FileReader), c == ' ' || c == '\t');

				if (c != '\n') {
					printf("Include Directive Does Not End With New Line\n");
					exit_compiler();
				}

				FileReader.line++;
				push_file(top, file_name, &FileReader);

				return;

			} else {
				printf("Include Directive must continue with \" or < \n");
				exit_compiler();
			}

		}
	
	}
	
	FileReader.pos = pos;

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
