#pragma once
#ifndef try_message //��õ� �޽����� ������
#define try_message
#define trying_again ""
#define try_succeed ""
#define try_falied "�޸𸮰� �����մϴ�."
#define file_falied "���� ���⿡ �����ؿ����ϴ�."
#define mid_in_falied "������ �Է¹��� �޸𸮰� �����մϴ�. ������ �ִ� � �ܾ�� �� �ܾ �Է��Ͽ����Ƿ� �� �̻� �Է��� ���� �ʽ��ϴ�."
#endif // !try_message

#ifndef ����3_H //���� 3 ���� ���
#define ����3_H
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#ifndef realloc_try
#define realloc_try 10 //malloc ��õ� Ƚ��(malloc�� �������� �� ���)
#endif // !realloc_try
#ifndef try_message //��õ� �����ð� ������
#define trying_again "Memory problem has occurred. now trying again..."
#define try_succeed "Memory problem fixed."
#define try_falied "Memory problem: fix failed... Now exiting program..."
#endif // !try_message

//���� �Լ�
extern const char* sgets(int len, int relen, FILE* _Stream); //����ñ��� ���� ���������� ������ �Է¹޴� �Լ�
extern void* calloc_in(const size_t len); //sgets �Լ����� �迭 ���� ���� �Ҵ��� �� ���
extern void* realloc_in(void* to, const size_t len); //sgets �Լ����� �迭 ���� ������ �� ���
// extern void arrset(char* arr, size_t len); //�迭�� ������ ���� 0���� �ʱ�ȭ�ϴ� �Լ�, memset���� ��ü
extern void freeset(void** p); //�����Ҵ� ������ ���
#endif

#define flielocate "dict_test.TXT" //���ϸ�
#define filelen 1348428 //���� ũ��
#define fileline 51845 //���� '\n' ��

//return �ڷ������� char*(*)[2]�� ������ �� ���� void������ �ӽ� ó��
void* dictread();
void* wordfinder(const char* str, const char* (*dict)[2]);
int dictlen(const char* (*dict)[2]);

/*
FILE*fp=NULL:
fp=afopen*/

/*
�ܾ� �ָ� �� ���ϱ�, ��ǻ�ʹ� �ܾ ���� ����(ȥ�� 10��)(�ߺ��� �Ұ�), �ܾ� �м�(����ü�� qsort ���� ����)(�� �Լ��� ��� ���� ���ΰ�)
*/