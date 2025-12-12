# MIPS目标代码生成测试目录

## 目录结构

### 细粒度功能测试 (unit_tests/)
- `M01_arithmetic_instructions.txt` - 算术指令生成测试
- `M02_comparison_instructions.txt` - 比较指令生成测试
- `M03_assignment_instructions.txt` - 赋值指令生成测试
- `M04_control_flow.txt` - 控制流指令生成测试
- `M05_register_allocation.txt` - 寄存器分配测试
- `M06_stack_management.txt` - 栈管理测试
- `M07_io_syscalls.txt` - I/O系统调用测试

### 综合测试 (integration_tests/)
- `I01_complete_program.txt` - 完整程序MIPS生成测试

## 测试说明

### 细粒度功能测试
每个测试文件测试一个特定的MIPS生成功能，验证生成的MIPS代码正确性。

### 综合测试
测试完整程序的MIPS生成，验证所有功能协同工作。

## 运行测试

使用编译器对测试文件进行MIPS生成：
```bash
./parser < test_file.txt
```

预期输出应包含：
- 生成的MIPS汇编代码（保存在output.asm）

## 验证方法

生成的MIPS代码可以在MARS或QTSPIM模拟器上运行验证。

## 注意

MIPS生成在TAC优化后执行，所有测试文件必须是语义正确的。

