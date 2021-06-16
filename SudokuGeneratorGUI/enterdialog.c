#include "enterdialog.h"
#include "resource.h"

extern HINSTANCE hIns;
extern HWND hWnd;

static BOOL CALLBACK enter(HWND hwndDlg, UINT message, WPARAM wParam, LPARAM lParam);

static char lines[9][16];
static PSUDOKUPUZZLE puzzle;

int GetSudokuProblem(PSUDOKUPUZZLE pPuzzle) {
	puzzle = pPuzzle;
	return DialogBoxA(hIns, IDD_DIALOG4, hWnd, enter);
}

static BOOL CALLBACK enter(HWND hwndDlg, UINT message, WPARAM wParam, LPARAM lParam) {
	switch (message) {
	case WM_INITDIALOG:
		return TRUE;
	case WM_COMMAND:
		switch (LOWORD(wParam)) {
		case BTOK:
			for (int i = 0; i < 9; i++) {
				*(WORD*)lines[i] = 16;
			}
			SendDlgItemMessageA(hwndDlg, IDC_EDIT_L1, EM_GETLINE, 0, lines[0]);
			SendDlgItemMessageA(hwndDlg, IDC_EDIT_L2, EM_GETLINE, 0, lines[1]);
			SendDlgItemMessageA(hwndDlg, IDC_EDIT_L3, EM_GETLINE, 0, lines[2]);
			SendDlgItemMessageA(hwndDlg, IDC_EDIT_L4, EM_GETLINE, 0, lines[3]);
			SendDlgItemMessageA(hwndDlg, IDC_EDIT_L5, EM_GETLINE, 0, lines[4]);
			SendDlgItemMessageA(hwndDlg, IDC_EDIT_L6, EM_GETLINE, 0, lines[5]);
			SendDlgItemMessageA(hwndDlg, IDC_EDIT_L7, EM_GETLINE, 0, lines[6]);
			SendDlgItemMessageA(hwndDlg, IDC_EDIT_L8, EM_GETLINE, 0, lines[7]);
			SendDlgItemMessageA(hwndDlg, IDC_EDIT_L9, EM_GETLINE, 0, lines[8]);
			
			for (int line = 0; line < 9; line++) {
				for (int c = 0; c < 9; c++) {
					if (lines[line][c] == '\0') {
						MessageBoxA(hwndDlg, "输入的数字过少", "错误", MB_ICONERROR | MB_OK);
						return TRUE;
					}
					UpdateNumber(&puzzle->problem, (UINT8)(lines[line][c] - '0'), Position(line + 1, c + 1));
				}
			}
			puzzle->clueNum = puzzle->problem.filledNum;
			EndDialog(hwndDlg, 1);
			break;
		case BTCANCEL:
		case IDCANCEL:
			EndDialog(hwndDlg, 0);
			break;
		}
		return TRUE;
	}
	return FALSE;
}