#include "lexer.h"
#include "sections.h"

#define MAX_NAME_LEN 512

typedef struct NameContext {

	int len;
	char* buf;

} NameContext;

Type check_keyword() {

	/* may be switched to state of chars & hashing later */

	int longest = sizeof("_STATIC_ASSERT") - 1;
	
	int len = Token.index - 1;

	if (len > longest || len == 1)
		return;

	switch (len) {
	
	case 2:
		if (strcmp(Token.lexeme, "if") == 0)
			Token.type = IF;
		break;
		
	case 3:
		switch (Token.lexeme[0]) {
		case 'i':
			if (strcmp(Token.lexeme, "int") == 0)
				Token.type = INT;
			break;
		};
		break;
	
	case 4:
		switch (Token.lexeme[0]) {
		case 'e':
			if (strcmp(Token.lexeme, "else") == 0)
				Token.type = ELSE;
			else if (strcmp(Token.lexeme, "elif") == 0)
				Token.type = ELIF;
			break;
		case 'c':
			if (strcmp(Token.lexeme, "case") == 0)
				Token.type = CASE;
			break;
		case 'l':
			if (strcmp(Token.lexeme, "line") == 0)
				Token.type = LINE;
			else if (strcmp(Token.lexeme, "long") == 0)
				Token.type = LONG;
			break;
		case 't':
			if (strcmp(Token.lexeme, "true") == 0)
				Token.type = TRUE;
			break;
		case 'b':
			if (strcmp(Token.lexeme, "bool") == 0)
				Token.type = BOOL;
			break;

		case 'a':
			if (strcmp(Token.lexeme, "auto") == 0)
				Token.type = AUTO;
			break;
		};

		break;
	
	case 5:
		if (strcmp(Token.lexeme, "while") == 0)
			Token.type = WHILE;
		else if (strcmp(Token.lexeme, "embed") == 0)
			Token.type = EMBED;
		else if (strcmp(Token.lexeme, "undef") == 0)
			Token.type = UNDEF;
		else if (strcmp(Token.lexeme, "error") == 0)
			Token.type = ERROR;
		else if (strcmp(Token.lexeme, "endif") == 0)
			Token.type = ENDIF;
		else if (strcmp(Token.lexeme, "false") == 0)
			Token.type = FALSE;
		break;
	
	case 7:
		if (strncmp(Token.lexeme, "elifdef", 7) == 0)
			Token.type = ELIFDEF;
		else if (strncmp(Token.lexeme, "include", 7) == 0)
			Token.type = INCLUDE;
		else if (strncmp(Token.lexeme, "warning", 7) == 0)
			Token.type = WARNING;
		else if (strncmp(Token.lexeme, "defined", 7) == 0)
			Token.type = DEFINED;
		else if (strncmp(Token.lexeme, "alignas", 7) == 0)
			Token.type = ALIGNAS;
		else if (strncmp(Token.lexeme, "alignof", 7) == 0)
			Token.type = ALIGNOF;
		else if (strncmp(Token.lexeme, "typedef", 7) == 0)
			Token.type = TYPEDEF;
		else if (strncmp(Token.lexeme, "nullptr", 7) == 0)
			Token.type = NULLPTR;
		break;
	
	case 6:
		if (strncmp(Token.lexeme, "pragma", 6) == 0)
			Token.type = PRAGMA;
		else if (strncmp(Token.lexeme, "extern", 6) == 0)
			Token.type = EXTERN;
		else if (strncmp(Token.lexeme, "static", 6) == 0)
			Token.type = STATIC;
		else if (strncmp(Token.lexeme, "inline", 6) == 0)
			Token.type = INLINE;
		else if (strncmp(Token.lexeme, "define", 6) == 0)
			Token.type = DEFINE;
		break;
	
	case 8:
		if (strncmp(Token.lexeme, "elifndef", 8) == 0)
			Token.type = ELIFNDEF;
		else if (strncmp(Token.lexeme, "register", 8) == 0)
			Token.type = REGISTER;
		else if (strncmp(Token.lexeme, "restrict", 8) == 0)
			Token.type = RESTRICT;
		break;
	
	case 9:
		if (strncmp(Token.lexeme, "constexpr", 9) == 0)
			Token.type = CONSTEXPR;
		break;
	
	case 14:
		if (strncmp(Token.lexeme, "_STATIC_ASSERT", 14) == 0)
			Token.type = _STATIC_ASSERT;
		break;
	
	case 13:
		if (strncmp(Token.lexeme, "STATIC_ASSERT", 13) == 0)
			Token.type = STATIC_ASSERT;
		break;

	default:
		return IDENTIFIER;

	};

}

/* construct code point */
unsigned int check_utf8() {
	
	unsigned int cp;
		
	int c = peek();

	if ((c & 0x80) == 0x00) {      
		cp = c;
	}
	else if ((c & 0xE0) == 0xC0) {
		advance();
		int c2 = peek();
		cp = ((c & 0x1F) << 6) | (c2 & 0x3F);		
	}
	else if ((c & 0xF0) == 0xE0) { 
		advance();
		int c2 = peek();
		advance();
		int c3 = peek();
		cp = ((c & 0x0F) << 12) |
			((c2 & 0x3F) << 6) |
			(c3 & 0x3F);
	}
	else if ((c & 0xF8) == 0xF0) { 
		advance();
		int c2 = peek();
		advance();
		int c3 = peek();
		advance();
		int c4 = peek();
		cp = ((c & 0x07) << 18) |
			((c2 & 0x3F) << 12) |
			((c3 & 0x3F) << 6) |
			(c4 & 0x3F);
	}

	else {
		
		printf(COLOR_ERROR "Invalid character in source code\n" COLOR_RESET);
		print_line();
		// solution for error
		exit_compiler();
	}

	return cp;
	
}

typedef unsigned short uint16_t;

unsigned int encode_utf16_val(unsigned int cp) {
	
	if (cp <= 0xFFFF) {	
		return cp;
	}
	else {
		unsigned int v = cp - 0x10000;
		uint16_t a = (uint16_t)(0xD800 + (v >> 10));
		uint16_t b = (uint16_t)(0xDC00 + (v & 0x3FF));
		unsigned int val = a | b << 16;
		return val;
	}
}


Type name() {
	
	char name_buf[MAX_NAME_LEN];
	NameContext ctx = { 0 };
	ctx.buf = name_buf;

	while (is_identifier_continue()) {
		if (ctx.len >= MAX_NAME_LEN - 1) {
			printf(COLOR_ACADEMIC "Identifier(Name) length can not exceed %d characters\n" COLOR_RESET,MAX_NAME_LEN);

			exit_compiler();
		}
		name_buf[ctx.len++] = peek();
		advance();
	}
		
	name_buf[ctx.len] = '\0';
	
	return check_keyword();

}

// handle utf8 to data segment
void string() {

	Token.index = 0;
	int c = peek();
	while (c != "\"" && Token.index < MAX_STR_LEN) {	
		write_to_data(c);
		Token.index++;
		advance();
		c = peek();	
	}
}
	
void file_str() {
	
	Token.index = 0;
	int c = peek();
	
	while (c != '\n' && c != '"' && Token.index < FILE_LEN) {			
		Token.lexeme[Token.index++] = c;
		advance();
		c = peek();
		if (c == '\\') {
			Token.lexeme[Token.index] = c;
			advance();
			if (peek() == '\n')
				advance();
			else
				Token.index++;
		}
	}
	 
	if (c != '"') {

		exit_compiler();

	}
	
	advance();
	Token.lexeme[Token.index] = '\0';
	Token.type = FILE_STR;

}


int code_point;

void char32_literal() {

	Token.index = 0;
	int c = peek();
	
}

void char32_string() {


}

void char16_literal() {


}

void char16_string() {


}

void wide_string() {
	
	
}


void char_literal() {



}

void wide_char() {



}

void u_literals() {
	
	enum { INITAL , UTF8 };
	int u_state = INITAL; // base case
	
	while (1) {
		switch (u_state) {

		case INITAL:
			switch (peek()) {
			case '"':
				advance();
				char16_string();
				return;
			case '\'':
				advance();
				char16_literal();
				return;
			case '8':
				u_state = UTF8;
				break;
			default:
				Token.lexeme[Token.index++] = peek();
				advance();
				name();
				return;
			};
			break;

		case UTF8:
			switch (peek()) {
			case '"':
				advance();
				Token.type = UTF8_STR;
				string(); // both "" and u8"" are utf8 strings
				return;
			case '\'':
				Token.type = UTF8_LITERAL;
				char_literal(); // both '' and u8'' are utf8 char literals
				return;
			default:
				Token.lexeme[Token.index++] = peek();
				advance();
				name();
				return;
			};
			break;
		};

	} // end while

}

// may be continue as identifier
void str_literals() {

	Token.index = 0;
		
	switch (peek()) {
	
	case 'L':
		Token.lexeme[Token.index++] = peek();
		advance();
		if (peek() == '"') {
			advance();
			wide_string();
		}
		else if (peek() == '\'') {
			advance();
			wide_char();
		}
		else {
			name();
		}
		break;
	
	case 'u':
		Token.lexeme[Token.index++] = peek();
		advance();
		u_literals();
		break;
	
	case 'U':
		Token.lexeme[Token.index++] = peek();
		advance();
		if (peek() == '"') {
			advance();
			char32_string();
		}
		else if (peek() == '\'') {
			advance();
			char32_literal();
		}
		else {
			name();
		}
		break;
	
	default:
		name();
		break;
	
	};
	
}
