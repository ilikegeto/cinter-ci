#ifndef PARSER_H
#define PARSER_H
#include "lexer.h"
#include "AST.h"
#include "scope.h"

typedef struct PARSER_S 
{
    lexer_T* lexer;
    token_T* current_token;
    token_T* prev_token;
    scope_T* scope;
} parser_T;
void debug_parser();

parser_T* init_parser(lexer_T* lexer);

void parser_eat(parser_T* parser, int token_ty);
ast_T* parser_parse(parser_T* parser, scope_T* scope);

ast_T* parser_parse_statement(parser_T* parser, scope_T* scope);
ast_T* parser_parse_statements(parser_T* parser, scope_T* scope);

ast_T* parser_parse_expr(parser_T* parser, scope_T* scope);
ast_T* parser_parse_factor(parser_T* parser, scope_T* scope);
ast_T* parser_parse_term(parser_T* parser, scope_T* scope);
ast_T* parser_parse_func_call(parser_T* parser, scope_T* scope);

ast_T* parser_parse_var(parser_T* parser, scope_T* scope);
ast_T* parser_parse_var_def(parser_T* parser, scope_T* scope);
ast_T* parser_parse_func_def(parser_T* parser, scope_T* scope);

ast_T* parser_parse_string(parser_T* parser, scope_T* scope);
ast_T* parser_parse_id(parser_T* parser, scope_T* scope);

#endif