#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#pragma warning(disable:4996)

#define _CRT_SECURE_NO_WARNINGS
#define Size 5 //���� �� ���� ����
#define Players 4  //�÷��̾� ��
#define Line_To_Win 5 //�¸��ϱ� ���� �ϼ��ؾ��ϴ� ���� ��
#define Init_Gap 1 //��½� ���� �� ���ڰ��� �¿� ����
#define Board_Gap 2 //��½� ���� �� ����
#define Max_Across_Board 3 //��½� �¿�� �ִ�� ����� �� �ִ� ������ ��, 2�� �÷��̿����� �̻��
#define Num_Digit 2 //������ �ȿ� ���� ������ �ִ� �ڸ���
#define Weight(num) ((num)*(num)*(num)*4) //�켱�� ���(�� �ٿ� num��ŭ�� ���� ���� ������ ���� ��, ������ ���� �ٿ� ���� �켱��)
#define Num_of_Bingo "������ ��: " //������ ��� �� ���� �Ʒ��� ���� ����, �׳� ���� ���� �� ���� ������ ���� �� �ٲٸ� �ȵ�
typedef signed int BoardType; //���� �� ������ �ڷ���, �ҷȴ��� ���θ� ��ȣ�� �Ǵ��ϱ⿡ �ݵ�� signed�� ��� ��

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
	char* boardname[Players] = { "�÷��̾�", "COM1","COM2","COM3"};
	//���� �̸�, �ݵ�� �迭�� ��� ��ҿ� ���ڿ��� �־�� ��(���ڿ��� ���� ���� ��Ұ� �ִٸ� ��� �� ���� �߻�)
	//���� ��� ���� �̸��� ���̴� (Size * (Num_Digit + 2 + Init_Gap)�� �ʰ��� �� ����(���� ��� �� ���� �߻�)
	char ifrandom[Players] = { 0,3,3,3 }; //�÷��̾ ���ڸ� �θ��� ���
	srand(time(NULL));
	RandomBoard(board[0]); //�÷��̾� ���� �Է�
	RandomBoard(board[1]); //���� ���� ����
	RandomBoard(board[2]); //���� ���� ����
	RandomBoard(board[3]); //���� ���� ����

	while (!Bingo(board, Players, boardname, ifrandom, lines)); //���ڰ� ���� ������ Bingo�Լ� �ݺ�(1ȸ �ݺ��� ���� 1���� ���ڸ� �θ�)
	//��� �÷��̾ ���ڸ� �ϳ��� �θ� �� ���𰡸� �ϰ� �ʹٸ� while�� �߰�ȣ ���� ���� ��
	printBoard(board, Players, boardname, lines, 0);
	puts("���ڰ� �����Ǿ����ϴ�.");
	for (int j = 0; j < Players; j++)
		if (lines[j] >= Line_To_Win)
			printf(" %s", boardname[j]);
	puts("�� ����Դϴ�.");
}

char Bingo(BoardType(*board)[Size][Size], const int players, const char* boardname[], const char* how_Plck, const BoardType* lines) {
	BoardType num = 0;
	for (int i = 0; i < players; i++) {
		printf("%s�� �����Դϴ�.\n", boardname[i]);
		switch (how_Plck[i]){
		case 0: //���� ���� �����
			printBoard(board, players, boardname, lines, i + 1);
			printf("���� ���ڸ� �Է��� �ֽʽÿ�. �̹� �Էµ� ���ڵ� �Է��� �����մϴ�. ");
			scanf("%d", &num); //���� �Է�
			printf("%d�� �Է��߽��ϴ�.\n\n", num);
			break;
		case 1: //���� ���� ����(�̹� �ҷ��� ���ڵ� ����)
			num = rand() % (Size * Size) + 1;
			printf("%s��(��) �������� ���ڸ� �����մϴ�. �� ���ڴ� %d�Դϴ�.\n\n", boardname[i], num);
			break;
			//���ϴ� ��� ��Ģ�� ������ ���� ����
		case 2: //�ܼ��� �ߺ��� ���ϴ� ���� ����
			do {
				num = rand() % (Size * Size) + 1;
				for (BoardType j = 0; j < Size * Size; j++) {
					if (((BoardType*)(board[i]))[j] == num * (-1)) {
						num = 0;
						break;
					}
				}
			} while (!num);
			printf("%s��(��) �������� ���ڸ� �����մϴ�. �� ���ڴ� %d�Դϴ�.\n\n", boardname[i], num);
			break;
		case 3:
			num = check_best(board[i]);
			printf("%s��(��) ���ڸ� �����մϴ�. �� ���ڴ� %d�Դϴ�.\n\n", boardname[i], num);
			break;
		}
		call_erase(board, players, num); //���� �����
		check_lines(board, players, lines); //�� �� Ȯ��
		for (int j = 0; j < players; j++)
			if (lines[j] >= Line_To_Win)
				return 1;
		//������ �� �̻��� Ŭ�����ߴٸ� ���� ����
		if (!how_Plck[i]) { //����� ���� ���ڸ� �Է��� ��� ���ڸ� ���� ���� ���� ���� �ֱ�
			printBoard(board, Players, boardname, lines, i + 1);
			system("pause"); //���� Ȯ���� �ð� �ֱ�
			system("cls"); //�� �� ����� ȭ�� Ŭ����(�������� ���ڸ� ������� �ش� ����)
		}
	}
	return 0;
}

BoardType** DrawBoard(BoardType board[Size][Size], char* boardname) {
	BoardType i = 1, j, k;

	printf("%s�� ���带 �Է��մϴ�.\n", boardname);
	puts("1������ ������ ��� �� ��� ���� ���������� �Է��մϴ�. ������ ������ ���̸� �ʰ��ϴ� ���� �Է��� ���, ���������� ������� ���� �� ������ ������ �ֽñ� �ٶ��ϴ�.");
	//���ܻ�Ȳ �� �Ϻ��� å���� ����ڿ��� ���ѱ�� ���� �����
	// for (BoardType(*p)[2] = sketch; p < sketch + sizeof(sketch) / (sizeof(BoardType) * 2); p++) 
	while (i <= Size * Size) { //�ݺ� Ƚ���� Size*Size(���忡 ���� �ִ� ����)�� �ǵ��� ��
		printf("%d�� ���ڸ� �� ��° �� �� ��° ���� �����Ͻðڽ��ϱ�: ", i);
		scanf("%d%d", &j, &k); //1������ ��� ������ ����, �ٷ� �������� �ʰ� �� Ȯ��

		//�ڷ����� ����Ͽ� ���α׷� ������ ���� �� �ִ� ���ܻ�Ȳ
		if (j > Size || j <= 0 || k > Size || k <= 0)
			puts("�߸��� �Է��Դϴ�. ��ġ�� �ٽ� �Է��� �ֽñ� �ٶ��ϴ�.");
		else if (board[j - 1][k - 1])
			puts("�̹� �� �ڸ��� �ٸ� ���ڰ� �ֽ��ϴ�. ��ġ�� �ٽ� �Է��� �ֽñ� �ٶ��ϴ�.");
		else {
			//�� �־��ֱ�
			board[j - 1][k - 1] = i;
			i++; //���ܻ�Ȳ�� ��� i�� �������� ����
			system("cls"); //�Է¹��� �� ��� ���̸� ȭ���� ���������� Ŭ����
			printBoard(board, 1, &boardname, NULL, 0); //�Է��� ������ ���� ���� ��Ȳ ���
		}
	}

	/*���� Ȯ�ο� �ڵ�
	if (!boardValidationCheck(board)) {
		puts("���� �Է��� �Ϸ��Ͽ����ϴ�.");
		system("pause"); //����� ���带 Ȯ���� �� �ֵ��� ��� ���
		system("cls"); //���� �Լ� ���� �ø� ����� ȭ�� Ŭ����
		return board; //������ ������ ���� �ּҸ� return
	}
	else {
		puts("���ڸ� �ٽ� �Է��� �ֽñ� �ٶ��ϴ�.");
		return DrawBoard(board, boardname); //���� �߻��� ó������ �ٽ� ���� �Է�
	}
	*/
}

BoardType** RandomBoard(BoardType board[Size][Size]) {
	BoardType i, j, randint, stand, prev;
	for (i = 0; i < Size * Size; i++) {
		randint = rand() % (Size * Size - i); //0���� (�ִ� ����-�̹� ���� ������ ��-1) ������ ������ �� �̱�
		randint++; //���忡 ���ڴ� 1���� ���Ƿ� �̸� ���� ó��
		//�̷��� �ϸ� �������� �ٽ� �̴� ���� ����

		prev = 0;
		stand = 0;
		while (stand != randint) {
			stand = randint;
			for (j = 0; j < i; j++) //ã�� �� ������ ���� ������ ����ŭ ����(�̰� ��� �츮�� ���� ���̱� ����)
				if (((BoardType*)board)[j] > prev &&((BoardType*)board)[j] <= stand) randint++;
			//�տ����� ���ǿ� ���� �ʾ� �Ѿ����, ���ڸ� ���� ���� ã�� ������ ���� ���� �� ��쵵 ����� �ؾ� �ϴ�, ���� ���ڰ� ���������� �̸� �ݺ���
			prev = stand;
		}
		((BoardType*)board)[i] = randint; //�� �־��ֱ�
	}

	/*���� Ȯ�ο� �ڵ�
	if (!boardValidationCheck(board))
		return board; //������ ������ ���� �ּҸ� return(���� �����̹Ƿ� ������ ��� ����)
	else {
		puts("���带 �ٽ� �����մϴ�.");
		return RandomBoard(board); //���� �߻��� ó������ �ٽ� ���� ����
	}
	*/
}

char boardValidationCheck(const BoardType board[Size][Size]) {
	//�ڵ� © �� ���忡 ���ڰ� ����� �ԷµǾ����� Ȯ���ϴ� �ڵ�
	int i, j, check = 0;
	char flag = 0, code = 0;
	char* boardname[1] = { "�����߻� ����" };
	for (i = 0; i < Size; i++) {
		for (j = 0; j < Size; j++) {
			if (!board[i][j] || board[i][j] > Size * Size) {
				//���ڰ� ���ų� �ִ� ���ڸ� �ʰ��ϴ� ��츦 �Ÿ�
				flag = 1;
				break;
			}
			else
				check += board[i][j];
			//�˻��� ���� �� ���� ��� ���� ���� 1���� �ִ� ���ڱ����� �հ� ������ Ȯ���ϴ� �������� ����
		}
		if (flag) break; //�̹� flag�� �������� ������ break
	}
	check -= (Size * Size) * ((Size * Size) + 1) / 2; //1���� n������ ��: n*(n+1)/2
	//������ ���ٸ� check�� 0�� �Ǿ�� ��


	if (flag) code = 1;
	else if (check) code = 2;
	else { //���� ������ Ȯ�εǸ�
		return code; //������ ������ ���� ���ٴ� �ڵ� 0�� return
	}

	puts("������ �߻��Ͽ����ϴ�.");
	switch (code) {
	case 1:
		printf("�ùٸ��� ���� ���ڰ� ��� �ִ� ĭ�� �ֽ��ϴ�. ���� �߰�: %d�� %d��\n", i + 1, j + 1);
		break;
	case 2:
		printf("������ ���� �ùٸ��� �ʽ��ϴ�. %d�� ���ڶ��ϴ�.\n", check * (-1));
		break;
	}
	printBoard(board, 1, &boardname, NULL, 0); //���� Ȯ���� ������ ���� ���� ��Ȳ ���
	return code;
}

void printBoard(const BoardType(*board)[Size][Size], const int players, const char* boardname[], const BoardType* lines, const int opened_Player) {
	//lines�� �Լ� ȣ��� ���� �� ������ ����� 2�� ���� �ʱ� ���� �޾Ƽ� ���
	for (int i = 0; i < players; i += Max_Across_Board) { //���尡 ���� ���η� ��� ���带 ��� ���ϸ� Max_Across_Board��ŭ ��� �Ʒ��ٿ� ��µǵ��� �ϴ� �ݺ���
		for (int k = i, j; k < (players > i + Max_Across_Board ? i + Max_Across_Board : players); k++) { //�켱 ������ �̸� ���
			j = strlen(boardname[k]); //�߾������� ���� �켱 �̸��� ���� �ޱ�, �̶� �̸��� ���̴� ������ ���� ���̸� �Ѿ�� ���ƾ� ��(�ٸ� ���� ó�� �ʿ�)
			for (BoardType l = 0; l < (Size * (Num_Digit + 2 + Init_Gap) - j) / 2; l++) putchar(' '); //�߾������� ���� ���� ä���
			printf("%s", boardname[k]);
			for (BoardType l = 0; l < (Size * (Num_Digit + 2 + Init_Gap) - j) / 2; l++) putchar(' '); //�߾������� ���� ���� ä���
			if ((Size * (Num_Digit + 2 + Init_Gap) - j) % 2) putchar(' '); //�̸� �� ������ ���̰� Ȧ���� �����ʿ� ���� �ϳ� �� �ֱ�
			for (int m = 0; m < Board_Gap; m++) putchar(' '); //���带 �������� ����(�¿찣��)
		}
		putchar('\n');
		for (BoardType j = 0; j < Size; j++) { //�ϴ� ���κ��� ����(�Ʒ� ���� ����� �� ���ٿ� ����� ���� �����Ƿ�)
			for (int k = i; k < (players > i + Max_Across_Board ? i + Max_Across_Board : players); k++) { //������(Max_Across_Board�� �̳��� board��)
				for (BoardType l = 0; l < Size; l++) { //������(board �� ����)
					if (board[k][j][l] < 0)
						printf("(%*d)", Num_Digit, board[k][j][l] * (-1)); //�Ҹ� ���� ���(�Ҹ� ���� ������ ���, ������ ����)
					else{
						if (!opened_Player || opened_Player - 1 == k) //���� ���� ����� ���(opened_Player==0�̸� ��� ���� ����)
							printf(" %*d ", Num_Digit, board[k][j][l]); //���� ���, �ڸ����� Num_Digit(���� ��� Num_Digit==2�� %2d��, Num_Digit==3�̸� %3d�� ��)
						else {
							for (int m = 1; m < Num_Digit + 2; m++) putchar(' '); //�������� ĭ ���߱�
							putchar('-'); //'-'���(���� ����)
						}
					}
					for (int m = 0; m < Init_Gap; m++) putchar(' '); //���ڸ� �������� ����
				}
				for (int m = 0; m < Board_Gap; m++) putchar(' '); //���带 �������� ����(�¿찣��)
			}
			putchar('\n'); //������ ���� �� ����� ���� �ٺ���
		}
		if (lines != NULL)
			for (int k = i, j; k < (players > i + Max_Across_Board ? i + Max_Across_Board : players); k++) { //�켱 ������ �̸� ���
				j = strlen(Num_of_Bingo) + Num_Digit; //�߾������� ���� �켱 ���� �� ǥ�� ���� �ޱ�
				for (BoardType l = 0; l < (Size * (Num_Digit + 2 + Init_Gap) - j) / 2; l++) putchar(' '); //�߾������� ���� ���� ä���
				printf("%s%*d", Num_of_Bingo, Num_Digit, lines[k]);
				for (BoardType l = 0; l < (Size * (Num_Digit + 2 + Init_Gap) - j) / 2; l++) putchar(' '); //�߾������� ���� ���� ä���
				if ((Size * (Num_Digit + 2 + Init_Gap) - j) % 2) putchar(' '); //���� �� ǥ�� �� ������ ���̰� Ȧ���� �����ʿ� ���� �ϳ� �� �ֱ�
				for (int m = 0; m < Board_Gap; m++) putchar(' '); //���带 �������� ����(�¿찣��)
			}
		for (int m = 0; m < Board_Gap; m++) putchar('\n'); //���带 �������� ����(���ϰ���)
	}
}

void call_erase(BoardType(*board)[Size][Size], const int players, const BoardType num) {
	for (int i = 0; i < players; i++)
		for (int j = 0; j < Size; j++)
			for (int k = 0; k < Size; k++)
				if (board[i][j][k] == num)
					board[i][j][k] *= (-1);
	//board�� �� �� num�� ��ġ�ϴ� ���� ������ ������ �ٲ�(�Ҹ� ���� ó��)
	//�ݴ�� ����ϴ� �͵� ������ ��
	//�ߺ��� ���ڸ� ����ϴ� ������ �� ������ ó���� �����ϵ���(�ٸ� �Լ��� �ٲپ �̰� �ٲ� �ʿ䰡 ������), �̹� �� ���ڸ� ���Ҵٰ� break������ ����
}

BoardType* check_lines(BoardType(*board)[Size][Size], const int players, BoardType* res) {
	for (int i = 0, flag = 1; i < players; i++) {
		res[i] = 0;
		//������
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

		//������
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

		//���� ��~������ �Ʒ� �밢��
		//�̵��� x, y ��ǥ ���� ���ٴ� Ư¡�� ����(�̸� �̿���)
		flag = 1;
		for (BoardType j = 0; j < Size; j++) {
			if (board[i][j][j] >= 0) {
				flag = 0;
				break;
			}
		}
		if (flag) res[i]++;

		//���� �Ʒ�~������ �� �밢��
		//�̵��� x, y ��ǥ ���� ���� (���� ���� ���� + 1)�� �׻� ���ٴ� Ư¡�� ����
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
	//��� ĭ�� ����, ���� 1�پ� �ּ� 2�� ���� �ϼ��� ������ �ֹǷ� �⺻ �켱���� 2�� ������
	//�� �̹� �Ҹ� ���ڸ� �켱�� ����(0)

	//�밢�� �켱�� ����
	for (BoardType j = 0; j < Size; j++)
		if (board[j][j] > 0)
			res[j][j]++;
	for (BoardType j = 0; j < Size; j++)
		if (board[j][Size - j - 1] > 0)
			res[j][Size - j - 1]++;

	//������, �����ٿ��� ������ ������ ���� ���� �켱�� ����
	for (BoardType j = 0; j < Size; j++) {
		cnt = 0;
		for (BoardType k = 0; k < Size; k++)
			if (board[j][k] < 0) cnt++; //������ �� ���
		for (BoardType k = 0; k < Size; k++)
			if (board[j][k] > 0) res[j][k] += Weight(cnt); //�켱�� ���ϱ�

	}
	for (BoardType j = 0; j < Size; j++) {
		cnt = 0;
		for (BoardType k = 0; k < Size; k++)
			if (board[k][j] < 0) cnt++; //������ �� ���
		for (BoardType k = 0; k < Size; k++)
			if (board[k][j] > 0) res[k][j] += Weight(cnt); //�켱�� ���ϱ�

	}

	//�밢��
	cnt = 0;
	for (BoardType j = 0; j < Size; j++)
		if (board[j][j] < 0) cnt++; //������ �� ���
	for (BoardType j = 0; j < Size; j++)
		if (board[j][j] > 0) res[j][j] += Weight(cnt); //�켱�� ���ϱ�

	cnt = 0;
	for (BoardType j = 0; j < Size; j++)
		if (board[j][Size - j - 1] < 0) cnt++; //������ �� ���
	for (BoardType j = 0; j < Size; j++)
		if (board[j][Size - j - 1] > 0) res[j][Size - j - 1] += Weight(cnt); //�켱�� ���ϱ�

	//�켱���� ������ ���� �ٲ� �� ����
	//�켱���� ���ϴ� ���� ���� ����� AI�� �̿��ϴ� ���̳�, �׷��Ա��� �� ���� �ƴϹǷ�...

	//�ּ��� �� ���
	//max: �ִ� �켱��, cnt: �ִ� �켱���� ����(�켱�� ���� ������ �� �߿��� ���� ����(������ ����̹Ƿ�))
	for (BoardType j = 0; j < Size * Size; j++) {
		if (((int*)res)[j] > max) {
			max = ((int*)res)[j];
			cnt = 1;
		}
		else if (((int*)res)[j] == max)
			cnt++;
	}
	cnt = rand() % cnt; //���� �߿��� ���� ������ ������ ���� cnt�� �ʿ� �����Ƿ�, ���� ���ð��� ���⿡ ������
	for (BoardType j = 0; j < Size; j++)
		for (BoardType k = 0; k < Size; k++)
			if (res[j][k] == max) {
				if (cnt) cnt--;
				//cnt�� 0�� �ƴϸ� ���� �� �̰� �ƴ϶� �ٸ� ���̹Ƿ�, ī��Ʈ�� 1 ���̰� �ѱ�
				else return board[j][k];
				//�������� ���� ĭ�� ���� ���� return
			}
}