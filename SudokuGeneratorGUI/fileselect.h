#pragma once
#include <windows.h>
#include <shobjidl.h>

void InitFileService(void);

void UninitFileService(void);

void GetOpenFilePath(PWSTR* path);
// ��ȡ��Ҫ�򿪵��ļ���·�������û��رմ��ڣ�pathΪNULL

void GetWriteFilePath(PWSTR* path);
// ��ȡ��Ҫ�������ļ���·�������û��رմ��ڣ�pathΪNULL
