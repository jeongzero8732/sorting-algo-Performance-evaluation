#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

typedef struct str
{
	int id;
	int year;
	int year_id;//stable sorting을 위해
	char title[150];
	int title_id;//stable sorting 을 위해
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


/*메모리 초기화*/
//Sorting_Table* init_memory(Sorting_Table*,int);

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
void merge_sorting_year(DataSet*, int, int, DataSet*);
void MergeTwoArea_year(DataSet* dataSet, int left, int mid, int right, DataSet*);
void merge_year(DataSet*, int*);
void quick_sorting_year(DataSet*, int*);
void quick_year(DataSet* dataSet, int p, int r, DataSet* tmp);
int partition_year(DataSet* dataSet, int p, int r, DataSet* tmp);
void heap_sorting_year(DataSet*, int*);
void radix_sorting_year(DataSet*, int*);

/*타이틀 sorting 함수*/
void insert_sorting_title(DataSet*, int*);
void merge_title(DataSet* dataSet, int* lineCount);
void MergeTwoArea_title(DataSet* dataSet, int left, int mid, int right, DataSet*);
void merge_sorting_title(DataSet*, int*);
void quick_sorting_title(DataSet*, int*);
void quick_title(DataSet* dataSet, int p, int r, DataSet* tmp);
int partition_title(DataSet* dataSet, int p, int r, DataSet* tmp);
void heap_sorting_title(DataSet*, int*);
void radix_sorting_title(DataSet*, int*);

/*메뉴얼 함수*/
int menual(DataSet*, int*, int*);
void sorting_menual(DataSet*, int, int*, int*);


/*결과 출력*/
void show_result(DataSet*);

int main()
{
	printf("@@@%d@@@\n", sizeof(DataSet));
	int Datasize, i;
	int lineCount = -1;
	DataSet* dataSet = NULL;

	Datasize = Calc_line();//저장할 데이터 수 계산
	dataSet = (DataSet*)malloc(sizeof(DataSet) * Datasize);
	memset(dataSet, 0, sizeof(DataSet));


	file_read(dataSet, &lineCount); //파일에서 데이터 가져오기

	//for (int i = 0; i < lineCount; i++)
	//{
	//	printf("영화 제목 : %-40s 년도 : %d   \n", dataSet[i].title, dataSet[i].year);
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
	//printf("[[[%d\n",lineCount); lineCount와 Datasize-lineCount를 이용하여 제거된 데이터의 개수 확인 가능
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
	// 	  1.1 만약 head다음에 " 이게오면 저장x
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

int menual(DataSet* dataSet, int* lineCount, int* Datasize)
{
	int number = 0;
	for (;;)
	{
		//("cls");
		printf(" -----------------------------------시스템 메뉴얼------------------------------------\n");
		printf("					 																 \n");
		printf("		              1. 기존 파일 데이터 확인하기			                 \n");
		printf("		              2. 년도 기준으로 데이터 정렬하기		   		         \n");
		printf("		              3. 영화 제목 기준으로 데이터 정렬하기		         \n");
		printf("		              4. 시스템 종료			 						     \n");
		printf("																					 \n");
		printf(" ------------------------------------------------------------------------------------\n\n\n");

		printf("===> 번호를 입력하세요: ");
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
	printf(" -----------------------------------정렬 방법 선택------------------------------------\n");
	printf("					 																 \n");
	printf("		                     1. 삽입 정렬				                     \n");
	printf("		                     2. 합병 정렬	   				        		 \n");
	printf("		                     3. 퀵 정렬										 \n");
	printf("		                     4. 힙 정렬										 \n");
	printf("		                     5. 기수 정렬										 \n");
	printf(" ------------------------------------------------------------------------------------\n\n\n");

	for (;;)
	{
		printf("===> 번호를 입력하세요: ");
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
				printf("걸린시간:%.3lf\n", (double)(end - start));
				for (int i = 0; i < *lineCount; i++)
				{
					printf("영화 제목 : %-40s 년도 : %d    장르: %s \n", dataSet[i].title, dataSet[i].year, dataSet[i].genre);
				}
				return;
			case 3:
				start = clock();
				quick_sorting_year(dataSet, lineCount);
				end = clock();
				printf("걸린시간:%.3lf\n", (double)(end - start));
				for (int i = 0; i < *lineCount; i++)
				{
					printf("영화 제목 : %-40s 년도 : %d    장르: %s \n", dataSet[i].title, dataSet[i].year, dataSet[i].genre);
				}
				return;
			case 4:
				heap_sorting_year(dataSet, lineCount);
				return;
			case 5:
				radix_sorting_year(dataSet, lineCount);
				return;
			default:
				printf("잘못 입력했습니다. 다시 입력하세요 \n");
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
				printf("걸린시간:%.3lf\n", (double)(end - start));
				for (int i = 0; i < *lineCount; i++)
				{
					printf("영화 제목 : %-40s 년도 : %d    장르: %s \n", dataSet[i].title, dataSet[i].year, dataSet[i].genre);
				}
				return;
			case 3:
				start = time(NULL);
				quick_sorting_title(dataSet, lineCount);
				end = time(NULL);
				printf("걸린시간:%.3lf\n", (double)(end - start));
				for (int i = 0; i < *lineCount; i++)
				{
					printf("영화 제목 : %-40s 년도 : %d    장르: %s \n", dataSet[i].title, dataSet[i].year, dataSet[i].genre);
				}
				return;
			case 4:
				heap_sorting_title(dataSet, lineCount);
				return;
			case 5:
				radix_sorting_title(dataSet, lineCount);
				return;
			default:
				printf("잘못 입력했습니다. 다시 입력하세요 \n");
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
	printf("삽입 정렬 성공!!!!!      걸린시간 : %.3lf\n", (double)(end - start));
	for (int i = 0; i < *lineCount; i++)
	{
		printf("id: %d        영화 제목 : %-60s 년도 : %d    장르: %s \n", dataSet[i].id, dataSet[i].title, dataSet[i].year, dataSet[i].genre);
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

		// 둘로 나눠서 각각을 정렬한다.
		merge_sorting_year(dataSet, left, mid, sortArr);
		merge_sorting_year(dataSet, mid + 1, right, sortArr);

		// 정렬된 두 배열을 병합한다.
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
		if (dataSet[j].year <= x)//피벗과 하나씩 비교
		{
			//피벗과 같은 값이면 년도id 값을 비교하여 
			i = i + 1;
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
		printf("id: %d        영화 제목 : %-60s 년도 : %d    장르: %s \n", dataSet[i].id, dataSet[i].title, dataSet[i].year, dataSet[i].genre);
	}
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
	//1. 전체 파일 내용 확인하기 
	//2. 정렬된 파일 내용 확인하기
	//	2.1 정렬방법 선택
	//		2.1.1 년도 기준
	//		2.1.2 타이틀 기준
}