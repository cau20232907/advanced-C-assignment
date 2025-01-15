#include <stdio.h>
#include <time.h>


enum day_type { sun, mon, tue, wed, thu, fri, sat };
typedef struct datetime {
	int year;
	char month;
	char dayn;
	enum day_type dayt;
	char hour;
	char minute;
	char second;
} datetime;

datetime datetime_calculater(int timeint, const datetime timezone);
char ifleapyear(const int year);
char month_calculater(int* ptimeint, char* pmonth, const char cntmonth, const char nextmonth);


void main1() {
	int timeint = 0;
	const datetime timezone = { 1970,0,0,thu,9,0,0 };
	datetime res_time = { 0, };
	while (1) {
		if (time(NULL) != timeint) {
			timeint = time(NULL);
			res_time = datetime_calculater(timeint, timezone);
			printf("���� �ð��� %4d�� %2d�� %2d�� %2d�� %2d�� %2d�� �Դϴ�.\n", res_time.year, res_time.month, res_time.dayn, res_time.hour, res_time.minute, res_time.second);
		}
	}
}


datetime datetime_calculater(int timeint, const datetime timezone) {
	datetime fulltime = { 0,0,0,0,0,0,0 };

	//�� ���
	timeint += timezone.second; //GMT����
	fulltime.second = timeint % 60;
	timeint /= 60;

	//�� ���
	timeint += timezone.minute;
	fulltime.minute = timeint % 60;
	timeint /= 60;

	//�� ���
	timeint += timezone.hour;
	fulltime.hour = timeint % 24;
	timeint /= 24;

	//��¥���� ���� ����

	fulltime.dayt = (timeint + timezone.dayt) % 7;
	//���� ����, timezone.dayt���� ������ ��¥�� ���� ������ ���´ٰ� ����, ��¥�� �����ֱ� ���� ���
	//�ٸ� �� �ڵ忡���� ������ Ȱ������ �����Ƿ� �ǹ� ����

	timeint += timezone.month * 30 + timezone.dayn; //timezone.month���� 1���� ������ 30�Ϸ� ���
	fulltime.year += timeint / 365; //�ϴ� 1���� 365���̶� �ϰ� �⵵�� �����
	timeint %= 365; //������ �⵵ �κ��� �ʿ� �����Ƿ� �������� ��
	for (int i = timezone.year; i < timezone.year + fulltime.year; i++)
		if (ifleapyear(i)) {
			timeint--;
			//������ ������ �Ϸ縦 ��, ���� ����� �⵵ �⺻������ �̿� ���� �⵵�� �ձ��� ���
			if (timeint < 0) {
				fulltime.year--;
				if (i < timezone.year + fulltime.year) timeint += 365;
				else timeint += 366; //������ �⵵ üũ���� �⵵�� �ٲ�� ���� üũ�ÿ� �ߺ� ��� �ǹǷ� �̸� ����� 1�� ������
			}
		}
	fulltime.year += timezone.year; //�⵵ �⺻��
	//�⵵ Ȯ�� �Ϸ�

	//�� Ȯ��
	//���⼭ timeint�� 0�� ��� 1�� 1����(0���� �ƴ�), ���� 1���� �������ٴ� ���� �����ϰ� ����ؾ� ��, ���� ��ĥ �̳��� ���, <=�� �ƴ� <�� �����
	fulltime.month = timeint / 30 + 1; //�ϴ� 1������ 30���̶� �ϰ� ���� �����, timeint�� 30 �̳����� 1���� ���� ������ 1�� ������, �ٸ� ����� 13���� ������
	timeint %= 30;
	switch (fulltime.month)
	{
	case 1: //1���� ��� �߰��� �� �۾��� ����
		break;
	default:
		//1���� �� 31��, 30�Ͽ��� 1���� ���ڶ�
		month_calculater(&timeint, &fulltime.month, 1, 3); //�Լ� ������ �Լ� ����, �� 2���� ���ܷ� ���� �Լ��� ���� �ʰ� �״�� �ۼ��Ͽ����Ƿ� �Ʒ��� �����ص� ����
		if (fulltime.month == 2 && (timeint < 29 || (timeint < 28 && !ifleapyear(fulltime.year)))) {
			//2���̸� 29�� �̳�(������ �ƴϸ� 28�� �̳�)�� ���, ���� �¾Ƶ� ��¥�� Ʋ���� 3���̹Ƿ� ó������ ����(���ǹ��� �Լ� ������ �� ����)
			break;
		}

		//2���� ������ ��� 29�Ϸ� 30�Ͽ� 1���� ����, �ƴϸ� 28�Ϸ� 2���� ����
		if (ifleapyear(fulltime.year)) timeint++;
		else timeint += 2;
		if (timeint >= 30) { //3�� 1��, 2�Ϸ� 3�������� 2���� ���� ��� ���� �ٲ���
			fulltime.month++;
			timeint -= 30;
		}
		if (fulltime.month <= 3) break; //3���ΰ�� Ż��

		//���� ���� ��Ȳ�� �ݺ��Ǵ� ���⼭���ʹ� ���� ����(2�� ��¥ ����, 2�� ��¥ ������� �Լ��� ���� �������� �� ������ �� ���� ���ܷ� ���ܳ���)
		if (month_calculater(&timeint, &fulltime.month, 3, 5)) break;
		if (month_calculater(&timeint, &fulltime.month, 5, 7)) break;
		if (month_calculater(&timeint, &fulltime.month, 7, 8)) break;
		if (month_calculater(&timeint, &fulltime.month, 8, 10)) break;
		if (month_calculater(&timeint, &fulltime.month, 10, 12)) break;
		if (month_calculater(&timeint, &fulltime.month, 12, 13)) break;

		break;
	}
	fulltime.dayn = timeint + 1;
	return fulltime;
}

char ifleapyear(const int year) {
	if (year % 4) return 0; //false, ���� �� ��찡 ���� �״� ���� ����
	else if (!(year % 100) && year % 400) return 0; //false, 400���� ������ �������� ������ 100���� ������ �������� ���
	else return 1; //true, �� ��
}

char month_calculater(int* ptimeint, char* pmonth, const char cntmonth, const char nextmonth) {
	//��� �� ������ �ݺ��Ǿ ���� ����
	(*ptimeint)--; //�� ���� 31��, 30�Ͽ��� 1���� ���ڶ� ��� �Ϸ縦 ����
	if (*ptimeint < 0) {
		//31���� ������� �ʾ� 1���� ��ƾ� �ϴ� ���
		(*pmonth)--;
		(*ptimeint) += 30;
		if (*pmonth == cntmonth) (*ptimeint)++; //31���� ���, 1�� ���� ���� ������ 30�ϰ� 31�� ��� 30�Ϸ� ǥ�õǴ� ������ ���� 1�� ����
	}
	if (*pmonth <= nextmonth) return 1; //���� 31���� �ִ� �� ������ ���ϰ�� Ż��
	else return 0;
}