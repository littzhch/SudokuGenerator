#include "cmdinteract.h"
#include "SudokuIO.h"
#include "sudoku.h"
#include "MultiThread.h"
#include <Windows.h>
//TODO: 更详细的提示
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
		WoinExit(WOIN_REPO_NOTEXIST, NULL, "数独题目存储文件不存在，已重新创建", silent);
	case -1:
		ErrExit(ERR_REPO_CANTOPEN, NULL, "无法打开数独题目存储文件，可能不存在或被占用", silent);
	}
}

static inline void CommandInit(_Bool silent) {
	int code = SetupRepository();
	switch (code) {
	case 0:
		return;
	case 1:
		ErrExit(ERR_REPO_EXIST, NULL, "数独题目存储文件已存在", silent);
	case -1:
		ErrExit(ERR_REPO_CANTOPEN, NULL, "无法创建数独题目存储文件", silent);
	}
}

static inline void CommandExport(const char* filepath, _Bool silent) {
	int code = ExportRepoAsJson(filepath);
	switch (code) {
	case 0:
		return;
	case -1:
		ErrExit(ERR_REPO_CANTOPEN, NULL, "无法打开数独题目存储文件，可能不存在或被占用", silent);
	case -2:
		ErrExit(ERR_FILE_CANTOPEN, NULL, "无法打开目标文件", silent);
	case -3:
		ErrExit(ERR_REPO_EMPTY, NULL, "数独题目存储文件为空", silent);
	}
}

static inline void CommandSolve(const char* filepath, int amount, _Bool silent) { //TODO: 完成数独生成
	if (!silent) {
		puts("暂未开发");
	}
}

static inline void CommandGenerate(int thread, int amount, _Bool silent) { //TODO: 添加提示数输入
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
		ErrExit(ERR_REPO_CANTOPEN, NULL, "无法打开数独题目存储文件，可能不存在或被占用", silent);
	}
}

static void PrintProgress(int current, int total) {  //TODO: 为进度条添加绿色背景
	HANDLE screen = GetStdHandle(STD_OUTPUT_HANDLE); //TODO: 简化计算
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
		puts("进度：");
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