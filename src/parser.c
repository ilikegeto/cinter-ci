#include "parser.h"
#include <stdio.h>
#include <string.h>
#include <stdbool.h> 

bool parser_debug = false;
void debug_parser()
{
    parser_debug = true;
}

/*static scope_T* get_node_scope(parser_T* parser, ast_T* node)
{
    return node->scope == (void*) 0 ? parser->scope : node->scope;
}*/

parser_T* init_parser(lexer_T* lexer)
{
    parser_T* parser = calloc(1, sizeof(struct PARSER_S));
    parser->lexer = lexer;
    parser->current_token = lexer_get_next_token(lexer);
    parser->prev_token = parser->current_token;
    parser->scope = init_scope();

    return parser;
}

void parser_eat(parser_T* parser, int token_ty)
{
    if (parser_debug) printf("debug 1.5 [parser_eat]\n");
    if ((int)parser->current_token->type == token_ty)
    {
        parser->prev_token = parser->current_token;
        parser->current_token = lexer_get_next_token(parser->lexer);
    }
    else
    {
        printf("what is that token? 'shotgun'");
        printf("unexpected token '%s', with type '%d'\n", parser->current_token->value, parser->current_token->type);
        exit(1);
    }
}
ast_T* parser_parse(parser_T* parser, scope_T* scope)
{
    return parser_parse_statements(parser, scope);
}

ast_T* parser_parse_statement(parser_T* parser, scope_T* scope)
{
    if (parser_debug) printf("debug 1.5 [parser_parse_statement]\n");
    switch (parser->current_token->type)
    {
        case TOKEN_ID: return parser_parse_id(parser, scope);
        default: if (parser_debug) printf("debug 2.5 [parser_parse_statement]\n");
    }
    return init_ast(AST_NOOP);
}
ast_T* parser_parse_statements(parser_T* parser, scope_T* scope)
{
    ast_T* compound = init_ast(AST_COMPOUND);
    compound->scope = scope;
    compound->compound_value = calloc(1, sizeof(struct AST_STRUCT*));

    ast_T* ast_statement = parser_parse_statement(parser, scope);
    ast_statement->scope = scope;
    compound->compound_value[0] = ast_statement;
    compound->compound_size += 1;

    while (parser->current_token->type == TOKEN_SEMIC)
    {
        parser_eat(parser, TOKEN_SEMIC);

        ast_T* ast_statement = parser_parse_statement(parser, scope);

        if (ast_statement)
        {
            compound->compound_size += 1;
            compound->compound_value = realloc(
                compound->compound_value,
                compound->compound_size * sizeof(struct AST_STRUCT*)
            );
            compound->compound_value[compound->compound_size - 1] = ast_statement;
        }
    }

    return compound;
}

ast_T* parser_parse_expr(parser_T* parser, scope_T* scope)
{
    if (parser_debug) printf("debug 1.5 [parser_parse_expr]\n");
    switch (parser->current_token->type)
    {
        case TOKEN_STRING: return parser_parse_string(parser, scope);
        case TOKEN_ID: return parser_parse_id(parser, scope);

    }
    return init_ast(AST_NOOP);
}
ast_T* parser_parse_factor(parser_T* parser, scope_T* scope)
{}
ast_T* parser_parse_term(parser_T* parser, scope_T* scope)
{}
ast_T* parser_parse_func_call(parser_T* parser, scope_T* scope)
{
    if (parser_debug) printf("debug 1.5 [parser_parse_func_call]\n");
    ast_T* func_call = init_ast(AST_FUNCTION_CALL);
    func_call->func_call_name = parser->prev_token->value;
    parser_eat(parser, TOKEN_LPAREN);
    func_call->func_call_argument = calloc(1, sizeof(struct AST_S*));

    ast_T* ast_expr = parser_parse_expr(parser, scope);
    func_call->func_call_argument[0] = ast_expr;
    func_call->func_call_arg_size = 1;
    while (parser->current_token->type == TOKEN_COMMA)
    {
        parser_eat(parser, TOKEN_COMMA);
        ast_T* next_expr = parser_parse_expr(parser, scope);
        
        func_call->func_call_arg_size += 1;
        func_call->func_call_argument = realloc(
            func_call->func_call_argument, 
            func_call->func_call_arg_size * sizeof(struct AST_S*) 
        );
        func_call->func_call_argument[func_call->func_call_arg_size - 1] = next_expr;
    }

    parser_eat(parser, TOKEN_RPAREN);

    func_call->scope = scope;

    return func_call;
}

ast_T* parser_parse_var_def(parser_T* parser, scope_T* scope)
{
    if (parser_debug) printf("debug 1.5 [parser_parse_var_def]\n");
    parser_eat(parser, TOKEN_ID);
    char* def_var_name = parser->current_token->value;
    parser_eat(parser, TOKEN_ID);
    parser_eat(parser, TOKEN_EQUALS);
    ast_T* def_var_value = parser_parse_expr(parser, scope);

    ast_T* var_def = init_ast(AST_VAR_DEF);
    var_def->var_def_var_name = def_var_name;
    var_def->var_def_value = def_var_value;

    if (parser_debug) printf("debug 2.5 [parser_parse_var_def]\n");

    var_def->scope = scope;

    return var_def;
}

ast_T* parser_parse_func_def(parser_T* parser, scope_T* scope)
{
    if (parser_debug) printf("debug 1.5 [parser_parse_func_def]\n");
    
    ast_T* ast = init_ast(AST_FUNC_DEF);
    parser_eat(parser, TOKEN_ID); //func

    char* func_name = parser->current_token->value;
    ast->func_def_name = calloc(strlen(func_name) + 1, sizeof(char));
    strcpy(ast->func_def_name, func_name);

    parser_eat(parser, TOKEN_ID); // name
    parser_eat(parser, TOKEN_LPAREN);  

    ast->func_def_args = calloc(1, sizeof(struct AST_S*));
    ast_T* args = parser_parse_var(parser, scope);
    ast->func_def_args_size += 1;
    ast->func_def_args[ast->func_def_args_size - 1] = args;

    while (parser->current_token->type == TOKEN_COMMA)
    {
        parser_eat(parser, TOKEN_COMMA);
        ast->func_def_args_size += 1;

        ast->func_def_args = realloc(ast->func_def_args, ast->func_def_args_size * sizeof(struct AST_S*));
        ast_T* args = parser_parse_var(parser, scope);
        ast->func_def_args[ast->func_def_args_size - 1] = args;
    }

    parser_eat(parser, TOKEN_RPAREN);  
    parser_eat(parser, TOKEN_LBRACE); 

    ast->func_def_body = parser_parse_statements(parser, scope);

    parser_eat(parser, TOKEN_RBRACE);  

    ast->scope = scope;

    return ast;
}

ast_T* parser_parse_var(parser_T* parser, scope_T* scope)
{
    if (parser_debug) printf("debug 1.5 [parser_parse_var]\n");
    char* token_value = parser->current_token->value;
    parser_eat(parser, TOKEN_ID);

    if (parser->current_token->type == TOKEN_LPAREN) return parser_parse_func_call(parser, scope);

    ast_T* ast_var = init_ast(AST_VARIABLES);
    ast_var->variable_name = token_value;

    ast_var->scope = scope;

    return ast_var;
}

ast_T* parser_parse_string(parser_T* parser, scope_T* scope)
{
    if (parser_debug) printf("debug 1.5 [parser_parse_string]\n");
    ast_T* ast_str = init_ast(AST_STRING);
    ast_str->string_value = parser->current_token->value;

    parser_eat(parser, TOKEN_STRING);
    ast_str->scope = scope;
    return ast_str;
}

ast_T* parser_parse_id(parser_T* parser, scope_T* scope)
{
    if (parser_debug) printf("debug 1.5 [parser_parse_id]\n");
    if (strcmp(parser->current_token->value, "let") == 0)
    {   
        return parser_parse_var_def(parser, scope);
    }
    else if (strcmp(parser->current_token->value, "func") == 0)
    {
        return parser_parse_func_def(parser, scope);
    }
    else
    {
        return parser_parse_var(parser, scope);
    }
}