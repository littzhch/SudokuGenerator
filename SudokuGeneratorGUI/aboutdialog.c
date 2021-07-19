#include "dialogs.h"
#include "resource.h"

extern HINSTANCE hIns;
extern HWND hWnd;
static BOOL CALLBACK about(HWND hDlg, UINT msgType, WPARAM wParam, LPARAM lParam);
static inline char* GetSelfVersion(void);

void DLG_ShowAboutWindow(void) {
	DialogBoxA(hIns, (LPCSTR) IDD_DIALOG1, hWnd, (DLGPROC) about);
}

static BOOL CALLBACK about(HWND hDlg, UINT msgType, WPARAM wParam, LPARAM lParam) {
	switch (msgType) {
	case WM_INITDIALOG:
		SendDlgItemMessageW(hDlg, IDC_PICT, STM_SETIMAGE, IMAGE_ICON, 
			(LPARAM) LoadIconA(hIns, MAKEINTRESOURCEA(IDI_ICON1)));
		SetDlgItemTextA(hDlg, IDC_TEXT, GetSelfVersion());
		return TRUE;
	case WM_COMMAND:
		if (LOWORD(wParam) == IDC_BUTTON_REPORT) {
			ShellExecuteA(hDlg, "open", "explorer.exe", 
				"https://github.com/littzhch/SudokuGenerator", NULL, SW_SHOW);
			return TRUE;
		}
		EndDialog(hDlg, 0);
		return TRUE;
	}
	return FALSE;
}


static inline char* GetSelfVersion(void) {
	static char* selfpath[MAX_PATH] = { 0 };
	static char* result[44];

	GetModuleFileNameA(NULL, (LPSTR) selfpath, MAX_PATH);
	UINT16 version[4] = { 0, 0, 0, 0 };
	DWORD useless;
	DWORD size = GetFileVersionInfoSizeA((LPCSTR) selfpath, (LPDWORD) &useless);
	if (size) {
		LPVOID pdata = malloc(size);
		if (pdata != 0) {
			if (GetFileVersionInfoA((LPCSTR)selfpath, (DWORD)0, (DWORD)size, (LPVOID)pdata)) {
				VS_FIXEDFILEINFO* pInfo;
				UINT len;
				if (VerQueryValueA(pdata, "\\", &pInfo, &len)) {
					*((UINT32*)version) = pInfo->dwProductVersionMS;
					*((UINT32*)(version + 2)) = pInfo->dwProductVersionLS;
				}
			}
		}
		free(pdata);
	}
	sprintf_s((char* const)result, 44, "%d.%d.%d.%d", version[1], version[0], version[3], version[2]); //×Ö½ÚÐò
	return (char*)result;
}


