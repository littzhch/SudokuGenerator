#include <stdlib.h>
#include <stdio.h>
#include "sudoku.h"


static inline void FillSquare(PSUDOKU pSudoku);												// 填充数独盘
static inline void RemoveNumbers_1(PSUDOKU pSudoku, int clueNum);							// 打洞
static inline void RemoveNumbers_2(PSUDOKU pSudoku, int clueNum);
static int FillCell(PSUDOKU pSudoku, int idx);												// 在FillSquare()中被调用，递归函数
static inline int HaveSingleAnswerAfterRemove(const PSUDOKU pSudoku, int removeIdx);		// 在RemoveNumbers中被调用
static int SolveCell(PSUDOKU pSudoku, int * zeroIdxs, 
	int arrMax, int currentPos);														    // 在HaveSingleAnswer中被调用，递归函数
static inline void shuffle(int * numbers, int amount);
static inline void exchange(int* a, int* b);
static int RemoveCell(PSUDOKU pSudoku, int* idxOrder, int clueNum);


void GenerateSudoku(PSUDOKUPUZZLE pPuzzle) {   //TRY: 添加错误处理
	FillSquare(&pPuzzle->problem);
	pPuzzle->answer = pPuzzle->problem;
	RemoveNumbers_1(&pPuzzle->problem, pPuzzle->clueNum);
}


static inline void FillSquare(PSUDOKU pSudoku) {
	SuInitialize(pSudoku);
	FillCell(pSudoku, 0);
}



static void RemoveNumbers_1(PSUDOKU pSudoku, int clueNum) {  //TODO: 选择合适的RemoveNumbers算法
	int randIdx;
	while (pSudoku->filledNum > clueNum) {
		while (!pSudoku->elements[randIdx = RandNum(0, 80)])
			;
		if (HaveSingleAnswerAfterRemove(pSudoku, randIdx)) {
			UpdateNumber(pSudoku, 0, randIdx);
		}
	}
}


static inline void RemoveNumbers_2(PSUDOKU pSudoku, int clueNum) {
	int idxOrder[81];
	for (int idx = 0; idx < 81; idx++) {
		idxOrder[idx] = idx;
	}
	shuffle(idxOrder, 81);
	RemoveCell(pSudoku, idxOrder, clueNum);
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


static inline void shuffle(int* numbers, int amount) {
	int dest;
	for (int idx = 0; idx < amount; idx++) {
		dest = RandNum(idx, amount - 1);
		exchange(numbers + idx, numbers + dest);
	}
}


static inline void exchange(int* a, int* b) {
	if (a == b) {
		return;
	}
	*a = (*a) ^ (*b);
	*b = (*a) ^ (*b);
	*a = (*a) ^ (*b);
}


static int RemoveCell(PSUDOKU pSudoku, int* idxOrder, int clueNum) {
	if (pSudoku->filledNum == clueNum) {
		return 1;
	}
	int cellNum;
	for (int idx = 0; idx < 81; idx++) {
		if (pSudoku->elements[idxOrder[idx]]) {
			if (HaveSingleAnswerAfterRemove(pSudoku, idx)) {
				cellNum = pSudoku->elements[idxOrder[idx]];
				UpdateNumber(pSudoku, 0, idxOrder[idx]);
				if (RemoveCell(pSudoku, idxOrder, clueNum)) {
					return 1;
				}
				UpdateNumber(pSudoku, cellNum, idxOrder[idx]);
			}
		}
	}
	return 0;
}