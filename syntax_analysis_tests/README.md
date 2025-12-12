# 语法分析测试目录

## 目录结构

### 细粒度功能测试 (unit_tests/)
- `S01_variable_declaration.txt` - 变量声明测试
- `S02_array_declaration.txt` - 数组声明测试
- `S03_arithmetic_expressions.txt` - 算术表达式测试
- `S04_comparison_expressions.txt` - 比较表达式测试
- `S05_assignment_expressions.txt` - 赋值表达式测试
- `S06_compound_assignment.txt` - 复合赋值测试
- `S07_increment_decrement.txt` - 自增自减测试
- `S08_if_statement.txt` - if语句测试
- `S09_while_statement.txt` - while语句测试
- `S10_for_statement.txt` - for语句测试
- `S11_break_continue.txt` - break/continue语句测试
- `S12_compound_statement.txt` - 复合语句测试
- `S13_io_statements.txt` - I/O语句测试

### 异常代码测试 (error_tests/)
- `E01_missing_semicolon.txt` - 缺少分号错误
- `E02_mismatched_braces.txt` - 括号不匹配错误
- `E03_missing_expression.txt` - 缺少表达式错误
- `E04_invalid_array_decl.txt` - 无效数组声明错误
- `E05_missing_keyword.txt` - 缺少关键字错误

### 综合测试 (integration_tests/)
- `I01_complete_program.txt` - 完整程序语法分析测试

## 测试说明

### 细粒度功能测试
每个测试文件只测试一个特定的语法功能，确保该语法结构能被正确解析。

### 异常代码测试
每个测试文件只包含一个语法错误，用于测试语法错误报告机制。

### 综合测试
测试完整程序的语法分析，验证所有语法结构协同工作。

## 运行测试

使用编译器对测试文件进行语法分析：
```bash
./parser < test_file.txt
```

预期输出应包含：
- AST树结构
- 语法错误信息（如果有）

