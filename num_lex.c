#include "lexer.h"

void check_double() {

	errno = 0;
	double dval;
	float fval;

	switch (Token.type) {

	case DOUBLE_LITERAL:
		dval = strtod(Token.lexeme, NULL, 0);
		if (errno == ERANGE) {
			
			exit_compiler();
		}
		// write to data segment
		break;

	case FLOAT_LITERAL:
		fval = strtof(Token.lexeme, NULL, 0);
		if (errno == ERANGE) {

			exit_compiler();
		}
		// write to data segment

		break;

	};
	
}

int fnumber() {

	
}

void check_integer() {
	
	errno = 0;
	unsigned long long val = strtoull(Token.lexeme, NULL, 0);
	
	if (errno == ERANGE) {	
		exit_compiler();
	}	

	Token.number_size = 8;

	if (val <= 0xffffffff)
		Token.number_size = 4;
	if (val <= 0xffff)
		Token.number_size = 2;
	if (val <= 0xff)
		Token.number_size = 1;
		
	memcpy(&val,Token.number,Token.number_size);

}

void check_suffix() {

	int c = peek();
	
	suffix:
	switch (Token.type) {
	case INTEGER_LITERAL:
		switch (c) {
		case 'l':
		case 'L': // 12L
			advance();
			Token.type = L;			
			break;
		case 'u':
		case 'U': //12u
			advance();
			Token.type = U;
			break;
		};
		break;
	
	case U: // 12uL
		switch (c) {
		case 'L':
		case 'l':
			advance();
			Token.type = UL;
			break;
		};
		break;
	
	case L: // 12lu
		switch (c) {
		case 'U':
		case 'u':
			advance();
			Token.type = UL;
			return;			
		case 'l':
		case 'L': // 12ll
			advance();
			Token.type = LL;
			break;	
		};
		break;

	case LL:
		switch (c) {
		case 'U': // 12llu
		case 'u':
			advance();
			Token.type = ULL;
			return;
		};
		break;

	case UL:
		switch (c) {
		case 'L': // 12ull
		case 'l':
			advance();
			Token.type = ULL;		
			return;
		};
		break;

	};

	c = peek();
	goto suffix;
	
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
	Token.type = INTEGER;
	check_suffix();
	check_integer();

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
	Token.type = INTEGER;
	check_suffix();
	check_integer();
	
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
	Token.type = INTEGER;
	check_suffix();
	check_integer();
}

void number_after_0() {

	int c = peek();
	
	switch (c) {
	case 'x':
	case 'X':
		advance();
		Token.lexeme[Token.index++] = c;
		number_hex();
		break;
	case 'b':
	case 'B':
		advance();
		Token.lexeme[Token.index++] = c;
		number_binary();
		break;
	case '0':
		advance();
		Token.lexeme[Token.index++] = c;
		number_flat();
		break;
	case '.':
		advance();
		Token.lexeme[Token.index++] = c;
		fnumber();
		break;
	default:
		if (is_digit(c)) {
			advance();
			Token.lexeme[Token.index++] = c;
			number_octal();
		}
		break;
	};

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
	Token.type = INTEGER;
	check_suffix();
	check_integer();
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
