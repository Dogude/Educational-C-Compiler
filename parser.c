#include "parser.h"

void start() {

	switch (Token.type) {

	case SHARP:
		state = PREPROCESSOR;
		break;

	case TYPEDEF:
		break;

	case NEW_LINE:
		break;

	case STATIC:
		break;

	case INLINE:
		break;
		
	case ENUM:
	
		break;
	
	case STRUCT:
		break;
	
	case INT:
		break;
	case VOID:
		break;
	case DOUBLE:
		break;
	case FLOAT:
		BREAK;
	
	default:
		// error
		break;

	};

}

void preprocessor() {

	switch (Token.type) {

	case DEFINE:
		break;
	case ELIFDEF:
		break;
	case ELIFNDEF:
		break;
	case INCLUDE:
		break;
	case EMBED:
		state = EMBED_DIRECTIVE;
		break;
	case IFDEF:
		break;
	case IF:
		break;
	case ELSE:
		break;
	case ELIF:
		break;
	
	};

}

void embed_directive() {

	enum EmbedState {

		SEQUENCE,
		PARAMETER
	};

	int embed_state = SEQUENCE;

	embed:

	lexer();

	switch (embed_state) {
	
	case SEQUENCE:
		if (Token.type == 0) {
			FILE* embed_file = fopen(Token.lexeme,"rb");
			
			state = PARAMETER;
		}
		break;
			
	case PARAMETER:
		if (Token.type == EMBED_IF_EMPTY) {
			
		}
		break;

	default:
		break;
	};


	goto embed;

}

void parser() {

	state = START;

	parser_loop:

	lexer();

	switch (state) {

	case START:
		start();
		break;

	case PREPROCESSOR:
		preprocessor();
		break;
	
	case EMBED_DIRECTIVE:
		embed_directive();
		break;

	default:
		break;
	}

	goto parser_loop;

}
