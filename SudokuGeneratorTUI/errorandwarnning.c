#include <windows.h>
#include "cmdinteract.h"

_Noreturn ErrExit(int errType, const char * errContent, const char* message, _Bool silent) {
	if (!silent) {
		HANDLE output = GetStdHandle(STD_OUTPUT_HANDLE);
		SetConsoleTextAttribute(output, FOREGROUND_INTENSITY | FOREGROUND_RED);
		printf("Error ");
		SetConsoleTextAttribute(output,
			FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
		if (errContent) {
			printf("%s: ", errContent);
		}
		puts(message);
	}
	exit(errType);
}

void WarnningInfo(const char* message) {
	HANDLE output = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(output, FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN);
	printf("Warnning ");
	SetConsoleTextAttribute(output,
		FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
	puts(message);
}