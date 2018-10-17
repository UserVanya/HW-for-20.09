#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include<string.h>

void FIFOInit(int result)
{
    if(result < 0)
    {
        printf("Can\'t create FIFO\n");
        exit(-1);
    }
}
void OpenInit(int fd)
{
    if(fd < 0)
    {
        printf("Can\'t open FIFO\n");
        exit(-1);
    }
}
void CloseInit(int result)
{
    if(result)
    {
         printf("Can\'t close file\n");
    }
}
void WriteInit(size_t size, char* string)
{
    if((int)size != (int)strlen(string))
    {
         printf("Can\'t write whole string:%s\n", string);
         exit(-1);
    }
}


void ReadInit(size_t size)
{
    if(size < 0)
    {
       printf("Can\'t read string from FIFO\n");
       exit(-1);
    }
}
void ForkInit(int result)
{
    if (result < 0)
    {
        printf("Can`t fork child\n");
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
        OpenInit(outputFD = open(fifoPath, O_WRONLY));
        outputString = ScanStr(outputString, buff, f);
        WriteInit(write(outputFD, outputString, strlen(outputString)), outputString);
        CloseInit(close(outputFD));
    }
    free(outputString);
}


void FromPipe(int inputFD, char* fifoPath, int buff, FILE* f)
{
    int i = 0;
    char* inputString = (char*) calloc(buff, sizeof(char));
    while (strcmp("exit\n", inputString) != 0)
    {
        ClearString(inputString, strlen(inputString));
        OpenInit(inputFD = open(fifoPath, O_RDONLY));
        ReadInit(read(inputFD, inputString, buff));
        fprintf(f, "1:%s", inputString);    
        CloseInit(close(inputFD));    
    }
    free(inputString);
}
int main()
{
    int buff = 271;
    int outputFD, inputFD;
    (void)umask(0);
    //FIFOInit(mknod("first.fifo", S_IFIFO | 0777, 0));
    //FIFOInit(mknod("second.fifo", S_IFIFO | 0777, 0));
    printf("second:\n");
    pid_t pid;
    pid = fork();
    if (pid == 0) IntoPipe(outputFD, "first.fifo", buff, stdin);
    else FromPipe(inputFD, "second.fifo", buff, stdout);
}
