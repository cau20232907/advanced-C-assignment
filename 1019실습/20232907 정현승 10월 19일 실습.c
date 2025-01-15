#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void page67();
void page71();

#define dig 16 /*자릿수 관련한 것들 #define로 빼줌*/
#define encode_KR 2 /*한글 1자당 바이트 수*/
void main() {
	char* digit[] = { "영","일","이","삼","사","오","육","칠","팔","구" };
	char* place[] = { "천","백","십",digit[0] + strlen(digit[0])/*'\0'(널문자) 있는데 아무데나 찍어주면 됨, 여기서는 "영" 문자열의 맨 뒤에 있는 '\0'을 찍어준 것, 0이 아니라 1~9까지 아무 숫자나 넣어도 잘 돌아감*/ };
	/*마지막 place[3]에 널문자 안 찍고(저런 식 안 쓰고) ""로만 써도 잘 돌아감, 저 식이 이해 안 되면 식 지우고 "" 넣어줄 것*/
	char* place4[] = { "조 ","억 ","만 ",digit[0] + strlen(digit[0]) };
	long long num = 0;
	char num_dig[dig] = { 0, }; /*어차피 0~9만 들어가니 1바이트만 써 줘도 문제없음*/
	char res[((dig - 1) * 2 + 1 + 3) * encode_KR + 4] = { 0 };/*최대 7글자 들어가니 7*(글자당 바이트 수)+(널문자 1개)==15로 길이 맞춤*/
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
					/*place[3]에 널문자의 주소를 찍어줬으니 예외처리 필요가 없음(기존 문자열에 ""을 더하게 되니 아무런 변화도 생기지 않음)*/
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
	char* s = (char*)&a; // 경고를 방지하기 위한 형 변환
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