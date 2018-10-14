#include <stdio.h>
#include <stdlib.h>
#include <string.h>
void Split(char* string, char* delimeters, char*** tokens, int* tokensCount)
{
    if(string && delimeters && tokens && tokensCount)
    {
        tokensCount[0] = 0;
        char* temp = (char*) calloc (strlen(string), sizeof(char));
        temp = strtok(string, delimeters);
        strcpy(tokens[0][tokensCount[0]], temp);
        while(temp)
        {
            tokens[0][tokensCount[0]] = realloc(tokens[0][tokensCount[0]], strlen(temp) * sizeof(char));
            temp = strtok(NULL, delimeters);
            tokensCount[0] ++;
            if(temp) strcpy(tokens[0][tokensCount[0]], temp);
            else tokens[0][tokensCount[0]] = NULL;
        }
        tokens[0] = realloc (tokens[0], (tokensCount[0] + 1) * sizeof(char*));
        free(temp);
    }
    else printf("\n string can`t  be splitted");
}

