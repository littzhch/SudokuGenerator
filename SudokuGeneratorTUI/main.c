#include "cmdinteract.h"
#include "SudokuIO.h"
#include "sudoku.h"
#include "MultiThread.h"
#include <Windows.h>
//TODO: ����ϸ����ʾ
static inline void CommandClean(_Bool silent);
static inline void CommandInit(_Bool silent);
static inline void CommandExport(const char* filepath, _Bool silent);
static inline void CommandSolve(const char* filepath, int amount, _Bool silent);
static inline void CommandGenerate(int thread, int amount, _Bool silent);
static void PrintProgress(int current, int total);

int main(int argc, char* argv[]) {
	COMMAND cmd;
	ReadCommand(&cmd, argc, argv);

	switch (cmd.type) {
	case TYPE_NONE:
		PrintWelcome(argv[0]);
		break;
	case TYPE_CLEAN:
		CommandClean(cmd.silent);
		break;
	case TYPE_SOLVE:
		CommandSolve(cmd.filepath, cmd.amount, cmd.silent);
		break;
	case TYPE_GENERATE:
		CommandGenerate(cmd.thread, cmd.amount, cmd.silent);
		break;
	case TYPE_INIT:
		CommandInit(cmd.silent);
		break;
	case TYPE_EXPORT:
		CommandExport(cmd.filepath, cmd.silent);
		break;
	case TYPE_HELP:
		PrintHelp();
		break;
	}
	return 0;
}

static inline void CommandClean(_Bool silent) {
	int code = CleanRepository();
	switch (code) {
	case 0:
		return;
	case 1:
		WoinExit(WOIN_REPO_NOTEXIST, NULL, "������Ŀ�洢�ļ������ڣ������´���", silent);
	case -1:
		ErrExit(ERR_REPO_CANTOPEN, NULL, "�޷���������Ŀ�洢�ļ������ܲ����ڻ�ռ��", silent);
	}
}

static inline void CommandInit(_Bool silent) {
	int code = SetupRepository();
	switch (code) {
	case 0:
		return;
	case 1:
		ErrExit(ERR_REPO_EXIST, NULL, "������Ŀ�洢�ļ��Ѵ���", silent);
	case -1:
		ErrExit(ERR_REPO_CANTOPEN, NULL, "�޷�����������Ŀ�洢�ļ�", silent);
	}
}

static inline void CommandExport(const char* filepath, _Bool silent) {
	int code = ExportRepoAsJson(filepath);
	switch (code) {
	case 0:
		return;
	case -1:
		ErrExit(ERR_REPO_CANTOPEN, NULL, "�޷���������Ŀ�洢�ļ������ܲ����ڻ�ռ��", silent);
	case -2:
		ErrExit(ERR_FILE_CANTOPEN, NULL, "�޷���Ŀ���ļ�", silent);
	case -3:
		ErrExit(ERR_REPO_EMPTY, NULL, "������Ŀ�洢�ļ�Ϊ��", silent);
	}
}

static inline void CommandSolve(const char* filepath, int amount, _Bool silent) { //TODO: �����������
	if (!silent) {
		puts("��δ����");
	}
}

static inline void CommandGenerate(int thread, int amount, _Bool silent) { //TODO: �����ʾ������
	if (!thread) {
		thread = 1;
	}
	if (!amount) {
		amount = 1;
	}
	void (*InfoProc)(int, int) = NULL;
	if (!silent) {
		InfoProc = PrintProgress;
	}
	PSUDOKUPUZZLE puzzles = malloc(amount * sizeof(SUDOKUPUZZLE));
	GenerateSudokuMT(puzzles, amount, 30, 30, thread, InfoProc);
	int code = AddToRepository(puzzles, amount);
	free(puzzles);
	if (code == -1) {
		ErrExit(ERR_REPO_CANTOPEN, NULL, "�޷���������Ŀ�洢�ļ������ܲ����ڻ�ռ��", silent);
	}
}

static void PrintProgress(int current, int total) {  //TODO: Ϊ�����������ɫ����
	HANDLE screen = GetStdHandle(STD_OUTPUT_HANDLE); //TODO: �򻯼���
	CONSOLE_SCREEN_BUFFER_INFO screenInfo;
	GetConsoleScreenBufferInfo(screen, &screenInfo);
	int maxchar = screenInfo.dwSize.X;
	int maxbar = maxchar * 2 / 3;
	int finished = maxbar * current / total;
	int unfinished = maxbar - finished;
	int bufferPtr = 0;
	static char buffer[512];
	static first = 1;

	if (first) {
		puts("���ȣ�");
		first = 0;
	}
	buffer[bufferPtr++] = '\r';
	buffer[bufferPtr++] = '[';
	while (finished--) {
		buffer[bufferPtr++] = '#';
	}
	while (unfinished--) {
		buffer[bufferPtr++] = '-';
	}
	buffer[bufferPtr++] = ']';
	buffer[bufferPtr++] = ' ';
	sprintf_s(buffer + bufferPtr, 5, "%3d", 100 * current / total);
	bufferPtr += 3;
	buffer[bufferPtr++] = '%';
	buffer[bufferPtr] = '\0';

	fputs(buffer, stdout);
	if (current == total) {
		puts("");
	}
}