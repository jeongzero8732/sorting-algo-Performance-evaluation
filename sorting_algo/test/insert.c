#include <stdio.h>

typedef char Data;

int main()
{
	//Data arr[5] = {'b','a','e','d','c'};
	char ary[10] = "Dfbs";
	char ary1[10] = "Asca";
	char ary2[10] = "Babs";
	char ary3[10] = "Abss";
	char ary4[10] = "Ufsu";
	//Data arr[5] = {3,5,2,1,6};


	

	char key[200] = { NULL };

	for (j = 1; j < *lineCount; j++)
	{

		strcpy(key, dataSet[j].title);
		i = j - 1;

		while (i >= 0 && dataSet[i].title > key)
		{
			strcpy(dataSet[i + 1].title, dataSet[i].title);
			i = i - 1;
		}
		strcpy(dataSet[i + 1].title, key);
	}

	i = 0;
	for(i=0;i<5;i++)
	{
		printf("%c ", arr[i]);
	}
	
	if("abc"=="")
	return 0;
}