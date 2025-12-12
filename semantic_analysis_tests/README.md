# 语义分析测试目录

## 目录结构

### 细粒度功能测试 (unit_tests/)
#### 声明与初始化（规则1-3）
- `M01_duplicate_declaration.txt` - 重复定义标识符（规则1）
- `M02_undeclared_variable.txt` - 使用未声明的变量（规则2）
- `M03_uninitialized_variable.txt` - 变量在初始化前被使用（规则3）

#### 数组相关（规则4-10）
- `M04_array_dimension_zero.txt` - 数组维度必须为正整数（规则4）
- `M05_non_array_subscript.txt` - 对非数组对象使用下标（规则5）
- `M06_float_array_index.txt` - 数组下标不能是浮点数（规则6）
- `M07_array_as_index.txt` - 数组下标不能是数组（规则7）
- `M08_array_arithmetic.txt` - 数组不能直接参与算术运算（规则9）
- `M09_array_comparison.txt` - 数组不能直接参与比较（规则9）
- `M10_array_assignment.txt` - 不能直接给数组整体赋值（规则10）

#### 赋值检查（规则11-14）
- `M11_non_lvalue_assignment.txt` - 赋值左侧不是可赋值表达式（规则11）
- `M12_float_to_int.txt` - 不允许将float赋给int（规则13）
- `M13_int_to_char.txt` - 只能向char赋char值（规则14）

#### 运算检查（规则15-17）
- `M14_non_lvalue_increment.txt` - 自增自减需要左值（规则15）
- `M15_array_increment.txt` - 不能对数组整体自增自减（规则16）

#### 控制流检查（规则18-22）
- `M16_if_array_condition.txt` - if条件应为数值/布尔表达式（规则18）
- `M17_while_array_condition.txt` - while条件应为数值/布尔表达式（规则19）
- `M18_for_array_condition.txt` - for条件应为数值/布尔表达式（规则20）
- `M19_break_outside_loop.txt` - break只能出现在循环内部（规则21）
- `M20_continue_outside_loop.txt` - continue只能出现在循环内部（规则22）

#### I/O检查（规则23-25）
- `M21_print_array.txt` - print不支持直接输出数组（规则23）
- `M22_scan_undeclared.txt` - scan作用于未声明变量（规则24）
- `M23_scan_array.txt` - scan不支持对整个数组操作（规则25）

### 异常代码测试 (error_tests/)
每个测试文件对应一条语义规则，只包含一个语义错误：
- `E01_rule01.txt` - 规则1：重复定义
- `E02_rule02.txt` - 规则2：未声明变量
- `E03_rule03.txt` - 规则3：未初始化变量
- `E04_rule04.txt` - 规则4：数组维度为0
- `E05_rule05.txt` - 规则5：非数组使用下标
- `E06_rule06.txt` - 规则6：浮点数作为数组下标
- `E07_rule07.txt` - 规则7：数组作为数组下标
- `E08_rule09.txt` - 规则9：数组参与算术运算
- `E09_rule10.txt` - 规则10：数组整体赋值
- `E10_rule11.txt` - 规则11：非左值赋值
- `E11_rule13.txt` - 规则13：float赋给int
- `E12_rule14.txt` - 规则14：int赋给char
- `E13_rule15.txt` - 规则15：非左值自增
- `E14_rule16.txt` - 规则16：数组自增
- `E15_rule18.txt` - 规则18：if条件为数组
- `E16_rule19.txt` - 规则19：while条件为数组
- `E17_rule20.txt` - 规则20：for条件为数组
- `E18_rule21.txt` - 规则21：循环外break
- `E19_rule22.txt` - 规则22：循环外continue
- `E20_rule23.txt` - 规则23：print数组
- `E21_rule24.txt` - 规则24：scan未声明变量
- `E22_rule25.txt` - 规则25：scan数组

### 综合测试 (integration_tests/)
- `I01_semantically_correct.txt` - 语义正确的完整程序

## 测试说明

### 细粒度功能测试
每个测试文件测试一条特定的语义规则，确保该规则能被正确检测。

### 异常代码测试
每个测试文件只包含一个语义错误，用于测试错误报告机制。

### 综合测试
测试语义正确的完整程序，验证所有语义检查协同工作。

## 运行测试

使用编译器对测试文件进行语义分析：
```bash
./parser < test_file.txt
```

预期输出应包含：
- 符号表变化过程
- 语义错误信息（如果有）

