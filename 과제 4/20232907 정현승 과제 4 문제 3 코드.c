#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#pragma warning(disable:6031)

#define MAX_strlen 20 //���ڿ��� �ִ� ����(���� ��� ����)

//enum day_type { sun, mon, tue, wed, thu, fri, sat };
enum month_type { JAN = 1, FEB, MAR, APR, MAY, JUN, JUL, AUG, SEP, OCT, NOV, DEC };

typedef struct datetime { //���� 2���� ��� �װ�
	unsigned int year;
	enum month_type month : 5;
	unsigned char dayn : 5;
	//enum day_type dayt : 3; //���� 2����ó�� ���⼭�� ����� ���� ���� ����
	//unsigned char hour : 5; //�̻��
	//unsigned char minute : 6; //�̻��
	//unsigned char second : 6; //�̻��
} datetime;

struct _Loan; //����ü ��������

typedef struct _Book {
	char* title; //å �̸�
	char* author; //���ڸ�
	char* press; //���ǻ�
	struct _Loan* status; //���� ���� ��Ȳ, ���� ȿ���� ���� ���⵵ ����
	struct _Book* prev; //Linkedlist ���
	struct _Book* next; //Linkedlist ���
} book;

typedef struct _Person {
	char* name;
	char* phone;
	//���� ������ å�� ���� ���̱⿡ ���� ��Ȳ�� ���⿡ �����ϱ�� �����
	struct _Person* prev; //Linkedlist ���
	struct _Person* next; //Linkedlist ���
} person;

typedef struct _Loan {
	struct _Loan* prev; //Linkedlist ���
	struct _Loan* next; //Linkedlist ���
	book* objbook;
	person* objper;
	datetime rantdate; //������
	datetime retrndate; //�ݳ���, ���� 0�� ��� �ݳ��� �� �� ��
} rent;

void booklist(const book* p);
rent* rentbook(const book* pb, const person* pp, rent* pr);
rent* retrnbook(rent* pr);
void rentlist(const rent* p);
datetime datein();
unsigned long long datetolld(const datetime obj);
int datecmp(const datetime datet1, const datetime datet2);
char ifleapyear(const int year);

void main() {
	book bookshelf[5] = {
		{"���̽����α׷���","ȫ�浿","A���ǻ�",NULL,NULL,bookshelf + 1},
		{"���ο� �ڷᱸ��" ,"���浿","C���ǻ�",NULL,bookshelf,bookshelf + 2},
		{"���� C ���α׷���","������","�������̵��",NULL,bookshelf + 1,bookshelf + 3},
		{"���� C++ ���α׷���",bookshelf[2].author,bookshelf[2].press,NULL, bookshelf + 2,bookshelf + 4},
		{"�Ѱ��׽�Ʈ�밡�󵵼�","�Ѱ��׽�Ʈ�밡������","�Ѱ��׽�Ʈ�밡��ȸ��",NULL,bookshelf + 3,NULL}
	};
	person members[5] = {
		{"�̼���","010-4444-5555",NULL,members + 1},
		{"���󿡸�","010-3333-8888",members,members + 2},
		{"�����","010-3434-3434",members + 1,members + 3},
		{"����� ������","010-4757-0040",members + 2,members + 4},
		{"�Ѱ��׽�Ʈ�밡���̸�","�Ѱ��׽�Ʈ�밡���ȣ",members + 3,NULL}
	};
	rent* rentptr = NULL; //�������� ó���� ���� �� ����
	char cmd = 1;
	while (1) {
		if (cmd) {
			puts("���Ͻô� ����� �Է��� �ּ���.");
			puts("1. å ��� ǥ���ϱ�");
			puts("2. ���� ����ϱ�");
			puts("3. �ݳ� �ϱ�");
			puts("4. ���� ��� ǥ���ϱ�");
		}
		else cmd = 1;
		switch (getchar()) {
		case '\n':
		case '\t':
		case ' ':
			cmd = 0; //�� ��� ���� �� ��� ����
			break;
		case '1':
			booklist(bookshelf);
			putchar('\n');
			break;
		case '2':
			while (getchar() != '\n'); //�ٺ��� ���ڰ� ���� ������ ��� �Է¹޾� ������
			rentptr = rentbook(bookshelf, members, rentptr); //rentptr�� NULL�� ���� �ʵ��� return�� ����
			putchar('\n');
			break;
		case '3':
			while (getchar() != '\n'); //�ٺ��� ���ڰ� ���� ������ ��� �Է¹޾� ������
			retrnbook(rentptr); //return���� ó������̳� NULL�� ���� ���� �־� ����� �����
			putchar('\n');
			break;
		case '4':
			rentlist(rentptr);
			putchar('\n');
			break;
		default:
			puts("�߸��� �Է��Դϴ�. �ٽ� �Է��� �ֽñ� �ٶ��ϴ�.");
		}
	}
	//return�� �����Ƿ� free�� ����
}

void booklist(const book* p) {
	char res[MAX_strlen * 3 + 11] = { 0, }; //�Ѳ����� ���
	if (!p) {
		puts("���� å�� �����ϴ�.");
		return;
	}
	while (p->prev) p = p->prev; //�� ������ �̵�
	while (p) {
		sprintf(res,"%-*s %-*s %-*s ", MAX_strlen, p->title, MAX_strlen, p->author, MAX_strlen, p->press);
		if (p->status) strcat(res, "������"); //�������̸� ���� ������ ����Ǿ� ���� ���̹Ƿ� �̸� ǥ��
		else strcat(res, "����");
		puts(res);
		p = p->next;
	}
}

rent* rentbook(const book* pb, const person* pp, rent* pr) {
	rent* res = NULL;
	char name[MAX_strlen + 2] = { 1, }; //name[0]�� �ӽ� flag�� Ȱ��
	if (!pb) {
		puts("���� å�� �����ϴ�.");
		return;
	}
	if (!pp) {
		puts("���� ȸ���� �����ϴ�.");
		return;
	}

	while (pb->prev) pb = pb->prev; //�� ������ �̵�
	while (pb) { //���� ������ å�� �ִ��� Ȯ��
		if (!pb->status) {
			*name = 0;
			break;
		}
		if (pb->next) pb = pb->next;
		else break;
	}
	if (*name) {
		puts("���� ������ å�� �����ϴ�.");
		return pr; //main�Լ����� ���� �߻��� ���� ���� ��ġ
	}
	
	res = calloc(1, sizeof(rent));
	if (!res) {
		puts("���� ��Ͽ� �����߽��ϴ�.");
		return pr;
	}

	while (!res->objper) {
		printf("������ ����� �̸��� �Է��� �ּ���:");
		fgets(name, sizeof(name) / sizeof(char), stdin);
		if (name[strlen(name) - 1] != '\n') //if���� while���� ��ģ�ٴ� ���� �������� �߰��ߴµ� ���� else �����ϱ� �����Ƽ� ���� if... �ٵ� state�� �����ϱ� ���� ������ ������ ���� �ͺ��� �̰� �� ȿ������ ���� �ΰ� ���� �� ��
			while (name[strlen(name) - 1] != '\n') //�߸��� �Է��� ���(ȸ���� �̸��� ���̴� MAX_strlen�� �ʰ��� �� ���µ� �̸� �ʰ��� �Է��� ���� ���)
				fgets(name, sizeof(name) / sizeof(char), stdin); //�ϴ� �߷��� ������ ���� �Է� �޾� ������
		else {
			//ȸ�� ã��
			while (pp->prev) pp = pp->prev; //�� ������ �̵�
			name[strlen(name) - 1] = '\0'; //������ ���� ���� �����ֱ�
			while (pp) {
				if (!strcmp(pp->name, name)) {
					res->objper = pp;
					break;
				}
				if (pp->next) pp = pp->next;
				else break; //������ ���� ���� �����ص��� �ʱ� ������ �ʿ�
			}
		}
		if (!res->objper) puts("�߸��� �Է��Դϴ�.");
	}

	while (!res->objbook) {
		printf("������ å�� �̸��� �Է��� �ּ���:");
		fgets(name, sizeof(name) / sizeof(char), stdin);
		if (name[strlen(name) - 1] != '\n') //�߸��� �Է��� ���(å �̸��� ���̵� MAX_strlen�� �ʰ��� �� ���µ� �̸� �ʰ��� �Է��� ���� ���)
			while (name[strlen(name) - 1] != '\n') //�ϴ� �߷��� ������ ���� �Է� �޾� ������
				fgets(name, sizeof(name) / sizeof(char), stdin);
		else {
			//å ã��
			while (pb->prev) pb = pb->prev; //�� ������ �̵�
			name[strlen(name) - 1] = '\0'; //������ ���� ���� �����ֱ�
			while (pb) {
				if (!strcmp(pb->title, name) && !pb->status) {
					//�̸��� �°� ���� ���� ����� ���ٸ�
					res->objbook = pb;
					break;
				}
				if (pb->next) pb = pb->next;
				else break;
			}
		}
		if (!res->objbook) puts("�߸��� �Է��Դϴ�.");
	}

	puts("���� ��¥�� �Է��� �ּ���.");
	res->rantdate = datein();

	res->objbook->status = res; //å�� ���� ��� ���
	puts("����ó���� �Ϸ�Ǿ����ϴ�. 7�� �̳��� �ݳ����ֽñ� �ٶ��ϴ�.");

	//Linkedlist ����
	if(!pr) return res; //���� �źν� �۾��� �������� ����
	while (pr->next) pr = pr->next; //�� �ڷ� �̵�
	pr->next = res;
	res->prev = pr;
	return res;
}

rent* retrnbook(rent* pr) {
	rent* res = NULL;
	char name[MAX_strlen + 2] = { 0, };

	if (!pr) {
		puts("���� �뿩 ����� �����ϴ�.");
		return NULL;
	}

	while (pr->next) pr = pr->next; //�� �ڷ� �̵�
	while (pr) { //�������� å�� �ִ��� Ȯ��
		if (!pr->retrndate.dayn) break; //�������̸� ���� ������ ����Ǿ� ���� ��
		pr = pr->prev;
	}
	if (!pr) {
		puts("���� �������� å�� �����ϴ�.");
		return NULL;
	}

	while (!res) {
		printf("�ݳ��� å�� �̸��� �Է��� �ּ���:");
		fgets(name, sizeof(name) / sizeof(char), stdin);
		if (name[strlen(name) - 1] != '\n') //�߸��� �Է��� ���(å �̸��� ���̵� MAX_strlen�� �ʰ��� �� ���µ� �̸� �ʰ��� �Է��� ���� ���)
			while (name[strlen(name) - 1] != '\n') //�ϴ� �߷��� ������ ���� �Է� �޾� ������
				fgets(name, sizeof(name) / sizeof(char), stdin);
		else {
			//ȸ�� ã��
			while (pr->next) pr = pr->next; //�� �ڷ� �̵�
			name[strlen(name) - 1] = '\0'; //������ ���� ���� �����ֱ�
			while (pr) { //�������� �ֱٿ� ���� ���̹Ƿ� �ڿ������� ã��
				if (!strcmp(pr->objbook->title, name) && !((pr->retrndate).dayn)) {
					//�ݳ������ �־ ����� �������� ����
					res = pr; //������ ��������
					break;
				}
				if (pr->prev) pr = pr->prev;
				else break;
			}
		}
		if (!res) puts("�߸��� �Է��Դϴ�.");
	}

	puts("�ݳ� ��¥�� �Է��� �ּ���.");
	res->retrndate = datein();
	while (datecmp(res->rantdate, res->retrndate) > 0) {
		puts("�ݳ� ��¥�� ���� ��¥���� �����ϴ�.");
		puts("�ݳ� ��¥�� �Է��� �ּ���.");
		res->retrndate = datein();
	}

	res->objbook->status = NULL; //å������ �����Ȳ �ʱ�ȭ
	puts("�ݳ�ó���� �Ϸ�Ǿ����ϴ�.");

	return res;
}

void rentlist(const rent* p) {
	char res[MAX_strlen * 3 + 35] = { 0, }; //�Ѳ����� ���
	if (!p) {
		puts("���� �뿩 ����� �����ϴ�.");
		return;
	}
	while (p->prev) p = p->prev; //�� ������ �̵�
	while (p) {
		sprintf(res, "%-*s %s(%s)", MAX_strlen, p->objbook->title, p->objper->name, p->objper->phone);
		sprintf(res, "%-*s", (MAX_strlen * 3 + 4), res); //���� ä���... �����Ѱ�?
		sprintf(res, "%s %lld", res, datetolld(p->rantdate));

		if (p->retrndate.dayn) sprintf(res, "%s %lld", res, datetolld(p->retrndate));
		else strcat(res," (������)"); //�������̸� �ݳ��� ������ ���ٴ� ���� �̿�
		puts(res);
		p = p->next;
	}
}

unsigned long long datetolld(const datetime obj) {
	unsigned long long res = 0;
	res += obj.year * 10000LL;
	res += obj.month * 100;
	res += obj.dayn * 1;
	return res;
}

datetime datein() { //��¥ �Է�
	datetime res = { 0, };
	int tmp = 0; //���� �Է��� �ӽ÷� ���� ����
	printf("�⵵ �Է�: ");
	scanf("%d", &tmp);
	res.year = tmp;
	tmp = 0;
	while (!tmp)
	{
		printf("�� �Է�: ");
		scanf("%d", &tmp);
		if (tmp <= 0 || tmp > 12) {
			puts("�ùٸ��� ���� �� �Է��Դϴ�.");
			tmp = 0;
		}
	}
	res.month = tmp;
	tmp = 0;
	while (!tmp)
	{
		printf("�� �Է�: ");
		scanf("%d", &tmp);
		if (tmp <= 0 || tmp > 31 ||
			(tmp > 30 && (res.month == APR || res.month == JUN || res.month == SEP || res.month == NOV)) ||
			(tmp > 29 && res.month == FEB) || (tmp > 28 && res.month == FEB && !ifleapyear(res.year))) {
			puts("�ùٸ��� ���� �� �Է��Դϴ�.");
			tmp = 0;
		}
	}
	res.dayn = tmp;
	return res;
}

int datecmp(const datetime datet1, const datetime datet2) { //�ð� ��
	if (datet1.year != datet2.year) return datet1.year - datet2.year;
	if (datet1.month != datet2.month) return datet1.month - datet2.month;
	if (datet1.dayn != datet2.dayn) return datet1.dayn - datet2.dayn;
	return 0;
}

char ifleapyear(const int year) { //���� 2���� ��� �Լ��� �״�� ������
	if (year % 4) return 0; //false, ���� �� ��찡 ���� �״� ���� ����
	else if (!(year % 100) && year % 400) return 0; //false, 400���� ������ �������� ������ 100���� ������ �������� ���
	else return 1; //true, �� ��
}