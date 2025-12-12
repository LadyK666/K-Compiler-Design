#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
测试parser语法分析输出格式
"""

import subprocess
import sys
import os

# 测试文件
test_file = "syntax_analysis_tests/unit_tests/S01_variable_declaration.txt"

# 读取测试文件
with open(test_file, 'r', encoding='utf-8') as f:
    input_content = f.read()

print("=" * 60)
print("测试parser语法分析输出格式")
print("=" * 60)
print(f"测试文件: {test_file}")
print(f"文件内容:\n{input_content}")
print("=" * 60)

# 使用cmd方式执行
abs_test_file = os.path.abspath(test_file)
abs_parser = "parser.exe"

cmd = f'cmd /c "{abs_parser}  {abs_test_file}"'
print(f"\n执行命令: {cmd}\n")
print("=" * 60)
print("完整输出:")
print("=" * 60)

process = subprocess.Popen(
    cmd,
    shell=True,
    stdout=subprocess.PIPE,
    stderr=subprocess.STDOUT,
    text=True,
    encoding='utf-8',
    errors='replace',
    cwd=os.getcwd()
)

stdout, stderr = process.communicate(timeout=10)
print(stdout)
print("=" * 60)
print(f"返回码: {process.returncode}")
print(f"输出长度: {len(stdout)}")
print("=" * 60)

# 尝试查找关键标记
print("\n查找关键标记:")
print("-" * 60)
if "抽象语法树" in stdout:
    print("✓ 找到 '抽象语法树'")
    idx = stdout.find("抽象语法树")
    print(f"  位置: {idx}")
    print(f"  上下文: {stdout[max(0, idx-20):idx+100]}")
else:
    print("✗ 未找到 '抽象语法树'")

if "NODE_" in stdout:
    print("✓ 找到 'NODE_'")
    idx = stdout.find("NODE_")
    print(f"  位置: {idx}")
    print(f"  上下文: {stdout[max(0, idx-20):idx+100]}")
else:
    print("✗ 未找到 'NODE_'")

if "变量声明" in stdout:
    print("✓ 找到 '变量声明'")
    idx = stdout.find("变量声明")
    print(f"  位置: {idx}")
    print(f"  上下文: {stdout[max(0, idx-20):idx+100]}")
else:
    print("✗ 未找到 '变量声明'")

if "Grammar Error" in stdout:
    print("✓ 找到 'Grammar Error'")
else:
    print("✗ 未找到 'Grammar Error'")

print("=" * 60)

