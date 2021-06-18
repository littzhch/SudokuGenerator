#pragma once
#include <windows.h>
#include "dialogs.h"

void ReactToMenuClick(int menuItem);

int ImportAllFromFile(const char* filepath, int* noAnswerCount);
