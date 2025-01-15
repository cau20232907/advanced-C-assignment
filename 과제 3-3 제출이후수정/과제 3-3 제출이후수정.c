#include "20232907 정현승 과제 3 공통 헤더.h"

typedef struct longint_with_size {
	unsigned int* num; //숫자 저장 위치(동적 할당 사용)
	size_t size : sizeof(size_t) - 1; //동적할당 받은 크기
	char code : 1; //숫자의 부호, 0이면 양수
} lint;
//숫자 저장 방식은 작은 digit을 앞에 저장함

lint strtolint(const char* in);
char* linttostr(const lint in);
lint lintcpy(const lint num);
lint add(const lint num1, const lint num2);
lint sub(const lint num1, const lint num2);
lint mul(const lint num1, const lint num2);
lint push(const lint num, size_t len);
char MUX_uint(unsigned int to, unsigned int controller);
unsigned char last_digit_loc(const lint num);
char lintcmp(const lint num1, const lint num2);
void freeset_lint(lint* p);

void main() {
	char* strX = NULL, * strY = NULL, * strZ = NULL;
	lint numX = { 0,0,0 }, numY = { 0,0,0 }, sum = { 0,0,0 }, subt = { 0,0,0 }, mult = { 0,0,0 };

	while (1) {
		//문자열 입력
		printf("input X: "); //X 입력
		strX = sgets(70, 10, stdin);
		printf("input Y: "); //Y 입력
		strY = sgets(70, 10, stdin);
		if (!strX || !strY) break;

		//숫자로 바꾸기
		numX = strtolint(strX);
		numY = strtolint(strY);
		if (!numX.num || !numY.num) break;
		freeset(&strX);
		freeset(&strY);

		//연산 시행
		sum = add(numX, numY);
		subt = sub(numX, numY);
		mult = mul(numX, numY);
		if (!sum.num || !subt.num || !mult.num) break;

		/*
		strX = linttostr(numX);
		strY = linttostr(numY);
		*/
		freeset_lint(&numX);
		freeset_lint(&numY);
		/*
		if (!strX || !strY) break;
		printf("X: ");
		puts(strX);
		printf("Y: ");
		puts(strY);
		freeset(&strX);
		freeset(&strY);*/

		//문자열로 바꾸기
		strX = linttostr(sum);
		strY = linttostr(subt);
		strZ = linttostr(mult);
		if (!strX || !strY || !strZ) break;
		freeset_lint(&sum);
		freeset_lint(&subt);
		freeset_lint(&mult);
		//출력
		putchar('\n');
		printf("X + Y :  ");
		puts(strX);
		printf("X - Y :  ");
		puts(strY);
		printf("X * Y :  ");
		puts(strZ);
		freeset(&strX);
		freeset(&strY);
		freeset(&strZ);
		putchar('\n');
		putchar('\n');
	}

	//동적할당 해제 확인(동적할당 실패 시 break;를 하기에 해제를 여기서 해 주어야 함, freeset 함수가 이미 해제되었을때는 처리 없이 넘긴다는 점을 이용)
	freeset(&strX);
	freeset(&strY);
	freeset_lint(&numX);
	freeset_lint(&numY);
	freeset_lint(&sum);
	freeset_lint(&subt);
	freeset_lint(&mult);
}

lint strtolint(const char* in) { //문자를 숫자로 바꾸는 함수
	lint res = { 0,0,0 };
	char* stp = NULL; //숫자로 변환을 시작하는 부분을 나타내는 포인터
	char nums[10] = { 0, }; //9자리씩 끊어서 숫자로 바꿀 때, 바꿀 숫자를 저장해 둘 공간
	lint tmp1 = { 0,0,0 }, tmp2 = { 0,0,0 }; //연산값을 임시로 저장할 변수
	lint m10 = { 0,0,0 }; //십진수 기준 9개 자릿수당 배수의 값을 저장할 변수
	char flag = 0; //동적할당 실패 여부를 확인하는 변수
	//lint와 int의 곱이 구현되어 있지 않아, int도 lint로 만들어주어야 함
	unsigned int n10 = 1000000000; //10의 9제곱
	m10.num = &n10;
	m10.size = sizeof(n10);

	tmp1.num = calloc_in(sizeof(unsigned int)); //문자를 숫자로 바꾼 후 여기에 저장할 예정
	if (!tmp1.num) return tmp1;
	tmp1.size = sizeof(unsigned int); //할당받은 크기 저장

	if (*in == '-') {
		tmp1.code = 1;
		in++;
	}
	else if (*in == '+') {
		tmp1.code = 0;
		in++;
	}

	//변환
	stp = in;
	strncpy(nums, stp, 9); //숫자 복사
	*(tmp1.num) = atoi(nums);
	res = lintcpy(tmp1);
	if (!res.num) {
		freeset_lint(&tmp1);
		return res;
	}
	stp += 9; //숫자로 바꾼 만큼 뒤로 옮김

	//작은 숫자부터 시작
	while (stp < in + strlen(in)) {
		if (stp + 9 > in + strlen(in)) { //다음 번이 마지막이고 이번은 자릿수가 모자라 기존 수에 1000000000을 곱하면 안 되는 경우
			for (flag = (stp + 9) - (in + strlen(in)); flag; flag--) {
				n10 /= 10; //자릿수를 맞추기 위해 10의 (남은 자릿수) 제곱이 n10(=m10)이 되도록 만들어 줌
			} //사용하지 않는 변수 활용(지금은 0이어야 함)
		}

		strncpy(nums, stp, 9); //숫자 복사
		*(tmp1.num) = atoi(nums);
		tmp2 = mul(m10, res);
		if (!tmp2.num) {
			flag = 1;
			break;
		}
		freeset_lint(&res);
		res = add(tmp1, tmp2);
		if (!res.num) {
			flag = 1;
			break;
		}
		freeset_lint(&tmp2);
		stp += 9; //숫자로 바꾼 만큼 뒤로 옮김
	}
	freeset_lint(&tmp1);
	freeset_lint(&tmp2);
	return res;
}

char* linttostr(const lint in) { //숫자를 문자로 바꾸는 함수
	//이것이 인터넷에서 참고한 함수, 출처: https://stackoverflow.com/questions/8023414/how-to-convert-a-128-bit-integer-to-a-decimal-ascii-string-in-c
	char* res = NULL; //결과값을 저장할 변수
	unsigned long long left = 0;
	lint num = { 0,0,0 };
	size_t pl; //연산 위치
	size_t res_len = 0; //res에 할당된 문자열 크기
	char* pos = NULL; //커서

	num = lintcpy(in); //숫자를 직접 바꿔야 하므로 별도 복사
	if (!num.num) return res;
	res_len = in.size / 2 * 5 + 1;
	res = calloc_in(res_len); //int 배열 크기의 2.5배(바이트 기준으로는 10배) + 1('\0')의 문자열 일단 선언
	if (!res) {
		freeset_lint(&num);
		return res;
	}
	pos = res + res_len - 2; //할당받은 곳의 마지막 공간 직전을 가리킴(마지막 공간은 '\0' 필요)
	while (1) {
		pl = num.size / sizeof(unsigned int);
		if (!num.num[pl - 1]) { //가장 큰 digit의 정수가 모두 비워지면
			num.size -= sizeof(unsigned int);
			pl--;
			if (!pl) break; //마지막 정수까지 비워지면 break;
		}
		if (pos < res) { //재할당 필요시(주어진 공간 전부 사용)
			pos = calloc_in(res_len + 10); //10개 공간 더 받기
			if (!pos) {
				freeset_lint(&num);
				freeset(res);
				return NULL;
			}
			res_len += 10;
			strcpy(pos + 10, res); //기존 res에서 복사
			freeset(&res); //res 할당 해제
			res = pos; //새로 할당된 위치를 res에 저장
			pos = res + 10 - 1;
		}
		left = num.num[pl - 1];
		while (pl) {
			num.num[pl - 1] = left / 10;
			pl--;
			if (!pl) break;
			left = ((left % 10) << 8 * sizeof(unsigned int)) + num.num[pl - 1];
		}
		left = left % 10;
		*pos-- = '0' + left;
	}
	freeset_lint(&num); //0이 된 num 동적할당 해제
	if (in.code) { //num이 해제되었으므로 in으로 검사해야 함, 결과값이 음수인경우 부호 붙이기
		if (pos < res) { //재할당 필요시(주어진 공간 전부 사용)
			pos = calloc_in(res_len + 1); //10개 공간 더 받기
			if (!pos) {
				freeset(res);
				return NULL;
			}
			res_len += 1;
			strcpy(pos + 1, res); //기존 res에서 복사
			freeset(&res); //res 할당 해제
			res = pos; //새로 할당된 위치를 res에 저장
		}
		*pos = '-';
	}
	else pos++;
	//이러면 pos가 가리키는 위치는 문자열 시작 위치
	if (pos != res) strcpy(res, pos); //문자열 시작 위치가 res와 일치하지 않으면 일치하도록 문자열을 옮겨줌
	if (!*res || !strcmp(res, "-")) *res = '0'; //0이면 어떤 문자도 들어가지 않는데 이를 막기 위함
	return res;
}

lint lintcpy(const lint num) { //변수 복사
	lint res = { 0,0,0 };
	size_t pl = 0;
	res.num = calloc_in(num.size);
	if (!res.num) return res;
	for (pl = 0; pl < num.size / sizeof(unsigned int); pl++)
		res.num[pl] = num.num[pl];
	res.size = num.size;
	res.code = num.code;
	return res;
}

lint add(const lint num1, const lint num2) {
	lint res = { 0,0,0 }; //return값을 저장할 공간
	unsigned long long tmp = 0; //연산값을 임시 저장할 함수
	size_t pl = 0; //연산 위치
	//unsigned int* ptmp = NULL; //realloc시 realloc_in의 return값을 임시 저장할 변수

	if (num1.code ^ num2.code) { //둘의 부호가 서로 다르면(빼기 연산에 해당)
		if (num2.code) {
			res = num2; //얕은 복사
			res.code = 0;
			return sub(num1, res);
		}
		else {
			res = num1; //얕은 복사
			res.code = 0;
			return sub(num2, res);
		}
	}

	tmp = num1.size > num2.size ? num1.size : num2.size; //둘 중 큰 크기 저장
	tmp += sizeof(unsigned int); //1개 int 자리만큼 더하기
	res.num = calloc_in(tmp);
	if (!res.num) return res;
	res.size = tmp; //할당받은 크기 저장

	//계산
	tmp = 0;
	while (pl * sizeof(unsigned int) < num1.size || pl * sizeof(unsigned int) < num2.size || tmp) { //두 숫자 다 저장된 크기를 벗어나고 tmp에 저장된 수가 0일 때까지 반복
		if (pl * sizeof(unsigned int) < num1.size) tmp += (num1.num)[pl]; //num1 더하기
		if (pl * sizeof(unsigned int) < num2.size) tmp += (num2.num)[pl]; //num2 더하기
		res.num[pl] = (unsigned int)tmp; //둘의 합을 unsigned int 형으로 저장함
		tmp >>= 8 * sizeof(unsigned int); //저장된 부분을 지우고 저장 안 된 앞 부분을 뒷자리로 뺌
		pl++;
	}

	//불필요한 저장공간 숨김처리, 이후 불필요한 저장공간이 있는지를 확인하지 않기 때문에 필요(이 프로그램에서 return값으로 string 변환 외 추가 작업을 하지는 않으므로 삭제할 필요는 없음)
	if (pl * sizeof(unsigned int) != res.size) { //필요한 공간보다 더 큰 크기가 할당되었으면
		//ptmp = realloc_in(res.num, pl * sizeof(unsigned int));
		//if (ptmp) res.num = ptmp;
		res.size = pl * sizeof(unsigned int);
	}
	res.code = num1.code;
	return res;
}

lint sub(const lint num1, const lint num2) {
	lint res = { 0,0,0 }, ltmp = { 0,0,0 }; //return값을 저장할 공간
	signed long long tmp = 0; //연산값을 임시 저장할 함수
	size_t pl = 0; //연산 위치
	char left = 0;
	const signed long long debt = (long long)1 << (8 * sizeof(unsigned int)); //unsigned int형이 저장할 수 있는 최대값 + 1
	//unsigned int* ptmp = NULL; //realloc시 realloc_in의 return값을 임시 저장할 변수

	if (num1.code ^ num2.code) {
		res = num2; //얕은 복사
		if (num2.code) res.code = 0; //+-, 음수를 빼는 경우(== 양수를 더하는 경우)
		else res.code = 1; //-+, 음수에 양수를 빼는 경우(== 음수를 빼는 경우)
		// if문은 code를 서로 변경한 것(code는 동적할당 받은 것이 아니라서 바꿔도 원본에 지장 없음)
		return add(num1, res); //이들은 더하기 연산으로 처리됨
	}
	else if (num1.code && num2.code) { //음수에서 음수를 빼는 경우
		ltmp = num1;
		res = num2;
		ltmp.code = 0;
		res.code = 0;
		return sub(res, ltmp); //양수에서 양수를 빼는 것으로 바꿔주기
	}
	else if (lintcmp(num1, num2) < 0) { //결과가 음수값이면(순서 바꿔서 빼고 부호 붙이기)
		res = sub(num2, num1);
		res.code = 1;
		return res;
	}
	//남은 경우의 수는 큰 양수에서 작은 양수를 빼는 것 뿐

	tmp = num1.size; //둘 중 큰 크기 저장
	res.num = calloc_in(tmp);
	if (!res.num) return res;
	res.size = tmp; //할당받은 크기 저장

	//계산
	tmp = 0;
	while (pl * sizeof(unsigned int) < num1.size || pl * sizeof(unsigned int) < num2.size || tmp) { //두 숫자 다 저장된 크기를 벗어나고 tmp에 저장된 수가 0일 때까지 반복
		if (pl * sizeof(unsigned int) < num1.size) tmp += (num1.num)[pl]; //num1 더하기
		if (pl * sizeof(unsigned int) < num2.size) tmp -= (num2.num)[pl]; //num2 빼기
		if (tmp < 0) { //현재 결과값이 0보다 작은 경우
			tmp += debt; //unsigned int의 최대 숫자 + 1 더해주기(이러면 양수가 됨)
			left = 1; //flag 켜기(이 더한 걸 나중에 빼 주어야 함)
		}
		res.num[pl] = (unsigned int)tmp; //둘의 합을 unsigned int 형으로 저장함
		tmp >>= 8 * sizeof(unsigned int); //저장된 부분을 지우고 저장 안 된 앞 부분을 뒷자리로 뺌
		pl++;
		if (left) {
			tmp--; //더한 만큼 빼기(더한 수는 shift 연산으로 1이 되었음)
			left = 0;
		}
	}

	//불필요한 저장공간 숨김처리, 이후 불필요한 저장공간이 있는지를 확인하지 않기 때문에 필요(이 프로그램에서 return값으로 string 변환 외 추가 작업을 하지는 않으므로 삭제할 필요는 없음)
	while (!last_digit_loc(res) && res.size) res.size -= sizeof(unsigned int);
	if (!res.size) res.size = sizeof(unsigned int); //size가 0이 되는 것 방지
	/*if (pl * sizeof(unsigned int) != res.size) { //필요한 공간보다 더 큰 크기가 할당되었으면
		ptmp = realloc_in(res.num, pl * sizeof(unsigned int));
		if (ptmp) res.num = ptmp;
		res.size = pl * sizeof(unsigned int);
		//재할당에 실패하기 어렵겠지만 만약 그런다면 할당된 크기만 바꿔 저장함, 속이는 것이지만 메모리 부족 전까지는 문제 없음
	}*/
	return res;
}

lint mul(const lint num1, const lint num2) {
	lint res = { 0,0,0 }; //return값을 저장할 공간
	unsigned long long tmp = 0; //연산값을 임시 저장할 변수
	lint add_res = { 0,0,0 }; //더한 결과를 저장할 공간
	lint push_res = { 0,0,0 }; //push한 결과를 저장할 공간 + 임시 포인터 저장공간
	size_t pl = 0; //연산 위치
	char flag = 0; //동적 할당 오류 확인
	unsigned int cont = 1; //MUX에 controller로 넣을 정수
	size_t pu_pl = 0; //push해야 하는 자릿수
	unsigned int base = 0; //0을 표기하기 위한 상수(0 위치 찍기 위함)

	res.num = &base; //이거 하나때문에 base 변수를 선언함
	res.size = sizeof(unsigned int); //크기 저장

	//계산
	tmp = 0;
	while (pl * sizeof(unsigned int) < num2.size) {
		if (MUX_uint(num2.num[pl], cont)) { //비트 하나씩 검사하고 이번 비트가 1이면(2진수의 곱셈)
			push_res = push(num1, pu_pl);
			if (!push_res.num) {
				flag = 1;
				break;
			}
			add_res = add(res, push_res); //push한 것을 add함
			if (!add_res.num) {
				flag = 1;
				break;
			}
			freeset_lint(&push_res); //free를 해 주어야 하기 때문에 add함수의 매개변수로 바로 push를 사용할 수 없었음
			if (res.num != &base) freeset_lint(&res);
			res = add_res;
		}
		pu_pl++;
		cont <<= 1;
		if (!cont) { //다른 자릿수로 넘어갈 필요 존재
			pl++;
			cont = 1;
		}
	}
	if (flag) { //동적할당 실패는 한번에 처리
		freeset_lint(&res);
		freeset_lint(&add_res);
		freeset_lint(&push_res);
	}
	res.code = num1.code ^ num2.code;
	return res;
}

lint push(const lint num, size_t len) { //<<연산, 곱셈에 활용
	lint res = { 0,0,0 }; //return값을 저장할 공간
	unsigned long long tmp = 0; //연산값을 임시 저장할 함수
	size_t pl = 0; //연산 위치
	size_t move = 0; //int 단위로 몇 번 이동해야 하는지를 저장하는 함수
	char flag = (len + last_digit_loc(num)) > (8 * sizeof(unsigned int)); //비트 단위로 밀려서 int형 하나가 더 필요할 경우인지 확인

	move = len / (8 * sizeof(unsigned int));
	len %= (8 * sizeof(unsigned int));
	tmp = num.size + (move + flag) * sizeof(unsigned int); //res.num의 길이(기존 길이+int 단위로 밀리는 길이, 여기에 비트 단위로 밀려서 int형 하나가 더 필요할 경우 이것도 셈)

	res.num = calloc_in(tmp);
	if (!res.num) return res;
	res.size = tmp; //할당받은 크기 저장


	//계산
	pl = num.size / sizeof(unsigned int); //반복문 시작부터 빼므로 1 더해줌
	tmp = 0;
	if (!flag) {
		pl--;
		tmp += num.num[pl];
		tmp <<= (8 * sizeof(unsigned int));
	}
	while (pl) {
		pl--;
		tmp += num.num[pl];
		tmp <<= len;
		res.num[pl + move + 1] = tmp >> (8 * sizeof(unsigned int)); //앞 32비트 저장
		tmp <<= (8 * sizeof(unsigned int)) - len;
	}
	tmp <<= len;
	res.num[move] = tmp >> (8 * sizeof(unsigned int)); //앞 32비트 저장
	res.code = num.code;
	return res;
}

char MUX_uint(unsigned int to, unsigned int controller) { //특정 자릿수에 비트값이 1인지 0인지 확인하는 함수
	if (to & controller) return 1;
	else return 0;
}

unsigned char last_digit_loc(const lint num) { //가장 큰 자릿수(int 마지막 공간)에 자릿수가 몇부터 저장되어 있는가 확인
	unsigned char res = 0; //return값을 저장할 공간
	unsigned int fin = num.num[num.size / sizeof(unsigned int) - 1]; //가장 큰 자릿수(int 마지막 공간) 복사
	for (res = 0; fin; res++) fin >>= 1; //하나씩 움직이면서 fin이 0이 될 때가 언제인지 확인
	return res;
}

char lintcmp(const lint num1, const lint num2) { // 크기 비교
	size_t pl = num1.size / sizeof(unsigned int);
	//부호 자체가 다르면
	if (num1.code != num2.code) {
		if (num1.code) return 4;
		return (-4);
	}
	//할당된 크기 자체가 다르면
	else if (num1.size > num2.size ^ num1.code) return 3;
	else if (num1.size < num2.size ^ num1.code) return (-3);
	//할당된 크기는 같지만 같은 크기 안에서 첫 번째로 등장하는 숫자의 위치가 다른 경우
	else if (last_digit_loc(num1) > last_digit_loc(num2) ^ num1.code) return 2;
	else if (last_digit_loc(num1) < last_digit_loc(num2) ^ num1.code) return (-2);
	//이진수 기준 자릿수는 같지만 내부의 숫자 크기가 다른 경우
	while (pl) {
		pl--;
		if (num1.num[pl] > num2.num[pl] ^ num1.code) return 1;
		else if (num1.num[pl] < num2.num[pl] ^ num1.code) return (-1);
	}
	//같은 경우
	return 0;
}

void freeset_lint(lint* p) { //lint의 freeset
	if (p->num) freeset(&(p->num));
	p->size = 0;
	p->code = 0;
}