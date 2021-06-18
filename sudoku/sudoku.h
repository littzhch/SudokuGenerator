#pragma once
#ifdef SUDOKU_EXPORTS
#define SUDOKU_API __declspec(dllexport)
#else
#define SUDOKU_API __declspec(dllimport)
#endif

#include <windows.h>

#define Position(row, col)     (9 * (row)  + (col) - 10)		// 将行数和列数转换为位置索引值
#define GetRow(idx)			   ((idx) / 9 + 1)					// 1-9
#define GetCol(idx)			   ((idx) % 9 + 1)					// 1-9
#define GetBlock(idx)          (((GetRow(idx) - 1) / 3) * 3 + ((GetCol(idx) - 1) / 3) + 1) // 1-9
#define IsValid(bits, number)  (((bits) >> (number)) & 1)		// bits为GetValidNumber()的返回值，number为需要检验的数字
#define RandNum(min, max)      ((UINT8) (rand() % ((max) - (min) + 1) + (min)))	// 返回[min, max]区间内的一个随机整数

typedef struct sudoku {
	UINT8 elements[81];
	int filledNum;
} SUDOKU, *PSUDOKU;

typedef struct {
	SUDOKU problem;
	SUDOKU answer;
	int clueNum;
} SUDOKUPUZZLE, * PSUDOKUPUZZLE;

SUDOKU_API void SuInitialize(PSUDOKU pSudoku);
// 初始化SUDOKU，使其成为一个空的数独盘

SUDOKU_API void UpdateNumber(PSUDOKU pSudoku, UINT8 num, int index);
// 向pSudoku中填入或移除数字，同时更改filledNum
// num 为0~9，填入0代表移除数字
// index为位置索引，0~80，也可用Position()宏从行数、列数中生成

SUDOKU_API UINT16 GetValidNumber(PSUDOKU pSudoku, int index);
// 获取index位置可填入的数字
// 返回值需用IsValid()宏检验

SUDOKU_API void PrintSudoku(PSUDOKU pSudoku);
// 在终端中打印数独盘


SUDOKU_API void GenerateSudoku(PSUDOKUPUZZLE pPuzzle);
// 生成数独，clueNum指定提示数数量；problem和answer不需要初始化
// 可重入函数，适用于多线程同时调用

SUDOKU_API int SolveSudoku(PSUDOKUPUZZLE pPuzzle);
// 求解数独，只求出数独的一个解
// 不修正filledNum，不更改clueNum
// 若成功求出解，返回0；若无解，返回-1