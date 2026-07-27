#ifndef TOKEN_H
#define TOKEN_H

typedef enum 
{
    TOKEN_ID,
    TOKEN_EQUALS,
    TOKEN_STRING,
    TOKEN_SEMIC,
    TOKEN_LPAREN,
    TOKEN_RPAREN,
    TOKEN_DOT,
    TOKEN_COMMA,
    TOKEN_LBRACE,
    TOKEN_RBRACE,
    TOKEN_UDNERSCORE,
    TOKEN_EOF,
} token_type;

typedef struct TOKEN_S
{
    token_type type;
    char* value;
} token_T;


token_T* init_token(int type, char* value);

#endif