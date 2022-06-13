#include <stdlib.h>
#include <stdio.h>

enum WG_ERRORS {
    WG_Syntax_Error,
    WG_Type_Error,
    WG_Reference_Error,
    WG_NULL_Pointer_Error,
    WG_Gerneric_Error,
    WG_User_Error,
};

void throwError(FILE* file, int line, int col, int type, char* message) {
    int currLine = 1, chr;
    fseek(file, 0, SEEK_SET);

    while(currLine < line){
        chr = getc(file);
        if(chr == '\n') {
            currLine++;
        }
    }    
    printf("%d |", line);

    chr = -2;
    while (chr != '\n' && chr != EOF)
    {
        chr = getc(file);
        putchar(chr);
    }
    
    printf("\n");
    int nnum = line;

    while (nnum)
    {
        putchar(' ');
        nnum = (int) nnum / 10;
    }
    
    for(int i = 0; i < col; i++) {
        putchar(' ');
    }
    printf("^^^\n");

    switch (type)
    {
    case WG_Syntax_Error:
        printf("Syntax Error: ");
        break;
    
    default:
        break;
    }

    printf("%s at line %d, column %d\n", message, line, col);
    exit(0);
}