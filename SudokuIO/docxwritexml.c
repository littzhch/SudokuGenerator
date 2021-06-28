#include <stdio.h>
#include "sudoku.h"


static const char* xmlHeader = "header";
static const char* xmlEnd = "end";
static const char* tableEnd = "tableEnd";

static inline void WriteXmlTable(const PSUDOKU pSudoku, _Bool isAnswer, int puzzleNumber, FILE* file);

static inline void WriteTableHeader(int puzzleNumber, _Bool isAnswer, FILE* file);
static inline void WriteTableCell(int cellIdx, int cellNum, FILE* file);

void WriteDocumentXml(const PSUDOKUPUZZLE pPuzzles, int puzzleNum, FILE* file) {
	fputs(xmlHeader, file);
	for (int idx = 0; idx < puzzleNum; idx++) {
		WriteXmlTable(&((pPuzzles + idx)->problem), (_Bool)0, idx + 1, file);
	}
	for (int idx = 0; idx < puzzleNum; idx++) {
		WriteXmlTable(&((pPuzzles + idx)->answer), (_Bool)1, idx + 1, file);
	}
	fputs(xmlEnd, file);
}

static inline void WriteXmlTable(const PSUDOKU pSudoku, _Bool isAnswer, int puzzleNumber, FILE* file) {
	WriteTableHeader(puzzleNumber, isAnswer, file);
	for (int idx = 0; idx < 81; idx++) {
		WriteTableCell(idx, (int)pSudoku->elements[idx], file);
	}
	fputs(tableEnd, file);
}

static inline void WriteTableHeader(int puzzleNumber, _Bool isAnswer, FILE* file) {

}

static inline void WriteTableCell(int cellIdx, int cellNum, FILE* file) {

}