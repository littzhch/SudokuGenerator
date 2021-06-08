#include <stdio.h>
#include <time.h>
#include <ctype.h>
#include "sudoku.h"
#include "MultiThread.h"
#include "SudokuIO.h" //TODO: Ϊͷ�ļ����ע��

int main1(void) {
	SUDOKUPUZZLE puzzle;
	puzzle.clueNum = 25;
	int i = 5;
	srand(GetCurrentThreadId());
	while (i--) {
		GenerateSudoku(&puzzle);
		PrintSudoku(&puzzle.problem);
		puts("-");
		PrintSudoku(&puzzle.answer);
		puts("-");
	}
	getchar();
}

int main2(void) {
	SUDOKU sudoku = {
		   {0,0,4,0,0,0,0,0,3,
			5,0,9,0,0,3,0,6,0,
			0,0,0,7,6,0,0,0,0,
			2,0,1,0,0,0,0,0,0,
			3,0,0,0,9,1,0,4,8,
			0,4,0,0,3,0,0,1,0,
			0,0,2,9,1,0,0,0,0,
			4,0,0,0,8,0,2,0,0,
			9,5,0,6,0,2,8,3,0},

			30

	};
	PrintSudoku(&sudoku);
	SolveSudoku(&sudoku);
	PrintSudoku(&sudoku);
}

int main3(void) {
	int t1 = time(NULL);
	PSUDOKUPUZZLE sp = malloc(sizeof(SUDOKUPUZZLE) * 100000);
	GenerateSudokuMT(sp, 100000, 31, 31, 64, 1);
	int t2 = time(NULL);
	printf("%d\n", t2 - t1);
}

int main4(void) {
	srand(time(NULL));
	SUDOKUPUZZLE sudokup;
	sudokup.clueNum = 24;
	GenerateSudoku(&sudokup);
	PrintSudoku(&sudokup.problem);
}

int main5(void) {
	printf("%zd\n", sizeof(SUDOKUPUZZLE));
	SetupRepository();
	SUDOKUPUZZLE puzzles[1000];
	GenerateSudokuMT(puzzles, 1000, 30, 60, 32, 1);
	int code = AddToRepository(puzzles, 1000);
	printf("%d\n", code);
	
	printf("%d\n", GetPuzzleAmountInRepository());
	return 0;
}

int main(void) {
	CleanRepository();
	SUDOKUPUZZLE puzzles[IMPORTBUFFERLEN];
	int amount;
	while (1) {
		amount = ImportPuzzleFromJson(puzzles, "result.json");
		for (int idx = 0; idx < amount; idx++) {
			SolveSudoku(puzzles + idx);
		}
		AddToRepository(puzzles, amount);
		if (amount < IMPORTBUFFERLEN) {
			break;
		}
	}
	ExportRepoAsJson("result2.json");
}

int main7(void) {
	CleanRepository();
	SUDOKUPUZZLE puzzles[1000];
	GenerateSudokuMT(puzzles, 1000, 29, 70, 64, 1);
	AddToRepository(puzzles, 1000);
	ExportRepoAsJson("result.json");
	return 0;
}