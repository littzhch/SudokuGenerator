/*
 *	Dancing Links X
 *	使用数组代替指针链表
 */

#include "sudoku.h"
#include <stdio.h>

typedef UINT32 ELEMENT;

static ELEMENT left[3000];
static ELEMENT right[3000];
static ELEMENT up[3000];
static ELEMENT down[3000];
static ELEMENT col[3000];
static int row[3000];

static ELEMENT stack[3000];
static int stackPtr;

static ELEMENT generator;
static PSUDOKU answer;

#define LeftElmt(element)   (left[element])
#define RightElmt(element)  (right[element])
#define UpElmt(element)     (up[element])
#define DownElmt(element)   (down[element])
#define ColElmt(element)    (col[element])
#define RowNum(element)     (row[element])

#define GetRowNum(cellIdx, fillNum)  (81 * ((fillNum) - 1) + (cellIdx) + 1)

#define InitStack()		(stackPtr = 0)
#define StackPush(elmt) (stack[stackPtr++] = elmt)
#define StackPop()		(stack[--stackPtr])

#define GenerateNextElement()	(generator++)
#define InitGenerator()			(generator = 325)
// 条件宏
#define POS_UNIQUE 0
#define ROW_UNIQUE 1
#define COL_UNIQUE 2
#define BLK_UNIQUE 3

static inline void SetupArrays(PSUDOKU pSudoku);

static inline void RemoveColumn(ELEMENT elmtInCol);
static inline void RemoveRow(ELEMENT elmtInRow);
static inline void RestoreColumn(ELEMENT elmtInCol);
static inline void RestoreRow(ELEMENT elmtInRow);
static inline void RemoveAll(ELEMENT elmt, int* rowCount, int* colCount);
static inline void RestoreAll(int rowCount, int colCount);
static inline void WriteAnswer(int rowNum);

static int Dancing(void);

static inline ELEMENT GetColumn(int condition, int cellIdx, int fillNum);
static inline void AddRow(int cellIdx, int fillNum);

int SolveSudoku(PSUDOKUPUZZLE pPuzzle) {
	answer = &pPuzzle->answer;
	SuInitialize(answer);
	InitStack();
	InitGenerator();
	SetupArrays(&pPuzzle->problem);
	puts("start");
	return Dancing();
}

static void SetupArrays(PSUDOKU pSudoku) {
	for (ELEMENT i = 0; i < 325; i++) {
		LeftElmt(i) = i - 1;
		RightElmt(i) = i + 1;
		DownElmt(i) = i;
		UpElmt(i) = i;
		RowNum(i) = 0;
		ColElmt(i) = i;
	}
	LeftElmt(0) = 324;
	RightElmt(324) = 0;

	for (int cellIdx = 0; cellIdx < 81; cellIdx++) {
		if (pSudoku->elements[cellIdx] == 0) {
			for (int num = 1; num <= 9; num++) {
				AddRow(cellIdx, num);
			}
		}
		else {
			AddRow(cellIdx, pSudoku->elements[cellIdx]);
		}
	}
}

static inline ELEMENT GetColumn(int condition, int cellIdx, int fillNum) {
	int base = 81 * condition + 1;
	switch (condition) {
	case POS_UNIQUE:
		base += cellIdx;
		break;
	case ROW_UNIQUE:
		base += Position(GetRow(cellIdx), fillNum);
		break;
	case COL_UNIQUE:
		base += Position(GetCol(cellIdx), fillNum);
		break;
	case BLK_UNIQUE:
		base += Position(GetBlock(cellIdx), fillNum);
		break;
	}
	return (ELEMENT)base;
}

static inline void AddRow(int cellIdx, int fillNum) {
	int rowNum = GetRowNum(cellIdx, fillNum);
	ELEMENT elmts[4], column;
	for (int condition = 0; condition < 4; condition++) {
		elmts[condition] = GenerateNextElement();
		column = GetColumn(condition, cellIdx, fillNum);

		DownElmt(elmts[condition]) = DownElmt(column);
		UpElmt(elmts[condition]) = column;
		DownElmt(column) = elmts[condition];
		UpElmt(DownElmt(elmts[condition])) = elmts[condition];

		RowNum(elmts[condition]) = rowNum;
		ColElmt(elmts[condition]) = column;
	}

	for (int idx = 0; idx < 4; idx++) {
		LeftElmt(elmts[idx]) = elmts[((idx + 4) - 1) % 4];
		RightElmt(elmts[idx]) = elmts[(idx + 1) % 4];
	}
}


static inline void RemoveColumn(ELEMENT elmtInCol) {
	ELEMENT origin = elmtInCol;
	do {
		RightElmt(LeftElmt(elmtInCol)) = RightElmt(elmtInCol);
		LeftElmt(RightElmt(elmtInCol)) = LeftElmt(elmtInCol);
	} while ((elmtInCol = DownElmt(elmtInCol)) != origin);
}

static inline void RemoveRow(ELEMENT elmtInRow) {
	ELEMENT origin = elmtInRow;
	do {
		DownElmt(UpElmt(elmtInRow)) = DownElmt(elmtInRow);
		UpElmt(DownElmt(elmtInRow)) = UpElmt(elmtInRow);
	} while ((elmtInRow = RightElmt(elmtInRow)) != origin);
}

static inline void RestoreColumn(ELEMENT elmtInCol) {
	ELEMENT origin = elmtInCol;
	do {
		RightElmt(LeftElmt(elmtInCol)) = elmtInCol;
		LeftElmt(RightElmt(elmtInCol)) = elmtInCol;
	} while ((elmtInCol = DownElmt(elmtInCol)) != origin);
}

static inline void RestoreRow(ELEMENT elmtInRow) {
	ELEMENT origin = elmtInRow;
	do {
		DownElmt(UpElmt(elmtInRow)) = elmtInRow;
		UpElmt(DownElmt(elmtInRow)) = elmtInRow;
	} while ((elmtInRow = RightElmt(elmtInRow)) != origin);
}

static inline void RemoveAll(ELEMENT elmt, int* rowCount, int* colCount) { // 先删行， 再删列
	ELEMENT origin = elmt;
	ELEMENT colElmt;
	do {
		colElmt = elmt;
		while ((colElmt = DownElmt(colElmt)) != elmt) {
			if (colElmt != ColElmt(colElmt)) {
				RemoveRow(colElmt);
				(*rowCount)++;
				StackPush(colElmt);
			}
		}
	} while ((elmt = RightElmt(elmt)) != origin);
	
	int i = 4;
	while (i--) {
		RemoveColumn(elmt);
		(*colCount)++;
		StackPush(elmt);
		elmt = RightElmt(elmt);
	}
}

static inline void RestoreAll(int rowCount, int colCount) { // 先恢复列， 再恢复行
	while (colCount--) {
		RestoreColumn(StackPop());
	}
	while (rowCount--) {
		RestoreRow(StackPop());
	}
}

static inline void WriteAnswer(int rowNum) {
	int cellIdx = (rowNum - 1) % 81;
	int fillNum = (rowNum - 1) / 81 + 1;
	UpdateNumber(answer, (UINT8)fillNum, cellIdx);
}

static int Dancing(void) {  // -1：重新递归  0: 找到答案
	ELEMENT start = RightElmt(0);
	if (start == 0) {
		return 0;
	}
	int rowRemoveCount;
	int colRemoveCount;
	while ((start = DownElmt(start)) != ColElmt(start)) {
		rowRemoveCount = 0;
		colRemoveCount = 0;
		RemoveAll(start, &rowRemoveCount, &colRemoveCount);
		if (!Dancing()) {
			WriteAnswer(RowNum(start));
			return 0;
		}
		RestoreAll(rowRemoveCount, colRemoveCount);
	}
	return -1;
}
