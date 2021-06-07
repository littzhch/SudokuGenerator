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


int AddToRepository(PSUDOKUPUZZLE puzzles, int amount) {
	if (! HaveRepository()) {
		return -1;
	}
	FILE* file = NULL;
	fopen_s(&file, repoName, "ab+");
	if (!file) {
		return -1;
	}
	
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