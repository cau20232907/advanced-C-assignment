#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void page67();
void page71();

#define dig 16 /*�ڸ��� ������ �͵� #define�� ����*/
#define encode_KR 2 /*�ѱ� 1�ڴ� ����Ʈ ��*/
void main() {
	char* digit[] = { "��","��","��","��","��","��","��","ĥ","��","��" };
	char* place[] = { "õ","��","��",digit[0] + strlen(digit[0])/*'\0'(�ι���) �ִµ� �ƹ����� ����ָ� ��, ���⼭�� "��" ���ڿ��� �� �ڿ� �ִ� '\0'�� ����� ��, 0�� �ƴ϶� 1~9���� �ƹ� ���ڳ� �־ �� ���ư�*/ };
	/*������ place[3]�� �ι��� �� ���(���� �� �� ����) ""�θ� �ᵵ �� ���ư�, �� ���� ���� �� �Ǹ� �� ����� "" �־��� ��*/
	char* place4[] = { "�� ","�� ","�� ",digit[0] + strlen(digit[0]) };
	long long num = 0;
	char num_dig[dig] = { 0, }; /*������ 0~9�� ���� 1����Ʈ�� �� �൵ ��������*/
	char res[((dig - 1) * 2 + 1 + 3) * encode_KR + 4] = { 0 };/*�ִ� 7���� ���� 7*(���ڴ� ����Ʈ ��)+(�ι��� 1��)==15�� ���� ����*/
	scanf("%lld", &num);
	if (num == 0) strcat(res, digit[0]);
	else {
		for (int i = dig - 1; i >= 0; i--) {
			num_dig[i] = num % 10;
			num /= 10;
		}
		for (int i = 0, flag; i < 4; i++) {
			flag = 0;
			for (int j = 0; j < 4; j++) {
				if (num_dig[i * 4 + j]) {
					strcat(res, digit[num_dig[i * 4 + j]]);
					strcat(res, place[j]);
					/*place[3]�� �ι����� �ּҸ� ��������� ����ó�� �ʿ䰡 ����(���� ���ڿ��� ""�� ���ϰ� �Ǵ� �ƹ��� ��ȭ�� ������ ����)*/
					flag++;
				}
			}
			if (flag) strcat(res, place4[i]);
		}
	}
	puts(res);
}

void page67() {
	int a = 0x12345678;
	char* s = (char*)&a; // ��� �����ϱ� ���� �� ��ȯ
	int i;
	for (i = 0; i < 4; i++) {
		printf("%X ", *s);
		s++;
	}
}

void page71() { //x86 only
	int A[5] = { 0 }, B[4] = { 0 };
	int* ptr = &B[6];
	*ptr = 1;
	printf("%d\n", A[0]);
}