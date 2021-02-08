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
	int year_id;
	char title[200];
	int title_id;
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
}Time_Calc; //시간측정

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
//큐 관련 구조체
typedef LQueue Queue;
void QueueInit(Queue * pq);
int QIsEmpty(Queue * pq);
void Enqueue(Queue * pq, DataSet data);
DataSet Dequeue(Queue * pq);
//큐 관련 함수

/*파일에서 데이터 읽어오는 함수*/
void file_read(DataSet*, int*);

/*파일의 크기 계산*/
int Calc_line();

/*영화제목,년도,장르, 인덱스 추출 함수*/
void export_movie_id(char*, DataSet*, IndexSet*, int*);
void export_index(char*, DataSet*, IndexSet*, int*);
void export_year_title(char*, DataSet*, IndexSet*, int*);

/*년도 sorting 함수*/
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

/*타이틀 sorting 함수*/
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

/*메뉴얼 함수*/
int menual(DataSet*, int*, int*, int*, int, Time_Calc*);
void sorting_menual(DataSet*, int*, int*, int*);

/*결과 출력*/
void show_result(DataSet*, int*);

int main()
{
	int Datasize, i;
	int lineCount = -1;
	int year_stable = 0;
	DataSet* dataSet = NULL; //데이터 저장을 위한 구조체

	Datasize = Calc_line();//저장할 데이터 수 계산
	dataSet = (DataSet*)malloc(sizeof(DataSet) * Datasize);
	memset(dataSet, 0, sizeof(DataSet));//구조체 초기화

	file_read(dataSet, &lineCount); //파일에서 데이터 가져오기

	sorting_menual(dataSet, &lineCount, &Datasize, &year_stable); //메뉴얼

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

	stream = fopen("movies_3000.txt", "r");

	if (stream == NULL)
	{
		printf("파일 오픈 실패!!\n");
		return;
	}

	while (fgets(read_line, 500, stream))
	{
		if ((*lineCount) == -1)
		{
			(*lineCount)++;
			continue;
		}
		/*읽어온 한줄을 가지고 영화 아이디 뽑는 함수
		   1.앞에서부터 읽어서 처음 콤마를 만나면 함수 탈출
		   2. 나올때 콤마의 인덱스(head)를 가지고 나옴
		   3. 여기서 영화 id 추출 가능  -->dataSet[lineCount].id 에 저장
		*/
		export_movie_id(read_line, dataSet, &indexSet, lineCount);


		/* 뒤에서부터 인덱스를 감소시키면서 콤마가 나올때 까지 찾는 함수
		   1.맨 뒤에서 부터 읽으면서 콤마가 나오면 그 위치의 인덱스(tail) 저장
			  1.1 여기서 장르 추출을 위한 인덱스 추출가능
			  1.2 만약 콤마가 나오기 전에 ) 이거 나오면 장르가 없다는 소리이므로 잠시 일시정지

		   2. 그다음 부터는 저장을 안하고 진행 - 그다음 년도가 나오게 되므로 )를 만나면 해당 인덱스(year_tail_index) 저장
		   3. 계속 년도 저장하다가 (를 만나면 그 위치의 인덱스(year_head_index)를 저장하고 함수 탈출
		   4. 여기서 년도 추출을 위한 인덱스 추출 가능
		*/
		export_index(read_line, dataSet, &indexSet, lineCount);


		/* 제목,년도,장르 추출함수
		   1. 인덱스 head부터 year_head_index-1까지 - 제목추출  : dataSet[lineCount].title에 저장, dataSet[lineCount].title_id 증가
			  1.1 만약 head다음에 " 이게오면 저장x
		   2. 인덱스 year_head_index+1 부터 year_tail_index-1 까지 - 년도 추출 -> atoi 함수로 정수로 바로 바꾸기 : dataSet[lineCount].year, dataSet[lineCount].year_id 증가
		   3. 인덱스 tail+1부터 라인의 끝까지 - 장르 추출 : dataSet[lineCount].genre
		*/
		export_year_title(read_line, dataSet, &indexSet, lineCount);

		(*lineCount)++;

		//인덱스 구조체 초기화
		memset(&indexSet, 0, sizeof(IndexSet));
	}
	fclose(stream);

}

int Calc_line()
{
	FILE* fp;
	char buf[500] = { 0 };
	int count = 0;
	fp = fopen("movies_3000.txt", "r");

	while (fgets(buf, sizeof(buf), fp))
	{
		count++;
	}
	fclose(fp);
	return count;
}//총 라인수
void export_movie_id(char* read_line, DataSet* dataSet, IndexSet* indexSet, int* lineCount)
{
	int index = 0;
	char buf[100] = { 0 };

	for (index = 0;; index++) //인덱스 0부터 콤마전까지 확인
	{
		if (read_line[index] == ',')
		{
			(*indexSet).head = index;//id다음 나오는 콤마의 위치인덱스
			break;
		}
		buf[index] = read_line[index];
	}
	dataSet[(*lineCount)].id = atoi(buf); //문자열 id값을 정수로 바꿔서 저장
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
			//라틴어? 읽으면 그 데이터 버리기
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

			(*indexSet).tail = index_behind - 1; //장르의 시작 인덱스 구함
			flag = 1;
		}
		else if (read_line[index_behind] == ')')//수정해야함. 장르가 없는 경우 콤마 오른쪽에 () 이거 올수 있음.
		{

			if (flag == 1)//콤마 다음에 나오는 년도의 끝을 의미하는 )
			{
				(*indexSet).year_tail_index = index_behind;//년도의 종료 인덱스
			}
			else // no genre를 표현하는 )
			{
				continue;
			}
		}
		else if (read_line[index_behind] == '(')//이것도 수정해야함. (no genre) 이코드는 이거 읽으면 종료됨
		{
			if (flag == 1)
			{
				(*indexSet).year_head_index = index_behind; //년도의 시작 인덱스
				return;
			}
			else
			{
				continue;
			}
		}
		else if (index_behind == ((*indexSet).head + 1))
		{
			//이떄 년도가 없다는 뜻 . 년도에 -1 대입
			//dataSet[*lineCount].year = -1;
			break;
		}
		//년도 가 없으면 () 이게 아예 없다. 
	}
}
void export_year_title(char* read_line, DataSet* dataSet, IndexSet* indexSet, int* lineCount)
{
	// 1. 인덱스 head부터 year_head_index - 1까지 - 제목추출  : dataSet[lineCount].title에 저장, dataSet[lineCount].title_id 증가
	//      1.1 만약 head다음에 " 이게오면 저장x
	int i = 0, j = 0;
	int year_index_buf;
	char title_buf[500] = { 0 };
	char year_buf[100] = { 0 };
	char genre_buf[100] = { 0 };

	year_index_buf = (*indexSet).year_head_index;

	if (read_line[(*indexSet).year_head_index - 1] == ' ') //제목과 년도 사이에 공백있으면 제거
	{
		year_index_buf = ((*indexSet).year_head_index) - 1;
	}

	for (i = (*indexSet).head + 1; i < year_index_buf; i++)
	{
		if (read_line[i] > 126 || read_line[i] < 0)
		{
			//라틴어? 만나면 그 데이터 버리기
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
		strcpy(dataSet[*lineCount].title, title_buf); //제목을 리얼 저장
		dataSet[*lineCount].title_id = *lineCount; //stable sorting을 위해
		j = 0;
	}

	// 2. 인덱스 year_head_index + 1 부터 year_tail_index - 1 까지 - 년도 추출->atoi 함수로 정수로 바로 바꾸기 : dataSet[lineCount].year, dataSet[lineCount].year_id 증가
	for (i = (*indexSet).year_head_index + 1; i < (*indexSet).year_tail_index; i++)
	{
		year_buf[j] = read_line[i];//년도 버퍼에 저장
		j++;
	}
	dataSet[*lineCount].year = atoi(year_buf);//년도를 리얼저장
	dataSet[*lineCount].year_id = *lineCount; //stable sorting을 위해


	j = 0;
	// 3. 인덱스 tail + 1부터 라인의 끝까지 - 장르 추출 : dataSet[lineCount].genre
	for (i = (*indexSet).tail + 2; i < strlen(read_line); i++)
	{
		if (read_line[i] == '\n')
			break;
		//문자 하나 읽어서 ( 이거면 장르 없는 거.
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

int menual(DataSet* dataSet, int* lineCount, int* Datasize, int* year_stable, int num, Time_Calc* time)
{
	int number = 0;
	int menu_flag = 0;
	int sort_count = 0;
	int result;
	int ii = 0;
	int dsize;
	clock_t start = 0, end = 0;

	DataSet* dataSet_buf = NULL; //임시 데이터 저장을 위한 구조체

	dsize = Calc_line();//저장할 데이터 수 계산
	dataSet_buf = (DataSet*)malloc(sizeof(DataSet) * dsize);

	while (ii <= (*lineCount))
	{
		memcpy(&dataSet_buf[ii], &dataSet[ii], sizeof(DataSet));
		ii++;
	}


	for (;;)
	{
		system("cls");
		if (menu_flag == 0)
		{

			printf(" -----------------------------------시스템 메뉴얼----------------------------------------------\n");
			printf("\n \t \t \t \t(정렬 순서 : 타이틀 -> 년도) \n");
			printf("\n\t\t\t\t1. 기존 파일 데이터 확인하기 \n");
			printf("\n\t\t\t\t2. 영화 제목 기준으로 데이터 정렬하기 \n");
			printf("\n\t\t\t\t3. 시스템 종료 \n");
			printf("\n---------------------------------------------------------------------------------------------\n");
		}
		else if (menu_flag == 1)
		{

			printf(" -----------------------------------시스템 메뉴얼----------------------------------------------\n");
			printf("\n \t \t \t \t(정렬 순서 : 타이틀 -> 년도) \n");
			printf("\n\t\t\t\t1. 기존 파일 데이터 확인하기 \n");
			printf("\n\t\t\t\t2. 년도 기준으로 데이터 정렬하기 \n");
			printf("\n\t\t\t\t3. 시스템 종료 \n");
			printf("\n---------------------------------------------------------------------------------------------\n");
		}



		printf("===> 번호를 입력하세요: ");
		scanf("%d", &number);

		switch (number)
		{
		case 1:

			show_result(dataSet_buf, lineCount);
			printf("계속 진행하려면 아무 키나 누르세요.!!!");
			getch();
			break;
		case 2: //
			if (menu_flag == 0)
			{//타이틀
				menu_flag = 1;
				if (num == 1)//삽입-타이틀
				{
					start = clock();
					insert_sorting_title(dataSet, lineCount);
					end = clock();
					time->insert_title = (double)(end - start);

					printf("\n\n결과를 보시겠습니까? (1:예 2:아니요)\n");
					scanf("%d", &result);
					if (result == 1)
					{
						show_result(dataSet, lineCount);
						printf("계속 진행하려면 아무 키나 누르세요.!!!");
						getch();
					}


				}
				else if (num == 2)//합병-타이틀
				{
					start = clock();
					merge_title(dataSet, lineCount);
					end = clock();
					time->merge_title = (double)(end - start);

					printf("\n\n결과를 보시겠습니까? (1:예 2:아니요)\n");
					scanf("%d", &result);
					if (result == 1)
					{
						show_result(dataSet, lineCount);
						printf("계속 진행하려면 아무 키나 누르세요.!!!");
						getch();
					}
				}
				else if (num == 3)//퀵-타이틀
				{
					start = clock();
					quick_sorting_title(dataSet, lineCount);
					end = clock();
					time->quick_title = (double)(end - start);

					printf("\n\n결과를 보시겠습니까? (1:예 2:아니요)\n");
					scanf("%d", &result);
					if (result == 1)
					{
						show_result(dataSet, lineCount);
						printf("계속 진행하려면 아무 키나 누르세요.!!!");
						getch();
					}

				}
				else if (num == 4)//힙-타이틀
				{
					start = clock();
					heap_sorting_title(dataSet, lineCount, year_stable);
					end = clock();
					time->heap_title = (double)(end - start);

					printf("\n\n결과를 보시겠습니까? (1:예 2:아니요)\n");
					scanf("%d", &result);
					if (result == 1)
					{
						show_result(dataSet, lineCount);
						printf("계속 진행하려면 아무 키나 누르세요.!!!");
						getch();
					}

				}
				else if (num == 5)//기수-타이틀
				{
					start = clock();
					radix_sorting_title(dataSet, lineCount, ' ', 'z');
					end = clock();
					time->radix_title = (double)(end - start);


					printf("\n\n결과를 보시겠습니까? (1:예 2:아니요)\n");
					scanf("%d", &result);
					if (result == 1)
					{
						show_result(dataSet, lineCount);
						printf("계속 진행하려면 아무 키나 누르세요.!!!");
						getch();
					}

				}

				break;
			}
			else if (menu_flag == 1)
			{//년도
				menu_flag = 0;

				if (num == 1)//삽입-년도
				{
					start = clock();
					insert_sorting_year(dataSet, lineCount);
					end = clock();
					time->insert_year = (double)(end - start);

					printf("\n\n결과를 보시겠습니까? (1:예 2:아니요)\n");
					scanf("%d", &result);
					if (result == 1)
					{
						show_result(dataSet, lineCount);
						printf("계속 진행하려면 아무 키나 누르세요.!!!");
						getch();
					}

				}
				else if (num == 2)//합병-년도
				{
					start = clock();
					merge_year(dataSet, lineCount);
					end = clock();
					time->merge_year = (double)(end - start);

					printf("\n\n결과를 보시겠습니까? (1:예 2:아니요)\n");
					scanf("%d", &result);
					if (result == 1)
					{
						show_result(dataSet, lineCount);
						printf("계속 진행하려면 아무 키나 누르세요.!!!");
						getch();
					}

				}
				else if (num == 3)//퀵-년도
				{
					start = clock();
					quick_sorting_year(dataSet, lineCount);
					end = clock();
					time->quick_year = (double)(end - start);

					printf("\n\n결과를 보시겠습니까? (1:예 2:아니요)\n");
					scanf("%d", &result);
					if (result == 1)
					{
						show_result(dataSet, lineCount);
						printf("계속 진행하려면 아무 키나 누르세요.!!!");
						getch();
					}

				}
				else if (num == 4)//힙-년도
				{
					start = clock();
					heap_sorting_year(dataSet, lineCount);
					end = clock();
					time->heap_year = (double)(end - start);

					printf("\n\n결과를 보시겠습니까? (1:예 2:아니요)\n");
					scanf("%d", &result);
					if (result == 1)
					{
						show_result(dataSet, lineCount);
						printf("계속 진행하려면 아무 키나 누르세요.!!!");
						getch();
					}

				}
				else if (num == 5)//기수-년도
				{
					start = clock();
					radix_sorting_year(dataSet, lineCount, 4);
					end = clock();
					time->radix_year = (double)(end - start);

					printf("\n\n결과를 보시겠습니까? (1:예 2:아니요)\n");
					scanf("%d", &result);
					if (result == 1)
					{
						show_result(dataSet, lineCount);
						printf("계속 진행하려면 아무 키나 누르세요.!!!");
						getch();
					}

				}

				(*lineCount) = -1;
				memset(dataSet, 0, sizeof(DataSet));
				file_read(dataSet, lineCount); //두번 정렬이 끝나면 다시 새롭게 파일에서 데이터 가져오기 - 기존 정렬 안된 상태로

				return;
			}
		case 3:
			return;
		default:
			printf("다시입력하세요!!\n");
			break;
		}
	}

}
void sorting_menual(DataSet* dataSet, int* lineCount, int* Datasize, int* year_stable)
{
	int result = 0;
	int number = 0;
	Time_Calc time = { 0 };

	for (;;)
	{
		system("cls");
		printf(" -----------------------------------정렬 방법 선택----------------------------------------------------------------------\n");
		printf("\n\t\t\t\t1. 삽입 정렬 \n");
		printf("\n\t\t\t\t2. 합병 정렬 \n");
		printf("\n\t\t\t\t3. 퀵 정렬 \n");
		printf("\n\t\t\t\t4. 힙 정렬 \n");
		printf("\n\t\t\t\t5. 기수 정렬 \n");
		printf("\n\t\t\t\t0. 나가기\n");
		printf("\n------------------------------------------------------------------------------------------------------------------------\n\n\n");

		printf(" -----------------------------------------------------------------------------------------------------------------------\n");
		printf("\n\t\tl\t 종류\t  ㅣ\t제목 정렬\t\t\t|\t년도 정렬  \tl\t\t\n");
		printf(" -----------------------------------------------------------------------------------------------------------------------\n");
		printf("\n\t\t\t삽입정렬  ㅣ\t  %.4lf밀리초\t\t\t %.4lf밀리초 \t\n", time.insert_title, time.insert_year);
		printf("\n\t\t\t합병정렬  ㅣ\t  %.4lf밀리초\t\t\t %.4lf밀리초 \t\n", time.merge_title, time.merge_year);
		printf("\n\t\t\t  퀵정렬  ㅣ\t  %.4lf밀리초\t\t\t %.4lf밀리초 \t\n", time.quick_title, time.quick_year);
		printf("\n\t\t\t  힙정렬  ㅣ\t  %.4lf밀리초\t\t\t %.4lf밀리초 \t\n", time.heap_title, time.heap_year);
		printf("\n\t\t\t기수정렬  ㅣ\t  %.4lf밀리초\t\t\t %.4lf밀리초 \t\n", time.radix_title, time.radix_year);
		printf("\n----------------------------------------------------------------------------------------------------------------------\n\n\n");


		printf("===> 번호를 입력하세요: ");
		scanf("%d", &number);

		if (number != 1 && number != 2 && number != 3 && number != 4 && number != 5 && number != 0)
		{
			printf("다시 입력하세요\n");
			continue;
		}
		else if (number == 0)
		{
			return;
		}
		else if (number == 1)
		{

		}

		menual(dataSet, lineCount, Datasize, year_stable, number, &time);//메뉴얼


	}
}

void insert_sorting_year(DataSet* dataSet, int* lineCount)
{
	int j = 0;
	int i = 0;
	DataSet* key = NULL;
	key = (DataSet*)malloc(sizeof(DataSet)*(*lineCount));
	//키값 저장을 위한 동적할당
	for (i = 1; i < *lineCount; i++)
	{
		memcpy(key, &(dataSet[i]), sizeof(DataSet));
		//좌측에서 시작한 키값을 우측으로 이동시키며 키값 기준 좌측 값을 비교
		for (j = i - 1; j >= 0 && dataSet[j].year > key->year; j--)
		{
			memcpy(&(dataSet[j + 1]), &(dataSet[j]), sizeof(DataSet));
			//옆으로 한칸씩 밀기
		}
		memcpy(&(dataSet[j + 1]), key, sizeof(DataSet));//키값을 j+1 인덱스에 저장
	}
	free(key);
}
void merge_sorting_year(DataSet* dataSet, int left, int right, DataSet* sortArr)
{
	int mid;
	if (left < right)
	{
		// 중간 지점을 계산한다.
		mid = (left + right) / 2;

		// 둘로 나눠서 각각을 정렬한다. divide
		merge_sorting_year(dataSet, left, mid, sortArr);
		merge_sorting_year(dataSet, mid + 1, right, sortArr);

		// 분할된 두 배열을 병합한다. qunquer
		MergeTwoArea_year(dataSet, left, mid, right, sortArr);
	}
}
void quick_sorting_year(DataSet* dataSet, int* lineCount)
{

	DataSet* tmp = (DataSet*)malloc(sizeof(DataSet) * (*lineCount));
	quick_year(dataSet, 0, (*lineCount) - 1, tmp);
	free(tmp);

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
	//피벗은 맨 우측으로 잡고 시작

	//피벗 값 찾기
	for (i = start; i < end; i++)
	{
		if (dataSet[i].year < pivot)
		{//피벗 보다 작으면 tmp 저장 후 pointer 인덱스에 저장
			if (pointer != i)
			{
				memcpy(tmp, &dataSet[i], sizeof(DataSet));
				memcpy(&dataSet[i], &dataSet[pointer], sizeof(DataSet));
				memcpy(&dataSet[pointer], tmp, sizeof(DataSet));
			}
			pointer++;
		}
	}
	//피벗과 pointer가 가리키는 대상 교환
	memcpy(tmp, &dataSet[end], sizeof(DataSet));
	memcpy(&dataSet[end], &dataSet[pointer], sizeof(DataSet));
	memcpy(&dataSet[pointer], tmp, sizeof(DataSet));

	quick_year(dataSet, start, pointer - 1, tmp);
	quick_year(dataSet, pointer + 1, end, tmp);

}
void heap_sorting_year(DataSet* dataSet, int* lineCount)
{
	int i = 0;
	DataSet* t = (DataSet*)malloc(sizeof(DataSet)*(*lineCount));
	build_max_heap_year(dataSet, lineCount); //힙 구조 만들기

	for (i = (*lineCount) - 1; i >= 0; i--)
	{
		//루트값 빼서 맨 뒤 저장,  맨 마지막 값 올리기
		memcpy(t, &dataSet[0], sizeof(DataSet));
		memcpy(&dataSet[0], &dataSet[i], sizeof(DataSet));
		memcpy(&dataSet[i], t, sizeof(DataSet));

		//힙 사이즈 감소하고 다시 깨진 힙구조 다시 재구성
		heapify_year(dataSet, i, 0);

	}
	free(t);
}
void build_max_heap_year(DataSet* dataSet, int* lineCount)
{
	//힙 구조를 만들기 --> 전체크기의 / 2 부터 bottom up으로 비교 // 리프노드제외 부모노드부터 시작
	int i = 0;
	for (i = (*lineCount) / 2 - 1; i >= 0; i--)
		heapify_year(dataSet, *lineCount, i);
}
void heapify_year(DataSet* dataSet, int n, int i)
{
	DataSet* t = NULL;
	int largest = i;
	int l = 2 * i + 1;  // left = 2*i + 1
	int r = 2 * i + 2;  // right = 2*i + 2
	t = (DataSet*)malloc(sizeof(DataSet));

	// 좌측 자식이 더크면
	if (l < n && dataSet[l].year > dataSet[largest].year)
	{
		largest = l;
	}
	//힙을 stable 하게 만들기 위한 코드
	//if (l < n && dataSet[l].year >= dataSet[largest].year)
	//{
	//	if (dataSet[l].year > dataSet[largest].year)
	//	{
	//		largest = l;
	//	}
	//	else if (dataSet[l].year = dataSet[largest].year)
	//	{
	//		//if (dataSet[l].year_id > dataSet[largest].year_id && dataSet[l].title_id > dataSet[largest].title_id)
	//		if (dataSet[l].year_id < dataSet[largest].year_id)
	//		{
	//			largest = l; //같은 년도의 stable 맞추기 위한
	//		}
	//	}
	//}

	// 우측 자식이 더크면
	if (r < n && dataSet[r].year > dataSet[largest].year)
	{
		largest = r;
	}

	//stable 하게 만들기위한 코드 부분
	//if (r < n && dataSet[r].year >= dataSet[largest].year)
	//{
	//	if (dataSet[r].year > dataSet[largest].year)
	//	{
	//		largest = r;
	//	}
	//	else if (dataSet[r].year = dataSet[largest].year)
	//	{
	//		//if (dataSet[r].year_id > dataSet[largest].year_id && dataSet[r].title_id > dataSet[largest].title_id)
	//		if (dataSet[r].year_id < dataSet[largest].year_id)
	//		{
	//			largest = r;
	//		}
	//	}
	//}

	// 루트가 아니면 
	if (largest != i)
	{
		//i 와 t 가 가리키는 대상 교환
		memcpy(t, &dataSet[i], sizeof(DataSet));
		memcpy(&dataSet[i], &dataSet[largest], sizeof(DataSet));
		memcpy(&dataSet[largest], t, sizeof(DataSet));
		heapify_year(dataSet, n, largest); // 힙 재구성
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

	// 총 10개의 버킷 초기화
	for (bi = 0; bi < BUCKET_NUM_YEAR; bi++)
		QueueInit(&buckets[bi]);

	// 가장 긴 데이터의 길이만큼 반복 --> 여기서는 4로 고정
	for (pos = 0; pos < maxLen; pos++)
	{
		// 정렬 대상의 수만큼 반복
		for (di = 0; di < *lineCount; di++)
		{
			// N번째 자리의 숫자 추출
			radix = (dataSet[di].year / divfac) % 10;

			// 추출한 숫자를 근거로 데이터 버킷에 저장
			Enqueue(&buckets[radix], dataSet[di]);
		}

		// 버킷 수만큼 반복
		for (bi = 0, di = 0; bi < BUCKET_NUM_YEAR; bi++)
		{
			// 버킷에 저장된 것 순서대로 다 꺼내서 다시 arr에 저장
			while (!QIsEmpty(&buckets[bi]))
			{
				dataSet[di++] = Dequeue(&buckets[bi]);
			}
		}

		// N번째 자리의 숫자 추출을 위한 피제수의 증가
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
	key = (DataSet*)malloc(sizeof(DataSet)*(*lineCount)); //비교를 위해 동적할당

	for (j = 1; j < *lineCount; j++)
	{

		memcpy(key, &dataSet[j], sizeof(DataSet));
		i = j - 1;

		while (i >= 0 && strcmp(dataSet[i].title, key->title) > 0) //key값과 계속 비교하여 
		{
			memcpy(&dataSet[i + 1], &dataSet[i], sizeof(DataSet)); //키 값보다 크다면 옆으로 계속 밀기
			i = i - 1;
		}
		memcpy(&dataSet[i + 1], key, sizeof(DataSet)); // 키 값을 해당 위치로 저장
	}
	free(key);
}
void merge_sorting_title(DataSet* dataSet, int left, int right, DataSet* sortArr)
{
	int mid;
	if (left < right)
	{
		// 중간 지점을 계산한다.
		mid = (left + right) / 2;

		// 둘로 나눠서 각각을 정렬한다.
		merge_sorting_title(dataSet, left, mid, sortArr);
		merge_sorting_title(dataSet, mid + 1, right, sortArr);

		// 정렬된 두 배열을 병합한다.
		MergeTwoArea_title(dataSet, left, mid, right, sortArr);
	}
}
void quick_sorting_title(DataSet* dataSet, int* lineCount)
{
	DataSet* tmp = (DataSet*)malloc(sizeof(DataSet) * (*lineCount));//저장할 임시 공간 재구호출안에서 만들면 동적할당에 많은 시간 소비
	quick_title(dataSet, 0, (*lineCount) - 1, tmp);
	free(tmp);
}
void quick_title(DataSet* dataSet, int p, int r, DataSet* tmp)
{

	int q;

	if (p < r)
	{
		q = partition_title(dataSet, p, r, tmp);//피벗 저장
		quick_title(dataSet, p, q - 1, tmp); //피벗 기준 좌측 재귀진행
		quick_title(dataSet, q + 1, r, tmp);//우측 진행
	}
}
int partition_title(DataSet* dataSet, int p, int r, DataSet* tmp)
{
	int i, j, k;
	char x[300] = { 0 };

	strcpy(tmp->title, dataSet[r].title);

	i = p - 1;//피벗을 맨좌측 값으로 설정
	for (j = p; j < r; j++)
	{
		k = strcmp(tmp->title, dataSet[j].title);//피벗보다 큰 값은 우측 작은 값은 좌측
		if (k >= 0)//피벗 보다 크면
		{
			i = i + 1; //해당인덱스를 j 데이터와 스왑하기 위한
			memcpy(tmp, &dataSet[i], sizeof(DataSet));//임시저장
			memcpy(&dataSet[i], &dataSet[j], sizeof(DataSet));//임시저장
			memcpy(&dataSet[j], tmp, sizeof(DataSet));//임시저장
		}
	}
	memcpy(tmp, &dataSet[i + 1], sizeof(DataSet));//임시저장
	memcpy(&dataSet[i + 1], &dataSet[r], sizeof(DataSet));//임시저장
	memcpy(&dataSet[r], tmp, sizeof(DataSet));//임시저장

	return i + 1;
}
void heap_sorting_title(DataSet* dataSet, int* lineCount, int* year_stable)
{
	int i = 0;
	int size = *lineCount;
	DataSet* t = (DataSet*)malloc(sizeof(DataSet)*(*lineCount));


	build_max_heap_title(dataSet, size, year_stable); //맥스힙구조 만들기

	while (size > 0)
	{
		memcpy(t, &dataSet[0], sizeof(DataSet));
		memcpy(&dataSet[0], &dataSet[size - 1], sizeof(DataSet));
		memcpy(&dataSet[size - 1], t, sizeof(DataSet));
		//루트와 맨 마지막 데이터 스왑
		size--;//힙사이즈 감소

		heapify_title(dataSet, size, 0, year_stable);
		//힙구조가 깨지니 재구성
	}
	free(t);
}
void build_max_heap_title(DataSet* dataSet, int lineCount, int* year_stable)
{
	int mid;
	for (mid = (lineCount) / 2 - 1; mid >= 0; mid--)
	{	//밑에서 부터 진행
		heapify_title(dataSet, lineCount, mid, year_stable);
	}
}
void heapify_title(DataSet* dataSet, int n, int mid, int* year_stable)
{
	DataSet* t = NULL;
	int tmp;
	int parent_node = mid;  //루트를 가장 큰 값으로  초기화
	int left = 2 * parent_node + 1;  // 왼쪽 자식
	int right = 2 * parent_node + 2;  // 오른쪽 자식
	int largest = parent_node;;
	t = (DataSet*)malloc(sizeof(DataSet));

	//왼쪽 자식이 더크면
	if (left < n)
	{
		tmp = strcmp(dataSet[left].title, dataSet[largest].title);
		if (tmp > 0)
		{
			//left 인덱스를 largetst로 하기
			largest = left;
		}
	}


	// 오른쪽 자식이 더크면
	if (right < n)
	{
		tmp = strcmp(dataSet[right].title, dataSet[largest].title);
		if (tmp > 0)
		{
			//right 인덱스를 largest로 하기
			largest = right;
		}

	}

	// largest가 root가 아니면
	if (largest != parent_node)
	{
		dataSet[largest].year_id = (*year_stable)++;
		memcpy(t, &dataSet[largest], sizeof(DataSet));
		memcpy(&dataSet[largest], &dataSet[parent_node], sizeof(DataSet));
		memcpy(&dataSet[parent_node], t, sizeof(DataSet));
		heapify_title(dataSet, n, largest, year_stable);
		//다시 밑에 자식도 수정
	}
	free(t);
}
void radix_sorting_title(DataSet* dataSet, int* lineCount, char lower, char upper)
{
	int size = *lineCount;
	int maxIndex = 0;
	int i = 0;

	//문자열의 길이가 가장 긴 것 찾기
	for (i = 0; i < size; i++) {
		if (strlen(dataSet[i].title) - 1 > maxIndex) {
			maxIndex = strlen(dataSet[i].title) - 1;
		}
	}

	for (i = maxIndex; i >= 0; i--) {
		//문자열 기준으로 counting sort
		countingSort_title(dataSet, size, i, lower, upper);
	}

}
void countingSort_title(DataSet* dataSet, int size, int index, char lower, char upper)
{
	int countArray[96] = { 0 };
	int i;
	//임시저장 배열 선언
	DataSet* tempArray = (DataSet*)malloc(sizeof(DataSet)*size);



	//만약 size 매개변수로 넘어온 가장 긴 문자열보다 작은 인덱스 값은 0으로 채워서 비교한다.
	for (i = 0; i < size; i++) {
		int charIndex = (strlen(dataSet[i].title) - 1 < index) ? 0 : ((dataSet[i].title[index] - lower) + 1);
		countArray[charIndex]++;
	}

	//계수의 누적합 구하기 
	for (i = 1; i < 96; i++) {
		countArray[i] += countArray[i - 1];
	}

	//LSD 방식으로 index 값보다 작으면 0 , 같거나 크면 마지막 인덱스부터 빼서 charIndex에 담기
	for (i = size - 1; i >= 0; i--) {
		int charIndex = (strlen(dataSet[i].title) - 1 < index) ? 0 : (dataSet[i].title[index] - lower) + 1;
		memcpy(&tempArray[countArray[charIndex] - 1], &dataSet[i], sizeof(DataSet));
		//임시 배열에 저장하기
		countArray[charIndex]--;//누적합 감소
	}

	for (i = 0; i < size; i++) {
		memcpy(&dataSet[i], &tempArray[i], sizeof(DataSet));
		//임시 배열에 저장된 값을 dataSet 배열에 복사
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
	DataSet* sortArr = (DataSet*)malloc(sizeof(DataSet) * (*lineCount));
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
		//병합 할 두 영역의 데이터들 비교하여
		//순서대로 sortArr에 담기
		if (dataSet[fIdx].year <= dataSet[rIdx].year)
			memcpy(&sortArr[k++], &dataSet[fIdx++], sizeof(DataSet));
		else
			memcpy(&sortArr[k++], &dataSet[rIdx++], sizeof(DataSet));
	}

	while (fIdx <= mid) // 배열의 뒷부분이 다 sortArr로 옮겨지면 나머지 옮기기
		memcpy(&sortArr[k++], &dataSet[fIdx++], sizeof(DataSet));
	while (rIdx <= right)// 배열의 앞부분이  다 옮겨졌다면 나머지 옮기기
		memcpy(&sortArr[k++], &dataSet[rIdx++], sizeof(DataSet));

	for (i = left; i <= right; i++)
		memcpy(&dataSet[i], &sortArr[i], sizeof(DataSet));
	//임시배열 전부 기존 구조체에 복사

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
		//분할된 데이터를 비교하여 

		if (buf >= 0)
			memcpy(&sortArr[k++], &dataSet[fIdx++], sizeof(DataSet));//인덱스 rIdx가 더크면
		else
			memcpy(&sortArr[k++], &dataSet[rIdx++], sizeof(DataSet));//인덱스 fIdx가 더크면
	}

	while (fIdx <= mid)//좌측 값이 아직 남았다면 나머지 저장
		memcpy(&sortArr[k++], &dataSet[fIdx++], sizeof(DataSet));
	while (rIdx <= right)//우측 값이 아직 남았다면 나머지 저장
		memcpy(&sortArr[k++], &dataSet[rIdx++], sizeof(DataSet));

	for (i = left; i <= right; i++)
		memcpy(&dataSet[i], &sortArr[i], sizeof(DataSet)); //임시저장한 데이터를 기존 구조체에 삽입
}
void show_result(DataSet* dataSet, int* lineCount)
{
	int i = 0;
	for (i = 0; i < *lineCount; i++)
	{
		   printf("--------------------------------------------------------------------------------------------\n");
		   printf("id: %d\n",dataSet[i].id);
		   printf("타이틀: %s\n", dataSet[i].title);
		   printf("년도: %d\n", dataSet[i].year);
	//	printf("목록: %d,  %d   영화제목:  %-40s             년도: %d\n", dataSet[i].id, dataSet[i].title_id, dataSet[i].title, dataSet[i].year);
		   printf("장르: %s\n",dataSet[i].genre);
	       printf("--------------------------------------------------------------------------------------------\n");
	}
}