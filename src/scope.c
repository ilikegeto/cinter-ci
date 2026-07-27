#include "scope.h"
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

bool scope_debug = false;
void debug_scope()
{
    scope_debug = true;
}

scope_T* init_scope()
{
    scope_T* scope = calloc(1, sizeof(struct SCOPE_S));
    scope->func_def = (void*) 0;
    scope->func_def_size = 0;

    scope->var_def = (void*) 0;
    scope->var_def_size = 0;

    return scope;
}

ast_T* scope_add_func_def(scope_T* scope, ast_T* fdef)
{
    if (scope_debug) printf("debug 1.8 [scope_add_func_def]\n");
    scope->func_def_size += 1;
    if (scope->func_def == (void*) 0)
    {
        scope->func_def = calloc(1, sizeof(struct AST_S*));
    }
    else
    {
        scope->func_def = realloc(scope->func_def, scope->func_def_size * sizeof(struct AST_S*));
    }

    scope->func_def[scope->func_def_size - 1] = fdef;
    return fdef;
}

ast_T* scope_get_func_def(scope_T* scope, const char* fname)
{
    for (int i = 0; i < scope->func_def_size; i++)
    {
        ast_T* fdef = scope->func_def[i];
        if (strcmp(fdef->func_def_name, fname) == 0)
        {
            return fdef;
        }
    }
    return (void*) 0;
}

ast_T* scope_add_var_def(scope_T* scope, ast_T* vdef)
{
    if (scope->var_def == (void*) 0)
    {
        scope->var_def = calloc(1, sizeof(struct AST_S*));
        scope->var_def[0] = vdef;
        scope->var_def_size = 1;
    }
    else
    {
        scope->var_def_size += 1;
        scope->var_def = realloc(scope->var_def, scope->var_def_size * sizeof(struct AST_S*));
        scope->var_def[scope->var_def_size - 1] = vdef;    
    }
    return vdef;
}
ast_T* scope_get_var_def(scope_T* scope, const char* name)
{
    for (int i = 0; i < scope->var_def_size; i++)
    {
        ast_T* vdev = scope->var_def[i];
        if (strcmp(vdev->var_def_var_name, name) == 0)
        {
            return vdev;
        }
    }
    return (void*) 0;
}