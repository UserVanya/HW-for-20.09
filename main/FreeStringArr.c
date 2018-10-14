#include <stdio.h>
#include <stdlib.h>
void FreeStrArr(char** arr, int n)
{
    for(int i = 0; i < n; i++)free(arr[i]);
    free(arr);
}
