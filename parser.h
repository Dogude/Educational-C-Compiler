#pragma once
#include "lexer.h"

enum ParserState {
    START,
    PREPROCESSOR,
    EMBED_DIRECTIVE,
    PRAGMA_DIRECTIVE
};

int state;
struct Scope *current_scope;

struct Symbol {
    char* name;                     
    int type;                
    int offset;              
};

struct Scope {
    struct Symbol** table;
    struct Scope* parent;
};

