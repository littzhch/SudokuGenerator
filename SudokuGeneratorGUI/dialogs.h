#pragma once
#include <windows.h>
#include <shobjidl.h>
#include <CommCtrl.h>
#include <process.h>
#include <stdio.h>
#include "sudoku.h"


/* aboutdialog.c */
void DLG_ShowAboutWindow(void);
// չʾ ���� ����


/* enterdialog.c */
int DLG_GetSudokuProblem(PSUDOKUPUZZLE pPuzzle);
// ��ȡ�û����롣 ���ɹ����ط���ֵ�����û��˳�����0


/* fileselect.c */
void DLG_InitFileService(void);
// ��ʼ��������������֮ǰ����

void DLG_UninitFileService(void);
// �������������ǰ����

void DLG_GetOpenFilePath(PWSTR* path);
// ��ȡ��Ҫ�򿪵��ļ���·�������û��رմ��ڣ�pathΪNULL

int DLG_GetWriteFilePath(PWSTR* path);
// ��ȡ��Ҫ�������ļ�������·����������׺�������ļ�����
// ��Ϊdocx��ʽ������1
// ��Ϊjson��ʽ������2
// ���û��رմ��ڣ�pathΪNULL, ����ֵΪ0


/* generatedialog.c */
struct gInfo {
	int num;
	int clue1; // ��֤ clue1 <= clue2
	int clue2;
	int trd;
}; // used by DLG_AskForSettings()

void DLG_AskForSettings(struct gInfo* info);
// ѯ�����ã���info->num == 0��������û�ȡ��������


/* progressdialog.c */
#define WNDTYPE_NORMAL                0
#define WNDTYPE_MARQUEE               1
#define WM_UPDATEPROGRESS  (WM_USER + 1)

void DLG_PopUpProgress(int wndType, wchar_t* leftText);
// �������ȴ���

void DLG_SetProgress(int current, int total);
// ���õ�ǰ����

void DLG_ChangeTextWL(wchar_t* text);
// �ı���ȴ����е��������

void DLG_ChangeTextWR(wchar_t* text);
// �ı���ȴ����е��Ҳ�����

void DLG_StopProgress(void);
// �رս��ȴ���