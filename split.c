#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// Мы так на семинаре дальше вопроса, о необходимости условия (a[strlen(a) - 1] == 0) не сдвинулись
// - Дальше у меня были вопросы про (buff * i) - 1. Зачем вычитать 1?
// - В     char*** tokens;
//       tokens = (char***) calloc (1, sizeof(char**));
//   нет необходимости, т.к. можно просто на куче выделить один char**
// - Нужно память выделять в одном месте. Сейчас часть выделяется до split, а часть внутри. Вынесите, пожалуйста, всё наружу.
// Все спорные на ваш взгляд моменты давайте обсуждать на семинаре, а не в комментариях к коду.

char* ScanStr(char* a, int buff)
{
    int i = 1;
    fgets(a, buff, stdin);
    // Если честно, то код ниже выглядит очень сложно и нет уверенности, что работает корректно
    // Задайте buff = 1 и проверьте работоспособность.
    // Я бы предложил конечно же добаться работоспособности, а потом удалить этот кусок, и пользоваться просто gets,
    // предполагая, что больше 1024 строки никто вводить не будет.
    //Ошибка при buff = 1 возникала из-за того , что я не поменял значение 1024, которое было ранее на buff.
    while (!((a[strlen(a) - 1] == 0) ^ (a[strlen(a) - 1] == '\n')))
    {
        ++i;
        a = (char*) realloc (a, (buff * i) - 1);
        fgets(&a[(buff * (i - 1)) - 1], buff, stdin);
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
    
    // Кажется, что произошла некая путаница с указателями. Можно на семинаре обсудить.
    char*** tokens;
    tokens = (char***) calloc (1, sizeof(char**));
    char* delimeters;
    delimeters = (char*) malloc (buff);
    delimeters = ScanStr(delimeters, buff);
    
    // FIXIT: Точно не нужно выделять один int в куче. 
    // Лучше просто создать переменную на стеке int tokensCount;
    // а в ф-ю передать Split(..., &tokensCount);
    // В данном случае да, но это функция main, которую я писал для себя.
    
    
    int tokensCount;
    Split(string, delimeters, tokens, &tokensCount);
    for (int i = 0; i < tokensCount; ++i)
    {
        printf("%s\n", (*tokens)[i]);
    }
    //И да , тут ещеи память не высвобождается for'ом
    free(**tokens);
    free(*tokens);
    free(tokens);
    free(delimeters);
    free(string);
}
