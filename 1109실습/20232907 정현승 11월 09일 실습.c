#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <ctype.h>
//void 중간실습1();
#define MAX_strlen 102

void main() {
	char str[MAX_strlen], * tmp;
	int res = 0;
	fgets(str, MAX_strlen, stdin);
	tmp = strtok(str, "+ \n");
	while (tmp) {
		res += atoi(tmp);
		tmp = strtok(NULL, "+ \n");
	}
	sprintf(str, "%d", res);
	puts(str);
}
/*
void 중간실습1() {
	char str[MAX_strlen]="apple";
	char let = 'e';
	unsigned int cnt = 1;
	printf("(%u) %s\n", cnt, str);
	while (1) {
		printf("끝말잇기 단어는? ");
		fgets(str, MAX_strlen, stdin);
		str[strlen(str) - 1] = 0;
		if (*str == let) {
			cnt++;
			printf("(%u) %s\n", cnt, str);
			let = str[strlen(str) - 1];
		}
		else {
			puts("옳지 않은 끝말잇기 단어입니다. 다시 입력하세요.");
		}
	}
}

#ifdef MAX_strlen
#undef MAX_strlen
#endif // MAX_strlen
*/