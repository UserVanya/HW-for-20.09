#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <unistd.h>
#include <time.h>

#define IDENTIAL 0
#define TABLE_LIMIT 10
#define BUFF 1024
#define SMALLBUFF 16

void CheckForReadingError(FILE* f)
{
    if(ferror(f))
    {
        perror("File reading error");
        exit(-1);
    }
}
char* ScanStr(char* a, int buff, FILE* f)
{
    int lastSymbol = 0;
    int i = 1;

    if(feof(f))
    {
        perror("Program reached EOF");
        return NULL;
    }
    CheckForReadingError(f);

    fgets(a, buff, f);
    if(feof(f))
    {
        perror("Program reached EOF");
        return NULL;
    }
    CheckForReadingError(f);

    // fgets saving last symbol for \0, that is why if variable 'a' isn`t fully scanned it consists only 9 elements
    // The last symbol can`t  be '\n'.

    lastSymbol = strlen(a) - 1;

    while(a[lastSymbol] != '\n')
    {
        i++;
        a =(char*) realloc(a, buff * i * sizeof(char));
        fgets(a + strlen(a), buff, f);

        if(feof(f))
        {
            perror("Program reached EOF");
            return NULL;
        }
        CheckForReadingError(f);

        lastSymbol = strlen(a) - 1;
    }

    // allocating exact memory block
    a =(char*) realloc(a, strlen(a) * sizeof(char));

    return a;
}

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

int ScanFileAndReturnNumberOfLines (char*** lines, FILE* f)
{
    int i = 1;
    for(i = 0; 1; i++)
    {
        *lines = (char**) realloc(*lines, (i + 1) * sizeof(char*));
        (*lines)[i] = (char*) calloc (BUFF, sizeof(char));
        (*lines)[i] = ScanStr((*lines)[i], BUFF, f);
        if((*lines)[i] == NULL) return i;
    }
}
void SplitTimeAndType (int amountOfItems, char** lines, char*** tokens, int* tokensCount, int* timeForSuchWare, char** typeOfItem)
{
    for (int i = 0; i < amountOfItems; i++)
    {
        Split(lines[i], ":\n", &(tokens[i]), tokensCount);
        if (*tokensCount > 2) printf("error in line %d", i+1);
        timeForSuchWare[i] = atoi(tokens[i][1]);
        typeOfItem[i] = tokens[i][0];
    }
    return;
}
void SplitAmountAndType (int amountOfItems, char** lines, char*** tokens, int* tokensCount, int* amountOfItemsOfSuchType, char** typeOfItem)
{
    for (int i = 0; i < amountOfItems; i++)
    {
        Split(lines[i], ":\n", &(tokens[i]), tokensCount);
        if (*tokensCount > 2) printf("error in line %d", i+1);
        amountOfItemsOfSuchType[i] = atoi(tokens[i][1]);
        typeOfItem[i] = tokens[i][0];
    }
    return;
}
void FileInit (FILE* f)
{
    if(!f)
    {
        fclose(f);
        printf("Error. Can`t find file");
        return;
    }
}
void PrintStrArr (char** str, int n)
{
    printf ("\nString array:\n\n");
    for (int i = 0; i < n; ++i)
    {
        printf ("%d string is:%s\n", i, str[i]);
    }
    printf ("\n");
}
void KeyCheck (key_t key)
{
    if(key < 0)
    {
        printf("Can\'t generate key\n");
        exit(-1);
    }
}
void SemgetCheck(int semid)
{
    if (semid < 0)
    {
        printf("Can\'t get semid\n");
        exit(-1);
    }
}
void SemInit(int amountOfSemaphors, struct sembuf* mybuf)
{
    for (int i = 0; i < amountOfSemaphors; ++i)
    {
        mybuf->sem_op = 0;
        mybuf->sem_num = i;
        mybuf->sem_flg = 0;
    }
}
void SemopCheck (int result)
{
    if (result < 0)
    {
        printf("Can`t wait for condition\n");
        exit(-1);
    }
}

void Ai(int semid, int amountOfSemaphors, int i, struct sembuf* mybuf, int n)
{
    mybuf[i].sem_op = n;
    printf("A\n");
    SemopCheck(semop(semid, mybuf, amountOfSemaphors));
}
void Di(int semid, int amountOfSemaphors, int i, struct sembuf* mybuf, int n)
{
    mybuf[i].sem_op = (-1) * n;
    printf("D\n");
    SemopCheck(semop(semid, mybuf, amountOfSemaphors));
}
void Zi(int semid, int amountOfSemaphors, int i, struct sembuf* mybuf)
{
    mybuf[i].sem_op = 0;
    SemopCheck(semop(semid, mybuf, amountOfSemaphors));
}
void EnoughTokensAlloc(char* string, char*** tokens)
{
    (*tokens) =(char**) calloc(strlen(string), sizeof(char*));
    for(int i = 0; i <(int)strlen(string); ++i) (*tokens)[i] =(char*) calloc(strlen(string), sizeof(char));
}
void FreeStrArr(char** arr, int n)
{
    for(int i = 0; i < n; i++)free(arr[i]);
    free(arr);
}
void MatchItemsAndTimesOfTypes(char** typeOfItem, int* timeForSuchWare, char** typeOfWare, int* timesForSuchTypeOfItem,
                               int amountOfTypesOfItems, int amountOfTypesOfWare)
{
    for(int i = 0; i < amountOfTypesOfItems; i++)
    {
        for(int j = 0; j < amountOfTypesOfWare; j++)
        {
            if(strcmp(typeOfItem[i], typeOfWare[j]) == IDENTIAL) timesForSuchTypeOfItem[i] = timeForSuchWare[j];
        }
    }
}
void ShmgetCheck (int result)
{
    if(result < 0)
    {
        printf("Can\'t create shared memory\n");
        exit(-1);
    }
}
void ShmatCheck (int* result)
{
    if(result == (int *)(-1))
    {
        printf("Can't attach shared memory\n");
        exit(-1);
    }
}
int main()
{
    int tokensCount;

    FILE* wareToWash = fopen("/home/ivan/Programming/Washer/Ware", "r");
    FileInit(wareToWash);
    char** lines = (char**) calloc(SMALLBUFF, sizeof(char*));
    int amountOfTypesOfItems = ScanFileAndReturnNumberOfLines(&lines, wareToWash);

    char*** tokensOne = (char***)calloc(amountOfTypesOfItems, sizeof(char**));
    for (int i = 0; i < amountOfTypesOfItems; ++i) EnoughTokensAlloc(lines[i], &(tokensOne[i]));
    char** typeOfItem = (char**)calloc(amountOfTypesOfItems, sizeof(char*));
    int* amountOfItemsOfSuchType  = (int*)calloc(amountOfTypesOfItems, sizeof(int));
    SplitAmountAndType(amountOfTypesOfItems, lines, tokensOne, &tokensCount, amountOfItemsOfSuchType, typeOfItem);

    FILE* timeToWashForEachType = fopen("/home/ivan/Programming/Washer/TimeToWashForEachType", "r");
    FileInit(timeToWashForEachType);
    int amountOfTypesOfWare = ScanFileAndReturnNumberOfLines(&lines, timeToWashForEachType);

    char*** tokensTwo = (char***)calloc(amountOfTypesOfWare, sizeof(char**));
    for (int i = 0; i < amountOfTypesOfWare; ++i) EnoughTokensAlloc(lines[i], &(tokensTwo[i]));
    char** typeOfWare = (char**)calloc(amountOfTypesOfWare, sizeof(char*));
    int* timeForSuchWare  = (int*)calloc(amountOfTypesOfWare, sizeof(int));
    SplitTimeAndType(amountOfTypesOfWare, lines, tokensTwo, &tokensCount, timeForSuchWare, typeOfWare);

    int* timesForSuchTypeOfItem = (int*)calloc(amountOfTypesOfItems, sizeof(int));
    MatchItemsAndTimesOfTypes(typeOfItem, timeForSuchWare, typeOfWare, timesForSuchTypeOfItem, amountOfTypesOfItems, amountOfTypesOfWare);



    int* numberOfItemsOnTable;
    int shmid;
    key_t fkey;
    KeyCheck(fkey = ftok("FileToMakeKeys", 0));
    ShmgetCheck(shmid = shmget(fkey, sizeof(int), 0666|IPC_CREAT));
    ShmatCheck(numberOfItemsOnTable = (int*)shmat(shmid, NULL, 0));
    *numberOfItemsOnTable = 0;
    key_t skey;
    int semid;
    struct sembuf mybuf;
    KeyCheck(skey = ftok("FileToMakeKeys", 1));
    SemgetCheck(semid = semget(skey, 1, 0666 | IPC_CREAT));
    SemInit(1, &mybuf);
    Ai(semid, 1, 0, &mybuf, 1);
    int start = time(NULL);
    for (int i = 0; i < amountOfTypesOfItems; i++)
    {
        for(int j = 0; j < amountOfItemsOfSuchType[i]; j++)
        {
            if(*numberOfItemsOnTable < TABLE_LIMIT)
            {
		printf("%d\n", *numberOfItemsOnTable);
                sleep(timesForSuchTypeOfItem[i]);
                int end = time(NULL);
                printf("Washed %s, washing tooked %f seconds\n", typeOfItem[i], difftime(end, start));
                Di(semid, 1, 0, &mybuf, 1);
                (*numberOfItemsOnTable)++;
                Ai(semid, 1, 0, &mybuf, 1);
            }
            else j--;
        }
    }
    printf("process ended successfully\n");
    return 0;
}
