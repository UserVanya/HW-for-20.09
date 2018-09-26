#include<stdio.h>
#include<string.h>
#include<stdlib.h>

// FIXIT: 1) нужно переименовать ss1024 в нечто более явно отражающее суть.
// 2) 1024 нужно вынести в отдельную константу с ясным именем
// 3) За константой 10 кроется '\n' ? Лучше так и написать 
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

// Кажется, что подход с strncmp ниже не будет работать. Пусть строка раздетителей это " \t:", т.е. разбиваем по пробелам, 
// символам табуляции и двоеточиям. Видимо текущий код разобьет только по пробелам. Посмотрите ещё раз, что делает ф-я strtok.
// Постарайтесь ей воспользоваться.

// FIXIT: 1) Память выделяется, но не освобождается
// 2) Вокруг бинарных операций должны быть пробелы: strlen(delimeters) - 1 и т.п.

void Split (char* string/*ñòðîêà*/, char* delimeters/*ðàçäåëÿþùèé çíàê*/, char*** tokens/*äâóìåðíûé ìàññèâ ñëîâ*/, int* tokensCount/*ñ÷åò÷èê êîëè÷åñòâà ñëîâ*/)
{
	int k = 0;
	*tokensCount = 0;
	for (int i = 0; i < strlen(string); ++i)
	{
		if((!strncmp(&string[i], delimeters, strlen(delimeters)-1)) || (string[i] == 10))
		{
			//Âûäåëÿþ ïàìÿòü ïîä 1 ñëîâî
			if ((*tokensCount) == 0)
			{
				*tokens = (char**) malloc ( sizeof(char*));
			}
			else
			{
				*tokens = (char**) realloc (*tokens, (*tokensCount) * sizeof(char*));	
			}
			//Âûäåëÿþ ïàìÿòü ïîä ìàññèâ ñèìâîëîâ
			(*tokens)[*tokensCount] = (char*) calloc ((i - k + 1), sizeof(char));   
			//êîïèðóþ òåêñò ñ k ïî i ñèìâîë string â (*tokens)[*tokensCount]
			strncpy((*tokens)[(*tokensCount)], &string[k], i - k);
			// ñëîâîdelñëîâî
			// 0123456789
			// Ìû ñåé÷àñ íà i = 5, à íàäî, ÷òîáû ïîêàçûâàë íà 8;
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
	Split(string/*ñòðîêà*/, delimeters/*ðàçäåëÿþùèé çíàê*/, tokens/*äâóìåðíûé ìàññèâ ñëîâ*/, tokensCount/*ñ÷åò÷èê êîëè÷åñòâà ñëîâ*/);
	for (int i = 0; i < *tokensCount; ++i)
	{
		printf("%s\n", (*tokens)[i]);	
	}	
}
