#define try_message //��õ� �޽��� �����
#define trying_again "������ ������ �����մϴ�. �ٽ� �õ��ϰ� �ֽ��ϴ�..."
#define try_succeed "������� ������ �ذ��Ͽ����ϴ�."
#define try_falied "������ ������ ������ �����ϴ�. �´� �� ���� ���� �����̹Ƿ� ���� ����� ���� �ʰ� �Ѿ�ϴ�."
#include "20232907 ������ ���� 3 ���� ���.h"
#define MAX_strlen 61 //�ִ� ���ڿ� ��
#define rpt 5
typedef unsigned char rpt_type;
typedef unsigned char strlen_type;
#define check_range 3

void sentence_selector(char to[MAX_strlen]); //���� ����
strlen_type check_up(const char ans[MAX_strlen], char *to); //���� Ȯ��

void main() {
	char ans[MAX_strlen], * in = NULL; //���� ����, �Է��� �� ���� 
	clock_t pre, aft; //����, ���� �ð� ����
	float gap, total = 0;
	strlen_type len;

	srand(time(NULL));
	for (rpt_type cnt = 0; cnt < rpt; cnt++) {
		sentence_selector(ans);
		printf("(%d/%d) ������ �Է��ϼ���.\n", cnt + 1, rpt);
		printf("= %s\n", ans);
		printf("= ");
		pre = clock();
		in = sgets(MAX_strlen, 10, stdin);
		aft = clock();
		if (in) { //�Է��� ���������� �Ǿ��ٸ� Ʋ�� �� �� ���� ���
			len = strlen(ans);
			len -= check_up(ans, in);
			putchar(' ');
			putchar(' '); //���� ������ �Է��� ���ڰ� ���� �ʴ� �������� ���� 2���� �̸� ����
			puts(in);
			gap = len / ((aft - pre) / 60.0 / CLOCKS_PER_SEC); //�д� Ÿ�� ���
			printf("%.0fŸ/��\n", gap);
			total += gap; //���� ���, �Ҽ��� ������ ���� 5ȸ�� ���� ���̴� �Ͱ� ��������� ������ ����
			freeset(&in);
		}
	}
	printf("����� ������ %.0f�� �Դϴ�.", total);
}

void sentence_selector(char to[MAX_strlen]) {
	strlen_type wordlen = 0, sentlen = 0; //���� �̹� �ܾ� ����, ���� ���� ���� ����
	char* pos = to; //Ŀ��
	arrset(to, MAX_strlen); //���� ���� ���� �����
	while (1) {
		wordlen = rand() % 7 + 4; //�ܾ� ����: 4~10��
		if (sentlen + wordlen > 50) break; //�ܾ ������ ���� ���̸� �ʰ��ϰ� �Ǵ� ���, ���� �ߴ�(������ ���� ���� 40~50��)
		for (strlen_type i = 0; i < wordlen; i++) {
			*pos++ = rand() % 26 + 'A'; //A~Z�� ������ ����, Ŀ�� ������
			sentlen++; //���� �� ����
		}
		*pos++ = ' '; //���� ����
	}
	*(pos - 1) = '\0'; //������ ���� ������ �������� '\0'�� �ٲ���
}

strlen_type check_up(const char ans[MAX_strlen], char *to) { //�� ���α׷����� ���� �ָ��� �κ�, �̰� ��Ȯ�� �Ϸ��� AI�� �ʿ���
	strlen_type cnt = 0, ani = 0, check = 0; //���� Ʋ�� ���� ��, ans�� Ŀ��, ���ڰ� �� �� �����ų� �и� ��� Ȯ���� �� ���̴� ����
	//cnt�� ��Ȯ���� �Է� �ӵ� ���ÿ� ���� �� ���� ����
	unsigned int toi = 0; //to�� Ŀ��, ���ڰ� �󸶳� ���� �� �𸣹Ƿ� type �и�
	//���ڰ� �и� ���ɼ� ������ ans�� Ŀ���� to�� Ŀ���� �и���
	char flag = 0;
	while (ani < MAX_strlen && (ans[ani] || to[toi])) {
		if (ans[ani] == to[toi]) { //Ʋ�� �κ� ����
			if (to[toi] != '^') to[toi] = ' ';
		}
		else if (!ans[ani]) { //�̹� �� �κ��� ���� ���
			while (to[toi]) {
				to[toi++] = '^';
				cnt++;
			}
			break;
		}
		else if (!to[toi]) { //�Է��� ���� ���� ���
			to[toi - 1] = '^'; //to[toi]�� ������ ��� '\0'�� ����� �����Ͽ�, ������ ���� ������ ���ɼ����� ���� �� ĭ �տ� ������
			break;
		}
		else {
			flag = 0;
			for (check = 1; check < check_range * 2; check++) { //�� �� ����� ������
				//���� �� ���� �߰����� �������� Ȯ��
				if (!strncmp(ans + ani + check, to + toi, check_range)) {
					to[toi] = '^';
					ani += check;
					cnt++;
					flag += 2;
					break;
				}
				//���� �� ���� �߰��� �� ����ִ��� Ȯ��
				if (!strncmp(ans + ani, to + toi + check, check_range)) {
					for (strlen_type i = 0; i < check; i++) {
						to[toi++] = '^';
					}
					to[toi] = ' '; //�̸� �� ���� ������ ��������� ����
					cnt += check;
					flag++;
					break;
				}
			}
			//�� �̿��� ���(�׳� ���� �ϳ��� Ʋ�� ������ Ȯ��)
			if (!flag) {
				to[toi] = '^';
				cnt++;
			}
		}
		ani++;
		toi++;
	}
	return cnt;
}