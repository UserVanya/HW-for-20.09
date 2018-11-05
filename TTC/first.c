#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include<string.h>

// FIXIT: 1) код first.c и second.c в значительной мере дублируют друг друга
// Достаточно написать одну программу, которая, например, считывает параметр командной строки,
// и в зависимости от него решает первый это или второй собеседник.
// Опять же, да, но разве есть что-то глубокое, почему нужно использовать именно одну программу, а не две?
// Я реализовал ее так...
// RE: Можно было бы и две оставить, если бы весь дублирующийся код вынесли в отдельный (3й) файл. Дублирование кода - потенциальный
// источник ошибок, т.к. при любом изменении требований нужно поменять два, а не одно место в коде.

void FIFOCheck(int result)
{
    if(result < 0)
    {
        printf("Can\'t create FIFO\n");
        exit(-1);
    }
}
void OpenCheck(int fd)
{
    if(fd < 0)
    {
        printf("Can\'t open FIFO for writting\n");
        exit(-1);
    }
}
void CloseCheck(int result)
{
    if(result)
    {
         printf("Can\'t close file\n");
    }
}
void WriteCheck(size_t size, char* string)
{
    if((int)size != (int)strlen(string))
    {
         printf("Can\'t write whole string:%s\n", string);
         exit(-1);
    }
}
void ReadCheck(size_t size)
{
    if(size < 0)
    {
       printf("Can\'t read string from FIFO\n");
       exit(-1);
    }
}
char* ScanStr(char* a, int buff, FILE* f)
{
    int lastSymbol = 0;
    int i = 1;

    fgets(a, buff, f);

    // fgets saving last symbol for \0, that is why if variable 'a' isn`t fully scanned it consists only 9 elements
    // The last symbol can`t  be '\n'.

    lastSymbol = strlen(a) - 1;

    while(a[lastSymbol] != '\n')
    {
        i++;
        a =(char*) realloc(a, buff * i * sizeof(char));
        fgets(a + strlen(a), buff, f);
        lastSymbol = strlen(a) - 1;
    }

    // allocating exact memory block

    a =(char*) realloc(a, strlen(a) * sizeof(char));

    return a;
}

// Ф-я http://www.cplusplus.com/reference/cstring/memset/ делает ровно то же самое. Лучше использовать готовое.
void ClearString(char* string, int n)
{
    for (int i = 0; i < n; i++)
    {
	string[i] = '\0';
    }
}

void IntoPipe(int outputFD, char* fifoPath, int buff, FILE* f)
{
    int i = 0;
    char* outputString = (char*) calloc(buff, sizeof(char));
    while(strcmp("exit\n", outputString) != 0)
    {
        ClearString(outputString, strlen(outputString));
	// FIXIT: вы на каждое сообщение закрываете и открываете FIFO.
	// Открыть достаточно один раз до цикла
	// Вы же в прошлых упражнениях, где читали строки из файла, не переоткрывали его после каждой строки.
        OpenCheck(outputFD = open(fifoPath, O_WRONLY));
        outputString = ScanStr(outputString, buff, f);
        WriteCheck(write(outputFD, outputString, strlen(outputString)), outputString);
        CloseCheck(close(outputFD));
    }
    free(outputString);
}


void FromPipe(int inputFD, char* fifoPath, int buff, FILE* f)
{
    char* inputString = (char*) calloc(buff, sizeof(char));
    while (strcmp("exit\n", inputString) != 0)
    {
        ClearString(inputString, strlen(inputString));
        OpenCheck(inputFD = open(fifoPath, O_RDONLY));
	// вот здесь вы можете проверить, что если read вернул 0, то выйти из цикла while
        ReadCheck(read(inputFD, inputString, buff));
        fprintf(f, "2:%s", inputString);    
        CloseCheck(close(inputFD));    
    }
    free(inputString);
}
int main()
{
    int buff = 271;
    int outputFD, inputFD;
    (void)umask(0);
    FIFOCheck(mknod("first.fifo", S_IFIFO | 0777, 0));
    FIFOCheck(mknod("second.fifo", S_IFIFO | 0777, 0));
    printf("first:\n");
    pid_t pid;
    pid = fork();
    if (pid == 0) FromPipe(inputFD, "first.fifo", buff, stdout);    
    else IntoPipe(outputFD, "second.fifo", buff, stdin);
}

