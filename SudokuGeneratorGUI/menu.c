#include "menu.h"
#include "resource.h"
#include "sudokuio.h"
#include "sudoku.h"

extern HWND hWnd;
extern HINSTANCE hIns;

static void GenerateThreadProc(void* arg);
static void FileDropProc(void* arg);
static void SolveProc(void* arg);
static void ExportThreadProc(void* arg);
static inline char* W2A(wchar_t* source);
static void UpdateNoAnswerAmount(int* count);

struct TRDINFO {
	int fType;
	PWSTR path;
};

void ReactToMenuClick(int menuItem) {
	struct gInfo info;
	wchar_t* path;
	struct TRDINFO trdInfo;

	switch (menuItem) {

	case ID_MENU_ABOUT:
		DLG_ShowAboutWindow();
		break;

	case ID_MENU_GENERATE:
		DLG_AskForSettings(&info);
		if (info.num) {
			_beginthread(GenerateThreadProc, 0, &info);
			DLG_PopUpProgress(WNDTYPE_NORMAL, L"��������...");
		}
		break;

	case ID_MENU_DEL:
		if (GetPuzzleAmountInRepository() == 0) {
			MessageBoxA(hWnd, "��Ŀ�����Ѿ�Ϊ 0", "��ʾ",
				MB_OK | MB_ICONINFORMATION);
			break;
		}
		if (MessageBoxA(hWnd, "������Ŀ������գ�ȷ��Ҫ������", "����",
			MB_OKCANCEL | MB_ICONWARNING) == IDOK) {
			CleanRepository();
		}
		break;

	case ID_MENU_EXPORT:
		if (GetPuzzleAmountInRepository() == 0) {
			MessageBoxA(hWnd, "��Ŀ����Ϊ 0", "��ʾ",
				MB_OK | MB_ICONINFORMATION);
			break;
		}
		int fileType = DLG_GetWriteFilePath(&path);
		if (fileType) {
			trdInfo = (struct TRDINFO){ fileType, path };
			_beginthread(ExportThreadProc, 0, (void*)&trdInfo);
			DLG_PopUpProgress(WNDTYPE_MARQUEE, L"���ڵ���...");
		}
		break;

	case ID_MENU_ENTER:
		SUDOKUPUZZLE sp;
		if (DLG_GetSudokuProblem(&sp)) {
			switch (SolveSudoku(&sp)) {
			case 0:
				AddToRepository(&sp, 1);
				MessageBoxA(hWnd, "��Ŀ�Ѽ���洢����", "������", MB_OK);  //TODO: ʹ�ø��ϸ��������ȷ�Լ��
				break;
			case -1:
				MessageBoxA(hWnd, "��Ŀ�޽�", "����", MB_OK | MB_ICONERROR);
				break;
			}
		}
		break;

	case ID_MENU_FROMFILE:
		DLG_GetOpenFilePath(&path);
		if (path) {
			_beginthread(SolveProc, 0, (void*)W2A(path));
			DLG_PopUpProgress(WNDTYPE_MARQUEE, L"���ڶ�ȡ�ļ�...");
		}
		break;
	}
}

int ImportAllFromFile(const char* filepath, int* noAnswerCount) {
	SUDOKUPUZZLE puzzles[IMPORTBUFFERLEN];
	int num;
	while ((num = ImportPuzzleFromJson(puzzles, filepath)) == IMPORTBUFFERLEN) {
		for (int idx = 0; idx < IMPORTBUFFERLEN; idx++) {
			if (SolveSudoku(puzzles + idx) == -1) {
				UpdateNoAnswerAmount(noAnswerCount);
			}
		}
		if (AddToRepository(puzzles, IMPORTBUFFERLEN)) {
			return -1; // �޷��������洢
		}
	}
	if (num == -1) {
		return -2; // �޷���Ŀ���ļ�
	}
	else {
		for (int idx = 0; idx < num; idx++) {
			if (SolveSudoku(puzzles + idx) == -1) {
				UpdateNoAnswerAmount(noAnswerCount);
			}
		}
		if (AddToRepository(puzzles, num)) {
			return -1;
		}
	}
	return 0;
}


static void ExportThreadProc(void* arg) {
	struct TRDINFO* pInfo = (struct TRDINFO*)arg;
	int result;
	if (pInfo->fType == 2) {
		result = ExportRepoAsJson(W2A(pInfo->path));
	}
	else if (pInfo->fType == 1) {
		wcscat_s(pInfo->path, MAX_PATH, L".docx");
		result = ExportRepoAsDocx(W2A(pInfo->path));
	}
	else {
		result = 0;
	}
	switch (result) {
	case 0:
		DLG_StopProgress();
		break;
	case -1:
		DLG_StopProgress();
		MessageBoxA(hWnd, "�޷��������洢�ļ������ܲ����ڻ�ռ��", "����", MB_OK | MB_ICONERROR);
		break;
	case -2:
		DLG_StopProgress();
		MessageBoxA(hWnd, "�޷���Ŀ���ļ�", "����", MB_OK | MB_ICONERROR);
		break;
	case -4:
		DLG_StopProgress();
		MessageBoxA(hWnd, "�޷�����ʱ�ļ�", "����", MB_OK | MB_ICONERROR);
		break;
	}
}

static void GenerateThreadProc(void* arg) {
	struct gInfo* info = (struct gInfo*)arg;
	PSUDOKUPUZZLE puzzles = malloc(info->num * sizeof(SUDOKUPUZZLE));
	if (puzzles != NULL) {
		GenerateSudokuMT(puzzles, info->num, info->clue1, info->clue2, info->trd, DLG_SetProgress);
		AddToRepository(puzzles, info->num);
		free(puzzles);
		Sleep(800);
		DLG_StopProgress();
	}
}


static void SolveProc(void* arg) {
	char* filepath = (char*)arg;
	int noAnswerCount = 0;
	switch (ImportAllFromFile(filepath, &noAnswerCount)) {
	case 0:
		DLG_StopProgress();
		break;
	case -1:
		DLG_StopProgress();
		MessageBoxA(hWnd, "�޷��������洢�ļ������ܲ����ڻ�ռ��", "����", MB_OK | MB_ICONERROR);
		break;
	case -2:
		DLG_StopProgress();
		MessageBoxA(hWnd, "�޷���Ŀ���ļ�", "����", MB_OK | MB_ICONERROR);
		break;
	}
}

static void UpdateNoAnswerAmount(int* count) {
	wchar_t noAnswerStr[64];
	(*count)++;
	swprintf_s(noAnswerStr, 64, L"����%d���޽�����", *count);
	DLG_ChangeTextWR(noAnswerStr);
}



static inline char* W2A(wchar_t* source) {
	static char result[MAX_PATH];
	size_t num;
	wcstombs_s(&num, result, 256, source, MAX_PATH - 1);
	return result;
}