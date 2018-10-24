#include <stdio.h>
#include <stdlib.h>
#include <string.h>
void Split(char* string, char* delimeters, char*** tokens, int* tokensCountPtr)
{
    if(string && delimeters && tokens && tokensCount)
    {
        // FIXIT: 1) tokensCount[0], tokens[0] сбивают с толку. мы обсуждали, что можно сделать так:
        //  void Split(... , int* tokensCountPtr) {
        int tokensCount = 0;
        // ...
        *tokensCountPtr = tokensCount;
        // аналогично для tokens
        // 2) на самом деле вы делаете излишние манипуляции с памятью. прочитайте, пожалуйста, что именно делает
        // strtok ... кажется выделять память под temp не нужно
        // у вас под строку (*tokens)[0] память не была выделена, но вы туда что-то копируете strcpy(tokens[0][tokensCount[0]], temp); (до цикла while)
        // зачем делать реаллоки в цикле, если память выделяется 1 раз для каждого нового токена у вас.
        // Но все же лучший выход прочитать внимательно про strtok и убрать все ненужные выделения памяти
        char* temp = (char*) calloc (strlen(string), sizeof(char));
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
        free(temp);
    }
    else printf("\n string can`t  be splitted");
}

