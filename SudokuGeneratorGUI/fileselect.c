#include "dialogs.h"

static wchar_t filePath[MAX_PATH];

void DLG_InitFileService(void) {
	CoInitializeEx(NULL, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE);
}

void DLG_GetOpenFilePath(PWSTR* path) {
	IFileOpenDialog* pFileOpen;
	IShellItem* pItem;
	CoCreateInstance(&CLSID_FileOpenDialog, NULL, CLSCTX_ALL,
		&IID_IFileOpenDialog, (void**)&pFileOpen);
	pFileOpen->lpVtbl->SetTitle(pFileOpen, L"选择一个文件");
	pFileOpen->lpVtbl->Show(pFileOpen, NULL);
	HRESULT hr = pFileOpen->lpVtbl->GetResult(pFileOpen, &pItem);
	pFileOpen->lpVtbl->Release(pFileOpen);

	if (hr == S_OK) {
		pItem->lpVtbl->GetDisplayName(pItem, SIGDN_FILESYSPATH, path);
		wcscpy_s(filePath, MAX_PATH, *path);
		CoTaskMemFree(*path);
		pItem->lpVtbl->Release(pItem);
		*path = filePath;
		return;
	}
	*path = NULL;
}


int DLG_GetWriteFilePath(PWSTR* path) { //TODO: 提供选择文件类型和自动添加后缀名的功能
	IFileSaveDialog* pFileSave;
	IShellItem* pItem;
	int fileType;
	CoCreateInstance(&CLSID_FileSaveDialog, NULL, CLSCTX_ALL,
		&IID_IFileSaveDialog, (void**)&pFileSave);
	pFileSave->lpVtbl->SetTitle(pFileSave, L"选择保存位置");
	COMDLG_FILTERSPEC rgSpec[] = {
		{ L"Word文档", L"*.docx"},
		{ L"json格式", L" *.*"}
	};
	pFileSave->lpVtbl->SetFileTypes(pFileSave, 2, rgSpec);
	pFileSave->lpVtbl->Show(pFileSave, NULL);

	HRESULT hr1 = pFileSave->lpVtbl->GetResult(pFileSave, &pItem);
	HRESULT hr2 = pFileSave->lpVtbl->GetFileTypeIndex(pFileSave, &fileType);
	pFileSave->lpVtbl->Release(pFileSave);

	if (hr1 == S_OK) {
		pItem->lpVtbl->GetDisplayName(pItem, SIGDN_FILESYSPATH, path);
		wcscpy_s(filePath, MAX_PATH, *path);
		CoTaskMemFree(*path);
		pItem->lpVtbl->Release(pItem);
		if (hr2 == S_OK) {
			*path = filePath;
			return fileType;
		}
	}
	*path = NULL;
	return 0;
}

void DLG_UninitFileService(void) {
	CoUninitialize();
}