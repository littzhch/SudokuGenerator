#include "dialogs.h"

IShellItem* pItem = NULL;

void DLG_InitFileService(void) {
	CoInitializeEx(NULL, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE);
}

void DLG_GetOpenFilePath(PWSTR* path) {
	IFileOpenDialog* pFileOpen;
	if (pItem)
		pItem->lpVtbl->Release(pItem);
	pItem = NULL;
	CoCreateInstance(&CLSID_FileOpenDialog, NULL, CLSCTX_ALL,
		&IID_IFileOpenDialog, (void**)&pFileOpen);
	pFileOpen->lpVtbl->SetTitle(pFileOpen, L"选择一个文件");
	pFileOpen->lpVtbl->Show(pFileOpen, NULL);
	HRESULT hr = pFileOpen->lpVtbl->GetResult(pFileOpen, &pItem);
	pFileOpen->lpVtbl->Release(pFileOpen);
	if (pItem) {
		pItem->lpVtbl->GetDisplayName(pItem, SIGDN_FILESYSPATH, path);
	}
	else {
		*path = NULL;
	}
}


void DLG_GetWriteFilePath(PWSTR* path) { //TODO: 提供选择文件类型和自动添加后缀名的功能
	IFileSaveDialog* pFileSave;
	if (pItem)
		pItem->lpVtbl->Release(pItem);
	pItem = NULL;
	CoCreateInstance(&CLSID_FileSaveDialog, NULL, CLSCTX_ALL,
		&IID_IFileSaveDialog, (void**)&pFileSave);
	pFileSave->lpVtbl->SetTitle(pFileSave, L"选择保存位置");
	pFileSave->lpVtbl->Show(pFileSave, NULL);

	HRESULT hr = pFileSave->lpVtbl->GetResult(pFileSave, &pItem);
	pFileSave->lpVtbl->Release(pFileSave);
	if (pItem) {
		pItem->lpVtbl->GetDisplayName(pItem, SIGDN_FILESYSPATH, path);
	}
	else {
		*path = NULL;
	}
}

void DLG_UninitFileService(void) {
	if (pItem)
		pItem->lpVtbl->Release(pItem);
	CoUninitialize();
}