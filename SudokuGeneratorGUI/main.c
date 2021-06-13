#include <Windows.h>
#include <Commctrl.h>

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
void ChangeText(HWND hButton);
HWND hButton1, hButton2;
int num = 0;

int WINAPI wWinMain(
	HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	PWSTR szCmdLine,
	int iCmdShow) {

	WNDCLASS wc = { 0 };
	wc.lpfnWndProc = WndProc;
	wc.hInstance = hInstance;
	wc.lpszClassName = L"MyWindowClass";
	RegisterClassW(&wc);

	HWND hwnd = CreateWindowExW(
		0,
		L"MyWindowClass",
		L"Hello Window",
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		NULL,
		NULL,
		hInstance,
		NULL
	);
	ShowWindow(hwnd, iCmdShow);

	hButton1 = CreateWindowW(
		L"BUTTON",
		L"0",
		WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
		1000,
		500,
		100,
		50,
		hwnd,
		NULL,
		hInstance,
		NULL
	);

	hButton2 = CreateWindowW(
		L"BUTTON",
		L"0",
		WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
		1110,
		500,
		100,
		50,
		hwnd,
		NULL,
		hInstance,
		NULL
	);

	MSG msg;
	while (GetMessageW(&msg, NULL, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessageW(&msg);
	}

	return msg.wParam;
}


LRESULT CALLBACK WndProc(HWND hwnd, UINT msgType, WPARAM wParam, LPARAM lParam) {
	HDC hdc;
	PAINTSTRUCT ps;
	HWND hButton;

	switch (msgType) {
	case WM_COMMAND:
		hButton = (HWND)lParam;
		if (HIWORD(wParam) == BN_CLICKED) {
			ChangeText(hButton);
		}
		return 0;
	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);
		FillRect(hdc, &ps.rcPaint, (HBRUSH)(COLOR_WINDOW + 1));
		EndPaint(hwnd, &ps);
		return 0;
	case WM_CLOSE:
		if (MessageBoxW(hwnd, L"真的要退出吗？", L"不要啊！", MB_OKCANCEL) == IDOK) {
			DestroyWindow(hwnd);
		}
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return DefWindowProcW(hwnd, msgType, wParam, lParam);
}

void ChangeText(HWND hButton) {
	if (hButton == hButton1)
		num++;
	else
		num--;
	wchar_t str[10];
	wsprintf(str, L"%d", num);
	SendMessage(hButton1, WM_SETTEXT, 0, str);
	SendMessage(hButton2, WM_SETTEXT, 0, str);
}