# 数独生成器 SudokuGenerator

这是[sudoku-generator](https://github.com/littzhch/sudoku-generator)的C语言版本，只使用了 win32 API 和 C标准库

## 功能

- 使用多线程技术生成数独题目
- 读取用户输入并求解数独
- 从文件中导入并求解数独
- 可将数独题目导出为json或docx格式
- 有命令行和图形两套用户界面

## 已知问题

- 由于糟糕的算法设计，当需要生成的数独的提示数数量小于30时，生成时有概率卡住，卡住的概率与生成数独的数量成正相关，与提示数数量成负相关（如果卡住了，请使用任务管理器强制结束任务）
- 由于作者偷懒没有对文件进行压缩，导出的docx文件的体积是正常情况下的五倍以上（用Word编辑保存一下可以变回正常大小）
- 没有做很多测试

## 文档

[命令行程序的使用方法](https://github.com/littzhch/SudokuGenerator/blob/master/SudokuGeneratorTUI/sdkg_tui.md)

图形程序没有文档（别问，问就是懒）

[导入数独的文件格式](https://github.com/littzhch/SudokuGenerator/blob/master/SudokuIO/input_file.md)