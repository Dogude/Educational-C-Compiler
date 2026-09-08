#pragma once
#include "lexer.h"

typedef enum {
    SYM_VARIABLE,
    SYM_TYPEDEF,
    SYM_FUNCTION,
    SYM_STRUCT,
    SYM_ENUM,
    SYM_LABEL
} SymbolKind;

enum ParserState {

    START,
    PREPROCESSOR,
    EMBED_DIRECTIVE,

};

int state;
struct Scope *current_scope;

struct Symbol {
    char* name;              
    SymbolKind kind;         
    int type;                
    int offset;              
};

struct Scope {
    struct Symbol** table;
    struct Scope* parent;
};

