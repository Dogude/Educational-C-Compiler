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
	
	case CONST:
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
	case PRAGMA:
		state = PRAGMA_DIRECTIVE;
		break;
	
	};

}

/* a comma seperated unsigned char array for context */
void embed_directive() {

	enum EmbedState {

		SEQUENCE,
		PARAMETER,
		PARAMETER_OPEN,
		PARAMETER_CLOSE,
		FINISH
	};

	int embed_state = SEQUENCE;
	
	FILE* file;

	Token.state = FILE_MOD; // switch to lexer mode to read file path in ""

	while (1) {

		lexer();

		switch (embed_state) {

		case SEQUENCE:
		
			switch (Token.type) {

			case FILE_STR:
				embed_state = PARAMETER;
				Token.state = STR_MOD;
				break;
			default:
				// error case
				break;

			};
			break;

		case PARAMETER:
			switch (Token.type) {

			case EMBED_IF_EMPTY:
				break;

			case EMBED_LIMIT:
				break;

			case EMBED_PREFIX:
				break;

			case EMBED_SUFFIX:
				break;

			case NEW_LINE:
				return;

			default:
				// error case
				break;

			};
			break;

		case PARAMETER_OPEN:

			break;

		case PARAMETER_CLOSE:
			switch (Token.type) {
			case CLOSE_PAR:
				embed_state = FINISH;
				break;
			default:
				// error
				break;
			};
			break;

		case FINISH:
			switch (Token.type) {
			case NEW_LINE:
				return;
			default:
				// error
				break;
			};
			break;

		};

	}

}

void pragma_directive() {

	

}

void parser() {

	state = START;

	while (1) {

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

		case PRAGMA_DIRECTIVE:
			pragma_directive();
			break;

		default:
			break;
		}
	
	}

}
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
	
	case CONST:
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
	case PRAGMA:
		state = PRAGMA_DIRECTIVE;
		break;
	
	};

}

/* a comma seperated unsigned char array for context */
void embed_directive() {

	enum EmbedState {

		SEQUENCE,
		PARAMETER,
		PARAMETER_OPEN,
		PARAMETER_CLOSE,
		FINISH
	};

	int embed_state = SEQUENCE;
	
	FILE* file;

	Token.state = FILE_MOD; // switch to lexer mode to read file path in ""

	while (1) {

		lexer();

		switch (embed_state) {

		case SEQUENCE:
		
			switch (Token.type) {

			case FILE_STR:
				embed_state = PARAMETER;
				Token.state = STR_MOD;
				break;
			default:
				// error case
				break;

			};
			break;

		case PARAMETER:
			switch (Token.type) {

			case EMBED_IF_EMPTY:
				break;

			case EMBED_LIMIT:
				break;

			case EMBED_PREFIX:
				break;

			case EMBED_SUFFIX:
				break;

			case NEW_LINE:
				return;

			default:
				// error case
				break;

			};
			break;

		case PARAMETER_OPEN:

			break;

		case PARAMETER_CLOSE:
			switch (Token.type) {
			case CLOSE_PAR:
				embed_state = FINISH;
				break;
			default:
				// error
				break;
			};
			break;

		case FINISH:
			switch (Token.type) {
			case NEW_LINE:
				return;
			default:
				// error
				break;
			};
			break;

		};

	}

}

void pragma_directive() {

	

}

void parser() {

	state = START;

	while (1) {

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

		case PRAGMA_DIRECTIVE:
			pragma_directive();
			break;

		default:
			break;
		}
	
	}

}
