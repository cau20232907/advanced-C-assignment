#define try_message //재시도 메시지 덮어쓰기
#define trying_again "저장할 공간이 부족합니다. 다시 시도하고 있습니다..."
#define try_succeed "저장공간 문제를 해결하였습니다."
#define try_falied "문장을 저장할 공간이 없습니다. 맞는 게 거의 없는 수준이므로 점수 계산을 하지 않고 넘어갑니다."
#include "20232907 정현승 과제 3 공통 헤더.h"
#define MAX_strlen 61 //최대 문자열 수
#define rpt 5
typedef unsigned char rpt_type;
typedef unsigned char strlen_type;
#define check_range 3

void sentence_selector(char to[MAX_strlen]); //문장 설정
strlen_type check_up(const char ans[MAX_strlen], char *to); //문장 확인

void main() {
	char ans[MAX_strlen], * in = NULL; //각각 정답, 입력한 답 저장 
	clock_t pre, aft; //이전, 이후 시간 저장
	float gap, total = 0;
	strlen_type len;

	srand(time(NULL));
	for (rpt_type cnt = 0; cnt < rpt; cnt++) {
		sentence_selector(ans);
		printf("(%d/%d) 다음을 입력하세요.\n", cnt + 1, rpt);
		printf("= %s\n", ans);
		printf("= ");
		pre = clock();
		in = sgets(MAX_strlen, 10, stdin);
		aft = clock();
		if (in) { //입력이 정상적으로 되었다면 틀린 곳 및 문법 계산
			len = strlen(ans);
			len -= check_up(ans, in);
			putchar(' ');
			putchar(' '); //오류 지적과 입력한 문자가 맞지 않는 점때문에 공백 2개로 이를 맞춤
			puts(in);
			gap = len / ((aft - pre) / 60.0 / CLOCKS_PER_SEC); //분당 타수 계산
			printf("%.0f타/분\n", gap);
			total += gap; //점수 계산, 소수점 문제로 인해 5회의 합이 보이는 것과 같으리라는 보장은 없음
			freeset(&in);
		}
	}
	printf("당신의 점수는 %.0f점 입니다.", total);
}

void sentence_selector(char to[MAX_strlen]) {
	strlen_type wordlen = 0, sentlen = 0; //각각 이번 단어 길이, 공백 제외 문장 길이
	char* pos = to; //커서
	arrset(to, MAX_strlen); //기존 내용 전부 지우기
	while (1) {
		wordlen = rand() % 7 + 4; //단어 길이: 4~10자
		if (sentlen + wordlen > 50) break; //단어를 넣으면 문장 길이를 초과하게 되는 경우, 생성 중단(문장은 공백 제외 40~50자)
		for (strlen_type i = 0; i < wordlen; i++) {
			*pos++ = rand() % 26 + 'A'; //A~Z중 무작위 문자, 커서 움직임
			sentlen++; //글자 수 세기
		}
		*pos++ = ' '; //공백 삽입
	}
	*(pos - 1) = '\0'; //공백이 들어갔던 문장의 마지막을 '\0'로 바꿔줌
}

strlen_type check_up(const char ans[MAX_strlen], char *to) { //이 프로그램에서 가장 애매한 부분, 이걸 정확히 하려면 AI가 필요함
	strlen_type cnt = 0, ani = 0, check = 0; //각각 틀린 문자 수, ans의 커서, 글자가 몇 개 빠졌거나 밀린 경우 확인할 때 쓰이는 변수
	//cnt는 정확히는 입력 속도 계산시에 빼야 할 글자 수임
	unsigned int toi = 0; //to의 커서, 글자가 얼마나 들어올 지 모르므로 type 분리
	//글자가 밀릴 가능성 때문에 ans의 커서와 to의 커서를 분리함
	char flag = 0;
	while (ani < MAX_strlen && (ans[ani] || to[toi])) {
		if (ans[ani] == to[toi]) { //틀린 부분 없음
			if (to[toi] != '^') to[toi] = ' ';
		}
		else if (!ans[ani]) { //이미 답 부분은 끝난 경우
			while (to[toi]) {
				to[toi++] = '^';
				cnt++;
			}
			break;
		}
		else if (!to[toi]) { //입력이 먼저 끝난 경우
			to[toi - 1] = '^'; //to[toi]에 저장한 경우 '\0'을 지우고 저장하여, 문장의 끝이 없어질 가능성으로 인해 한 칸 앞에 저장함
			break;
		}
		else {
			flag = 0;
			for (check = 1; check < check_range * 2; check++) { //둘 중 가까운 쪽으로
				//글자 몇 개가 중간에서 빠졌는지 확인
				if (!strncmp(ans + ani + check, to + toi, check_range)) {
					to[toi] = '^';
					ani += check;
					cnt++;
					flag += 2;
					break;
				}
				//글자 몇 개가 중간에 더 들어있는지 확인
				if (!strncmp(ans + ani, to + toi + check, check_range)) {
					for (strlen_type i = 0; i < check; i++) {
						to[toi++] = '^';
					}
					to[toi] = ' '; //이를 써 주지 않으면 덮어씌워지지 않음
					cnt += check;
					flag++;
					break;
				}
			}
			//이 이외의 경우(그냥 글자 하나만 틀린 것으로 확인)
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