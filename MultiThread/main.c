#include <windows.h>
#include <process.h>
#include <stdlib.h>
#include <stdio.h>
#include "sudoku.h"
#include "MultiThread.h"

static _Bool printInfo;
static PSUDOKUPUZZLE start;
static int puzzlePtr;
static int maxNum;
CRITICAL_SECTION csNext;

static unsigned int _stdcall WorkThreadProc(void *);
static PSUDOKUPUZZLE GetNext(void);

void GenerateSudokuMT(
	PSUDOKUPUZZLE pPuzzleStart,
	int amount,
	int minClue,
	int maxClue,
	int maxThread,
	_Bool showInfo)
{
	printInfo = showInfo;
	start = pPuzzleStart;
	puzzlePtr = 0;
	maxNum = amount;
	InitializeCriticalSection(&csNext);

	for (int idx = 0; idx < amount; idx++) {
		(pPuzzleStart + idx)->clueNum = RandNum(minClue, maxClue);
	}
	if (maxThread > amount) {
		maxThread = amount;
	}


	HANDLE* threadHds = malloc(sizeof(HANDLE) * maxThread);
	for (int idx = 0; idx < maxThread; idx++) {
		threadHds[idx] = _beginthreadex(NULL, 0, WorkThreadProc, NULL, 0, NULL);
	}
	WaitForMultipleObjects(maxThread, threadHds, TRUE, INFINITE);


	for (int idx = 0; idx < maxThread; idx++) {
		CloseHandle(threadHds[idx]);
	}
	free(threadHds);
	DeleteCriticalSection(&csNext);
}


unsigned int _stdcall WorkThreadProc(void* arg) {
	srand(GetCurrentThreadId());
	PSUDOKUPUZZLE pPuzzle;
	while ((pPuzzle = GetNext())) {
		GenerateSudoku(pPuzzle);
	}
	return 0;
}

static PSUDOKUPUZZLE GetNext(void) {
	if (puzzlePtr >= maxNum) {
		return NULL;
	}

	PSUDOKUPUZZLE result;
	EnterCriticalSection(&csNext);
	result = start + puzzlePtr++;
	LeaveCriticalSection(&csNext);
	return result;
}