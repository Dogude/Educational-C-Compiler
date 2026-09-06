#include "lexer.h"

enum IdentifierState {




};

void name() {

	int c = peek();
	while (is_alpha(c)) {

		if (Token.index >= LEXEME_SIZE - 1) {

			exit_compiler();
		}

		Token.lexeme[Token.index++] = c;
		advance();
		c = peek();	
	}

}


void c16_string() {



}

void string() {

	

}


void char32_tf() {



}

void c32_string() {



}

void char16_tf() {



}

void wide_string() {
	
	Token.index = 0;

}

void identifier() {

	int c = peek();

	Token.index = 0;

	switch (c) {

	case 'L':
		Token.lexeme[Token.index++] = c;
		advance();
		if (peek() == '"') {
			advance();
			wide_string();
		}
		else if (peek() == '\'') {
			
		}
		else {
			name();
		}
		break;
	
	case 'u':
		Token.lexeme[Token.index++] = c;
		advance();
		if (peek() == '"') {
			advance();
			c16_string();
		}
		else if (peek() == '\'') {
			advance();
			char16_tf();
		}
		else if (peek() == '8') {
			advance();
			if (peek() == '"') {
				advance();

			}
			else if (peek() == '\'') {
				advance();
				
			}
		}
		else {
			name();

		}
		break;
	
	case 'U':
		Token.lexeme[Token.index++] = c;
		advance();
		if (peek() == '"') {
			advance();
			c32_string();
		}
		else if (peek() == '\'') {
			advance();
			char32_tf();

		}
		else {
			name();
		}
		break;

	};
	
}
