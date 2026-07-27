#include "lexer.h"
#include "token.h"
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/* Provide minimal token constant fallbacks if token.h doesn't define them. */
/*#ifndef TOKEN_EQUALS
#define TOKEN_EQUALS 1
#endif 
#ifndef TOKEN_SEMIC
#define TOKEN_SEMIC 2
#endif
#ifndef TOKEN_LPAREN
#define TOKEN_LPAREN 3
#endif
#ifndef TOKEN_RPAREN
#define TOKEN_RPAREN 4
#endif*/

lexer_T* init_lexer(char* contents)
{
    lexer_T* lexer = calloc(1, sizeof(struct LEXER_S));
    lexer->contents = contents;
    lexer->i = 0;
    lexer->c = contents[lexer->i];

    return lexer;
}

void lexer_advc(lexer_T* lexer)
{
    if (lexer->c != '\0' && lexer->i < strlen(lexer->contents)) 
    {
        lexer->i += 1;
        lexer->c = lexer->contents[lexer->i];

    }
}
void lexer_skip_ws(lexer_T* lexer)
{
    while (lexer->c == ' ' || lexer->c == 10)
    {
        lexer_advc(lexer);
    }
}

token_T* lexer_get_next_token(lexer_T* lexer)
{
    while (lexer->c != '\0' && lexer->i < strlen(lexer->contents)) 
    {
        if (lexer->c == ' ' || lexer->c == 10) {lexer_skip_ws(lexer); continue;}
        if (isalnum(lexer->c)) return lexer_collect_id(lexer);
        if (lexer->c == '"') return lexer_collect_string(lexer);
        if (isalpha(lexer->c) || lexer->c == '_') return lexer_collect_id(lexer); 
        switch (lexer->c)
        {
            case '=': return lexer_advc_w_token(lexer, init_token(TOKEN_EQUALS, lexer_get_current_char_as_str(lexer))); break;
            case ';': return lexer_advc_w_token(lexer, init_token(TOKEN_SEMIC, lexer_get_current_char_as_str(lexer))); break;
            case '(': return lexer_advc_w_token(lexer, init_token(TOKEN_LPAREN, lexer_get_current_char_as_str(lexer))); break;
            case ')': return lexer_advc_w_token(lexer, init_token(TOKEN_RPAREN, lexer_get_current_char_as_str(lexer))); break;
            case '.': return lexer_advc_w_token(lexer, init_token(TOKEN_DOT, lexer_get_current_char_as_str(lexer))); break;
            case ',': return lexer_advc_w_token(lexer, init_token(TOKEN_COMMA, lexer_get_current_char_as_str(lexer))); break;
            case '{': return lexer_advc_w_token(lexer, init_token(TOKEN_LBRACE, lexer_get_current_char_as_str(lexer))); break;
            case '}': return lexer_advc_w_token(lexer, init_token(TOKEN_RBRACE, lexer_get_current_char_as_str(lexer))); break;
            case '_': return lexer_advc_w_token(lexer, init_token(TOKEN_UDNERSCORE, lexer_get_current_char_as_str(lexer))); break;
            default: lexer_advc(lexer); break;
        }
    }
    return init_token(TOKEN_EOF, "\0");
}
token_T* lexer_collect_string(lexer_T* lexer)
{
    lexer_advc(lexer);
    char* value = calloc(1, sizeof(char));
    value[0] = '\0';
    while (lexer->c != '"')
    {
        char* s = lexer_get_current_char_as_str(lexer);
        value = realloc(value, (strlen(value) + strlen(s) + 1) * sizeof(char));
        strcat(value, s);
        lexer_advc(lexer);
    }
    lexer_advc(lexer);
    return init_token(TOKEN_STRING, value);
}

token_T* lexer_collect_id(lexer_T* lexer)
{
    char* value = calloc(1, sizeof(char));
    value[0] = '\0';
    while (isalnum(lexer->c) || lexer->c == '_' || lexer->c == '.')
    {
        char* s = lexer_get_current_char_as_str(lexer);
        value = realloc(value, (strlen(value) + strlen(s) + 1) * sizeof(char));
        strcat(value, s);
        lexer_advc(lexer);
    }
    ///lexer_advc(lexer);
    return init_token(TOKEN_ID, value);
}
token_T* lexer_advc_w_token(lexer_T* lexer, token_T* token)
{
    lexer_advc(lexer);
    return token;
}
char* lexer_get_current_char_as_str(lexer_T* lexer)
{
    char* str = calloc(2, sizeof(char));
    str[0] = lexer->c;
    str[1] = '\0';

    return str;
}