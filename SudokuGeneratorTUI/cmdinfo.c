#include <stdio.h>
#include <windows.h>
#include "cmdinteract.h"


static char* usage = 
	"�÷���\nsdkg <����> [-num <��������>] [-trd <�߳���>] [-clue <��ʾ������>] [-file <�ļ�·��>] [-silent]\n\n"		  
	"������\n\t"
	  "generate | g: ��������\n"
	"\tsolve | s:    �������\n"
	"\tinit | i:     ���������洢�ļ�\n"
	"\tclean | c:    �����������\n"
	"\texport | e:   ������������\n"
	"\tquery | q:    ��ѯ�����洢�ļ��е�������Ŀ����\n"
	"\thelp | h:     �鿴����\n\n"
	"ѡ�\n"
	"\t-num:    ָ������������������������Ϊgenerateʱ���ã�Ĭ��Ϊ1\n"
	"\t-trd:    ָ����������ʱ��������߳�����������Ϊgenerateʱ���ã���ΧΪ1~64��Ĭ��Ϊ1\n"
	"\t-clue:   ָ��������������ʾ��������������Ϊgenerateʱ���ã�����ָ��һ������������\n"
	"\t-file:   ָ���ļ�·����������Ϊsolve��exportʱ���á�������Ϊsolve��ָ����Ҫ�����������Ŀ�����ļ���\n"
	"\t         ������Ϊexport��ָ���������ļ�·�����ļ���\n"
	"\t-silent: �������������������help���κβ�������\n\n"
	"��ϸ�÷������ https://github.com/littzhch/SudokuGenerator\n";


static inline char* GetSelfVersion(const char* selfpath);
static inline char* GetSelfFilePath(void);

void PrintHelp(void) {
	puts(usage);
}


void PrintWelcome(void) {
	char* selfpath = GetSelfFilePath();
	char* ver = GetSelfVersion(selfpath);
	printf("SudokuGenerator version ");
	puts(ver);
	puts("copyright (c) 2021: littzhch");
	printf("type \"%s help\" for more information\n", selfpath);
}


static inline char* GetSelfVersion(const char* selfpath) {
	static char* result[44];
	UINT16 version[4] = {0, 0, 0, 0};
	DWORD useless;
	DWORD size = GetFileVersionInfoSizeA(selfpath, &useless);
	if (size) {
		LPVOID pdata = malloc(size);
		if (pdata != 0) {
			if (GetFileVersionInfoA(selfpath, 0, size, pdata)) {
				VS_FIXEDFILEINFO* pInfo;
				UINT len;
				if (VerQueryValueA(pdata, "\\", &pInfo, &len)) {
					*((UINT32*)version) = pInfo->dwProductVersionMS;
					*((UINT32*)(version + 2)) = pInfo->dwProductVersionLS;
				}
			}
		}
		free(pdata);
	}
	sprintf_s((char* const)result, 44, "%d.%d.%d.%d", version[1], version[0], version[3], version[2]); //�ֽ���
	return (char*)result;
}

static inline char* GetSelfFilePath(void) {
	static char selfpath[512] = {0};
	GetModuleFileNameA(NULL, selfpath, 512);
	return selfpath;
}