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
			printf("현재 시각은 %4d년 %2d월 %2d일 %2d시 %2d분 %2d초 입니다.\n", res_time.year, res_time.month, res_time.dayn, res_time.hour, res_time.minute, res_time.second);
		}
	}
}


datetime datetime_calculater(int timeint, const datetime timezone) {
	datetime fulltime = { 0,0,0,0,0,0,0 };

	//초 기록
	timeint += timezone.second; //GMT보정
	fulltime.second = timeint % 60;
	timeint /= 60;

	//분 기록
	timeint += timezone.minute;
	fulltime.minute = timeint % 60;
	timeint /= 60;

	//시 기록
	timeint += timezone.hour;
	fulltime.hour = timeint % 24;
	timeint /= 24;

	//날짜만이 남은 상태

	fulltime.dayt = (timeint + timezone.dayt) % 7;
	//요일 설정, timezone.dayt에는 설정한 날짜에 따른 요일이 들어온다고 보고, 날짜를 더해주기 전에 계산
	//다만 이 코드에서는 요일을 활용하지 않으므로 의미 없음

	timeint += timezone.month * 30 + timezone.dayn; //timezone.month에서 1달은 무조건 30일로 계산
	fulltime.year += timeint / 365; //일단 1년이 365일이라 하고 년도를 계산함
	timeint %= 365; //이제는 년도 부분이 필요 없으므로 나머지만 봄
	for (int i = timezone.year; i < timezone.year + fulltime.year; i++)
		if (ifleapyear(i)) {
			timeint--;
			//윤년이 있으면 하루를 뺌, 윤년 계산은 년도 기본값부터 이와 계산된 년도의 합까지 계산
			if (timeint < 0) {
				fulltime.year--;
				if (i < timezone.year + fulltime.year) timeint += 365;
				else timeint += 366; //마지막 년도 체크에서 년도가 바뀌면 윤년 체크시에 중복 계산 되므로 이를 대비해 1을 더해줌
			}
		}
	fulltime.year += timezone.year; //년도 기본값
	//년도 확인 완료

	//월 확인
	//여기서 timeint가 0인 경우 1월 1일임(0일이 아님), 따라서 1일이 더해진다는 점을 감안하고 계산해야 함, 따라서 며칠 이내의 경우, <=가 아닌 <를 사용함
	fulltime.month = timeint / 30 + 1; //일단 1개월이 30일이라 하고 달을 계산함, timeint가 30 이내여도 1월인 등의 문제로 1을 더해줌, 다만 현재는 13월이 가능함
	timeint %= 30;
	switch (fulltime.month)
	{
	case 1: //1월인 경우 추가로 할 작업이 없음
		break;
	default:
		//1월은 총 31일, 30일에서 1일이 모자람
		month_calculater(&timeint, &fulltime.month, 1, 3); //함수 내용은 함수 참고, 단 2월은 예외로 인해 함수로 빼지 않고 그대로 작성하였으므로 아래를 참고해도 좋음
		if (fulltime.month == 2 && (timeint < 29 || (timeint < 28 && !ifleapyear(fulltime.year)))) {
			//2월이며 29일 이내(윤년이 아니면 28일 이내)인 경우, 월이 맞아도 날짜가 틀리면 3월이므로 처리하지 않음(조건문을 함수 밖으로 뺀 이유)
			break;
		}

		//2월은 윤년인 경우 29일로 30일에 1일이 남고, 아니면 28일로 2일이 남음
		if (ifleapyear(fulltime.year)) timeint++;
		else timeint += 2;
		if (timeint >= 30) { //3월 1일, 2일로 3월이지만 2월로 계산된 경우 달을 바꿔줌
			fulltime.month++;
			timeint -= 30;
		}
		if (fulltime.month <= 3) break; //3월인경우 탈출

		//이후 같은 상황이 반복되니 여기서부터는 따로 빼줌(2월 날짜 부족, 2월 날짜 보충까지 함수로 빼면 복잡해질 수 있으니 위 둘은 예외로 남겨놓음)
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
	if (year % 4) return 0; //false, 보통 이 경우가 많을 테니 먼저 빼줌
	else if (!(year % 100) && year % 400) return 0; //false, 400으로 나누어 떨어지지 않으나 100으로 나누어 떨어지는 경우
	else return 1; //true, 그 외
}

char month_calculater(int* ptimeint, char* pmonth, const char cntmonth, const char nextmonth) {
	//계속 이 구문이 반복되어서 따로 빼줌
	(*ptimeint)--; //한 달이 31일, 30일에서 1일이 모자란 경우 하루를 빼줌
	if (*ptimeint < 0) {
		//31일을 계산하지 않아 1월을 깎아야 하는 경우
		(*pmonth)--;
		(*ptimeint) += 30;
		if (*pmonth == cntmonth) (*ptimeint)++; //31일인 경우, 1을 빼는 데서 끝내면 30일과 31일 모두 30일로 표시되니 구분을 위해 1을 더함
	}
	if (*pmonth <= nextmonth) return 1; //다음 31일이 있는 달 이전의 달일경우 탈출
	else return 0;
}