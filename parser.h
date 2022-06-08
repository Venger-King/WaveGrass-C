#include "lexer.h"
#include <stdio.h>

#define FALSE 0
#define TRUE 1

int needMore = TRUE;

void parse(FILE * file) {
    setWorkFile(file);

    Token token;
    while(needMore) {
        token = requestNextToken();
        if(token.type == WG_Number) {
            printf("%d\n", * (double *) token.value);
        } else {
            printf("%s\n", (char *) token.value);
        }
        if(token.type == WG_Delimiter) needMore = FALSE;
    }
}