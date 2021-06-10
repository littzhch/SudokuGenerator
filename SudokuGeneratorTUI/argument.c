#include <string.h>
#include <windows.h>
#include "cmdinteract.h"

#define INF -1 //used by CheckNumber()

enum lookfor {file, silent, num, trd};
static int ReadOperate(const char* arg); // 有错误处理
static int ReadSwitch(const char* arg); // 有错误处理
static int CheckNumber(const char* arg, int min, int max); //有错误处理

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
		}
	}

	int type = pCommand->type;
	if (!(0b0001 & type)) {
		if (pCommand->silent) {
			ErrExit(ERR_ARG_MISMATCH, "-silent", "当前操作不允许该选项", 0);
		}
	}
	if (!(0b0010 & type)) {
		if (pCommand->filepath) {
			ErrExit(ERR_ARG_MISMATCH, "-file", "当前操作不允许该选项", pCommand->silent);
		}
	}
	if (!(0b0100 & type)) {
		if (pCommand->thread) {
			ErrExit(ERR_ARG_MISMATCH, "-trd", "当前操作不允许该选项", pCommand->silent);
		}
	}
	if (!(0b1000 & type)) {
		if (pCommand->amount) {
			ErrExit(ERR_ARG_MISMATCH, "-num", "当前操作不允许该选项", pCommand->silent);
		}
	}

	if (type == TYPE_EXPORT) {
		if (!pCommand->filepath) {
			ErrExit(ERR_ARG_MISMATCH, argv[1], "未指定 -file 选项", pCommand->silent);
		}
	}
}

static int ReadOperate(const char* arg) {
	int len = strlen(arg);
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
	ErrExit(ERR_ARG_WRONGFORMAT, arg, "无效的选项", 0);
}

static int CheckNumber(const char* arg, int min, int max) {
	int ptr = -1;
	while (arg[++ptr]) {
		if (!isdigit(arg[ptr])) {
			ErrExit(ERR_ARG_WRONGFORMAT, arg, "未发现数字", 0);
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