#include "lexer.h"

void check_keyword() {

	/* may be switched to state of chars later */

	int longest = sizeof("_STATIC_ASSERT") - 1;
	
	int len = Token.index - 1;

	if (len > longest)
		return;

	switch (len) {
	
	case 2:
		if (strcnmp(Token.lexeme, "if", 2) == 0)
			Token.type = IF;
		break;
	
	case 4:
		if (strcnmp(Token.lexeme, "else", 4) == 0)
			Token.type = ELSE;
		else if (strcnmp(Token.lexeme, "case", 4) == 0)
			Token.type = CASE;
		else if (strcnmp(Token.lexeme, "elif", 4) == 0)
			Token.type = ELIF;
		else if (strcnmp(Token.lexeme, "line", 4) == 0)
			Token.type = LINE;
		else if (strcnmp(Token.lexeme, "true", 4) == 0)
			Token.type = TRUE;
		else if (strcnmp(Token.lexeme, "bool", 4) == 0)
			Token.type = BOOL;
		else if (strcnmp(Token.lexeme, "auto", 4) == 0)
			Token.type = AUTO;
		else if (strcnmp(Token.lexeme, "long", 4) == 0)
			Token.type = LONG;		
		break;
	
	case 5:
		if (strcnmp(Token.lexeme, "while", 5) == 0)
			Token.type = WHILE;
		else if (strcnmp(Token.lexeme, "embed", 5) == 0)
			Token.type = EMBED;
		else if (strcnmp(Token.lexeme, "undef", 5) == 0)
			Token.type = UNDEF;
		else if (strcnmp(Token.lexeme, "error", 5) == 0)
			Token.type = ERROR;
		else if (strcnmp(Token.lexeme, "endif", 5) == 0)
			Token.type = ENDIF;
		else if (strcnmp(Token.lexeme, "false", 5) == 0)
			Token.type = FALSE;
		break;
	
	case 7:
		if (strcnmp(Token.lexeme, "elifdef", 7) == 0)
			Token.type = ELIFDEF;
		else if (strcnmp(Token.lexeme, "include", 7) == 0)
			Token.type = INCLUDE;
		else if (strcnmp(Token.lexeme, "warning", 7) == 0)
			Token.type = WARNING;
		else if (strcnmp(Token.lexeme, "defined", 7) == 0)
			Token.type = DEFINED;
		else if (strcnmp(Token.lexeme, "alignas", 7) == 0)
			Token.type = ALIGNAS;
		else if (strcnmp(Token.lexeme, "alignof", 7) == 0)
			Token.type = ALIGNOF;
		else if (strcnmp(Token.lexeme, "typedef", 7) == 0)
			Token.type = TYPEDEF;
		else if (strcnmp(Token.lexeme, "nullptr", 7) == 0)
			Token.type = NULLPTR;
		break;
	
	case 6:
		if (strcnmp(Token.lexeme, "pragma", 6) == 0)
			Token.type = PRAGMA;
		else if (strcnmp(Token.lexeme, "extern", 6) == 0)
			Token.type = EXTERN;
		else if (strcnmp(Token.lexeme, "static", 6) == 0)
			Token.type = STATIC;
		else if (strcnmp(Token.lexeme, "inline", 6) == 0)
			Token.type = INLINE;
		else if (strcnmp(Token.lexeme, "define", 6) == 0)
			Token.type = DEFINE;
		break;
	
	case 8:
		if (strcnmp(Token.lexeme, "elifndef", 8) == 0)
			Token.type = ELIFNDEF;
		else if (strcnmp(Token.lexeme, "register", 8) == 0)
			Token.type = REGISTER;
		else if (strcnmp(Token.lexeme, "restrict", 8) == 0)
			Token.type = RESTRICT;
		break;
	
	case 9:
		if (strcnmp(Token.lexeme, "constexpr", 9) == 0)
			Token.type = CONSTEXPR;
		break;
	
	case 14:
		if (strcnmp(Token.lexeme, "_STATIC_ASSERT", 14) == 0)
			Token.type = _STATIC_ASSERT;
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
