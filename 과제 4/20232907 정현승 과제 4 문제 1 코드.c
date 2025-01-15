#define _CRT_SECURE_NO_WARNINGS
#pragma warning(disable:6031)
#include <stdio.h>
#include <string.h>
#define len 50

void main() {
	char space[len] = { 0, }, cnt = 0, * pl = space;
	while (pl < space + len) {
		printf("= %d바이트 남음\n>", len - (int)(pl - space));
		scanf("%hhd", pl);
		pl++;
		switch (*(pl - 1))
		{
		case 1:
			if (pl + sizeof(char) > space + len) cnt--;
			else scanf("%hhd", pl);
			pl += sizeof(char);
			break;
		case 2:
			if (pl + sizeof(short) > space + len) cnt--;
			else scanf("%hd", (short*)pl);
			pl += sizeof(short);
			break;
		case 3:
			if (pl + sizeof(int) > space + len) cnt--;
			else scanf("%d", (int*)pl);
			pl += sizeof(int);
			break;
		case 4:
			if (pl + sizeof(long long) > space + len) cnt--;
			else scanf("%lld", (long long*)pl);
			pl += sizeof(long long);
			break;
		default:
			pl--;
			cnt--;
			break;
		}
		cnt++;
	}
	puts("= 저장 공간이 가득 찼습니다.");
	pl = space;
	printf("%hhd\n", cnt);
	while (cnt > 0) {
		pl++;
		switch (*(pl - 1))
		{
		case 1:
			printf("%hhdC ", *pl);
			pl += sizeof(char);
			break;
		case 2:
			printf("%hdS ", *(short*)pl);
			pl += sizeof(short);
			break;
		case 3:
			printf("%dI ", *(int*)pl);
			pl += sizeof(int);
			break;
		case 4:
			printf("%lldLL ", *(long long*)pl);
			pl += sizeof(long long);
			break;
		default:
			break;
		}
		cnt--;
	}
}