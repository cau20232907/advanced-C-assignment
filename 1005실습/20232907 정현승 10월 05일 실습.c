#include <stdio.h>
#include <time.h>
#define Time_Starter clock() //사전 작업도 시간에 포함하려면 이를 0으로 바꾸어주면 됨
#define Time_Limit 5 * CLOCKS_PER_SEC //시간제한

void main() {
	double Pi = 0;
	unsigned int diviser = 1;
	clock_t startc = Time_Starter;

	for (diviser = 1; diviser >= 1 && (clock() - startc) < Time_Limit; diviser++) {
		//왼쪽 조건은 오버플로 방지
		if (diviser % 2) Pi += 4.0 / (diviser * 2 - 1);
		else Pi -= 4.0 / (diviser * 2 - 1);
	}
	diviser--;
	printf("소요된 시간: %.4f초\n", (double)(clock() - startc) / CLOCKS_PER_SEC);
	printf("PI == %.10lf\n", Pi);
	printf("반복한 횟수: %d", diviser);
}