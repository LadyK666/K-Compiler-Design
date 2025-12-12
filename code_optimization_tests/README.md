# 代码优化测试目录

## 目录结构

### 细粒度功能测试 (unit_tests/)
- `O01_constant_folding.txt` - 常量折叠优化测试
- `O02_common_subexpression.txt` - 公共子表达式消除测试
- `O03_loop_invariant.txt` - 循环不变代码外提测试
- `O04_peephole.txt` - 窥孔优化测试

### 综合测试 (integration_tests/)
- `I01_multi_optimization.txt` - 多种优化综合测试

## 测试说明

### 细粒度功能测试
每个测试文件专门测试一种优化技术，验证优化效果。

### 综合测试
测试多种优化技术协同工作，验证优化效果叠加。

## 运行测试

使用编译器对测试文件进行优化：
```bash
./parser < test_file.txt
```

预期输出应包含：
- 原始TAC代码
- 优化详情（每种优化的应用情况）
- 优化后的TAC代码

## 注意

代码优化在TAC生成后执行，所有测试文件必须是语义正确的。

