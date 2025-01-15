#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define Across_Obj 5 //��ٸ��� �����ϴ� �ο���
#define Down_len 80 //��ٸ��� ���Ʒ� ����
#define Line_gap 1 //��ٸ� �������� ����
#define Max_line_in_a_row 6 //�ֺ��� ��ٸ� ���� �� �ٿ� ���ӵ� �� �ִ� ��ٸ� ���μ��� ����
#define Ch_Down '|' //��ٸ� ������ ǥ��
#define Ch_Acr '-' // ��ٸ� ������ ǥ��
#define Ch_Void ' ' // ��ٸ� �� �� ǥ��
typedef char acr_type; // �������� ���� type
typedef unsigned char dwn_type; // �������� ���� type
typedef char str_type; // ���ڿ� ������ ���� type
typedef int rpt_type; // �ݺ� Ƚ���� ���� type
//�Ʒ� 4���� �޸� ����� ȿ�������� �ϱ� ���� ���� type�� �ٲ���(���� ��� Across_Obj�� 5�̹Ƿ� for������ Across_Obj ���ڸ�ŭ �ݺ����� �� �� char �ڷ����� ����ص� �������� �̷��� �ڵ��ϴٰ� ���ڱ� 300���� �þ�� �Ʒ� �ڵ带 �� �ٲ�� �ϹǷ� ���� ����)

void normal_game(char* head[Across_Obj], char* tail[Across_Obj]); // �Ϲ� ��ٸ�
void rept(rpt_type num, char* head[Across_Obj], char* tail[Across_Obj]); // 1000�� �ݺ��ϴ� �ڵ� (num=1000)
void line_place(char acr_info[Across_Obj - 1][Down_len]); //������ �߱�
void print_line(char acr_info[Across_Obj - 1][Down_len]); //��ٸ� �� ǥ��
void print_res(char acr_info[Across_Obj - 1][Down_len], char* head_info[Across_Obj], char* tail_info[Across_Obj]); //��ü ��ٸ� ǥ��
void cal_res(char acr_info[Across_Obj - 1][Down_len], acr_type res[Across_Obj]); //��� ���

void main() {
	char* head[Across_Obj] = { "1","2","3","4","5" };
	char* tail[Across_Obj] = { "A","B","C","D","E" };
	srand(time(NULL));
	normal_game(head, tail);
	// rept(1000, head, tail);
}

void normal_game(char* head[Across_Obj], char* tail[Across_Obj]) {
	char acr_line[Across_Obj - 1][Down_len] = { 0, };
	acr_type res[Across_Obj] = { 0, };
	srand(time(NULL));
	line_place(acr_line);
	print_res(acr_line, head, tail);
	cal_res(acr_line, res);
	for (acr_type i = 0; i < Across_Obj - 1; i++) {
		printf("%s -> %s, ", head[i], tail[res[i]]);
	}
	printf("%s -> %s\n", head[Across_Obj - 1], tail[res[Across_Obj - 1]]);
}

void rept(rpt_type num, char* head[Across_Obj], char* tail[Across_Obj]) {
	char acr_line[Across_Obj - 1][Down_len] = { 0, };
	acr_type res[Across_Obj] = { 0, };
	rpt_type res_col[Across_Obj][Across_Obj] = { 0, }; // 1000�� ��� ī����
	for (rpt_type i = 0; i < num; i++) {
		line_place(acr_line);
		cal_res(acr_line, res);
		for (acr_type j = 0; j < Across_Obj; j++) // ��迡 �ֱ�
			res_col[j][res[j]]++;
	}
	for (acr_type i = 0; i < Across_Obj; i++) { // ��� ���
		printf("%s: ",head[i]);
		for (acr_type j = 0; j < Across_Obj - 1; j++)
			printf("%s:%3d, ",tail[j],res_col[i][j]);
		printf("%s:%3d\n", tail[Across_Obj - 1], res_col[i][Across_Obj - 1]); // �������� ',' ����� �ʿ� ������ ���� ���
	}
}

void line_place(char acr_info[Across_Obj - 1][Down_len]) {
	dwn_type cnt[Across_Obj - 1][2] = { 0, }; //�� ���� ������ ��([arr][0]�� �� ����, [arr][1]�� ���� ����(�ֺ��� ���μ��� ���� ���¿��� ���ӵ� ���μ��� ����))
	//���� ������ �Ź� ����� ���� ������ ������
	acr_type arr = 0;
	for (dwn_type i = 0; i < Down_len; i++) { //�ϴ��� �� �ٴ� �ϳ����� ���� �־���
		do arr = rand() % (Across_Obj - 1); //������ ���� ����
		while (cnt[arr][0] >= Down_len * 2 / Across_Obj);
		if (cnt[arr][1] >= Max_line_in_a_row) { //�ֺ��� ��ٸ� ���� �� �ٿ� ���ӵ� �� �ִ� ��ٸ� ���μ��� ������ �Ѿ��� ���
			if (arr >= (Across_Obj - 2) || (arr > 0 && rand() % 2))	arr--;
			else arr++;
			//��ٸ� ��ġ ����(���� ��ٸ��� �� �ٷ� �ű�)
		}
		for (acr_type j = 0; j < Across_Obj - 1; j++) {
			if (j == arr) acr_info[arr][i] = Ch_Acr; //������ �־��ֱ�
			else acr_info[j][i] = Ch_Void; //���� �־��ֱ�
		}
		cnt[arr][0]++;
		cnt[arr][1]++;
		//���� ����
		if (arr > 0) cnt[arr - 1][1] = 0;
		if (arr < (Across_Obj - 2)) cnt[arr + 1][1] = 0;
		//���� ���� �ʱ�ȭ
	}
}

void print_res(char acr_info[Across_Obj - 1][Down_len], char* head_info[Across_Obj], char* tail_info[Across_Obj]) {
	for (acr_type i = 0; i < Across_Obj - 1; i++) {
		printf(head_info[i]);
		for (str_type j = strlen(head_info[i]); j < Line_gap + 1; j++)
			putchar(Ch_Void); //���� ���� ��ġ���� �� ���� ���
	}
	puts(head_info[Across_Obj - 1]); //�������� �� ���� ����� �ʿ� ������ ���� ���, ���� �ǽ�
	print_line(acr_info);
	for (acr_type i = 0; i < Across_Obj - 1; i++) {
		printf(tail_info[i]);
		for (str_type j = strlen(tail_info[i]); j < Line_gap + 1; j++)
			putchar(Ch_Void); //���� ���� ��ġ���� �� ���� ���
	}
	puts(tail_info[Across_Obj - 1]); //�������� �� ���� ����� �ʿ� ������ ���� ���, ���� �ǽ�
}

void print_line(char acr_info[Across_Obj - 1][Down_len]) {
	for (dwn_type i = 0; i < Down_len; i++) {
		putchar(Ch_Down);
		for (acr_type j = 0; j < Across_Obj - 1; j++) {
			for (str_type k = 0; k < Line_gap; k++)
				putchar(acr_info[j][i]); //������ ǥ��
			putchar(Ch_Down);
		}
		putchar('\n');
	}
}

void cal_res(char acr_info[Across_Obj - 1][Down_len], acr_type res[Across_Obj]) {
	acr_type cur = 0; //���� ��
	for (acr_type i = 0; i < Across_Obj; i++) {
		cur = i;
		for (dwn_type j = 0; j < Down_len; j++) {
			if (cur > 0 && acr_info[cur - 1][j] == Ch_Acr) cur--;
			else if (cur < Across_Obj - 1 && acr_info[cur][j] == Ch_Acr) cur++;
			//�տ��� �迭 ���� ����� �ʰ� �ϱ� ���� ����, �ڴ� ���� �׾��� �ִ��� Ȯ��
			//VC���� and�� ���� �̹� False�̸� ���� ������ ���� �ʴ´ٴ� ���� �̿�
		}
		res[i] = cur; //������ �������� ����� ��ġ�� ����(��ġ������ ������)
	}
}