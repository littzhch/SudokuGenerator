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
	UINT16 rowValids[9];
	UINT16 colValids[9];
	UINT16 blkValids[9];
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
// 向pSudoku中填入或移除数字
// 只应该填入满足数独条件的数字，不检查
// num 为0~9，填入0代表移除数字
// index为位置索引，0~80，也可用Position()宏从行数、列数中生成
// num 和 index 的范围不检查

SUDOKU_API int UpdateNumberSafe(PSUDOKU pSudoku, UINT8 num, int index);
// 向pSudoku中填入或移除满足数独条件的数字
// 若数字满足数独条件，返回0
// 若数字不满足条件，不改变源数独，返回非零值
// num 为0~9，填入0代表移除数字
// index为位置索引，0~80，也可用Position()宏从行数、列数中生成
// num 和 index 的范围不检查

SUDOKU_API UINT16 GetValidNumber(PSUDOKU pSudoku, int index);
// 获取index位置可填入的数字
// 返回值需用IsValid()宏检验


SUDOKU_API void GenerateSudoku(PSUDOKUPUZZLE pPuzzle);
// 生成数独，clueNum指定提示数数量；problem和answer不需要初始化
// 可重入函数，适用于多线程同时调用

SUDOKU_API int SolveSudoku(PSUDOKUPUZZLE pPuzzle);
// 求解数独，只求出数独的一个解
// pPuzzle->problem 需要是 只且必须 使用SuInitialize()和UpdateNumber()产生的题目
// pPuzzle->answer 无要求
// 不更改clueNum
// 若成功求出解，返回0；若无解，返回-1


SUDOKU_API void GenerateSudokuMT(
	PSUDOKUPUZZLE pPuzzleStart,						// 生成的数独题目的存储空间首地址，以数组方式存储，不需要初始化
	int amount,										// 生成数独题目的数量
	int minClue,									// 提示数的最小值（包含）
	int maxClue, 									// 提示数的最大值（包含）
	int maxThread,									// 最大线程数，范围为[1, 64]，不检查
	void (*InfoProgress)(int current, int total));  // 进度显示回调函数
/* 以多线程方式生成数独题目
 * 在数独生成期间，InfoPrograss函数每间隔约0.3秒被调用一次，
 * 传入当前已经生成和正在生成的数独总数（current）和需要生成的总数独数量（total）。
 * 保证 current == total 的情况只在最后一次调用出现，且调用时所有数独全部生成完成。
 */