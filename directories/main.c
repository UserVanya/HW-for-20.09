#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>

#define SIMILAR 0
#define TRUE 1
#define BUFF 1024

int StatCheck(int result, char* path)
{
    if (result < 0)
    {
        printf("Function stat failed on path:%s\n", path);
        return 0;
    }
    return 1;
}
int IsFileInCurrentDirectory(char* path, char* fileName)
{
    DIR* address = opendir(path);
    struct dirent* dirInfo;
    while((dirInfo = readdir(address)))
    {
        if(strcmp(dirInfo->d_name, fileName) == SIMILAR)
        return 1;
    }
    return 0;
}
char* MakeFullPath(char* PathToLookFor, char* toAdd, char* currentDir)
{
    strcpy(PathToLookFor, currentDir);
    strcat(PathToLookFor, "/");
    strcat(PathToLookFor, toAdd);
    return PathToLookFor;
}
void FindFile(char* currentDir, int depth, char* fileName)
{
    if (depth < 0) return;
    struct dirent* CurrFileInfo;
    struct stat info;
    DIR* address = opendir(currentDir);
    char* PathToLookFor = (char*) calloc(BUFF, sizeof(char));
    while((CurrFileInfo = readdir(address)))
    {
        if((strcmp(CurrFileInfo->d_name, "..") == SIMILAR) || (strcmp(CurrFileInfo->d_name, ".") == SIMILAR)) continue;
        MakeFullPath(PathToLookFor, CurrFileInfo->d_name, currentDir);
        if(!StatCheck(stat(PathToLookFor, &info), PathToLookFor)) continue;
        if(S_ISDIR(info.st_mode)) FindFile(PathToLookFor, depth - 1, fileName);
        else if(strcmp(CurrFileInfo->d_name, fileName) == SIMILAR) printf("The path is:%s\n", currentDir);
    }
    free(PathToLookFor);
}


int main(int argc, char *argv[])
{  
    int depth = atoi(argv[2]);
    FindFile(argv[1], depth, argv[3]);
}
