#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define default_money 5000 //�ʱ��ں�
#define default_price 300 //���Ű�
#define default_fare 600 //�����
#define player_num 2 //�÷��̾� ��
#define max_turn 30 //�ִ� �� ��
#define board_size 10
#define board_acr 5
#define board_acr_gap 1
#define board_dwn 2 //3 �̻��̸� Ȱ���ϰ����� 2�� �̿����� ���� ������ ����
#define namelen 10

struct _City; //�Լ� ���� ����ó�� ��µ� �Ǵ����� ���߰ڳ׿�...

typedef struct _Player {
	char* name;
	struct _City* place; //�̰� ���� �Ǵ� ������ Ȯ���� �غ��� �� �� ���ƿ�...
	unsigned int money;
} person;

typedef struct _City {
	char* name;
	struct _Player* owner; //����
	unsigned int price; //���Ű�
	unsigned int fare; //�����
} city;

void printboard(const city board[board_size]);

void main() {
	city board[board_size] = {
		{"START",NULL,0,0},
		{"Seoul",NULL,default_price,default_fare},
		{"Tokyo",NULL,default_price,default_fare},
		{"Sydney",NULL,default_price,default_fare},
		{"LA",NULL,default_price,default_fare},
		{"Cairo",NULL,default_price,default_fare},
		{"Phuket",NULL,default_price,default_fare},
		{"New delhi",NULL,default_price,default_fare},
		{"Hanoi",NULL,default_price,default_fare},
		{"Paris",NULL,default_price,default_fare}
	};
	person player[player_num] = { {"COM1",board,default_money},{"COM2",board,default_money} };
	char dice = 0;
	person* bankrupt = NULL; //�Ļ����� ���� �ߵ� ���� ����� �Ļ��� �÷��̾� ����
	unsigned int i = 0; //�ݺ������� ���
	person* p = player; //�ݺ������� ���

	srand(time(NULL));

	for (i = 0; i < max_turn && !bankrupt; i++) { //�� ����
		for (p = player; p < player + player_num && !bankrupt; p++) {
			dice = rand() % 6 + 1;
			p->place = board + (((p->place - board) + dice) % board_size); //ĭ �̵�
			system("cls");
			printboard(board); //�Ź� ���� �׸���
			printf("%s�� �ֻ���: %hhd\n", p->name, dice);
			if (p->place == board) //���� ��ġ�� ���(���� ������� ����)
				puts(p->place->name); //�̸� ��� �� �߰� �۾� ����
			else if (!(p->place->owner)) { //������ ���� ���
				printf("%s (���� ����)\n", p->place->name);
				if (p->money >= p->place->price) { //���� ������ ���
					printf("%s��/�� ������ �� ����\n", p->place->name);
					p->place->owner = p;
					p->money -= p->place->price; //���� ����
				}
				else
					printf("%s��/�� ������ �� ����\n", p->place->name);
			}
			else if (p->place->owner == p) //������ ���� ������ �ڱ� �ڽ��� ���
				printf("%s (%s)\n", p->place->name, p->name);
			else {  //������ ���� ������ �ٸ� ���
				printf("%s (%s)\n", p->place->name, p->place->owner->name);
				if (p->money < p->place->fare) { //����Ḧ ������ �ɷ��� ���ٸ�
					printf("%s��/�� %s�� ����Ḧ ������ �� ����", p->name, p->place->name);
					bankrupt = p;
					break;
				}
				p->money -= p->place->fare;
				p->place->owner->money += p->place->fare;
				printf("%s��/�� %s���� %u ����\n", p->name, p->place->owner->name, p->place->fare);
				printf("%s�� �ܰ� %u.\n", p->place->owner->name, p->place->owner->money);
			}
			printf("%s�� �ܰ� %u.\n", p->name, p->money);
			system("pause"); //ȭ���� ������
		}
	}

	putchar('\n');
	if (bankrupt)
		printf("%s�� �Ļ����� ������ ����Ǿ����ϴ�.\n", bankrupt->name);
	else
		printf("������ �� �� %u���� ��� ���� ������ �����մϴ�\n", max_turn);

	for (p = player; p < player + player_num; p++) {
		//���� ���� ���������� �ܰ� �����ֱ�
		if (p == bankrupt) printf("%s �Ļ�\n", p->name);
		else printf("%s�� �ܰ� %u.\n", p->name, p->money);
	}
}

void printboard(const city board[board_size]) { //���� �� �÷��̾��� ��ġ�� �������� �ʱ� ������ �̰� �ϳ��� �����
	unsigned char i = 0, k = 0;
	unsigned int j = 0;
	char str[((namelen + 2 + board_acr_gap) * board_acr + 1) * 4 + 1] = { 0, }, * pl = str;
	//���� ����� string�� ������ �迭�� Ŀ�� ����
	size_t len = 0; //strlen �Լ��� ���÷� ����Կ� ����, �̸� �����ϴ� �� ȿ�����̶� ������ ������ ���� ����

	//k�� 0�϶� 5+i�� ����ϰ�, k�� 1�϶� 4-i�� ����ϴ� ���: (board_acr - k) - i * (k * 2 - 1)
	//���� k�� 0�϶� 5, 1�϶� 4�̸�, �ڴ� k�� 0�̸� i�� (-1)�� ���� ���� ����, k�� 1�̸� i�� 1�� ���� ���� ��(����, ���� ��ȯ)
#define boardpl_cal ((board_acr - k) - i * (k * 2 - 1)) //�̸� #define����(�Լ� �������� #undef�� ���ַ� �ϱ⿡ ���⿡ �ۼ���)
	for (k = 0; k < 2; k++) {
		for (i = 0; i < board_acr; i++) { //���� ��� ǥ��
			*pl++ = '+'; //�� ���� ���鼭 Ŀ�� �ű��
			for (j = 0; j < namelen; j++)
				*pl++ = '-';
			*pl++ = '+';
			for (j = 0; j < board_acr_gap; j++)
				*pl++ = ' '; //ĭ ����
		}
		*pl++ = '\n';
		for (i = 0; i < board_acr; i++) { //���� �̸�
			*pl++ = '|';
			len = strlen(board[boardpl_cal].name);
			for (j = 0; j < (namelen - len + 1) / 2; j++) *pl++ = ' ';
			//�߾������� ���� �� ���� ���
			//�� ������ ���� �� ������ ���� ��ġ���� ������ �տ� ������ �ϳ� �� �����
			strcpy(pl, board[boardpl_cal].name);
			pl += len;
			for (j = 0; j < (namelen - len) / 2; j++) *pl++ = ' ';
			//�߾������� ���� �� ���� ���
			*pl++ = '|';
			for (j = 0; j < board_acr_gap; j++)
				*pl++ = ' '; //ĭ ����
		}
		*pl++ = '\n';
		for (i = 0; i < board_acr; i++) { //���� �̸�
			*pl++ = '|';
			if (board[boardpl_cal].owner) { //������ �ִٸ� ���� �̸� ���
				len = strlen((board[boardpl_cal].owner)->name);
				for (j = 0; j < (namelen - len + 1) / 2 - 1; j++) //�߾������� ���� �� ���� ���
					*pl++ = ' '; //�� ������ ���� �� ������ ���� ��ġ���� ������ �տ� ������ �ϳ� �� �����
				*pl++ = '(';
				strcpy(pl, (board[boardpl_cal].owner)->name);
				pl += len;
				*pl++ = ')';
				for (j = 0; j < (namelen - len) / 2 - 1; j++) //�߾������� ���� �� ���� ���
					*pl++ = ' ';
			}
			else for (j = 0; j < namelen; j++)
				*pl++ = ' '; //������ �������� ä����
			*pl++ = '|';
			for (j = 0; j < board_acr_gap; j++)
				*pl++ = ' '; //ĭ ����
		}
		*pl++ = '\n';
		for (i = 0; i < board_acr; i++) { //�Ʒ��� ��� ǥ��
			*pl++ = '+'; //�� ���� ���鼭 Ŀ�� �ű��
			for (j = 0; j < namelen; j++)
				*pl++ = '-';
			*pl++ = '+';
			for (j = 0; j < board_acr_gap; j++)
				*pl++ = ' '; //ĭ ����
		}
		*pl++ = '\n';
		*pl = '\0'; //���ڿ��� �������� '\0'���� �ٲ���
		puts(str);
		pl = str; //Ŀ�� �ʱ�ȭ

		if (!k) { //ȭ��ǥ ǥ��
			len = 2;
			for (j = 0; j < (namelen - len + 1) / 2 + 1; j++) *pl++ = ' ';
			//�߾������� ���� �� ���� ���
			//�� ������ ���� �� ������ ���� ��ġ���� ������ �տ� ������ �ϳ� �� �����
			*pl++ = '/';
			*pl++ = '\\'; //�������÷� ���������� ���δٰ� ������
			for (j = 0; j < (namelen - len) / 2 + 1; j++) *pl++ = ' ';
			//�߾������� ���� �� ���� ���
			for (j = 0; j < (namelen + 2 + board_acr_gap) * (board_acr - 2) + board_acr_gap; j++)
				*pl++ = ' '; //���� ȭ��ǥ ��ġ���� ���� ���

			for (k = 0; k < 3; k++) { //�� �̻��� str ������ ��ƿ �� ����
				//ȭ��ǥ ���� ��� �ݺ���
				for (j = 0; j < (namelen - len + 1) / 2 + 1; j++) *pl++ = ' ';
				//�߾������� ���� �� ���� ���
				//�� ������ ���� �� ������ ���� ��ġ���� ������ �տ� ������ �ϳ� �� �����
				*pl++ = '|'; //ȭ��ǥ ���� �κ�
				*pl++ = '|';
				*pl++ = '\n'; //�� ����
				for (j = 0; j < (namelen - len + 1) / 2 + 1; j++) *pl++ = ' ';
				//�߾������� ���� �� ���� ���
				//�� ������ ���� �� ������ ���� ��ġ���� ������ �տ� ������ �ϳ� �� �����
				*pl++ = '|'; //ȭ��ǥ ���� �κ�
				*pl++ = '|';
				for (j = 0; j < (namelen - len) / 2 + 1; j++) *pl++ = ' ';
				//�߾������� ���� �� ���� ���
				for (j = 0; j < (namelen + 2 + board_acr_gap) * (board_acr - 2) + board_acr_gap; j++)
					*pl++ = ' '; //���� ȭ��ǥ ��ġ���� ���� ���
			}
			k = 0; //���� �� ���� �ٽ� ���󺹱�

			for (j = 0; j < (namelen - len + 1) / 2 + 1; j++) *pl++ = ' ';
			//�߾������� ���� �� ���� ���
			//�� ������ ���� �� ������ ���� ��ġ���� ������ �տ� ������ �ϳ� �� �����
			*pl++ = '\\'; //�������÷� ���������� ���δٰ� ������
			*pl++ = '/';

			*pl++ = '\n';
			*pl = '\0'; //���ڿ��� �������� '\0'���� �ٲ���
			puts(str);
			pl = str; //Ŀ�� �ʱ�ȭ
		}
	}
#undef boardpl_cal
}