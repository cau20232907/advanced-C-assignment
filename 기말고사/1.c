#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define try_falied "메모리가 부족합니다."
#define file_falied "파일 열기에 실패해였습니다."
#define realloc_try 10 //malloc 재시도 횟수(malloc에 실패했을 때 사용)
#define flielocate "dict_test.TXT" //파일명
#define filelen 1348428 //파일 크기
#define fileline 51845 //파일 '\n' 수

int dictcmp(const char* (*words1)[2], const char* (*words2)[2]);
extern const char* sgets(int len, int relen, FILE* _Stream); //개행시까지 거의 무제한으로 문장을 입력받는 함수
extern void* calloc_in(const size_t len); //sgets 함수에서 배열 공간 최초 할당할 때 사용
extern void* realloc_in(void* to, const size_t len); //sgets 함수에서 배열 공간 연장할 때 사용
extern void freeset(void** p); //동적할당 해제에 사용
//return 자료형에는 char*(*)[2]을 설정할 수 없어 void형으로 임시 처리
void* dictread();
void* wordfinder(const char* str, const char* (*dict)[2]);
int dictlen(const char* (*dict)[2]);

void main() {
	char* (*dict)[2] = NULL;
	dict = dictread();
	if (!dict) return;
	qsort(dict, dictlen(dict), sizeof(char* [2]), dictcmp);
	printf("10000 : %s\n", (*(dict + 10000))[0]);
	printf("20000 : %s\n", (*(dict + 20000))[0]);
	printf("30000 : %s\n", (*(dict + 30000))[0]);
	printf("40000 : %s\n", (*(dict + 40000))[0]);
	
	freeset(&((*dict)[0]));
	freeset(&dict);
}

int dictcmp(const char* (*words1)[2], const char* (*words2)[2]) {
	const char* p1 = (*words1)[0], *p2 = (*words2)[0];
	while (tolower(*p1) == tolower(*p2) && *p1 != '\0') {
		p1++;
		p2++;
	}
	return tolower(*p1) - tolower(*p2);
}

void* dictread() {
	char* (*dict)[2] = NULL, * (*pdict)[2] = NULL; //char[flieline][2]의 포인터
	FILE* lib = NULL;
	char* tmp = NULL, * pos = NULL; //각각 string 저장, 단어 시작 위치 저장, 현재 커서 저장(변수 하나에 계속 입력받을 예정이므로 사용)
	size_t tmp_len = 0, dict_len = 0; //현재 tmp, dict의 할당받은 길이
	void* ptmp = NULL; // 포인터 변경 시 등에 활용하는 임시 포인터변수

	//최초할당 및 파일 열기
	tmp = (char*)calloc_in((filelen + 3) * sizeof(char)); //+2하면 재할당 대상이 됩니다.
	dict = calloc_in((fileline) * sizeof(char* [2]));
	lib = fopen(flielocate, "r");
	if (!lib || !tmp || !dict) {
		freeset(&tmp);
		freeset(&dict);
		if (lib) fclose(lib);
		else puts(file_falied);
		return NULL;
	}
	tmp_len += (filelen + 3);
	dict_len += (fileline);

	//입력
	pos = tmp;
	while (!feof(lib)) { //파일을 모두 읽었으면 break;
		fgets(pos, tmp_len - (pos - tmp), lib); //파일의 끝까지 계속 읽음
		pos += strlen(pos); //커서 이동, 첫 번째 NULL문자를 가리키게 됨
		if (tmp_len - (pos - tmp) <= 1) { //아직 문장을 다 받은 게 아니나 최대 용량에 도달한 경우(다만 계산 자체의 문제가 있는 게 아닌 이상 실행될 일은 없음, 최초 할당을 파일 크기 이상으로 받았기 때문)
			ptmp = realloc_in(tmp, tmp_len + sizeof(char));
			if (ptmp) {
				tmp = ptmp;
				pos = tmp + tmp_len - 1; //tmp의 위치가 변경될 수 있으니 pos 위치도 변경(길이가 최대로 찬 것이므로 기존 tmp_len-1(마지막 칸 '\0'부터 다음 입력 시작)을 가리키게 함
				tmp_len++; //realloc 성공시 현재 크기 변경
			}
			else break; //realloc 실패시 break(예외는 한번에 처리)
		}
	}

	fclose(lib);
	lib = NULL;
	if (!tmp) {
		freeset(&tmp);
		freeset(&dict);
		return NULL;
	}

	//사전 단어별로 나누기
	//사전 단어와 뜻 분리, 단어와 뜻의 구분이 ":"로 이루어져 있음(양쪽에 space까지 있음)을 고려, 단 뜻에는 띄어쓰기가 있음
	pdict = dict;
	(*pdict)[0] = strtok(tmp, ":"); //의미와 뜻을 가르는 ':'로 분리하기
	while ((*pdict)[0]) {
		if ((*pdict)[0][strlen((*pdict)[0]) - 1] == ' ')(*pdict)[0][strlen((*pdict)[0]) - 1] = '\0'; //영단어 뒤 공백 제거
		(*pdict)[1] = strtok(NULL, "\n");
		while (*((*pdict)[1]) == ' ') ((*pdict)[1])++; //뜻 앞 공백 제거
		pdict++;
		if (pdict - dict > dict_len) { //저장할 수 있는 최대 용량에 도달한 경우
			ptmp = realloc_in(dict, dict_len + sizeof(char* [2]));
			if (ptmp) {
				dict = ptmp;
				pdict = dict + dict_len;
				dict_len++; //realloc 성공시 현재 크기 변경
			}
			else break; //실패시 break;
		}
		(*pdict)[0] = strtok(NULL, ":");
	}
	if (!dict) {
		freeset(&tmp);
		freeset(&dict);
		return NULL;
	}
	return dict; //tmp의 주소가 (*dict)[0]에 저장되어 있어 별도로 넘겨줄 필요가 없음
}

void* wordfinder(const char* str, const char* (*dict)[2]) {
	//사전의 길이를 전달하지 않으므로 이진탐색을 사용하기 어려움(#define으로 정의한 건 파일 읽기에만 사용함)
	while ((*dict)[0] != NULL && strcmp(str, (*dict)[0])) dict++; //선형 탐색, 일치하는 단어 찾기
	//dict 공간에 여유를 주었으므로 사전 범위를 한 칸 넘는 걸로는 문제가 발생하지 않음
	if ((*dict)[0]) return dict; //일치하는 단어를 찾은 경우(단어의 첫번째 글자가 '\0'이 아닌 경우)
	else return NULL; //일치하는 단어가 없는 경우
}

int dictlen(const char* (*dict)[2]) { //사전의 길이 세기
	const char* (*tmp)[2] = dict;
	int res = 0;
	tmp = dict; //여기서는 dict2를 임시 변수로 활용
	while (((*tmp)[0]) != NULL) {
		res++;
		tmp++;
	}
	return res;
}

const char* sgets(int len, int relen, FILE* _Stream) { //할당받은 길이는 return하지 않음: 주어진 길이 외 사용 불가
	//len은 최초할당, relen은 재할당시 길이
	//FILE* _Stream 은 fgets에 그대로 넣을 것이고 이 이외의 활용은 없음(fgets의 매개변수를 이름까지 그대로 가져온 것)
	char* res = NULL, * ptmp = NULL, * pos = NULL; //각각 string 저장, 단어 시작 위치 저장, 현재 커서 저장(in 변수 하나에 계속 입력받을 예정이므로 사용), 포인터 변경 시 등에 활용하는 임시 포인터변수
	size_t res_len = 0; //현재 res의 할당받은 길이
	int tmp = 0;

	//최초할당
	res = (char*)calloc_in(len * sizeof(char));
	if (!res) return NULL;
	res_len += len;

	//입력
	pos = res;
	while (1) {
		fgets(pos, res_len - (pos - res), _Stream);
		pos += strlen(pos); //커서 이동, 첫 번째 NULL문자를 가리키게 됨
		if (*(pos - 1) == '\n') { //문장 끝이 개행문자면 입력이 끝났으므로 return
			*(pos - 1) = '\0'; //문장 끝 개행문자 삭제
			return res;
		}
		else { //아니면 아직 문장을 다 받은 게 아니고 최대 용량에 도달했으므로 realloc 시행
			ptmp = realloc_in(res, res_len + relen * sizeof(char));
			if (ptmp) {
				res = ptmp;
				pos = res + res_len - 1; //res의 위치가 변경될 수 있으니 pos 위치도 변경(길이가 최대로 찬 것이므로 기존 res_len-1(마지막 칸 '\0'부터 다음 입력 시작)을 가리키게 함
				res_len += relen; //realloc 성공시 현재 크기 변경
			}
			else { //realloc 실패시
				freeset(&res);
				return NULL;
			}
		}
	}
}

void* calloc_in(const size_t len) {
	//calloc 기능 + 실패시 재시도
	unsigned int cnt = 0;
	void* tmp = NULL;
	while (!tmp && cnt < realloc_try) {
		cnt++;
		tmp = malloc(len);
	}
	if (tmp) {
		memset(tmp, 0, len);
		return tmp;
	}
	else {
		puts(try_falied);
		return NULL;
	}
}

void* realloc_in(void* to, const size_t len) {
	//realloc 기능 + 실패시 재시도
	unsigned int cnt = 0;
	void* tmp = NULL;
	while (!tmp && cnt < realloc_try) {
		cnt++;
		tmp = realloc(to, len);
	}
	if (tmp) return tmp;
	else {
		puts(try_falied);
		return NULL;
	}
}

void freeset(void** p) {
	if (*p = NULL) {
		free(*p);
		*p = NULL;
	}
}