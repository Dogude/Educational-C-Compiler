#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#pragma comment(linker, "/STACK:8388608")

#define CHUNK_SIZE 4096 
#define FILE_NAME_LEN 256
#define LEXEME_SIZE 1024

#define MAX_DEPTH (1024*3)
int recursion_depth;

typedef struct {
	unsigned char* data; 
	size_t size;         
	size_t capacity;   
} x64_Opcodes;

enum Type {
	Identifier,
	StringLiteral,
	Ull_Literal,
	Integer_Literal,
	Long,
	Int,
	Unsigned,
	Struct,
	Enum,
	Union,
	Plus,
	PlusEqual,
	SemiColomn
};

struct FileReader {
	FILE* file;
	unsigned char buffer[CHUNK_SIZE];
	size_t line;
	size_t last_line;
	size_t pos;         
	size_t size;         
	int eof;
} FileReader;
	
typedef struct IncludeStack {
	FILE *file;
	char *filename;
	size_t pos;
	size_t line;
	size_t last_line;
	struct IncludeStack* prev;
} IncludeStack;

void push_file(IncludeStack** top, char* file_name, struct FileReader* fr) {
			
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
	fr->file = node->file;
	node->filename = fname;
	node->prev = NULL;
	
	if (*top == NULL) {		
		*top = node;
	} else {
		(*top)->pos = fr->pos;
		(*top)->line = fr->line;
		(*top)->last_line = fr->last_line;
		node->prev = *top;
		*top = node;
	}
	
	fr->pos = 0;
	fr->line = 1;
	fr->last_line = 0;

}

void pop_file(IncludeStack** top) {


}

struct Token {
	enum Type type;
	size_t line;
	char *lexeme;	
	union {
		char c_literal;
		int integral;
		unsigned int u_literal;
		long long long_integral;
		unsigned long long u_long_literal;
		float single_precison;
		double double_precision;
	} numbers;	
} Token;

typedef enum {
	SYM_VARIABLE,
	SYM_TYPEDEF,
	SYM_FUNCTION,
	SYM_STRUCT,
	SYM_ENUM,
	SYM_LABEL	
} SymbolKind;

struct Symbol {
    char* name;              // Ortak alan (herkes için)
	SymbolKind kind;         // Hangi tip olduğunu belirten TAG
    int type;                // C tipi (TYPE_INT, TYPE_FLOAT vs.)
    int offset;              // Stack/global offset'i
	
    union {                  // Her tipe özel alanlar
        struct {
            int array_size;  // Dizi boyutu
        } variable;

        struct {
            int original_type;   // typedef int my_int; -> original_type = TYPE_INT
            int pointer_depth;   // typedef int* my_ptr; -> pointer_depth = 1
        } typedef_info;

        struct {
            int param_count;
            int return_type;
            struct Symbol** params;  // Parametre sembolleri
        } function;

        struct {
            int size;             // Struct'ın toplam boyutu
            int field_count;
            struct Symbol** fields;  // Alanlar
        } struct_info;

        struct {
            int enum_value;       // enum sabitinin değeri
        } enum_info;
    } fields;

} ParserState ;

struct Scope {
	struct Symbol** table; 
	struct Scope* parent; 
};

struct Scope* current_scope;

int next_char(struct FileReader* fr) {
	if (fr->pos >= fr->size) {
		fr->size = fread(fr->buffer, 1, CHUNK_SIZE, fr->file);
		fr->pos = 0;
		if (fr->size == 0) {
			fr->eof = 1;
			return EOF;
		}
	}
	return fr->buffer[fr->pos++];
}

int next_peek(struct FileReader* fr) {
	
	if (fr->pos < fr->size ) {
		return fr->buffer[fr->pos];		
	}
	else {
		fr->size = fread(fr->buffer, 1, CHUNK_SIZE, fr->file);
		fr->pos = 0;
		if (fr->size == 0) {
			fr->eof = 1;
			return EOF;
		}
		return fr->buffer[fr->pos];
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

void print_line() {
	

}

void number(int c) {
	
	int next = next_peek(&FileReader);

	if (c == '0' && (next == 'b' || next == 'B')) {
		

	}
	else if (c == '0' && (next == 'x' || next == 'X')) {
		


	}
	else if (c == '0' && is_digit(next)) {
		
			

	} 


}

void wide_string() {

	next_char(&FileReader); /* skip " */
	
	

}

void string() {



}

void identifier(int c) {
	
	if (c == 'L' && next_peek(&FileReader) == '"') {
	
		wide_string();
		return;
	}
	
}

void lexer() {

	int c = next_char(&FileReader);
	if (c == EOF)return;
	if (c > 127) {
			
		exit_compiler();
	}
		
	switch (c) {
	
	case '+':
				
	case '-':

	case '%':
	
	case '?':
	
	case '^':
	
	case '"':
		string();
		break;
	case '\r':
		break;
		
	default:
		if (is_digit(c)) {

			Token.lexeme[0] = c;
			number(c);

		}

		else if (is_alpha(c)) {

			Token.lexeme[0] = c;
			identifier(c);

		}

		break;
	}


	FileReader.last_line = FileReader.pos - 1;
	FileReader.line++;
	
}

void exit_compiler() {



	
	
	free(Token.lexeme);
	exit(1);
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
	push_file(&top, source, &FileReader);
		
	while (!FileReader.eof) {				
		
		lexer();		
		
	}	
	
}

int main(int argc , char *argv[]) {
	
	Token.lexeme = malloc(LEXEME_SIZE);
	
	lexer_start(argv[1]);


}
