#include "common.h"

void push_include(CompilerMemory *cm , const char *file) {
	
	IncludeContext *c = cm->include.head;
	int i = cm->include.size;
	
	FILE *f = fopen(file, "rb");
	if(!f){
		
		exit_compiler();
	}

	fseek(f, 0, SEEK_END);
	long size = ftell(f);	
	unsigned char *source = malloc(size);
	
	if(!source) {

		exit_compiler();
	}

	fread(source,1,size,f);
	fclose(f);
	
	c[i].source = source;	
	c[i].fpos = 0;
	c[i].last_line = 0;
	c[i].line = 0;
	c[i].len = size;

}

enum SymbolType {

    SYMBOL_CHAR = 0,
    SYMBOL_SHORT = 128,
    SYMBOL_INT = SYMBOL_SHORT + 128,
    SYMBOL_LONG =  SYMBOL_INT + 128,
    SYMBOL_LONG_LONG = SYMBOL_LONG + 128,
     
    SYMBOL_FLOAT = SYMBOL_LONG_LONG + 128,
    SYMBOL_DOUBLE = SYMBOL_FLOAT + 128,
    
};

enum ParserState {
    START,
    PREPROCESSOR,
    EMBED_DIRECTIVE,
    PRAGMA_DIRECTIVE,
    EXPR_STATE, OPERAND, OPERATOR
};

void preprocessor_expr_eval(SymbolContext* c) {
// handle #if #elif expressions	


}


void preprocessor_expr_parser(SymbolContext* c) {
// handle #if #elif expressions	
	

}


// generate x64 opcode
void expr_eval(TokenContext * s){


}

void expr_parser(SymbolContext* c) {
// handle C expressions	
	
	c->state = OPERAND ;
	Token token;
	lexer(&token);
	Token a[EXPR_STACK]; // default limit (256) on cpu stack
	Token b[EXPR_STACK]; // default limit (256) on cpu stack
	TokenContext operator = {a,0,EXPR_STACK};
	TokenContext operand  = {b,0,EXPR_STACK};

	while(1) {
		lexer(&token);
		switch(c->state) {
			case OPERAND:
				switch(token.type) {
					case PLUS_PLUS:
						token.type = PRE_PLUS_PLUS;
						token.info.precedence = PRECEDENCE_PRE_PLUS_PLUS;
						push(&operator,&token);
						break;	
					case MINUS:
						token.type = UNARY_MINUS;
						token.info.precedence = PRECEDENCE_UNARY_MINUS;
						push(&operator,&token);
						break;	
					case PLUS:
						token.type = UNARY_PLUS;
						token.info.precedence = PRECEDENCE_UNARY_PLUS;
						push(&operator,&token);
						break;		
					case IDENTIFIER:
						token.info.index = 123;
						
						c->state = OPERAND;
						break;

				}

				break;

			case OPERATOR:
				switch(token.type){
					
				}
			
				break;

		}
    

	}

	// final state of parser	
	// while not stack empty

}

void start(SymbolContext * c , Token * t) {

	switch (t->type) {
	
	case SHARP:
		c->state = PREPROCESSOR;
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
	}

}

void preprocessor(SymbolContext* c) {

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
		c->state = EMBED_DIRECTIVE;
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
		c->state = PRAGMA_DIRECTIVE;
		break;

	}

}

/* a comma seperated unsigned char array for context */
void embed_directive(SymbolContext* c) {

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
		
			switch (t.type) {

			case FILE_STR:
				embed_state = PARAMETER;
				Token.state = STR_MOD;
				break;
			default:
				// error case
				break;

			}
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

			}
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
			}
			break;

		case FINISH:
			switch (Token.type) {
			case NEW_LINE:
				return;
			default:
				// error
				break;
			}
			break;

		}

	}

}

void pragma_directive(SymbolContext* c) {

	
}

void parser(const char *main) {
	
	CompilerMemory cm = {0};
	push_include(&cm,main);
	
	switch (cm.state) {

	case START:
		start(&cm);
		break;

	case PREPROCESSOR:
		preprocessor(&cm);
		break;

	case EMBED_DIRECTIVE:
		embed_directive(&cm);
		break;

	case PRAGMA_DIRECTIVE:
		pragma_directive(&cm);
		break;

	default:
		break;
	}

	
}