#include <stdio.h>
#include <conio.h>
#include "sudoku.h"
#include "SudokuIO.h"

static inline int GetSingleSudoku(PSUDOKUPUZZLE puzzle);
static inline int GetSingleLine(PSUDOKUPUZZLE puzzle, int line);
static inline void Eatline(void);
static inline void CleanNumbers(PSUDOKU pSudoku, int line);

int GetPuzzleFromUser(PSUDOKUPUZZLE puzzles) {
	printf("�밴��������������0����ո����ּ��ÿո��������������%d������\n", IMPORTBUFFERLEN);
	int idx;
	for (idx = 0; idx < IMPORTBUFFERLEN; idx++) {
		printf("��%d��������Ŀ��\n", idx + 1);
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
	puts("��c���������룬������������˳�");
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
	printf("��%d�У�", line);
	for (int idx = 1; idx <= 9; idx++) {
		if (scanf_s("%lld", &number) != 1) {
			puts("��������");
			goto error;
		}
		if (number < 0 || number > 9) {
			printf("%lld������Χ\n", number);
			goto error;
		}
		if (UpdateNumberSafe(&puzzle->problem, (UINT8)number, Position(line, idx))) {
			printf("%lld��������������\n", number);
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