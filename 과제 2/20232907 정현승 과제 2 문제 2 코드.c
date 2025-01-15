#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#pragma warning(disable:4996)

#define _CRT_SECURE_NO_WARNINGS
#define Size 5 //보드 한 변의 길이
#define Players 4  //플레이어 수
#define Line_To_Win 5 //승리하기 위해 완성해야하는 줄의 수
#define Init_Gap 1 //출력시 보드 내 숫자간의 좌우 간격
#define Board_Gap 2 //출력시 보드 간 간격
#define Max_Across_Board 3 //출력시 좌우로 최대로 출력할 수 있는 보드의 수, 2인 플레이에서는 미사용
#define Num_Digit 2 //보드판 안에 들어가는 숫자의 최대 자릿수
#define Weight(num) ((num)*(num)*(num)*4) //우선도 계산(한 줄에 num만큼의 수가 열려 있으면 같은 줄, 열리지 않은 줄에 더할 우선도)
#define Num_of_Bingo "빙고의 수: " //빙고판 출력 시 보드 아래에 들어가는 문장, 그냥 많이 쓰일 것 같아 빼놓은 것일 뿐 바꾸면 안됨
typedef signed int BoardType; //보드 내 숫자의 자료형, 불렸는지 여부를 부호로 판단하기에 반드시 signed로 써야 함

char Bingo(BoardType(*board)[Size][Size], const int players, const char* boardname[], const char* how_Plck, const BoardType* lines);
BoardType** DrawBoard(BoardType board[Size][Size], char* boardname);
BoardType** RandomBoard(BoardType board[Size][Size]);
char boardValidationCheck(const BoardType board[Size][Size]);
void printBoard(const BoardType(*board)[Size][Size], const int players, const char* boardname[], const BoardType* lines, const int opened_Player);
void call_erase(BoardType(*board)[Size][Size], const int players, const BoardType num);
BoardType* check_lines(BoardType(*board)[Size][Size], const int players, BoardType* res);
BoardType check_best(BoardType board[Size][Size]);

void main() {
	BoardType board[Players][Size][Size] = { 0, };
	BoardType lines[Players] = { 0, };
	char* boardname[Players] = { "플레이어", "COM1","COM2","COM3"};
	//보드 이름, 반드시 배열의 모든 요소에 문자열을 넣어야 함(문자열이 들어가지 않은 요소가 있다면 출력 중 에러 발생)
	//또한 모든 보드 이름의 길이는 (Size * (Num_Digit + 2 + Init_Gap)를 초과할 수 없음(보드 출력 시 문제 발생)
	char ifrandom[Players] = { 0,3,3,3 }; //플레이어가 숫자를 부르는 방법
	srand(time(NULL));
	RandomBoard(board[0]); //플레이어 보드 입력
	RandomBoard(board[1]); //랜덤 보드 설정
	RandomBoard(board[2]); //랜덤 보드 설정
	RandomBoard(board[3]); //랜덤 보드 설정

	while (!Bingo(board, Players, boardname, ifrandom, lines)); //승자가 나올 때까지 Bingo함수 반복(1회 반복당 각자 1개씩 숫자를 부름)
	//모든 플레이어가 숫자를 하나씩 부른 후 무언가를 하고 싶다면 while문 중괄호 열고 쓰면 됨
	printBoard(board, Players, boardname, lines, 0);
	puts("승자가 결정되었습니다.");
	for (int j = 0; j < Players; j++)
		if (lines[j] >= Line_To_Win)
			printf(" %s", boardname[j]);
	puts("의 우승입니다.");
}

char Bingo(BoardType(*board)[Size][Size], const int players, const char* boardname[], const char* how_Plck, const BoardType* lines) {
	BoardType num = 0;
	for (int i = 0; i < players; i++) {
		printf("%s의 차례입니다.\n", boardname[i]);
		switch (how_Plck[i]){
		case 0: //직접 숫자 지우기
			printBoard(board, players, boardname, lines, i + 1);
			printf("지울 숫자를 입력해 주십시오. 이미 입력된 숫자도 입력이 가능합니다. ");
			scanf("%d", &num); //숫자 입력
			printf("%d를 입력했습니다.\n\n", num);
			break;
		case 1: //완전 랜덤 선택(이미 불렀던 숫자도 가능)
			num = rand() % (Size * Size) + 1;
			printf("%s이(가) 무작위로 숫자를 선택합니다. 고른 숫자는 %d입니다.\n\n", boardname[i], num);
			break;
			//이하는 몇가지 규칙을 적용한 랜덤 선택
		case 2: //단순히 중복을 피하는 랜덤 선택
			do {
				num = rand() % (Size * Size) + 1;
				for (BoardType j = 0; j < Size * Size; j++) {
					if (((BoardType*)(board[i]))[j] == num * (-1)) {
						num = 0;
						break;
					}
				}
			} while (!num);
			printf("%s이(가) 무작위로 숫자를 선택합니다. 고른 숫자는 %d입니다.\n\n", boardname[i], num);
			break;
		case 3:
			num = check_best(board[i]);
			printf("%s이(가) 숫자를 선택합니다. 고른 숫자는 %d입니다.\n\n", boardname[i], num);
			break;
		}
		call_erase(board, players, num); //숫자 지우기
		check_lines(board, players, lines); //줄 수 확인
		for (int j = 0; j < players; j++)
			if (lines[j] >= Line_To_Win)
				return 1;
		//지정된 줄 이상을 클리어했다면 게임 종료
		if (!how_Plck[i]) { //사람이 직접 숫자를 입력한 경우 숫자를 지운 후의 보드 보여 주기
			printBoard(board, Players, boardname, lines, i + 1);
			system("pause"); //보드 확인할 시간 주기
			system("cls"); //매 턴 진행시 화면 클리어(무작위로 숫자를 골랐으면 해당 없음)
		}
	}
	return 0;
}

BoardType** DrawBoard(BoardType board[Size][Size], char* boardname) {
	BoardType i = 1, j, k;

	printf("%s의 보드를 입력합니다.\n", boardname);
	puts("1번부터 보드의 어느 행 어느 열에 저장할지를 입력합니다. 음수나 보드의 길이를 초과하는 수를 입력할 경우, 정상적으로 저장되지 않을 수 있으니 주의해 주시기 바랍니다.");
	//예외상황 중 일부의 책임을 사용자에게 떠넘기기 위한 경고문구
	// for (BoardType(*p)[2] = sketch; p < sketch + sizeof(sketch) / (sizeof(BoardType) * 2); p++) 
	while (i <= Size * Size) { //반복 횟수가 Size*Size(보드에 들어가는 최대 숫자)가 되도록 함
		printf("%d번 숫자를 몇 번째 행 몇 번째 열에 저장하시겠습니까: ", i);
		scanf("%d%d", &j, &k); //1번부터 어디에 넣을지 저장, 바로 저장하지 않고 값 확인

		//자료형이 허용하여 프로그램 내에서 잡을 수 있는 예외상황
		if (j > Size || j <= 0 || k > Size || k <= 0)
			puts("잘못된 입력입니다. 위치를 다시 입력해 주시기 바랍니다.");
		else if (board[j - 1][k - 1])
			puts("이미 그 자리에 다른 숫자가 있습니다. 위치를 다시 입력해 주시기 바랍니다.");
		else {
			//값 넣어주기
			board[j - 1][k - 1] = i;
			i++; //예외상황인 경우 i가 증가하지 않음
			system("cls"); //입력받은 게 계속 쌓이면 화면이 더러워지니 클리어
			printBoard(board, 1, &boardname, NULL, 0); //입력이 쉽도록 현재 보드 상황 출력
		}
	}

	/*문제 확인용 코드
	if (!boardValidationCheck(board)) {
		puts("숫자 입력을 완료하였습니다.");
		system("pause"); //사람이 보드를 확인할 수 있도록 잠시 대기
		system("cls"); //다음 함수 실행 시를 대비해 화면 클리어
		return board; //문제가 없으면 보드 주소를 return
	}
	else {
		puts("숫자를 다시 입력해 주시기 바랍니다.");
		return DrawBoard(board, boardname); //문제 발생시 처음부터 다시 보드 입력
	}
	*/
}

BoardType** RandomBoard(BoardType board[Size][Size]) {
	BoardType i, j, randint, stand, prev;
	for (i = 0; i < Size * Size; i++) {
		randint = rand() % (Size * Size - i); //0부터 (최대 숫자-이미 뽑은 숫자의 수-1) 사이의 무작위 값 뽑기
		randint++; //보드에 숫자는 1부터 들어가므로 이를 위한 처리
		//이렇게 하면 랜덤값을 다시 뽑는 일은 없음

		prev = 0;
		stand = 0;
		while (stand != randint) {
			stand = randint;
			for (j = 0; j < i; j++) //찾은 그 수보다 작은 숫자의 수만큼 더함(이게 방금 우리가 뽑은 수이기 때문)
				if (((BoardType*)board)[j] > prev &&((BoardType*)board)[j] <= stand) randint++;
			//앞에서는 조건에 맞지 않아 넘어갔지만, 숫자를 더해 보니 찾은 수보다 작은 수가 된 경우도 계산을 해야 하니, 더한 숫자가 없을때까지 이를 반복함
			prev = stand;
		}
		((BoardType*)board)[i] = randint; //값 넣어주기
	}

	/*문제 확인용 코드
	if (!boardValidationCheck(board))
		return board; //문제가 없으면 보드 주소를 return(랜덤 배정이므로 별도의 출력 없음)
	else {
		puts("보드를 다시 설정합니다.");
		return RandomBoard(board); //문제 발생시 처음부터 다시 보드 설정
	}
	*/
}

char boardValidationCheck(const BoardType board[Size][Size]) {
	//코드 짤 때 보드에 숫자가 제대로 입력되었는지 확인하는 코드
	int i, j, check = 0;
	char flag = 0, code = 0;
	char* boardname[1] = { "문제발생 보드" };
	for (i = 0; i < Size; i++) {
		for (j = 0; j < Size; j++) {
			if (!board[i][j] || board[i][j] > Size * Size) {
				//숫자가 없거나 최대 숫자를 초과하는 경우를 거름
				flag = 1;
				break;
			}
			else
				check += board[i][j];
			//검산은 보드 내 적힌 모든 수의 합이 1부터 최대 숫자까지의 합과 같은지 확인하는 방향으로 진행
		}
		if (flag) break; //이미 flag가 켜졌으면 빠르게 break
	}
	check -= (Size * Size) * ((Size * Size) + 1) / 2; //1부터 n까지의 합: n*(n+1)/2
	//문제가 없다면 check도 0이 되어야 함


	if (flag) code = 1;
	else if (check) code = 2;
	else { //문제 없음이 확인되면
		return code; //문제가 없으면 문제 없다는 코드 0을 return
	}

	puts("문제가 발생하였습니다.");
	switch (code) {
	case 1:
		printf("올바르지 않은 숫자가 들어 있는 칸이 있습니다. 최초 발견: %d행 %d열\n", i + 1, j + 1);
		break;
	case 2:
		printf("숫자의 합이 올바르지 않습니다. %d가 모자랍니다.\n", check * (-1));
		break;
	}
	printBoard(board, 1, &boardname, NULL, 0); //문제 확인이 쉽도록 현재 보드 상황 출력
	return code;
}

void printBoard(const BoardType(*board)[Size][Size], const int players, const char* boardname[], const BoardType* lines, const int opened_Player) {
	//lines는 함수 호출로 구할 수 있지만 계산을 2번 하지 않기 위해 받아서 사용
	for (int i = 0; i < players; i += Max_Across_Board) { //보드가 많아 가로로 모든 보드를 출력 못하면 Max_Across_Board만큼 끊어서 아랫줄에 출력되도록 하는 반복문
		for (int k = i, j; k < (players > i + Max_Across_Board ? i + Max_Across_Board : players); k++) { //우선 보드의 이름 출력
			j = strlen(boardname[k]); //중앙정렬을 위해 우선 이름의 길이 받기, 이때 이름의 길이는 보드의 가로 길이를 넘어가지 말아야 함(다른 데서 처리 필요)
			for (BoardType l = 0; l < (Size * (Num_Digit + 2 + Init_Gap) - j) / 2; l++) putchar(' '); //중앙정렬을 위해 공백 채우기
			printf("%s", boardname[k]);
			for (BoardType l = 0; l < (Size * (Num_Digit + 2 + Init_Gap) - j) / 2; l++) putchar(' '); //중앙정렬을 위해 공백 채우기
			if ((Size * (Num_Digit + 2 + Init_Gap) - j) % 2) putchar(' '); //이름 외 간격의 길이가 홀수면 오른쪽에 공백 하나 더 넣기
			for (int m = 0; m < Board_Gap; m++) putchar(' '); //보드를 공백으로 구분(좌우간격)
		}
		putchar('\n');
		for (BoardType j = 0; j < Size; j++) { //일단 세로부터 설정(아래 줄을 출력한 뒤 윗줄에 출력할 수는 없으므로)
			for (int k = i; k < (players > i + Max_Across_Board ? i + Max_Across_Board : players); k++) { //가로줄(Max_Across_Board개 이내의 board간)
				for (BoardType l = 0; l < Size; l++) { //가로줄(board 내 숫자)
					if (board[k][j][l] < 0)
						printf("(%*d)", Num_Digit, board[k][j][l] * (-1)); //불린 숫자 출력(불린 숫자 구분은 양수, 음수로 진행)
					else{
						if (!opened_Player || opened_Player - 1 == k) //보드 공개 대상인 경우(opened_Player==0이면 모두 보드 공개)
							printf(" %*d ", Num_Digit, board[k][j][l]); //숫자 출력, 자릿수는 Num_Digit(예를 들어 Num_Digit==2면 %2d가, Num_Digit==3이면 %3d가 됨)
						else {
							for (int m = 1; m < Num_Digit + 2; m++) putchar(' '); //공백으로 칸 맞추기
							putchar('-'); //'-'출력(숫자 가림)
						}
					}
					for (int m = 0; m < Init_Gap; m++) putchar(' '); //숫자를 공백으로 구분
				}
				for (int m = 0; m < Board_Gap; m++) putchar(' '); //보드를 공백으로 구분(좌우간격)
			}
			putchar('\n'); //보드의 다음 줄 출력을 위해 줄변경
		}
		if (lines != NULL)
			for (int k = i, j; k < (players > i + Max_Across_Board ? i + Max_Across_Board : players); k++) { //우선 보드의 이름 출력
				j = strlen(Num_of_Bingo) + Num_Digit; //중앙정렬을 위해 우선 빙고 줄 표시 길이 받기
				for (BoardType l = 0; l < (Size * (Num_Digit + 2 + Init_Gap) - j) / 2; l++) putchar(' '); //중앙정렬을 위해 공백 채우기
				printf("%s%*d", Num_of_Bingo, Num_Digit, lines[k]);
				for (BoardType l = 0; l < (Size * (Num_Digit + 2 + Init_Gap) - j) / 2; l++) putchar(' '); //중앙정렬을 위해 공백 채우기
				if ((Size * (Num_Digit + 2 + Init_Gap) - j) % 2) putchar(' '); //빙고 줄 표시 외 간격의 길이가 홀수면 오른쪽에 공백 하나 더 넣기
				for (int m = 0; m < Board_Gap; m++) putchar(' '); //보드를 공백으로 구분(좌우간격)
			}
		for (int m = 0; m < Board_Gap; m++) putchar('\n'); //보드를 공백으로 구분(상하간격)
	}
}

void call_erase(BoardType(*board)[Size][Size], const int players, const BoardType num) {
	for (int i = 0; i < players; i++)
		for (int j = 0; j < Size; j++)
			for (int k = 0; k < Size; k++)
				if (board[i][j][k] == num)
					board[i][j][k] *= (-1);
	//board내 수 중 num과 일치하는 수가 있으면 음수로 바꿈(불린 수로 처리)
	//반대로 취소하는 것도 가능은 함
	//중복된 숫자를 허용하는 게임을 할 때에도 처리가 가능하도록(다른 함수는 바꾸어도 이건 바꿀 필요가 없도록), 이미 그 숫자를 보았다고 break하지는 않음
}

BoardType* check_lines(BoardType(*board)[Size][Size], const int players, BoardType* res) {
	for (int i = 0, flag = 1; i < players; i++) {
		res[i] = 0;
		//가로줄
		for (BoardType j = 0; j < Size; j++) {
			flag = 1;
			for (BoardType k = 0; k < Size; k++) {
				if (board[i][j][k] >= 0) {
					flag = 0;
					break;
				}
			}
			if (flag) res[i]++;
		}

		//세로줄
		for (BoardType j = 0; j < Size; j++) {
			flag = 1;
			for (BoardType k = 0; k < Size; k++) {
				if (board[i][k][j] >= 0) {
					flag = 0;
					break;
				}
			}
			if (flag) res[i]++;
		}

		//왼쪽 위~오른쪽 아래 대각선
		//이들은 x, y 좌표 값이 같다는 특징이 있음(이를 이용함)
		flag = 1;
		for (BoardType j = 0; j < Size; j++) {
			if (board[i][j][j] >= 0) {
				flag = 0;
				break;
			}
		}
		if (flag) res[i]++;

		//왼쪽 아래~오른쪽 위 대각선
		//이들은 x, y 좌표 값의 합이 (한쪽 변의 길이 + 1)로 항상 같다는 특징이 있음
		flag = 1;
		for (BoardType j = 0; j < Size; j++) {
			if (board[i][j][Size - j - 1] >= 0) {
				flag = 0;
				break;
			}
		}
		if (flag) res[i]++;
	}
	return res;
}

BoardType check_best(BoardType board[Size][Size]) {
	int res[Size][Size] = { 0, };
	int cnt = 0, max = 0;
	
	for (BoardType j = 0; j < Size; j++)
		for (BoardType k = 0; k < Size; k++) {
			if (board[j][k] > 0) res[j][k] = 2;
			else res[j][k] = 0;
		}
	//모든 칸은 가로, 세로 1줄씩 최소 2개 줄의 완성에 영향을 주므로 기본 우선도를 2로 설정함
	//단 이미 불린 숫자면 우선도 삭제(0)

	//대각선 우선도 설정
	for (BoardType j = 0; j < Size; j++)
		if (board[j][j] > 0)
			res[j][j]++;
	for (BoardType j = 0; j < Size; j++)
		if (board[j][Size - j - 1] > 0)
			res[j][Size - j - 1]++;

	//가로줄, 세로줄에서 지워진 숫자의 수를 세서 우선도 설정
	for (BoardType j = 0; j < Size; j++) {
		cnt = 0;
		for (BoardType k = 0; k < Size; k++)
			if (board[j][k] < 0) cnt++; //지워진 수 계산
		for (BoardType k = 0; k < Size; k++)
			if (board[j][k] > 0) res[j][k] += Weight(cnt); //우선도 합하기

	}
	for (BoardType j = 0; j < Size; j++) {
		cnt = 0;
		for (BoardType k = 0; k < Size; k++)
			if (board[k][j] < 0) cnt++; //지워진 수 계산
		for (BoardType k = 0; k < Size; k++)
			if (board[k][j] > 0) res[k][j] += Weight(cnt); //우선도 합하기

	}

	//대각선
	cnt = 0;
	for (BoardType j = 0; j < Size; j++)
		if (board[j][j] < 0) cnt++; //지워진 수 계산
	for (BoardType j = 0; j < Size; j++)
		if (board[j][j] > 0) res[j][j] += Weight(cnt); //우선도 합하기

	cnt = 0;
	for (BoardType j = 0; j < Size; j++)
		if (board[j][Size - j - 1] < 0) cnt++; //지워진 수 계산
	for (BoardType j = 0; j < Size; j++)
		if (board[j][Size - j - 1] > 0) res[j][Size - j - 1] += Weight(cnt); //우선도 합하기

	//우선도는 관점에 따라 바꿀 수 있음
	//우선도를 구하는 가장 좋은 방법은 AI를 이용하는 것이나, 그렇게까지 할 것은 아니므로...

	//최선의 수 계산
	//max: 최대 우선도, cnt: 최대 우선도의 갯수(우선도 동점 나오면 그 중에서 랜덤 선택(어차피 찍기이므로))
	for (BoardType j = 0; j < Size * Size; j++) {
		if (((int*)res)[j] > max) {
			max = ((int*)res)[j];
			cnt = 1;
		}
		else if (((int*)res)[j] == max)
			cnt++;
	}
	cnt = rand() % cnt; //동점 중에서 랜덤 선택을 했으면 이제 cnt가 필요 없으므로, 랜덤 선택값을 여기에 저장함
	for (BoardType j = 0; j < Size; j++)
		for (BoardType k = 0; k < Size; k++)
			if (res[j][k] == max) {
				if (cnt) cnt--;
				//cnt가 0이 아니면 뽑인 게 이게 아니라 다른 것이므로, 카운트를 1 줄이고 넘김
				else return board[j][k];
				//뽑혔으면 뽑힌 칸에 적힌 숫자 return
			}
}