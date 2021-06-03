#pragma once
#ifdef SUDOKUOPERATE_EXPORTS
#define SUDOKUOPERATE_API __declspec(dllexport)
#else
#define SUDOKUOPERATE_API __declspec(dllimport)
#endif

#include "sudoku.h"

SUDOKUOPERATE_API void GenerateSudoku(PSUDOKU pProblem, PSUDOKU pAnswer, int clueNum);
// 生成数独，题目保存在pProblem中，答案保存在pAnswer中，clueNum指定提示数数量
// 适用于多线程同时调用，同一线程多次调用结果相同

SUDOKUOPERATE_API void SolveSudoku(PSUDOKU pProblem);
// 求解数独，只填充数独的一个解