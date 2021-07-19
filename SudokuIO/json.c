#include <stdio.h>
#include <ctype.h>
#include "sudoku.h"
#include "SudokuIO.h"

static void PrintHeader(int amount, FILE* file);
static void PrintPuzzle(PSUDOKUPUZZLE pPuzzle, _Bool isEnd, FILE* file);
static void PrintEnd(FILE* file);
static void PrintNumbers(PSUDOKU pSudoku, FILE* file);
static int ReadSingleSudoku(PSUDOKUPUZZLE pPuzzle, FILE* file);
// 从json格式文件中读取一个数独题目。成功返回0；若读到EOF，返回EOF
static inline void EatBlank(FILE* file);
extern char REPOPATH[];

int ExportRepoAsJson(const char* filepath) {
	FILE* repo;
	FILE* target;

	if (fopen_s(&repo, REPOPATH, "rb")) {
		return -1;
	}
	int num;
	fread(&num, sizeof(int), 1, repo);
	if (!num) {
		fclose(repo);
		return -3;
	}
	if (fopen_s(&target, filepath, "w")) {
		fclose(repo);
		return -2;
	}
	
	PrintHeader(num, target);
	SUDOKUPUZZLE puzzleBuffer;
	num--;
	while(num--) {
		fread(&puzzleBuffer, sizeof(SUDOKUPUZZLE), 1, repo);
		PrintPuzzle(&puzzleBuffer, 0, target);
	}
	fread(&puzzleBuffer, sizeof(SUDOKUPUZZLE), 1, repo);
	PrintPuzzle(&puzzleBuffer, 1, target);
	PrintEnd(target);

	fclose(repo);
	fclose(target);
	return 0;
}


int ImportPuzzleFromJson(PSUDOKUPUZZLE puzzles, const char* filepath) {
	static FILE* importFile = NULL;
	int code;
	if (!importFile) {
		code = fopen_s(&importFile, filepath, "r");
		if (code) {
			return -1;
		}
	}

	int idx = 0;
	for (; idx < IMPORTBUFFERLEN; idx++) {
		if (ReadSingleSudoku(puzzles + idx, importFile) == EOF) {
			fclose(importFile);
			importFile = NULL;
			break;
		}
	}
	return idx;
}


static void PrintHeader(int amount, FILE* file) {
	fprintf_s(file, "{\n\t\"puzzle_amount\": %d,"
					 "\n\t\"puzzle_list\": [\n", amount);
}

static void PrintPuzzle(PSUDOKUPUZZLE pPuzzle, _Bool isEnd, FILE* file) {
	int clue = pPuzzle->clueNum;
	fprintf_s(file, "\t\t{\n\t\t\t\"clue\": %d,\n\t\t\t\"problem\": [\n", clue);
	PrintNumbers(&pPuzzle->problem, file);
	fputs("\t\t\t],\n\t\t\t\"answer\": [\n", file);
	PrintNumbers(&pPuzzle->answer, file);
	fputs("\t\t\t]\n\t\t}", file);
	if (isEnd) {
		fputs("\n", file);
	}
	else {
		fputs(",\n", file);
	}
}

static void PrintEnd(FILE* file) {
	fputs("\t]\n}", file);
}

static void PrintNumbers(PSUDOKU pSudoku, FILE* file) {
	fprintf_s(file, "\t\t\t\t%hhu, ", pSudoku->elements[0]);
	for (int idx = 1; idx < 80; idx++) {
		if (!(idx % 9)) {
			if (!(idx % 27)) {
				fputs("\n", file);
			}
			fputs("\n\t\t\t\t", file);
		}
		fprintf_s(file, "%hhu, ", pSudoku->elements[idx]);
		if (!((idx + 1) % 3)) {
			fputs(" ", file);
		}
	}
	fprintf_s(file, "%hhu\n", pSudoku->elements[80]);
}

static int ReadSingleSudoku(PSUDOKUPUZZLE pPuzzle, FILE* file) {
	char ch;
	int status = 0; // 0: 等待读入"["，1:等待读入数字，2: 等待读入","， 3: 等待读入"]"
	int currentIdx = 0;
	SuInitialize(&pPuzzle->problem);
	EatBlank(file);
	while ((ch = fgetc(file)) != EOF) {
		switch (status) {
		case 0:
			if (ch == '[') {
				status = 1;
			}
			goto next;
		case 1:
			if (isdigit(ch)) {
				if (UpdateNumberSafe(&pPuzzle->problem, ch - '0', currentIdx++)) {
					break;
				}
				status = (currentIdx == 81) ? 3 : 2;
				goto next;
			}
			break;
		case 2:
			if (ch == ',') {
				status = 1;
				goto next;
			}
			break;
		case 3:
			if (ch == ']' && pPuzzle->problem.filledNum < 81) {
				pPuzzle->clueNum = pPuzzle->problem.filledNum;
				return 0;
			}
			break;
		}
		SuInitialize(&pPuzzle->problem);
		status = 0;
		currentIdx = 0;
		next:
		EatBlank(file);
	}
	return EOF;
}


static inline void EatBlank(FILE* file) {
	char ch;
	while (isspace(ch = fgetc(file)))
		;
	if (ch != EOF) {
		ungetc(ch, file);
	}
}