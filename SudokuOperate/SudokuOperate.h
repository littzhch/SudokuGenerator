#pragma once
#ifdef SUDOKUOPERATE_EXPORTS
#define SUDOKUOPERATE_API __declspec(dllexport)
#else
#define SUDOKUOPERATE_API __declspec(dllimport)
#endif

#include "sudoku.h"

SUDOKUOPERATE_API void GenerateSudoku(PSUDOKU pProblem, PSUDOKU pAnswer, int clueNum);