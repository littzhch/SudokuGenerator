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
// 初始化模块，在调用其它函数前必须调用


SUDOKUIO_API int SetupRepository(void);
// 创建存储库，若成功，返回0；若已存在存储库，返回1；若失败，返回-1

SUDOKUIO_API int CleanRepository(void);
// 清除存储库。若成功，返回0；若无存储库，创建新的存储库，返回1；若失败，返回-1

SUDOKUIO_API int GetPuzzleAmountInRepository(void);
// 获取存储库中的数独题目数量。若无存储库或存储库打开失败，返回-1

SUDOKUIO_API int AddToRepository(const PSUDOKUPUZZLE puzzles, int amount);
// 向存储库中添加数独。若成功，返回0；若无存储库或存储库打开失败，返回-1



SUDOKUIO_API int ExportRepoAsJson(const char * filepath);
// 导出为json格式。正常返回0
// 若无法打开存储库，返回-1；若无法打开目标文件，返回-2；若存储库中数独数量为0，返回-3；

SUDOKUIO_API int ImportPuzzleFromJson(PSUDOKUPUZZLE puzzles, const char * filepath);
// 从json文件中导入数独题目。返回写入puzzles中的数独题目个数；若个数等于IMPORTBUFFERLEN，
// 则可以继续导入，若个数小于IMPORTBUFFERLEN，则说明已经读取完成；若返回-1，说明无法打开文件


SUDOKUIO_API UINT32 Crc32(FILE* file);