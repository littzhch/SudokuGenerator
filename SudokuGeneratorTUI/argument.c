#include <string.h>
#include <windows.h>
#include "cmdinteract.h"

#define INF -1 //used by CheckNumber()

enum lookfor {file, silent, num, trd, clue};
static int ReadOperate(const char* arg);
static int ReadSwitch(const char* arg);
static int CheckNumber(const char* arg, int min, int max);
static void CheckClue(const char* arg, COMMAND* pCommand);

void ReadCommand(COMMAND* pCommand, int argc, const char* argv[]) {
	ZeroMemory(pCommand, sizeof(COMMAND));
	if (argc == 1) {
		pCommand->type = TYPE_NONE;
		return;
	}	
	
	pCommand->type = ReadOperate(argv[1]);
	int currentswitch;
	for (int idx = 2; idx < argc; idx++) {
		currentswitch = ReadSwitch(argv[idx]);
		switch (currentswitch) {
		case silent:
			pCommand->silent = 1;
			continue;
		case file:
			if (idx < argc - 1) {
				pCommand->filepath = argv[++idx];
				continue;
			}
			ErrExit(ERR_ARG_WRONGFORMAT, "-file", "选项后无参数", 0);
		case num:
			if (idx < argc - 1) {
				pCommand->amount = CheckNumber(argv[++idx], 1, INF);
				continue;
			}
			ErrExit(ERR_ARG_WRONGFORMAT, "-num", "选项后无参数", 0);
		case trd:
			if (idx < argc - 1) {
				pCommand->thread= CheckNumber(argv[++idx], 1, 64);
				continue;
			}
			ErrExit(ERR_ARG_WRONGFORMAT, "-trd", "选项后无参数", 0);
		case clue:
			if (idx < argc - 1) {
				CheckClue(argv[++idx], pCommand);
				continue;
			}
			ErrExit(ERR_ARG_WRONGFORMAT, "-clue", "选项后无参数", 0);
		}
	}

	int type = pCommand->type;
	if (!(0b00001 & type)) {
		if (pCommand->silent) {
			ErrExit(ERR_ARG_MISMATCH, "-silent", "当前操作不允许该选项", 0);
		}
	}
	if (!(0b00010 & type)) {
		if (pCommand->filepath) {
			ErrExit(ERR_ARG_MISMATCH, "-file", "当前操作不允许该选项", pCommand->silent);
		}
	}
	if (!(0b00100 & type)) {
		if (pCommand->thread) {
			ErrExit(ERR_ARG_MISMATCH, "-trd", "当前操作不允许该选项", pCommand->silent);
		}
	}
	if (!(0b01000 & type)) {
		if (pCommand->amount) {
			ErrExit(ERR_ARG_MISMATCH, "-num", "当前操作不允许该选项", pCommand->silent);
		}
	}
	if (!(0b10000 & type)) {
		if (pCommand->clue1) {
			ErrExit(ERR_ARG_MISMATCH, "-clue", "当前操作不允许该选项", pCommand->silent);
		}
	}

	if (type == TYPE_EXPORT) {
		if (!pCommand->filepath) {
			ErrExit(ERR_ARG_MISMATCH, argv[1], "未指定 -file 选项", pCommand->silent);
		}
	}
}

static int ReadOperate(const char* arg) {
	size_t len = strlen(arg);
	switch (arg[0]) {
	case 'g':
		if ((len == 1) || (!strcmp(arg, "generate"))) {
			return TYPE_GENERATE;
		}
		break;
	case 's':
		if ((len == 1) || (!strcmp(arg, "solve"))) {
			return TYPE_SOLVE;
		}
		break;
	case 'i':
		if ((len == 1) || (!strcmp(arg, "init"))) {
			return TYPE_INIT;
		}
		break;
	case 'c':
		if ((len == 1) || (!strcmp(arg, "clean"))) {
			return TYPE_CLEAN;
		}
		break;
	case 'e':
		if ((len == 1) || (!strcmp(arg, "export"))) {
			return TYPE_EXPORT;
		}
		break;
	case 'h':
		if ((len == 1) || (!strcmp(arg, "help"))) {
			return TYPE_HELP;
		}
		break;
	}
	ErrExit(ERR_ARG_WRONGFORMAT, arg, "未知的操作", 0);
}

static int ReadSwitch(const char* arg) {
	if (arg[0] != '-') {
		ErrExit(ERR_ARG_WRONGFORMAT, arg, "无效的选项", 0);
	}
	
	if (!strcmp("file", arg + 1)) {
		return file;
	}
	if (!strcmp("silent", arg + 1)) {
		return silent;
	}
	if (!strcmp("num", arg + 1)) {
		return num;
	}
	if (!strcmp("trd", arg + 1)) {
		return trd;
	}
	if (!strcmp("clue", arg + 1)) {
		return clue;
	}
	ErrExit(ERR_ARG_WRONGFORMAT, arg, "无效的选项", 0);
}

static int CheckNumber(const char* arg, int min, int max) {
	int ptr = -1;
	while (arg[++ptr]) {
		if (!isdigit(arg[ptr])) {
			ErrExit(ERR_ARG_WRONGFORMAT, arg, "未发现正整数", 0);
		}
	}

	int number;
	sscanf_s(arg, "%d", &number);
	if (min > 0) {
		if (number < min) {
			ErrExit(ERR_ARG_WRONGFORMAT, arg, "数字超出范围", 0);
		}
	}
	if (max > 0) {
		if (number > max) {
			ErrExit(ERR_ARG_WRONGFORMAT, arg, "数字超出范围", 0);
		}
	}
	return number;
}

static void CheckClue(const char* arg, COMMAND* pCommand) {
	int ptr = -1;
	_Bool twoclue = 0;
	while (arg[++ptr]) {
		if (! isdigit(arg[ptr])) {
			if (arg[ptr] != '-') {
				ErrExit(ERR_ARG_WRONGFORMAT, arg, "无效的提示数输入", 0);
			}
			else {
				if (!twoclue) {
					twoclue = 1;
				}
				else {
					ErrExit(ERR_ARG_WRONGFORMAT, arg, "无效的提示数输入", 0);
				}
			}
		}
	}

	int clue1;
	int clue2;
	int number;
	if (twoclue) {
		number = sscanf_s(arg, "%d-%d", &clue1, &clue2);
		if (number != 2) {
			ErrExit(ERR_ARG_WRONGFORMAT, arg, "无效的提示数输入", 0);
		}
	}
	else {
		sscanf_s(arg, "%d", &clue1);
	}

	if (clue1 < 17 || clue1 > 81) {
		ErrExit(ERR_ARG_WRONGFORMAT, arg, "提示数超出范围", 0);
	}
	if (twoclue) {
		if (clue2 < 17 || clue2 > 81) {
			ErrExit(ERR_ARG_WRONGFORMAT, arg, "提示数超出范围", 0);
		}
		if (clue1 > clue2) {
			pCommand->clue1 = clue2;
			pCommand->clue2 = clue1;
		}
		else {
			pCommand->clue1 = clue1;
			pCommand->clue2 = clue2;
		}
	}
	else {
		pCommand->clue1 = clue1;
		pCommand->clue2 = clue1;
	}
}