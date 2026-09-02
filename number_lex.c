#include "lexer.h"

enum NumberStates {

	INTEGER,
	SUFFIX,
	BINARY,
	HEX,
	OCTAL,

	QUOTE,
	AFTER_0,
	NUMBER_FLAT,
	SUCCESS

};

enum FloatNumberStates {

	DOUBLE,
	FLOAT,
	LONG_DOUBLE

};

int f_number(int c, int index) {

	

}

void check_integer() {

	Token.lexeme[Token.index] = '\0';

	if (strlen(Token.lexeme) > 20) { // 64-bit unsigned long long
		print_line();
		printf("\n\nerror : number literal too big for unsigned long long");
		exit_compiler();
	}
		
	errno = 0;
	unsigned long long val = strtoull(Token.lexeme, NULL, 0);
	
}


void number_binary() {

	int c = peek();
	
	if (c != '1' || c != '0') {

		exit_compiler();
	}

	while (c == '1' || c == '0') {
		Token.lexeme[Token.index++] = c;
		advance();	
		if (c == '\'') {
			advance();
		}
		c = peek();
	}
	
	if (c = 'u' || 'U') {
		

	}

	else if (c == 'l' || c == 'L') {


	}


}

void number_after_0() {

	int c = peek();
		
	if (c == 'x' || c == 'X') {
		advance();
		Token.lexeme[Token.index++] = c;
		Token.state = HEX;
	}
	else if (c == 'b' || c == 'B') {
		advance();
		Token.lexeme[Token.index++] = c;
		Token.state = BINARY;
	}
	else if (c == '0') {
		advance();
		Token.lexeme[Token.index++] = c;
		Token.state = NUMBER_FLAT;
	}
	else if (is_digit(c)) {
		advance();
		Token.lexeme[Token.index++] = c;
		Token.state = OCTAL;
	}		
	else if (is_alpha(c)) {
		print_line();
		printf("\n\nerror : number literal can not include '%c' after 0", c);
		exit_compiler();
	}
	

}

inline int number_hex(int c, int state, int index) {

	if (is_digit(c))
		Token.lexeme[index] = c;
	else if (c >= 'a' && c <= 'f')
		Token.lexeme[index] = c;
	else if (c >= 'A' && c <= 'F')
		Token.lexeme[index] = c;

	else if (c == 'u' || c == 'U') {


	}
	else if (c == 'l' || c == 'L') {


	}
	else if (is_alpha(c)) {
		print_line();
		printf("\n\nerror : can not include '%c' in hex number", c);
		exit_compiler();
	}
	else {
		state = SUCCESS;
	}


	return state;

}

inline int number_flat(int c, int state, int index) {

	if (is_digit(c))
		Token.lexeme[index] = c;

	else if (c == 'u' || c == 'U') {


	}
	else if (c == 'l' || c == 'L') {


	}
	else if (is_alpha(c)) {
		print_line();
		printf("\n\nerror : number literal can not include '%c'", c);
		exit_compiler();
	}
	else {
		state = SUCCESS;
	}
	return state;

}

void number_integer() {

	int c = peek();

	if (c == '0') {
		advance();
		Token.lexeme[Token.index++] = c;
		Token.state = AFTER_0;
	}
	else {
		advance();
		Token.lexeme[Token.index++] = c;
		Token.state = NUMBER_FLAT;
	}


}

inline int number_octal(int c, int state, int index) {

	if (c >= '0' && c <= '7')
		Token.lexeme[index] = c;

	else if (c == 'u' || c == 'U') {


	}
	else if (c == 'l' || c == 'L') {


	}
	else if (is_alpha(c) || c == '8' || c == '9') {
		print_line();
		printf("\n\nerror : can not include '%c' in octal number", c);
		exit_compiler();
	}
	else {
		state = SUCCESS;
	}

	return state;
}

void number() {

	Token.index = 0;
	Token.state = INTEGER;

	number_loop:

	switch (Token.state) {
	
	case INTEGER:
		number_integer();
		break;

	case AFTER_0:
		number_after_0();
		break;

	case NUMBER_FLAT:
		number_flat(c, state, index);
		break;

	case HEX:
		number_hex(c, state, index);
		break;

	case BINARY:
		number_binary(c, state, index);
		break;

	case OCTAL:
		number_octal(c, state, index);
		break;

	case SUFFIX:
		break;

	case SUCCESS:
		return;

	defaut:
		break;

	};
	
	goto number_loop;

}