#include <stdio.h>
#include "sudoku.h"



void SuInitialize(PSUDOKU pSudoku) {
	ZeroMemory(pSudoku, sizeof(SUDOKU));
}


void UpdateNumber(PSUDOKU pSudoku, UINT8 num, int index) {
	if (pSudoku->elements[index]) {
		if (!num)
			pSudoku->filledNum--;
	}
	else {
		if (num)
			pSudoku->filledNum++;
	}
	pSudoku->elements[index] = num;
}


UINT16 GetValidNumber(PSUDOKU pSudoku, int index) {
	UINT16 result = 0b0000001111111110;
	int row = index / 9 + 1;
	int col = index % 9 + 1;
	int blockCenterRow = ((row - 1) / 3) * 3 + 2;
	int blockCenterCol = ((col - 1) / 3) * 3 + 2;
	for (int idx = 1; idx <= 9; idx++) {
		result &= ~(1 << pSudoku->elements[Position(row, idx)]);
		result &= ~(1 << pSudoku->elements[Position(idx, col)]);
	}
	for (int i = blockCenterRow - 1; i <= blockCenterRow + 1; i++) {
		for (int j = blockCenterCol - 1; j <= blockCenterCol + 1; j++) {
			if ((i != row) && (j != col)) {
				result &= ~(1 << pSudoku->elements[Position(i, j)]);
			}
		}
	}
	return result;
}

void PrintSudoku(PSUDOKU pSudoku) {
	for (int i = 1; i <= 9; i++) {
		for (int j = 1; j <= 9; j++) {
			printf("%d ", pSudoku->elements[Position(i, j)]);
		}
		puts("");
	}
}