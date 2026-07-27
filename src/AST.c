#include "AST.h"

ast_T* init_ast(int type)
{  
    ast_T* ast = calloc(1, sizeof(struct AST_S));
    ast->type = type;
    ast->scope = (void*) 0;

    ast->var_def_var_name = (void*)0;
    ast->var_def_value = (void*)0;

    ast->func_def_body = (void*) 0; 
    ast->func_def_name = (void*) 0;
    ast->func_def_args = (void*) 0;
    ast->func_def_args_size = 0;

    ast->variable_name = (void*)0;

    ast->func_call_name = (void*)0;
    ast->func_call_argument = (void*) 0;
    ast->func_call_arg_size = 0;

    ast->string_value = (void*) 0;

    ast->compound_value = (void*) 0; 
    ast->compound_size = 0;

    return ast;
}