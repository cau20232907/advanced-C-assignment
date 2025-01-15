#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define Across_Obj 5 //사다리에 참여하는 인원수
#define Down_len 80 //사다리의 위아래 길이
#define Line_gap 1 //사다리 세로줄의 간격
#define Max_line_in_a_row 6 //주변에 사다리 없이 한 줄에 연속될 수 있는 사다리 가로선의 갯수
#define Ch_Down '|' //사다리 세로줄 표시
#define Ch_Acr '-' // 사다리 가로줄 표시
#define Ch_Void ' ' // 사다리 빈 줄 표시
typedef char acr_type; // 가로줄의 변수 type
typedef unsigned char dwn_type; // 세로줄의 변수 type
typedef char str_type; // 문자열 길이의 변수 type
typedef int rpt_type; // 반복 횟수의 변수 type
//아래 4개는 메모리 사용을 효율적으로 하기 위해 변수 type을 바꿔줌(예를 들어 Across_Obj가 5이므로 for문으로 Across_Obj 숫자만큼 반복문을 돌 때 char 자료형만 사용해도 괜찮으나 이렇게 코딩하다가 갑자기 300으로 늘어나면 아래 코드를 다 바꿔야 하므로 따로 빼줌)

void normal_game(char* head[Across_Obj], char* tail[Across_Obj]); // 일반 사다리
void rept(rpt_type num, char* head[Across_Obj], char* tail[Across_Obj]); // 1000번 반복하는 코드 (num=1000)
void line_place(char acr_info[Across_Obj - 1][Down_len]); //가로줄 긋기
void print_line(char acr_info[Across_Obj - 1][Down_len]); //사다리 줄 표시
void print_res(char acr_info[Across_Obj - 1][Down_len], char* head_info[Across_Obj], char* tail_info[Across_Obj]); //전체 사다리 표시
void cal_res(char acr_info[Across_Obj - 1][Down_len], acr_type res[Across_Obj]); //결과 계산

void main() {
	char* head[Across_Obj] = { "1","2","3","4","5" };
	char* tail[Across_Obj] = { "A","B","C","D","E" };
	srand(time(NULL));
	normal_game(head, tail);
	// rept(1000, head, tail);
}

void normal_game(char* head[Across_Obj], char* tail[Across_Obj]) {
	char acr_line[Across_Obj - 1][Down_len] = { 0, };
	acr_type res[Across_Obj] = { 0, };
	srand(time(NULL));
	line_place(acr_line);
	print_res(acr_line, head, tail);
	cal_res(acr_line, res);
	for (acr_type i = 0; i < Across_Obj - 1; i++) {
		printf("%s -> %s, ", head[i], tail[res[i]]);
	}
	printf("%s -> %s\n", head[Across_Obj - 1], tail[res[Across_Obj - 1]]);
}

void rept(rpt_type num, char* head[Across_Obj], char* tail[Across_Obj]) {
	char acr_line[Across_Obj - 1][Down_len] = { 0, };
	acr_type res[Across_Obj] = { 0, };
	rpt_type res_col[Across_Obj][Across_Obj] = { 0, }; // 1000번 통계 카운터
	for (rpt_type i = 0; i < num; i++) {
		line_place(acr_line);
		cal_res(acr_line, res);
		for (acr_type j = 0; j < Across_Obj; j++) // 통계에 넣기
			res_col[j][res[j]]++;
	}
	for (acr_type i = 0; i < Across_Obj; i++) { // 통계 출력
		printf("%s: ",head[i]);
		for (acr_type j = 0; j < Across_Obj - 1; j++)
			printf("%s:%3d, ",tail[j],res_col[i][j]);
		printf("%s:%3d\n", tail[Across_Obj - 1], res_col[i][Across_Obj - 1]); // 마지막은 ',' 출력이 필요 없으니 따로 출력
	}
}

void line_place(char acr_info[Across_Obj - 1][Down_len]) {
	dwn_type cnt[Across_Obj - 1][2] = { 0, }; //각 줄의 갯수를 셈([arr][0]은 총 갯수, [arr][1]은 연속 갯수(주변에 가로선이 없는 상태에서 연속된 가로선의 갯수))
	//연속 갯수를 매번 계산할 수는 없으니 저장함
	acr_type arr = 0;
	for (dwn_type i = 0; i < Down_len; i++) { //일단은 한 줄당 하나씩만 줄을 넣어줌
		do arr = rand() % (Across_Obj - 1); //세로줄 갯수 조절
		while (cnt[arr][0] >= Down_len * 2 / Across_Obj);
		if (cnt[arr][1] >= Max_line_in_a_row) { //주변에 사다리 없이 한 줄에 연속될 수 있는 사다리 가로선의 갯수를 넘었을 경우
			if (arr >= (Across_Obj - 2) || (arr > 0 && rand() % 2))	arr--;
			else arr++;
			//사다리 위치 조정(가로 사다리를 옆 줄로 옮김)
		}
		for (acr_type j = 0; j < Across_Obj - 1; j++) {
			if (j == arr) acr_info[arr][i] = Ch_Acr; //가로줄 넣어주기
			else acr_info[j][i] = Ch_Void; //공백 넣어주기
		}
		cnt[arr][0]++;
		cnt[arr][1]++;
		//갯수 세기
		if (arr > 0) cnt[arr - 1][1] = 0;
		if (arr < (Across_Obj - 2)) cnt[arr + 1][1] = 0;
		//연속 갯수 초기화
	}
}

void print_res(char acr_info[Across_Obj - 1][Down_len], char* head_info[Across_Obj], char* tail_info[Across_Obj]) {
	for (acr_type i = 0; i < Across_Obj - 1; i++) {
		printf(head_info[i]);
		for (str_type j = strlen(head_info[i]); j < Line_gap + 1; j++)
			putchar(Ch_Void); //다음 줄의 위치까지 빈 공간 출력
	}
	puts(head_info[Across_Obj - 1]); //마지막은 빈 공간 출력이 필요 없으니 따로 출력, 개행 실시
	print_line(acr_info);
	for (acr_type i = 0; i < Across_Obj - 1; i++) {
		printf(tail_info[i]);
		for (str_type j = strlen(tail_info[i]); j < Line_gap + 1; j++)
			putchar(Ch_Void); //다음 줄의 위치까지 빈 공간 출력
	}
	puts(tail_info[Across_Obj - 1]); //마지막은 빈 공간 출력이 필요 없으니 따로 출력, 개행 실시
}

void print_line(char acr_info[Across_Obj - 1][Down_len]) {
	for (dwn_type i = 0; i < Down_len; i++) {
		putchar(Ch_Down);
		for (acr_type j = 0; j < Across_Obj - 1; j++) {
			for (str_type k = 0; k < Line_gap; k++)
				putchar(acr_info[j][i]); //가로줄 표시
			putchar(Ch_Down);
		}
		putchar('\n');
	}
}

void cal_res(char acr_info[Across_Obj - 1][Down_len], acr_type res[Across_Obj]) {
	acr_type cur = 0; //현재 줄
	for (acr_type i = 0; i < Across_Obj; i++) {
		cur = i;
		for (dwn_type j = 0; j < Down_len; j++) {
			if (cur > 0 && acr_info[cur - 1][j] == Ch_Acr) cur--;
			else if (cur < Across_Obj - 1 && acr_info[cur][j] == Ch_Acr) cur++;
			//앞에는 배열 범위 벗어나지 않게 하기 위한 조건, 뒤는 선이 그어저 있는지 확인
			//VC에서 and는 앞이 이미 False이면 뒤쪽 연산을 하지 않는다는 점을 이용
		}
		res[i] = cur; //끝까지 내려오면 결과의 위치를 저장(위치인지라 숫자임)
	}
}