#include "fileselect.h"

IShellItem* pItem1 = NULL;
IShellItem* pItem2 = NULL;

void InitFileService(void) {
	CoInitializeEx(NULL, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE);

}

void GetOpenFilePath(PWSTR* path) {
	IFileOpenDialog* pFileOpen;
	CoCreateInstance(&CLSID_FileOpenDialog, NULL, CLSCTX_ALL,
		&IID_IFileOpenDialog, (void**)&pFileOpen);
	pFileOpen->lpVtbl->SetTitle(pFileOpen, L"选择一个文件");
	pFileOpen->lpVtbl->Show(pFileOpen, NULL);
	HRESULT hr = pFileOpen->lpVtbl->GetResult(pFileOpen, &pItem1);
	pFileOpen->lpVtbl->Release(pFileOpen);
	if (pItem1) {
		pItem1->lpVtbl->GetDisplayName(pItem1, SIGDN_FILESYSPATH, path);
	}
	else {
		*path = NULL;
	}
}


void GetWriteFilePath(PWSTR* path) {
	IFileSaveDialog* pFileSave;
	CoCreateInstance(&CLSID_FileSaveDialog, NULL, CLSCTX_ALL,
		&IID_IFileSaveDialog, (void**)&pFileSave);
	pFileSave->lpVtbl->SetTitle(pFileSave, L"选择保存位置");
	pFileSave->lpVtbl->Show(pFileSave, NULL);

	HRESULT hr = pFileSave->lpVtbl->GetResult(pFileSave, &pItem2);
	pFileSave->lpVtbl->Release(pFileSave);
	if (pItem2) {
		pItem2->lpVtbl->GetDisplayName(pItem2, SIGDN_FILESYSPATH, path);
	}
	else {
		*path = NULL;
	}
}

void UninitFileService(void) {
	if (pItem1)
		pItem1->lpVtbl->Release(pItem1);
	if (pItem2)
		pItem2->lpVtbl->Release(pItem2);
	CoUninitialize();
}