#include "lexer.h"
#include <stdio.h>

#define FALSE 0
#define TRUE 1

int needMore = TRUE;

typedef struct WG_AST
{
    int operation;
    union 
    {
        struct WG_AST * lhs;
        int lhs1;
        char * lhs2;
    };
    union
    {
        struct WG_AST * rhs;
        int rhs1;
        char * rhs2;
    };
} WG_AST;

WG_AST * parseNext(Token curr, Token prev, int delim, int depth) {

}


void parse(FILE *file)
{
    setWorkFile(file);

    Token token;
    while (needMore)
    {
        token = requestNextToken();
        if (token.type == WG_Number)
        {
            printf("%d\n", * (double *) token.value);
        }
        else
        {
            printf("%s\n", (char *) token.value);
        }
        if (token.type == WG_Delimiter)
            needMore = FALSE;
    }
}