#include "20232907 정현승 과제 5 공통 헤더.h"

void main() {
	char* (*dict)[2] = NULL, * (*ptr)[2] = NULL; //사전 정보, 그 중 입력한 단어의 위치 저장
	const char* in = NULL; //사용자 입력 저장(입력 길이는 모름)
	dict = dictread();
	if (!dict) return;

	while (1) {
		printf("단어? ");
		in = sgets(10, 5, stdin);
		if (!in) { //in 할당 실패시
			while (getchar() != '\n'); //남은 입력 받아 버리기
			puts(mid_in_falied);
			continue;
		}
		ptr = wordfinder(in, dict);
		if (ptr) printf("%s %s\n", (*ptr)[0], (*ptr)[1]);
		else puts("입력된 단어가 사전에 없습니다.");
		freeset(&in);
	}
	freeset(&((*dict)[0]));
	freeset(&dict);
}