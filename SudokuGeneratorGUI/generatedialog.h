#pragma once
#include <windows.h>
#include "resource.h"


struct gInfo {
	int num;
	int clue1; // 保证 clue1 <= clue2
	int clue2;
	int trd;
};

void AskForSettings(struct gInfo* info);
// 询问设置，若info->num == 0，则代表用户取消了设置