#pragma once
#ifdef SUDOKUIO_EXPORTS
#define SUDOKUIO_API __declspec(dllexport)
#else
#define SUDOKUIO_API __declspec(dllimport)
#endif

#include "sudoku.h"


SUDOKUIO_API int SetupRepository(void);
// 创建存储库，若成功，返回0；若已存在存储库，返回1；若失败，返回-1

SUDOKUIO_API int CleanRepository(void);
// 清除存储库。若成功，返回0；若无存储库，创建新的存储库，返回1；若失败，返回-1

SUDOKUIO_API int GetPuzzleAmountInRepository(void);
// 获取存储库中的数独题目数量。若无存储库或存储库打开失败，返回-1

SUDOKUIO_API int AddToRepository(PSUDOKUPUZZLE puzzles, int amount);
// 向存储库中添加数独。若成功，返回0；若无存储库或存储库打开失败，返回-1