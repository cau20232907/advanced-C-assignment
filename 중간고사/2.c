#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <time.h>
#include <string.h>

void main2() {
	int temp, score = 0, total = 0;
	clock_t startc, endc;
	scanf("%d", &temp);
	puts("0��");
	startc = clock();
	for (int i = 0; i < 9; i++) {
		scanf("%d", &temp);
		endc = clock();
		if (endc - startc == CLOCKS_PER_SEC) {
			score = 2000;
		}
		else if (endc - startc < CLOCKS_PER_SEC) {
			score = (endc - startc);
		}
		else if (endc - startc < 2 * CLOCKS_PER_SEC) {
			score = 2000 - (endc - startc);
		}
		else score = 0;
		printf("(%dȸ) %.3f��(+%d)\n", i + 1, ((endc - startc) * 1.0 / CLOCKS_PER_SEC), score);
		total += score;
		startc = endc;
	}
	printf("\n�� ���� %d��", total);
}