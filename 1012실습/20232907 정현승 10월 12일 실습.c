#include <stdio.h>
#pragma warning(disable:4996)



void main() {
	int number[10] = { 1,2,3,4,5,6,7,8,9,10 }, * ptr;
	for (ptr = number; ptr < number; ptr++)
		if (*ptr % 2) (*ptr) *= 10;
}










/*
void swap(int* a, int* b);
void swap_arr(int* arr, int len);

void main() {
	int a = 10, b = 20;
	int data[5] = { 1,2,3,4,5 };
}

void swap(int* a, int* b) {
	int tmp = *a;
	*a = *b;
	*b = tmp;
}

void swap_arr(int*arr, int len) {
	int* max = arr + len - 1;
	while (arr < max) {
		swap(arr, max);
		arr++;
		max--;
	}
	
	for (int i = 0, tmp; i < len / 2; i++) {
		tmp = arr[i];
		arr[i] = arr[4 - i];
		arr[4 - i] = tmp;
	}
	
}
*/