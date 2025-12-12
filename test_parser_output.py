#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
测试parser输出捕获
"""

import subprocess
import sys
import os

# 测试文件
test_file = "lexical_analysis_tests/unit_tests/L01_keywords.txt"

# 读取测试文件
with open(test_file, 'r', encoding='utf-8') as f:
    input_content = f.read()

print("=" * 60)
print("测试1: 使用communicate()")
print("=" * 60)

# 方法1: 使用communicate
process = subprocess.Popen(
    "parser.exe",
    shell=True,
    stdin=subprocess.PIPE,
    stdout=subprocess.PIPE,
    stderr=subprocess.STDOUT,
    text=True,
    encoding='utf-8',
    errors='replace',
    cwd=os.getcwd()
)

stdout, stderr = process.communicate(input=input_content, timeout=10)
print(f"返回码: {process.returncode}")
print(f"输出长度: {len(stdout) if stdout else 0}")
print(f"输出前200字符:")
print(stdout[:200] if stdout else "(无输出)")
print("=" * 60)

print("\n测试2: 使用文件重定向（cmd方式）")
print("=" * 60)

# 方法2: 使用cmd重定向
import tempfile
with tempfile.NamedTemporaryFile(mode='w', encoding='utf-8', delete=False, suffix='.txt') as tmp:
    tmp.write(input_content)
    tmp_path = tmp.name

try:
    cmd = f'cmd /c "parser.exe < {tmp_path}"'
    process2 = subprocess.Popen(
        cmd,
        shell=True,
        stdout=subprocess.PIPE,
        stderr=subprocess.STDOUT,
        text=True,
        encoding='utf-8',
        errors='replace',
        cwd=os.getcwd()
    )
    stdout2, stderr2 = process2.communicate(timeout=10)
    print(f"返回码: {process2.returncode}")
    print(f"输出长度: {len(stdout2) if stdout2 else 0}")
    print(f"输出前200字符:")
    print(stdout2[:200] if stdout2 else "(无输出)")
finally:
    os.unlink(tmp_path)

print("=" * 60)

