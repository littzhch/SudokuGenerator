#include <stdio.h>
#include <time.h>
#include "sudoku.h"
#include "MultiThread.h"

int main1(void) {
	SUDOKU problem;
	SUDOKU answer;
	int i = 5;
	srand(GetCurrentThreadId());
	time_t t1 = time(NULL);
	while (i--) {
		GenerateSudoku(&problem, &answer, 30);
		PrintSudoku(&problem);
		puts("-");
		PrintSudoku(&answer);
		puts("-");
	}
	time_t t2 = time(NULL);
	printf("%lld", t2 - t1);
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

int main(void) {
	int t1 = time(NULL);
	PSUDOKUPUZZLE sp = malloc(sizeof(SUDOKUPUZZLE) * 200000);
	GenerateSudokuMT(sp, 2000000, 31, 31, 64, 0);
	int t2 = time(NULL);
	printf("%d\n", t2 - t1);
}