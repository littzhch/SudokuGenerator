#pragma once
#ifdef SUDOKUIO_EXPORTS
#define SUDOKUIO_API __declspec(dllexport)
#else
#define SUDOKUIO_API __declspec(dllimport)
#endif

#include "sudoku.h"