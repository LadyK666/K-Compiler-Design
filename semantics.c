#include "stdio.h"   // 标准输入输出，用于日志打印
#include "stdlib.h"  // malloc/free
#include "string.h"  // strcpy/memset
#include "def.h"     // AST 与符号表相关定义

// 全局作用域栈与循环深度
static Scope* g_scopeStack = NULL;  // 作用域链表的栈顶
static int g_loopDepth = 0;         // 当前嵌套的循环深度
static int g_semanticErrors = 0;    // 累计语义错误计数

// 日志辅助：作用域变化时打印
static void log_scope_push(int level) {        // 进入新作用域时调用
    printf("[符号表] 进入作用域 level=%d\n", level);
}

// 日志辅助：退出作用域
static void log_scope_pop(int level) {         // 离开作用域时调用
    printf("[符号表] 离开作用域 level=%d\n", level);
}

static void log_symbol_insert(const char* name, ValueType type, int isArray, int dimCount) { // 插入符号时记录
    const char* t = (type == VAL_INT) ? "int" : (type == VAL_FLOAT) ? "float" : (type == VAL_CHAR) ? "char" : "unknown"; // 文本化类型
    if (isArray) { // 根据是否数组输出不同信息
        printf("[符号表] 声明数组 %s 类型=%s 维度=%d\n", name, t, dimCount);
    } else {
        printf("[符号表] 声明变量 %s 类型=%s\n", name, t);
    }
}

// 记录语义错误并计数
static void report_semantic_error(const char* msg) { // 打印并累加错误
    printf("[语义错误] %s\n", msg);
    g_semanticErrors++;
}

// 作用域管理：压栈一个新作用域
static void pushScope(void) {
    Scope* s = (Scope*)malloc(sizeof(Scope));                    // 分配作用域节点
    s->level = (g_scopeStack ? g_scopeStack->level + 1 : 0);     // 层级 = 顶层 + 1 或 0
    s->symbols = NULL;                                           // 符号链初始化为空
    s->next = g_scopeStack;                                      // 链接到旧栈顶
    g_scopeStack = s;                                            // 更新栈顶
    log_scope_push(s->level);                                    // 打印进入作用域日志
}

// 作用域管理：弹栈并释放当前作用域
static void popScope(void) {
    if (!g_scopeStack) return;                                   // 无作用域则直接返回
    Symbol* sym = g_scopeStack->symbols;                         // 遍历当前作用域符号
    while (sym) {
        Symbol* nxt = sym->next;                                 // 暂存下一个
        free(sym);                                               // 释放当前符号
        sym = nxt;                                               // 前进
    }
    log_scope_pop(g_scopeStack->level);                          // 打印退出作用域日志
    Scope* old = g_scopeStack;                                   // 暂存旧栈顶
    g_scopeStack = g_scopeStack->next;                           // 弹栈
    free(old);                                                   // 释放旧作用域
}

// 当前作用域内查找符号
static Symbol* findSymbolCurrent(const char* name) {
    if (!g_scopeStack) return NULL;               // 无作用域直接返回
    Symbol* s = g_scopeStack->symbols;            // 从当前符号链表起始
    while (s) {
        if (strcmp(s->name, name) == 0) return s; // 命名匹配则返回
        s = s->next;                              // 继续下一个
    }
    return NULL;                                  // 未找到
}

// 沿作用域链查找符号
static Symbol* findSymbol(const char* name) {
    Scope* sc = g_scopeStack;                     // 从当前作用域开始
    while (sc) {
        Symbol* s = sc->symbols;                  // 遍历该作用域的符号链
        while (s) {
            if (strcmp(s->name, name) == 0) return s; // 找到则返回
            s = s->next;                          // 下一个符号
        }
        sc = sc->next;                            // 继续外层作用域
    }
    return NULL;                                  // 整个作用域链未找到
}

// 插入符号，含当前作用域重定义检查
static Symbol* insertSymbol(const char* name, ValueType type, int isArray, int dimCount, int* dims) {
    if (!g_scopeStack) pushScope();                          // 确保存在作用域
    if (findSymbolCurrent(name)) {                           // 检查当前作用域重定义
        report_semantic_error("重复定义标识符");
        return NULL;
    }
    Symbol* s = (Symbol*)malloc(sizeof(Symbol));             // 分配符号
    strcpy(s->name, name);                                   // 设置名称
    s->type = type;                                          // 设置基础类型
    s->isArray = isArray;                                    // 是否数组
    s->dimCount = dimCount;                                  // 维度数量
    s->initialized = 0;                                      // 初始为未初始化
    memset(s->dims, 0, sizeof(s->dims));                     // 清空维度信息
    if (dims && dimCount > 0) {                              // 拷贝维度
        int i;
        for (i = 0; i < dimCount && i < 8; ++i) s->dims[i] = dims[i];
    }
    s->next = g_scopeStack->symbols;                         // 插入链表头
    g_scopeStack->symbols = s;                               // 更新头指针
    log_symbol_insert(name, type, isArray, dimCount);        // 打印符号插入日志
    return s;                                                // 返回新符号
}

// 类型提升：用于算术运算结果
static ValueType promote(ValueType a, ValueType b) {
    if (a == VAL_UNKNOWN || b == VAL_UNKNOWN) return VAL_UNKNOWN; // 不明类型保持未知
    if (a == VAL_FLOAT || b == VAL_FLOAT) return VAL_FLOAT;       // 任何一方 float 提升为 float
    // char 与 int 一并提升为 int
    return VAL_INT;
}

// 数值类型判断
static int is_numeric(ValueType t) {
    return t == VAL_INT || t == VAL_FLOAT || t == VAL_CHAR;       // char 视作数值类型
}

// 把 ArrayDims AST 转成维度数组
static int flattenDims(ASTNode* node, int* dims, int maxDims) {
    if (!node || !dims) return 0;                                  // 空节点直接返回
    if (node->type == NODE_INT_CONST) {                            // 叶子整型常量即单维
        if (maxDims > 0) dims[0] = node->value.int_val;            // 记录维度大小
        return 1;                                                  // 返回已填维度数
    }
    if (node->type == NODE_STMLIST) {                              // STMLIST 链接多维
        int count = flattenDims(node->child, dims, maxDims);       // 处理左侧链
        if (count < maxDims && node->right && node->right->type == NODE_INT_CONST) {
            dims[count] = node->right->value.int_val;              // 追加右侧维度
            return count + 1;                                      // 返回新增后的维度数
        }
        return count;                                              // 返回累积维度
    }
    return 0;                                                      // 其他节点视为无维度
}

// 表达式类型推导（checkInit 控制未初始化使用是否报错）
static TypeInfo evalExp(ASTNode* node, int checkInit);

// 标记左值已初始化（赋值 / ++ -- 后）
static void mark_initialized(ASTNode* lval, int force) {
    if (!lval) return;                                   // 空节点直接返回
    if (lval->type == NODE_VAR) {                        // 普通变量左值
        Symbol* s = findSymbol(lval->value.var_name);    // 查符号
        if (s) s->initialized = 1;                       // 标记已初始化
    } else if (lval->type == NODE_ARRAY_ACCESS) {        // 数组元素左值
        ASTNode* base = lval->left;                      // 找到最底层基变量
        while (base && base->type == NODE_ARRAY_ACCESS) base = base->left;
        if (base && base->type == NODE_VAR) {
            Symbol* s = findSymbol(base->value.var_name);
            if (s) s->initialized = 1;                   // 记为已初始化
        }
    }
    (void)force;                                         // 预留参数未用
}

// 构造一个默认的类型信息对象
static TypeInfo typeinfo_default(void) {
    TypeInfo t;                                          // 临时结构
    t.base = VAL_UNKNOWN;                                // 基础类型未知
    t.isArray = 0;                                       // 非数组
    t.dimCount = 0;                                      // 无维度
    memset(t.dims, 0, sizeof(t.dims));                   // 维度清零
    t.isLValue = 0;                                      // 默认非左值
    t.initialized = 0;                                   // 默认未初始化
    return t;                                            // 返回默认对象
}

// 递归求表达式类型并执行语义检查
static TypeInfo evalExp(ASTNode* node, int checkInit) {
    if (!node) return typeinfo_default();                   // 空节点返回默认
    TypeInfo res = typeinfo_default();                      // 初始化返回值
    switch (node->type) {
        case NODE_INT_CONST:                                // 整型常量
            res.base = VAL_INT; res.isArray = 0; res.isLValue = 0; res.initialized = 1;
            break;
        case NODE_FLOAT_CONST:                              // 浮点常量
            res.base = VAL_FLOAT; res.isArray = 0; res.isLValue = 0; res.initialized = 1;
            break;
        case NODE_CHAR_CONST:                               // 字符常量
            res.base = VAL_CHAR; res.isArray = 0; res.isLValue = 0; res.initialized = 1;
            break;
        case NODE_VAR: {                                    // 变量引用
            Symbol* s = findSymbol(node->value.var_name);   // 查符号表
            if (!s) {
                report_semantic_error("使用了未声明的变量");
                break;
            }
            res.base = s->type;                             // 基础类型
            res.isArray = s->isArray;                       // 是否数组
            res.dimCount = s->dimCount;                     // 维度数
            memcpy(res.dims, s->dims, sizeof(res.dims));    // 维度拷贝
            res.isLValue = 1;                               // 变量是左值
            res.initialized = s->initialized;               // 初始化状态
            if (checkInit && !s->initialized) {             // 需要检查且未初始化
                report_semantic_error("变量在初始化前被使用");
            }
            break;
        }
        case NODE_ARRAY_ACCESS: {                           // 数组访问
            TypeInfo base = evalExp(node->left, checkInit); // 解析基数组
            TypeInfo idx = evalExp(node->right, 1);         // 解析下标，强制检查初始化
            if (idx.isArray) {                              // 下标不能是数组
                report_semantic_error("数组下标必须是整型表达式");
            } else if (idx.base == VAL_FLOAT) {            // 下标不能是浮点数
                report_semantic_error("数组下标不能是浮点数");
            } else if (!is_numeric(idx.base)) {             // 下标必须是数值类型（int或char）
                report_semantic_error("数组下标必须是整型表达式");
            }
            if (!base.isArray) {                            // 基不是数组
                report_semantic_error("对非数组对象使用下标");
                break;
            }
            if (base.dimCount <= 0) {                       // 缺少维度信息
                report_semantic_error("数组维度信息缺失");
                break;
            }
            res = base;                                     // 结果继承基类型
            res.dimCount -= 1;                              // 维度减少一层
            int i;
            for (i = 0; i < res.dimCount; ++i) res.dims[i] = base.dims[i + 1]; // 维度左移
            res.isArray = (res.dimCount > 0);               // 剩余维度决定是否仍为数组
            res.isLValue = 1;                               // 数组元素是左值
            break;
        }
        case NODE_ASSIGN:
        case NODE_PLUS_ASSIGN:
        case NODE_MINUS_ASSIGN:
        case NODE_STAR_ASSIGN:
        case NODE_DIV_ASSIGN: {                             // 各类赋值
            TypeInfo lhs = evalExp(node->left, 0);          // 左值不检查初始化
            TypeInfo rhs = evalExp(node->right, 1);         // 右值需检查
            if (!lhs.isLValue) report_semantic_error("赋值左侧不是可赋值表达式");
            if (lhs.isArray) report_semantic_error("不能直接给数组整体赋值");
            if (!is_numeric(lhs.base) || !is_numeric(rhs.base)) report_semantic_error("赋值操作数必须是数值类型");
            if (lhs.base == VAL_INT && rhs.base == VAL_FLOAT) report_semantic_error("不允许将 float 赋给 int"); // 窄化
            else if (lhs.base == VAL_CHAR && rhs.base != VAL_CHAR) report_semantic_error("只能向 char 赋 char 值");
            res.base = lhs.base;                            // 结果类型与左值一致
            res.isArray = 0;
            res.isLValue = 0;
            mark_initialized(node->left, 1);                // 赋值后标记初始化
            break;
        }
        case NODE_INCREMENT:
        case NODE_DECREMENT: {                              // 自增自减
            TypeInfo t = evalExp(node->left, 0);            // 左值不检查初始化
            if (!t.isLValue) report_semantic_error("自增自减需要左值");
            if (t.isArray) report_semantic_error("不能对数组整体自增自减");
            if (!is_numeric(t.base)) report_semantic_error("自增自减操作数必须为数值类型");
            res = t;
            res.isLValue = 0;                               // 自增结果非左值
            mark_initialized(node->left, 1);                // 视为已初始化
            break;
        }
        case NODE_PLUS:
        case NODE_MINUS:
        case NODE_STAR:
        case NODE_DIV: {                                    // 算术运算
            TypeInfo a = evalExp(node->left, 1);
            TypeInfo b = evalExp(node->right, 1);
            if (a.isArray || b.isArray) report_semantic_error("数组不能直接参与算术运算");
            if (!is_numeric(a.base) || !is_numeric(b.base)) report_semantic_error("算术运算需要数值类型");
            res.base = promote(a.base, b.base);             // 类型提升
            res.isLValue = 0;                               // 算术表达式结果不是左值
            break;
        }
        case NODE_EQ: case NODE_NE: case NODE_GT: case NODE_GE: case NODE_LT: case NODE_LE: { // 比较
            TypeInfo a = evalExp(node->left, 1);
            TypeInfo b = evalExp(node->right, 1);
            if (a.isArray || b.isArray) report_semantic_error("数组不能直接参与比较");
            if (!is_numeric(a.base) || !is_numeric(b.base)) report_semantic_error("比较运算需要数值类型");
            res.base = VAL_INT;                             // 比较结果视为 int（布尔）
            res.isArray = 0;
            res.isLValue = 0;                               // 比较表达式结果不是左值
            break;
        }
        case NODE_UMINUS: {                                 // 取负
            TypeInfo t = evalExp(node->left, 1);
            if (t.isArray) report_semantic_error("数组不能取负");
            if (!is_numeric(t.base)) report_semantic_error("取负需要数值类型");
            res = t;
            res.isLValue = 0;
            break;
        }
        case NODE_PAREN_EXP:                                // 括号表达式
            res = evalExp(node->child, checkInit);
            break;
        default:
            break;                                          // 其他不处理
    }
    return res;                                             // 返回类型信息
}

// 语句/节点分析调度声明
static void analyzeNode(ASTNode* node);

// 顺序遍历语句列表
static void analyzeStmtList(ASTNode* node) {
    if (!node) return;
    analyzeNode(node->child);  // 处理当前
    analyzeNode(node->right);  // 处理后续
}

// 进入新作用域分析复合语句
static void analyzeCompSt(ASTNode* node) {
    pushScope();               // 新作用域
    analyzeNode(node->child);  // 分析内部
    popScope();                // 作用域结束
}

// for 语句：处理 init/cond/update/body，并管理作用域和循环深度
static void analyzeFor(ASTNode* node) {
    // for 节点结构：child=初始化Stmt，left=条件Exp，right->child=更新Stmt，right->right=循环体
    pushScope();
    g_loopDepth++;
    analyzeNode(node->child);
    if (node->left) {
        TypeInfo cond = evalExp(node->left, 1);
        if (cond.isArray || !is_numeric(cond.base)) report_semantic_error("for条件应为数值/布尔表达式");
    }
    if (node->right && node->right->child) analyzeNode(node->right->child);
    if (node->right && node->right->right) analyzeNode(node->right->right);
    g_loopDepth--;
    popScope();
}

// 根据节点类型分发具体语义检查逻辑
static void analyzeNode(ASTNode* node) {
    if (!node) return;                             // 空节点直接返回
    switch (node->type) {                          // 分派节点类型
        case NODE_PROGRAM:                         // 程序根节点
            pushScope();                           // 进入全局作用域
            analyzeNode(node->child);              // 处理程序体
            popScope();                            // 退出全局作用域
            break;
        case NODE_STMLIST:                         // 语句链表
            analyzeStmtList(node);                 // 顺序遍历 child/right
            break;
        case NODE_COMP_STMT:                       // 复合语句
            analyzeCompSt(node);                   // 新作用域处理内部
            break;
        case NODE_EXP_STMT:                        // 表达式语句
            evalExp(node->child, 1);               // 仅做表达式检查
            break;
        case NODE_SCAN_STMT: {                     // 输入语句
            Symbol* s = findSymbol(node->value.var_name); // 查变量符号
            if (!s) report_semantic_error("scan 作用于未声明变量"); // 未声明
            else if (s->isArray) report_semantic_error("scan 不支持对整个数组操作"); // 不允许数组整体
            else s->initialized = 1;               // 标记为已初始化
            break;
        }
        case NODE_PRINT_STMT: {                    // 输出语句
            TypeInfo t = evalExp(node->child, 1);  // 计算输出表达式类型
            if (t.isArray) report_semantic_error("print 不支持直接输出数组"); // 禁止数组
            break;
        }
        case NODE_IF_STMT:                         // if
        case NODE_IF_ELSE_STMT: {                  // if-else
            TypeInfo cond = evalExp(node->child, 1);      // 检查条件
            if (cond.isArray || !is_numeric(cond.base))   // 条件必须数值/布尔
                report_semantic_error("if 条件应为数值/布尔表达式");
            analyzeNode(node->left);                      // then 分支
            if (node->type == NODE_IF_ELSE_STMT)          // else 分支（如有）
                analyzeNode(node->right);
            break;
        }
        case NODE_WHILE_STMT: {                   // while 循环
            TypeInfo cond = evalExp(node->child, 1);      // 检查条件
            if (cond.isArray || !is_numeric(cond.base))   // 条件必须数值/布尔
                report_semantic_error("while 条件应为数值/布尔表达式");
            g_loopDepth++;                                 // 进入循环
            analyzeNode(node->left);                       // 循环体
            g_loopDepth--;                                 // 退出循环
            break;
        }
        case NODE_FOR_STMT:                        // for 循环
            analyzeFor(node);                      // 专用处理
            break;
        case NODE_BREAK_STMT:                      // break
            if (g_loopDepth <= 0) report_semantic_error("break 只能出现在循环内部"); // 非循环内报错
            break;
        case NODE_CONTINUE_STMT:                   // continue
            if (g_loopDepth <= 0) report_semantic_error("continue 只能出现在循环内部"); // 非循环内报错
            break;
        case NODE_VAR_DECL: {                      // 变量声明
            ValueType tp = node->decl_type;        // 声明的类型
            insertSymbol(node->value.var_name, tp, 0, 0, NULL); // 插入符号表
            break;
        }
        case NODE_ARRAY_DECL: {                    // 数组声明
            ValueType tp = node->decl_type;        // 元素类型
            int dims[8] = {0};                     // 维度数组
            int cnt = flattenDims(node->child, dims, 8); // 提取维度
            if (cnt <= 0) report_semantic_error("数组声明缺少维度"); // 无维度
            int i;
            for (i = 0; i < cnt; ++i) {            // 校验每个维度
                if (dims[i] <= 0) report_semantic_error("数组维度必须为正整数");
            }
            insertSymbol(node->value.var_name, tp, 1, cnt, dims); // 插入数组符号
            break;
        }
        case NODE_ASSIGN:
        case NODE_PLUS_ASSIGN:
        case NODE_MINUS_ASSIGN:
        case NODE_STAR_ASSIGN:
        case NODE_DIV_ASSIGN:
        case NODE_INCREMENT:
        case NODE_DECREMENT:
        case NODE_PLUS:
        case NODE_MINUS:
        case NODE_STAR:
        case NODE_DIV:
        case NODE_EQ:
        case NODE_NE:
        case NODE_GT:
        case NODE_GE:
        case NODE_LT:
        case NODE_LE:
        case NODE_UMINUS:
        case NODE_PAREN_EXP:
        case NODE_ARRAY_ACCESS:
        case NODE_VAR:
        case NODE_INT_CONST:
        case NODE_FLOAT_CONST:
        case NODE_CHAR_CONST:
            evalExp(node, 1);                      // 统一走表达式检查
            break;
        default:                                   // 未覆盖类型
            break;
    }
}

// 语义检查入口：遍历 AST 并打印错误计数
void checkSemantics(ASTNode* root) {
    g_semanticErrors = 0;
    g_loopDepth = 0;
    // 主程序外层作用域已经由 analyzeNode 内部创建
    analyzeNode(root);
    printf("[语义检查完成] 发现错误数量: %d\n", g_semanticErrors);
}

// 获取语义错误数量（供外部判断是否继续生成代码）
int getSemanticErrorCount(void) {
    return g_semanticErrors;
}

