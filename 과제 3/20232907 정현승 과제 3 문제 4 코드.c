#include "20232907 정현승 과제 3 공통 헤더.h"
#define MAX_num_digit 4

void main() {
	char* in = NULL, * pos = NULL, * stp = NULL; //각각 입력받은 문장, 작업이 진행되는 위치(커서), 숫자가 시작되는 위치
	char nums[MAX_num_digit + 1] = { 0, }; //숫자로 변경할 문자 형태의 숫자 저장, 10000 이하의 정수이므로 길이가 5인 배열 사용
	char code = '+'; //더하기를 해야 하는지, 빼기를 해야 하는지 숫자 앞에 있는 기호를 저장하는 함수, 기본값은 0에서 더하기
	int res = 0; //결과를 저장할 변수(입력된 숫자는 10000 이하지만 그 결과는 10000을 넘을 수도 있기에 int형 사용)

	in = sgets(10, 10, stdin);

	//계산
	for (pos = in, stp = in; code; pos++) {
		if (!isdigit(*pos)) { //숫자가 아닌 문자가 등장하면
			if (pos != in && pos != stp) {
				//첫 글자가 연산자거나 연산자 2개가 들어온 경우는 연산을 하지 않음(전자는 첫 번째 수가 음수인 경우, 후자는 음수를 더하거나 빼는 경우에 해당될 수 있음)

				//num 동적할당 필요없음(배열로 선언함)
				strncpy(nums, stp, pos - stp); //숫자가 시작하는 부분부터 숫자 자릿수만큼 nums에 복사
				nums[pos - stp] = '\0'; //숫자가 끝난 다음 문자를 '\0' 으로 바꿔줌
				switch (code) {
				case '+': //더하기라면
					res += atoi(nums);
					break;
				case '-': //빼기라면
				case '_': //빼기 기호를 잘못 누른 경우
					res -= atoi(nums);
					break;
					/*이하는 과제 요구사항은 아님
					//연산 순서도 사칙연산 법칙을 지키는 것이 아니라 옛날 기계식 계산기(공학용 계산기가 아닌 기본적인 기능만 들어있는 계산기)처럼 무조건 순서대로 진행하는 방식임
					case '*': //곱하기라면
						res *= atoi(nums);
						break;
					case '/': //나누기라면
						res /= atoi(nums);
						break;
					case '%':
						res %= atoi(nums);
						break;
					default:
						break;*/
				}
				code = *pos; //현재 연산자 저장(다음 연산에 사용), 입력된 문장이 끝나면 for문 조건에 따라 탈출
			}
			else if (code == '-' && *pos == '-') code = '+'; //음수를 빼는 경우 합연산을 해야 하기 때문
			else if (*pos != '+') code = *pos; //연산자 저장(다음 연산에 사용)
			//- 다음에 +가 오면 양수를 뺀 것므로 그대로 빼기 연산을 해야 하기 때문에 이 경우면 저장된 연산자를 바꾸지 않음
			stp = pos + 1; //다음 숫자의 시작 위치 지정(할당된 문장 범위를 넘어가더라도 그 위치를 읽지는 않으니 상관 없음)
		}
	}
	freeset(&in);
	printf("=%d", res); //결과 출력
}