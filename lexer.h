#include <stdio.h>
#include <stdlib.h>
#include "stack.h"
#include "throwerror.h"

enum TokenTypes
{
    WG_Number,
    WG_String,
    WG_Variable,
    WG_Keyword,
    WG_Assignment,
    WG_Operators,
    WG_Comparators,
    WG_Delimiter,
    WG_Bracket,
    WG_Comment
};

int eof_reached = 0;
Stack * bracket_stack;

typedef struct
{
    int type;
    int col;
    int line;
    int extra;
    void *value;
} Token;

FILE *workfile;
int line;
int col;

int setWorkFile(FILE *file)
{
    workfile = file;
    line = 1;
    col = 0;

    bracket_stack = createStack(2);
}

int peekNext()
{
    int chr = getc(workfile);
    fseek(workfile, -1, SEEK_CUR);
    return chr;
}

Token parseNum(int curr)
{
    int chr;
    int num = curr;

    do
    {
        chr = getc(workfile);
        if (!(chr >= '0' && chr <= '9'))
        {
            fseek(workfile, -1, SEEK_CUR);
            break;
        }
        num = num * 10 + chr - 48;

    } while (chr >= '0' && chr <= '9');

    if (chr == EOF)
        eof_reached = 1;
    Token token = (Token) { WG_Number, line, col, 0, &num };

    return token;
}

Token parseString(char curr, char delim)
{
    Token token;
    token.type = WG_String;

    long int length = 0;

    do
    {
        curr = getc(workfile);

        if (curr == EOF)
        {
            exit(1);
        }

        length++;

        if (curr == delim)
        {
            fseek(workfile, -length, SEEK_CUR);
            length--;

            token.value = malloc(sizeof(unsigned char) * length);
            fread(token.value, 1, length, workfile);
        }
    } while (curr != delim);
    curr = getc(workfile);

    if (curr == EOF)
        eof_reached = 1;
    return token;
}

Token parseName(char curr)
{
    Token token;
    token.type = WG_Variable;
    long int length = 1;

    do
    {
        curr = getc(workfile);
        length++;
        if (!(curr >= 'a' && curr <= 'z') || (curr >= 'A' && curr <= 'Z'))
        {
            if(curr == EOF) {
                length--;
            }

            fseek(workfile, -length, SEEK_CUR);

            if(curr != EOF) {
                length--;
            }

            token.value = malloc(sizeof(unsigned char) * length);
            fread(token.value, 1, length, workfile);
        }

    } while ((curr >= 'a' && curr <= 'z') || (curr >= 'A' && curr <= 'Z'));

    if (curr == EOF)
        eof_reached = 1;

    return token;
}

Token requestNextToken()
{

    Token token;
    if (eof_reached)
    {
        if(bracket_stack->length != 0) {
            throwError(workfile, line, col, WG_Syntax_Error, "Unmatched parathesis");
            exit(1);
        }
        token.value = NULL;
        token.type = WG_Delimiter;

        deleteStack(bracket_stack);
        return token;
    }

    int chr;
    chr = getc(workfile);
    col++;

    while (chr == ' ')
    {
        chr = getc(workfile);
        col++;
    }

    token.line = line;
    token.col = col;
        
    if(chr == '/') {
        if(peekNext() == '*') {
            fseek(workfile, 2, SEEK_CUR);
            int end = 1;
            while(end) {
                chr = getc(workfile);
                col++;
                if(chr == '*') {
                    if(peekNext() == '/') {
                        end = 0;
                    }
                } else if(chr == '\n') {
                    line++;
                    col = 0;
                } else if(chr == EOF) {
                    throwError(workfile, line, col, WG_Syntax_Error, "Reached EOF while reading a comment");
                }
            }
            token.value = NULL;
            token.type = WG_Comment;
        } else {
            token.type = WG_Operators;
            int value = chr;
            token.value = &value;
        }
    } else if (chr >= '0' && chr <= '9')
    {
        token = parseNum(chr - 48);
    }
    else if (chr == '"' || chr == '\'')
    {
        token = parseString(chr, chr);
    }
    else if ((chr >= 'a' && chr <= 'z') || (chr >= 'A' && chr <= 'Z'))
    {
        token = parseName(chr);
    }
    else if (chr == '+' || chr == '-' || chr == '*')
    {
        token.type = WG_Operators;
        int value = chr;
        token.value = &value;
    }
    else if (chr == '=')
    {
        token.type = WG_Assignment;
        int value = chr;
        token.value = &value;
        
    } else if(chr == '(' || chr == '[' || chr == '{') {
        token.type = WG_Bracket;
        int value = chr;
        token.value = &value;
    
        pushStack(bracket_stack, chr);
    } else if(chr == ')' || chr == ']' || chr == '}') {
        int throw_error = 0;
        peekStack(bracket_stack);

        if(bracket_stack->length == 0) {
            throw_error = 1;
        }
        else if(chr == ')') {
            if(peekStack(bracket_stack) == '(') {
                popStack(bracket_stack);
            } else throw_error = 1;
        } else if(peekStack(bracket_stack) == chr - 2) {
            popStack(bracket_stack);
        } else throw_error = 1;

        if(throw_error == 1) {
            throwError(workfile, token.line, token.col, WG_Syntax_Error, "Unmatched parathesis");
            exit(1);
        }
        token.type = WG_Bracket;
        int value = chr;
        token.value = &value;
    } 
    else if (chr == ';')
    {
        
        if(bracket_stack->length != 0) {
            printf("Unmatched parathesis, at line %d, column %d\n", line, col);
            exit(1);
        }

        token.value = NULL;
        token.type = WG_Delimiter;
    }
    else if (chr == EOF)
    {
        if(bracket_stack->length != 0) {
            printf("Unmatched parathesis, at line %d, column %d\n", line, col);
            deleteStack(bracket_stack);
            exit(1);
        }
        deleteStack(bracket_stack);

        token.value = NULL;
        token.type = WG_Delimiter;
    }

    if (peekNext() == EOF)
        eof_reached = 1;

    return token;
}