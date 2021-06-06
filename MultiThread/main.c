#include <windows.h>
#include <process.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "sudoku.h"
#include "MultiThread.h"

static _Bool printInfo;
static PSUDOKUPUZZLE start;
static int puzzlePtr;
static int maxNum;
CRITICAL_SECTION csNext;
static char buffer[512];

static unsigned int _stdcall WorkThreadProc(void*);
static inline PSUDOKUPUZZLE GetNext(void);
static inline int PrintProgress(int current, int total);

void GenerateSudokuMT(
	PSUDOKUPUZZLE pPuzzleStart,
	int amount,
	int minClue,
	int maxClue,
	int maxThread,
	_Bool showInfo)
{
	srand(time(NULL));
	printInfo = showInfo;
	start = pPuzzleStart;
	puzzlePtr = 0;
	maxNum = amount;
	InitializeCriticalSection(&csNext);
	//TODO: 删除多余提示，使用回调函数显示进度

	if (showInfo) {
		printf("generating clue numbers...");
	}
	for (int idx = 0; idx < amount; idx++) {
		(pPuzzleStart + idx)->clueNum = RandNum(minClue, maxClue);
	}
	if (showInfo) {
		puts("Done\ncreating threads");
	}
	HANDLE* threadHds = malloc(sizeof(HANDLE) * maxThread);
	unsigned int threadId;
	for (int idx = 0; idx < maxThread; idx++) {
		threadHds[idx] = _beginthreadex(NULL, 0, WorkThreadProc, NULL, 0, &threadId);
		if (showInfo) {
			printf("thread 0x%04x created\n", threadId);
		}
	}

	if (showInfo) {
		puts("Done\nProgress:");
		while (PrintProgress(puzzlePtr, amount))
			Sleep(300);
	}

	WaitForMultipleObjects(maxThread, threadHds, TRUE, INFINITE);
	for (int idx = 0; idx < maxThread; idx++) {
		CloseHandle(threadHds[idx]);
	}
	free(threadHds);
	DeleteCriticalSection(&csNext);
	if (showInfo) {
		puts("Generation finished");
	}
}


unsigned int _stdcall WorkThreadProc(void* arg) {  //TODO: 添加生成时间限制
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

static inline int PrintProgress(int current, int total) {
	HANDLE screen = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_SCREEN_BUFFER_INFO screenInfo;
	GetConsoleScreenBufferInfo(screen, &screenInfo);
	int maxchar = screenInfo.dwSize.X;
	int maxbar = maxchar * 2 / 3;
	int finished = maxbar * current / total;
	int unfinished = maxbar - finished;
	int bufferPtr = 0;

	buffer[bufferPtr++] = '\r';
	buffer[bufferPtr++] = '[';
	while (finished--) {
		buffer[bufferPtr++] = '#';
	}
	while (unfinished--) {
		buffer[bufferPtr++] = '-';
	}
	buffer[bufferPtr++] = ']';
	buffer[bufferPtr++] = ' ';
	sprintf_s(buffer + bufferPtr, 5, "%3d", 100 * current / total);
	bufferPtr += 3;
	buffer[bufferPtr++] = '%';
	buffer[bufferPtr] = '\0';

	fputs(buffer, stdout);

	if (current >= total) {
		putchar('\n');
		return 0;
	}
	return 1;
}