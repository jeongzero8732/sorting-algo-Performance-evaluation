#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#define TRUE   1
#define FALSE   0
#define BUCKET_NUM_YEAR 10
#define BUCKET_NUM_TITLE 10
typedef struct str
{
	int id;
	int year;
	int year_id;//stable sorting�� ����
	char title[200];
	int title_id;//stable sorting �� ����
	char genre[200];
	int line;
}DataSet;

typedef struct kk
{
	int head;
	int tail;
	int year_tail_index;
	int year_head_index;
}IndexSet;

typedef struct ww
{
	float insert_year;
	float merge_year;
	float quick_year;
	float heap_year;
	float radix_year;
	float insert_title;
	float merge_title;
	float quick_title;
	float heap_title;
	float radix_title;
}Time_Calc;
typedef struct _node
{
	DataSet data;
	struct _node * next;
} Node;

typedef struct _lQueue
{
	Node * front;
	Node * rear;
} LQueue;

typedef LQueue Queue;
void QueueInit(Queue * pq);
int QIsEmpty(Queue * pq);
void Enqueue(Queue * pq, DataSet data);
DataSet Dequeue(Queue * pq);
//ť ���� �Լ�

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
void insert_sorting_year_id(DataSet* dataSet, int* lineCount);
void sort_year_id(DataSet* dataSet, int);
void merge_sorting_year(DataSet*, int, int, DataSet*);
void MergeTwoArea_year(DataSet* dataSet, int left, int mid, int right, DataSet*);
void merge_year(DataSet*, int*);
void quick_sorting_year(DataSet*, int*);
void quick_year(DataSet* dataSet, int p, int r, DataSet*);
int partition_year(DataSet* dataSet, int p, int r, DataSet* tmp);
void heap_sorting_year(DataSet*, int*);
void build_max_heap_year(DataSet* dataSet, int* lineCount);
void heapify_year(DataSet* dataSet, int n, int i);
void radix_sorting_year(DataSet*, int*);

/*Ÿ��Ʋ sorting �Լ�*/
void insert_sorting_title(DataSet*, int*);
void merge_title(DataSet* dataSet, int* lineCount);
void MergeTwoArea_title(DataSet* dataSet, int left, int mid, int right, DataSet*);
void merge_sorting_title(DataSet* dataSet, int left, int right, DataSet* sortArr);
void quick_sorting_title(DataSet*, int*);
void quick_title(DataSet* dataSet, int p, int r, DataSet* tmp);
int partition_title(DataSet* dataSet, int p, int r, DataSet* tmp);
void heap_sorting_title(DataSet* dataSet, int* lineCount);
void build_max_heap_title(DataSet* dataSet, int lineCount, int*);
void heapify_title(DataSet* dataSet, int n, int i, int*);
void radix_sorting_title(DataSet* dataSet, int* lineCount, char lower, char upper);
void countingSort_title(DataSet* dataSet, int size, int index, char lower, char upper);

/*�޴��� �Լ�*/
int menual(DataSet*, int*, int*, int*);
void sorting_menual(DataSet*, int*, int*, int*, int*, Time_Calc*);

/*��� ���*/
void show_result(DataSet*, int*);

int main()
{
	int Datasize, i;
	int lineCount = -1;
	int year_stable = 0;
	DataSet* dataSet = NULL;

	Datasize = Calc_line();//������ ������ �� ���
	dataSet = (DataSet*)malloc(sizeof(DataSet) * Datasize);
	memset(dataSet, 0, sizeof(DataSet));

	file_read(dataSet, &lineCount); //���Ͽ��� ������ ��������

	menual(dataSet, &lineCount, &Datasize, &year_stable);

	free(dataSet);
	return 0;
}
void file_read(DataSet* dataSet, int* lineCount)
{
	int i = 0;

	FILE* stream;
	char read_line[500] = { 0 };
	IndexSet indexSet = { 0,0,0,0 };


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
	//      1.1 ���� head������ " �̰Կ��� ����x
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

int menual(DataSet* dataSet, int* lineCount, int* Datasize, int* year_stable)
{
	int number = 0;
	int menu_flag = 0;
	int sort_count = 0;
	Time_Calc time = { 0 };
	for (;;)
	{

		if (menu_flag == 0)
		{
			printf(" -----------------------------------�ý��� �޴���----------------------------------------------\n");
			printf("\n \t \t \t \t(���� ���� : Ÿ��Ʋ -> �⵵) \n");
			printf("\n\t\t\t\t1. ���� ���� ������ Ȯ���ϱ� \n");
			printf("\n\t\t\t\t2. ��ȭ ���� �������� ������ �����ϱ� \n");
			printf("\n\t\t\t\t3. �ý��� ���� \n");
			printf("\n---------------------------------------------------------------------------------------------\n");
		}
		else if (menu_flag == 1)
		{
			printf(" -----------------------------------�ý��� �޴���----------------------------------------------\n");
			printf("\n \t \t \t \t(���� ���� : Ÿ��Ʋ -> �⵵) \n");
			printf("\n\t\t\t\t1. ���� ���� ������ Ȯ���ϱ� \n");
			printf("\n\t\t\t\t2. �⵵ �������� ������ �����ϱ� \n");
			printf("\n\t\t\t\t3. �ý��� ���� \n");
			printf("\n---------------------------------------------------------------------------------------------\n");
		}

		printf(" ----------------------------------------------------------------------------------------------\n");
		printf("\n\t\tl\t ����\t  ��\t���� ����\t\t\t|\t�⵵ ����  \tl\t\t\n");
		printf(" ----------------------------------------------------------------------------------------------\n");
		printf("\n\t\t\t��������  ��\t  %.4lf�и���\t\t\t %.4lf�и��� \t\n", time.insert_title, time.insert_year);
		printf("\n\t\t\t�պ�����  ��\t  %.4lf�и���\t\t\t %.4lf�и��� \t\n", time.merge_title, time.merge_year);
		printf("\n\t\t\t  ������  ��\t  %.4lf�и���\t\t\t %.4lf�и��� \t\n", time.quick_title, time.quick_year);
		printf("\n\t\t\t  ������  ��\t  %.4lf�и���\t\t\t %.4lf�и��� \t\n", time.heap_title, time.heap_year);
		printf("\n\t\t\t�������  ��\t  %.4lf�и���\t\t\t %.4lf�и��� \t\n", time.radix_title, time.radix_year);
		printf("\n---------------------------------------------------------------------------------------------\n\n\n");

		printf("===> ��ȣ�� �Է��ϼ���: ");
		scanf("%d", &number);

		switch (number)
		{
		case 1:
			(*lineCount) = -1;
			memset(dataSet, 0, sizeof(DataSet));
			file_read(dataSet, lineCount); //���Ͽ��� ������ ��������
			show_result(dataSet, lineCount);
			break;
		case 2:
			if (sort_count % 2 == 0)
			{
				if (sort_count != 0)
				{
					(*lineCount) = -1;
					memset(dataSet, 0, sizeof(DataSet));
					file_read(dataSet, lineCount); //���Ͽ��� ������ ��������
				}
			}
			sorting_menual(dataSet, &menu_flag, lineCount, Datasize, year_stable, &time);
			sort_count++;
			break;
		case 3:
			return;
		default:
			printf("�ٽ��Է��ϼ���!!\n");
			break;
		}
	}

}
void sorting_menual(DataSet* dataSet, int* select, int* lineCount, int* Datasize, int* year_stable, Time_Calc* time)
{
	clock_t start = 0, end = 0;
	int result = 0;
	int number = 0;
	system("cls");
	printf(" -----------------------------------���� ��� ����------------------------------------\n");
	printf("\n\t\t\t\t1. ���� ���� \n");
	printf("\n\t\t\t\t2. �պ� ���� \n");
	printf("\n\t\t\t\t3. �� ���� \n");
	printf("\n\t\t\t\t4. �� ���� \n");
	printf("\n\t\t\t\t5. ��� ���� \n");
	printf("\n------------------------------------------------------------------------------------\n\n\n");

	for (;;)
	{
		printf("===> ��ȣ�� �Է��ϼ���: ");
		scanf("%d", &number);
		if (*select == 0)
		{
			*select = 1;
			switch (number)
			{
			case 1:
				start = clock();
				insert_sorting_title(dataSet, lineCount);
				end = clock();
				time->insert_title = (double)(end - start);
				printf("\n\n����� ���ðڽ��ϱ�? (1:�� 2:�ƴϿ�)\n");
				scanf("%d", &result);
				if (result == 1)
					show_result(dataSet, lineCount);
				else
					return;
				return;
			case 2:
				start = clock();
				merge_title(dataSet, lineCount);
				end = clock();
				time->merge_title = (double)(end - start);
				printf("\n\n����� ���ðڽ��ϱ�? (1:�� 2:�ƴϿ�)\n");
				scanf("%d", &result);
				if (result == 1)
					show_result(dataSet, lineCount);
				else
					return;
				return;
			case 3:
				start = clock();
				quick_sorting_title(dataSet, lineCount);
				end = clock();
				time->quick_title = (double)(end - start);
				printf("\n\n����� ���ðڽ��ϱ�? (1:�� 2:�ƴϿ�)\n");
				scanf("%d", &result);
				if (result == 1)
					show_result(dataSet, lineCount);
				else
					return;
				return;
			case 4:
				start = clock();
				heap_sorting_title(dataSet, lineCount, year_stable);
				end = clock();
				time->heap_title = (double)(end - start);
				printf("\n\n����� ���ðڽ��ϱ�? (1:�� 2:�ƴϿ�)\n");
				scanf("%d", &result);
				if (result == 1)
					show_result(dataSet, lineCount);
				else
					return;
				return;
			case 5:
				start = clock();
				radix_sorting_title(dataSet, lineCount, ' ', 'z');
				end = clock();
				time->radix_title = (double)(end - start);
				show_result(dataSet, lineCount);
				printf("\n\n����� ���ðڽ��ϱ�? (1:�� 2:�ƴϿ�)\n");
				scanf("%d", &result);
				if (result == 1)
					show_result(dataSet, lineCount);
				else
					return;
				return;
			default:
				printf("�߸� �Է��߽��ϴ�. �ٽ� �Է��ϼ��� \n");
				break;
			}
		}
		else if (*select == 1)
		{
			*select = 0;
			switch (number)
			{
			case 1:
				start = clock();
				insert_sorting_year(dataSet, lineCount);
				end = clock();
				time->insert_year = (double)(end - start);
				printf("\n\n����� ���ðڽ��ϱ�? (1:�� 2:�ƴϿ�)\n");
				scanf("%d", &result);
				if (result == 1)
					show_result(dataSet, lineCount);
				else
					return;
				return;
			case 2:

				start = clock();
				merge_year(dataSet, lineCount);
				end = clock();
				time->merge_year = (double)(end - start);
				printf("\n\n����� ���ðڽ��ϱ�? (1:�� 2:�ƴϿ�)\n");
				scanf("%d", &result);
				if (result == 1)
					show_result(dataSet, lineCount);
				else
					return;
				return;
			case 3:
				start = clock();
				quick_sorting_year(dataSet, lineCount);
				end = clock();
				time->quick_year = (double)(end - start);
				printf("\n\n����� ���ðڽ��ϱ�? (1:�� 2:�ƴϿ�)\n");
				scanf("%d", &result);
				if (result == 1)
					show_result(dataSet, lineCount);
				else
					return;
				return;
			case 4:
				start = clock();
				heap_sorting_year(dataSet, lineCount);
				end = clock();
				time->heap_year = (double)(end - start);
				printf("\n\n����� ���ðڽ��ϱ�? (1:�� 2:�ƴϿ�)\n");
				scanf("%d", &result);
				if (result == 1)
					show_result(dataSet, lineCount);
				else
					return;
				return;
			case 5:
				start = clock();
				radix_sorting_year(dataSet, lineCount, 4);
				end = clock();
				time->radix_year = (double)(end - start);
				printf("\n\n����� ���ðڽ��ϱ�? (1:�� 2:�ƴϿ�)\n");
				scanf("%d", &result);
				if (result == 1)
					show_result(dataSet, lineCount);
				else
					return;


				return;
			default:
				printf("�߸� �Է��߽��ϴ�. �ٽ� �Է��ϼ��� \n");
				break;
			}
		}
	}
}

void insert_sorting_year(DataSet* dataSet, int* lineCount)
{
	int j = 0;
	int i = 0;
	DataSet* key = NULL;
	key = (DataSet*)malloc(sizeof(DataSet)*(*lineCount));

	for (i = 1; i < *lineCount; i++)
	{
		memcpy(key, &(dataSet[i]), sizeof(DataSet));
		for (j = i - 1; j >= 0 && dataSet[j].year > key->year; j--)
		{
			memcpy(&(dataSet[j + 1]), &(dataSet[j]), sizeof(DataSet));
		}
		memcpy(&(dataSet[j + 1]), key, sizeof(DataSet));
	}
	free(key);
}
void insert_sorting_year_id(DataSet* dataSet, int* lineCount)
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
		for (j = i - 1; j >= 0 && dataSet[j].year_id > key->year_id; j--)
		{
			memcpy(&(dataSet[j + 1]), &(dataSet[j]), sizeof(DataSet));
		}
		//   dataSet[j + 1] = key;
		memcpy(&(dataSet[j + 1]), key, sizeof(DataSet));
	}
	end = time(NULL);

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

	DataSet* tmp = (DataSet*)malloc(sizeof(DataSet) * (*lineCount));
	quick_year(dataSet, 0, (*lineCount) - 1, tmp);
	free(tmp);

}
void sort_year_id(DataSet* dataSet, int lineCount)
{
	int i = 1;
	int a = 0;
	int count = 0;
	int b = 0;
	int j[2000] = { 0 };
	int k = 0;

	while (i < lineCount)
	{


		//tmp[i] = dataSet[i];
		if (dataSet[i].year_id == dataSet[i + 1].year_id)
		{
			continue;
		}
		else
		{
			j[k++] = i + 1;//�⵵�� �ε��� ����
			count++;
		}
		i++;
	}

	while (b < lineCount)
	{
		insert_sorting_year_id(dataSet, &j[b]);
		b++;
	}

}
void quick_year(DataSet* dataSet, int start, int end, DataSet* tmp)
{
	int pivot = 0;
	int pointer = 0;
	int i;
	if (start >= end)
		return 0;
	pivot = dataSet[end].year;
	pointer = start;

	for (i = start; i < end; i++)
	{
		if (dataSet[i].year < pivot)
		{
			if (pointer != i)
			{
				memcpy(tmp, &dataSet[i], sizeof(DataSet));
				memcpy(&dataSet[i], &dataSet[pointer], sizeof(DataSet));
				memcpy(&dataSet[pointer], tmp, sizeof(DataSet));
			}
			pointer++;
		}
	}
	memcpy(tmp, &dataSet[end], sizeof(DataSet));
	memcpy(&dataSet[end], &dataSet[pointer], sizeof(DataSet));
	memcpy(&dataSet[pointer], tmp, sizeof(DataSet));

	quick_year(dataSet, start, pointer - 1, tmp);
	quick_year(dataSet, pointer + 1, end, tmp);

}
int partition_year(DataSet* dataSet, int p, int r, DataSet* tmp)
{
	int x, i, j, k;
	x = dataSet[r].year;
	i = p - 1;
	for (j = p; j < r; j++)
	{
		if (dataSet[j].year <= x)
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
void heap_sorting_year(DataSet* dataSet, int* lineCount)
{
	int i = 0;
	DataSet* t = (DataSet*)malloc(sizeof(DataSet)*(*lineCount));
	build_max_heap_year(dataSet, lineCount);

	for (i = (*lineCount) - 1; i >= 0; i--)
	{
		memcpy(t, &dataSet[0], sizeof(DataSet));
		memcpy(&dataSet[0], &dataSet[i], sizeof(DataSet));
		memcpy(&dataSet[i], t, sizeof(DataSet));

		/*decrease the size of the heap*/
		heapify_year(dataSet, i, 0);

	}
	free(t);
}
void build_max_heap_year(DataSet* dataSet, int* lineCount)
{
	int i = 0;
	for (i = (*lineCount) / 2 - 1; i >= 0; i--)
		heapify_year(dataSet, *lineCount, i);
}
void heapify_year(DataSet* dataSet, int n, int i)
{
	DataSet* t = NULL;
	int largest = i;  // Initialize largest as root
	int l = 2 * i + 1;  // left = 2*i + 1
	int r = 2 * i + 2;  // right = 2*i + 2
	t = (DataSet*)malloc(sizeof(DataSet));

	// If left child is larger than root
	if (l < n && dataSet[l].year >= dataSet[largest].year)
	{
		if (dataSet[l].year > dataSet[largest].year)
		{
			largest = l;
		}
		else if (dataSet[l].year = dataSet[largest].year)
		{
			//if (dataSet[l].year_id > dataSet[largest].year_id && dataSet[l].title_id > dataSet[largest].title_id)
			if (dataSet[l].year_id < dataSet[largest].year_id)
			{
				largest = l; //���� �⵵�� stable ���߱� ����
			}
		}
	}

	// If right child is larger than largest
	if (r < n && dataSet[r].year >= dataSet[largest].year)
	{
		if (dataSet[r].year > dataSet[largest].year)
		{
			largest = r;
		}
		else if (dataSet[r].year = dataSet[largest].year)
		{
			//if (dataSet[r].year_id > dataSet[largest].year_id && dataSet[r].title_id > dataSet[largest].title_id)
			if (dataSet[r].year_id < dataSet[largest].year_id)
			{
				largest = r;
			}
		}
	}
	// If largest is not root
	if (largest != i)
	{
		//swap(&dataSet[i], &dataSet[largest]); /* swap elements*/
		memcpy(t, &dataSet[i], sizeof(DataSet));
		memcpy(&dataSet[i], &dataSet[largest], sizeof(DataSet));
		memcpy(&dataSet[largest], t, sizeof(DataSet));
		heapify_year(dataSet, n, largest); /* recursively call heapify*/
	}
	free(t);
}
void radix_sorting_year(DataSet* dataSet, int* lineCount, int maxLen)
{

	Queue buckets[BUCKET_NUM_YEAR];
	//DataSet tmp;
	int bi;
	int pos;
	int di;
	int divfac = 1;
	int radix;

	// �� 10���� ��Ŷ �ʱ�ȭ
	for (bi = 0; bi < BUCKET_NUM_YEAR; bi++)
		QueueInit(&buckets[bi]);

	// ���� �� �������� ���̸�ŭ �ݺ�
	for (pos = 0; pos < maxLen; pos++)
	{
		// ���� ����� ����ŭ �ݺ�
		for (di = 0; di < *lineCount; di++)
		{
			// N��° �ڸ��� ���� ����
			radix = (dataSet[di].year / divfac) % 10;

			// ������ ���ڸ� �ٰŷ� ������ ��Ŷ�� ����
			Enqueue(&buckets[radix], dataSet[di]);
		}

		// ��Ŷ ����ŭ �ݺ�
		for (bi = 0, di = 0; bi < BUCKET_NUM_YEAR; bi++)
		{
			// ��Ŷ�� ����� �� ������� �� ������ �ٽ� arr�� ����
			while (!QIsEmpty(&buckets[bi]))
			{
				dataSet[di++] = Dequeue(&buckets[bi]);
			}
		}

		// N��° �ڸ��� ���� ������ ���� �������� ����
		divfac *= 10;
	}

}
void QueueInit(Queue * pq)
{
	pq->front = NULL;
	pq->rear = NULL;
}
int QIsEmpty(Queue * pq)
{
	if (pq->front == NULL)
		return TRUE;
	else
		return FALSE;
}
void Enqueue(Queue * pq, DataSet data)
{
	Node * newNode = (Node*)malloc(sizeof(Node));
	newNode->next = NULL;
	newNode->data = data;

	if (QIsEmpty(pq))
	{
		pq->front = newNode;
		pq->rear = newNode;
	}
	else
	{
		pq->rear->next = newNode;
		pq->rear = newNode;
	}
}
DataSet Dequeue(Queue * pq)
{
	Node * delNode;
	DataSet retData;

	if (QIsEmpty(pq))
	{
		printf("Queue Memory Error!");
		exit(-1);
	}

	delNode = pq->front;
	retData = delNode->data;
	pq->front = pq->front->next;

	free(delNode);
	return retData;
}
void insert_sorting_title(DataSet* dataSet, int* lineCount)
{
	int j = 0;
	int i = 0;

	DataSet* key = NULL;
	DataSet* key2 = NULL;
	key = (DataSet*)malloc(sizeof(DataSet)*(*lineCount));
	key2 = (DataSet*)malloc(sizeof(DataSet)*(*lineCount));

	for (j = 1; j < *lineCount; j++)
	{

		memcpy(key, &dataSet[j], sizeof(DataSet));
		i = j - 1;

		while (i >= 0 && strcmp(dataSet[i].title, key->title) > 0)//�⵵ ���� ������stable�� �ʿ�
		{
			memcpy(&dataSet[i + 1], &dataSet[i], sizeof(DataSet));
			i = i - 1;
		}
		memcpy(&dataSet[i + 1], key, sizeof(DataSet));
	}
	free(key);
	free(key2);
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
	DataSet* tmp = (DataSet*)malloc(sizeof(DataSet) * (*lineCount));
	quick_title(dataSet, 0, (*lineCount) - 1, tmp);
	free(tmp);
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
int partition_title(DataSet* dataSet, int p, int r, DataSet* tmp)
{
	int i, j, k;
	char x[300] = { 0 };

	strcpy(tmp->title, dataSet[r].title);

	i = p - 1;
	for (j = p; j < r; j++)
	{
		k = strcmp(tmp->title, dataSet[j].title);
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
void heap_sorting_title(DataSet* dataSet, int* lineCount, int* year_stable)
{
	int i = 0;
	int size = *lineCount;
	DataSet* t = (DataSet*)malloc(sizeof(DataSet)*(*lineCount));


	build_max_heap_title(dataSet, size, year_stable);

	while (size > 0)
	{
		memcpy(t, &dataSet[0], sizeof(DataSet));
		memcpy(&dataSet[0], &dataSet[size - 1], sizeof(DataSet));
		memcpy(&dataSet[size - 1], t, sizeof(DataSet));

		size--;
		/*decrease the size of the heap*/
		heapify_title(dataSet, size, 0, year_stable);

	}
	free(t);
}
void build_max_heap_title(DataSet* dataSet, int lineCount, int* year_stable)
{
	int mid;
	for (mid = (lineCount) / 2 - 1; mid >= 0; mid--)
	{
		heapify_title(dataSet, lineCount, mid, year_stable);
	}
}
void heapify_title(DataSet* dataSet, int n, int mid, int* year_stable)
{
	DataSet* t = NULL;
	int tmp;
	int parent_node = mid;  // Initialize largest as root
	int left = 2 * parent_node + 1;  // left = 2*i + 1
	int right = 2 * parent_node + 2;  // right = 2*i + 2
	int largest = parent_node;;
	t = (DataSet*)malloc(sizeof(DataSet));



	// If left child is larger than root
	if (left < n)
	{
		tmp = strcmp(dataSet[left].title, dataSet[largest].title);
		if (tmp > 0)
		{
			largest = left;
		}
		else if (tmp == 0)
		{
			if (dataSet[left].title_id > dataSet[largest].title_id)
				largest = left;
		}
	}


	// If right child is larger than largest
	if (right < n)
	{
		tmp = strcmp(dataSet[right].title, dataSet[largest].title);
		if (tmp > 0)
		{
			largest = right;
		}
		else if (tmp == 0)
		{
			if (dataSet[right].title_id > dataSet[largest].title_id)
				largest = right;
		}

	}

	// If largest is not root
	if (largest != parent_node)
	{
		//swap(&dataSet[i], &dataSet[largest]); /* swap elements*/
		dataSet[largest].year_id = (*year_stable)++;
		memcpy(t, &dataSet[largest], sizeof(DataSet));
		memcpy(&dataSet[largest], &dataSet[parent_node], sizeof(DataSet));
		memcpy(&dataSet[parent_node], t, sizeof(DataSet));
		heapify_title(dataSet, n, largest, year_stable); /* recursively call heapify*/
	}
	free(t);
}
void radix_sorting_title(DataSet* dataSet, int* lineCount, char lower, char upper)
{
	int size = *lineCount;
	int maxIndex = 0;
	int i = 0;

	//���ڿ��� ���̰� ���� �� �� ã��
	for (i = 0; i < size; i++) {
		if (strlen(dataSet[i].title) - 1 > maxIndex) {
			maxIndex = strlen(dataSet[i].title) - 1;
		}
	}

	for (i = maxIndex; i >= 0; i--) {
		//���ڿ� �������� counting sort
		countingSort_title(dataSet, size, i, lower, upper);
	}

}
void countingSort_title(DataSet* dataSet, int size, int index, char lower, char upper)
{
	int countArray[96] = { 0 };
	int i;
	//�ӽ����� �迭 ����
	DataSet* tempArray = (DataSet*)malloc(sizeof(DataSet)*size);



	//���� size �Ű������� �Ѿ�� ���� �� ���ڿ����� ���� �ε��� ���� 0���� ä���� ���Ѵ�.
	for (i = 0; i < size; i++) {
		int charIndex = (strlen(dataSet[i].title) - 1 < index) ? 0 : ((dataSet[i].title[index] - lower) + 1);
		countArray[charIndex]++;
	}

	//sum up countArray;countArray will hold last index for the char at each strings index
	//����� ������ ���ϱ� 
	for (i = 1; i < 96; i++) {
		countArray[i] += countArray[i - 1];
	}

	//LSD ������� index ������ ������ 0 , ���ų� ũ�� ������ �ε������� ���� charIndex�� ���
	for (i = size - 1; i >= 0; i--) {
		int charIndex = (strlen(dataSet[i].title) - 1 < index) ? 0 : (dataSet[i].title[index] - lower) + 1;
		memcpy(&tempArray[countArray[charIndex] - 1], &dataSet[i], sizeof(DataSet));
		//�ӽ� �迭�� �����ϱ�
		countArray[charIndex]--;//������ ����
	}

	for (i = 0; i < size; i++) {
		memcpy(&dataSet[i], &tempArray[i], sizeof(DataSet));
		//�ӽ� �迭�� ����� ���� dataSet �迭�� ����
	}
	free(tempArray);
}
void merge_year(DataSet* dataSet, int* lineCount)
{
	DataSet* sortArr = (DataSet*)malloc(sizeof(DataSet)*(*lineCount));
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
	int fIdx = left;
	int rIdx = mid + 1;
	int k = left;
	int i;

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

}
void MergeTwoArea_title(DataSet* dataSet, int left, int mid, int right, DataSet* sortArr)
{
	int fIdx = left;
	int rIdx = mid + 1;
	int k = left;
	int i; int buf;

	while (fIdx <= mid && rIdx <= right)
	{
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
}
void show_result(DataSet* dataSet, int* lineCount)
{
	int i = 0;
	for (i = 0; i < *lineCount; i++)
	{
		//   printf("---------------------------------------------------------\n");
		   //printf("id: %d\n",dataSet[i].id);
		printf("��ȭ����:  %-40s             �⵵: %d\n", dataSet[i].title, dataSet[i].year);
		//printf("�帣: %s\n",dataSet[i].genre);
	 //   printf("---------------------------------------------------------\n");
	}
}