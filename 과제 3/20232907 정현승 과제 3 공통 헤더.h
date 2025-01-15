#pragma once
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
extern void arrset(char* arr, size_t len); //�迭�� ������ ���� 0���� �ʱ�ȭ�ϴ� �Լ�
extern void freeset(void** p); //�����Ҵ� ������ ���