#include "stdio.h"      // printf
#include "parser.tab.h" // bison 生成的头文件
#include "def.h"        // AST 结构与节点类型
#include "stdlib.h"     // malloc/free
#include "string.h"     // strcpy

/**************** AST 构造与展示函数 *********************/

// 创建一个基础节点，初始化通用字段
ASTNode* createNode(NodeType type) {
    ASTNode* node = (ASTNode*)malloc(sizeof(ASTNode)); // 申请节点内存
    node->type = type;                                 // 记录节点类型
    node->decl_type = VAL_UNKNOWN;                     // 默认声明类型未知
    node->left = NULL;                                 // 左孩子置空
    node->right = NULL;                                // 右孩子置空
    node->child = NULL;                                // child 置空
    return node;
}

// 创建整型常量节点
ASTNode* createIntNode(int val) {
    ASTNode* node = createNode(NODE_INT_CONST); // 基础节点
    node->value.int_val = val;                  // 填充整型值
    return node;
}

// 创建浮点常量节点
ASTNode* createFloatNode(float val) {
    ASTNode* node = createNode(NODE_FLOAT_CONST); // 基础节点
    node->value.float_val = val;                  // 填充浮点值
    return node;
}

// 创建变量节点
ASTNode* createVarNode(char* name) {
    ASTNode* node = createNode(NODE_VAR);    // 基础节点
    strcpy(node->value.var_name, name);      // 记录变量名
    return node;
}

// 输出缩进，方便层次化打印
void printIndent(int indent) {
    int i;
    for (i = 0; i < indent; i++) {
        printf("    "); // 四空格缩进
    }
}

// 递归打印 AST，按照节点类型输出含义
void displayAST(ASTNode* node, int indent) {
    if (node == NULL) return;
    
    switch (node->type) {
        case NODE_PROGRAM:
            displayAST(node->child, indent);                        // 程序节点只打印子树
            break;
            
        case NODE_STMLIST:
            displayAST(node->child, indent);                        // 打印当前语句
            if (node->right != NULL) {                              // 还有后续语句
                displayAST(node->right, indent);
            }
            break;
            
        case NODE_EXP_STMT:
            printIndent(indent);
            printf("表达式语句：\n");
            displayAST(node->child, indent + 1);                    // 打印子表达式
            break;
            
        case NODE_COMP_STMT:
            displayAST(node->child, indent);                        // 复合语句打印内部
            break;
            
        case NODE_SCAN_STMT:
            printIndent(indent);
            printf("输入变量：%s\n", node->value.var_name);
            break;
            
        case NODE_PRINT_STMT:
            printIndent(indent);
            printf("输出表达式:\n");
            displayAST(node->child, indent + 1);                    // 打印被输出的表达式
            break;
            
        case NODE_IF_STMT:
            printIndent(indent);
            printf("条件语句(if_then)：\n");
            printIndent(indent + 1);
            printf("条件：\n");
            displayAST(node->child, indent + 2);                    // 条件
            printIndent(indent + 1);
            printf("if子句：\n");
            displayAST(node->left, indent + 2);                     // then 分支
            break;
            
        case NODE_IF_ELSE_STMT:
            printIndent(indent);
            printf("条件语句(if_then_else)：\n");
            printIndent(indent + 1);
            printf("条件：\n");
            displayAST(node->child, indent + 2);                    // 条件
            printIndent(indent + 1);
            printf("if子句：\n");
            displayAST(node->left, indent + 2);                     // then 分支
            printIndent(indent + 1);
            printf("else子句：\n");
            displayAST(node->right, indent + 2);                    // else 分支
            break;
            
        case NODE_WHILE_STMT:
            printIndent(indent);
            printf("循环语句：\n");
            printIndent(indent + 1);
            printf("条件：\n");
            displayAST(node->child, indent + 2);                    // 循环条件
            printIndent(indent + 1);
            printf("循环体：\n");
            displayAST(node->left, indent + 2);                     // 循环体
            break;
            
        case NODE_INT_CONST:
            printIndent(indent);
            printf("整型常量：%d\n", node->value.int_val);          // 打印整型值
            break;
            
        case NODE_FLOAT_CONST:
            printIndent(indent);
            printf("浮点常量：%f\n", node->value.float_val);        // 打印浮点值
            break;
            
        case NODE_VAR:
            printIndent(indent);
            printf("变量：%s\n", node->value.var_name);             // 打印变量名
            break;
            
        case NODE_ASSIGN:
            printIndent(indent);
            printf("=\n");
            displayAST(node->left, indent + 1);                     // 左侧
            displayAST(node->right, indent + 1);                    // 右侧
            break;
            
        case NODE_PLUS:
            printIndent(indent);
            printf("+\n");
            displayAST(node->left, indent + 1);                     // 左操作数
            displayAST(node->right, indent + 1);                    // 右操作数
            break;
            
        case NODE_MINUS:
            printIndent(indent);
            printf("-\n");
            displayAST(node->left, indent + 1);
            displayAST(node->right, indent + 1);
            break;
            
        case NODE_STAR:
            printIndent(indent);
            printf("*\n");
            displayAST(node->left, indent + 1);
            displayAST(node->right, indent + 1);
            break;
            
        case NODE_DIV:
            printIndent(indent);
            printf("/\n");
            displayAST(node->left, indent + 1);
            displayAST(node->right, indent + 1);
            break;
            
        case NODE_EQ:
            printIndent(indent);
            printf("==\n");
            displayAST(node->left, indent + 1);
            displayAST(node->right, indent + 1);
            break;
            
        case NODE_NE:
            printIndent(indent);
            printf("!=\n");
            displayAST(node->left, indent + 1);
            displayAST(node->right, indent + 1);
            break;
            
        case NODE_GT:
            printIndent(indent);
            printf(">\n");
            displayAST(node->left, indent + 1);
            displayAST(node->right, indent + 1);
            break;
            
        case NODE_GE:
            printIndent(indent);
            printf(">=\n");
            displayAST(node->left, indent + 1);
            displayAST(node->right, indent + 1);
            break;
            
        case NODE_LT:
            printIndent(indent);
            printf("<\n");
            displayAST(node->left, indent + 1);
            displayAST(node->right, indent + 1);
            break;
            
        case NODE_LE:
            printIndent(indent);
            printf("<=\n");
            displayAST(node->left, indent + 1);
            displayAST(node->right, indent + 1);
            break;
            
        case NODE_UMINUS:
            printIndent(indent);
            printf("单目-\n");
            displayAST(node->left, indent + 1);                     // 被取负的表达式
            break;
            
        case NODE_PAREN_EXP:
            displayAST(node->child, indent);                        // 直接展开括号内
            break;
            
        case NODE_VAR_DECL:
            printIndent(indent);
            if (node->decl_type == VAL_INT) {                       // 区分声明类型
                printf("变量声明(int)：%s\n", node->value.var_name);
            } else if (node->decl_type == VAL_FLOAT) {
                printf("变量声明(float)：%s\n", node->value.var_name);
            } else {
                printf("变量声明(char)：%s\n", node->value.var_name);
            }
            break;
            
        case NODE_ARRAY_DECL:
            printIndent(indent);
            if (node->decl_type == VAL_INT) {                       // 区分数组元素类型
                printf("数组声明(int)：%s\n", node->value.var_name);
            } else if (node->decl_type == VAL_FLOAT) {
                printf("数组声明(float)：%s\n", node->value.var_name);
            } else {
                printf("数组声明(char)：%s\n", node->value.var_name);
            }
            printIndent(indent + 1);
            printf("维度：\n");
            displayAST(node->child, indent + 2);                    // 打印维度节点链
            break;
            
        case NODE_ARRAY_ACCESS:
            printIndent(indent);
            printf("数组访问：\n");
            displayAST(node->left, indent + 1);                     // 数组基
            printIndent(indent + 1);
            printf("索引：\n");
            displayAST(node->right, indent + 2);                    // 索引表达式
            break;
            
        case NODE_PLUS_ASSIGN:
            printIndent(indent);
            printf("+=\n");
            displayAST(node->left, indent + 1);
            displayAST(node->right, indent + 1);
            break;
            
        case NODE_MINUS_ASSIGN:
            printIndent(indent);
            printf("-=\n");
            displayAST(node->left, indent + 1);
            displayAST(node->right, indent + 1);
            break;
            
        case NODE_STAR_ASSIGN:
            printIndent(indent);
            printf("*=\n");
            displayAST(node->left, indent + 1);
            displayAST(node->right, indent + 1);
            break;
            
        case NODE_DIV_ASSIGN:
            printIndent(indent);
            printf("/=\n");
            displayAST(node->left, indent + 1);
            displayAST(node->right, indent + 1);
            break;
            
        case NODE_INCREMENT:
            printIndent(indent);
            printf("++\n");
            displayAST(node->left, indent + 1);
            break;
            
        case NODE_DECREMENT:
            printIndent(indent);
            printf("--\n");
            displayAST(node->left, indent + 1);
            break;
            
        case NODE_FOR_STMT:
            printIndent(indent);
            printf("循环语句(for)：\n");
            printIndent(indent + 1);
            printf("初始化：\n");
            displayAST(node->child, indent + 2);                    // init
            printIndent(indent + 1);
            printf("条件：\n");
            displayAST(node->left, indent + 2);                     // cond
            if (node->right != NULL) {
                printIndent(indent + 1);
                printf("更新：\n");
                displayAST(node->right->child, indent + 2);         // update
                printIndent(indent + 1);
                printf("循环体：\n");
                displayAST(node->right->right, indent + 2);         // body
            }
            break;
            
        case NODE_BREAK_STMT:
            printIndent(indent);
            printf("break语句\n");                                 // break
            break;
            
        case NODE_CONTINUE_STMT:
            printIndent(indent);
            printf("continue语句\n");                              // continue
            break;
            
        case NODE_CHAR_CONST:
            printIndent(indent);
            printf("字符常量：%c\n", (char)node->value.int_val);    // 打印字符
            break;
            
        default:
            break;
    }
}

// 打印源代码（带行号），便于与 AST 对照
void printSourceCode(const char* filename) {
    FILE *fp = fopen(filename, "r");               // 打开源码文件
    if (fp == NULL) {                              // 打不开则直接返回
        return;
    }
    
    printf("========== 源代码 ==========\n");       // 输出分隔线
    char line[1024];                               // 行缓冲
    int lineNum = 1;                               // 行号计数
    while (fgets(line, sizeof(line), fp) != NULL) {// 逐行读取
        printf("%3d: %s", lineNum++, line);        // 打印行号+内容
    }
    printf("==========================\n\n");       // 结束分隔线
    fclose(fp);                                    // 关闭文件
}

/*****************************************************/
