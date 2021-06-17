#pragma once
#include <windows.h>
#include <CommCtrl.h>
#include <process.h>
#include "resource.h"

#define WNDTYPE_NORMAL  0
#define WNDTYPE_MARQUEE 1

#define WM_UPDATEPROGRESS (WM_USER + 1)

void PopUpProgress(int wndType, wchar_t* text);
// 弹出进度窗口

void SetProgress(int current, int total);
// 设置当前进度，当 current == total 时自动关闭

void ChangeTextW(wchar_t* text);

void StopProgress(void);