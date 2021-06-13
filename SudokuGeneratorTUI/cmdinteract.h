#pragma once
#include <stdio.h>
#include "sudoku.h"


typedef struct {
	int type;
	int amount;
	int thread;
	int clue1;
	int clue2;
	const char* filepath;
	_Bool silent;
} COMMAND;

// COMMAND.type ��
#define TYPE_SOLVE			0b0100011     // ����λ�Ӹߵ��ͣ�clue num trd file silent
#define TYPE_GENERATE		0b0111101
#define TYPE_INIT			0b0100001
#define TYPE_CLEAN          0b1100001
#define TYPE_HELP           0b0100000
#define TYPE_EXPORT			0b1100011
#define TYPE_QUERY          0b1100000
#define TYPE_NONE			0b0000000

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

#define ERR_REPO_CANTOPEN    2
#define ERR_REPO_EXIST       3
#define ERR_REPO_EMPTY       4
#define ERR_FILE_CANTOPEN    5

_Noreturn ErrExit(int errType, const char* errContent, const char* message, _Bool silent);

void WarnningInfo(const char* message);

int GetPuzzleFromUser(PSUDOKUPUZZLE puzzles);
// ���û�����ȡ������������Ŀ������������IMPORTBUFFERLEN