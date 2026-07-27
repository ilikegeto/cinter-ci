#ifndef SCOPE_H
#define SCOPE_H
#include "AST.h"

typedef struct SCOPE_S 
{
    ast_T** func_def;
    size_t func_def_size;

    ast_T** var_def;
    size_t var_def_size;
} scope_T;
void debug_scope();
scope_T* init_scope();
ast_T* scope_add_func_def(scope_T* scope, ast_T* fdef);
ast_T* scope_get_func_def(scope_T* scope, const char* fname);

ast_T* scope_add_var_def(scope_T* scope, ast_T* vdef);
ast_T* scope_get_var_def(scope_T* scope, const char* name);

#endif