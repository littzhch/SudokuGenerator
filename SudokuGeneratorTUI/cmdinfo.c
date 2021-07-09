#include <stdio.h>
#include <windows.h>
#include "cmdinteract.h"


static char* usage = 
	"用法：\nsdkg <操作> [-num <生成数量>] [-trd <线程数>] [-clue <提示数数量>] [-file <文件路径>] [-silent]\n\n"		  
	"操作：\n\t"
	  "generate | g: 生成数独\n"
	"\tsolve | s:    求解数独\n"
	"\tinit | i:     创建数独存储文件\n"
	"\tclean | c:    清除所有数独\n"
	"\texport | e:   导出所有数独\n"
	"\tquery | q:    查询数独存储文件中的数独题目数量\n"
	"\thelp | h:     查看帮助\n\n"
	"选项：\n"
	"\t-num:    指定生成数独的数量。当操作为generate时可用，默认为1\n"
	"\t-trd:    指定生成数独时的最大工作线程数。当操作为generate时可用，范围为1~64，默认为1\n"
	"\t-clue:   指定生成数独的提示数数量。当操作为generate时可用，可以指定一个或两个数字\n"
	"\t-file:   指定文件路径。当操作为solve或export时可用。若操作为solve，指定需要导入的数独题目所在文件；\n"
	"\t         若操作为export，指定导出的文件路径和文件名\n"
	"\t-silent: 不产生命令行输出，除help外任何操作可用\n\n"
	"详细用法请访问 https://github.com/littzhch/SudokuGenerator\n";


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
	sprintf_s((char* const)result, 44, "%d.%d.%d.%d", version[1], version[0], version[3], version[2]); //字节序
	return (char*)result;
}

static inline char* GetSelfFilePath(void) {
	static char selfpath[512] = {0};
	GetModuleFileNameA(NULL, selfpath, 512);
	return selfpath;
}