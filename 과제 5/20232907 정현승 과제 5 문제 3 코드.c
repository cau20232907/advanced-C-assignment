#include "20232907 ������ ���� 5 ���� ���.h"
#define sort_len 10 //���� �� ����ؾ� �ϴ� �ܾ��� ��

int lenfirst(const char* (*words1)[2], const char* (*words2)[2]);
int vowelfirst(const char* (*words1)[2], const char* (*words2)[2]);

void main3() {
	char* (*dict1)[2] = NULL, * (*dict2)[2] = NULL, * (*ptr)[2] = NULL;
	unsigned int dict_len = 0; //dict�� �迭 ���̸� �����ϱ� ���� ����(�� ���縦 ����)
	dict1 = dictread();
	if (!dict1) return;
	dict2 = dict1; //���⼭�� dict2�� �ӽ� ������ Ȱ��
	dict_len = dictlen(dict1);
	dict2 = calloc_in((dict_len + 1) * sizeof(char* [2]));
	if (!dict2) {
		freeset(&((*dict1)[0]));
		freeset(&dict1);
		return;
	}
	memcpy(dict2, dict1, (dict_len) * sizeof(char* [2]));
	//dict2�� �����Ҵ��� dict1 �迭�� ���� ���� ����(������ ������ ����), sort 2���� ����� ��� �����ϴ� �� ���
	//�̹� �ۼ��� ���� �Լ����� �����ϱ� �����Ƽ� ���� ���...

	qsort(dict1, dict_len, sizeof(char* [2]), lenfirst);
	qsort(dict2, dict_len, sizeof(char* [2]), vowelfirst);

	//���
	dict_len = dict_len < sort_len ? dict_len : sort_len;
	//�� �̻� ������� �ʴ� ���� Ȱ��, ������ ���̰� 10���� ���� ���� �����
	printf("�ܾ��� ���̰� �� ������ %d�� �ܾ�:\n", dict_len);
	for (ptr = dict1; ptr < dict1 + dict_len; ptr++)
		printf("%s %s\n", (*ptr)[0], (*ptr)[1]);

	printf("\n������ ���� ���� ���Ե� %d�� �ܾ�:\n", dict_len);
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
	int vowel = 0; //�ܾ��� ���� ��(�ܾ� 2 - �ܾ� 1) ����
	const char* word1 = (*words1)[0], * word2 = (*words2)[0]; //�ܾ� ��ġ�� ���� ����(++������ �����ϱ� ����)
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