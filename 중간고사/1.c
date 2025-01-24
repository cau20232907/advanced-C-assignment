#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>

void main1() {
	char space[50];
	char type = 0, cnt = 0, pl = 0;
	while (pl < 50) {
		printf("= %d바이트 남음\n>", 50 - pl);
		scanf("%hhd", &type);
		space[pl] = type;
		pl++;
		switch (type)
		{
		case 1:
			if (pl + sizeof(char) > 50) {
				pl += sizeof(char);
				cnt--;
				break;
			}
			scanf("%hhd", space + pl);
			pl += sizeof(char);
			break;
		case 2:
			if (pl + sizeof(short) > 50) {
				pl += sizeof(short);
				cnt--;
				break;
			}
			scanf("%hd", space + pl);
			pl += sizeof(short);
			break;
		case 3:
			if (pl + sizeof(int) > 50) {
				pl += sizeof(int);
				cnt--;
				break;
			}
			scanf("%d", space + pl);
			pl += sizeof(int);
			break;
		case 4:
			if (pl + sizeof(long long) > 50) {
				pl += sizeof(long long);
				cnt--;
				break;
			}
			scanf("%lld", space + pl);
			pl += sizeof(long long);
			break;
		default:
			pl--;
			break;
		}
		cnt++;
	}
	puts("= 저장 공간이 가득 찼습니다.");
	pl = 0;
	printf("%hhd\n", cnt);
	while (cnt > 0) {
		type = space[pl++];
		switch (type)
		{
		case 1:
			printf("%hhdC ", *(space + pl));
			pl += sizeof(char);
			break;
		case 2:
			printf("%hdS ", *(short*)(space + pl));
			pl += sizeof(short);
			break;
		case 3:
			printf("%dI ", *(int*)(space + pl));
			pl += sizeof(int);
			break;
		case 4:
			printf("%lldLL ", *(long long *)(space + pl));
			pl += sizeof(long long);
			break;
		default:
			break;
		}
		cnt--;
	}
}