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
	pFileOpen->lpVtbl->SetTitle(pFileOpen, L"ѡ��һ���ļ�");
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


int DLG_GetWriteFilePath(PWSTR* path) { //TODO: �ṩѡ���ļ����ͺ��Զ���Ӻ�׺���Ĺ���
	IFileSaveDialog* pFileSave;
	IShellItem* pItem;
	int fileType;
	CoCreateInstance(&CLSID_FileSaveDialog, NULL, CLSCTX_ALL,
		&IID_IFileSaveDialog, (void**)&pFileSave);
	pFileSave->lpVtbl->SetTitle(pFileSave, L"ѡ�񱣴�λ��");
	COMDLG_FILTERSPEC rgSpec[] = {
		{ L"Word�ĵ�", L"*.docx"},
		{ L"json��ʽ", L" *.*"}
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