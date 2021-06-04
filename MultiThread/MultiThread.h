#pragma once
#ifdef MULTITHREAD_EXPORTS
#define MULTITHREAD_API __declspec(dllexport)
#else
#define MULTITHREAD_API __declspec(dllimport)
#endif

#include "sudoku.h"

MULTITHREAD_API void GenerateSudokuMT(
	PSUDOKUPUZZLE pPuzzleStart, 
	int amount, 
	int minClue, 
	int maxClue, 
	int maxThread, 
	BOOL showInfo);