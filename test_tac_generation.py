#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
中间代码生成（TAC）测试自动化脚本
自动测试 tac_generation_tests 文件夹下的所有测试文件
将结果输出到 CSV 文件
"""

import os
import sys
import subprocess
import csv
import datetime
import re
from pathlib import Path

# 测试文件夹路径
TEST_DIR = "tac_generation_tests"

# 根据操作系统确定parser命令
if sys.platform == "win32":
    # Windows系统，尝试多个可能的命令
    if os.path.exists("parser.exe"):
        PARSER_CMD = "parser.exe"
    elif os.path.exists("parser"):
        PARSER_CMD = "parser"
    else:
        PARSER_CMD = "./parser"
else:
    # Linux/macOS系统
    PARSER_CMD = "./parser"

# CSV输出文件名
OUTPUT_CSV = f"tac_generation_test_results_{datetime.datetime.now().strftime('%Y%m%d_%H%M%S')}.csv"

def get_test_type(folder_name):
    """根据文件夹名称返回测试类型"""
    type_map = {
        "unit_tests": "细粒度功能测试",
        "error_tests": "异常代码测试",
        "integration_tests": "综合测试"
    }
    return type_map.get(folder_name, "未知类型")

def read_file_content(file_path):
    """读取文件内容"""
    try:
        with open(file_path, 'r', encoding='utf-8', errors='replace') as f:
            content = f.read()
        return content.strip()
    except Exception as e:
        return f"读取文件失败: {str(e)}"

def run_parser(test_file):
    """运行 parser 命令并捕获输出"""
    try:
        if sys.platform == "win32":
            # Windows 系统，使用cmd重定向方式（已验证可用）
            abs_test_file = os.path.abspath(test_file)
            abs_parser = os.path.abspath(PARSER_CMD.lstrip('./'))
            if not os.path.exists(abs_parser):
                # 尝试相对路径
                abs_parser = PARSER_CMD
            
            # 使用cmd /c执行重定向命令（与test_lexical_analysis.py一致）
            # 注意：使用两个空格分隔parser和文件路径
            cmd = f'cmd /c "{abs_parser}  {abs_test_file}"'
            process = subprocess.Popen(
                cmd,
                shell=True,
                stdout=subprocess.PIPE,
                stderr=subprocess.STDOUT,  # 合并stderr到stdout
                text=True,
                encoding='utf-8',
                errors='replace',
                cwd=os.getcwd()
            )
            stdout, stderr = process.communicate(timeout=10)
        else:
            # Linux/macOS 系统，使用文件重定向
            with open(test_file, 'r', encoding='utf-8', errors='replace') as f:
                process = subprocess.Popen(
                    [PARSER_CMD],
                    stdin=f,
                    stdout=subprocess.PIPE,
                    stderr=subprocess.STDOUT,  # 合并stderr到stdout
                    text=True,
                    encoding='utf-8',
                    errors='replace'
                )
            stdout, stderr = process.communicate(timeout=10)
        
        # 合并stdout和stderr
        output = stdout if stdout else ""
        if stderr and stderr != stdout:
            output += "\n" + stderr
        
        # 返回输出和返回码
        return_code = process.returncode if process.returncode is not None else 0
        return output.strip(), return_code
        
    except subprocess.TimeoutExpired:
        if 'process' in locals():
            try:
                process.kill()
            except:
                pass
        return "执行超时（超过10秒）", -1
    except FileNotFoundError:
        return f"错误: 找不到可执行文件 {PARSER_CMD}，请确保已编译生成 parser", -1
    except Exception as e:
        return f"执行错误: {str(e)}", -1

def extract_tac_output(output):
    """从完整输出中提取原始TAC部分"""
    if not output:
        return ""
    
    # 根据实际输出格式，原始TAC部分包含：
    # ===== TAC (三地址码，原始) =====
    # ...TAC指令...
    # ===== TAC End =====
    
    # 方法1: 精确匹配原始TAC部分（不包括优化后的TAC）
    # 查找 "===== TAC (三地址码，原始) =====" 到 "===== TAC End =====" 之间的内容
    pattern1 = r'===== TAC \(三地址码，原始\) =====\n(.*?)\n===== TAC End ====='
    match1 = re.search(pattern1, output, re.DOTALL)
    if match1:
        return match1.group(1).strip()
    
    # 方法2: 如果没找到标准格式，尝试查找包含"原始"的TAC部分
    pattern2 = r'===== TAC.*?原始.*?=====\n(.*?)(?=\n===== TAC.*?优化|\n===== TAC End|\n=====.*?=====|$)'
    match2 = re.search(pattern2, output, re.DOTALL)
    if match2:
        tac_content = match2.group(1).strip()
        # 移除可能包含的结束标记
        tac_content = re.sub(r'\n===== TAC End =====.*$', '', tac_content, flags=re.DOTALL).strip()
        if tac_content:
            return tac_content
    
    # 方法3: 查找第一个TAC部分（在优化TAC之前，应该是原始TAC）
    # 确保不包含"优化"关键字
    pattern3 = r'===== TAC.*?=====\n(.*?)(?=\n===== TAC.*?优化|\n===== TAC End|\n=====.*?=====|$)'
    match3 = re.search(pattern3, output, re.DOTALL)
    if match3:
        tac_content = match3.group(1).strip()
        # 确保不包含"优化"关键字，且包含TAC指令特征
        if '优化' not in match3.group(0) and any(keyword in tac_content for keyword in ['=', '+', '-', '*', '/', '==', '!=', '>', '<', '>=', '<=', 'goto', 'if', 'label', '//']):
            return tac_content
    
    # 如果都没找到，返回空字符串
    return ""

def escape_csv_field(text):
    """转义CSV字段中的特殊字符"""
    if not text:
        return ""
    # 替换双引号为两个双引号，并用双引号包裹整个字段
    text = str(text).replace('"', '""')
    # 如果包含换行符、逗号或双引号，需要用双引号包裹
    if '\n' in text or ',' in text or '"' in text:
        return f'"{text}"'
    return text

def main():
    """主函数"""
    print("=" * 60)
    print("中间代码生成（TAC）测试自动化脚本")
    print("=" * 60)
    print(f"测试目录: {TEST_DIR}")
    print(f"解析器命令: {PARSER_CMD}")
    print(f"输出文件: {OUTPUT_CSV}")
    print("=" * 60)
    
    # 检查测试目录是否存在
    if not os.path.exists(TEST_DIR):
        print(f"错误: 测试目录 {TEST_DIR} 不存在！")
        sys.exit(1)
    
    # 检查 parser 是否存在
    parser_path = PARSER_CMD.lstrip('./')
    parser_exists = (
        os.path.exists(parser_path) or 
        os.path.exists(parser_path + ".exe") or
        os.path.exists("./parser") or
        os.path.exists("./parser.exe")
    )
    if not parser_exists:
        print(f"警告: 找不到 parser 可执行文件，请确保已编译生成 parser")
        print(f"尝试的命令: {PARSER_CMD}")
        response = input("是否继续？(y/n): ")
        if response.lower() != 'y':
            sys.exit(1)
    
    # 收集所有测试文件
    test_files = []
    test_dirs = ["unit_tests", "integration_tests"]
    
    for test_dir in test_dirs:
        dir_path = os.path.join(TEST_DIR, test_dir)
        if os.path.exists(dir_path):
            for file in os.listdir(dir_path):
                if file.endswith('.txt'):
                    file_path = os.path.join(dir_path, file)
                    test_files.append((file_path, test_dir, file))
    
    if not test_files:
        print(f"错误: 在 {TEST_DIR} 中未找到任何测试文件！")
        sys.exit(1)
    
    print(f"\n找到 {len(test_files)} 个测试文件")
    print("开始测试...\n")
    
    # 准备CSV数据
    csv_data = []
    csv_data.append([
        "测试文件名",
        "测试类型",
        "文件路径",
        "文件内容",
        "原始TAC输出",
        "返回码",
        "测试状态"
    ])
    
    # 遍历测试文件
    success_count = 0
    fail_count = 0
    
    for idx, (file_path, test_dir, file_name) in enumerate(test_files, 1):
        print(f"[{idx}/{len(test_files)}] 测试: {file_name} ({get_test_type(test_dir)})")
        
        # 读取文件内容
        file_content = read_file_content(file_path)
        
        # 运行 parser
        full_output, return_code = run_parser(file_path)
        
        # 提取原始TAC部分
        tac_output = extract_tac_output(full_output)
        
        # 调试输出
        if tac_output:
            output_preview = tac_output[:100].replace('\n', ' ') if tac_output else "(无TAC输出)"
            print(f"  TAC输出预览: {output_preview}...")
        else:
            print(f"  警告: 未找到原始TAC输出")
            if full_output:
                print(f"  完整输出前100字符: {full_output[:100]}...")
        
        # 判断测试状态
        # TAC生成在语义分析通过后才会执行，所以返回码为0且有TAC输出算成功
        if return_code == 0 and tac_output:
            status = "成功"
            success_count += 1
        else:
            status = "失败/错误"
            fail_count += 1
        
        # 添加到CSV数据（使用相对路径）
        rel_path = os.path.relpath(file_path, os.getcwd())
        csv_data.append([
            file_name,
            get_test_type(test_dir),
            rel_path,
            file_content,
            tac_output,  # 只保存原始TAC部分
            return_code,
            status
        ])
        
        # 显示输出长度（用于调试）
        output_len = len(tac_output) if tac_output else 0
        print(f"  状态: {status} (返回码: {return_code}, TAC输出长度: {output_len} 字符)")
    
    # 写入CSV文件
    print(f"\n正在写入结果到 {OUTPUT_CSV}...")
    try:
        with open(OUTPUT_CSV, 'w', encoding='utf-8-sig', newline='') as f:
            writer = csv.writer(f)
            for row in csv_data:
                # 手动处理每一行，确保正确转义
                escaped_row = [escape_csv_field(cell) for cell in row]
                writer.writerow(escaped_row)
        print(f"结果已保存到: {OUTPUT_CSV}")
    except Exception as e:
        print(f"错误: 写入CSV文件失败: {str(e)}")
        sys.exit(1)
    
    # 输出统计信息
    print("\n" + "=" * 60)
    print("测试完成！")
    print("=" * 60)
    print(f"总测试数: {len(test_files)}")
    print(f"成功: {success_count}")
    print(f"失败/错误: {fail_count}")
    print(f"结果文件: {OUTPUT_CSV}")
    print("=" * 60)

if __name__ == "__main__":
    main()

