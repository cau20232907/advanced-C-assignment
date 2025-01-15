#include <stdio.h>
#include <time.h>
#define Time_Starter clock() //���� �۾��� �ð��� �����Ϸ��� �̸� 0���� �ٲپ��ָ� ��
#define Time_Limit 5 * CLOCKS_PER_SEC //�ð�����

void main() {
	double Pi = 0;
	unsigned int diviser = 1;
	clock_t startc = Time_Starter;

	for (diviser = 1; diviser >= 1 && (clock() - startc) < Time_Limit; diviser++) {
		//���� ������ �����÷� ����
		if (diviser % 2) Pi += 4.0 / (diviser * 2 - 1);
		else Pi -= 4.0 / (diviser * 2 - 1);
	}
	diviser--;
	printf("�ҿ�� �ð�: %.4f��\n", (double)(clock() - startc) / CLOCKS_PER_SEC);
	printf("PI == %.10lf\n", Pi);
	printf("�ݺ��� Ƚ��: %d", diviser);
}