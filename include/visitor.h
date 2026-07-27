#ifndef VISITOR_H
#define VISITOR_H
#include "AST.h"
#include "scope.h"
// AST_VAR_DEF,
// AST_VARIABLES,
// AST_FUNCTION_CALL,
// AST_STRING,
// AST_COMPOUND
typedef struct VISITOR_S {} visitor_T;

void debug_visitor();

visitor_T* visitor_init();

ast_T* visitor_visit(visitor_T* visitor, ast_T* node);
ast_T* visitor_visit_var_def(visitor_T* visitor, ast_T* node);
ast_T* visitor_visit_func_def(visitor_T* visitor, ast_T* node);
ast_T* visitor_visit_var(visitor_T* visitor, ast_T* node);
ast_T* visitor_visit_func_call(visitor_T* visitor, ast_T* node);
ast_T* visitor_visit_string(visitor_T* visitor, ast_T* node);
ast_T* visitor_visit_compound(visitor_T* visitor, ast_T* node);
#endif 