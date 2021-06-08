#pragma once
#ifdef MULTITHREAD_EXPORTS
#define MULTITHREAD_API __declspec(dllexport)
#else
#define MULTITHREAD_API __declspec(dllimport)
#endif

#include "sudoku.h"

MULTITHREAD_API void GenerateSudokuMT(
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