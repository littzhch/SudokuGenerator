#include "SudokuIO.h"
#include "MultiThread.h"
#include <Windows.h>

int main(int argc, char* argv[]) {
	FILE* file;
	fopen_s(&file, argv[1], "rb");
	printf("%08X", Crc32(file));
	return 0;
}
