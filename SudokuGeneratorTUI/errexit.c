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

_Noreturn WoinExit(int woinType, const char* woinContent, const char* message, _Bool silent) {
	if (!silent) {
		HANDLE output = GetStdHandle(STD_OUTPUT_HANDLE);
		SetConsoleTextAttribute(output, FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN);
		printf("Warnning ");
		SetConsoleTextAttribute(output,
			FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
		if (woinContent) {
			printf("%s: ", woinContent);
		}
		puts(message);
	}
	exit(woinType);
}