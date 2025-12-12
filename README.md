# 编译器项目 - 完整实现

## 📚 项目概述

这是一个完整的编译器实现项目，从词法分析到目标代码生成，涵盖了编译原理的核心内容。该编译器针对一种类C语言的简化语言，能够生成可在MIPS模拟器（MARS/QTSPIM）上运行的汇编代码。

---

## 🛠️ 编译工具链

```
源代码 (.txt)
    ↓
[词法分析 - Flex]
    ↓
Token序列
    ↓
[语法分析 - Bison]
    ↓
抽象语法树 (AST)
    ↓
[语义分析]
    ↓
带类型信息的AST + 符号表
    ↓
[中间代码生成]
    ↓
三地址码 (TAC)
    ↓
[代码优化]
    ↓
优化后的TAC
    ↓
[MIPS代码生成]
    ↓
MIPS汇编代码 (.asm)
    ↓
[MARS/QTSPIM]
    ↓
可执行程序
```

---

## 📋 一级目录结构

```
step2/
├── 核心源代码文件
│   ├── lex.l                    # Flex词法分析器定义
│   ├── parser.y                 # Bison语法分析器定义
│   ├── def.h                    # 数据结构和函数声明
│   ├── displayAST.c             # AST构建和显示
│   ├── semantics.c              # 语义分析和符号表管理
│   ├── codegen.c                # TAC生成和优化
│   ├── mips_gen.c               # MIPS目标代码生成
│   └── Makefile                 # 编译配置
│
├── 编译生成文件
│   ├── parser.exe               # 编译器可执行文件（Windows）
│   ├── parser                   # 编译器可执行文件（Linux/macOS）
│   ├── lex.yy.c                  # Flex生成的词法分析器代码
│   ├── parser.tab.c             # Bison生成的语法分析器代码
│   ├── parser.tab.h             # Bison生成的头文件
│   ├── optimized.tac             # 优化后的三地址码输出
│   └── output.asm               # MIPS汇编代码输出
│
├── 测试文件夹
│   ├── lexical_analysis_tests/      # 词法分析测试用例
│   ├── syntax_analysis_tests/       # 语法分析测试用例
│   ├── semantic_analysis_tests/      # 语义分析测试用例
│   ├── tac_generation_tests/        # 中间代码生成测试用例
│   ├── code_optimization_tests/     # 代码优化测试用例
│   └── mips_generation_tests/        # MIPS代码生成测试用例
│
├── 测试脚本
│   ├── test_lexical_analysis.py      # 词法分析自动化测试脚本
│   ├── test_syntax_analysis.py       # 语法分析自动化测试脚本
│   ├── test_semantic_analysis.py     # 语义分析自动化测试脚本
│   ├── test_tac_generation.py       # TAC生成自动化测试脚本
│   ├── test_mips_generation.py       # MIPS生成自动化测试脚本
│   └── codetohtml.py                 # 代码转HTML工具
│
├── 测试结果文件
│   ├── lexical_analysis_test_results_final.csv
│   ├── syntax_analysis_test_results_final.csv
│   ├── semantic_analysis_test_results_final.csv
│   ├── tac_generation_test_results_final.csv
│   ├── code_optimization_test_results_final.csv
│   └── mips_generation_test_results_final.csv
│
├── 文档文件夹
│   └── 其余文件/                  # 其他文档和测试文件
│
└── README.md                   # 项目说明文档（本文档）
```

---

## 🚀 快速开始

### 环境要求

- **操作系统**：Linux / macOS / Windows (with MinGW/Cygwin)
- **编译器**：GCC 4.8+
- **工具链**：
  - Flex 2.5+
  - Bison 3.0+
  - Make

### 编译项目

```bash
# 清理旧文件
make clean

# 编译编译器
make
```

编译成功后会生成可执行文件 `parser`。

### 运行编译器

**执行命令**：
```bash
./parser 测试代码文件名
```

**说明**：
- 在Windows系统上，使用 `parser.exe` 或 `parser`（取决于编译生成的可执行文件名）
- 在Linux/macOS系统上，使用 `./parser`
- 测试代码文件应为 `.txt` 格式的源代码文件

**示例**：
```bash
# Linux/macOS
./parser lexical_analysis_tests/unit_tests/L01_keywords.txt

# Windows
parser.exe lexical_analysis_tests\unit_tests\L01_keywords.txt
```

**输出说明**：
- 编译器会在控制台输出完整的编译过程信息
- 如果编译成功，会生成 `optimized.tac`（优化后的三地址码）和 `output.asm`（MIPS汇编代码）
- 如果存在语义错误，会在控制台输出错误信息，但不会生成目标代码

### 输出文件

运行后会生成以下文件：

| 文件名 | 说明 |
|--------|------|
| `optimized.tac` | 优化后的三地址码 |
| `output.asm` | MIPS汇编代码 |

### 在MARS中运行

1. 下载并启动MARS模拟器
2. File → Open → 选择 `output.asm`
3. Run → Assemble
4. Run → Go
5. 在Run I/O窗口查看输入输出

---


## 🔍 编译器各阶段输出

### 1. 源代码显示

```
========== 源代码 ==========
int a;
int b;
{
    a = 10;
    b = 20;
    print a + b;
}
============================
```

### 2. 词法分析结果

```
========================================
词法分析 (Lexical Analysis)
========================================
+----+-------------------+----------------+
| 行 | Token             | Token类型      |
+----+-------------------+----------------+
|  1 | int               | TYPE_INT       |
|  1 | a                 | ID             |
|  1 | ;                 | SEMI           |
...
```

### 3. 抽象语法树（AST）

```
========== 抽象语法树 ==========
NODE_PROGRAM
  NODE_STMLIST
    NODE_VAR_DECL (int)
      Var: a
    NODE_STMLIST
      NODE_VAR_DECL (int)
        Var: b
...
```

### 4. 语义分析

```
========================================
语义分析 (Semantic Analysis)
========================================
[符号表] 进入作用域 level=0
[符号表] 声明变量 a 类型=int
[符号表] 声明变量 b 类型=int
...
```

### 5. 三地址码（原始）

```
===== TAC (三地址码，原始) =====
t0 = 10
a = t0
t1 = 20
b = t1
t2 = a + b
print t2
===== TAC End =====
```

### 6. 代码优化

```
===== 优化详情 =====
[常量折叠] t0 = 10 → (常量)
[常量折叠] t1 = 20 → (常量)
[CSE] t2 = a + b (新建)
====================
```

### 7. 三地址码（优化后）

```
===== TAC (三地址码，优化后) =====
a = 10
b = 20
t2 = a + b
print t2
===== TAC End =====
```

### 8. MIPS汇编代码

```
========================================
生成MIPS汇编代码
========================================

[MIPS生成] 代码已写入: output.asm
[寄存器分配] 使用了 3 个变量
[栈管理] 最大栈大小: 12 字节
========================================
```

---


## 👥 贡献者

- LadyK

---

---

**最后更新**：2025年12月12日  
**版本**：v1.0

