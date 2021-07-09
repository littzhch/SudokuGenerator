#include <stdio.h>
#include "dialogs.h"
#include "multithread.h"
#include "resource.h"

BOOL CALLBACK generate(HWND hDlg, UINT msgType, WPARAM wParam, LPARAM lParam);

extern HWND hWnd;
extern HINSTANCE hIns;
static char lastAmount[32] = "1";
static char lastClueLeft[32] = "40";
static char lastClueRight[32] = "40";
static char lastThread[32] = "1";

static struct gInfo* pInfo;

void DLG_AskForSettings(struct gInfo* info) {
	pInfo = info;
	ZeroMemory(info, sizeof(struct gInfo));
	DialogBoxW(hIns, MAKEINTRESOURCEW(IDD_DIALOG3), hWnd, (DLGPROC)generate);
}


BOOL CALLBACK generate(HWND hDlg, UINT msgType, WPARAM wParam, LPARAM lParam) {
	switch (msgType) {
	case WM_INITDIALOG:
		SendDlgItemMessageA(hDlg, IDC_EDIT1, WM_SETTEXT, 0, (LPARAM)lastAmount);
		SendDlgItemMessageA(hDlg, IDC_EDIT2, WM_SETTEXT, 0, (LPARAM)lastClueLeft);
		SendDlgItemMessageA(hDlg, IDC_EDIT3, WM_SETTEXT, 0, (LPARAM)lastClueRight);
		SendDlgItemMessageA(hDlg, IDC_EDIT4, WM_SETTEXT, 0, (LPARAM)lastThread);
		return TRUE; 
	case WM_COMMAND:
		if (LOWORD(wParam) == BTOK || LOWORD(wParam) == BTCANCEL || LOWORD(wParam) == IDCANCEL) {
			*(WORD*)lastAmount = 32;
			*(WORD*)lastClueLeft = 32;
			*(WORD*)lastClueRight = 32;
			*(WORD*)lastThread = 32;
			SendDlgItemMessageA(hDlg, IDC_EDIT1, EM_GETLINE, 0, (LPARAM)lastAmount);
			SendDlgItemMessageA(hDlg, IDC_EDIT2, EM_GETLINE, 0, (LPARAM)lastClueLeft);
			SendDlgItemMessageA(hDlg, IDC_EDIT3, EM_GETLINE, 0, (LPARAM)lastClueRight);
			SendDlgItemMessageA(hDlg, IDC_EDIT4, EM_GETLINE, 0, (LPARAM)lastThread);
			switch (LOWORD(wParam)) {
			case BTOK:
				sscanf_s(lastAmount, "%d", &pInfo->num);
				sscanf_s(lastClueLeft, "%d", &pInfo->clue1);
				sscanf_s(lastClueRight, "%d", &pInfo->clue2);
				sscanf_s(lastThread, "%d", &pInfo->trd);
				if (pInfo->num == 0) {
					MessageBoxA(hDlg, "数独数量不能小于1", "错误", MB_ICONERROR | MB_OK);
					return TRUE;
				}
				if (pInfo->clue1 < 20 || pInfo->clue1 > 80 || pInfo->clue2 < 20 || pInfo->clue2 > 80) {
					MessageBoxA(hDlg, "数独提示数超出范围[20, 80]", "错误", MB_ICONERROR | MB_OK);
					return TRUE;
				}
				if (pInfo->trd == 0 || pInfo->trd > 64) {
					MessageBoxA(hDlg, "最大工作线程数超出范围[1, 64]", "错误", MB_ICONERROR | MB_OK);
					return TRUE;
				}
				if (pInfo->clue1 > pInfo->clue2) {
					pInfo->clue1 = pInfo->clue1 ^ pInfo->clue2;
					pInfo->clue2 = pInfo->clue1 ^ pInfo->clue2;
					pInfo->clue1 = pInfo->clue1 ^ pInfo->clue2;
				}
				EndDialog(hDlg, 0);
				break;
			case BTCANCEL:
			case IDCANCEL:
				pInfo->num = 0;
				EndDialog(hDlg, 0);
				break;
			}
		}
		return TRUE;
	}
	return FALSE;
}