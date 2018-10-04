#include <stdio.h>
#include <string.h>
#include <stdlib.h>


char* ScanStr(char* a, int buff)
{
    int i = 1;
    fgets(a, buff, stdin);
    while (!((a[strlen(a)-1] == 0) ^ (a[strlen(a)-1] == '\n')))
    {
        ++i;
        a = (char*) realloc (a, (1024 * i) - 1);
        fgets(&a[(buff * (i-1)) - 1], buff, stdin);
    }
    return a;
}


void Split (char* string, char* delimeters, char*** tokens, int* tokensCount)
{
    *tokensCount = 0;
    *tokens = (char**) calloc (strlen (string), sizeof(char*));
    **tokens = (char*) calloc (strlen (string), sizeof(char));
    (*tokens)[(*tokensCount)] = strtok (string, delimeters); //во втором параметре указаны разделители (пробел, запятая, точка)
    while ((*tokens)[(*tokensCount)] != NULL) //пока есть лексемы
    {
        (*tokensCount) ++;
        (*tokens)[(*tokensCount)] = strtok (NULL, delimeters);
    }
}

int main(void)
{
    int buff = 1024;
    char* string;
    string = (char*) malloc (buff);
    string = ScanStr(string, buff);
    char*** tokens;
    tokens = (char***) calloc (1, sizeof(char**));
    char* delimeters;
    delimeters = (char*) malloc (buff);
    delimeters = ScanStr(delimeters, buff);
    int* tokensCount;
    tokensCount = (int*) calloc(1, sizeof(int));
    Split(string, delimeters, tokens, tokensCount);
    for (int i = 0; i < (*tokensCount); ++i)
    {
        printf("%s\n", (*tokens)[i]);
    }
    free(**tokens);
    free(*tokens);
    free(tokens);
    free(delimeters);
    free(string);
    free(tokensCount);
}
