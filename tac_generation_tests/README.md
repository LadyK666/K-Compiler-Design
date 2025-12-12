# 中间代码生成（TAC）测试目录

## 目录结构

### 细粒度功能测试 (unit_tests/)
- `T01_arithmetic_expressions.txt` - 算术表达式TAC生成
- `T02_comparison_expressions.txt` - 比较表达式TAC生成
- `T03_assignment.txt` - 赋值语句TAC生成
- `T04_compound_assignment.txt` - 复合赋值TAC生成
- `T05_increment_decrement.txt` - 自增自减TAC生成
- `T06_if_statement.txt` - if语句TAC生成
- `T07_while_statement.txt` - while语句TAC生成
- `T08_for_statement.txt` - for语句TAC生成
- `T09_break_continue.txt` - break/continue TAC生成
- `T10_array_access.txt` - 数组访问TAC生成
- `T11_io_statements.txt` - I/O语句TAC生成

### 综合测试 (integration_tests/)
- `I01_complete_program.txt` - 完整程序TAC生成测试

## 测试说明

### 细粒度功能测试
每个测试文件测试一个特定的TAC生成功能，验证生成的TAC指令正确性。

### 综合测试
测试完整程序的TAC生成，验证所有功能协同工作。

## 运行测试

使用编译器对测试文件进行TAC生成：
```bash
./parser < test_file.txt
```

预期输出应包含：
- 原始TAC代码
- 优化后的TAC代码（如果启用优化）

## 注意

TAC生成在语义分析通过后才会执行，因此所有测试文件必须是语义正确的。

