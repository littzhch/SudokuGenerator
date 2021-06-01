#include <stdio.h>
#include "sudoku.h"

int main(void) {
	SUDOKU sudoku;
	SuInitialize(&sudoku);
	UpdateNumber(&sudoku, 2, Position(1, 1));
	UpdateNumber(&sudoku, 9, Position(3, 3));
	UpdateNumber(&sudoku, 3, Position(8, 2));
	UpdateNumber(&sudoku, 5, Position(1, 7));
	int a = sudoku.elements[6];
	UINT16 rst;
	rst = GetValidNumber(&sudoku, Position(1, 2));
	printf("1 -%d\n", IsValid(rst, 1));
	printf("2 -%d\n", IsValid(rst, 2));
	printf("3 -%d\n", IsValid(rst, 3));
	printf("4 -%d\n", IsValid(rst, 4));
	printf("5 -%d\n", IsValid(rst, 5));
	printf("6 -%d\n", IsValid(rst, 6));
	printf("7 -%d\n", IsValid(rst, 7));
	printf("8 -%d\n", IsValid(rst, 8));
	printf("9 -%d\n", IsValid(rst, 9));
}