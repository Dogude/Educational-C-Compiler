#include "parser.h"

#define SymbolVectorInit (4 * 1024 + 3)
#define SymbolVectorGrow 2

#define EXPR_STACK 256

// ParserState

void init_vector(ctx* c) {

	sym* p = malloc(sizeof(sym) * SymbolVectorInit);
	if (!p) {
		printf(COLOR_ERROR "Memory Error.\n" COLOR_RESET);
		exit_compiler();
	}

	c->head = p;
	c->size = 0;
	c->capacity = SymbolVectorInit;
	c->state = START;
}
// 
void append_vector(ctx* c , sym * s) {
	
	if (c->size < c->capacity) {	
		size_t i = c->size;
		memcpy(&c->head[i], s, sizeof(sym));
		c->size++;
	}
	else {
		size_t new_cap = c->size * SymbolVectorGrow;
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

void release_vector(ctx* c) {
	
	free(c->head);
}

static unsigned int hash_function(const unsigned char *str , ctx* c) {
    unsigned int hash = 5381; // 1010100000101
    int c;
    while ((c = *str++)) {
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */
    }
    return hash % c->capacity; // 0 4095
}

typedef struct ExprStack {
	struct Token *head;
	unsigned int top;
	unsigned int size;
	unsigned int capacity;		
} ExprStack ;


void preprocessor_expr_eval(ctx* c) {
// handle #if #elif expressions	


}


void preprocessor_expr_parser(ctx* c) {
// handle #if #elif expressions	
	

}


void expr_eval(ExprStack * s){


}


void push(ExprStack * s , struct Token * t){
	
	if(s->size < s->capacity){
		s->head[s->size] = *t;
		s->size++;
	} 
	else {
		
		struct Token *ptr = NULL;
		unsigned int new_capacity = s->size * 2;
		
		// can not exceed 0XFFFFFFF

		if(s->size == EXPR_STACK){
			ptr = malloc(new_capacity * sizeof(struct Token));
			if(!ptr){

				exit_compiler();
			}		
			memcpy(ptr, s->head, sizeof(struct Token) * EXPR_STACK);
		
		} else {

			ptr = realloc(s->head, new_capacity * sizeof(struct Token));
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

struct Token * pop(ExprStack * s){

	

}

void expr_parser(ctx* c) {
// handle C expressions	
	
	c->state = OPERAND ;
	struct Token token ;
	lexer(&token);
	struct Token a[EXPR_STACK]; // default limit (256) on cpu stack
	struct Token b[EXPR_STACK]; // default limit (256) on cpu stack
	ExprStack operator = {&a,0,0,EXPR_STACK};
	ExprStack operand  = {&b,0,0,EXPR_STACK};

	while(1) {

		lexer(&token);

		switch(c->state) {

			case OPERAND:
				switch(token.type){
					case PLUS_PLUS:
						token.type = PRE_PLUS_PLUS;
						token.precedence = PRECEDENCE_PRE_PLUS_PLUS;
						push(&operator,&token);
						break;	
					case MINUS:
						token.type = UNARY_MINUS;
						token.precedence = PRECEDENCE_UNARY_MINUS;
						push(&operator,&token);
						break;	
					case PLUS:
						token.type = UNARY_PLUS;
						token.precedence = PRECEDENCE_UNARY_PLUS;
						push(&operator,&token);
						break;		
					case IDENTIFIER:
						token.index = 123;
						
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

void start(ctx * c) {

	switch (Token.type) {
	
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

void preprocessor(ctx* c) {

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
void embed_directive(ctx* c) {

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

void pragma_directive(ctx* c) {

	
}

void parser() {

	ctx parser_context = { 0 };
	init_vector(&parser_context);
	
	lexer();

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
