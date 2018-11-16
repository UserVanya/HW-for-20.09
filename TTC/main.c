#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define COINCIDE 0

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

void IntoPipe(int outputFD, char* fifoPath, int buff, FILE* f)
{
    char* outputString = (char*) calloc(buff, sizeof(char));
    OpenCheck(outputFD = open(fifoPath, O_WRONLY));
    while(strcmp("exit\n", outputString) != COINCIDE)
    {
        memset(outputString, '\0', strlen(outputString));
        outputString = ScanStr(outputString, buff, f);
        WriteCheck(write(outputFD, outputString, strlen(outputString)), outputString);
    }
    CloseCheck(close(outputFD));
    free(outputString);
}


void FromPipe(int inputFD, char* fifoPath, int buff, FILE* f, int inputConsole)
{
    char* inputString = (char*) calloc(buff, sizeof(char));
    OpenCheck(inputFD = open(fifoPath, O_RDONLY));
    while (strcmp("exit\n", inputString) != COINCIDE)
    {
        memset(inputString, '\0', strlen(inputString));
        ReadCheck(read(inputFD, inputString, buff));
        fprintf(f, "%d:%s", inputConsole, inputString);
    }
    CloseCheck(close(inputFD));
    free(inputString);
}
int main()
{
    int buff = 271;
    int outputFD = 0, inputFD = 0;
    (void)umask(0);
    FIFOCheck(mknod("first.fifo", S_IFIFO | 0777, 0));
    FIFOCheck(mknod("second.fifo", S_IFIFO | 0777, 0));
    int consoleNumber;
    printf("Enter the number of console:");
    scanf("%d",&consoleNumber);
    if (consoleNumber == 1)
    {
        printf("first:\n");
        pid_t pid;
        pid = fork();
        if (pid == 0) FromPipe(inputFD, "first.fifo", buff, stdout, 2);
        else IntoPipe(outputFD, "second.fifo", buff, stdin);
    }
    if (consoleNumber == 2)
    {
        printf("second:\n");
        pid_t pid;
        pid = fork();
        if (pid == 0) FromPipe(inputFD, "second.fifo", buff, stdout, 1);
        else IntoPipe(outputFD, "first.fifo", buff, stdin);
    }
}
