#include <stdio.h>
#include "cmdinteract.h"


static char* usage = 
	"用法：\nsdkg <操作> [-num <生成数量>] [-trd <线程数>] [-file <文件路径>] [-silent]\n\n"		  
	"操作：\n\t"
	  "generate | g: 生成数独\n"
	"\tsolve | s:    求解数独\n"
	"\tinit | i:     创建数独存储文件\n"
	"\tclean | c:    清除所有数独\n"
	"\texport | e:   导出所有数独\n"
	"\thelp | h:     查看帮助\n\n"
	"选项：\n"
	"\t-num:    指定生成数独的数量。当操作为generate时可用，默认为1\n"
	"\t-trd:    指定生成数独时的最大工作线程数。当操作为generate时可用，范围为1~64，默认为1\n"
	"\t-file:   指定文件路径。当操作为solve或export时可用。若操作为solve，指定需要导入的数独题目所在文件；\n"
	"\t         若操作为export，指定导出的文件路径和文件名\n"
	"\t-silent: 不产生命令行输出，任何操作可用\n\n"
	"详细用法请访问 https://github.com/littzhch/SudokuGenerator\n";


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


static char* GetSelfVersion(void) { //TODO: 完成版本号获取
	return "0.0.1";
}

static void DeleteVer(char* ver) {
	return;
}