#include <stdio.h>
#include <stdlib.h>
#include <string.h>
void EnoughTokensAlloc(char* string, char*** tokens)
{
    tokens[0] =(char**) calloc(strlen(string), sizeof(char*));
    for(int i = 0; i <(int)strlen(string); ++i)
    {
        tokens[0][i] =(char*) calloc(strlen(string), sizeof(char));
    }
}
