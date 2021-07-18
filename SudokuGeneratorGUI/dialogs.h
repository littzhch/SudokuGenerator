#pragma once
#include <windows.h>
#include <shobjidl.h>
#include <CommCtrl.h>
#include <process.h>
#include <stdio.h>
#include "sudoku.h"


/* aboutdialog.c */
void DLG_ShowAboutWindow(void);
// 展示 关于 窗口


/* enterdialog.c */
int DLG_GetSudokuProblem(PSUDOKUPUZZLE pPuzzle);
// 获取用户输入。 若成功返回非零值；若用户退出返回0


/* fileselect.c */
void DLG_InitFileService(void);
// 初始化，在其它函数之前调用

void DLG_UninitFileService(void);
// 结束，程序结束前调用

void DLG_GetOpenFilePath(PWSTR* path);
// 获取需要打开的文件的路径，若用户关闭窗口，path为NULL

int DLG_GetWriteFilePath(PWSTR* path);
// 获取需要创建的文件的完整路径（不带后缀名）和文件类型
// 若为docx格式，返回1
// 若为json格式，返回2
// 若用户关闭窗口，path为NULL, 返回值为0


/* generatedialog.c */
struct gInfo {
	int num;
	int clue1; // 保证 clue1 <= clue2
	int clue2;
	int trd;
}; // used by DLG_AskForSettings()

void DLG_AskForSettings(struct gInfo* info);
// 询问设置，若info->num == 0，则代表用户取消了设置


/* progressdialog.c */
#define WNDTYPE_NORMAL                0
#define WNDTYPE_MARQUEE               1
#define WM_UPDATEPROGRESS  (WM_USER + 1)

void DLG_PopUpProgress(int wndType, wchar_t* leftText);
// 弹出进度窗口

void DLG_SetProgress(int current, int total);
// 设置当前进度

void DLG_ChangeTextWL(wchar_t* text);
// 改变进度窗口中的左侧文字

void DLG_ChangeTextWR(wchar_t* text);
// 改变进度窗口中的右侧文字

void DLG_StopProgress(void);
// 关闭进度窗口