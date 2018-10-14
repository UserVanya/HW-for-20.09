#include <stdio.h>
void PrintStrArr (char** str, int n)
{
    printf ("\nString array:\n\n");
    for (int i = 0; i < n; ++i)
    {
        printf ("%d string is:%s\n", i, str[i]);
    }
    printf ("\n");
}
