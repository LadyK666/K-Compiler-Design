import pandas as pd
import os

def csv_to_html_report(csv_path, output_path="report.html"):
    """
    读取指定CSV文件，并将其三列内容 ('测试文件名', '文件内容', '原始TAC输出')
    转化为一个美观的HTML报告文件。

    Args:
        csv_path (str): 输入的CSV文件路径。
        output_path (str): 输出的HTML文件路径。
    """
    try:
        # 1. 读取CSV文件
        df = pd.read_csv(csv_path)

        # 检查所需的列是否存在
        required_columns = ['测试文件名', '文件内容', '原始TAC输出']
        if not all(col in df.columns for col in required_columns):
            missing_cols = [col for col in required_columns if col not in df.columns]
            print(f"❌ 错误：CSV文件缺少以下必需的列：{missing_cols}")
            return

        # 2. HTML 头部和 CSS 样式 (高级感、美观、加粗)
        html_start = f"""
<!DOCTYPE html>
<html lang="zh-CN">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>报告展示：{os.path.basename(csv_path)}</title>
    <style>
        /* 全局字体和背景 */
        body {{
            font-family: 'Segoe UI', Tahoma, Geneva, Verdana, sans-serif;
            background-color: #f4f7f6; /* 轻柔的背景色 */
            color: #333;
            line-height: 1.6;
            padding: 20px;
        }}

        /* 报告标题 */
        h1 {{
            color: #1a237e; /* 品牌蓝色 */
            text-align: center;
            font-weight: 700; /* 字体加粗 */
            margin-bottom: 30px;
            letter-spacing: 1px;
            border-bottom: 3px solid #e0e0e0;
            padding-bottom: 10px;
        }}

        /* 容器（列表） */
        .report-container {{
            display: flex;
            flex-direction: column;
            gap: 20px; /* 框之间的间隔 */
            max-width: 1200px;
            margin: 0 auto; /* 居中 */
        }}

        /* 单行框（Card） */
        .data-card {{
            background-color: #ffffff; /* 白色背景 */
            border-radius: 12px; /* 圆角 */
            box-shadow: 0 6px 20px rgba(0, 0, 0, 0.1); /* 柔和阴影，增加高级感 */
            padding: 25px;
            display: grid;
            grid-template-columns: 1fr 1fr; /* 分为左右两列 */
            gap: 20px;
            transition: transform 0.3s ease, box-shadow 0.3s ease;
        }}
        
        /* 鼠标悬停效果 */
        .data-card:hover {{
            transform: translateY(-5px);
            box-shadow: 0 10px 30px rgba(0, 0, 0, 0.15);
        }}

        /* 字段标题样式 */
        .field-title {{
            font-weight: 700; /* 字体加粗 */
            color: #00796b; /* 强调色 */
            margin-bottom: 8px;
            display: block;
            font-size: 1.1em;
            text-transform: uppercase;
        }}

        /* 内容文本样式 */
        .content-text {{
            font-family: 'Consolas', 'Courier New', monospace; /* 程序员字体 */
            white-space: pre-wrap; /* 保留空白和换行 */
            background-color: #f0f4f8; /* 内容背景色 */
            padding: 15px;
            border-radius: 6px;
            border-left: 5px solid #00796b; /* 左侧强调线 */
            overflow: auto; /* 防止内容过长溢出 */
            max-height: 400px; /* 限制高度 */
        }}
        
        /* 左上角 '测试文件名' 特殊布局 */
        .header-box {{
            grid-column: 1 / -1; /* 跨越整个两列 */
            background-color: #e8eaf6; /* 浅蓝色背景 */
            padding: 10px 15px;
            border-radius: 8px;
            margin-bottom: 10px;
            font-size: 1.2em;
            font-weight: 700;
            color: #1a237e;
            border-left: 8px solid #3f51b5;
        }}
        
        /* 左右内容容器 */
        .left-content, .right-content {{
            /* 保持 flex 布局或 block 布局，适应内容 */
        }}
    </style>
</head>
<body>
    <h1>CSV 数据分析报告：{os.path.basename(csv_path)}</h1>
    <div class="report-container">
"""

        # 3. 遍历数据行，生成 HTML 内容
        content_rows = ""
        for index, row in df.iterrows():
            # 获取数据
            filename = row.get('测试文件名', 'N/A')
            file_content = str(row.get('文件内容', 'N/A'))
            tac_output = str(row.get('原始TAC输出', 'N/A'))

            # 生成一个数据框/卡片
            card_html = f"""
        <div class="data-card">
            <div class="header-box">
                测试文件名: {filename}
            </div>
            
            <div class="left-content">
                <span class="field-title">文件内容</span>
                <pre class="content-text">{file_content}</pre>
            </div>
            
            <div class="right-content">
                <span class="field-title">原始TAC输出</span>
                <pre class="content-text">{tac_output}</pre>
            </div>
        </div>
"""
            content_rows += card_html

        # 4. HTML 结尾
        html_end = """
    </div>
</body>
</html>
"""

        # 5. 写入最终的 HTML 文件
        final_html = html_start + content_rows + html_end
        with open(output_path, 'w', encoding='utf-8') as f:
            f.write(final_html)

        print(f"✅ 成功：HTML报告已生成，文件路径：{os.path.abspath(output_path)}")

    except FileNotFoundError:
        print(f"❌ 错误：未找到文件 {csv_path}")
    except pd.errors.EmptyDataError:
        print(f"❌ 错误：CSV文件为空")
    except Exception as e:
        print(f"❌ 发生错误: {e}")

# --- 示例运行 ---
if __name__ == "__main__":
    # 请将 'your_data.csv' 替换为您实际的 CSV 文件名
    CSV_FILE = 'tac_generation_test_results_final.csv'
    OUTPUT_FILE = 'analysis_report.html'
    
    # 假设您的 CSV 文件在运行脚本的同一目录下
    # 您可以根据需要修改路径
    csv_to_html_report(CSV_FILE, OUTPUT_FILE)