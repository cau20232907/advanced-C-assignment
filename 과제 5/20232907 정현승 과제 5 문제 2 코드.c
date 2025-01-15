#include "20232907 정현승 과제 5 공통 헤더.h"

void main2() {
	char last = '\0'; //전 입력 마지막 글자 저장
	const char* in = NULL; //이번 입력 저장 위치
	unsigned int score = 0, fails = 3; //점수, 실수 기회 저장
	char* (*dict)[2] = NULL, * (*ptr)[2] = NULL; //사전 정보, 그 중 입력한 단어의 위치 저장
	const char* dictstr = NULL; //이미 불린 단어의 영어 단어를 지워버리기 때문에 문자열 free를 위한 별도 저장 필요

	dict = dictread();
	if (!dict) return;
	dictstr = (*dict)[0];

	
	//컴퓨터가 단어 고르기
	srand(time(NULL));
	//무작위 숫자 고르기
	for (unsigned int i = 0;;i++) { //RAND_MAX 값이 2의 40억 제곱 이상을 넘어가지는 않을 것이라고 가정함...
		if (RAND_MAX == (1 << i) - 1) {
			for (int j = 0; j < 32; j += i)
				score += rand() << j;
			break;
		}
	}
	//rand 반환 값의 최대값이 사전의 길이를 넘지 못해 넣는 처리이며, 아직 사용하지 않은 변수 score를 빌려 사용함
	ptr = dict + (score % dictlen(dict)); //사전에 있는 단어 중 무작위로 하나를 고름
	(*ptr)[1] = NULL; //중복된 단어 입력 방지(확인)를 위해 단어의 뜻을 지워버리기(뜻을 사용하지 않기 때문에 가능한 것)
	in = (*ptr)[0]; //고른 문장은 출력을 위해 저장
	last = in[strlen(in) - 1]; //마지막 글자 저장
	printf("컴퓨터: %s\n", in);
	score = 0;

	while (fails) {
		printf("끝말잇기 단어: ");
		in = sgets(10, 3, stdin); //입력, 이미 함수가 구현되어 있으므로 그대로 사용함
		if (!in) { //in 할당 실패시
			while (getchar() != '\n'); //일단 남은 입력 받아 버리기
			puts(mid_in_falied);
			fails--;
		}
		else if (strlen(in) < 3 || strlen(in) > 10) { //단어 길이가 문제 조건을 벗어나는 경우
			puts("올바르지 않은 단어의 길이입니다.");
			fails--;
		}
		else if (last && last != *in) { //이전 끝자리와 시작 문자가 맞지 않는 경우
			puts("단어가 이어지지 않습니다.");
			fails--;
		}
		else {
			ptr = wordfinder(in, dict);
			if (ptr && (*ptr)[1]) { //입력된 단어가 사전에 있고 이전에 입력된 단어가 아니라면
				if (last) score++; //첫 단어가 아니면 점수 올리기
				(*ptr)[1] = NULL; //중복된 단어 입력 방지(확인)를 위해 단어의 뜻을 지워버리기(뜻을 사용하지 않기 때문에 가능한 것)
				last = in[strlen(in) - 1]; //마지막 글자 저장
			}
			else if (ptr) {
				puts("이미 나온 단어입니다.");
				fails--;
			}
			else {
				puts("없는 단어입니다.");
				fails--;
			}
		}
		freeset(&in);
		if (fails) printf("현재 점수 %d점\n", score);
	}
	printf("최종 점수: %d", score);
	freeset(&dictstr);
	freeset(&dict);
}