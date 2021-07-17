#include <stdio.h>
#include <io.h>
#include "sudoku.h"
#include "sudokuIO.h"
#include "resource.h"

extern char REPOPATH[];
static inline void WriteDocumentXml(const PSUDOKUPUZZLE pPuzzles, int puzzleNum, FILE* file);
static inline UINT32 Crc32(FILE* file);
static inline void WriteTemplate(int id, FILE* file);

int ExportRepoAsDocx(const char* filepath) {
    FILE* repo, *target, *temp;
    if (fopen_s(&repo, REPOPATH, "rb")) {
        return -1;
    }
    int num;
    fread(&num, sizeof(int), 1, repo);
    if (!num) {
        fclose(repo);
        return -3;
    }
    if (fopen_s(&target, filepath, "wb")) {
        fclose(repo);
        return -2;
    }
    char tempFilePath[MAX_PATH];
    GetTempPathA(MAX_PATH, tempFilePath);
    strcat_s(tempFilePath, MAX_PATH, "sudoku_generator_document_xml_temp_file");
    if (fopen_s(&temp, tempFilePath, "w,ccs=UTF-8")) {
        fclose(repo);
        fclose(target);
        return -4;
    }

    PSUDOKUPUZZLE puzzles = (PSUDOKUPUZZLE)malloc(num * sizeof(SUDOKUPUZZLE));
    fread(puzzles, sizeof(SUDOKUPUZZLE), num, repo);
    WriteDocumentXml(puzzles, num, temp);
    freopen_s(&temp, tempFilePath, "rb", temp);
    UINT32 crc = Crc32(temp);
    UINT32 size = _filelength(_fileno(temp));
    UINT32 uint32 = 17;
    char* name = "word/document.xml";
    char buffer;
    WriteTemplate(IDR_ZIP1, target);
    fwrite(&crc, 4, 1, target);
    fwrite(&size, 4, 1, target);
    fwrite(&size, 4, 1, target);
    fwrite(&uint32, 4, 1, target);
    fwrite(name, uint32, 1, target);
    rewind(temp);
    for (UINT32 i = 0; i < size; i++) {
        fread(&buffer, 1, 1, temp);
        fwrite(&buffer, 1, 1, target);
    }
    WriteTemplate(IDR_ZIP2, target);
    fwrite(&crc, 4, 1, target);
    fwrite(&size, 4, 1, target);
    fwrite(&size, 4, 1, target);
    WriteTemplate(IDR_ZIP3, target);
    uint32 = size + 895;
    fwrite(&uint32, 4, 1, target);
    WriteTemplate(IDR_ZIP4, target);
    uint32 = size + 1549;
    fwrite(&uint32, 4, 1, target);
    WriteTemplate(IDR_ZIP5, target);
    uint32 = size + 2822;
    fwrite(&uint32, 4, 1, target);
    WriteTemplate(IDR_ZIP6, target);
    uint32 = size + 5827;
    fwrite(&uint32, 4, 1, target);
    WriteTemplate(IDR_ZIP7, target);
    uint32 = size + 7569;
    fwrite(&uint32, 4, 1, target);
    WriteTemplate(IDR_ZIP8, target);
    uint32 = size + 7927;
    fwrite(&uint32, 4, 1, target);
    WriteTemplate(IDR_ZIP9, target);
    uint32 = size + 8222;
    fwrite(&uint32, 4, 1, target);
    WriteTemplate(IDR_ZIP10, target);
    uint32 = size + 8608;
    fwrite(&uint32, 4, 1, target);
    WriteTemplate(IDR_ZIP11, target);
    uint32 = size + 8882;
    fwrite(&uint32, 4, 1, target);
    buffer = 0;
    fwrite(&buffer, 1, 1, target);
    fwrite(&buffer, 1, 1, target);

    fclose(repo);
    fclose(temp);
    fclose(target);
    free(puzzles);
    return 0;
}

static inline void WriteTemplate(int id, FILE* file) {
    HRSRC hRsrc = FindResourceA(GetModuleHandleA("SudokuIO.dll"), MAKEINTRESOURCEA(id), "ZIP");
    void* sourcePtr = LockResource(LoadResource(GetModuleHandleA("SudokuIO.dll"), hRsrc));
    DWORD resourceSize = SizeofResource(GetModuleHandleA("SudokuIO.dll"), hRsrc);
    fwrite(sourcePtr, resourceSize, 1, file);
}

static inline UINT32 Crc32(FILE* file) {
    UINT32 crc = 0xffffffff;
    UINT32 poly = 0x04c11db7;
    UINT32 buffer1;
    unsigned char buffer2;
    rewind(file);
    fread(&buffer1, 1, 4, file);
    for (int i = 0; i < 32; i++) {
        crc ^= ((buffer1 >> i) & 1) << (31 - i);
    }
    while (fread(&buffer2, 1, 1, file)) {
        for (int i = 0; i < 8; i++) {
            if (crc & 0x80000000) {
                crc <<= 1;
                crc |= (buffer2 >> i) & 1;
                crc ^= poly;
            }
            else {
                crc <<= 1;
                crc |= (buffer2 >> i) & 1;
            }
        }
    }
    for (int i = 0; i < 32; i++) {
        if (crc & 0x80000000) {
            crc <<= 1;
            crc ^= poly;
        }
        else {
            crc <<= 1;
        }
    }
    buffer1 = 0;
    for (int i = 0; i < 32; i++) {
        buffer1 |= ((crc >> i) & 1) << (31 - i);
    }
    return buffer1 ^ 0xffffffff;
}



static const wchar_t* xmlHeader = 
    L"<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>\
    <w:document\
    xmlns:wpc=\"http://schemas.microsoft.com/office/word/2010/wordprocessingCanvas\"\
    xmlns:cx=\"http://schemas.microsoft.com/office/drawing/2014/chartex\"\
    xmlns:cx1=\"http://schemas.microsoft.com/office/drawing/2015/9/8/chartex\"\
    xmlns:cx2=\"http://schemas.microsoft.com/office/drawing/2015/10/21/chartex\"\
    xmlns:cx3=\"http://schemas.microsoft.com/office/drawing/2016/5/9/chartex\"\
    xmlns:cx4=\"http://schemas.microsoft.com/office/drawing/2016/5/10/chartex\"\
    xmlns:cx5=\"http://schemas.microsoft.com/office/drawing/2016/5/11/chartex\"\
    xmlns:cx6=\"http://schemas.microsoft.com/office/drawing/2016/5/12/chartex\"\
    xmlns:cx7=\"http://schemas.microsoft.com/office/drawing/2016/5/13/chartex\"\
    xmlns:cx8=\"http://schemas.microsoft.com/office/drawing/2016/5/14/chartex\"\
    xmlns:mc=\"http://schemas.openxmlformats.org/markup-compatibility/2006\"\
    xmlns:aink=\"http://schemas.microsoft.com/office/drawing/2016/ink\"\
    xmlns:am3d=\"http://schemas.microsoft.com/office/drawing/2017/model3d\"\
    xmlns:o=\"urn:schemas-microsoft-com:office:office\"\
    xmlns:r=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships\"\
    xmlns:m=\"http://schemas.openxmlformats.org/officeDocument/2006/math\"\
    xmlns:v=\"urn:schemas-microsoft-com:vml\"\
    xmlns:wp14=\"http://schemas.microsoft.com/office/word/2010/wordprocessingDrawing\"\
    xmlns:wp=\"http://schemas.openxmlformats.org/drawingml/2006/wordprocessingDrawing\"\
    xmlns:w10=\"urn:schemas-microsoft-com:office:word\"\
    xmlns:w=\"http://schemas.openxmlformats.org/wordprocessingml/2006/main\"\
    xmlns:w14=\"http://schemas.microsoft.com/office/word/2010/wordml\"\
    xmlns:w15=\"http://schemas.microsoft.com/office/word/2012/wordml\"\
    xmlns:w16cex=\"http://schemas.microsoft.com/office/word/2018/wordml/cex\"\
    xmlns:w16cid=\"http://schemas.microsoft.com/office/word/2016/wordml/cid\"\
    xmlns:w16=\"http://schemas.microsoft.com/office/word/2018/wordml\"\
    xmlns:w16se=\"http://schemas.microsoft.com/office/word/2015/wordml/symex\"\
    xmlns:wpg=\"http://schemas.microsoft.com/office/word/2010/wordprocessingGroup\"\
    xmlns:wpi=\"http://schemas.microsoft.com/office/word/2010/wordprocessingInk\"\
    xmlns:wne=\"http://schemas.microsoft.com/office/word/2006/wordml\"\
    xmlns:wps=\"http://schemas.microsoft.com/office/word/2010/wordprocessingShape\" \
    mc:Ignorable=\"w14 w15 w16se w16cid w16 w16cex wp14\">\
    <w:body>";

static const wchar_t* xmlEnd = L"</w:body></w:document>";

static const wchar_t* tableHeaderCtrlStr =
L"<w:pPr><w:spacing w:afterLines=\"50\" w:after=\"156\"/></w:pPr><w:p><w:r>"
"<w:rPr><w:rFonts w:ascii=\"宋体\" w:eastAsia=\"宋体\" w:hAnsi=\"宋体\"/>"
"<w:sz w:val=\"24\"/><w:szCs w:val=\"24\"/></w:rPr><w:t>题目%d%s：</w:t></w:r></w:p>"
"<w:tbl><w:tblPr><w:tblW w:w=\"0\" w:type=\"auto\"/><w:jc w:val=\"center\"/>"
"<w:tblLook w:val=\"04A0\" w:firstRow=\"1\" w:lastRow=\"0\" w:firstColumn=\"1\" w:lastColumn=\"0\" "
"w:noHBand=\"0\" w:noVBand=\"1\"/></w:tblPr><w:tblGrid>"
"<w:gridCol w:w=\"567\"/>"
"<w:gridCol w:w=\"567\"/>"
"<w:gridCol w:w=\"567\"/>"
"<w:gridCol w:w=\"567\"/>"
"<w:gridCol w:w=\"567\"/>"
"<w:gridCol w:w=\"567\"/>"
"<w:gridCol w:w=\"567\"/>"
"<w:gridCol w:w=\"567\"/>"
"<w:gridCol w:w=\"567\"/>"
"</w:tblGrid>";



static const wchar_t* tableEnd = L"</w:tbl>";

static const wchar_t* tableCellCtrlStr =
L"<w:vAlign w:val=\"center\"/>\
</w:tcPr><w:p><w:pPr><w:spacing w:line=\"0\" w:lineRule=\"atLeast\"/>\
<w:jc w:val=\"center\"/><w:rPr><w:rFonts w:ascii=\"Cambria\" w:hAnsi=\"Cambria\"/><w:sz w:val=\"36\"/>\
<w:szCs w:val=\"40\"/></w:rPr></w:pPr><w:r><w:rPr><w:rFonts w:ascii=\"Cambria\" \
w:hAnsi=\"Cambria\"/><w:sz w:val=\"36\"/><w:szCs w:val=\"40\"/></w:rPr>\
<w:t>%s</w:t>\
</w:r></w:p></w:tc>";


static inline void WriteXmlTable(const PSUDOKU pSudoku, _Bool isAnswer, int puzzleNumber, FILE* file);
static inline void AddPageBreak(FILE* file);
static inline void AddSpace(FILE* file);

static inline void WriteDocumentXml(const PSUDOKUPUZZLE pPuzzles, int puzzleNum, FILE* file) {
	fputws(xmlHeader, file);
	for (int idx = 0; idx < puzzleNum; idx++) {
		WriteXmlTable(&((pPuzzles + idx)->problem), (_Bool)0, idx + 1, file);
        if (idx < (puzzleNum - 1)) {
            if (idx % 2) {
                AddPageBreak(file);
            }
            else {
                AddSpace(file);
            }
        }
        else {
            AddPageBreak(file);
        }
	}
	for (int idx = 0; idx < puzzleNum; idx++) {
		WriteXmlTable(&((pPuzzles + idx)->answer), (_Bool)1, idx + 1, file);
        if (idx < (puzzleNum - 1)) {
            if (idx % 2) {
                AddPageBreak(file);
            }
            else {
                AddSpace(file);
            }
        }
	}
	fputws(xmlEnd, file);
}


static inline void WriteTableHeader(int puzzleNumber, int clueNum, _Bool isAnswer, FILE* file);
static inline void WriteTableCell(int cellIdx, int cellNum, FILE* file);

static inline void WriteXmlTable(const PSUDOKU pSudoku, _Bool isAnswer, int puzzleNumber, FILE* file) {
	WriteTableHeader(puzzleNumber, pSudoku->filledNum, isAnswer, file);
	for (int idx = 0; idx < 81; idx++) {
        if (GetCol(idx) == 1) {
            fputws(L"<w:tr><w:trPr><w:trHeight w:hRule=\"exact\" w:val=\"567\"/>"
                "<w:jc w:val=\"center\"/></w:trPr>", file);
	}
        WriteTableCell(idx, pSudoku->elements[idx], file);
        if (GetCol(idx) == 9) {
            fputws(L"</w:tr>", file);
        }
	}
	fputws(tableEnd, file);
}

static inline void WriteTableHeader(int puzzleNumber, int clueNum, _Bool isAnswer, FILE* file) {
    if (isAnswer) {
        fwprintf(file, tableHeaderCtrlStr, puzzleNumber, L"答案");
    }
    else {
        wchar_t clueStr[60];
        swprintf_s(clueStr, 60, L"（%d个提示数）", clueNum);
        fwprintf(file, tableHeaderCtrlStr, puzzleNumber, clueStr);
    }
}


static inline void WriteBorders(int cellIdx, FILE* file);

static inline void WriteTableCell(int cellIdx, int cellNum, FILE* file) {
    static wchar_t cellNumStr[10] = L"1";
    fputws(L"<w:tc><w:tcPr><w:tcW w:w=\"567\" w:type=\"dxa\"/>", file);
    WriteBorders(cellIdx, file);
    if (cellNum == 0) {
        fwprintf(file, tableCellCtrlStr, L"");
    }
    else {
        swprintf_s(cellNumStr, 10, L"%d", cellNum);
        fwprintf(file, tableCellCtrlStr, cellNumStr);
    }
}

static inline void AddPageBreak(FILE* file) {
    fputws(L"<w:p><w:pPr><w:widowControl/><w:jc w:val=\"left\"/></w:pPr>"
            "<w:r><w:br w:type=\"page\"/></w:r></w:p>", file);
}

static inline void AddSpace(FILE* file) {
    fputws(L"<w:p/><w:p/><w:p/><w:p/><w:p/>", file);
}

static inline void WriteBorders(int cellIdx, FILE* file) {
    int col = GetCol(cellIdx);
    int borderWidth[4]; // 0: 上  1:右 ...

    switch (GetRow(cellIdx)) {
    case 1:
        borderWidth[0] = 22;
        borderWidth[2] = 4;
        break;
    case 3:
    case 6:
        borderWidth[0] = 4;
        borderWidth[2] = 16;
        break;
    case 9:
        borderWidth[0] = 4;
        borderWidth[2] = 22;
        break;
    default:
        borderWidth[0] = 4;
        borderWidth[2] = 4;
        break;
}

    switch (GetCol(cellIdx)) {
    case 1:
        borderWidth[3] = 22;
        borderWidth[1] = 4;
        break;
    case 3:
    case 6:
        borderWidth[3] = 4;
        borderWidth[1] = 16;
        break;
    case 9:
        borderWidth[3] = 4;
        borderWidth[1] = 22;
        break;
    default:
        borderWidth[3] = 4;
        borderWidth[1] = 4;
        break;
    }

    fputws(L"<w:tcBorders>", file);
    fwprintf(file, L"<w:top w:val=\"single\" w:sz=\"%d\" w:space=\"0\" w:color=\"auto\"/>", borderWidth[0]);
    fwprintf(file, L"<w:right w:val=\"single\" w:sz=\"%d\" w:space=\"0\" w:color=\"auto\"/>", borderWidth[1]);
    fwprintf(file, L"<w:bottom w:val=\"single\" w:sz=\"%d\" w:space=\"0\" w:color=\"auto\"/>", borderWidth[2]);
    fwprintf(file, L"<w:left w:val=\"single\" w:sz=\"%d\" w:space=\"0\" w:color=\"auto\"/>", borderWidth[3]);
    fputws(L"</w:tcBorders>", file);
}