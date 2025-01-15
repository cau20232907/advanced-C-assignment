#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define default_money 5000 //초기자본
#define default_price 300 //구매가
#define default_fare 600 //통행료
#define player_num 2 //플레이어 수
#define max_turn 30 //최대 턴 수
#define board_size 10
#define board_acr 5
#define board_acr_gap 1
#define board_dwn 2 //3 이상이면 활용하겠으나 2라서 이용하지 않을 것으로 예상
#define namelen 10

struct _City; //함수 원형 선언처럼 썼는데 되는지는 봐야겠네요...

typedef struct _Player {
	char* name;
	struct _City* place; //이게 원래 되는 건지는 확인을 해봐야 할 것 같아요...
	unsigned int money;
} person;

typedef struct _City {
	char* name;
	struct _Player* owner; //주인
	unsigned int price; //구매가
	unsigned int fare; //통행료
} city;

void printboard(const city board[board_size]);

void main() {
	city board[board_size] = {
		{"START",NULL,0,0},
		{"Seoul",NULL,default_price,default_fare},
		{"Tokyo",NULL,default_price,default_fare},
		{"Sydney",NULL,default_price,default_fare},
		{"LA",NULL,default_price,default_fare},
		{"Cairo",NULL,default_price,default_fare},
		{"Phuket",NULL,default_price,default_fare},
		{"New delhi",NULL,default_price,default_fare},
		{"Hanoi",NULL,default_price,default_fare},
		{"Paris",NULL,default_price,default_fare}
	};
	person player[player_num] = { {"COM1",board,default_money},{"COM2",board,default_money} };
	char dice = 0;
	person* bankrupt = NULL; //파산으로 인한 중도 게임 종료시 파산한 플레이어 저장
	unsigned int i = 0; //반복문에서 사용
	person* p = player; //반복문에서 사용

	srand(time(NULL));

	for (i = 0; i < max_turn && !bankrupt; i++) { //턴 진행
		for (p = player; p < player + player_num && !bankrupt; p++) {
			dice = rand() % 6 + 1;
			p->place = board + (((p->place - board) + dice) % board_size); //칸 이동
			system("cls");
			printboard(board); //매번 보드 그리기
			printf("%s의 주사위: %hhd\n", p->name, dice);
			if (p->place == board) //시작 위치인 경우(땅을 사고팔지 않음)
				puts(p->place->name); //이름 출력 외 추가 작업 없음
			else if (!(p->place->owner)) { //주인이 없는 경우
				printf("%s (주인 없음)\n", p->place->name);
				if (p->money >= p->place->price) { //구매 가능한 경우
					printf("%s을/를 구매할 수 있음\n", p->place->name);
					p->place->owner = p;
					p->money -= p->place->price; //구매 진행
				}
				else
					printf("%s을/를 구매할 수 없음\n", p->place->name);
			}
			else if (p->place->owner == p) //도착한 땅의 주인이 자기 자신인 경우
				printf("%s (%s)\n", p->place->name, p->name);
			else {  //도착한 땅의 주인이 다른 경우
				printf("%s (%s)\n", p->place->name, p->place->owner->name);
				if (p->money < p->place->fare) { //통행료를 지불할 능력이 없다면
					printf("%s이/가 %s의 통행료를 지불할 수 없음", p->name, p->place->name);
					bankrupt = p;
					break;
				}
				p->money -= p->place->fare;
				p->place->owner->money += p->place->fare;
				printf("%s이/가 %s에게 %u 지불\n", p->name, p->place->owner->name, p->place->fare);
				printf("%s의 잔고 %u.\n", p->place->owner->name, p->place->owner->money);
			}
			printf("%s의 잔고 %u.\n", p->name, p->money);
			system("pause"); //화면을 보여줌
		}
	}

	putchar('\n');
	if (bankrupt)
		printf("%s의 파산으로 게임이 종료되었습니다.\n", bankrupt->name);
	else
		printf("지정된 턴 수 %u턴이 모두 지나 게임을 종료합니다\n", max_turn);

	for (p = player; p < player + player_num; p++) {
		//게임 종료 시점에서의 잔고 보여주기
		if (p == bankrupt) printf("%s 파산\n", p->name);
		else printf("%s의 잔고 %u.\n", p->name, p->money);
	}
}

void printboard(const city board[board_size]) { //보드 내 플레이어의 위치를 보여주지 않기 때문에 이거 하나면 충분함
	unsigned char i = 0, k = 0;
	unsigned int j = 0;
	char str[((namelen + 2 + board_acr_gap) * board_acr + 1) * 4 + 1] = { 0, }, * pl = str;
	//각각 출력할 string을 저장할 배열과 커서 선언
	size_t len = 0; //strlen 함수를 수시로 사용함에 따라, 이를 저장하는 게 효율적이라 생각해 저장할 변수 생성

	//k가 0일때 5+i를 계산하고, k가 1일때 4-i를 계산하는 방법: (board_acr - k) - i * (k * 2 - 1)
	//앞은 k가 0일때 5, 1일때 4이며, 뒤는 k가 0이면 i에 (-1)을 곱한 것을 빼고, k가 1이면 i에 1을 곱한 것을 뺌(덧셈, 뺄셈 전환)
#define boardpl_cal ((board_acr - k) - i * (k * 2 - 1)) //이를 #define해줌(함수 마지막에 #undef를 해주려 하기에 여기에 작성함)
	for (k = 0; k < 2; k++) {
		for (i = 0; i < board_acr; i++) { //위쪽 경계 표시
			*pl++ = '+'; //한 글자 쓰면서 커서 옮기기
			for (j = 0; j < namelen; j++)
				*pl++ = '-';
			*pl++ = '+';
			for (j = 0; j < board_acr_gap; j++)
				*pl++ = ' '; //칸 간격
		}
		*pl++ = '\n';
		for (i = 0; i < board_acr; i++) { //도시 이름
			*pl++ = '|';
			len = strlen(board[boardpl_cal].name);
			for (j = 0; j < (namelen - len + 1) / 2; j++) *pl++ = ' ';
			//중앙정렬을 위한 앞 공백 출력
			//앞 공백의 수와 뒤 공백의 수가 일치하지 않으면 앞에 공백을 하나 덜 출력함
			strcpy(pl, board[boardpl_cal].name);
			pl += len;
			for (j = 0; j < (namelen - len) / 2; j++) *pl++ = ' ';
			//중앙정렬을 위한 뒤 공백 출력
			*pl++ = '|';
			for (j = 0; j < board_acr_gap; j++)
				*pl++ = ' '; //칸 간격
		}
		*pl++ = '\n';
		for (i = 0; i < board_acr; i++) { //주인 이름
			*pl++ = '|';
			if (board[boardpl_cal].owner) { //주인이 있다면 주인 이름 출력
				len = strlen((board[boardpl_cal].owner)->name);
				for (j = 0; j < (namelen - len + 1) / 2 - 1; j++) //중앙정렬을 위한 앞 공백 출력
					*pl++ = ' '; //앞 공백의 수와 뒤 공백의 수가 일치하지 않으면 앞에 공백을 하나 덜 출력함
				*pl++ = '(';
				strcpy(pl, (board[boardpl_cal].owner)->name);
				pl += len;
				*pl++ = ')';
				for (j = 0; j < (namelen - len) / 2 - 1; j++) //중앙정렬을 위한 뒤 공백 출력
					*pl++ = ' ';
			}
			else for (j = 0; j < namelen; j++)
				*pl++ = ' '; //없으면 공백으로 채워줌
			*pl++ = '|';
			for (j = 0; j < board_acr_gap; j++)
				*pl++ = ' '; //칸 간격
		}
		*pl++ = '\n';
		for (i = 0; i < board_acr; i++) { //아래쪽 경계 표시
			*pl++ = '+'; //한 글자 쓰면서 커서 옮기기
			for (j = 0; j < namelen; j++)
				*pl++ = '-';
			*pl++ = '+';
			for (j = 0; j < board_acr_gap; j++)
				*pl++ = ' '; //칸 간격
		}
		*pl++ = '\n';
		*pl = '\0'; //문자열의 마지막을 '\0'으로 바꿔줌
		puts(str);
		pl = str; //커서 초기화

		if (!k) { //화살표 표시
			len = 2;
			for (j = 0; j < (namelen - len + 1) / 2 + 1; j++) *pl++ = ' ';
			//중앙정렬을 위한 앞 공백 출력
			//앞 공백의 수와 뒤 공백의 수가 일치하지 않으면 앞에 공백을 하나 덜 출력함
			*pl++ = '/';
			*pl++ = '\\'; //역슬래시로 정상적으로 보인다고 가정함
			for (j = 0; j < (namelen - len) / 2 + 1; j++) *pl++ = ' ';
			//중앙정렬을 위한 뒤 공백 출력
			for (j = 0; j < (namelen + 2 + board_acr_gap) * (board_acr - 2) + board_acr_gap; j++)
				*pl++ = ' '; //다음 화살표 위치까지 공백 출력

			for (k = 0; k < 3; k++) { //이 이상은 str 변수가 버틸 수 없음
				//화살표 꼬리 출력 반복문
				for (j = 0; j < (namelen - len + 1) / 2 + 1; j++) *pl++ = ' ';
				//중앙정렬을 위한 앞 공백 출력
				//앞 공백의 수와 뒤 공백의 수가 일치하지 않으면 앞에 공백을 하나 덜 출력함
				*pl++ = '|'; //화살표 꼬리 부분
				*pl++ = '|';
				*pl++ = '\n'; //줄 변경
				for (j = 0; j < (namelen - len + 1) / 2 + 1; j++) *pl++ = ' ';
				//중앙정렬을 위한 앞 공백 출력
				//앞 공백의 수와 뒤 공백의 수가 일치하지 않으면 앞에 공백을 하나 덜 출력함
				*pl++ = '|'; //화살표 꼬리 부분
				*pl++ = '|';
				for (j = 0; j < (namelen - len) / 2 + 1; j++) *pl++ = ' ';
				//중앙정렬을 위한 뒤 공백 출력
				for (j = 0; j < (namelen + 2 + board_acr_gap) * (board_acr - 2) + board_acr_gap; j++)
					*pl++ = ' '; //다음 화살표 위치까지 공백 출력
			}
			k = 0; //빌려 쓴 변수 다시 원상복귀

			for (j = 0; j < (namelen - len + 1) / 2 + 1; j++) *pl++ = ' ';
			//중앙정렬을 위한 앞 공백 출력
			//앞 공백의 수와 뒤 공백의 수가 일치하지 않으면 앞에 공백을 하나 덜 출력함
			*pl++ = '\\'; //역슬래시로 정상적으로 보인다고 가정함
			*pl++ = '/';

			*pl++ = '\n';
			*pl = '\0'; //문자열의 마지막을 '\0'으로 바꿔줌
			puts(str);
			pl = str; //커서 초기화
		}
	}
#undef boardpl_cal
}