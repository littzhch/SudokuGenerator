#pragma once
#include <windows.h>
#include "sudoku.h"

int GetSudokuProblem(PSUDOKUPUZZLE pPuzzle);
// 获取用户输入。 若成功返回非零值；若用户退出返回0