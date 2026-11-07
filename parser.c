#include "common.h"

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

void init_symbol(SymbolContext* c) {

	sym* p = malloc(sizeof(sym) * SYMBOL_VECTOR_INIT);
	if (!p) {
		printf(COLOR_ERROR "Memory Error.\n" COLOR_RESET);
		exit_compiler();
	}

	c->head = p;
	c->size = 0;
	c->capacity = SYMBOL_VECTOR_INIT;
	c->state = START;
}

void append_symbol(SymbolContext* c , sym * s) {
	
	if (c->size < c->capacity) {	
		size_t i = c->size;
		memcpy(&c->head[i], s, sizeof(sym));
		c->size++;
	}
	else {
		size_t new_cap = c->size * SYMBOL_VECTOR_GROW;
		void* ptr = realloc(c->head, new_cap * sizeof(sym));
		if (!ptr) {
			printf(COLOR_ERROR "Memory Error.\n" COLOR_RESET);
			exit_compiler();
		}

		c->head = ptr;
		size_t i = c->size;
		memcpy(&c->head[i], s, sizeof(sym));
		c->size++;
		c->capacity = new_cap;
	}
		
}

void release_symbols(SymbolContext* c) {
	
	free(c->head);
}

void init_macro(TokenContext* c) {

	Token *p = malloc(sizeof(Token) * MACRO_VECTOR_INIT);
	
	if (!p) {
		printf(COLOR_ERROR "Memory Error.\n" COLOR_RESET);
		exit_compiler();
	}

	c->head = p;
	c->size = 0;
	c->capacity = MACRO_VECTOR_INIT;

}

void append_macro(TokenContext* c , Token * t) {
	
	if (c->size < c->capacity) {	
		unsigned int i = c->size;
		memcpy(&c->head[i], t, sizeof(struct Token));
		c->size++;
	}
	else {
		unsigned int new_cap = c->size * MacroVectorGrow;
		void* ptr = realloc(c->head, new_cap * sizeof(struct Token));
		if (!ptr) {
			printf(COLOR_ERROR "Memory Error.\n" COLOR_RESET);
			exit_compiler();
		}

		c->head = ptr;
		unsigned int i = c->size;
		memcpy(&c->head[i], t, sizeof(struct Token));
		c->size++;
		c->capacity = new_cap;
	}
		
}

void release_macro(TokenContext* c) {
	
	free(c->head);
}


void preprocessor_expr_eval(SymbolContext* c) {
// handle #if #elif expressions	


}


void preprocessor_expr_parser(SymbolContext* c) {
// handle #if #elif expressions	
	

}


// generate x64 opcode
void expr_eval(TokenContext * s){


}

void push(TokenContext * s , Token * t){
	
	if(s->size < s->capacity) {
		s->head[s->size] = *t;
		s->size++;
	}  
	else {	
		struct Token *ptr = NULL;
		unsigned int new_capacity = s->size * 2;

		if(s->size == EXPR_STACK) {
			ptr = malloc(new_capacity * sizeof(struct Token));
			if(!ptr) {

				exit_compiler();
			}		
			memcpy(ptr, s->head, sizeof(Token) * EXPR_STACK);
		
		} else {

			ptr = realloc(s->head, new_capacity * sizeof(Token));
			if(!ptr){

				exit_compiler();
			}	
		
		}

		s->head = ptr;
		s->head[s->size] = *t;
		s->size++;
		s->capacity = new_capacity;
	
	}

}

Token * pop (TokenContext * s){


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

	if(operator.capacity > EXPR_STACK) {
		free(operator.head);
	}

	if(operand.capacity > EXPR_STACK){
		free(operand.head);
	}

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

void parser(IncludeContext * inc) {

	SymbolContext parser_context = { 0 };
	init_vector(&parser_context);
	init_symbol();

	switch (parser_context.state) {

	case START:
		start(&parser_context);
		break;

	case PREPROCESSOR:
		preprocessor(&parser_context);
		break;

	case EMBED_DIRECTIVE:
		embed_directive(&parser_context);
		break;

	case PRAGMA_DIRECTIVE:
		pragma_directive(&parser_context);
		break;

	default:
		break;
	}	
}