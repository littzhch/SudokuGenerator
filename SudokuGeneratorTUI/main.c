#include "cmdinteract.h"
#include "SudokuIO.h"
#include "sudoku.h"
#include "MultiThread.h"
#include <Windows.h>

static inline void CommandClean(_Bool silent);
static inline void CommandInit(_Bool silent);
static inline void CommandExport(const char* filepath, _Bool silent);
static inline void CommandSolve(const char* filepath, _Bool silent);
static inline void CommandGenerate(COMMAND command);
static void PrintProgress(int current, int total);


int main(int argc, char* argv[]) {
	IOInit();
	COMMAND cmd;
	ReadCommand(cmd, argc, argv);

	switch ((unsigned int) cmd[type]) {
	case TYPE_NONE:
		PrintWelcome();
		break;
	case TYPE_CLEAN:
		CommandClean(cmd[silent]);
		break;
	case TYPE_SOLVE:
		CommandSolve(cmd[file], cmd[silent]);
		break;
	case TYPE_GENERATE:
		CommandGenerate(cmd);
		break;
	case TYPE_INIT:
		CommandInit(cmd[silent]);
		break;
	case TYPE_EXPORT:
		CommandExport(cmd[file], cmd[silent]);
		break;
	case TYPE_HELP:
		PrintHelp();
		break;
	case TYPE_QUERY:
		int amount = GetPuzzleAmountInRepository();
		if (amount == -1) {
			ErrExit(ERR_REPO_CANTOPEN, NULL, "无法打开数独题目存储文件，可能不存在或被占用", 0);
		}
		printf("当前共有%d道数独题目\n", amount);
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
		if (!silent) {
			WarnningInfo("数独题目存储文件不存在，已重新创建");
		}
		return;
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
		ErrExit(ERR_FILE_CANTOPEN, NULL, "无法打开目标文件，可能被占用", silent);
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
			if (AddToRepository(puzzles, IMPORTBUFFERLEN)) {
				ErrExit(ERR_REPO_CANTOPEN, NULL, "无法打开数独题目存储文件，可能不存在或被占用", silent);
			}
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
			if (AddToRepository(puzzles, num)) {
				ErrExit(ERR_REPO_CANTOPEN, NULL, "无法打开数独题目存储文件，可能不存在或被占用", silent);
			}
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

static inline void CommandGenerate(COMMAND command) {
	if (!command[trd]) {
		command[trd] = 1;
	}
	if (!command[num]) {
		command[num] = 1;
	}
	if (!command[clue]) {
		command[clue] = command[clue_2] = 40;
	}

	void (*InfoProc)(int, int) = NULL;
	if (!command[silent]) {
		InfoProc = PrintProgress;
	}
	PSUDOKUPUZZLE puzzles = malloc(((int)command[num]) * sizeof(SUDOKUPUZZLE));
	GenerateSudokuMT(puzzles, command[num], command[clue], command[clue_2], command[trd], InfoProc);
	int code = AddToRepository(puzzles, command[num]);
	free(puzzles);
	if (code == -1) {
		ErrExit(ERR_REPO_CANTOPEN, NULL, "无法打开数独题目存储文件，可能不存在或被占用", command[silent]);
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