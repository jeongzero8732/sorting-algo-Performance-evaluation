#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

typedef struct str
{
	int id;
	int year;
	int year_id;//stable sorting�� ����
	char title[150];
	int title_id;//stable sorting �� ����
	char genre[150];
	int line;
}DataSet;

typedef struct kk
{
	int head;
	int tail;
	int year_tail_index;
	int year_head_index;
}IndexSet;

typedef struct zz
{
	char* insert_sorting_store;
	char* merge_sorting_store;
	char* quick_sorting_store;
	char* heap_sorting_store;
	char* radix_sorting_store;
}Sorting_Table;


/*�޸� �ʱ�ȭ*/
//Sorting_Table* init_memory(Sorting_Table*,int);

/*���Ͽ��� ������ �о���� �Լ�*/
void file_read(DataSet*, int*);

/*������ ũ�� ���*/
int Calc_line();

/*��ȭ����,�⵵,�帣, �ε��� ���� �Լ�*/
void export_movie_id(char*, DataSet*, IndexSet*, int*);
void export_index(char*, DataSet*, IndexSet*, int*);
void export_year_title(char*, DataSet*, IndexSet*, int*);

/*�⵵ sorting �Լ�*/
void insert_sorting_year(DataSet*, int*);
void merge_sorting_year(DataSet*, int, int, DataSet*);
void MergeTwoArea_year(DataSet* dataSet, int left, int mid, int right, DataSet*);
void merge_year(DataSet*, int*);
void quick_sorting_year(DataSet*, int*);
void quick_year(DataSet* dataSet, int p, int r, DataSet* tmp);
int partition_year(DataSet* dataSet, int p, int r, DataSet* tmp);
void heap_sorting_year(DataSet*, int*);
void radix_sorting_year(DataSet*, int*);

/*Ÿ��Ʋ sorting �Լ�*/
void insert_sorting_title(DataSet*, int*);
void merge_title(DataSet* dataSet, int* lineCount);
void MergeTwoArea_title(DataSet* dataSet, int left, int mid, int right, DataSet*);
void merge_sorting_title(DataSet*, int*);
void quick_sorting_title(DataSet*, int*);
void quick_title(DataSet* dataSet, int p, int r, DataSet* tmp);
int partition_title(DataSet* dataSet, int p, int r, DataSet* tmp);
void heap_sorting_title(DataSet*, int*);
void radix_sorting_title(DataSet*, int*);

/*�޴��� �Լ�*/
int menual(DataSet*, int*, int*);
void sorting_menual(DataSet*, int, int*, int*);


/*��� ���*/
void show_result(DataSet*);

int main()
{
	printf("@@@%d@@@\n", sizeof(DataSet));
	int Datasize, i;
	int lineCount = -1;
	DataSet* dataSet = NULL;

	Datasize = Calc_line();//������ ������ �� ���
	dataSet = (DataSet*)malloc(sizeof(DataSet) * Datasize);
	memset(dataSet, 0, sizeof(DataSet));


	file_read(dataSet, &lineCount); //���Ͽ��� ������ ��������

	//for (int i = 0; i < lineCount; i++)
	//{
	//	printf("��ȭ ���� : %-40s �⵵ : %d   \n", dataSet[i].title, dataSet[i].year);
	//}

	menual(dataSet, &lineCount, &Datasize);
	free(dataSet);

	//printf("%d\n",lineCount);
	return 0;
}
void file_read(DataSet* dataSet, int* lineCount)
{

	FILE* stream;
	char read_line[500] = { 0 };
	IndexSet indexSet = { 0,0,0,0 };

	int i = 0;
	int line = 0;

	stream = fopen("movies.txt", "r");

	while (fgets(read_line, 500, stream))
	{
		if ((*lineCount) == -1)
		{
			(*lineCount)++;
			continue;
		}
		/*�о�� ������ ������ ��ȭ ���̵� �̴� �Լ�
			1.�տ������� �о ó�� �޸��� ������ �Լ� Ż��
			2. ���ö� �޸��� �ε���(head)�� ������ ����
			3. ���⼭ ��ȭ id ���� ����  -->dataSet[lineCount].id �� ����
		*/
		export_movie_id(read_line, dataSet, &indexSet, lineCount);


		/* �ڿ������� �ε����� ���ҽ�Ű�鼭 �޸��� ���ö� ���� ã�� �Լ�
			1.�� �ڿ��� ���� �����鼭 �޸��� ������ �� ��ġ�� �ε���(tail) ����
				1.1 ���⼭ �帣 ������ ���� �ε��� ���Ⱑ��
				1.2 ���� �޸��� ������ ���� ) �̰� ������ �帣�� ���ٴ� �Ҹ��̹Ƿ� ��� �Ͻ�����

			2. �״��� ���ʹ� ������ ���ϰ� ���� - �״��� �⵵�� ������ �ǹǷ� )�� ������ �ش� �ε���(year_tail_index) ����
			3. ��� �⵵ �����ϴٰ� (�� ������ �� ��ġ�� �ε���(year_head_index)�� �����ϰ� �Լ� Ż��
			4. ���⼭ �⵵ ������ ���� �ε��� ���� ����
		*/
		export_index(read_line, dataSet, &indexSet, lineCount);


		/* ����,�⵵,�帣 �����Լ�
			1. �ε��� head���� year_head_index-1���� - ��������  : dataSet[lineCount].title�� ����, dataSet[lineCount].title_id ����
				1.1 ���� head������ " �̰Կ��� ����x
			2. �ε��� year_head_index+1 ���� year_tail_index-1 ���� - �⵵ ���� -> atoi �Լ��� ������ �ٷ� �ٲٱ� : dataSet[lineCount].year, dataSet[lineCount].year_id ����
			3. �ε��� tail+1���� ������ ������ - �帣 ���� : dataSet[lineCount].genre
		*/
		export_year_title(read_line, dataSet, &indexSet, lineCount);

		(*lineCount)++;

		//�ε��� ����ü �ʱ�ȭ
		memset(&indexSet, 0, sizeof(IndexSet));
	}
	fclose(stream);
	//printf("[[[%d\n",lineCount); lineCount�� Datasize-lineCount�� �̿��Ͽ� ���ŵ� �������� ���� Ȯ�� ����
}

int Calc_line()
{
	FILE* fp;
	char buf[500] = { 0 };
	int count = 0;
	fp = fopen("movies.txt", "r");

	while (fgets(buf, sizeof(buf), fp))
	{
		count++;
	}
	fclose(fp);
	return count;
}//�� ���μ�
void export_movie_id(char* read_line, DataSet* dataSet, IndexSet* indexSet, int* lineCount)
{
	int index = 0;
	char buf[100] = { 0 };

	for (index = 0;; index++) //�ε��� 0���� �޸������� Ȯ��
	{
		if (read_line[index] == ',')
		{
			(*indexSet).head = index;//id���� ������ �޸��� ��ġ�ε���
			break;
		}
		buf[index] = read_line[index];
	}
	dataSet[(*lineCount)].id = atoi(buf); //���ڿ� id���� ������ �ٲ㼭 ����
}

void export_index(char* read_line, DataSet* dataSet, IndexSet* indexSet, int* lineCount)
{
	int index_behind = 0;
	char buf[200] = { 0 };
	int flag = 0;
	int count = 1;
	int tmp = 0;




	for (index_behind = strlen(read_line);; index_behind--)
	{
		if (read_line[index_behind] > 126 && read_line[index_behind] < 0)
		{
			//��ƾ��? ������ �� ������ ������
			*lineCount--;
			return;
		}

		if (read_line[index_behind] == '\n')
		{
			continue;
		}

		if (read_line[index_behind] == ',')
		{
			if (((read_line[index_behind - 3] < 0x30) || (read_line[index_behind - 3] > 0x39)))
			{
				break;
			}

			(*indexSet).tail = index_behind - 1; //�帣�� ���� �ε��� ����
			flag = 1;
		}
		else if (read_line[index_behind] == ')')//�����ؾ���. �帣�� ���� ��� �޸� �����ʿ� () �̰� �ü� ����.
		{

			if (flag == 1)//�޸� ������ ������ �⵵�� ���� �ǹ��ϴ� )
			{
				(*indexSet).year_tail_index = index_behind;//�⵵�� ���� �ε���
			}
			else // no genre�� ǥ���ϴ� )
			{
				continue;
			}
		}
		else if (read_line[index_behind] == '(')//�̰͵� �����ؾ���. (no genre) ���ڵ�� �̰� ������ �����
		{
			if (flag == 1)
			{
				(*indexSet).year_head_index = index_behind; //�⵵�� ���� �ε���
				return;
			}
			else
			{
				continue;
			}
		}
		else if (index_behind == ((*indexSet).head + 1))
		{
			//�̋� �⵵�� ���ٴ� �� . �⵵�� -1 ����
			//dataSet[*lineCount].year = -1;
			break;
		}
		//�⵵ �� ������ () �̰� �ƿ� ����. 
	}
}
void export_year_title(char* read_line, DataSet* dataSet, IndexSet* indexSet, int* lineCount)
{
	// 1. �ε��� head���� year_head_index - 1���� - ��������  : dataSet[lineCount].title�� ����, dataSet[lineCount].title_id ����
	// 	  1.1 ���� head������ " �̰Կ��� ����x
	int i = 0, j = 0;
	int year_index_buf;
	char title_buf[500] = { 0 };
	char year_buf[100] = { 0 };
	char genre_buf[100] = { 0 };

	year_index_buf = (*indexSet).year_head_index;

	if (read_line[(*indexSet).year_head_index - 1] == ' ') //����� �⵵ ���̿� ���������� ����
	{
		year_index_buf = ((*indexSet).year_head_index) - 1;
	}

	for (i = (*indexSet).head + 1; i < year_index_buf; i++)
	{
		if (read_line[i] > 126 || read_line[i] < 0)
		{
			//��ƾ��? ������ �� ������ ������
			(*lineCount)--;
			return;
		}

		if (read_line[i] == '"')
		{
			continue;
		}
		title_buf[j] = read_line[i];
		j++;
	}
	if ((*indexSet).year_head_index == 0)
	{
		(*lineCount)--;
		return;
	}
	else
	{
		strcpy(dataSet[*lineCount].title, title_buf); //������ ���� ����
		dataSet[*lineCount].title_id = *lineCount; //stable sorting�� ����
		j = 0;
	}

	// 2. �ε��� year_head_index + 1 ���� year_tail_index - 1 ���� - �⵵ ����->atoi �Լ��� ������ �ٷ� �ٲٱ� : dataSet[lineCount].year, dataSet[lineCount].year_id ����
	for (i = (*indexSet).year_head_index + 1; i < (*indexSet).year_tail_index; i++)
	{
		year_buf[j] = read_line[i];//�⵵ ���ۿ� ����
		j++;
	}
	dataSet[*lineCount].year = atoi(year_buf);//�⵵�� ��������
	dataSet[*lineCount].year_id = *lineCount; //stable sorting�� ����


	j = 0;
	// 3. �ε��� tail + 1���� ������ ������ - �帣 ���� : dataSet[lineCount].genre
	for (i = (*indexSet).tail + 2; i < strlen(read_line); i++)
	{
		if (read_line[i] == '\n')
			break;
		//���� �ϳ� �о ( �̰Ÿ� �帣 ���� ��.
		if (read_line[i] == '(')
		{
			strcpy(dataSet[*lineCount].genre, "no genre");
			break;
		}
		genre_buf[j] = read_line[i];
		j++;
	}
	strcpy(dataSet[*lineCount].genre, genre_buf);
}

int menual(DataSet* dataSet, int* lineCount, int* Datasize)
{
	int number = 0;
	for (;;)
	{
		//("cls");
		printf(" -----------------------------------�ý��� �޴���------------------------------------\n");
		printf("					 																 \n");
		printf("		              1. ���� ���� ������ Ȯ���ϱ�			                 \n");
		printf("		              2. �⵵ �������� ������ �����ϱ�		   		         \n");
		printf("		              3. ��ȭ ���� �������� ������ �����ϱ�		         \n");
		printf("		              4. �ý��� ����			 						     \n");
		printf("																					 \n");
		printf(" ------------------------------------------------------------------------------------\n\n\n");

		printf("===> ��ȣ�� �Է��ϼ���: ");
		scanf("%d", &number);

		switch (number)
		{
		case 1:
			//show_result();
			break;
		case 2:
			sorting_menual(dataSet, 2, lineCount, Datasize);
			break;
		case 3:
			sorting_menual(dataSet, 3, lineCount, Datasize);
			break;
		case 4:
			return;
		default:
			break;
		}
	}

}
void sorting_menual(DataSet* dataSet, int select, int* lineCount, int* Datasize)
{
	system("cls");
	clock_t start = 0, end = 0;
	int number = 0;
	printf(" -----------------------------------���� ��� ����------------------------------------\n");
	printf("					 																 \n");
	printf("		                     1. ���� ����				                     \n");
	printf("		                     2. �պ� ����	   				        		 \n");
	printf("		                     3. �� ����										 \n");
	printf("		                     4. �� ����										 \n");
	printf("		                     5. ��� ����										 \n");
	printf(" ------------------------------------------------------------------------------------\n\n\n");

	for (;;)
	{
		printf("===> ��ȣ�� �Է��ϼ���: ");
		scanf("%d", &number);
		if (select == 2)
		{
			switch (number)
			{
			case 1:
				insert_sorting_year(dataSet, lineCount);
				return;
			case 2:

				start = clock();
				merge_year(dataSet, lineCount);
				end = clock();
				printf("�ɸ��ð�:%.3lf\n", (double)(end - start));
				for (int i = 0; i < *lineCount; i++)
				{
					printf("��ȭ ���� : %-40s �⵵ : %d    �帣: %s \n", dataSet[i].title, dataSet[i].year, dataSet[i].genre);
				}
				return;
			case 3:
				start = clock();
				quick_sorting_year(dataSet, lineCount);
				end = clock();
				printf("�ɸ��ð�:%.3lf\n", (double)(end - start));
				for (int i = 0; i < *lineCount; i++)
				{
					printf("��ȭ ���� : %-40s �⵵ : %d    �帣: %s \n", dataSet[i].title, dataSet[i].year, dataSet[i].genre);
				}
				return;
			case 4:
				heap_sorting_year(dataSet, lineCount);
				return;
			case 5:
				radix_sorting_year(dataSet, lineCount);
				return;
			default:
				printf("�߸� �Է��߽��ϴ�. �ٽ� �Է��ϼ��� \n");
				break;
			}
		}
		else if (select == 3)
		{
			switch (number)
			{
			case 1:
				insert_sorting_title(dataSet, lineCount);
				return;
			case 2:
				start = time(NULL);
				merge_title(dataSet, lineCount);
				end = time(NULL);
				printf("�ɸ��ð�:%.3lf\n", (double)(end - start));
				for (int i = 0; i < *lineCount; i++)
				{
					printf("��ȭ ���� : %-40s �⵵ : %d    �帣: %s \n", dataSet[i].title, dataSet[i].year, dataSet[i].genre);
				}
				return;
			case 3:
				start = time(NULL);
				quick_sorting_title(dataSet, lineCount);
				end = time(NULL);
				printf("�ɸ��ð�:%.3lf\n", (double)(end - start));
				for (int i = 0; i < *lineCount; i++)
				{
					printf("��ȭ ���� : %-40s �⵵ : %d    �帣: %s \n", dataSet[i].title, dataSet[i].year, dataSet[i].genre);
				}
				return;
			case 4:
				heap_sorting_title(dataSet, lineCount);
				return;
			case 5:
				radix_sorting_title(dataSet, lineCount);
				return;
			default:
				printf("�߸� �Է��߽��ϴ�. �ٽ� �Է��ϼ��� \n");
				break;
			}
		}
	}
	//system("cls");
}

void insert_sorting_year(DataSet* dataSet, int* lineCount)
{
	int j = 0;
	int i = 0;
	time_t start = 0, end = 0;
	DataSet* key = NULL;
	key = (DataSet*)malloc(sizeof(DataSet)*(*lineCount));

	start = time(NULL);
	for (i = 1; i < *lineCount; i++)
	{
		//key = &dataSet[i];
		memcpy(key, &(dataSet[i]), sizeof(DataSet));
		//i = j - 1;
		for (j = i - 1; j >= 0 && dataSet[j].year > key->year; j--)
		{
			memcpy(&(dataSet[j + 1]), &(dataSet[j]), sizeof(DataSet));
		}
		//	dataSet[j + 1] = key;
		memcpy(&(dataSet[j + 1]), key, sizeof(DataSet));
	}
	end = time(NULL);
	printf("���� ���� ����!!!!!      �ɸ��ð� : %.3lf\n", (double)(end - start));
	for (int i = 0; i < *lineCount; i++)
	{
		printf("id: %d        ��ȭ ���� : %-60s �⵵ : %d    �帣: %s \n", dataSet[i].id, dataSet[i].title, dataSet[i].year, dataSet[i].genre);
	}
	free(key);
}
void merge_sorting_year(DataSet* dataSet, int left, int right, DataSet* sortArr)
{
	int mid;
	if (left < right)
	{
		// �߰� ������ ����Ѵ�.
		mid = (left + right) / 2;

		// �ѷ� ������ ������ �����Ѵ�.
		merge_sorting_year(dataSet, left, mid, sortArr);
		merge_sorting_year(dataSet, mid + 1, right, sortArr);

		// ���ĵ� �� �迭�� �����Ѵ�.
		MergeTwoArea_year(dataSet, left, mid, right, sortArr);
	}
}

void quick_sorting_year(DataSet* dataSet, int* lineCount)
{
	DataSet* tmp = (DataSet*)malloc(sizeof(DataSet) * 28000);
	quick_year(dataSet, 0, (*lineCount) - 1, tmp);
}
void quick_year(DataSet* dataSet, int p, int r, DataSet* tmp)
{
	int q;

	if (p < r)
	{
		q = partition_year(dataSet, p, r, tmp);
		quick_year(dataSet, p, q - 1, tmp);
		quick_year(dataSet, q + 1, r, tmp);
	}
}
int partition_year(DataSet* dataSet, int p, int r, DataSet* tmp)
{
	int zzz = 30000;
	int x, i, j, k;
	x = dataSet[r].year;
	i = p - 1;
	for (j = p; j < r; j++)
	{
		if (dataSet[j].year <= x)//�ǹ��� �ϳ��� ��
		{
			//�ǹ��� ���� ���̸� �⵵id ���� ���Ͽ� 
			i = i + 1;
			memcpy(tmp, &dataSet[i], sizeof(DataSet));//�ӽ�����
			memcpy(&dataSet[i], &dataSet[j], sizeof(DataSet));//�ӽ�����
			memcpy(&dataSet[j], tmp, sizeof(DataSet));//�ӽ�����
		}
	}
	memcpy(tmp, &dataSet[i + 1], sizeof(DataSet));//�ӽ�����
	memcpy(&dataSet[i + 1], &dataSet[r], sizeof(DataSet));//�ӽ�����
	memcpy(&dataSet[r], tmp, sizeof(DataSet));//�ӽ�����

	return i + 1;
}
int partition_title(DataSet* dataSet, int p, int r, DataSet* tmp)
{
	int i, j, k;
	char x[300] = { 0 };
	strcpy(x, dataSet[r].title);
	i = p - 1;
	for (j = p; j < r; j++)
	{
		k = strcmp(x, dataSet[j].title);
		if (k >= 0)
		{
			i = i + 1;
			memcpy(tmp, &dataSet[i], sizeof(DataSet));//�ӽ�����
			memcpy(&dataSet[i], &dataSet[j], sizeof(DataSet));//�ӽ�����
			memcpy(&dataSet[j], tmp, sizeof(DataSet));//�ӽ�����
		}
	}
	memcpy(tmp, &dataSet[i + 1], sizeof(DataSet));//�ӽ�����
	memcpy(&dataSet[i + 1], &dataSet[r], sizeof(DataSet));//�ӽ�����
	memcpy(&dataSet[r], tmp, sizeof(DataSet));//�ӽ�����

	return i + 1;
}
void heap_sorting_year(DataSet* dataSet, int* Datasize)
{

}
void radix_sorting_year(DataSet* dataSet, int* Datasize)
{

}
void insert_sorting_title(DataSet* dataSet, int* lineCount)
{
	int j = 0;
	int i = 0;

	DataSet* key = NULL;
	key = (DataSet*)malloc(sizeof(DataSet)*(*lineCount));

	for (j = 1; j < *lineCount; j++)
	{

		memcpy(key, &dataSet[j], sizeof(DataSet));
		i = j - 1;

		while (i >= 0 && strcmp(dataSet[i].title, key->title) > 0)
		{
			memcpy(&dataSet[i + 1], &dataSet[i], sizeof(DataSet));
			i = i - 1;
		}
		memcpy(&dataSet[i + 1], key, sizeof(DataSet));
	}
	for (int i = 0; i < *lineCount; i++)
	{
		printf("id: %d        ��ȭ ���� : %-60s �⵵ : %d    �帣: %s \n", dataSet[i].id, dataSet[i].title, dataSet[i].year, dataSet[i].genre);
	}
}
void merge_sorting_title(DataSet* dataSet, int left, int right, DataSet* sortArr)
{
	int mid;
	if (left < right)
	{
		// �߰� ������ ����Ѵ�.
		mid = (left + right) / 2;

		// �ѷ� ������ ������ �����Ѵ�.
		merge_sorting_title(dataSet, left, mid, sortArr);
		merge_sorting_title(dataSet, mid + 1, right, sortArr);

		// ���ĵ� �� �迭�� �����Ѵ�.
		MergeTwoArea_title(dataSet, left, mid, right, sortArr);
	}
}
void quick_sorting_title(DataSet* dataSet, int* lineCount)
{
	DataSet* tmp = (DataSet*)malloc(sizeof(DataSet) * 28000);
	quick_title(dataSet, 0, (*lineCount) - 1, tmp);
}
void quick_title(DataSet* dataSet, int p, int r, DataSet* tmp)
{
	int q;

	if (p < r)
	{
		q = partition_title(dataSet, p, r, tmp);
		quick_title(dataSet, p, q - 1, tmp);
		quick_title(dataSet, q + 1, r, tmp);
	}
}
void heap_sorting_title(DataSet* dataSet, int* Datasize)
{

}
void radix_sorting_title(DataSet* dataSet, int* Datasize)
{

}
void merge_year(DataSet* dataSet, int* lineCount)
{
	DataSet* sortArr = (DataSet*)malloc(sizeof(DataSet) * 28000);
	merge_sorting_year(dataSet, 0, (*lineCount) - 1, sortArr);
	free(sortArr);
}
void merge_title(DataSet* dataSet, int* lineCount)
{
	DataSet* sortArr = (DataSet*)malloc(sizeof(DataSet) * 28000);
	merge_sorting_title(dataSet, 0, (*lineCount) - 1, sortArr);
	free(sortArr);
}
void MergeTwoArea_year(DataSet* dataSet, int left, int mid, int right, DataSet* sortArr)
{
	//	printf("%d\n",oo++);
	int fIdx = left;
	int rIdx = mid + 1;
	int k = left;
	int i;

	/*DataSet* sortArr = NULL;
	sortArr = (DataSet*)malloc(sizeof(DataSet)*(right+1));*/

	//DataSet sortArr[30000];

	while (fIdx <= mid && rIdx <= right)
	{
		if (dataSet[fIdx].year <= dataSet[rIdx].year)
			memcpy(&sortArr[k++], &dataSet[fIdx++], sizeof(DataSet));
		else
			memcpy(&sortArr[k++], &dataSet[rIdx++], sizeof(DataSet));
	}

	while (fIdx <= mid)
		memcpy(&sortArr[k++], &dataSet[fIdx++], sizeof(DataSet));
	while (rIdx <= right)
		memcpy(&sortArr[k++], &dataSet[rIdx++], sizeof(DataSet));

	for (i = left; i <= right; i++)
		memcpy(&dataSet[i], &sortArr[i], sizeof(DataSet));

	//free(sortArr);
}
void MergeTwoArea_title(DataSet* dataSet, int left, int mid, int right, DataSet* sortArr)
{
	//	printf("%d\n",oo++);
	int fIdx = left;
	int rIdx = mid + 1;
	int k = left;
	int i; int buf;

	/*DataSet* sortArr = NULL;
	sortArr = (DataSet*)malloc(sizeof(DataSet)*(right+1));*/

	//DataSet sortArr[30000];

	while (fIdx <= mid && rIdx <= right)
	{
		//if (dataSet[fIdx].year <= dataSet[rIdx].year)
		buf = strcmp(dataSet[rIdx].title, dataSet[fIdx].title);

		if (buf >= 0)
			memcpy(&sortArr[k++], &dataSet[fIdx++], sizeof(DataSet));
		else
			memcpy(&sortArr[k++], &dataSet[rIdx++], sizeof(DataSet));
	}

	while (fIdx <= mid)
		memcpy(&sortArr[k++], &dataSet[fIdx++], sizeof(DataSet));
	while (rIdx <= right)
		memcpy(&sortArr[k++], &dataSet[rIdx++], sizeof(DataSet));

	for (i = left; i <= right; i++)
		memcpy(&dataSet[i], &sortArr[i], sizeof(DataSet));

	//free(sortArr);
}
void show_result(DataSet* dataSet)
{
	//1. ��ü ���� ���� Ȯ���ϱ� 
	//2. ���ĵ� ���� ���� Ȯ���ϱ�
	//	2.1 ���Ĺ�� ����
	//		2.1.1 �⵵ ����
	//		2.1.2 Ÿ��Ʋ ����
}