#include <stdlib.h>
#include <time.h>
#include "SudokuOperate.h"


#define RANDNUM  ((UINT8) (rand() % 9 + 1))

static void FillSquare(PSUDOKU pSudoku);
static int FillCell(PSUDOKU pSudoku, int idx);

void GenerateSudoku(PSUDOKU pProblem, PSUDOKU pAnswer, int clueNum) {
	srand(GetCurrentThreadId());
	SuInitialize(pProblem);
	SuInitialize(pAnswer);

}


static void FillSquare(PSUDOKU pSudoku) {
	SuInitialize(pSudoku);
	UINT16 validNum;
	UINT8 result;
	for (int idx = 0; idx < 81; idx++) {
		validNum = GetValidNumber(pSudoku, idx);
		while (IsValid(validNum, (result = RANDNUM)));
	}
}

static int FillCell(PSUDOKU pSudoku, int idx) {
	UINT16 validNum = GetValidNumber(pSudoku, idx);
	if (!validNum) {
		return 1;
	}

}