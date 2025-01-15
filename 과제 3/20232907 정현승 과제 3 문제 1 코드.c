#include "20232907 ������ ���� 3 ���� ���.h"
#define end_in_str "end\n" //�������� string
#define malloc_strin_gap 201 //in�� malloc ����(�� á�� �� 201ĭ��ŭ ����)
#define malloc_word_gap 10 //word�� malloc ����(�� á�� �� 10ĭ��ŭ ����)
#define word_cut_chars " ,.-?\'\"\n\t"

int string_arr_cmp(const char** str1, const char** str2);

void main() {
	char* in = NULL, ** words = NULL, * pos = NULL; //���� string�� word ����, �ܾ� ���� ��ġ ����, ���� Ŀ�� ����(in ���� �ϳ��� ��� �Է¹��� �����̹Ƿ� ���)
	void* ptmp = NULL; //������ ���� �� � Ȱ���ϴ� �ӽ� �����ͺ���
	size_t in_len = 0, words_len = 0; //�� malloc�� ���� ũ�⸦ ��Ÿ���� ����
	unsigned int sentnum = 0, wordnum = 0, tmp = 0; //���� �����, �ܾ��, �ӽú���(��õ� üũ�� ��ġ�� �ܾ� �� å�� �� ����)

	//in �����Ҵ�
	in = (char*)calloc_in(malloc_strin_gap);
	if (!in) return;
	in_len += malloc_strin_gap;

	//�Է�
	tmp = 0;
	pos = in;
	while (tmp || strcmp(pos, end_in_str)) {
		//ù üũ�ô� calloc���� �����ؼ�, ""���� �ν��ϱ⿡ ������ �߻����� ����
		fgets(pos, in_len - (pos - in), stdin);
		if (!tmp && !strcmp(pos, end_in_str)) break; //�Է¹��� ������ end�̸� Ż��(tmp ������ �ļ�)
		pos += strlen(pos); //Ŀ�� �̵�, ù ��° NULL���ڸ� ����Ű�� ��
		if (*(pos - 1) == '\n') {
			//���� ���� ���๮�ڸ� ���� �� ����
			sentnum++;
			tmp = 0;
		}
		else tmp++;
		//�ƴϸ� ���� ������ �� ���� �� �ƴϹǷ� ���� ����(�̶� ���� �� end�� ���� �Է� �ް� ������ ���� ������ tmp ���� Ȱ��)
		//������ ���� �����ϱ� ���� isspace �Լ��� ������� ����
		if (strlen(in) >= in_len - 6) { //�ִ� �뷮 ���޷� ���� realloc ����
			//�뷮���� 5�� ���� �̳��� ���� ��쿡��, ����� �߾ �뷮�� �� �޴µ�, �̴� end���� �Է������� ���� �������� �߸��� �ٶ��� ���� �������� �νĵ��� �ʴ� �һ�縦 ���� ����
			ptmp = realloc_in(in, in_len + malloc_strin_gap * sizeof(char));
			if (ptmp) {
				in = ptmp;
				pos = in + in_len - 1; //in�� ��ġ�� ����� �� ������ pos ��ġ�� ����(���̰� �ִ�� �� ���̹Ƿ� ���� in_len-1(������ ĭ '\0'���� ���� �Է� ����)�� ����Ű�� ��
				in_len += malloc_strin_gap; //realloc ������ ���� ũ�� ����
			}
			else { //realloc ���н�
				freeset(&in);
				freeset(&words); //freeset �Լ� ������, �̹� NULL�̸� �ƹ��� ���� �������� ����
				return;
			}
		}
	}
	*(pos - 1) = '\0'; //end�� ������ �ƴϹǷ�, end ������ ���๮��(end�� �ܵ����� ���;� �ϴ� end ������ ���๮���� ��)
	/* �ʿ���� in ���Ҵ�
	ptmp = realloc_in(in, strlen(in) + 1);
	if(!ptmp) { //realloc ���н�
		freeset(&in);
		freeset(&words); //�Լ� ������, �̹� NULL�̸� �ƹ��� ���� �������� ����
		return;
	}*/
	
	//words �����Ҵ�
	tmp = 0;
	words = (char**)calloc_in(malloc_word_gap * sizeof(char*));
	if (!words) {
		freeset(&in);
		return;
	}
	words_len += malloc_word_gap;

	//�ܾ� �߶� ����
	ptmp = strtok(in, word_cut_chars); //in�� ������ �ʿ䰡 �����Ƿ� �״�� ���, ���� ����� �Ű����� in ���
	while (ptmp) {
		words[wordnum] = ptmp;
		wordnum++;
		if (wordnum == words_len) {
			ptmp = realloc_in(words, words_len + malloc_word_gap * sizeof(char*));
			if (ptmp) {
				words = ptmp;
				words_len += malloc_word_gap; //realloc ������ ���� ũ�� ����
			}
			else { //realloc ���н�
				freeset(&in);
				freeset(&words); //freeset �Լ� ������, �̹� NULL�̸� �ƹ��� ���� �������� ����
				return;
			}
		}
		ptmp = strtok(NULL, word_cut_chars); //���� �ݺ� �� ����Ȯ���� ���� strtok�� �ݺ��� ��������
	}
	//�ܾ� ����
	qsort(words, wordnum, sizeof(char*), string_arr_cmp);

	//�ߺ� �ܾ� ã��
	tmp = 0; //�̹����� �ߺ��� �ܾ� �� ����
	for (ptmp = words; (((char**)ptmp) + tmp + 1) < words + wordnum;) {
		if (!strcmp(*(char**)ptmp, ((char**)ptmp)[tmp + 1])) {
			((char**)ptmp)[tmp + 1] = NULL;
			tmp++;
		}
		else {
			((char**)ptmp)++;
			*((char**)ptmp) = ((char**)ptmp)[tmp];
			//�ߺ� �߻����� ���� ��ĭ�� ����� �о���
		}
	}

	printf("%d %d %d\n", sentnum, wordnum, wordnum - tmp);
	wordnum -= tmp; //tmp�� �ݺ� Ƚ���� Ȱ���ϱ� ���� �ߺ� ������ �ܾ� ������ wordnum���� ����
	for (tmp = 0; tmp < wordnum; tmp++) puts(words[tmp]);
	freeset(&in);
	freeset(&words);
}

int string_arr_cmp(const char** str1, const char** str2) { //qsort�� ���� �Լ�
	return strcmp(*str1, *str2);
}