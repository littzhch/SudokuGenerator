#pragma once
#ifdef SUDOKU_EXPORTS
#define SUDOKU_API __declspec(dllexport)
#else
#define SUDOKU_API __declspec(dllimport)
#endif

#include <windows.h>

#define Position(row, col)     (9 * (row)  + (col) - 10)		// ������������ת��Ϊλ������ֵ
#define GetRow(idx)			   ((idx) / 9 + 1)					// 1-9
#define GetCol(idx)			   ((idx) % 9 + 1)					// 1-9
#define GetBlock(idx)          (((GetRow(idx) - 1) / 3) * 3 + ((GetCol(idx) - 1) / 3) + 1) // 1-9
#define IsValid(bits, number)  (((bits) >> (number)) & 1)		// bitsΪGetValidNumber()�ķ���ֵ��numberΪ��Ҫ���������
#define RandNum(min, max)      ((UINT8) (rand() % ((max) - (min) + 1) + (min)))	// ����[min, max]�����ڵ�һ���������

typedef struct sudoku {
	UINT8 elements[81];
	UINT16 rowValids[9];
	UINT16 colValids[9];
	UINT16 blkValids[9];
	int filledNum;
} SUDOKU, *PSUDOKU;

typedef struct {
	SUDOKU problem;
	SUDOKU answer;
	int clueNum;
} SUDOKUPUZZLE, * PSUDOKUPUZZLE;

SUDOKU_API void SuInitialize(PSUDOKU pSudoku);
// ��ʼ��SUDOKU��ʹ���Ϊһ���յ�������

SUDOKU_API void UpdateNumber(PSUDOKU pSudoku, UINT8 num, int index);
// ��pSudoku��������Ƴ�����
// ֻӦ�����������������������֣������
// num Ϊ0~9������0�����Ƴ�����
// indexΪλ��������0~80��Ҳ����Position()�������������������
// num �� index �ķ�Χ�����

SUDOKU_API int UpdateNumberSafe(PSUDOKU pSudoku, UINT8 num, int index);
// ��pSudoku��������Ƴ�������������������
// ������������������������0
// �����ֲ��������������ı�Դ���������ط���ֵ
// num Ϊ0~9������0�����Ƴ�����
// indexΪλ��������0~80��Ҳ����Position()�������������������
// num �� index �ķ�Χ�����

SUDOKU_API UINT16 GetValidNumber(PSUDOKU pSudoku, int index);
// ��ȡindexλ�ÿ����������
// ����ֵ����IsValid()�����


SUDOKU_API void GenerateSudoku(PSUDOKUPUZZLE pPuzzle);
// ����������clueNumָ����ʾ��������problem��answer����Ҫ��ʼ��
// �����뺯���������ڶ��߳�ͬʱ����

SUDOKU_API int SolveSudoku(PSUDOKUPUZZLE pPuzzle);
// ���������ֻ���������һ����
// pPuzzle->problem ��Ҫ�� ֻ�ұ��� ʹ��SuInitialize()��UpdateNumber()��������Ŀ
// pPuzzle->answer ��Ҫ��
// ������clueNum
// ���ɹ�����⣬����0�����޽⣬����-1


SUDOKU_API void GenerateSudokuMT(
	PSUDOKUPUZZLE pPuzzleStart,						// ���ɵ�������Ŀ�Ĵ洢�ռ��׵�ַ�������鷽ʽ�洢������Ҫ��ʼ��
	int amount,										// ����������Ŀ������
	int minClue,									// ��ʾ������Сֵ��������
	int maxClue, 									// ��ʾ�������ֵ��������
	int maxThread,									// ����߳�������ΧΪ[1, 64]�������
	void (*InfoProgress)(int current, int total));  // ������ʾ�ص�����
/* �Զ��̷߳�ʽ����������Ŀ
 * �����������ڼ䣬InfoPrograss����ÿ���Լ0.3�뱻����һ�Σ�
 * ���뵱ǰ�Ѿ����ɺ��������ɵ�����������current������Ҫ���ɵ�������������total����
 * ��֤ current == total �����ֻ�����һ�ε��ó��֣��ҵ���ʱ��������ȫ��������ɡ�
 */