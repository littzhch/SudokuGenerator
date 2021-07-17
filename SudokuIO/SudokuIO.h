#pragma once
#ifdef SUDOKUIO_EXPORTS
#define SUDOKUIO_API __declspec(dllexport)
#else
#define SUDOKUIO_API __declspec(dllimport)
#endif

#include <stdio.h>
#include "sudoku.h"
#define REPONAME "sudoku.repo"
#define IMPORTBUFFERLEN 64

SUDOKUIO_API void IOInit(void);
// ��ʼ��ģ�飬�ڵ�����������ǰ�������


SUDOKUIO_API int SetupRepository(void);
// �����洢�⣬���ɹ�������0�����Ѵ��ڴ洢�⣬����1����ʧ�ܣ�����-1

SUDOKUIO_API int CleanRepository(void);
// ����洢�⡣���ɹ�������0�����޴洢�⣬�����µĴ洢�⣬����1����ʧ�ܣ�����-1

SUDOKUIO_API int GetPuzzleAmountInRepository(void);
// ��ȡ�洢���е�������Ŀ���������޴洢���洢���ʧ�ܣ�����-1

SUDOKUIO_API int AddToRepository(const PSUDOKUPUZZLE puzzles, int amount);
// ��洢����������������ɹ�������0�����޴洢���洢���ʧ�ܣ�����-1



SUDOKUIO_API int ExportRepoAsJson(const char * filepath);
// ����Ϊjson��ʽ����������0
// ���޷��򿪴洢�⣬����-1�����޷���Ŀ���ļ�������-2�����洢������������Ϊ0������-3��

SUDOKUIO_API int ImportPuzzleFromJson(PSUDOKUPUZZLE puzzles, const char * filepath);
// ��json�ļ��е���������Ŀ������д��puzzles�е�������Ŀ����������������IMPORTBUFFERLEN��
// ����Լ������룬������С��IMPORTBUFFERLEN����˵���Ѿ���ȡ��ɣ�������-1��˵���޷����ļ�


SUDOKUIO_API UINT32 Crc32(FILE* file);