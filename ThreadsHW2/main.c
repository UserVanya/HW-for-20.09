#include <stdio.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

// FIXIT: 1) не следует использовать транслитерацию. 
// 2) называйте структуры в одном стиле for_one_thread -> ForOneThread
// а дальше уже typedef struct ForOneThread ForOneThread;
// 3) стоит в комментарии указать результаты замеров ускорения. Нужно измерять wall-clock time
// https://stackoverflow.com/questions/17432502/how-can-i-measure-cpu-time-and-wall-clock-time-on-both-linux-windows

#define MNOGO 100000000
struct for_one_thread
{
    int* begin;
    int* end;
    int summ;
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
typedef struct for_one_thread ForOneThread;

void* AvarageValue(void* dummy)
{
    // FIXIT: можно первой строкой написать ForOneThread* oneThreadTask = (ForOneThread*)dummy;
    // а дальше не кастовать везде dummy к нужному типу
    
    int amountOfElements = (int)((((ForOneThread*)dummy)->end) - (((ForOneThread*)dummy)->begin));
    ((ForOneThread*)dummy)->summ = 0;
    for(int i = 0; i < amountOfElements; i++) ((ForOneThread*)dummy)->summ = ((ForOneThread*)dummy)->summ + (((ForOneThread*)dummy)->begin)[i];
    return NULL;
}
void* Dispersia(void* dummy)
{
    double temp = 0;
    int amountOfElements = (int)((((ForOneThread*)dummy)->end) - (((ForOneThread*)dummy)->begin));
    for (int i = 0; i < amountOfElements; i++)
    {
        double deviation = ((((ForOneThread*)dummy)->begin)[i] - ((ForOneThread*)dummy)->avarageValue); //deviation - отклонение
        temp = temp + deviation * deviation;
    }
    ((ForOneThread*)dummy)->dispersia = temp;
    return NULL;
}
double CalculateDispersia(int numOfThreads, ForOneThread* thidStruct, int numForOneThread, int* mass, pthread_t* thid)
{
    int begin = 0, end = 0;
    double temp = 0;
    
    // FIXIT: следующий ниже цикл дублируется дважды в коде. его стоит вынести в отдельную ф-ю
    for (int i = 0; i < numOfThreads; i++)
    {
        if (i != numOfThreads) end = end + numForOneThread;
        else end = MNOGO - 1;
        thidStruct[i].begin = &(mass[begin]);
        thidStruct[i].end = &(mass[end - 1]);
        begin = begin + numForOneThread;
        ThreadInit(pthread_create(&(thid[i]), (pthread_attr_t*)NULL, Dispersia, (void*)(&(thidStruct[i]))));
    }
    for (int i = 0; i < numOfThreads; i++)
    {
        pthread_join(thid[i], (void **)NULL);
        temp = temp + thidStruct[i].dispersia;
    }
    return (temp / (double)MNOGO);
}
double CalculateAvarageValue(int numOfThreads, ForOneThread* thidStruct, int numForOneThread, int* mass, pthread_t* thid)
{
    double avarageValue;
    int begin = 0, end = 0;
    for (int i = 0; i < numOfThreads; i++)
    {
        if (i != numOfThreads) end = end + numForOneThread;
        else end = MNOGO - 1;
        thidStruct[i].begin = &(mass[begin]);
        thidStruct[i].end = &(mass[end - 1]);
        begin = begin + numForOneThread;
        ThreadInit(pthread_create(&(thid[i]), (pthread_attr_t*)NULL, AvarageValue, (void*)(&(thidStruct[i]))));
    }
    
    // FIXIT: ничего не препятствует назвать переменную просто sum. так и стоит поступить.
    int summ = 0;
    for (int i = 0; i < numOfThreads; i++)
    {
        pthread_join(thid[i], (void **)NULL);
        summ = summ + thidStruct[i].summ;
    }
    avarageValue = (double)summ / MNOGO;
    for (int i = 0; i < numOfThreads; i++) thidStruct[i].avarageValue = avarageValue;
    return avarageValue;
}
int main()
{
    int * mass = (int*) malloc(MNOGO * sizeof(int));
    int numOfThreads;
    scanf("%d", &numOfThreads);
    int numForOneThread;


    ForOneThread* thidStruct = (ForOneThread*) calloc (numOfThreads, sizeof(ForOneThread));
    pthread_t* thid = (pthread_t*) calloc(numOfThreads, sizeof(pthread_t));
    //randomly fill the array
    for (int i = 0; i < MNOGO; i++)
    {
        mass[i] = rand()%2000;
    }
    numForOneThread = MNOGO / numOfThreads;
    //Let`s go:)

    double avarageValue;
    avarageValue = CalculateAvarageValue(numOfThreads, thidStruct, numForOneThread, mass, thid);
    printf("%lf\n", avarageValue);

    double dispersia;
    dispersia = CalculateDispersia(numOfThreads, thidStruct, numForOneThread, mass, thid);
    printf("\n%lf\n", dispersia);
    return 0;
}



