#ifndef AST_H
#define AST_H
#include <stdlib.h>
typedef enum 
{
    AST_VAR_DEF,
    AST_FUNC_DEF,
    AST_VARIABLES,
    AST_FUNCTION_CALL,
    AST_STRING,
    AST_COMPOUND,
    AST_NOOP
} ast_type;

typedef struct AST_S 
{
    ast_type type;

    struct SCOPE_S* scope;
    //ast_var_def
    char* var_def_var_name;
    struct AST_S* var_def_value;

    //func_def_body
    struct AST_S* func_def_body;
    char* func_def_name;
    struct AST_S** func_def_args;
    size_t func_def_args_size;

    //ast_variables
    char* variable_name;

    //ast_func_call
    char* func_call_name;
    struct AST_S** func_call_argument;
    size_t func_call_arg_size;

    //ast_sting
    char* string_value;

    //AST_compound
    struct AST_S** compound_value;
    size_t compound_size;
}ast_T;

ast_T* init_ast(int type);
#endif