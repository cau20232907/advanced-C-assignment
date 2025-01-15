#include <stdio.h>
#include <math.h>
// #pragma warning(disable:4996)

#define PI 3.14159265358979323846
#define radius 10
void draw_circle(char map[][radius * 2 + 1], int, int, char);
void draw_screen(char screen[][35], char map[][radius * 2 + 1], int cx, int cy);

/*10���� ��� ȸ��
double newx,newy,x=radius,y=0;
for(int i=0;i<360;i+=10){
newx=x*cos(PI/18) - y*sin(PI/18);
newy=y*cos(PI/18) + x*sin(PI/18);
printf("%d %d\n", newx, newy);
x=newx; y=newy;
}
/**/
void main() {
	char screen[80][35] = { 0, };
	char map[radius * 2 + 1][radius * 2 + 1] = { 0, };
	draw_screen(screen, map, 10, 24); // ���� ����ֱ�
	// �׸� ���� ��� ������ ������ ����⿡ �� �׸��� ��(�׸� �Ŀ� �����ϸ� map �ʱ�ȭ ������ ������ ��) ������ �����
	draw_circle(map, radius, radius, '*');
	//���غ��� ���� ���� 5�� �׸� �ʿ䰡 ����
	draw_screen(screen, map, 10, 10);
	draw_screen(screen, map, 26, 10);
	draw_screen(screen, map, 42, 10);
	draw_screen(screen, map, 18, 24);
	draw_screen(screen, map, 34, 24);

	for (int j = 0; j < 35; j++) {
		for (int i = 0; i < 80; i++) {
			putchar(screen[i][j]);
		}
		putchar('\n');
	}
}

void draw_circle(char map[][radius * 2 + 1], int cx, int cy, char true_char) {
	double radian;
	int angle, newx, newy;
	
	for (angle = 0; angle < 360; angle += 1) {
		//��Ȯ�� �켱���� �ؼ� angle ��ȭ���� 1�� ����
		radian = angle / 180.0 * PI;
		newx = radius * cos(radian) + 0.5 + cx;
		newy = radius * sin(radian) + 0.5 + cy;
		// 0.5�� �ݿø�(���е��� ����)
		map[newx][newy] = true_char;
	}
}

void draw_screen(char screen[][35], char map[radius * 2 + 1][radius * 2 + 1], int cx, int cy) {
	for (int i = 0; i <= radius * 2; i++) {
		for (int j = 0; j <= radius * 2; j++) {
			if (screen[cx + i - radius][cy + j - radius] == '*')
				; //�̹� ���� ���� ������ �߰� �۾��� ���� �ʰ� �ѱ�
			else if (map[i][j] == '*')
				screen[cx + i - radius][cy + j - radius] = '*';
			else if (screen[cx + i - radius][cy + j - radius] != ' ') {
				screen[cx + i - radius][cy + j - radius] = ' ';
				//����� ��� �ϱ� ���� �� ������ ������ �־��ֱ�
			}
		}
	}
}