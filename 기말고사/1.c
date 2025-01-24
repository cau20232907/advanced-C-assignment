#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define try_falied "�޸𸮰� �����մϴ�."
#define file_falied "���� ���⿡ �����ؿ����ϴ�."
#define realloc_try 10 //malloc ��õ� Ƚ��(malloc�� �������� �� ���)
#define flielocate "dict_test.TXT" //���ϸ�
#define filelen 1348428 //���� ũ��
#define fileline 51845 //���� '\n' ��

int dictcmp(const char* (*words1)[2], const char* (*words2)[2]);
extern const char* sgets(int len, int relen, FILE* _Stream); //����ñ��� ���� ���������� ������ �Է¹޴� �Լ�
extern void* calloc_in(const size_t len); //sgets �Լ����� �迭 ���� ���� �Ҵ��� �� ���
extern void* realloc_in(void* to, const size_t len); //sgets �Լ����� �迭 ���� ������ �� ���
extern void freeset(void** p); //�����Ҵ� ������ ���
//return �ڷ������� char*(*)[2]�� ������ �� ���� void������ �ӽ� ó��
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
	char* (*dict)[2] = NULL, * (*pdict)[2] = NULL; //char[flieline][2]�� ������
	FILE* lib = NULL;
	char* tmp = NULL, * pos = NULL; //���� string ����, �ܾ� ���� ��ġ ����, ���� Ŀ�� ����(���� �ϳ��� ��� �Է¹��� �����̹Ƿ� ���)
	size_t tmp_len = 0, dict_len = 0; //���� tmp, dict�� �Ҵ���� ����
	void* ptmp = NULL; // ������ ���� �� � Ȱ���ϴ� �ӽ� �����ͺ���

	//�����Ҵ� �� ���� ����
	tmp = (char*)calloc_in((filelen + 3) * sizeof(char)); //+2�ϸ� ���Ҵ� ����� �˴ϴ�.
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

	//�Է�
	pos = tmp;
	while (!feof(lib)) { //������ ��� �о����� break;
		fgets(pos, tmp_len - (pos - tmp), lib); //������ ������ ��� ����
		pos += strlen(pos); //Ŀ�� �̵�, ù ��° NULL���ڸ� ����Ű�� ��
		if (tmp_len - (pos - tmp) <= 1) { //���� ������ �� ���� �� �ƴϳ� �ִ� �뷮�� ������ ���(�ٸ� ��� ��ü�� ������ �ִ� �� �ƴ� �̻� ����� ���� ����, ���� �Ҵ��� ���� ũ�� �̻����� �޾ұ� ����)
			ptmp = realloc_in(tmp, tmp_len + sizeof(char));
			if (ptmp) {
				tmp = ptmp;
				pos = tmp + tmp_len - 1; //tmp�� ��ġ�� ����� �� ������ pos ��ġ�� ����(���̰� �ִ�� �� ���̹Ƿ� ���� tmp_len-1(������ ĭ '\0'���� ���� �Է� ����)�� ����Ű�� ��
				tmp_len++; //realloc ������ ���� ũ�� ����
			}
			else break; //realloc ���н� break(���ܴ� �ѹ��� ó��)
		}
	}

	fclose(lib);
	lib = NULL;
	if (!tmp) {
		freeset(&tmp);
		freeset(&dict);
		return NULL;
	}

	//���� �ܾ�� ������
	//���� �ܾ�� �� �и�, �ܾ�� ���� ������ ":"�� �̷���� ����(���ʿ� space���� ����)�� ���, �� �濡�� ���Ⱑ ����
	pdict = dict;
	(*pdict)[0] = strtok(tmp, ":"); //�ǹ̿� ���� ������ ':'�� �и��ϱ�
	while ((*pdict)[0]) {
		if ((*pdict)[0][strlen((*pdict)[0]) - 1] == ' ')(*pdict)[0][strlen((*pdict)[0]) - 1] = '\0'; //���ܾ� �� ���� ����
		(*pdict)[1] = strtok(NULL, "\n");
		while (*((*pdict)[1]) == ' ') ((*pdict)[1])++; //�� �� ���� ����
		pdict++;
		if (pdict - dict > dict_len) { //������ �� �ִ� �ִ� �뷮�� ������ ���
			ptmp = realloc_in(dict, dict_len + sizeof(char* [2]));
			if (ptmp) {
				dict = ptmp;
				pdict = dict + dict_len;
				dict_len++; //realloc ������ ���� ũ�� ����
			}
			else break; //���н� break;
		}
		(*pdict)[0] = strtok(NULL, ":");
	}
	if (!dict) {
		freeset(&tmp);
		freeset(&dict);
		return NULL;
	}
	return dict; //tmp�� �ּҰ� (*dict)[0]�� ����Ǿ� �־� ������ �Ѱ��� �ʿ䰡 ����
}

void* wordfinder(const char* str, const char* (*dict)[2]) {
	//������ ���̸� �������� �����Ƿ� ����Ž���� ����ϱ� �����(#define���� ������ �� ���� �б⿡�� �����)
	while ((*dict)[0] != NULL && strcmp(str, (*dict)[0])) dict++; //���� Ž��, ��ġ�ϴ� �ܾ� ã��
	//dict ������ ������ �־����Ƿ� ���� ������ �� ĭ �Ѵ� �ɷδ� ������ �߻����� ����
	if ((*dict)[0]) return dict; //��ġ�ϴ� �ܾ ã�� ���(�ܾ��� ù��° ���ڰ� '\0'�� �ƴ� ���)
	else return NULL; //��ġ�ϴ� �ܾ ���� ���
}

int dictlen(const char* (*dict)[2]) { //������ ���� ����
	const char* (*tmp)[2] = dict;
	int res = 0;
	tmp = dict; //���⼭�� dict2�� �ӽ� ������ Ȱ��
	while (((*tmp)[0]) != NULL) {
		res++;
		tmp++;
	}
	return res;
}

const char* sgets(int len, int relen, FILE* _Stream) { //�Ҵ���� ���̴� return���� ����: �־��� ���� �� ��� �Ұ�
	//len�� �����Ҵ�, relen�� ���Ҵ�� ����
	//FILE* _Stream �� fgets�� �״�� ���� ���̰� �� �̿��� Ȱ���� ����(fgets�� �Ű������� �̸����� �״�� ������ ��)
	char* res = NULL, * ptmp = NULL, * pos = NULL; //���� string ����, �ܾ� ���� ��ġ ����, ���� Ŀ�� ����(in ���� �ϳ��� ��� �Է¹��� �����̹Ƿ� ���), ������ ���� �� � Ȱ���ϴ� �ӽ� �����ͺ���
	size_t res_len = 0; //���� res�� �Ҵ���� ����
	int tmp = 0;

	//�����Ҵ�
	res = (char*)calloc_in(len * sizeof(char));
	if (!res) return NULL;
	res_len += len;

	//�Է�
	pos = res;
	while (1) {
		fgets(pos, res_len - (pos - res), _Stream);
		pos += strlen(pos); //Ŀ�� �̵�, ù ��° NULL���ڸ� ����Ű�� ��
		if (*(pos - 1) == '\n') { //���� ���� ���๮�ڸ� �Է��� �������Ƿ� return
			*(pos - 1) = '\0'; //���� �� ���๮�� ����
			return res;
		}
		else { //�ƴϸ� ���� ������ �� ���� �� �ƴϰ� �ִ� �뷮�� ���������Ƿ� realloc ����
			ptmp = realloc_in(res, res_len + relen * sizeof(char));
			if (ptmp) {
				res = ptmp;
				pos = res + res_len - 1; //res�� ��ġ�� ����� �� ������ pos ��ġ�� ����(���̰� �ִ�� �� ���̹Ƿ� ���� res_len-1(������ ĭ '\0'���� ���� �Է� ����)�� ����Ű�� ��
				res_len += relen; //realloc ������ ���� ũ�� ����
			}
			else { //realloc ���н�
				freeset(&res);
				return NULL;
			}
		}
	}
}

void* calloc_in(const size_t len) {
	//calloc ��� + ���н� ��õ�
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
	//realloc ��� + ���н� ��õ�
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