#include <windows.h>
#include "cmdinteract.h"

static WORD GetCurrentTextAttribute(HANDLE handle);

_Noreturn ErrExit(int errType, const char * errContent, const char* message, _Bool silent) {
	if (!silent) {
		HANDLE output = GetStdHandle(STD_OUTPUT_HANDLE);
		WORD textAttr = GetCurrentTextAttribute(output);
		WORD originAttr = textAttr;
		textAttr |= (FOREGROUND_INTENSITY | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_RED);
		textAttr ^= (FOREGROUND_GREEN | FOREGROUND_BLUE);
		SetConsoleTextAttribute(output, textAttr);
		printf("Error ");
		SetConsoleTextAttribute(output, originAttr);
		if (errContent) {
			printf("%s: ", errContent);
		}
		puts(message);
	}
	exit(errType);
}

void WarnningInfo(const char* message) {
	HANDLE output = GetStdHandle(STD_OUTPUT_HANDLE);
	WORD textAttr = GetCurrentTextAttribute(output);
	WORD originAttr = textAttr;
	textAttr |= (FOREGROUND_INTENSITY | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_RED);
	textAttr ^= FOREGROUND_BLUE;
	SetConsoleTextAttribute(output, textAttr);
	printf("Warnning ");
	SetConsoleTextAttribute(output, originAttr);
	puts(message);
}

static WORD GetCurrentTextAttribute(HANDLE handle) {
	CONSOLE_SCREEN_BUFFER_INFO csbInfo;
	GetConsoleScreenBufferInfo(handle, &csbInfo);
	return csbInfo.wAttributes;
}