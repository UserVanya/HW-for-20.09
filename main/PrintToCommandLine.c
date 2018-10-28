#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ScanStr.c"
#include "EnoughTokensAlloc.c"
#include "Split.c"
#include "PrintStringArray.c"
#include "FreeStringArr.c"
#include "FileInit.c"
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
int comparePTCL(const char*** x, const char*** y)
{
    //delivered address &(tokens[i])
    int a = atoi((char*)x[0][0]);
    int b = atoi((char*)y[0][0]);
    return (a - b);
}
void ScanCommandFile (FILE* f, char** lines, int numOfLines, int buff)
{
    for (int i = 0; i < numOfLines; i++)
        {
            lines[i] = (char*) calloc (buff, sizeof(char));
            lines[i] = ScanStr (lines[i], buff, f);
        }
}
void SortTokens(char*** tokens, int numOfLines, int* tokensCount)
{
    qsort(tokens, numOfLines, sizeof(char**), comparePTCL);
    for (int i = 0; i < numOfLines; i++)
    {
        int j = 0;
        while(tokens[i][j])
        {
            j++;
        }
        tokensCount[i] = j;
    }
}

void ForkAndExec (char *** tokens, int numOfLines, int timeout)
{
    pid_t* pid = (pid_t*) calloc (numOfLines + 1, sizeof(pid_t));
    for (int i = 0; i < numOfLines; ++i)
    {
        pid[i] = fork();
        if(pid[i] == 0) //if (child) continue;
        {
            sleep(atoi(tokens[i][0]));
            execvp (tokens[i][1], &(tokens[i][1]));
            printf("execvp failed\n");
            exit(-1);
        }
    }
    for(int i = 0; i < numOfLines; i++)
    {
        if (i == 0) sleep(atoi(tokens[i][0]) + timeout);
        else sleep(atoi(tokens[i][0]) - atoi(tokens[i - 1][0]));
        // kill(..., 1) для 1 есть именованная константа SIGHUP       
        if(kill (pid[i], 1)) printf("process with id %d: runtime error", pid[i]);
    }
    free(pid);
}
void PrintToCommandLine(char* path)
{
    int numOfLines, timeout, buff = 128;
    FILE* f = fopen(path, "r");
    FileInit(f);

    fscanf(f, "%d %d \n", &numOfLines, &timeout);
    char** lines = (char**) calloc (numOfLines, sizeof(char*));
    ScanCommandFile(f, lines, numOfLines, buff);

    fclose(f);
    int* tokensCount = (int*) calloc (numOfLines, sizeof(int));
    char*** tokens = (char***) calloc (numOfLines, sizeof(char**));
    for (int i = 0; i < numOfLines; i++)
    {
        EnoughTokensAlloc(lines[i], &(tokens[i]));
        Split(lines[i], " \n", &(tokens[i]), &(tokensCount[i]));
    }

    SortTokens (tokens, numOfLines, tokensCount);
    ForkAndExec(tokens, numOfLines, timeout);

    for(int i = 0; i < numOfLines; i++) FreeStrArr(tokens[i], tokensCount[i]);
    free(tokens);
    free(tokensCount);
    FreeStrArr(lines, numOfLines);
}

