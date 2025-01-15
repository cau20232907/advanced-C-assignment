#pragma once
#ifndef try_message //재시도 메시지가 없으면
#define try_message
#define trying_again ""
#define try_succeed ""
#define try_falied "메모리가 부족합니다."
#define file_falied "파일 열기에 실패해였습니다."
#define mid_in_falied "문장을 입력받을 메모리가 부족합니다. 사전에 있는 어떤 단어보다 긴 단어를 입력하였으므로 더 이상 입력을 받지 않습니다."
#endif // !try_message

#ifndef 과제3_H //과제 3 공통 헤더
#define 과제3_H
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#ifndef realloc_try
#define realloc_try 10 //malloc 재시도 횟수(malloc에 실패했을 때 사용)
#endif // !realloc_try
#ifndef try_message //재시도 메지시가 없으면
#define trying_again "Memory problem has occurred. now trying again..."
#define try_succeed "Memory problem fixed."
#define try_falied "Memory problem: fix failed... Now exiting program..."
#endif // !try_message

//공통 함수
extern const char* sgets(int len, int relen, FILE* _Stream); //개행시까지 거의 무제한으로 문장을 입력받는 함수
extern void* calloc_in(const size_t len); //sgets 함수에서 배열 공간 최초 할당할 때 사용
extern void* realloc_in(void* to, const size_t len); //sgets 함수에서 배열 공간 연장할 때 사용
// extern void arrset(char* arr, size_t len); //배열의 내용을 전부 0으로 초기화하는 함수, memset으로 대체
extern void freeset(void** p); //동적할당 해제에 사용
#endif

#define flielocate "dict_test.TXT" //파일명
#define filelen 1348428 //파일 크기
#define fileline 51845 //파일 '\n' 수

//return 자료형에는 char*(*)[2]을 설정할 수 없어 void형으로 임시 처리
void* dictread();
void* wordfinder(const char* str, const char* (*dict)[2]);
int dictlen(const char* (*dict)[2]);

/*
FILE*fp=NULL:
fp=afopen*/

/*
단어 주면 뜻 구하기, 컴퓨터는 단어를 고르지 않음(혼자 10번)(중복은 불가), 단어 분석(구조체와 qsort 쓰면 좋음)(비교 함수를 어떻게 만들 것인가)
*/