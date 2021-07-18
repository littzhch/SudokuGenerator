#include <windows.h>
#include <process.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "sudoku.h"
#include "MultiThread.h"
//TODO: 将multiThread合并入sudoku
static PSUDOKUPUZZLE start;
static int puzzlePtr;
static int maxNum;
CRITICAL_SECTION csNext;

static unsigned int _stdcall WorkThreadProc(void*);
static inline PSUDOKUPUZZLE GetNext(void);

void GenerateSudokuMT(
	PSUDOKUPUZZLE pPuzzleStart,
	int amount,
	int minClue,
	int maxClue,
	int maxThread,
	void (*InfoProgress)(int current, int total))
{
	start = pPuzzleStart;
	puzzlePtr = 0;
	maxNum = amount;
	InitializeCriticalSection(&csNext);
	srand((unsigned int)time(NULL));

	for (int idx = 0; idx < amount; idx++) {
		(pPuzzleStart + idx)->clueNum = RandNum(minClue, maxClue);
	}
	HANDLE* threadHds = malloc(sizeof(HANDLE) * maxThread);
	if (threadHds != 0) {
		ZeroMemory(threadHds, sizeof(HANDLE) * maxThread);
		for (int idx = 0; idx < maxThread; idx++) {
			threadHds[idx] = (HANDLE)_beginthreadex(NULL, 0, WorkThreadProc, NULL, 0, NULL);
		}

		if (InfoProgress) {
			while (puzzlePtr < amount) {
				InfoProgress(puzzlePtr, amount);
				Sleep(300);
			}
		}
		WaitForMultipleObjects(maxThread, threadHds, TRUE, INFINITE);
		if (InfoProgress) {
			InfoProgress(amount, amount);
		}

		for (int idx = 0; idx < maxThread; idx++) {
			if (threadHds[idx] != 0) {
				CloseHandle(threadHds[idx]);
			}
		}
		free(threadHds);
	}
	DeleteCriticalSection(&csNext);
}


unsigned int _stdcall WorkThreadProc(void* arg) {  //TRY: 添加生成时间限制
	srand(GetCurrentThreadId());
	PSUDOKUPUZZLE pPuzzle;
	while ((pPuzzle = GetNext())) {
		GenerateSudoku(pPuzzle);
	}
	return 0;
}

static inline PSUDOKUPUZZLE GetNext(void) {
	if (puzzlePtr >= maxNum) {
		return NULL;
	}
	PSUDOKUPUZZLE result;
	EnterCriticalSection(&csNext);
	result = start + puzzlePtr++;
	LeaveCriticalSection(&csNext);
	return result;
}