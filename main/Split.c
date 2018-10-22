#include <stdio.h>
#include <stdlib.h>
#include <string.h>
void Split(char* string, char* delimeters, char*** tokens, int* tokensCount)
{
    if(string && delimeters && tokens && tokensCount)
    {
        // FIXIT: 1) tokensCount[0], tokens[0] сбивают с толку. мы обсуждали, что можно сделать так:
        //  void Split(... , int* tokensCountPtr) {
        // int tokensCount = 0;
        // ...
        // *tokensCountPtr = tokensCount;
        // аналогично для tokens
        // 2) на самом деле вы делаете излишние манипуляции с памятью. прочитайте, пожалуйста, что именно делает
        // strtok ... кажется выделять память под temp не нужно
        // у вас под строку (*tokens)[0] память не была выделена, но вы туда что-то копируете strcpy(tokens[0][tokensCount[0]], temp); (до цикла while)
        // зачем делать реаллоки в цикле, если память выделяется 1 раз для каждого нового токена у вас.
        // Но все же лучший выход прочитать внимательно про strtok и убрать все ненужные выделения памяти.
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

