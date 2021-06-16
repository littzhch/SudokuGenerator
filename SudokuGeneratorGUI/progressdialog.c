#include "progressdialog.h"

static BOOL CALLBACK DlgProc(HWND hwndDlg, UINT message, WPARAM wParam, LPARAM lParam);

static HWND hDlg;
extern HINSTANCE hWnd;

static int type;
static wchar_t* textstr;

void PopUpProgress(int wndType, wchar_t* text) {
	type = wndType;
	textstr = text;
	DialogBoxA(NULL, IDD_DIALOG2, hWnd, DlgProc);
}


void SetProgress(int current, int total) {
	current = 100 * current / total;
	total = 100;
	SendMessageW(hDlg, WM_UPDATEPROGRESS, current, total);
}

void StopProgress(void) {
	EndDialog(hDlg, 0);
}

static BOOL CALLBACK DlgProc(HWND hwndDlg, UINT message, WPARAM wParam, LPARAM lParam) {
	switch (message) {
	case WM_INITDIALOG:
		hDlg = hwndDlg;
		SetDlgItemTextW(hwndDlg, IDC_STATIC_TEXT, textstr);
		if (type == WNDTYPE_MARQUEE) {
			int style = GetWindowLongW(GetDlgItem(hwndDlg, IDC_PROGRESS1), GWL_STYLE);
			style |= PBS_MARQUEE;
			SetWindowLongW(GetDlgItem(hwndDlg, IDC_PROGRESS1), GWL_STYLE, style);
			SendDlgItemMessageW(hwndDlg, IDC_PROGRESS1, PBM_SETMARQUEE, TRUE, 30);
		}
		return TRUE;
	case WM_UPDATEPROGRESS:
		SendDlgItemMessageA(hDlg, IDC_PROGRESS1, PBM_SETPOS, wParam, 0);
		return TRUE;
	default:
		return FALSE;
	}
}


/*
char num[10] = "111";
wsprintfA(num, "%d", err);
MessageBoxA(NULL, num, "hi", MB_OK);
*/