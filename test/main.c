#include "SudokuIO.h"
#include "MultiThread.h"
#include <Windows.h>

int main(int argc, char* argv[]) {
	IOInit();
	printf("%d\n", ExportRepoAsDocx("test.docx"));
}
