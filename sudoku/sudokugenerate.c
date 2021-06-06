#include <stdlib.h>
#include <stdio.h>
#include "sudoku.h"


static inline void FillSquare(PSUDOKU pSudoku);                  // 填充数独盘
static inline void RemoveNumbers_1(PSUDOKU pSudoku, int clueNum);  // 打洞
static inline void RemoveNumbers_2(PSUDOKU pSudoku, int clueNum);
static int FillCell(PSUDOKU pSudoku, int idx);					 // 在FillSquare()中被调用，递归函数
static inline int HaveSingleAnswer(const PSUDOKU pSudoku);       // 在RemoveNumbers中被调用
static int SolveCell(PSUDOKU pSudoku, int * zeroIdxs, 
	int arrMax, int currentPos, int* successTime);				 // 在HaveSingleAnswer中被调用，递归函数
static inline void shuffle(int * numbers, int amount);
static inline void exchange(int* a, int* b);
static int RemoveCell(PSUDOKU pSudoku, int* idxOrder, int clueNum);


void GenerateSudoku(PSUDOKUPUZZLE pPuzzle) {   //TODO: 添加错误处理
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


static inline int HaveSingleAnswer(const PSUDOKU pSudoku) {
	SUDOKU sudoku = *pSudoku;
	int arrMax = 0;
	int zeroIdxs[64];
	int successTime = 0;
	for (int idx = 0; idx < 81; idx++) {
		if (! sudoku.elements[idx]) {
			zeroIdxs[arrMax++] = idx;
		}
	}
	return SolveCell(&sudoku, zeroIdxs, arrMax, 0, &successTime);
}


static int SolveCell(PSUDOKU pSudoku, int* zeroIdxs, 
	int arrMax, int currentPos, int * pSuccessTime) {
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
			cellNum = pSudoku->elements[idxOrder[idx]];
			UpdateNumber(pSudoku, 0, idxOrder[idx]);
			if (HaveSingleAnswer(pSudoku)) {
				if (RemoveCell(pSudoku, idxOrder, clueNum)) {
					return 1;
				}
			}
			UpdateNumber(pSudoku, cellNum, idxOrder[idx]);
		}
	}
	return 0;
}