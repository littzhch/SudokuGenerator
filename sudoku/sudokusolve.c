/*
 *	Dancing Links X
 *	使用数组
 */

#include "sudoku.h"

typedef UINT32 ELEMENT;

static ELEMENT* left;
static ELEMENT* right;
static ELEMENT* up;
static ELEMENT* down;
static ELEMENT* col;
static int* row;

#define LeftElmt(element)   (left[element])
#define RightElmt(element)  (right[element])
#define UpElmt(element)     (up[element])
#define DownElmt(element)   (down[element])
#define ColElmt(element)    (col[element])
#define RowNum(element)     (row[element])

#define GetRowNum(cellIdx, fillNum)  (81 * ((fillNum) - 1) + (cellIdx) + 1)

// 条件宏
#define POS_UNIQUE 0
#define ROW_UNIQUE 1
#define COL_UNIQUE 2
#define BLK_UNIQUE 3

static void SetupArrays(PSUDOKU pSudoku);
static inline ELEMENT* GetColumn(int condition, int cellIdx, int fillNum);
static inline int GetRowNumber(int cellIdx, int fillNum);



static void SetupArrays(PSUDOKU pSudoku) {
	size_t arrayLen = sizeof(ELEMENT) * ((size_t) 81 * 36 - (size_t) pSudoku->filledNum * 32 + 325);
	left = (ELEMENT*)malloc(arrayLen);
	right = (ELEMENT*)malloc(arrayLen);
	up = (ELEMENT*)malloc(arrayLen);
	down = (ELEMENT*)malloc(arrayLen);
	col = (ELEMENT*)malloc(arrayLen);
	row = (int*)malloc(arrayLen);

	if (!(left && right && up && down && col && row)) {
		return;
	}

	ZeroMemory(down, arrayLen);

	for (ELEMENT i = 0; i < 325; i++) {
		LeftElmt(i) = i - 1;
		RightElmt(i) = i + 1;
		RowNum(i) = 0;
		ColElmt(i) = i;
	}
	LeftElmt(0) = 324;
	RightElmt(324) = 0;


}


static inline ELEMENT* GetColumn(int condition, int cellIdx, int fillNum) {
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