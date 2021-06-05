#pragma once
#ifdef SUDOKU_EXPORTS
#define SUDOKU_API __declspec(dllexport)
#else
#define SUDOKU_API __declspec(dllimport)
#endif

#include <windows.h>

#define Position(row, col)     (9 * row  + col - 10)
#define IsValid(bits, number)  ((bits >> number) & 1)
#define RandNum(min, max)      ((UINT8) (rand() % (max - min + 1) + min))

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

SUDOKU_API void UpdateNumber(PSUDOKU pSudoku, UINT8 num, int index);

SUDOKU_API UINT16 GetValidNumber(PSUDOKU pSudoku, int index);

SUDOKU_API void PrintSudoku(PSUDOKU pSudoku);



SUDOKU_API void GenerateSudoku(PSUDOKUPUZZLE pPuzzle);
// ����������clueNumָ����ʾ������
// �����ڶ��߳�ͬʱ����

SUDOKU_API void SolveSudoku(PSUDOKU pProblem);
// ���������ֻ���������һ����