#include "20232907 정현승 과제 5 공통 헤더.h"
#define sort_len 10 //정렬 후 출력해야 하는 단어의 수

int lenfirst(const char* (*words1)[2], const char* (*words2)[2]);
int vowelfirst(const char* (*words1)[2], const char* (*words2)[2]);

void main3() {
	char* (*dict1)[2] = NULL, * (*dict2)[2] = NULL, * (*ptr)[2] = NULL;
	unsigned int dict_len = 0; //dict의 배열 길이를 저장하기 위한 변수(값 복사를 위함)
	dict1 = dictread();
	if (!dict1) return;
	dict2 = dict1; //여기서는 dict2를 임시 변수로 활용
	dict_len = dictlen(dict1);
	dict2 = calloc_in((dict_len + 1) * sizeof(char* [2]));
	if (!dict2) {
		freeset(&((*dict1)[0]));
		freeset(&dict1);
		return;
	}
	memcpy(dict2, dict1, (dict_len) * sizeof(char* [2]));
	//dict2를 동적할당해 dict1 배열의 값을 깊은 복사(포인터 값들을 복사), sort 2번의 결과를 모두 저장하는 데 사용
	//이미 작성한 공통 함수들을 수정하기 귀찮아서 나온 결과...

	qsort(dict1, dict_len, sizeof(char* [2]), lenfirst);
	qsort(dict2, dict_len, sizeof(char* [2]), vowelfirst);

	//출력
	dict_len = dict_len < sort_len ? dict_len : sort_len;
	//더 이상 사용하지 않는 변수 활용, 사전의 길이가 10보다 작을 때를 대비함
	printf("단어의 길이가 긴 순서로 %d개 단어:\n", dict_len);
	for (ptr = dict1; ptr < dict1 + dict_len; ptr++)
		printf("%s %s\n", (*ptr)[0], (*ptr)[1]);

	printf("\n모음이 가장 많이 포함된 %d개 단어:\n", dict_len);
	for (ptr = dict2; ptr < dict2 + dict_len; ptr++)
		printf("%s %s\n", (*ptr)[0], (*ptr)[1]);

	freeset(&((*dict1)[0]));
	freeset(&dict1);
	freeset(&dict2);
}

int lenfirst(const char* (*words1)[2], const char* (*words2)[2]) {
	return strlen((*words2)[0]) - strlen((*words1)[0]);
}

int vowelfirst(const char* (*words1)[2], const char* (*words2)[2]) {
	int vowel = 0; //단어의 모음 수(단어 2 - 단어 1) 저장
	const char* word1 = (*words1)[0], * word2 = (*words2)[0]; //단어 위치를 따로 저장(++연산을 진행하기 때문)
	while (*word1) {
		if (*word1 == 'a' || *word1 == 'e' || *word1 == 'i' || *word1 == 'o' || *word1 == 'u' ||
			*word1 == 'A' || *word1 == 'E' || *word1 == 'I' || *word1 == 'O' || *word1 == 'U')
			vowel--;
		word1++;
	}
	while (*word2) {
		if (*word2 == 'a' || *word2 == 'e' || *word2 == 'i' || *word2 == 'o' || *word2 == 'u' ||
			*word2 == 'A' || *word2 == 'E' || *word2 == 'I' || *word2 == 'O' || *word2 == 'U')
			vowel++;
		word2++;
	}
	return vowel;
}