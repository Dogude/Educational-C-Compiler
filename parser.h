#pragma once
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef enum {
    SYM_VARIABLE,
    SYM_TYPEDEF,
    SYM_FUNCTION,
    SYM_STRUCT,
    SYM_ENUM,
    SYM_LABEL
} SymbolKind;

enum ParserState {




};

struct Symbol {
    char* name;              
    SymbolKind kind;         
    int type;                
    int offset;              

    union {                  
        struct {
            int array_size;  
        } variable;

        struct {
            int original_type;   
            int pointer_depth;   
        } typedef_info;

        struct {
            int param_count;
            int return_type;
            struct Symbol** params;  
        } function;

        struct {
            int size;             
            int field_count;
            struct Symbol** fields; 
        } struct_info;

        struct {
            int enum_value;       
        } enum_info;
    } fields;

} ParserState;

struct Scope {
    struct Symbol** table;
    struct Scope* parent;
};

struct Scope* current_scope;
