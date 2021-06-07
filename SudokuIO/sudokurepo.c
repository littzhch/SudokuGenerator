#include <stdio.h>
#include "SudokuIO.h"


static char* repoName = "sudoku.repo";
static inline int HaveRepository(void);

int SetupRepository(void) {
	if (HaveRepository()) {
		return 1;
	}
	FILE* file = NULL;
	fopen_s(&file, repoName, "wb");
	if (!file) {
		return -1;
	}
	int buffer = 0;
	fwrite(&buffer, sizeof(int), 1, file);
	fclose(file);
	return 0;
}


int CleanRepository(void) {
	int code = SetupRepository();
	if (code == 0) {
		return 1;
	}
	else if (code == -1){
		return -1;
	}
	
	FILE* file = NULL;
	fopen_s(&file, repoName, "wb");
	if (!file) {
		return -1;
	}
	int buffer = 0;
	fwrite(&buffer, sizeof(int), 1, file);
	fclose(file);
	return 0;
}


int GetPuzzleAmountInRepository(void) {
	FILE* file = NULL;
	fopen_s(&file, repoName, "rb");
	if (!file) {
		return -1;
	}
	int result;
	fread(&result, sizeof(int), 1, file);
	fclose(file);
	return result;
}

//TODO: 确定存储上限并加以限制，选择合适的数量类型
int AddToRepository(const PSUDOKUPUZZLE puzzles, int amount) {  //TODO: 使用更高效的存储方式
	FILE* file = NULL;
	fopen_s(&file, repoName, "rb+");
	if (!file) {
		return -1;
	}
	int number;
	fread(&number, sizeof(int), 1, file);
	number += amount;
	rewind(file);
	fwrite(&number, sizeof(int), 1, file);
	if (fseek(file, 0, SEEK_END)) {
		return -1;
	}
	fwrite(puzzles, sizeof(SUDOKUPUZZLE), amount, file);
	fclose(file);
	return 0;
}


static inline int HaveRepository(void) {
	FILE* file = NULL;
	fopen_s(&file, repoName, "rb");
	if (file) {
		fclose(file);
		return 1;
	}
	return 0;
}