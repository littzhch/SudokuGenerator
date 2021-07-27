#include <stdlib.h>
#include <stdio.h>
#include "sudoku.h"


static inline void FillSquare(PSUDOKU pSudoku);						// 填充数独盘
static inline int RemoveNumbers(PSUDOKU pSudoku, int clueNum);		// 打洞，若成功返回0，若死锁返回非零
static int FillCell(PSUDOKU pSudoku, int idx);						// 在FillSquare()中被调用，递归函数
static inline int HaveSingleAnswerAfterRemove(const PSUDOKU pSudoku, int removeIdx); // 在RemoveNumbers中被调用
static int SolveCell(PSUDOKU pSudoku, int * zeroIdxs, 
	int arrMax, int currentPos); // 在HaveSingleAnswer中被调用，递归函数


void GenerateSudoku(PSUDOKUPUZZLE pPuzzle) {   //TRY: 添加错误处理
	FillSquare(&pPuzzle->answer);
	do {
		pPuzzle->problem = pPuzzle->answer;
	} 
	while (RemoveNumbers(&pPuzzle->problem, pPuzzle->clueNum));
}



static inline void FillSquare(PSUDOKU pSudoku) {
	SuInitialize(pSudoku);
	FillCell(pSudoku, 0);
}

static int RemoveNumbers(PSUDOKU pSudoku, int clueNum) {
	int randIdx;
	int count = 200;
	while (pSudoku->filledNum > clueNum && --count) {
		while (!pSudoku->elements[randIdx = RandNum(0, 80)]);
		if (HaveSingleAnswerAfterRemove(pSudoku, randIdx)) {
			UpdateNumber(pSudoku, 0, randIdx);
		}
	}
	return (!count);
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

static inline int HaveSingleAnswerAfterRemove(const PSUDOKU pSudoku, int removeIdx) {
	SUDOKU sudoku = *pSudoku;
	UINT16 validNums = GetValidNumber(&sudoku, removeIdx);
	int zeroIdx[64];
	int zeroIdxPtr = 0;
	for (int idx = 0; idx < 81; idx++) {
		if (!sudoku.elements[idx]) {
			zeroIdx[zeroIdxPtr++] = idx;
		}
	}

	for (int num = 1; num <= 9; num++) {
		if (IsValid(validNums, num)) {
			UpdateNumber(&sudoku, num, removeIdx);
			if (SolveCell(&sudoku, zeroIdx, zeroIdxPtr, 0)) {
				return 0;
			}
		}
	}
	return 1;
}

static int SolveCell(PSUDOKU pSudoku, int* zeroIdxs, 
	int arrMax, int currentPos) {
	if (currentPos == arrMax) {
		return 1;
	}
	UINT16 validNum = GetValidNumber(pSudoku, zeroIdxs[currentPos]);
	if (!validNum) {
		return 0;
	}
	UINT8 start = RandNum(1, 9);
	for (int i = 1; i <= 9; i++, (start = start % 9 + 1)) {
		if (IsValid(validNum, start)) {
			UpdateNumber(pSudoku, start, zeroIdxs[currentPos]);
			if (SolveCell(pSudoku, zeroIdxs, arrMax, currentPos + 1)) {
				return 1;
			}
		}
	}
	UpdateNumber(pSudoku, 0, zeroIdxs[currentPos]);
	return 0;
}