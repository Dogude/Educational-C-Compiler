#include "common.h"



int is_digit(int c) {
	return c >= '0' && c <= '9';
}

int is_identifier(IncludeContext * s) {

	unsigned int cp = codepoint(s);

	if ((cp >= 'a' && cp <= 'z') || (cp >= 'A' && cp <= 'Z') || cp == '_') {
		return 1;
	}

	if (cp < 0x80) return 0;

	if (cp >= 0x2600 && cp <= 0x27BF) return 0; 
	if (cp >= 0x1F000 && cp <= 0x1FFFF) return 0; 
	if (cp >= 0xD800 && cp <= 0xDFFF) return 0; 
	if (cp >= 0xFE00 && cp <= 0xFE0F) return 0;

	if (cp <= 0x10FFFF) {
		return 1;
	}

	return 0;
}

int is_identifier_continue(IncludeContext * s) {
	
	if (peek(s) >= '0' && peek(s) <= '9') return 1;
	return is_identifier(s);

}

int is_xdigit(int c) {
	return (c >= 'a' && c <= 'f') ||
		(c >= 'A' && c <= 'F') ||
		(c >= '0' && c <= '9');
}

void check_keyword(IncludeContext * c , Token *t) {

	

}

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

void name(IncludeContext * c , Token * t) {
		
	while (is_identifier_continue()) {
		if (ctx.len >= MAX_NAME_LEN - 1) {
			printf(COLOR_ACADEMIC "Identifier(Name) length can not exceed %d characters\n" COLOR_RESET,MAX_NAME_LEN);

			exit_compiler();
		}
		name_buf[ctx.len++] = peek();
		advance();
	}
		
	name_buf[ctx.len] = '\0';
	
	check_keyword(t);

}

// handle utf8 to data segment
void string(IncludeContext * s , Token * t) {

	Token.index = 0;
	int c = peek(s);
	while (c != "\"" && Token.index < MAX_STR_LEN) {	
		write_to_data(c);
		Token.index++;
		advance();
		c = peek();	
	}
}
	
void file_str(Token * t) {
	
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


void char32_literal(Token * t) {

	Token.index = 0;
	int c = peek();
	
}

void char32_string(Token * t) {


}

void char16_literal(Token * t) {


}

void char16_string(Token * t) {


}

void wide_string(Token * t) {
	
	
}


void char_literal(Token * t) {



}

void wide_char(Token * t) {



}

void u_literals(Token * t) {
	
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
void str_literals(Token * t) {

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