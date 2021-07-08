#include <stdio.h>
#include <conio.h>
#include "sudoku.h"
#include "SudokuIO.h"

static inline int GetSingleSudoku(PSUDOKUPUZZLE puzzle);
static inline int GetSingleLine(PSUDOKUPUZZLE puzzle, int line);
static inline void Eatline(void);

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
	SuInitialize(puzzle);
	for (int idx = 1; idx <= 9; idx++) {
		if (GetSingleLine(puzzle, idx)) {
			idx--;
		}
	}
	puzzle->clueNum = puzzle->problem.filledNum;
	puts("按q键退出，按其它任意键继续输入");
	while (1) {
		if (_kbhit()) {
			if (_getch() == 'q') {
				return 1;
			}
			else {
				return 0;
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
			Eatline();
			return 1;
		}
		if (number < 0 || number > 9) {
			printf("%lld超出范围\n", number);
			Eatline();
			return 1;
		}
		UpdateNumber(&puzzle->problem, (UINT8) number, Position(line, idx));
	}
	Eatline();
	return 0;
}


static inline void Eatline(void) {
	while (getchar() != '\n')
		;
}