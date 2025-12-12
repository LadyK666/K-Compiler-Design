/***********定义结点类型和给出函数说明*********/

#ifndef DEF_H
#define DEF_H

// AST节点类型枚举：描述语法树中所有可能的节点种类
typedef enum {
    NODE_PROGRAM,      // 程序节点
    NODE_STMLIST,      // 语句列表节点
    NODE_EXP_STMT,     // 表达式语句节点
    NODE_COMP_STMT,    // 复合语句节点
    NODE_SCAN_STMT,    // 输入语句节点
    NODE_PRINT_STMT,   // 输出语句节点
    NODE_IF_STMT,      // if语句节点
    NODE_IF_ELSE_STMT, // if-else语句节点
    NODE_WHILE_STMT,   // while语句节点
    NODE_FOR_STMT,     // for语句节点
    NODE_BREAK_STMT,   // break语句节点
    NODE_CONTINUE_STMT,// continue语句节点
    NODE_INT_CONST,    // 整型常量节点
    NODE_FLOAT_CONST,  // 浮点常量节点
    NODE_CHAR_CONST,   // 字符常量节点
    NODE_VAR,          // 变量节点
    NODE_ARRAY_ACCESS, // 数组访问节点
    NODE_ASSIGN,       // 赋值表达式节点
    NODE_PLUS_ASSIGN,  // += 复合赋值节点
    NODE_MINUS_ASSIGN, // -= 复合赋值节点
    NODE_STAR_ASSIGN,  // *= 复合赋值节点
    NODE_DIV_ASSIGN,   // /= 复合赋值节点
    NODE_INCREMENT,    // ++ 自增节点
    NODE_DECREMENT,    // -- 自减节点
    NODE_PLUS,         // 加法表达式节点
    NODE_MINUS,        // 减法表达式节点
    NODE_STAR,         // 乘法表达式节点
    NODE_DIV,          // 除法表达式节点
    NODE_EQ,           // 等于表达式节点
    NODE_NE,           // 不等于表达式节点
    NODE_GT,           // 大于表达式节点
    NODE_GE,           // 大于等于表达式节点
    NODE_LT,           // 小于表达式节点
    NODE_LE,           // 小于等于表达式节点
    NODE_UMINUS,       // 单目减表达式节点
    NODE_PAREN_EXP,    // 括号表达式节点
    NODE_VAR_DECL,     // 变量声明节点
    NODE_ARRAY_DECL    // 数组声明节点
} NodeType;

// 基础类型：语义分析时使用的简单类型系统
typedef enum {
    VAL_UNKNOWN = -1,
    VAL_INT = 0,
    VAL_FLOAT = 1,
    VAL_CHAR = 2
} ValueType;

// 表达式类型信息：用于类型推导与检查
typedef struct {
    ValueType base;     // 基础类型
    int isArray;        // 是否为数组
    int dimCount;       // 数组维度数量
    int dims[8];        // 数组各维度大小（最多8维，够用即可）
    int isLValue;       // 是否为可赋值左值
    int initialized;    // 是否已被赋值（用于简单未初始化检查）
} TypeInfo;

// 符号表条目：记录单个标识符的属性
typedef struct Symbol {
    char name[32];
    ValueType type;
    int isArray;
    int dimCount;
    int dims[8];
    int initialized;
    struct Symbol* next;
} Symbol;

// 作用域（栈式管理）：形成作用域链
typedef struct Scope {
    int level;
    Symbol* symbols;
    struct Scope* next;
} Scope;

// AST节点结构：左右孩子 + child 组成多叉树表达
typedef struct ASTNode {
    NodeType type;              // 节点类型
    union {
        int int_val;            // 整型常量的值
        float float_val;        // 浮点常量的值
        char var_name[32];      // 变量名
    } value;
    ValueType decl_type;        // 声明类型（用于变量/数组声明，避免与 var_name 共用 union）
    struct ASTNode *left;       // 左子树
    struct ASTNode *right;      // 右子树
    struct ASTNode *child;      // 子节点（用于语句列表、条件语句等）
} ASTNode;

// 功能开关：是否在显示语法树前输出源代码（1 开启，0 关闭）
#define SHOW_SOURCE_CODE 1

// 函数声明
ASTNode* createNode(NodeType type);
ASTNode* createIntNode(int val);
ASTNode* createFloatNode(float val);
ASTNode* createVarNode(char* name);
void displayAST(ASTNode* node, int indent);
void printSourceCode(const char* filename);

// 语义分析接口
void checkSemantics(ASTNode* root);
int getSemanticErrorCount(void);   // 获取语义错误数量

// 中间代码生成接口
void generateTAC(ASTNode* root);   // 生成并打印三地址码

// MIPS目标代码生成接口
void generateMIPSFromTAC(void);    // 从TAC生成MIPS汇编代码

// 词法分析接口
const char* getTokenName(int token);           // 获取token名称
void performLexicalAnalysis(const char* filename);  // 执行词法分析

#endif

/*********************************************/
