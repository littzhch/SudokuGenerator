#pragma once
#include <windows.h>
#include "resource.h"


struct gInfo {
	int num;
	int clue1; // ��֤ clue1 <= clue2
	int clue2;
	int trd;
};

void AskForSettings(struct gInfo* info);
// ѯ�����ã���info->num == 0��������û�ȡ��������