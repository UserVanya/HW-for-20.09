#include <stdio.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#define AMOUNT_OF_ELEMENTS 100000000
struct ForOneThread
{
    int* begin;
    int* end;
    int sum;
    double avarageValue;
    double dispersia;
};

void ThreadInit (int result)
{
    if(result != 0)
    {
        printf ("Error on thread create, return value = %d\n", result);
        exit(-1);
    }
}
typedef struct ForOneThread ForOneThread;

void ThreadsInit(int numOfThreads, int numForOneThread, ForOneThread* thidStruct, int* mass, pthread_t* thid, void* (*FuncToDo)(void*))
{
    int begin = 0, end = 0;
    for (int i = 0; i < numOfThreads; i++)
    {
        if (i != numOfThreads) end = end + numForOneThread;
        else end = AMOUNT_OF_ELEMENTS - 1;
        thidStruct[i].begin = &(mass[begin]);
        thidStruct[i].end = &(mass[end - 1]);
        begin = begin + numForOneThread;
        ThreadInit(pthread_create(&(thid[i]), (pthread_attr_t*)NULL, FuncToDo, (void*)(&(thidStruct[i]))));
    }
}

void* AvarageValue(void* dummy)
{
    ForOneThread* oneThreadTask = (ForOneThread*)dummy;
    int amountOfElements = (int)(((oneThreadTask)->end) - ((oneThreadTask)->begin));
    (oneThreadTask)->sum = 0;
    for(int i = 0; i < amountOfElements; i++) (oneThreadTask)->sum = (oneThreadTask)->sum + ((oneThreadTask)->begin)[i];
    return NULL;
}

void* Dispersia(void* dummy)
{
    ForOneThread* oneThreadTask = (ForOneThread*)dummy;
    double temp = 0;
    int amountOfElements = (int)(((oneThreadTask)->end) - ((oneThreadTask)->begin));
    for (int i = 0; i < amountOfElements; i++)
    {
        double deviation = (((oneThreadTask)->begin)[i] - (oneThreadTask)->avarageValue); //deviation - отклонение
        temp = temp + deviation * deviation;
    }
    (oneThreadTask)->dispersia = temp;
    return NULL;
}
double CalculateDispersia(int numOfThreads, ForOneThread* thidStruct, int numForOneThread, int* mass, pthread_t* thid)
{
    double temp = 0;

    ThreadsInit(numOfThreads, numForOneThread, thidStruct, mass, thid, Dispersia);

    for (int i = 0; i < numOfThreads; i++)
    {
        pthread_join(thid[i], (void **)NULL);
        temp = temp + thidStruct[i].dispersia;
    }
    return (temp / (double)AMOUNT_OF_ELEMENTS);
}
double CalculateAvarageValue(int numOfThreads, ForOneThread* thidStruct, int numForOneThread, int* mass, pthread_t* thid)
{
    double avarageValue;

    ThreadsInit(numOfThreads, numForOneThread, thidStruct, mass, thid, AvarageValue);
    
    int sum = 0;
    for (int i = 0; i < numOfThreads; i++)
    {
        pthread_join(thid[i], (void **)NULL);
        sum = sum + thidStruct[i].sum;
    }
    avarageValue = (double)sum / AMOUNT_OF_ELEMENTS;
    for (int i = 0; i < numOfThreads; i++) thidStruct[i].avarageValue = avarageValue;
    return avarageValue;
}
int main()
{
    int * mass = (int*) malloc(AMOUNT_OF_ELEMENTS * sizeof(int));
    int numOfThreads;
    printf("Enter the number of threads:");
    scanf("%d", &numOfThreads);
    int numForOneThread;

    ForOneThread* thidStruct = (ForOneThread*) calloc (numOfThreads, sizeof(ForOneThread));
    pthread_t* thid = (pthread_t*) calloc(numOfThreads, sizeof(pthread_t));
    //randomly fill the array
    for (int i = 0; i < AMOUNT_OF_ELEMENTS; i++) mass[i] = rand()%2000;
    numForOneThread = AMOUNT_OF_ELEMENTS / numOfThreads;

    //Let`s go:)

    double avarageValue;
    avarageValue = CalculateAvarageValue(numOfThreads, thidStruct, numForOneThread, mass, thid);
    printf("\navarageValue = %lf\n\n", avarageValue);


    double dispersia;

    double start = time(NULL);
    for (int i = 0; i < 10; i++) dispersia = CalculateDispersia(numOfThreads, thidStruct, numForOneThread, mass, thid);
    double end = time(NULL);

    printf("Took %lf seconds to execute it\n", difftime(end, start)/10);
    printf("\ndispersia = %lf\n\n", dispersia);

    return 0;
}
