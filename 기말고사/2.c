#include <stdio.h>
#include <stdlib.h>

struct Node {
	char data[2];
	struct Node* next;
};

struct Node* make(char ver);
void display(struct Node* p);

void main2() {
	struct Node* start, *reverse;
	start = make(0);
	reverse = make(1);

	display(start);
	display(reverse);
}

struct Node* make(char ver) {
	struct Node* start = NULL;
	struct Node* p = NULL;
	char data[2] = { 0, };

	if (ver) data[0] = data[1] = 'Z';
	else data[0] = data[1] = 'A';

	start = malloc(sizeof(struct Node));
	
	while (data[0]) {
		if (p) p = p->next;
		else p = start;
		p->data[0] = data[0];
		p->data[1] = data[1];
		p->next = malloc(sizeof(struct Node));

		if (ver) {
			data[1]--;
			if (data[1] < 'A') {
				data[1] += 26;
				data[0]--;
				if (data[0] < 'A') data[0] = 0;
			}
		}
		else {
			data[1]++;
			if (data[1] > 'Z') {
				data[1] -= 26;
				data[0]++;
				if (data[0] > 'Z') data[0] = 0;
			}
		}
	}
	p->next = NULL;
	return start;
}

void display(struct Node* p) {
	while (p) {
		putchar(p->data[0]);
		putchar(p->data[1]);
		putchar(' ');
		p = p->next;
	}
	putchar('\n');
}