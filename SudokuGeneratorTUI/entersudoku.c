#include <stdio.h>
#include <conio.h>
#include "sudoku.h"
#include "SudokuIO.h"

static inline int GetSingleSudoku(PSUDOKUPUZZLE puzzle);
static inline int GetSingleLine(PSUDOKUPUZZLE puzzle, int line);
static inline void Eatline(void);
static inline void CleanNumbers(PSUDOKU pSudoku, int line);

int GetPuzzleFromUser(PSUDOKUPUZZLE puzzles) {
	printf("请按行输入数独，用0代表空格，数字间用空格隔开；最多可输入%d个数独\n", IMPORTBUFFERLEN);
	int idx;
	for (idx = 0; idx < IMPORTBUFFERLEN; idx++) {
		printf("第%d个数独题目：\n", idx + 1);
		if (GetSingleSudoku(puzzles + idx)) {
			break;
		}
	}
	return idx + 1;
}

static inline int GetSingleSudoku(PSUDOKUPUZZLE puzzle) {
	SuInitialize(&puzzle->problem);
	for (int idx = 1; idx <= 9; idx++) {
		if (GetSingleLine(puzzle, idx)) {
			idx--;
		}
	}
	puzzle->clueNum = puzzle->problem.filledNum;
	puts("按c键继续输入，按其它任意键退出");
	while (1) {
		if (_kbhit()) {
			if (_getch() == 'c') {
				return 0;
			}
			else {
				return 1;
			}
		}
		Sleep(10);
	}
}

static inline int GetSingleLine(PSUDOKUPUZZLE puzzle, int line) {
	long long int number;
	printf("第%d行：", line);
	for (int idx = 1; idx <= 9; idx++) {
		if (scanf_s("%lld", &number) != 1) {
			puts("输入有误");
			goto error;
		}
		if (number < 0 || number > 9) {
			printf("%lld超出范围\n", number);
			goto error;
		}
		if (UpdateNumberSafe(&puzzle->problem, (UINT8)number, Position(line, idx))) {
			printf("%lld不满足数独条件\n", number);
			goto error;
		}
	}
	Eatline();
	return 0;

error:
	Eatline();
	CleanNumbers(&puzzle->problem, line);
	return 1;

}

static inline void Eatline(void) {
	while (getchar() != '\n');
}

static inline void CleanNumbers(PSUDOKU pSudoku, int line) {
	for (int idx = 1; idx <= 9; idx++) {
		UpdateNumber(pSudoku, 0, Position(line, idx));
	}
}