#include <stdio.h>
#include <string.h>
#include <stdlib.h>


char* ScanStr(char* a, int buff)
{
    int i = 1;
    fgets(a, buff, stdin);
    // Если честно, то код ниже выглядит очень сложно и нет уверенности, что работает корректно
    // Задайте buff = 1 и проверьте работоспособность.
    // Я бы предложил конечно же добаться работоспособности, а потом удалить этот кусок, и пользоваться просто gets,
    // предполагая, что больше 1024 строки никто вводить не будет.
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
    
    // Задачей было поставлено написать функцию... Функция split работает корректно, что написано в main - это уже другой вопрос
    // Как делать лучше в данном случае, да, так лучше, но в отдельной ситуации это может быть не так, и да, я не понимаю 
    //зачем нужен тройной указатель...
    //Функция, состоящая из 9 строк сложно написана? Не знаю как написать ее по другому. Вы сказали , что будете ломать ф-ю, но 
    // пишите обычно не до конца понятное задание. Вернее сказать не до конца определенное. Поэтому нельзя написать функцию так,
    //Чтобы она вам понравилась, вернее можно, но угадав, либо умев программировать на вашем уровне, а если еще конкретнее,
    //то нужно просто быть вами. Допустим можно было написать: предполагается, что на вход не будет подано больше n символов...
    //Ошибка при buff = 1 возникала из-за того , что я не поменял значение 1024, которое было ранее на buff.
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
