#include "sudoku.h"

#define RandNum(min, max)  ((UINT8) (rand() % (max - min + 1) + min))
static int SolveCell(PSUDOKU pSudoku, int* zeroIdxs, int arrMax, int currentPos);

void SolveSudoku(PSUDOKU pProblem) {
	int arrMax = 0;
	int zeroIdxs[81];
	for (int idx = 0; idx < 81; idx++) {
		if (!pProblem->elements[idx]) {
			zeroIdxs[arrMax++] = idx;
		}
	}
	SolveCell(pProblem, zeroIdxs, arrMax, 0);
}

static int SolveCell(PSUDOKU pSudoku, int* zeroIdxs, int arrMax, int currentPos) {
	if (currentPos == arrMax) {
		return 0;
	}
	UINT16 validNum = GetValidNumber(pSudoku, zeroIdxs[currentPos]);
	if (!validNum) {
		return 1;
	}
	UINT8 start = RandNum(1, 9);
	for (int i = 1; i <= 9; i++, (start = start % 9 + 1)) {
		if (IsValid(validNum, start)) {
			UpdateNumber(pSudoku, start, zeroIdxs[currentPos]);
			if (!SolveCell(pSudoku, zeroIdxs, arrMax, currentPos + 1)) {
				return 0;
			}
		}
	}
	UpdateNumber(pSudoku, 0, zeroIdxs[currentPos]);
	return 1;
}