#ifndef LEXER_H
#define LEXER_H
#include "token.h"
typedef struct LEXER_S
{
    char c;
    unsigned int i;
    char* contents;
} lexer_T;
lexer_T* init_lexer(char* contents);

void lexer_advc(lexer_T* lexer);
void lexer_skip_ws(lexer_T* lexer);

token_T* lexer_get_next_token(lexer_T* lexer);
token_T* lexer_collect_string(lexer_T* lexer);
token_T* lexer_collect_id(lexer_T* lexer);
token_T* lexer_advc_w_token(lexer_T* lexer, token_T* token);

char* lexer_get_current_char_as_str(lexer_T* lexer);
#endif