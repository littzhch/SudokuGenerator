#pragma once
#include <windows.h>
#include <CommCtrl.h>
#include <process.h>
#include "resource.h"

#define WNDTYPE_NORMAL  0
#define WNDTYPE_MARQUEE 1

#define WM_UPDATEPROGRESS (WM_USER + 1)

void PopUpProgress(int wndType, wchar_t* text);
// �������ȴ���

void SetProgress(int current, int total);
// ���õ�ǰ���ȣ��� current == total ʱ�Զ��ر�

void ChangeTextW(wchar_t* text);

void StopProgress(void);