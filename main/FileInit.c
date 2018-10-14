#include <stdio.h>
void FileInit (FILE* f)
{
    if(!f)
    {
        fclose(f);
        printf("Error. Can`t find file");
        return;
    }
}
