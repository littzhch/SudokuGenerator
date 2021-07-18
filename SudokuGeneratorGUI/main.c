#include <windowsx.h>
#include <stdio.h>
#include <process.h>
#include <windows.h>
#include "dialogs.h"
#include "resource.h"
#include "menu.h"
#include "SudokuIO.h"

#pragma comment(linker,"\"/manifestdependency:type='win32' \
name='Microsoft.Windows.Common-Controls' version='6.0.0.0' \
processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")

//TRY: 支持只输入一个提示数
//TRY: 合并dropfile和菜单导入

HINSTANCE hIns;
HWND hWnd;

static HWND hPicture;
static HWND hText;

static inline void FillRects(RECT* result);
static inline void SetupMainWindowContent(void);
static inline void RegisterWindowClass(void);
static inline void UpdateChildPos(void);
static inline void UpdateRepoNum(void);
static void FileDropProc(void* arg);
static LRESULT CALLBACK MainWndProc(HWND hwnd, UINT msgType, WPARAM wParam, LPARAM lParam);


int WINAPI wWinMain(
	HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	PWSTR szCmdLine,
	int iCmdShow) 
{
	RECT rect;

	hIns = hInstance;
	DLG_InitFileService();
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
	return (int)msg.wParam;
}


static LRESULT CALLBACK MainWndProc(HWND hwnd, UINT msgType, WPARAM wParam, LPARAM lParam) {
	HDC hdc;
	PAINTSTRUCT ps;

	switch (msgType) {
	case WM_DROPFILES:
		_beginthread(FileDropProc, 0, (void*)wParam);
		DLG_PopUpProgress(WNDTYPE_MARQUEE, L"正在打开文件");
		UpdateRepoNum();
		return 0;

	case WM_CTLCOLORSTATIC:
		return (LRESULT)(HBRUSH)(COLOR_WINDOW + 1);

	case WM_COMMAND:
		ReactToMenuClick(LOWORD(wParam));
		UpdateRepoNum();
		return 0;

	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);
		FillRect(hdc, &ps.rcPaint, (HBRUSH)(COLOR_WINDOW + 1));
		EndPaint(hwnd, &ps);
		return 0;

	case WM_CLOSE:
		DLG_UninitFileService();
		DestroyWindow(hWnd);
		return 0;

	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;

	case WM_SIZE:
		UpdateChildPos();
		return 0;
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
		(HMENU)1,
		hIns,
		NULL
	);
	SendMessageW(hPicture, STM_SETIMAGE, IMAGE_ICON,
		(LPARAM)LoadIconW(hIns, MAKEINTRESOURCEW(IDI_ICON2)));

	hText = CreateWindowW(
		L"STATIC",
		NULL,
		WS_CHILD | WS_VISIBLE | SS_LEFT,
		0, 0, 300, 30,
		hWnd,
		(HMENU)2,
		hIns,
		NULL
	);
	UpdateRepoNum();
	UpdateChildPos();
}


static void FileDropProc(void* arg) {
	HDROP hDrop = (HDROP)arg;
	char filepath[MAX_PATH];
	wchar_t text[40];
	size_t strsize;
	int noAnswerCount = 0;

	int amount = DragQueryFileA(hDrop, (UINT)0xFFFFFFFF, NULL, 0);
	for (int idx = 0; idx < amount; idx++) {
		strsize = (size_t)DragQueryFileA(hDrop, idx, NULL, 0);
		DragQueryFileA(hDrop, idx, filepath, MAX_PATH);

		swprintf_s(text, 40, L"正在读取...   文件 %d/%d", idx + 1, amount);
		DLG_ChangeTextWL(text);
		switch (ImportAllFromFile(filepath, &noAnswerCount)) {
		case -1:
			DLG_StopProgress();
			MessageBoxA(hWnd, "无法打开数独存储文件，可能不存在或被占用", "错误", MB_OK | MB_ICONERROR);
			return;
		case -2:
			swprintf_s(text, 40, L"无法打开文件 %d", idx + 1);
			DLG_ChangeTextWL(text);
			Sleep(700);
			break;
		}
	}
	DLG_StopProgress();
}


/*
char num[10];
sprintf_s(num, 10, "%d", amount);
MessageBoxA(NULL, num, "a", MB_OK);
*/