#include <string.h>
#include <windows.h>
#include "cmdinteract.h"

#define INF -1 //used by CheckNumber()

enum lookfor {file, silent, num, trd};
static int ReadOperate(const char* arg); // �д�����
static int ReadSwitch(const char* arg); // �д�����
static int CheckNumber(const char* arg, int min, int max); //�д�����

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
			ErrExit(ERR_ARG_WRONGFORMAT, "-file", "ѡ����޲���", 0);
		case num:
			if (idx < argc - 1) {
				pCommand->amount = CheckNumber(argv[++idx], 1, INF);
				continue;
			}
			ErrExit(ERR_ARG_WRONGFORMAT, "-num", "ѡ����޲���", 0);
		case trd:
			if (idx < argc - 1) {
				pCommand->thread= CheckNumber(argv[++idx], 1, 64);
				continue;
			}
			ErrExit(ERR_ARG_WRONGFORMAT, "-trd", "ѡ����޲���", 0);
		}
	}

	int type = pCommand->type;
	if (!(0b0001 & type)) {
		if (pCommand->silent) {
			ErrExit(ERR_ARG_MISMATCH, "-silent", "��ǰ�����������ѡ��", 0);
		}
	}
	if (!(0b0010 & type)) {
		if (pCommand->filepath) {
			ErrExit(ERR_ARG_MISMATCH, "-file", "��ǰ�����������ѡ��", pCommand->silent);
		}
	}
	if (!(0b0100 & type)) {
		if (pCommand->thread) {
			ErrExit(ERR_ARG_MISMATCH, "-trd", "��ǰ�����������ѡ��", pCommand->silent);
		}
	}
	if (!(0b1000 & type)) {
		if (pCommand->amount) {
			ErrExit(ERR_ARG_MISMATCH, "-num", "��ǰ�����������ѡ��", pCommand->silent);
		}
	}

	if (type == TYPE_EXPORT) {
		if (!pCommand->filepath) {
			ErrExit(ERR_ARG_MISMATCH, argv[1], "δָ�� -file ѡ��", pCommand->silent);
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
	ErrExit(ERR_ARG_WRONGFORMAT, arg, "δ֪�Ĳ���", 0);
}

static int ReadSwitch(const char* arg) {
	if (arg[0] != '-') {
		ErrExit(ERR_ARG_WRONGFORMAT, arg, "��Ч��ѡ��", 0);
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
	ErrExit(ERR_ARG_WRONGFORMAT, arg, "��Ч��ѡ��", 0);
}

static int CheckNumber(const char* arg, int min, int max) {
	int ptr = -1;
	while (arg[++ptr]) {
		if (!isdigit(arg[ptr])) {
			ErrExit(ERR_ARG_WRONGFORMAT, arg, "δ��������", 0);
		}
	}

	int number;
	sscanf_s(arg, "%d", &number);
	if (min > 0) {
		if (number < min) {
			ErrExit(ERR_ARG_WRONGFORMAT, arg, "���ֳ�����Χ", 0);
		}
	}
	if (max > 0) {
		if (number > max) {
			ErrExit(ERR_ARG_WRONGFORMAT, arg, "���ֳ�����Χ", 0);
		}
	}
	return number;
}