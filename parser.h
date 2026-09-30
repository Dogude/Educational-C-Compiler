#pragma once
#include "lexer.h"

extern IncludeStack* top;
void push_file(IncludeStack** top, char* file_name);
void lexer();

enum ParserState {
    START,
    PREPROCESSOR,
    EMBED_DIRECTIVE,
    PRAGMA_DIRECTIVE,
    EXPR_STATE
};

typedef struct Symbol {
    char* name;                     
    unsigned int type;                
    int offset;
    size_t number;
    
} sym;

typedef struct SymbolContext {
    size_t size;
    size_t capacity;
    sym * head;
    int state;
} ctx ;


