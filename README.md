# 数独生成器 SudokuGenerator

这是[sudoku-generator](https://github.com/littzhch/sudoku-generator)的C语言版本，只使用了 win32 API 和 C标准库

## 功能

- 使用多线程技术生成数独题目
- 读取用户输入并求解数独
- 从文件中导入并求解数独
- 可将数独题目导出为json或docx格式
- 有命令行和图形两套用户界面

## 已知问题

- 作者偷懒没有对文件进行压缩，导致导出的docx文件很大，你要忍一下（忍不了的话用Word编辑保存一下可以变回正常大小）
- 没有做很多测试

## 文档

[命令行程序的使用方法](./SudokuGeneratorTUI/sdkg_tui.md)

图形程序没有文档（别问，问就是懒）

[数独导入文件格式说明](./SudokuIO/input_file.md)
