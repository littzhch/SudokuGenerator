#include <stdlib.h>
#include <stdio.h>
#include "SudokuOperate.h"


#define RandNum(min, max)  ((UINT8) (rand() % (max - min + 1) + min))

static void FillSquare(PSUDOKU pSudoku);
static void RemoveNumbers(PSUDOKU pSudoku, int clueNum);
static int FillCell(PSUDOKU pSudoku, int idx);
static int HaveSingleAnswer(const PSUDOKU pSudoku);
static int SolveCell(PSUDOKU pSudoku, int * zeroIdxs, int arrMax, int currentPos, int* successTime);


void GenerateSudoku(PSUDOKU pProblem, PSUDOKU pAnswer, int clueNum) {
	srand(GetCurrentThreadId());
	FillSquare(pProblem);
	*pAnswer = *pProblem;
	RemoveNumbers(pProblem, clueNum);
}


static void FillSquare(PSUDOKU pSudoku) {
	SuInitialize(pSudoku);
	FillCell(pSudoku, 0);
}


static void RemoveNumbers(PSUDOKU pSudoku, int clueNum) {
	int randIdx;
	int dotInfoPtr = 0;
	struct {
		int idx;
		UINT8 number;
	} dotInfo[64];

	while (pSudoku->filledNum > clueNum) {
		while (!pSudoku->elements[randIdx = RandNum(0, 80)]);
		dotInfo[dotInfoPtr].idx = randIdx;
		dotInfo[dotInfoPtr++].number = pSudoku->elements[randIdx];
		UpdateNumber(pSudoku, 0, randIdx);
		if (HaveSingleAnswer(pSudoku)) {
			continue;
		}
		randIdx = dotInfo[--dotInfoPtr].idx;
		UpdateNumber(pSudoku, dotInfo[dotInfoPtr].number, randIdx);
	}
}


static int FillCell(PSUDOKU pSudoku, int idx) {
	if (idx == 81) {
		return 1;
	}
	UINT16 validNum = GetValidNumber(pSudoku, idx);
	if (!validNum) {
		return 0;
	}
	UINT8 start = RandNum(1, 9);
	for (int i = 1; i <= 9; i++, (start = start % 9 + 1)) {
		if (IsValid(validNum, start)) {
			UpdateNumber(pSudoku, start, idx);
			if (FillCell(pSudoku, idx + 1)) {
				return 1;
			}
		}
	}
	UpdateNumber(pSudoku, 0, idx);
	return 0;
					
}


static int HaveSingleAnswer(const PSUDOKU pSudoku) {
	SUDOKU sudoku = *pSudoku;
	int arrMax = 0;
	int zeroIdxs[64]; //唯一解数独最少有17个提示数
	int successTime = 0;
	for (int idx = 0; idx < 81; idx++) {
		if (! sudoku.elements[idx]) {
			zeroIdxs[arrMax++] = idx;
		}
	}
	return SolveCell(&sudoku, zeroIdxs, arrMax, 0, &successTime);
}


static int SolveCell(PSUDOKU pSudoku, int* zeroIdxs, int arrMax, int currentPos, int * pSuccessTime) {
	if (currentPos == arrMax) {
		if (*pSuccessTime) {
			return 0;
		}
		*pSuccessTime = 1;
		return 1;
	}
	UINT16 validNum = GetValidNumber(pSudoku, zeroIdxs[currentPos]);
	if (!validNum) {
		return 1;
	}
	UINT8 start = RandNum(1, 9);
	for (int i = 1; i <= 9; i++, (start = start % 9 + 1)) {
		if (IsValid(validNum, start)) {
			UpdateNumber(pSudoku, start, zeroIdxs[currentPos]);
			if (! SolveCell(pSudoku, zeroIdxs, arrMax, currentPos + 1, pSuccessTime)) {
				return 0;
			}
		}
	}
	UpdateNumber(pSudoku, 0, zeroIdxs[currentPos]);
	return 1;
}