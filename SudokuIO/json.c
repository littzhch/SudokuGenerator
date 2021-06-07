#include <stdio.h>
#include "sudoku.h"
#include "SudokuIO.h"

static void PrintHeader(int amount, FILE* file);
static void PrintPuzzle(PSUDOKUPUZZLE pPuzzle, _Bool isEnd, FILE* file);
static void PrintEnd(FILE* file);
static void PrintNumbers(PSUDOKU pSudoku, FILE* file);

int ExportRepoAsJson(const char* filepath) {
	FILE* repo = NULL;
	FILE* target = NULL;
	fopen_s(&repo, REPONAME, "rb");
	if (!repo) {
		return -1;
	}
	int num;
	fread(&num, sizeof(int), 1, repo);
	if (!num) {
		fclose(repo);
		return -3;
	}
	fopen_s(&target, filepath, "w");
	if (!target) {
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