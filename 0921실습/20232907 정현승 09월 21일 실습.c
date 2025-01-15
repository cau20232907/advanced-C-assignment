#include <stdio.h>
#include <math.h>
// #pragma warning(disable:4996)

#define PI 3.14159265358979323846
#define radius 10
void draw_circle(char map[][radius * 2 + 1], int, int, char);
void draw_screen(char screen[][35], char map[][radius * 2 + 1], int cx, int cy);

/*10도씩 계속 회전
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
	draw_screen(screen, map, 10, 24); // 공백 찍어주기
	// 그린 원이 닿는 영역만 공백이 생기기에 원 그리기 전(그린 후에 시행하면 map 초기화 과정을 거져야 함) 공백을 찍어줌
	draw_circle(map, radius, radius, '*');
	//상각해보니 굳이 원을 5번 그릴 필요가 없음
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
		//정확도 우선으로 해서 angle 변화량을 1로 변경
		radian = angle / 180.0 * PI;
		newx = radius * cos(radian) + 0.5 + cx;
		newy = radius * sin(radian) + 0.5 + cy;
		// 0.5는 반올림(정밀도를 위함)
		map[newx][newy] = true_char;
	}
}

void draw_screen(char screen[][35], char map[radius * 2 + 1][radius * 2 + 1], int cx, int cy) {
	for (int i = 0; i <= radius * 2; i++) {
		for (int j = 0; j <= radius * 2; j++) {
			if (screen[cx + i - radius][cy + j - radius] == '*')
				; //이미 점이 찍혀 있으면 추가 작업을 하지 않고 넘김
			else if (map[i][j] == '*')
				screen[cx + i - radius][cy + j - radius] = '*';
			else if (screen[cx + i - radius][cy + j - radius] != ' ') {
				screen[cx + i - radius][cy + j - radius] = ' ';
				//출력을 췹게 하기 위해 빈 공간에 공백을 넣어주기
			}
		}
	}
}