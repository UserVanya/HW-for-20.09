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

void Split (char* string/*������*/, char* delimeters/*����������� ����*/, char*** tokens/*��������� ������ ����*/, int* tokensCount/*������� ���������� ����*/)
{
	int k = 0;
	*tokensCount = 0;
	for (int i = 0; i < strlen(string); ++i)
	{
		if((!strncmp(&string[i], delimeters, strlen(delimeters)-1)) || (string[i] == 10))
		{
			//������� ������ ��� 1 �����
			if ((*tokensCount) == 0)
			{
				*tokens = (char**) malloc ( sizeof(char*));
			}
			else
			{
				*tokens = (char**) realloc (*tokens, (*tokensCount) * sizeof(char*));	
			}
			//������� ������ ��� ������ ��������
			(*tokens)[*tokensCount] = (char*) calloc ((i - k + 1), sizeof(char));   
			//������� ����� � k �� i ������ string � (*tokens)[*tokensCount]
			strncpy((*tokens)[(*tokensCount)], &string[k], i - k);
			// �����del�����
			// 0123456789
			// �� ������ �� i = 5, � ����, ����� ��������� �� 8;
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
	Split(string/*������*/, delimeters/*����������� ����*/, tokens/*��������� ������ ����*/, tokensCount/*������� ���������� ����*/);
	for (int i = 0; i < *tokensCount; ++i)
	{
		printf("%s\n", (*tokens)[i]);	
	}	
}
