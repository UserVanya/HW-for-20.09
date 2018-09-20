#include<stdio.h>
#include<string.h>
#include<stdlib.h>

char* SS1024(char* a)
{
	int i = 1;
	fgets(a, 1024, stdin);
	while (!((a[strlen(a)-1] == 0) ^ (a[strlen(a)-1] == 10))) 
	{
		++i;
		a = (char*) realloc (a, (1024 * i) - 1);
		fgets(&a[(1024 * (i-1)) - 1], 1024, stdin);
	}
	return a;
}

void Split (char* string/*строка*/, char* delimeters/*раздел€ющий знак*/, char*** tokens/*двумерный массив слов*/, int* tokensCount/*счетчик количества слов*/)
{
	int k = 0;
	*tokensCount = 0;
	for (int i = 0; i < strlen(string); ++i)
	{
		if((!strncmp(&string[i], delimeters, strlen(delimeters)-1)) || (string[i] == 10))
		{
			//¬ыдел€ю пам€ть под 1 слово
			if ((*tokensCount) == 0)
			{
				*tokens = (char**) malloc ( sizeof(char*));
			}
			else
			{
				*tokens = (char**) realloc (*tokens, (*tokensCount) * sizeof(char*));	
			}
			//¬ыдел€ю пам€ть под массив символов
			(*tokens)[*tokensCount] = (char*) calloc ((i - k + 1), sizeof(char));   
			//копирую текст с k по i символ string в (*tokens)[*tokensCount]
			strncpy((*tokens)[(*tokensCount)], &string[k], i - k);
			// словоdelслово
			// 0123456789
			// ћы сейчас на i = 5, а надо, чтобы показывал на 8;
			(*tokensCount)++;
			k = i + (strlen(delimeters) - 1);
		}	
	}	
}

int main(void)
{
	int b = 0;
	char* string;
	char*** tokens;
	tokens = (char***) calloc (1, sizeof(char**));
	char* delimeters;
	int* tokensCount;
	tokensCount = (int*) calloc(1, sizeof(int)); 
	string = (char*) malloc (1024);
	delimeters = (char*) malloc (1024);
	string = SS1024(string);
	delimeters = SS1024(delimeters);
	Split(string/*строка*/, delimeters/*раздел€ющий знак*/, tokens/*двумерный массив слов*/, tokensCount/*счетчик количества слов*/);
	for (int i = 0; i < *tokensCount; ++i)
	{
		printf("%s\n", (*tokens)[i]);	
	}	
}
