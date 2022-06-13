#include "lexer.h"
#include "throwerror.h"
#include <stdio.h>

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

typedef union Toke_AST
{
    WG_AST * val1;
    Token * val2;
} Toke_AST;


WG_AST * parseNext(Toke_AST * prev, int delim, int depth) {
    Token curr = requestNextToken();

    if(curr.type == WG_String || curr.type == WG_Number || curr.type == WG_Variable) {
        Toke_AST toke;
        toke.val2 = &curr;
        return parseNext(&toke, delim, depth);
    } else if(curr.type == WG_Assignment) {
        if(!prev) {
            
        }
        curr = requestNextToken();
        while (curr.type != WG_Delimiter)
        {
            curr = requestNextToken();
            if(curr.type == WG_Number) {
                printf("%d\n", (double *) curr.value);
            }
        }
    }
}


void parse(FILE *file)
{
    setWorkFile(file);
    parseNext(NULL, ';', 0);
}