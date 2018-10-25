#include <stdio.h>
#include <stdlib.h>
#include <string.h>
void Split(char* string, char* delimeters, char*** tokens, int* tokensCountPtr)
{
    if(string && delimeters && tokens && tokensCountPtr)
    {
        int tokensCount = 0;
        char* temp;
        temp = strtok(string, delimeters);
        strcpy((*tokens)[tokensCount], temp);
        while(temp)
        {
            (*tokens)[tokensCount] = realloc((*tokens)[tokensCount], strlen(temp) * sizeof(char));
            temp = strtok(NULL, delimeters);
            tokensCount ++;
            if(temp) strcpy((*tokens)[tokensCount], temp);
            else (*tokens)[tokensCount] = NULL;
        }
        (*tokens) = realloc ((*tokens), (tokensCount + 1) * sizeof(char*));
        *tokensCountPtr = tokensCount;
    }
    else printf("\n string can`t  be splitted");
}
