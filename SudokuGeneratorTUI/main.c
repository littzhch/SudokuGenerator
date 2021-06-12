#include "cmdinteract.h"
#include "SudokuIO.h"
#include "sudoku.h"
#include "MultiThread.h"
#include <Windows.h>
//TODO: 写文档
//TODO: 测试
static inline void CommandClean(_Bool silent);
static inline void CommandInit(_Bool silent);
static inline void CommandExport(const char* filepath, _Bool silent);
static inline void CommandSolve(const char* filepath, _Bool silent);
static inline void CommandGenerate(COMMAND* pCommand);
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
		CommandSolve(cmd.filepath, cmd.silent);
		break;
	case TYPE_GENERATE:
		CommandGenerate(&cmd);
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

static inline void CommandSolve(const char* filepath, _Bool silent) {
	SUDOKUPUZZLE puzzles[IMPORTBUFFERLEN];
	int num;
	if (filepath) {
		while ((num = ImportPuzzleFromJson(puzzles, filepath)) == IMPORTBUFFERLEN) {
			for (int idx = 0; idx < IMPORTBUFFERLEN; idx++) {
				if (SolveSudoku(puzzles + idx) == -1) {
					WarnningInfo("发现一个无解数独");
				}
			}
			AddToRepository(puzzles, IMPORTBUFFERLEN);
		}
		if (num == -1) {
			ErrExit(ERR_FILE_CANTOPEN, NULL, "无法打开目标文件", silent);
		}
		else {
			for (int idx = 0; idx < num; idx++) {
				if (SolveSudoku(puzzles + idx) == -1) {
					WarnningInfo("发现一个无解数独");
				}
			}
			AddToRepository(puzzles, num);
		}
	}
	else {
		if (silent) {
			return;
		}
		num = GetPuzzleFromUser(puzzles);
		for (int idx = 0; idx < num; idx++) {
			if (SolveSudoku(puzzles + idx) == -1) {
				WarnningInfo("发现一个无解数独");
			}
		}
		AddToRepository(puzzles, num);
	}
}

static inline void CommandGenerate(COMMAND* pCommand) {
	if (!pCommand->thread) {
		pCommand->thread = 1;
	}
	if (!pCommand->amount) {
		pCommand->amount = 1;
	}
	if (!pCommand->clue1) {
		pCommand->clue1 = pCommand->clue2 = 40;
	}

	void (*InfoProc)(int, int) = NULL;
	if (!pCommand->silent) {
		InfoProc = PrintProgress;
	}
	PSUDOKUPUZZLE puzzles = malloc(pCommand->amount * sizeof(SUDOKUPUZZLE));
	GenerateSudokuMT(puzzles, pCommand->amount, pCommand->clue1, pCommand->clue2, pCommand->thread, InfoProc);
	int code = AddToRepository(puzzles, pCommand->amount);
	free(puzzles);
	if (code == -1) {
		ErrExit(ERR_REPO_CANTOPEN, NULL, "无法打开数独题目存储文件，可能不存在或被占用", pCommand->silent);
	}
}

static void PrintProgress(int current, int total) {
	static const int maxbar = 60;
	static char buffer[128];
	static first = 1;

	int finished = maxbar * current / total;
	int unfinished = maxbar - finished;
	int bufferPtr = 0;

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