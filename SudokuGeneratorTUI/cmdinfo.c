#include <stdio.h>
#include "cmdinteract.h"


static char* usage = 
	"�÷���\nsdkg <����> [-num <��������>] [-trd <�߳���>] [-file <�ļ�·��>] [-silent]\n\n"		  
	"������\n\t"
	  "generate | g: ��������\n"
	"\tsolve | s:    �������\n"
	"\tinit | i:     ���������洢�ļ�\n"
	"\tclean | c:    �����������\n"
	"\texport | e:   ������������\n"
	"\thelp | h:     �鿴����\n\n"
	"ѡ�\n"
	"\t-num:    ָ������������������������Ϊgenerateʱ���ã�Ĭ��Ϊ1\n"
	"\t-trd:    ָ����������ʱ��������߳�����������Ϊgenerateʱ���ã���ΧΪ1~64��Ĭ��Ϊ1\n"
	"\t-file:   ָ���ļ�·����������Ϊsolve��exportʱ���á�������Ϊsolve��ָ����Ҫ�����������Ŀ�����ļ���\n"
	"\t         ������Ϊexport��ָ���������ļ�·�����ļ���\n"
	"\t-silent: ������������������κβ�������\n\n"
	"��ϸ�÷������ https://github.com/littzhch/SudokuGenerator\n";


static char* GetSelfVersion(void);
static void DeleteVer(char* ver);

void PrintHelp(void) {
	puts(usage);
}


void PrintWelcome(const char* selfpath) {
	char* ver = GetSelfVersion();
	printf("SudokuGenerator version ");
	puts(ver);
	puts("copyright (c) 2021: littzhch");
	printf("type \"%s help\" for more information\n", selfpath);
	DeleteVer(ver);
}


static char* GetSelfVersion(void) { //TODO: ��ɰ汾�Ż�ȡ
	return "0.0.1";
}

static void DeleteVer(char* ver) {
	return;
}