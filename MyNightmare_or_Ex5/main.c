#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include <sys/wait.h>

// FIXIT: программа без fork'ов работать не сможет + предполагается, что ровно 4 строки считаются.
// Давайте на семинаре обсудим ваше решение.

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
char* ScanStr(char* a, int buff)
{
    int i = 1;
    fgets(a, buff, stdin);
    while (!((a[strlen(a) - 1] == 0) ^ (a[strlen(a) - 1] == '\n')))
    {
        ++i;
        a = (char*) realloc (a, (1024 * i) - 1);
        fgets(&a[(buff * (i - 1)) - 1], buff, stdin);
    }
    return a;
}
char* ScanStrF(char* a, int buff, FILE* f)
{
    int i = 1;
    fgets(a, buff, f);
    while (!((a[strlen(a) - 1] == 0) ^ (a[strlen(a) - 1] == '\n')))
    {
        ++i;
        a = (char*) realloc (a, (1024 * i) - 1);
        fgets(&a[(buff * (i - 1)) - 1], buff, f);
    }
    return a;
}
int main(void)
{
    // Считывание строк файла и запись в переменные
    FILE* f = fopen("/home/ivan/Programming/MyNighmare_or_Ex5/input", "r");
    if (f == NULL)
    {
        printf ("ошибка\n");
        return -1;
    }
    char** arrOfRows;
    int buff = 1024;
    arrOfRows = (char**) calloc (4, sizeof(char*));
    for (int i = 0; i < 4; ++i)
    {
       arrOfRows[i] = (char*) calloc (buff, sizeof(char));
       arrOfRows[i] = ScanStrF(arrOfRows[i], buff, f);
       printf("%s\n", arrOfRows[i]);
    }
    //разделение каждой из строк в массив слов с пробелами-разделителями
    char*** tokens;
    tokens = (char***) calloc (4, sizeof(char**));
    char** delimeters;
    delimeters = (char**) calloc (4, sizeof(char*));
    int** tokensCount;
    tokensCount = (int**) calloc(4, sizeof(int*));
    for (int i = 0; i < 4; ++i)
    {
        delimeters[i] = (char*) calloc (buff, sizeof(char));
        printf("Enter delimeters in %d line:\n", i);
        delimeters[i] = ScanStr(delimeters[i], buff);
        tokensCount[i] = (int*) calloc(1, sizeof(int));
        Split(arrOfRows[i], delimeters[i], &tokens[i], tokensCount[i]);
    }
    for (int i = 0; i < 4; ++i)
    {
        for(int j = 0; j < tokensCount[i][0]; j++)
        {
            printf("%s\n", tokens[i][j]);
        }
        printf("\n");
    }
    printf("parent id is %d\n\n", getpid());
    pid_t pid[5] = {getpid(), 1, 1, 1, 1};
    for (int i = 0; i < 4; ++i)
    {
        if (pid[i] != 1)
        {
            if (pid[i] == 0)
            {
                execvp (tokens[i][0], tokens[i]);
            }
            else
            {
                sleep(atoi(tokens[0][0]));
                if (i != 0)
                kill(pid[i], 1);
                if (i != 4)
                pid[i + 1] = fork();
            }
        }
    }
    fclose(f);
    free(*arrOfRows);
    free(arrOfRows);
    free(**tokens);
    free(*tokens);
    free(tokens);
    free(*delimeters);
    free(delimeters);
    free(*tokensCount);
    free(tokensCount);
}
