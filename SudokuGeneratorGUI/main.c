#include "progressdialog.h"
#include "fileselect.h"
#include "generatedialog.h"
#include "aboutdialog.h"
#include "enterdialog.h"
#include "resource.h"
#include "SudokuIO.h"
#include "multithread.h"
#include <windowsx.h>
#include <stdio.h>
#include <process.h>

#pragma comment(linker,"\"/manifestdependency:type='win32' \
name='Microsoft.Windows.Common-Controls' version='6.0.0.0' \
processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")


HINSTANCE hIns;
HWND hWnd;

static HWND hPicture;
static HWND hText;

static inline void FillRects(RECT* result);
static inline void SetupMainWindowContent(void);
static inline void RegisterWindowClass(void);
static inline void UpdateChildPos(void);
static void threadProc(void* arg);
static inline void UpdateRepoNum(void);

static LRESULT CALLBACK MainWndProc(HWND hwnd, UINT msgType, WPARAM wParam, LPARAM lParam);


int WINAPI wWinMain(
	HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	PWSTR szCmdLine,
	int iCmdShow) 
{
	RECT rect;
	hIns = hInstance;
	InitFileService();
	IOInit();
	RegisterWindowClass();
	FillRects(&rect);

	hWnd = CreateWindowExW(
		WS_EX_ACCEPTFILES,
		L"MainWindowClass",
		L"SudokuGenerator 数独生成器",
		WS_OVERLAPPEDWINDOW,
		rect.left,
		rect.top,
		rect.right - rect.left,
		rect.bottom - rect.top,
		NULL,
		LoadMenuW(hIns, MAKEINTRESOURCE(IDR_MENU1)),
		hInstance,
		NULL
	);
	SetupMainWindowContent();
	ShowWindow(hWnd, iCmdShow);

	MSG msg;
	while (GetMessageW(&msg, NULL, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessageW(&msg);
	}
	UninitFileService();
	return msg.wParam;
}

static LRESULT CALLBACK MainWndProc(HWND hwnd, UINT msgType, WPARAM wParam, LPARAM lParam) {
	HDC hdc;
	PAINTSTRUCT ps;
	struct gInfo info;

	switch (msgType) {
	case WM_CTLCOLORSTATIC:
		return (HBRUSH)(COLOR_WINDOW + 1);
	case WM_COMMAND:
		switch(LOWORD(wParam)) {
		case ID_MENU_ABOUT:
			ShowAboutWindow();
			break;
		case ID_MENU_GENERATE:
			AskForSettings(&info);
			if (info.num) {
				_beginthread(threadProc, 0, &info);
				PopUpProgress(WNDTYPE_NORMAL, L"正在生成...");
			}
			break;
		case ID_MENU_DEL:
			if (GetPuzzleAmountInRepository() == 0) {
				MessageBoxA(hWnd, "题目数量为0", "错误",
					MB_OKCANCEL | MB_ICONERROR);
				break;
			}
			if (MessageBoxA(hWnd, "所有题目将被清空，确定要继续吗？", "警告", 
				MB_OKCANCEL | MB_ICONWARNING) == IDOK) {
				CleanRepository();
				UpdateRepoNum();
			}
			break;
		case ID_MENU_ENTER:
			SUDOKUPUZZLE sp;
			if (GetSudokuProblem(&sp)) {
				MessageBoxA(NULL, "accept", "info", MB_OK);
			}
			else {
				MessageBoxA(NULL, "quited", "info", MB_OK);
			}
			break;
		}
		return 0;
	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);
		FillRect(hdc, &ps.rcPaint, (HBRUSH)(COLOR_WINDOW + 1));
		EndPaint(hwnd, &ps);
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	case WM_SIZE:
		UpdateChildPos();
	}
	return DefWindowProcW(hwnd, msgType, wParam, lParam);
}



static inline void RegisterWindowClass(void) {
	WNDCLASS wc = { 0 };
	wc.lpfnWndProc = MainWndProc;
	wc.hInstance = hIns;
	wc.hIcon = LoadIconA(hIns, MAKEINTRESOURCEA(IDI_ICON1));
	wc.lpszClassName = L"MainWindowClass";
	RegisterClassW(&wc);
}

static inline void FillRects(RECT* result) {
	int screenx = GetSystemMetrics(SM_CXSCREEN);
	int screeny = GetSystemMetrics(SM_CYSCREEN);
	result->left = screenx / 4;
	result->top = screeny / 4;
	result->right = screenx - screenx / 4;
	result->bottom = screeny - screeny / 4;
}

static inline void UpdateRepoNum(void) {
	int amount = GetPuzzleAmountInRepository();
	if (amount == -1) {
		SetupRepository();
		amount = 0;
	}
	wchar_t num[64];
	swprintf_s(num, (size_t)64, L"当前题目数量: %d", amount);
	SetWindowTextW(hText, num);
}

static inline void UpdateChildPos(void) {
	RECT rect;
	GetWindowRect(hWnd, &rect);
	int width = rect.right - rect.left;
	int height = rect.bottom - rect.top;

	int x = width / 2 - 150;
	int y = height / 2 - 100;
	MoveWindow(hPicture, x, y, 100, 100, TRUE);
	MoveWindow(hText, x + 120, y + 40, 300, 100, TRUE);
}


static inline void SetupMainWindowContent(void) {
	hPicture = CreateWindowW(
		L"STATIC",
		NULL,
		WS_CHILD | WS_VISIBLE | SS_ICON,
		100, 100, 150, 150,
		hWnd,
		1,
		hIns,
		NULL
	);
	SendMessageW(hPicture, STM_SETIMAGE, IMAGE_ICON,
		LoadIconW(hIns, MAKEINTRESOURCEW(IDI_ICON2)));

	hText = CreateWindowW(
		L"STATIC",
		NULL,
		WS_CHILD | WS_VISIBLE | SS_LEFT,
		0, 0, 300, 30,
		hWnd,
		2,
		hIns,
		NULL
	);
	UpdateRepoNum();
	UpdateChildPos();
}

static void threadProc(void* arg) {
	struct gInfo* info = (struct gInfo*)arg;
	PSUDOKUPUZZLE puzzles = malloc(info->num * sizeof(SUDOKUPUZZLE));
	if (puzzles != NULL) {
		GenerateSudokuMT(puzzles, info->num, info->clue1, info->clue2, info->trd, SetProgress);
		AddToRepository(puzzles, info->num);
		free(puzzles);
		UpdateRepoNum();
		Sleep(800);
		StopProgress();
	}
}




