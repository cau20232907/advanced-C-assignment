#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#pragma warning(disable:6031)

#define MAX_strlen 20 //문자열의 최대 길이(종류 상관 없이)

//enum day_type { sun, mon, tue, wed, thu, fri, sat };
enum month_type { JAN = 1, FEB, MAR, APR, MAY, JUN, JUL, AUG, SEP, OCT, NOV, DEC };

typedef struct datetime { //과제 2에서 썼던 그것
	unsigned int year;
	enum month_type month : 5;
	unsigned char dayn : 5;
	//enum day_type dayt : 3; //과제 2에서처럼 여기서도 사용은 하지 않을 예정
	//unsigned char hour : 5; //미사용
	//unsigned char minute : 6; //미사용
	//unsigned char second : 6; //미사용
} datetime;

struct _Loan; //구조체 원형선언

typedef struct _Book {
	char* title; //책 이름
	char* author; //저자명
	char* press; //출판사
	struct _Loan* status; //현재 대출 상황, 성능 효율을 위해 여기도 저장
	struct _Book* prev; //Linkedlist 사용
	struct _Book* next; //Linkedlist 사용
} book;

typedef struct _Person {
	char* name;
	char* phone;
	//대출 가능한 책이 여러 권이기에 대출 상황을 여기에 저장하기는 어려움
	struct _Person* prev; //Linkedlist 사용
	struct _Person* next; //Linkedlist 사용
} person;

typedef struct _Loan {
	struct _Loan* prev; //Linkedlist 사용
	struct _Loan* next; //Linkedlist 사용
	book* objbook;
	person* objper;
	datetime rantdate; //대출일
	datetime retrndate; //반납일, 값이 0인 경우 반납이 안 된 것
} rent;

void booklist(const book* p);
rent* rentbook(const book* pb, const person* pp, rent* pr);
rent* retrnbook(rent* pr);
void rentlist(const rent* p);
datetime datein();
unsigned long long datetolld(const datetime obj);
int datecmp(const datetime datet1, const datetime datet2);
char ifleapyear(const int year);

void main() {
	book bookshelf[5] = {
		{"파이썬프로그래밍","홍길동","A출판사",NULL,NULL,bookshelf + 1},
		{"새로운 자료구조" ,"강길동","C출판사",NULL,bookshelf,bookshelf + 2},
		{"열혈 C 프로그래밍","윤성우","오랜지미디어",NULL,bookshelf + 1,bookshelf + 3},
		{"열혈 C++ 프로그래밍",bookshelf[2].author,bookshelf[2].press,NULL, bookshelf + 2,bookshelf + 4},
		{"한계테스트용가상도서","한계테스트용가상저자","한계테스트용가상회사",NULL,bookshelf + 3,NULL}
	};
	person members[5] = {
		{"이순신","010-4444-5555",NULL,members + 1},
		{"도라에몽","010-3333-8888",members,members + 2},
		{"나상실","010-3434-3434",members + 1,members + 3},
		{"김승태 교수님","010-4757-0040",members + 2,members + 4},
		{"한계테스트용가상이름","한계테스트용가상번호",members + 3,NULL}
	};
	rent* rentptr = NULL; //대출기록은 처음에 없는 게 맞음
	char cmd = 1;
	while (1) {
		if (cmd) {
			puts("원하시는 기능을 입력해 주세요.");
			puts("1. 책 목록 표시하기");
			puts("2. 대출 등록하기");
			puts("3. 반납 하기");
			puts("4. 대출 목록 표시하기");
		}
		else cmd = 1;
		switch (getchar()) {
		case '\n':
		case '\t':
		case ' ':
			cmd = 0; //앞 기능 여러 번 출력 방지
			break;
		case '1':
			booklist(bookshelf);
			putchar('\n');
			break;
		case '2':
			while (getchar() != '\n'); //줄변경 문자가 나올 때까지 계속 입력받아 버리기
			rentptr = rentbook(bookshelf, members, rentptr); //rentptr이 NULL이 되지 않도록 return값 저장
			putchar('\n');
			break;
		case '3':
			while (getchar() != '\n'); //줄변경 문자가 나올 때까지 계속 입력받아 버리기
			retrnbook(rentptr); //return값은 처리결과이나 NULL이 나올 수도 있어 사용이 어려움
			putchar('\n');
			break;
		case '4':
			rentlist(rentptr);
			putchar('\n');
			break;
		default:
			puts("잘못된 입력입니다. 다시 입력해 주시기 바랍니다.");
		}
	}
	//return이 없으므로 free도 없음
}

void booklist(const book* p) {
	char res[MAX_strlen * 3 + 11] = { 0, }; //한꺼번에 출력
	if (!p) {
		puts("아직 책이 없습니다.");
		return;
	}
	while (p->prev) p = p->prev; //맨 앞으로 이동
	while (p) {
		sprintf(res,"%-*s %-*s %-*s ", MAX_strlen, p->title, MAX_strlen, p->author, MAX_strlen, p->press);
		if (p->status) strcat(res, "대출중"); //대출중이면 대출 정보가 저장되어 있을 것이므로 이를 표기
		else strcat(res, "보유");
		puts(res);
		p = p->next;
	}
}

rent* rentbook(const book* pb, const person* pp, rent* pr) {
	rent* res = NULL;
	char name[MAX_strlen + 2] = { 1, }; //name[0]은 임시 flag로 활용
	if (!pb) {
		puts("아직 책이 없습니다.");
		return;
	}
	if (!pp) {
		puts("아직 회원이 없습니다.");
		return;
	}

	while (pb->prev) pb = pb->prev; //맨 앞으로 이동
	while (pb) { //대출 가능한 책이 있는지 확인
		if (!pb->status) {
			*name = 0;
			break;
		}
		if (pb->next) pb = pb->next;
		else break;
	}
	if (*name) {
		puts("대출 가능한 책이 없습니다.");
		return pr; //main함수에서 오류 발생을 막기 위한 조치
	}
	
	res = calloc(1, sizeof(rent));
	if (!res) {
		puts("대출 등록에 실패했습니다.");
		return pr;
	}

	while (!res->objper) {
		printf("대출할 사람의 이름을 입력해 주세요:");
		fgets(name, sizeof(name) / sizeof(char), stdin);
		if (name[strlen(name) - 1] != '\n') //if문과 while문이 겹친다는 점을 마지막에 발견했는데 밑의 else 수정하기 귀찮아서 놔둔 if... 근데 state를 저장하기 위한 별도의 변수를 쓰는 것보다 이게 더 효과적일 지는 두고 봐야 할 일
			while (name[strlen(name) - 1] != '\n') //잘못된 입력의 경우(회원의 이름의 길이는 MAX_strlen을 초과할 수 없는데 이를 초과한 입력이 들어온 경우)
				fgets(name, sizeof(name) / sizeof(char), stdin); //일단 잘려서 들어오지 못한 입력 받아 버리기
		else {
			//회원 찾기
			while (pp->prev) pp = pp->prev; //맨 앞으로 이동
			name[strlen(name) - 1] = '\0'; //마지막 개행 문자 지워주기
			while (pp) {
				if (!strcmp(pp->name, name)) {
					res->objper = pp;
					break;
				}
				if (pp->next) pp = pp->next;
				else break; //포인터 값을 따로 저장해두지 않기 때문에 필요
			}
		}
		if (!res->objper) puts("잘못된 입력입니다.");
	}

	while (!res->objbook) {
		printf("대출할 책의 이름을 입력해 주세요:");
		fgets(name, sizeof(name) / sizeof(char), stdin);
		if (name[strlen(name) - 1] != '\n') //잘못된 입력의 경우(책 이름의 길이도 MAX_strlen을 초과할 수 없는데 이를 초과한 입력이 들어온 경우)
			while (name[strlen(name) - 1] != '\n') //일단 잘려서 들어오지 못한 입력 받아 버리기
				fgets(name, sizeof(name) / sizeof(char), stdin);
		else {
			//책 찾기
			while (pb->prev) pb = pb->prev; //맨 앞으로 이동
			name[strlen(name) - 1] = '\0'; //마지막 개행 문자 지워주기
			while (pb) {
				if (!strcmp(pb->title, name) && !pb->status) {
					//이름이 맞고 현재 대출 기록이 없다면
					res->objbook = pb;
					break;
				}
				if (pb->next) pb = pb->next;
				else break;
			}
		}
		if (!res->objbook) puts("잘못된 입력입니다.");
	}

	puts("대출 날짜를 입력해 주세요.");
	res->rantdate = datein();

	res->objbook->status = res; //책에 대출 기록 등록
	puts("대출처리가 완료되었습니다. 7일 이내에 반납해주시기 바랍니다.");

	//Linkedlist 연결
	if(!pr) return res; //연결 거부시 작업을 진행하지 않음
	while (pr->next) pr = pr->next; //맨 뒤로 이동
	pr->next = res;
	res->prev = pr;
	return res;
}

rent* retrnbook(rent* pr) {
	rent* res = NULL;
	char name[MAX_strlen + 2] = { 0, };

	if (!pr) {
		puts("아직 대여 기록이 없습니다.");
		return NULL;
	}

	while (pr->next) pr = pr->next; //맨 뒤로 이동
	while (pr) { //대출중인 책이 있는지 확인
		if (!pr->retrndate.dayn) break; //대출중이면 대출 정보가 저장되어 있을 것
		pr = pr->prev;
	}
	if (!pr) {
		puts("현재 대출중인 책이 없습니다.");
		return NULL;
	}

	while (!res) {
		printf("반납할 책의 이름을 입력해 주세요:");
		fgets(name, sizeof(name) / sizeof(char), stdin);
		if (name[strlen(name) - 1] != '\n') //잘못된 입력의 경우(책 이름의 길이도 MAX_strlen을 초과할 수 없는데 이를 초과한 입력이 들어온 경우)
			while (name[strlen(name) - 1] != '\n') //일단 잘려서 들어오지 못한 입력 받아 버리기
				fgets(name, sizeof(name) / sizeof(char), stdin);
		else {
			//회원 찾기
			while (pr->next) pr = pr->next; //맨 뒤로 이동
			name[strlen(name) - 1] = '\0'; //마지막 개행 문자 지워주기
			while (pr) { //대출기록은 최근에 있을 것이므로 뒤에서부터 찾음
				if (!strcmp(pr->objbook->title, name) && !((pr->retrndate).dayn)) {
					//반납기록이 있어도 기록을 가져오지 않음
					res = pr; //대출기록 가져오기
					break;
				}
				if (pr->prev) pr = pr->prev;
				else break;
			}
		}
		if (!res) puts("잘못된 입력입니다.");
	}

	puts("반납 날짜를 입력해 주세요.");
	res->retrndate = datein();
	while (datecmp(res->rantdate, res->retrndate) > 0) {
		puts("반납 날짜가 대출 날짜보다 빠릅니다.");
		puts("반납 날짜를 입력해 주세요.");
		res->retrndate = datein();
	}

	res->objbook->status = NULL; //책에서의 대출상황 초기화
	puts("반납처리가 완료되었습니다.");

	return res;
}

void rentlist(const rent* p) {
	char res[MAX_strlen * 3 + 35] = { 0, }; //한꺼번에 출력
	if (!p) {
		puts("아직 대여 기록이 없습니다.");
		return;
	}
	while (p->prev) p = p->prev; //맨 앞으로 이동
	while (p) {
		sprintf(res, "%-*s %s(%s)", MAX_strlen, p->objbook->title, p->objper->name, p->objper->phone);
		sprintf(res, "%-*s", (MAX_strlen * 3 + 4), res); //공백 채우기... 가능한가?
		sprintf(res, "%s %lld", res, datetolld(p->rantdate));

		if (p->retrndate.dayn) sprintf(res, "%s %lld", res, datetolld(p->retrndate));
		else strcat(res," (대출중)"); //대출중이면 반납일 정보가 없다는 점을 이용
		puts(res);
		p = p->next;
	}
}

unsigned long long datetolld(const datetime obj) {
	unsigned long long res = 0;
	res += obj.year * 10000LL;
	res += obj.month * 100;
	res += obj.dayn * 1;
	return res;
}

datetime datein() { //날짜 입력
	datetime res = { 0, };
	int tmp = 0; //정수 입력을 임시로 받을 변수
	printf("년도 입력: ");
	scanf("%d", &tmp);
	res.year = tmp;
	tmp = 0;
	while (!tmp)
	{
		printf("월 입력: ");
		scanf("%d", &tmp);
		if (tmp <= 0 || tmp > 12) {
			puts("올바르지 않은 월 입력입니다.");
			tmp = 0;
		}
	}
	res.month = tmp;
	tmp = 0;
	while (!tmp)
	{
		printf("일 입력: ");
		scanf("%d", &tmp);
		if (tmp <= 0 || tmp > 31 ||
			(tmp > 30 && (res.month == APR || res.month == JUN || res.month == SEP || res.month == NOV)) ||
			(tmp > 29 && res.month == FEB) || (tmp > 28 && res.month == FEB && !ifleapyear(res.year))) {
			puts("올바르지 않은 일 입력입니다.");
			tmp = 0;
		}
	}
	res.dayn = tmp;
	return res;
}

int datecmp(const datetime datet1, const datetime datet2) { //시간 비교
	if (datet1.year != datet2.year) return datet1.year - datet2.year;
	if (datet1.month != datet2.month) return datet1.month - datet2.month;
	if (datet1.dayn != datet2.dayn) return datet1.dayn - datet2.dayn;
	return 0;
}

char ifleapyear(const int year) { //과제 2에서 썼던 함수를 그대로 가져옴
	if (year % 4) return 0; //false, 보통 이 경우가 많을 테니 먼저 빼줌
	else if (!(year % 100) && year % 400) return 0; //false, 400으로 나누어 떨어지지 않으나 100으로 나누어 떨어지는 경우
	else return 1; //true, 그 외
}