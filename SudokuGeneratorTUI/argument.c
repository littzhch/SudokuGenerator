#include <string.h>
#include <windows.h>
#include "cmdinteract.h"


static const char* switchname[] = { "-silent", "-file", "-trd", "-num", "-clue" };

static int ReadOperate(const char* arg);
static int ReadSwitch(const char* arg);
static int CheckNumber(const char* arg, int min, int max);
static void CheckClue(const char* arg, COMMAND command);

void ReadCommand(COMMAND command, int argc, const char* argv[]) {
	ZeroMemory(command, sizeof(COMMAND));
	if (argc == 1) {
		command[type] = TYPE_NONE;
		return;
	}
	
	command[type] = ReadOperate(argv[1]);
	int currentswitch;
	for (int idx = 2; idx < argc; idx++) {
		currentswitch = ReadSwitch(argv[idx]);
		if ((idx == argc - 1) && (currentswitch != silent)) {
			ErrExit(ERR_ARG_WRONGFORMAT, switchname[currentswitch], "选项后无参数", 0);
		}
		switch (currentswitch) {
		case silent:
			command[silent] = 1;
			break;
		case file:
			command[file] = argv[++idx];
			break;
		case num:
			command[num] = CheckNumber(argv[++idx], 1, -1);
			break;
		case trd:
			command[trd] = CheckNumber(argv[++idx], 1, 64);
			break;
		case clue:
			CheckClue(argv[++idx], command);
			break;
		}
	}

	int operateType = (int)command[type];
	for (int start = silent; start <= clue; start++) {
		if (!(operateType & (1 << start))) {
			if (command[start]) {
				ErrExit(ERR_ARG_MISMATCH, switchname[start], 
					"当前操作不允许该选项", start == silent ? command[silent] : 0);
			}
		}
	}
	if (operateType == TYPE_EXPORT) {
		if (!command[file]) {
			ErrExit(ERR_ARG_MISMATCH, argv[1], "未指定 -file 选项", command[silent]);
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
	case 'q':
		if ((len == 1) || (!strcmp(arg, "query"))) {
			return TYPE_QUERY;
		}
		break;
	}
	ErrExit(ERR_ARG_WRONGFORMAT, arg, "未知的操作", 0);
}

static int ReadSwitch(const char* arg) {
	for (int stype = silent; stype <= clue; stype++) {
		if (!strcmp(switchname[stype], arg)) {
			return stype;
		}
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

static void CheckClue(const char* arg, COMMAND command) {
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
			command[clue] = clue2;
			command[clue_2] = clue1;
		}
		else {
			command[clue] = clue1;
			command[clue_2] = clue2;
		}
	}
	else {
		command[clue] = clue1;
		command[clue_2] = clue1;
	}
}