#include "20232907 ������ ���� 5 ���� ���.h"

void main2() {
	char last = '\0'; //�� �Է� ������ ���� ����
	const char* in = NULL; //�̹� �Է� ���� ��ġ
	unsigned int score = 0, fails = 3; //����, �Ǽ� ��ȸ ����
	char* (*dict)[2] = NULL, * (*ptr)[2] = NULL; //���� ����, �� �� �Է��� �ܾ��� ��ġ ����
	const char* dictstr = NULL; //�̹� �Ҹ� �ܾ��� ���� �ܾ ���������� ������ ���ڿ� free�� ���� ���� ���� �ʿ�

	dict = dictread();
	if (!dict) return;
	dictstr = (*dict)[0];

	
	//��ǻ�Ͱ� �ܾ� ����
	srand(time(NULL));
	//������ ���� ����
	for (unsigned int i = 0;;i++) { //RAND_MAX ���� 2�� 40�� ���� �̻��� �Ѿ���� ���� ���̶�� ������...
		if (RAND_MAX == (1 << i) - 1) {
			for (int j = 0; j < 32; j += i)
				score += rand() << j;
			break;
		}
	}
	//rand ��ȯ ���� �ִ밪�� ������ ���̸� ���� ���� �ִ� ó���̸�, ���� ������� ���� ���� score�� ���� �����
	ptr = dict + (score % dictlen(dict)); //������ �ִ� �ܾ� �� �������� �ϳ��� ��
	(*ptr)[1] = NULL; //�ߺ��� �ܾ� �Է� ����(Ȯ��)�� ���� �ܾ��� ���� ����������(���� ������� �ʱ� ������ ������ ��)
	in = (*ptr)[0]; //�� ������ ����� ���� ����
	last = in[strlen(in) - 1]; //������ ���� ����
	printf("��ǻ��: %s\n", in);
	score = 0;

	while (fails) {
		printf("�����ձ� �ܾ�: ");
		in = sgets(10, 3, stdin); //�Է�, �̹� �Լ��� �����Ǿ� �����Ƿ� �״�� �����
		if (!in) { //in �Ҵ� ���н�
			while (getchar() != '\n'); //�ϴ� ���� �Է� �޾� ������
			puts(mid_in_falied);
			fails--;
		}
		else if (strlen(in) < 3 || strlen(in) > 10) { //�ܾ� ���̰� ���� ������ ����� ���
			puts("�ùٸ��� ���� �ܾ��� �����Դϴ�.");
			fails--;
		}
		else if (last && last != *in) { //���� ���ڸ��� ���� ���ڰ� ���� �ʴ� ���
			puts("�ܾ �̾����� �ʽ��ϴ�.");
			fails--;
		}
		else {
			ptr = wordfinder(in, dict);
			if (ptr && (*ptr)[1]) { //�Էµ� �ܾ ������ �ְ� ������ �Էµ� �ܾ �ƴ϶��
				if (last) score++; //ù �ܾ �ƴϸ� ���� �ø���
				(*ptr)[1] = NULL; //�ߺ��� �ܾ� �Է� ����(Ȯ��)�� ���� �ܾ��� ���� ����������(���� ������� �ʱ� ������ ������ ��)
				last = in[strlen(in) - 1]; //������ ���� ����
			}
			else if (ptr) {
				puts("�̹� ���� �ܾ��Դϴ�.");
				fails--;
			}
			else {
				puts("���� �ܾ��Դϴ�.");
				fails--;
			}
		}
		freeset(&in);
		if (fails) printf("���� ���� %d��\n", score);
	}
	printf("���� ����: %d", score);
	freeset(&dictstr);
	freeset(&dict);
}