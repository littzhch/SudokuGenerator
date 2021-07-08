#include <stdio.h>
#include "sudoku.h"



void SuInitialize(PSUDOKU pSudoku) {
	ZeroMemory(pSudoku, sizeof(SUDOKU));
	int idx;
	for (idx = 0; idx < 9; idx++) {
		pSudoku->rowValids[idx] = 0b0000001111111110;
	}
	for (idx = 0; idx < 9; idx++) {
		pSudoku->colValids[idx] = 0b0000001111111110;
	}
	for (idx = 0; idx < 9; idx++) {
		pSudoku->blkValids[idx] = 0b0000001111111110;
	}
}


void UpdateNumber(PSUDOKU pSudoku, UINT8 num, int index) {
	int rowIdx = GetRow(index) - 1;
	int colIdx = GetCol(index) - 1;
	int blkIdx = GetBlock(index) - 1;
	UINT16 changeBits = 0;
	if (pSudoku->elements[index]) {
		changeBits ^= ((UINT16)1) << (pSudoku->elements[index]);
		pSudoku->filledNum--;
	}
	if (num) {
		changeBits ^= ((UINT16)1) << num;
		pSudoku->filledNum++;
	}
	pSudoku->rowValids[rowIdx] ^= changeBits;
	pSudoku->colValids[colIdx] ^= changeBits;
	pSudoku->blkValids[blkIdx] ^= changeBits;
	pSudoku->elements[index] = num;
}


UINT16 GetValidNumber(PSUDOKU pSudoku, int index) {
	int rowIdx = GetRow(index) - 1;
	int colIdx = GetCol(index) - 1;
	int blkIdx = GetBlock(index) - 1;
	return (pSudoku->rowValids[rowIdx] & pSudoku->colValids[colIdx] & pSudoku->blkValids[blkIdx]);
}
