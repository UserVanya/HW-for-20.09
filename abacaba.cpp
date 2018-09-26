#include <stdio.h>
#include <string.h>
#include <stdlib.h>
void StringIntPrint(char* a, char* b)
{
	int i;
	for ( i = 0; i < b - a; i++)
	{
		printf ("%hhd ", a[i]);
	}
	printf ("\n");
}

void strprint(char* a, char* b)
{
	int i;
	for ( i = 0; i <= b - a; i++)
	{
		printf ("%c", a[i]);
	}
	printf ("\n");
}

// Называйте ф-и в одном стиле: StringIntPrint, StrPrint, 
// В названии ф-й должен фигурировать какой-то глагол: printAlphabet, например.

void alphavit (char* a, int n)
{
	for (int i = 0; i < n; ++i)
	{
		a[(1 << i) - 1] = (char) ('a' + i);
		if( i != 0)
		{
			strncpy(&a[(1 << i)], &a[0], (1 << i) - 1);
		}
		strprint(a, &a[2 * ((1 << i) - 1) * sizeof(char)]);
	}
}
int main()
{
	char* a;
	int n, i;
	scanf ("%d", &n);
	a = (char*) malloc ( 2 * ((1 << n) - 1) * sizeof(char) + sizeof(char));
	alphavit(a, n);
	free(a);
}
