#include <stdio.h>
#include <time.h>
#include "sudoku.h"
#include "SudokuOperate.h"

int main(void) {
	SUDOKU problem;
	SUDOKU answer;
	int i = 5;
	srand(GetCurrentThreadId());
	time_t t1 = time(NULL);
	while (i--) {
		GenerateSudoku(&problem, &answer, 26);
		PrintSudoku(&problem);
		puts("-");
		PrintSudoku(&answer);
		puts("-");
	}
	time_t t2 = time(NULL);
	printf("%lld", t2 - t1);
	getchar();
}