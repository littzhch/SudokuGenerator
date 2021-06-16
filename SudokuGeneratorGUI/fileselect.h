#pragma once
#include <windows.h>
#include <shobjidl.h>

void InitFileService(void);

void UninitFileService(void);

void GetOpenFilePath(PWSTR* path);
// 获取需要打开的文件的路径，若用户关闭窗口，path为NULL

void GetWriteFilePath(PWSTR* path);
// 获取需要创建的文件的路径，若用户关闭窗口，path为NULL
