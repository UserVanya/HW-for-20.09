#include <stdio.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

// FIXIT:
// 2) называйте структуры в одном стиле for_one_thread -> ForOneThread
// Re: Я вообщем-то так и делал, тут всего одна структура. Если вы имеете ввиду общий стиль кода, т.е. я должен называть 
// структуры в том же стиле что и функции, то ок. Но это не понятно из контекста, логично делать такое же разделение помоему, ну
// т.е. условно переменные называть в одном стиле, функции в другом и соответственно структуры в третьем, уточните пожалуйста.
// Вообще я согласен, пока поменяю так, как думаю правильнее...
// а дальше уже typedef struct ForOneThread ForOneThread;
// 3) стоит в комментарии указать результаты замеров ускорения. Нужно измерять wall-clock time
// https://stackoverflow.com/questions/17432502/how-can-i-measure-cpu-time-and-wall-clock-time-on-both-linux-windows
// Я это сделал, и с вами об этом говорил, реализацию с замерами времени не вижу смысла сюда заносить, я могу показать 
// получившийся график

#define AMOUNT_OF_ELEMENTS 100000000

// Если вы хотите называть структуры именно в таком стиле, то все ф-и надо называть в том же: thread_init
// Вы же сами на семинаре говорили, что стиль ForOneThread, ThreadInit и т.п. вам больше нравится и вы все под него переделаете.
// Уточнил :)
struct for_one_thread
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
typedef struct for_one_thread for_one_thread;

void* AvarageValue(void* dummy)
{
    // FIXIT: можно первой строкой написать 
    // for_one_thread* oneThreadTask = (for_one_thread*)dummy;
    // int amountOfElements = (int)(oneThreadTask->end - oneThreadTask->begin);
    // а дальше не кастовать везде dummy к нужному типу
    //????
    // Re: см. пример использования выше
    
    int amountOfElements = (int)((((for_one_thread*)dummy)->end) - (((for_one_thread*)dummy)->begin));
    ((for_one_thread*)dummy)->sum = 0;
    for(int i = 0; i < amountOfElements; i++) ((for_one_thread*)dummy)->sum = ((for_one_thread*)dummy)->sum + (((for_one_thread*)dummy)->begin)[i];
    return NULL;
}
void* Dispersia(void* dummy)
{
    double temp = 0;
    int amountOfElements = (int)((((for_one_thread*)dummy)->end) - (((for_one_thread*)dummy)->begin));
    for (int i = 0; i < amountOfElements; i++)
    {
        double deviation = ((((for_one_thread*)dummy)->begin)[i] - ((for_one_thread*)dummy)->avarageValue); //deviation - отклонение
        temp = temp + deviation * deviation;
    }
    ((for_one_thread*)dummy)->dispersia = temp;
    return NULL;
}
double CalculateDispersia(int numOfThreads, for_one_thread* thidStruct, int numForOneThread, int* mass, pthread_t* thid)
{
    int begin = 0, end = 0;
    double temp = 0;
    
    // FIXIT: следующий ниже цикл дублируется дважды в коде. его стоит вынести в отдельную ф-ю
    // Честно, сам думал об этом, но я не помню как функции передавать в функцию, просто как видите там в конце фора с этим трабл.
    // Re: Давайте на семинаре покажу, как это делается. Можете сами заранее посмотреть презентацию про потоки - 
    // там в первой части было упражнение, где писали свою ф-ю сортировки и в качестве параметра передавали функцию-компаратор.
    for (int i = 0; i < numOfThreads; i++)
    {
        if (i != numOfThreads) end = end + numForOneThread;
        else end = AMOUNT_OF_ELEMENTS - 1;
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
    return (temp / (double)AMOUNT_OF_ELEMENTS);
}
double CalculateAvarageValue(int numOfThreads, for_one_thread* thidStruct, int numForOneThread, int* mass, pthread_t* thid)
{
    double avarageValue;
    int begin = 0, end = 0;
    for (int i = 0; i < numOfThreads; i++)
    {
        if (i != numOfThreads) end = end + numForOneThread;
        else end = AMOUNT_OF_ELEMENTS - 1;
        thidStruct[i].begin = &(mass[begin]);
        thidStruct[i].end = &(mass[end - 1]);
        begin = begin + numForOneThread;
        ThreadInit(pthread_create(&(thid[i]), (pthread_attr_t*)NULL, AvarageValue, (void*)(&(thidStruct[i]))));
    }
    
    
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
    scanf("%d", &numOfThreads);
    int numForOneThread;


    for_one_thread* thidStruct = (for_one_thread*) calloc (numOfThreads, sizeof(for_one_thread));
    pthread_t* thid = (pthread_t*) calloc(numOfThreads, sizeof(pthread_t));
    //randomly fill the array
    for (int i = 0; i < AMOUNT_OF_ELEMENTS; i++)
    {
        mass[i] = rand()%2000;
    }
    numForOneThread = AMOUNT_OF_ELEMENTS / numOfThreads;
    //Let`s go:)

    double avarageValue;
    avarageValue = CalculateAvarageValue(numOfThreads, thidStruct, numForOneThread, mass, thid);
    printf("%lf\n", avarageValue);

    double dispersia;
    dispersia = CalculateDispersia(numOfThreads, thidStruct, numForOneThread, mass, thid);
    printf("\n%lf\n", dispersia);
    return 0;
}



