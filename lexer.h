#include <stdio.h>
#include <stdlib.h>

enum TokenTypes
{
    WG_Number,
    WG_String,
    WG_Variable,
    WG_Keyword,
    WG_Assigenment,
    WG_Operators,
    WG_Comparators,
    WG_Delimiter
};

int eof_reached = 0;

typedef struct
{
    int type;
    void *value;
} Token;

FILE *workfile;

int setWorkFile(FILE *file)
{
    workfile = file;
}

int peekNext()
{
    int chr = fgetc(workfile);
    fseek(workfile, -1, SEEK_CUR);
    return chr;
}

Token parseNum(int curr)
{
    int chr;
    int num = curr;

    do
    {
        chr = fgetc(workfile);
        if (!(chr >= '0' && chr <= '9'))
        {
            fseek(workfile, -1, SEEK_CUR);
            break;
        }
        num = num * 10 + chr - 48;

    } while (chr >= '0' && chr <= '9');

    if (chr == EOF)
        eof_reached = 1;
    Token token = {WG_Number, &num};
    return token;
}

Token parseString(char curr, char delim)
{
    Token token;
    token.type = WG_String;

    long int length = 0;

    do
    {
        curr = fgetc(workfile);

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
    curr = fgetc(workfile);

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
        curr = fgetc(workfile);
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

    printf("%s|hallo\n", (char *) token.value);

    return token;
}

Token requestNextToken()
{

    Token token;
    if (eof_reached)
    {
        token.value = NULL;
        token.type = WG_Delimiter;

        return token;
    }

    int chr;
    chr = fgetc(workfile);

    while (chr == ' ')
    {
        chr = fgetc(workfile);
    }

    if (chr >= '0' && chr <= '9')
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
    else if (chr == '+' || chr == '-' || chr == '*' || chr == '/')
    {
        token.type = WG_Operators;
        int value = chr;
        token.value = &value;
    }
    else if (chr == '=')
    {
        token.type = WG_Assigenment;
        int value = chr;
        token.value = &value;
    }
    else if (chr == ';')
    {
        token.value = NULL;
        token.type = WG_Delimiter;
    }
    else if (chr == EOF)
    {
        token.value = NULL;
        token.type = WG_Delimiter;
    }

    if (peekNext() == EOF)
        eof_reached = 1;
    return token;
}