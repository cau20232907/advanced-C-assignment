#include "20232907 ������ ���� 5 ���� ���.h"

void main() {
	char* (*dict)[2] = NULL, * (*ptr)[2] = NULL; //���� ����, �� �� �Է��� �ܾ��� ��ġ ����
	const char* in = NULL; //����� �Է� ����(�Է� ���̴� ��)
	dict = dictread();
	if (!dict) return;

	while (1) {
		printf("�ܾ�? ");
		in = sgets(10, 5, stdin);
		if (!in) { //in �Ҵ� ���н�
			while (getchar() != '\n'); //���� �Է� �޾� ������
			puts(mid_in_falied);
			continue;
		}
		ptr = wordfinder(in, dict);
		if (ptr) printf("%s %s\n", (*ptr)[0], (*ptr)[1]);
		else puts("�Էµ� �ܾ ������ �����ϴ�.");
		freeset(&in);
	}
	freeset(&((*dict)[0]));
	freeset(&dict);
}