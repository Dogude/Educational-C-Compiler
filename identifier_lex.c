#include "lexer.h"

void check_keyword() {

	/* may be switched to state of chars later */

	int longest = sizeof("_STATIC_ASSERT") - 1;
	
	int len = Token.index - 1;

	if (len > longest || len == 1)
		return;

	switch (len) {
	
	case 2:
		if (strncmp(Token.lexeme, "if", 2) == 0)
			Token.type = IF;
		break;
		
	case 3:
		if (strncmp(Token.lexeme, "int", 3) == 0)
			Token.type = INT;
		break;

	case 4:
		if (strncmp(Token.lexeme, "else", 4) == 0)
			Token.type = ELSE;
		else if (strncmp(Token.lexeme, "case", 4) == 0)
			Token.type = CASE;
		else if (strncmp(Token.lexeme, "elif", 4) == 0)
			Token.type = ELIF;
		else if (strncmp(Token.lexeme, "line", 4) == 0)
			Token.type = LINE;
		else if (strncmp(Token.lexeme, "true", 4) == 0)
			Token.type = TRUE;
		else if (strncmp(Token.lexeme, "bool", 4) == 0)
			Token.type = BOOL;
		else if (strncmp(Token.lexeme, "auto", 4) == 0)
			Token.type = AUTO;
		else if (strncmp(Token.lexeme, "long", 4) == 0)
			Token.type = LONG;		
		break;
	
	case 5:
		if (strncmp(Token.lexeme, "while", 5) == 0)
			Token.type = WHILE;
		else if (strncmp(Token.lexeme, "embed", 5) == 0)
			Token.type = EMBED;
		else if (strncmp(Token.lexeme, "undef", 5) == 0)
			Token.type = UNDEF;
		else if (strncmp(Token.lexeme, "error", 5) == 0)
			Token.type = ERROR;
		else if (strncmp(Token.lexeme, "endif", 5) == 0)
			Token.type = ENDIF;
		else if (strncmp(Token.lexeme, "false", 5) == 0)
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

	};

}

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
