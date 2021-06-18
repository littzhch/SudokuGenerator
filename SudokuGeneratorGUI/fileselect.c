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
	pFileOpen->lpVtbl->SetTitle(pFileOpen, L"ѡ��һ���ļ�");
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


void DLG_GetWriteFilePath(PWSTR* path) { //TODO: �ṩѡ���ļ����ͺ��Զ���Ӻ�׺���Ĺ���
	IFileSaveDialog* pFileSave;
	if (pItem)
		pItem->lpVtbl->Release(pItem);
	pItem = NULL;
	CoCreateInstance(&CLSID_FileSaveDialog, NULL, CLSCTX_ALL,
		&IID_IFileSaveDialog, (void**)&pFileSave);
	pFileSave->lpVtbl->SetTitle(pFileSave, L"ѡ�񱣴�λ��");
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