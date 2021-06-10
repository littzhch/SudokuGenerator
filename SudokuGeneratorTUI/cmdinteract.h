#pragma once
#include <stdio.h>


typedef struct {
	int type;
	int amount;
	int thread;
	char* filepath;
	_Bool silent;
} COMMAND;

// COMMAND.type ��
#define TYPE_SOLVE			0b010011
#define TYPE_GENERATE		0b011101
#define TYPE_INIT			0b010001
#define TYPE_CLEAN          0b110001
#define TYPE_HELP           0b010000
#define TYPE_EXPORT			0b110011
#define TYPE_NONE			0b000000

void ReadCommand(COMMAND* pCommand, int argc, const char* argv[]);
// ���������в���������COMMAND����ָ��
// ��ָ����󣬱����˳���ȷ�����ص�ָ���ʽ��ȷ


void PrintWelcome(const char* selfpath);
// ��ӡ��ӭ��Ϣ�������汾�ŵ�

void PrintHelp(void);
// ��ӡ������Ϣ



// ����;������/�����˳�����ֵ
#define	ERR_ARG_MISMATCH    -2
#define	ERR_ARG_WRONGFORMAT -1

#define ERR_REPO_CANTOPEN    1
#define ERR_REPO_EXIST       2
#define ERR_REPO_EMPTY       3
#define WOIN_REPO_NOTEXIST   4
#define ERR_FILE_CANTOPEN    5
_Noreturn ErrExit(int errType, const char* errContent, const char* message, _Bool silent);
_Noreturn WoinExit(int woinType, const char* woinContent, const char* message, _Bool silent);