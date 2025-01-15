#include "20232907 정현승 과제 3 공통 헤더.h"
#define end_in_str "end\n" //종료조건 string
#define malloc_strin_gap 201 //in의 malloc 단위(다 찼을 때 201칸만큼 보충)
#define malloc_word_gap 10 //word의 malloc 단위(다 찼을 때 10칸만큼 보충)
#define word_cut_chars " ,.-?\'\"\n\t"

int string_arr_cmp(const char** str1, const char** str2);

void main() {
	char* in = NULL, ** words = NULL, * pos = NULL; //각각 string와 word 저장, 단어 시작 위치 저장, 현재 커서 저장(in 변수 하나에 계속 입력받을 예정이므로 사용)
	void* ptmp = NULL; //포인터 변경 시 등에 활용하는 임시 포인터변수
	size_t in_len = 0, words_len = 0; //각 malloc의 현재 크기를 나타내는 변수
	unsigned int sentnum = 0, wordnum = 0, tmp = 0; //각각 문장수, 단어수, 임시변수(재시도 체크나 겹치는 단어 수 책정 등 시행)

	//in 최초할당
	in = (char*)calloc_in(malloc_strin_gap);
	if (!in) return;
	in_len += malloc_strin_gap;

	//입력
	tmp = 0;
	pos = in;
	while (tmp || strcmp(pos, end_in_str)) {
		//첫 체크시는 calloc으로 진행해서, ""으로 인식하기에 문제가 발생하지 않음
		fgets(pos, in_len - (pos - in), stdin);
		if (!tmp && !strcmp(pos, end_in_str)) break; //입력받은 문장이 end이면 탈출(tmp 조건은 후술)
		pos += strlen(pos); //커서 이동, 첫 번째 NULL문자를 가리키게 됨
		if (*(pos - 1) == '\n') {
			//문장 끝이 개행문자면 문장 수 세기
			sentnum++;
			tmp = 0;
		}
		else tmp++;
		//아니면 아직 문장을 다 받은 게 아니므로 세지 않음(이때 남은 게 end라서 다음 입력 받고 끝내는 일이 없도록 tmp 변수 활용)
		//공백인 경우는 제외하기 위해 isspace 함수를 사용하지 않음
		if (strlen(in) >= in_len - 6) { //최대 용량 도달로 인한 realloc 시행
			//용량까지 5개 문자 이내로 남은 경우에는, 남기는 했어도 용량을 더 받는데, 이는 end만을 입력했으나 공간 부족으로 잘리는 바람에 종료 조건으로 인식되지 않는 불상사를 막기 위함
			ptmp = realloc_in(in, in_len + malloc_strin_gap * sizeof(char));
			if (ptmp) {
				in = ptmp;
				pos = in + in_len - 1; //in의 위치가 변경될 수 있으니 pos 위치도 변경(길이가 최대로 찬 것이므로 기존 in_len-1(마지막 칸 '\0'부터 다음 입력 시작)을 가리키게 함
				in_len += malloc_strin_gap; //realloc 성공시 현재 크기 변경
			}
			else { //realloc 실패시
				freeset(&in);
				freeset(&words); //freeset 함수 내에서, 이미 NULL이면 아무런 일을 시행하지 않음
				return;
			}
		}
	}
	*(pos - 1) = '\0'; //end는 문장이 아니므로, end 직전의 개행문자(end가 단독으로 들어와야 하니 end 직전이 개행문자일 것)
	/* 필요없는 in 재할당
	ptmp = realloc_in(in, strlen(in) + 1);
	if(!ptmp) { //realloc 실패시
		freeset(&in);
		freeset(&words); //함수 내에서, 이미 NULL이면 아무런 일을 시행하지 않음
		return;
	}*/
	
	//words 최초할당
	tmp = 0;
	words = (char**)calloc_in(malloc_word_gap * sizeof(char*));
	if (!words) {
		freeset(&in);
		return;
	}
	words_len += malloc_word_gap;

	//단어 잘라서 저장
	ptmp = strtok(in, word_cut_chars); //in을 저장할 필요가 없으므로 그대로 사용, 최초 사용은 매개변수 in 사용
	while (ptmp) {
		words[wordnum] = ptmp;
		wordnum++;
		if (wordnum == words_len) {
			ptmp = realloc_in(words, words_len + malloc_word_gap * sizeof(char*));
			if (ptmp) {
				words = ptmp;
				words_len += malloc_word_gap; //realloc 성공시 현재 크기 변경
			}
			else { //realloc 실패시
				freeset(&in);
				freeset(&words); //freeset 함수 내에서, 이미 NULL이면 아무런 일을 시행하지 않음
				return;
			}
		}
		ptmp = strtok(NULL, word_cut_chars); //다음 반복 및 조건확인을 위해 strtok은 반복문 마지막에
	}
	//단어 정렬
	qsort(words, wordnum, sizeof(char*), string_arr_cmp);

	//중복 단어 찾기
	tmp = 0; //이번에는 중복된 단어 수 저장
	for (ptmp = words; (((char**)ptmp) + tmp + 1) < words + wordnum;) {
		if (!strcmp(*(char**)ptmp, ((char**)ptmp)[tmp + 1])) {
			((char**)ptmp)[tmp + 1] = NULL;
			tmp++;
		}
		else {
			((char**)ptmp)++;
			*((char**)ptmp) = ((char**)ptmp)[tmp];
			//중복 발생으로 인해 빈칸이 생기면 밀어줌
		}
	}

	printf("%d %d %d\n", sentnum, wordnum, wordnum - tmp);
	wordnum -= tmp; //tmp를 반복 횟수로 활용하기 위해 중복 제외한 단어 갯수를 wordnum으로 저장
	for (tmp = 0; tmp < wordnum; tmp++) puts(words[tmp]);
	freeset(&in);
	freeset(&words);
}

int string_arr_cmp(const char** str1, const char** str2) { //qsort에 넣을 함수
	return strcmp(*str1, *str2);
}