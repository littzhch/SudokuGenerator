#pragma once
#include <stdio.h>
#include "sudoku.h"


typedef UINT64 COMMAND[7];
enum cmdIdx { silent, file, trd, num, clue, clue_2, type };

// OPERATE 宏
#define TYPE_SOLVE			0b0100011
#define TYPE_GENERATE		0b0111101
#define TYPE_INIT			0b0100001
#define TYPE_CLEAN          0b1100001
#define TYPE_HELP           0b0100000
#define TYPE_EXPORT			0b1100011
#define TYPE_QUERY          0b1100000
#define TYPE_NONE			0b0000000

void ReadCommand(COMMAND command, int argc, const char* argv[]);
// 分析命令行参数，生成COMMAND类型指令
// 若指令错误，报错退出；确保返回的指令格式正确


void PrintWelcome(void);
// 打印欢迎信息，包括版本号等

void PrintHelp(void);
// 打印帮助信息



// 错误和警告代号/程序退出返回值
#define	ERR_ARG_MISMATCH    -2
#define	ERR_ARG_WRONGFORMAT -1

#define ERR_REPO_CANTOPEN    2
#define ERR_REPO_EXIST       3
#define ERR_REPO_EMPTY       4
#define ERR_FILE_CANTOPEN    5

_Noreturn ErrExit(int errType, const char* errContent, const char* message, _Bool silent);

void WarnningInfo(const char* message);

int GetPuzzleFromUser(PSUDOKUPUZZLE puzzles);
// 从用户处获取数独，返回题目数量，不超过IMPORTBUFFERLEN