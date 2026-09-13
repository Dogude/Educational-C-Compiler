#include "lexer.h"


void check_double() {

	errno = 0;
	// strtod();
	// strtof();

}


int fnumber() {

	

}

void check_integer() {
	
	errno = 0;
	unsigned long long val = strtoull(Token.lexeme, NULL, 0);
	
	if (errno == ERANGE) {
		
		exit_compiler();
	}



}

void check_suffix() {

	int c = peek();
	
	suffix:
	switch (Token.type) {
	case INTEGER:
		switch (c) {
		case 'l':
		case 'L':
			advance();
			Token.type = L;
			c = peek();
			goto suffix;
			break;
		case 'u':
		case 'U':
			advance();
			Token.type = U;
			c = peek();
			goto suffix;
			break;
		};
		break;
	case U:
		switch (c) {
		case 'L':
		case 'l':
			advance();
			Token.type = UL;
			c = peek();
			goto suffix;
			break;
		};
		break;
	
	case L:
		switch (c) {
		case 'U':
		case 'u':
			advance();
			Token.type = UL;
			return;
			
		case 'l':
		case 'L':
			advance();
			Token.type = LL;
			c = peek();
			goto suffix;
			break;
		
		};
		break;
	case LL:
		switch (c) {
		case 'U':
		case 'u':
			advance();
			Token.type = ULL;
			return;
		};
		break;

	case UL:
		switch (c) {
		case 'L':
		case 'l':
			advance();
			Token.type = ULL;		
			return;
		};
		break;

	};
	
}

void number_binary() {

	int c = peek();
	
	while (c == '1' || c == '0') {
		Token.lexeme[Token.index++] = c;
		advance();
		if (peek() == '\'') {
			advance();
			if (c != '1' && c != '0') {
				c = 0x39; // '
				break;
			}
			else {
				Token.lexeme[Token.index++] = c;
				advance();
			}
		}
		c = peek();
	}
	
	if (c == '\'') {

		exit_compiler();
	}

	Token.lexeme[Token.index] = '\0';
	check_integer();
	Token.type = INTEGER;
	check_suffix();

}

void number_hex() {

	int c = peek();

	while (is_xdigit(c)) {
		Token.lexeme[Token.index++] = c;
		advance();
		if (peek() == '\'') {
			advance();
			if (!is_xdigit(peek())) {
				c = 0x39; // '
				break;
			}
			else {				
				Token.lexeme[Token.index++] = c;
				advance();
			}
		}
		c = peek();
	}

	if (c == '\'') {

		exit_compiler();
	}
	
	Token.lexeme[Token.index] = '\0';
	check_integer();
	Token.type = INTEGER;
	check_suffix();
	
}

void number_flat() {

	int c = peek();

	while (is_digit(c)) {
		Token.lexeme[Token.index++] = c;
		advance();
		if (peek() == '\'') {
			advance();
			if (!is_digit(peek())) {
				c = 0x39; // '
				break;
			}
			else {
				Token.lexeme[Token.index++] = c;
				advance();
			}
		}
		c = peek();
	}

	if (c == '\'') {

		exit_compiler();
	}

	Token.lexeme[Token.index] = '\0';
	check_integer();
	Token.type = INTEGER;
	check_suffix();

}

void number_after_0() {

	int c = peek();
		
	if (c == 'x' || c == 'X') {
		advance();
		Token.lexeme[Token.index++] = c;
		number_hex();
	}
	else if (c == 'b' || c == 'B') {
		advance();
		Token.lexeme[Token.index++] = c;
		number_binary();
	}
	else if (c == '0') {
		advance();
		Token.lexeme[Token.index++] = c;
		number_flat();
	}
	else if (is_digit(c)) {
		advance();
		Token.lexeme[Token.index++] = c;
		number_octal();
	}		
	
}

void number_octal() {

	int c = peek();

	while (c >= '0' && c <= '7') {
		Token.lexeme[Token.index++] = c;
		advance();
		if (peek() == '\'') {
			advance();
			if (c < '0' || c > '7') {
				c = 0x39; // '
				break;
			}
			else {
				Token.lexeme[Token.index++] = c;
				advance();
			}
		}
		c = peek();
	}

	if (c == '\'') {

		exit_compiler();
	}

	Token.lexeme[Token.index] = '\0';
	check_integer();
	Token.type = INTEGER;
	check_suffix();
}

void number() {

	Token.index = 0;

	int c = peek();

	if (c == '0') {
		advance();
		Token.lexeme[Token.index++] = c;
		number_after_0();
	}
	else {
		advance();
		Token.lexeme[Token.index++] = c;
		number_flat();
	}

}
