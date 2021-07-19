#include <Windows.h>
#include "SudokuIO.h"
#include "sudoku.h"

int main(int argc, char* argv[]) {
	IOInit();
	printf("%d\n", ExportRepoAsDocx("test.docx"));
}
