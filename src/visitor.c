#include "visitor.h"
#include "scope.h"
#include <stdio.h>
#include <string.h>
#include <stdbool.h> 

bool visitor_debug = false;
void debug_visitor()
{
    visitor_debug = true;
}

static ast_T* buildin_func_print(visitor_T* visitor, ast_T** args, int args_size)
{
    for (int i = 0; i < args_size; i++)
    {
        ast_T* visited_ast = visitor_visit(visitor, args[i]);

        switch (visited_ast->type)
        {
            case AST_STRING: printf("%s\n", visited_ast->string_value); break;
            default: printf("%p\n", visited_ast); break;
        }
    }
    return init_ast(AST_NOOP);
}

visitor_T* visitor_init()
{
    visitor_T* visitor = calloc(1, sizeof(struct VISITOR_S));
    return visitor;
}

ast_T* visitor_visit(visitor_T* visitor, ast_T* node)
{
    if (visitor_debug) printf("debug 1.6 [visitor_visit]\n");
    switch (node->type)
    {
        case AST_VAR_DEF: return visitor_visit_var_def(visitor, node); break;
        case AST_FUNC_DEF: return visitor_visit_func_def(visitor, node); break;
        case AST_VARIABLES: return visitor_visit_var(visitor,node); break;
        case AST_FUNCTION_CALL: return visitor_visit_func_call(visitor,node); break;
        case AST_STRING: return visitor_visit_string(visitor,node); break;
        case AST_COMPOUND: return visitor_visit_compound(visitor, node); break;
        case AST_NOOP: return node; break;
        default: printf("default\n");
    }
    printf("uncought statementn '%d'\n", node->type);
    exit(1);
    return init_ast(AST_NOOP);
}

ast_T* visitor_visit_var_def(visitor_T* visitor, ast_T* node)
{
    if (visitor_debug) printf("debug 1.6 [visitor_visit_var_def]\n");
    scope_add_var_def(node->scope, node);
    return node;
}

ast_T* visitor_visit_func_def(visitor_T* visitor, ast_T* node)
{
    if (visitor_debug) printf("debug 1.6 [visitor_visit_func_def] {%s}\n", node->func_def_name);
    /*if (node->scope == NULL) {
        printf("ERROR: node->scope bernilai NULL saat simpan function '%s'!\n", node->func_def_name);
        exit(1);
    }*/
    scope_add_func_def(node->scope, node);
    return node;
}

ast_T* visitor_visit_var(visitor_T* visitor, ast_T* node)
{
    if (visitor_debug) printf("debug 1.6 [visitor_visit_var]\n");
    ast_T* vdef = scope_get_var_def(node->scope, node->variable_name);
    if (vdef == (void*) 0)
    {
        printf("Undefined variable '%s'\n", node->variable_name);
        exit(1);
    }

    ast_T* ast_var = init_ast(AST_VARIABLES);

    return visitor_visit(visitor, vdef->var_def_value);
}

ast_T* visitor_visit_func_call(visitor_T* visitor, ast_T* node)
{
    if (visitor_debug) printf("debug 1.6 [visitor_visit_func_call]\n");
    if (strcmp(node->func_call_name, "log.print") == 0)
    {
        return buildin_func_print(visitor, node->func_call_argument, node->func_call_arg_size);
    }

    ast_T* fdef = scope_get_func_def(node->scope, node->func_call_name);

    if (fdef == (void*) 0)
    {
        printf("undefied method %s\n", node->func_call_name);
        exit(1);
    }
    if (visitor_debug) printf("calling %s\n", node->func_call_name);
    for (int i = 0; i < fdef->func_def_args_size; i++)
    {
        ast_T* ast_var = (ast_T*) fdef->func_def_args[i]; 
        ast_T* ast_value = (ast_T*) node->func_call_argument[i]; 

        ast_T* var_def = init_ast(AST_VAR_DEF);
        var_def->var_def_var_name = ast_var->variable_name; 
        var_def->var_def_value = visitor_visit(visitor, ast_value); 

        scope_add_var_def(fdef->func_def_body->scope, var_def);
    }

    return visitor_visit(visitor, fdef->func_def_body);
}

ast_T* visitor_visit_string(visitor_T* visitor, ast_T* node)
{
    if (visitor_debug) printf("debug 1.6 [visitor_visit_string]\n");
    return node;
}

ast_T* visitor_visit_compound(visitor_T* visitor, ast_T* node)
{
    if (visitor_debug) printf("debug 1.6 [visitor_visit_compound]\n");
    for (int i = 0; i < node->compound_size; i++)
    {
        if (visitor_debug) printf("debug 2.6 [visitor_visit_compound] %d\n", i);
        visitor_visit(visitor, node->compound_value[i]);
    }
    return init_ast(AST_NOOP);
}