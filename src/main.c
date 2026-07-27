#include <stdio.h>
#include "lexer.h"
#include "token.h"
#include "parser.h"
#include "visitor.h"
#include "io.h"
#include "scope.h"
/* lexer
    |-> parser <- and v
        |->Ast <- and v
            |->visitor <-*/

int main(int argc, char* argv[]) {
    lexer_T* lexer = init_lexer(get_file_contens(argv[1]));

    //debug_parser();
    //debug_visitor();
    //debug_scope();

    parser_T* parser = init_parser(lexer);
    ast_T* root = parser_parse(parser, parser->scope);
    visitor_T* visitor = visitor_init();
    visitor_visit(visitor, root);
    return 0;
}