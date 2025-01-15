#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#define namelen 18
#define codelen 9
#define phonelen 17
void 중간실습1();
void mystrcpy(char* des, const char* from);
void mystrncpy(char* des, const char* from, size_t size);

void main() {
	char str[7];
	mystrncpy(str, "Hello!", 6);
	puts(str);
	putchar(str[6]);
}

void 중간실습1() {
	char name[namelen], code[codelen], phone[phonelen];
	printf("name = ");
	fgets(name, namelen, stdin);
	name[strlen(name) - 1] = 0;
	printf("code = ");
	fgets(code, codelen, stdin);
	//fflush(stdin);
	code[strlen(code) - 1] = 0;
	printf("phone= ");
	fgets(phone, phonelen, stdin);
	phone[strlen(phone) - 1] = 0;
	printf("phone number of %s with code %s is %s", name, code, phone);
}

void mystrcpy(char* des, const char* from) {
	do *des++ = *from;
	while (*from++);
}

void mystrncpy(char* des, const char* from, size_t size) {
	do *des++ = *from;
	while (*from++ && --size);
}

void mystrcat(char* des, const char* from) {
	while (*des) des++;
	mystrcpy(des, from);
}

void mystrncat(char* des, const char* from, size_t size) {
	while (*des) des++;
	mystrncpy(des, from, size);
}

size_t mystrlen(const char* const str) {
	const char* p = str;
	while (*p) p++;
	return (size_t)(p - str);
}

char mystrcmp(const char* str1, const char* str2) {
	while (*str1 != '\0' && *str1++ != *str2++);
	return *str1 - *str2;
}

char mystrncmp(const char* str1, const char* str2, size_t size) {
	while (--size && *str1 != '\0' && *str1++ != *str2++);
	return *str1 - *str2;
}