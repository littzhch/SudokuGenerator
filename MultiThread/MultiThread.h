#pragma once
#ifdef MULTITHREAD_EXPORTS
#define MULTITHREAD_API __declspec(dllexport)
#else
#define MULTITHREAD_API __declspec(dllimport)
#endif

#include "sudoku.h"

MULTITHREAD_API void GenerateSudokuMT(
	PSUDOKUPUZZLE pPuzzleStart,						// 生成的数独题目的存储空间首地址，以数组方式存储，不需要初始化
	int amount,										// 生成数独题目的数量
	int minClue,									// 提示数的最小值（包含）
	int maxClue, 									// 提示数的最大值（包含）
	int maxThread,									// 最大线程数，范围为[1, 64]，不检查
	void (*InfoProgress)(int current, int total));  // 进度显示回调函数
/* 以多线程方式生成数独题目
 * 在数独生成期间，InfoPrograss函数每间隔约0.3秒被调用一次，
 * 传入当前已经生成和正在生成的数独总数（current）和需要生成的总数独数量（total）。
 * 保证 current == total 的情况只在最后一次调用出现，且调用时所有数独全部生成完成。
 */