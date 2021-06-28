#include "SudokuIO.h"
#include "sudoku.h"
#include "MultiThread.h"
#include <Windows.h>

int main(void) {
	SUDOKUPUZZLE puzzles[5];
	GenerateSudokuMT(puzzles, 5, 25, 30, 1, NULL);
	FILE* file;
	fopen_s(&file, "document.xml", "w, ccs=UTF-8");
	WriteDocumentXml(puzzles, 5, file);
	fclose(file);
	return 0;
}
