# 词法分析测试目录

## 目录结构

### 细粒度功能测试 (unit_tests/)
- `L01_keywords.txt` - 关键字识别测试
- `L02_identifiers.txt` - 标识符识别测试
- `L03_integer_constants.txt` - 整数常量识别测试
- `L04_float_constants.txt` - 浮点数常量识别测试
- `L05_char_constants.txt` - 字符常量识别测试
- `L06_operators.txt` - 运算符识别测试
- `L07_delimiters.txt` - 分隔符识别测试
- `L08_line_comment.txt` - 行注释识别测试
- `L09_block_comment.txt` - 块注释识别测试
- `L10_whitespace.txt` - 空白字符处理测试

### 异常代码测试 (error_tests/)
- `E01_unrecognized_char.txt` - 无法识别的字符错误
- `E02_invalid_char_constant.txt` - 无效字符常量错误
- `E03_invalid_float.txt` - 无效浮点数格式错误

### 综合测试 (integration_tests/)
- `I01_complete_program.txt` - 完整程序词法分析测试

## 测试说明

### 细粒度功能测试
每个测试文件只测试一个特定的词法功能，确保该功能正常工作。

### 异常代码测试
每个测试文件只包含一个词法错误，用于测试错误报告机制。

### 综合测试
测试完整程序的词法分析，验证所有功能协同工作。

## 运行测试

使用编译器对测试文件进行词法分析：
```bash
./parser < test_file.txt
```

预期输出应包含：
- 词法分析结果（Token, Token类型）
- 错误信息（如果有）

