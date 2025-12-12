#include "stdio.h"    // 输出 TAC
#include "stdlib.h"   // malloc/free/realloc
#include "string.h"   // strcpy/snprintf/strncpy/memset
#include "def.h"      // AST 定义

// 三地址码结构
typedef struct {
    char op[8];       // 运算/指令：+, -, *, /, ==, !=, >, >=, <, <=, =, += 等
    char arg1[64];    // 第一个操作数
    char arg2[64];    // 第二个操作数
    char res[64];     // 结果/目标（或条件跳转目标）
    char label[32];   // 标签名（label 指令用）
    int isLabel;      // 是否为标签指令
} TAC;

// TAC 动态数组
typedef struct {
    TAC* data;
    int sz;
    int cap;
} TACList;

// 简单三地址码生成：深度优先遍历 AST，先生成原始 TAC，再做优化。
// 仅在语义检查通过后调用。

typedef struct {
    char place[32];   // 暂存结果名（临时或变量名）
} ExprVal;

static TACList g_tac = {0};           // 原始 TAC
static TACList g_optimized_tac = {0}; // 优化后的TAC（供MIPS生成使用）
static int temp_id = 0;               // 临时变量计数
static int label_id = 0;              // 标签计数

// 标签栈：用于管理循环的continue和break标签
typedef struct {
    char continue_label[32];  // continue跳转目标（循环开始/更新标签）
    char break_label[32];    // break跳转目标（循环结束标签）
} LoopLabel;

static LoopLabel g_loop_stack[64];    // 循环标签栈
static int g_loop_stack_top = -1;      // 栈顶索引（-1表示空栈）

static void tacReserve(TACList* l, int need) {
    if (l->cap >= need) return;
    int ncap = l->cap ? l->cap * 2 : 64;
    while (ncap < need) ncap *= 2;
    l->data = (TAC*)realloc(l->data, ncap * sizeof(TAC));
    l->cap = ncap;
}

static void emit(const char* op, const char* res, const char* a1, const char* a2, const char* label, int isLabel) {
    tacReserve(&g_tac, g_tac.sz + 1);
    TAC* t = &g_tac.data[g_tac.sz++];
    memset(t, 0, sizeof(*t));
    if (op) strncpy(t->op, op, sizeof(t->op) - 1);
    if (res) strncpy(t->res, res, sizeof(t->res) - 1);
    if (a1) strncpy(t->arg1, a1, sizeof(t->arg1) - 1);
    if (a2) strncpy(t->arg2, a2, sizeof(t->arg2) - 1);
    if (label) strncpy(t->label, label, sizeof(t->label) - 1);
    t->isLabel = isLabel;
}

// 向指定列表追加一条 TAC
static void emitTo(TACList* l, const char* op, const char* res, const char* a1, const char* a2, const char* label, int isLabel) {
    tacReserve(l, l->sz + 1);
    TAC* t = &l->data[l->sz++];
    memset(t, 0, sizeof(*t));
    if (op) strncpy(t->op, op, sizeof(t->op) - 1);
    if (res) strncpy(t->res, res, sizeof(t->res) - 1);
    if (a1) strncpy(t->arg1, a1, sizeof(t->arg1) - 1);
    if (a2) strncpy(t->arg2, a2, sizeof(t->arg2) - 1);
    if (label) strncpy(t->label, label, sizeof(t->label) - 1);
    t->isLabel = isLabel;
}

static void newTemp(char* buf, size_t n) { snprintf(buf, n, "t%d", temp_id++); }
static void newLabel(char* buf, size_t n) { snprintf(buf, n, "L%d", label_id++); }

// 标签栈操作
static void pushLoopLabels(const char* continue_label, const char* break_label) {
    if (g_loop_stack_top < 63) {
        g_loop_stack_top++;
        strncpy(g_loop_stack[g_loop_stack_top].continue_label, continue_label, sizeof(g_loop_stack[g_loop_stack_top].continue_label) - 1);
        strncpy(g_loop_stack[g_loop_stack_top].break_label, break_label, sizeof(g_loop_stack[g_loop_stack_top].break_label) - 1);
    }
}

static void popLoopLabels(void) {
    if (g_loop_stack_top >= 0) {
        g_loop_stack_top--;
    }
}

static int getLoopLabels(char* continue_label, size_t continue_n, char* break_label, size_t break_n) {
    if (g_loop_stack_top < 0) {
        return 0;  // 栈为空，没有循环
    }
    strncpy(continue_label, g_loop_stack[g_loop_stack_top].continue_label, continue_n - 1);
    strncpy(break_label, g_loop_stack[g_loop_stack_top].break_label, break_n - 1);
    return 1;  // 成功获取
}

// 前置声明
static ExprVal genExp(ASTNode* node);
static void genStmt(ASTNode* node);
static void genStmtList(ASTNode* node);

// 生成数组访问的字符串表示（支持多维嵌套访问）
static void stringifyArrayAccess(ASTNode* node, char* buf, size_t n) {
    if (node->type == NODE_ARRAY_ACCESS) {
        char base[64];
        stringifyArrayAccess(node->left, base, sizeof(base));
        ExprVal idx = genExp(node->right);
        snprintf(buf, n, "%s[%s]", base, idx.place);
    } else if (node->type == NODE_VAR) {
        snprintf(buf, n, "%s", node->value.var_name);
    } else {
        snprintf(buf, n, "/*invalid_idx*/");
    }
}

// 生成表达式，返回保存结果的名字
static ExprVal genExp(ASTNode* node) {
    ExprVal ev = {0};
    if (!node) return ev;
    switch (node->type) {
        case NODE_INT_CONST:
            snprintf(ev.place, sizeof(ev.place), "%d", node->value.int_val);
            break;
        case NODE_FLOAT_CONST:
            snprintf(ev.place, sizeof(ev.place), "%g", node->value.float_val);
            break;
        case NODE_CHAR_CONST:
            snprintf(ev.place, sizeof(ev.place), "'%c'", (char)node->value.int_val);
            break;
        case NODE_VAR:
            snprintf(ev.place, sizeof(ev.place), "%s", node->value.var_name);
            break;
        case NODE_ARRAY_ACCESS: {
            stringifyArrayAccess(node, ev.place, sizeof(ev.place));
            break;
        }
        case NODE_ASSIGN:
        case NODE_PLUS_ASSIGN:
        case NODE_MINUS_ASSIGN:
        case NODE_STAR_ASSIGN:
        case NODE_DIV_ASSIGN: {
            // 左值
            char lbuf[64];
            stringifyArrayAccess(node->left, lbuf, sizeof(lbuf));
            // 右值
            ExprVal r = genExp(node->right);
            const char* op = (node->type == NODE_ASSIGN) ? "=" :
                              (node->type == NODE_PLUS_ASSIGN) ? "+=" :
                              (node->type == NODE_MINUS_ASSIGN) ? "-=" :
                              (node->type == NODE_STAR_ASSIGN) ? "*=" : "/=";
            emit(op, lbuf, r.place, "", NULL, 0);                 // lbuf op r
            snprintf(ev.place, sizeof(ev.place), "%s", lbuf); // 结果仍为左值
            break;
        }
        case NODE_INCREMENT:
        case NODE_DECREMENT: {
            char lbuf[64];
            stringifyArrayAccess(node->left, lbuf, sizeof(lbuf));
            const char* op = (node->type == NODE_INCREMENT) ? "++" : "--";
            emit(op, lbuf, "", "", NULL, 0);
            snprintf(ev.place, sizeof(ev.place), "%s", lbuf);
            break;
        }
        case NODE_PLUS:
        case NODE_MINUS:
        case NODE_STAR:
        case NODE_DIV:
        case NODE_EQ:
        case NODE_NE:
        case NODE_GT:
        case NODE_GE:
        case NODE_LT:
        case NODE_LE: {
            ExprVal a = genExp(node->left);
            ExprVal b = genExp(node->right);
            newTemp(ev.place, sizeof(ev.place));
            const char* op = NULL;
            switch (node->type) {
                case NODE_PLUS: op = "+"; break;
                case NODE_MINUS: op = "-"; break;
                case NODE_STAR: op = "*"; break;
                case NODE_DIV: op = "/"; break;
                case NODE_EQ: op = "=="; break;
                case NODE_NE: op = "!="; break;
                case NODE_GT: op = ">"; break;
                case NODE_GE: op = ">="; break;
                case NODE_LT: op = "<"; break;
                case NODE_LE: op = "<="; break;
                default: op = "?"; break;
            }
            emit(op, ev.place, a.place, b.place, NULL, 0);        // res = a op b
            break;
        }
        case NODE_UMINUS: {
            ExprVal t = genExp(node->left);
            newTemp(ev.place, sizeof(ev.place));
            emit("uminus", ev.place, t.place, "", NULL, 0);        // res = -t
            break;
        }
        case NODE_PAREN_EXP:
            ev = genExp(node->child);
            break;
        default:
            snprintf(ev.place, sizeof(ev.place), "/*unsupported*/");
            break;
    }
    return ev;
}

// 生成语句
static void genStmt(ASTNode* node) {
    if (!node) return;
    switch (node->type) {
        case NODE_EXP_STMT:
            genExp(node->child);                       // 仅生成表达式的 TAC
            break;
        case NODE_SCAN_STMT:
            emit("scan", node->value.var_name, "", "", NULL, 0);
            break;
        case NODE_PRINT_STMT: {
            ExprVal v = genExp(node->child);
            emit("print", v.place, "", "", NULL, 0);
            break;
        }
        case NODE_IF_STMT:
        case NODE_IF_ELSE_STMT: {
            ExprVal cond = genExp(node->child);
            char l_then[16], l_else[16], l_end[16];
            newLabel(l_then, sizeof(l_then));
            newLabel(l_end, sizeof(l_end));
            if (node->type == NODE_IF_ELSE_STMT) newLabel(l_else, sizeof(l_else));
            if (node->type == NODE_IF_ELSE_STMT) {
                emit("if", l_then, cond.place, "", NULL, 0);   // if cond goto then
                emit("goto", l_else, "", "", NULL, 0);         // goto else
                emit("label", "", "", "", l_then, 1);          // then:
                genStmt(node->left);                           // then
                emit("goto", l_end, "", "", NULL, 0);          // goto end
                emit("label", "", "", "", l_else, 1);          // else:
                genStmt(node->right);                          // else
                emit("label", "", "", "", l_end, 1);           // end:
            } else {
                emit("if", l_then, cond.place, "", NULL, 0);   // if cond goto then
                emit("goto", l_end, "", "", NULL, 0);          // goto end
                emit("label", "", "", "", l_then, 1);          // then:
                genStmt(node->left);                           // then
                emit("label", "", "", "", l_end, 1);           // end:
            }
            break;
        }
        case NODE_WHILE_STMT: {
            char l_cond[16], l_body[16], l_end[16];
            newLabel(l_cond, sizeof(l_cond));
            newLabel(l_body, sizeof(l_body));
            newLabel(l_end, sizeof(l_end));
            // 压入循环标签：continue跳转到条件检查，break跳转到结束
            pushLoopLabels(l_cond, l_end);
            emit("label", "", "", "", l_cond, 1);              // cond:
            ExprVal cond = genExp(node->child);
            emit("if", l_body, cond.place, "", NULL, 0);       // if cond goto body
            emit("goto", l_end, "", "", NULL, 0);              // goto end
            emit("label", "", "", "", l_body, 1);              // body:
            genStmt(node->left);                               // 循环体
            emit("goto", l_cond, "", "", NULL, 0);             // back to cond
            emit("label", "", "", "", l_end, 1);               // end:
            // 弹出循环标签
            popLoopLabels();
            break;
        }
        case NODE_FOR_STMT: {
            // child=init stmt, left=cond exp, right->child=update stmt, right->right=body
            char l_cond[16], l_body[16], l_update[16], l_end[16];
            newLabel(l_cond, sizeof(l_cond));
            newLabel(l_body, sizeof(l_body));
            newLabel(l_update, sizeof(l_update));
            newLabel(l_end, sizeof(l_end));
            // 压入循环标签：continue跳转到更新标签，break跳转到结束
            pushLoopLabels(l_update, l_end);
            genStmt(node->child);                               // init
            emit("label", "", "", "", l_cond, 1);               // cond:
            ExprVal cond = genExp(node->left);                  // cond
            emit("if", l_body, cond.place, "", NULL, 0);        // if cond goto body
            emit("goto", l_end, "", "", NULL, 0);               // goto end
            emit("label", "", "", "", l_body, 1);               // body:
            genStmt(node->right->right);                        // body
            emit("label", "", "", "", l_update, 1);             // update:
            genStmt(node->right->child);                        // update
            emit("goto", l_cond, "", "", NULL, 0);              // back to cond
            emit("label", "", "", "", l_end, 1);                // end:
            // 弹出循环标签
            popLoopLabels();
            break;
        }
        case NODE_BREAK_STMT: {
            char continue_label[32], break_label[32];
            if (getLoopLabels(continue_label, sizeof(continue_label), break_label, sizeof(break_label))) {
                // 从栈中获取break标签，生成跳转指令
                emit("goto", break_label, "", "", NULL, 0);
            } else {
                // 栈为空，说明break不在循环内（语义检查应该已经报错）
                emit("//", "break", "(error: not in loop)", "", NULL, 0);
            }
            break;
        }
        case NODE_CONTINUE_STMT: {
            char continue_label[32], break_label[32];
            if (getLoopLabels(continue_label, sizeof(continue_label), break_label, sizeof(break_label))) {
                // 从栈中获取continue标签，生成跳转指令
                emit("goto", continue_label, "", "", NULL, 0);
            } else {
                // 栈为空，说明continue不在循环内（语义检查应该已经报错）
                emit("//", "continue", "(error: not in loop)", "", NULL, 0);
            }
            break;
        }
        case NODE_COMP_STMT:
            genStmtList(node->child);
            break;
        case NODE_STMLIST:
            genStmtList(node);
            break;
        case NODE_VAR_DECL:
        case NODE_ARRAY_DECL:
            // 声明在此处不生成代码，仅保留
            emit("//", "decl", node->value.var_name, "", NULL, 0);
            break;
        default:
            // 其他节点按表达式处理尝试生成
            genExp(node);
            break;
    }
}

static void genStmtList(ASTNode* node) {
    if (!node) return;
    genStmt(node->child);
    genStmt(node->right);
}

// ---------- 优化工具 ----------

// 判断是否为纯二元运算，便于 CSE/DAG
static int isPureBinary(const char* op) {
    return strcmp(op, "+") == 0 || strcmp(op, "-") == 0 ||
           strcmp(op, "*") == 0 || strcmp(op, "/") == 0 ||
           strcmp(op, "==") == 0 || strcmp(op, "!=") == 0 ||
           strcmp(op, ">") == 0 || strcmp(op, ">=") == 0 ||
           strcmp(op, "<") == 0 || strcmp(op, "<=") == 0;
}

// 常量判断
static int isNumber(const char* s) {
    if (!s || !*s) return 0;
    char* end = NULL;
    strtod(s, &end);
    return end && *end == '\0';
}

// 常量折叠
static int foldBinary(const char* op, const char* a, const char* b, char* out, size_t n) {
    double x = strtod(a, NULL);
    double y = strtod(b, NULL);
    double r = 0.0;
    int handled = 1;
    if (strcmp(op, "+") == 0) r = x + y;
    else if (strcmp(op, "-") == 0) r = x - y;
    else if (strcmp(op, "*") == 0) r = x * y;
    else if (strcmp(op, "/") == 0) r = x / y;
    else if (strcmp(op, "==") == 0) r = (x == y);
    else if (strcmp(op, "!=") == 0) r = (x != y);
    else if (strcmp(op, ">") == 0) r = (x > y);
    else if (strcmp(op, ">=") == 0) r = (x >= y);
    else if (strcmp(op, "<") == 0) r = (x < y);
    else if (strcmp(op, "<=") == 0) r = (x <= y);
    else handled = 0;
    if (!handled) return 0;
    if (r == (int)r) snprintf(out, n, "%d", (int)r);
    else snprintf(out, n, "%g", r);
    return 1;
}

// 局部 DAG/CSE + 常量折叠 + 简单窥孔
// 可用表达式记录
typedef struct { char key[200]; char res[64]; } ExprEntry;

// 优化日志记录
typedef struct {
    char* logs[512];
    int count;
} OptLog;

static OptLog g_optLog = {0};

static void logOpt(const char* msg) {
    if (g_optLog.count < 512) {
        g_optLog.logs[g_optLog.count++] = strdup(msg);
    }
}

static void clearOptLog(void) {
    for (int i = 0; i < g_optLog.count; ++i) {
        free(g_optLog.logs[i]);
    }
    g_optLog.count = 0;
}

static void killExprs(ExprEntry* exprs, int* exprCount, const char* name) {
    int w = 0;
    for (int r = 0; r < *exprCount; ++r) {
        if (strstr(exprs[r].key, name)) continue;
        if (w != r) exprs[w] = exprs[r];
        w++;
    }
    *exprCount = w;
}

// 严格按照编译原理教科书的循环不变代码外提规则
// ⑴ 求出循环的所有不变运算
// ⑵ 检查每个不变运算是否满足外提条件
// ⑶ 满足条件的运算外提到前置结点

// 基本块结构（用于LICM分析）
typedef struct {
    int start;      // 基本块开始的TAC索引
    int end;        // 基本块结束的TAC索引（包含）
    int bb_id;      // 基本块编号
} BasicBlock;

// 循环结构（用于LICM分析）
typedef struct {
    int loopStart;      // 循环入口基本块的第一个TAC索引
    int loopEnd;        // 循环出口基本块的最后一个TAC索引
    char startLabel[32]; // 循环入口标签
    int nodes[256];     // 循环中包含的所有TAC索引
    int nodeCount;       // TAC索引数量
    int bbs[64];        // 循环中包含的基本块ID
    int bbCount;        // 基本块数量
    int backEdgeIdx;    // 对应的回边索引
} Loop;

// 辅助：找到TAC指令所在的基本块ID
static int findBasicBlockForTAC_licm(int tacIdx, BasicBlock* bbs, int bbCount) {
    for (int i = 0; i < bbCount; ++i) {
        if (tacIdx >= bbs[i].start && tacIdx <= bbs[i].end) {
            return i;
        }
    }
    return -1;
}

// 辅助：检查基本块bb是否是所有出口结点的必经结点
static int isDominatingAllExits_licm(int bb, int* exitBBs, int exitCount, int D[256][256]) {
    if (exitCount == 0) return 1;  // 没有出口，认为满足条件
    
    // 检查bb是否是每个出口结点的必经结点
    for (int i = 0; i < exitCount; ++i) {
        int exitBB = exitBBs[i];
        if (!D[bb][exitBB]) {
            // bb不是exitBB的必经结点
            return 0;
        }
    }
    return 1;
}

// 辅助：找到循环的所有出口结点（循环内基本块指向循环外基本块的边）
static void findLoopExits_licm(Loop* loop, int cfg[256][16], int* cfgCount, int bbCount, 
                              int* exitBBs, int* exitCount) {
    *exitCount = 0;
    
    // 标记循环内的基本块
    int inLoop[256] = {0};
    for (int i = 0; i < loop->bbCount; ++i) {
        inLoop[loop->bbs[i]] = 1;
    }
    
    // 遍历循环内的所有基本块
    for (int i = 0; i < loop->bbCount; ++i) {
        int bb = loop->bbs[i];
        // 检查该基本块的所有后继
        for (int j = 0; j < cfgCount[bb]; ++j) {
            int succ = cfg[bb][j];
            if (succ >= 0 && !inLoop[succ]) {
                // 后继在循环外，这是一个出口
                // 检查是否已记录
                int exists = 0;
                for (int k = 0; k < *exitCount; ++k) {
                    if (exitBBs[k] == succ) {
                        exists = 1;
                        break;
                    }
                }
                if (!exists && *exitCount < 64) {
                    exitBBs[*exitCount] = succ;
                    (*exitCount)++;
                }
            }
        }
    }
}

// 辅助：检查变量的所有使用点是否只能由指定定值点到达
static int allUsesReachByDef_licm(const TACList* in, Loop* loop, const char* var, int defPos) {
    // 找到循环中所有使用var的位置
    int usePositions[64];
    int useCount = 0;
    
    for (int idx = 0; idx < loop->nodeCount; ++idx) {
        int i = loop->nodes[idx];
        if (i == defPos) continue;  // 跳过定值点本身
        
        const TAC* t = &in->data[i];
        // 检查arg1和arg2是否使用var
        if ((t->arg1[0] && strcmp(t->arg1, var) == 0) ||
            (t->arg2[0] && strcmp(t->arg2, var) == 0)) {
            usePositions[useCount++] = i;
        }
    }
    
    if (useCount == 0) return 1;  // 没有使用点，认为满足条件
    
    // 检查每个使用点：从defPos到usePos之间是否有其他定值
    for (int u = 0; u < useCount; ++u) {
        int usePos = usePositions[u];
        
        // 检查defPos和usePos之间是否有其他定值
        // 简化：检查循环内是否有其他定值（更精确需要路径分析）
        for (int idx = 0; idx < loop->nodeCount; ++idx) {
            int i = loop->nodes[idx];
            if (i == defPos || i == usePos) continue;
            
            const TAC* t = &in->data[i];
            if (t->res[0] && strcmp(t->res, var) == 0 && !t->isLabel &&
                strcmp(t->op, "if") != 0 && strcmp(t->op, "goto") != 0) {
                // 发现其他定值点，但需要判断是否在defPos到usePos的路径上
                // 简化：如果循环内还有其他定值，保守返回0
                // 更精确的实现需要路径分析，这里先简化
                return 0;
            }
        }
    }
    
    return 1;
}

// 辅助：检查变量在循环后是否活跃
static int isLiveAfterLoop_licm(const TACList* in, Loop* loop, const char* var) {
    // 检查循环后的所有TAC指令
    for (int i = loop->loopEnd + 1; i < in->sz; ++i) {
        const TAC* t = &in->data[i];
        
        // 检查是否使用var（作为操作数）
        if ((t->arg1[0] && strcmp(t->arg1, var) == 0) ||
            (t->arg2[0] && strcmp(t->arg2, var) == 0)) {
            return 1;  // 在循环后使用，活跃
        }
        
        // 如果var被重新定值，则之后的使用不影响活跃性
        // 但这里简化处理：只要循环后有任何使用就认为活跃
    }
    
    // 临时变量（以 t 开头且后续是数字）通常不活跃
    if (var[0] == 't' && var[1] >= '0' && var[1] <= '9') {
        return 0;
    }
    
    // 保守假设：非临时变量可能活跃
    return 1;
}


static void loopInvariantCodeMotion(const TACList* in, TACList* out, int* hoistCount) {
    out->sz = 0;
    *hoistCount = 0;
    
    // ========== 旧的循环识别方法（通过回跳 goto）==========
    // 注释掉，保留作为参考
    /*
    typedef struct { 
        int loopStart;
        int loopEnd;
        char startLabel[32];
    } Loop;
    Loop loops[32];
    int loopCount = 0;
    
    for (int i = 0; i < in->sz; ++i) {
        const TAC* t = &in->data[i];
        if (strcmp(t->op, "goto") == 0) {
            for (int j = 0; j < i; ++j) {
                const TAC* label = &in->data[j];
                if (label->isLabel && strcmp(label->label, t->res) == 0) {
                    if (loopCount < 32) {
                        loops[loopCount].loopStart = j;
                        loops[loopCount].loopEnd = i;
                        strcpy(loops[loopCount].startLabel, label->label);
                        loopCount++;
                    }
                    break;
                }
            }
        }
    }
    */
    
    // ========== 新的循环识别方法（基于必经结点和回边）==========
    
    // 步骤1: 构建基本块
    BasicBlock bbs[256];
    int bbCount = 0;
    
    // 标记所有标签位置和跳转目标
    int labelPos[256];  // 标签名到TAC索引的映射（简化：用索引代替）
    char labelNames[256][32];
    int labelCount = 0;
    
    // 第一遍：识别所有标签
    for (int i = 0; i < in->sz; ++i) {
        const TAC* t = &in->data[i];
        if (t->isLabel && t->label[0]) {
            labelPos[labelCount] = i;
            strcpy(labelNames[labelCount], t->label);
            labelCount++;
        }
    }
    
    // 第二遍：划分基本块
    // 基本块开始于：程序开始、标签、跳转指令之后
    int bbStart = 0;
    int leader[256];  // 标记哪些TAC是基本块入口
    memset(leader, 0, sizeof(leader));
    
    leader[0] = 1;  // 程序开始是入口
    
    for (int i = 0; i < in->sz; ++i) {
        const TAC* t = &in->data[i];
        
        if (t->isLabel) {
            leader[i] = 1;  // 标签是入口
        }
        
        // 跳转指令之后是入口
        if (strcmp(t->op, "goto") == 0 || strcmp(t->op, "if") == 0) {
            if (i + 1 < in->sz) {
                leader[i + 1] = 1;
            }
        }
    }
    
    // 根据leader标记划分基本块
    for (int i = 0; i < in->sz; ++i) {
        if (leader[i] && i > bbStart) {
            // 结束当前基本块
            if (bbCount < 256) {
                bbs[bbCount].start = bbStart;
                bbs[bbCount].end = i - 1;
                bbs[bbCount].bb_id = bbCount;
                bbCount++;
            }
            bbStart = i;
        }
    }
    // 最后一个基本块
    if (bbCount < 256 && bbStart < in->sz) {
        bbs[bbCount].start = bbStart;
        bbs[bbCount].end = in->sz - 1;
        bbs[bbCount].bb_id = bbCount;
        bbCount++;
    }
    
    // 步骤2: 构建控制流图（CFG）
    // 对于每个基本块，找到它的后继
    int cfg[256][16];  // cfg[i][j] = 基本块i的第j个后继，-1表示结束
    int cfgCount[256]; // 每个基本块的后继数量
    memset(cfg, -1, sizeof(cfg));
    memset(cfgCount, 0, sizeof(cfgCount));
    
    // 建立标签名到基本块的映射
    int labelToBB[256];  // 标签索引到基本块ID的映射
    memset(labelToBB, -1, sizeof(labelToBB));
    
    // 方法1：通过基本块的第一条指令映射
    for (int i = 0; i < bbCount; ++i) {
        // 检查基本块开始是否是标签
        const TAC* first = &in->data[bbs[i].start];
        if (first->isLabel && first->label[0]) {
            // 找到标签索引
            for (int j = 0; j < labelCount; ++j) {
                if (strcmp(labelNames[j], first->label) == 0) {
                    labelToBB[j] = i;
                    break;
                }
            }
        }
    }
    
    // 方法2：如果标签在基本块内（不在第一条），也进行映射
    for (int j = 0; j < labelCount; ++j) {
        if (labelToBB[j] < 0) {
            // 标签还没有映射，查找包含该标签的基本块
            int labelTACIdx = labelPos[j];
            for (int i = 0; i < bbCount; ++i) {
                if (labelTACIdx >= bbs[i].start && labelTACIdx <= bbs[i].end) {
                    labelToBB[j] = i;
                    break;
                }
            }
        }
    }
    
    // 构建控制流图：遍历每个基本块，找到它的后继
    for (int i = 0; i < bbCount; ++i) {
        // 检查基本块的最后一条指令
        const TAC* last = &in->data[bbs[i].end];
        if (strcmp(last->op, "goto") == 0) {
            // 无条件跳转：找到目标标签对应的基本块
            int target = -1;
            for (int j = 0; j < labelCount; ++j) {
                if (strcmp(labelNames[j], last->res) == 0) {
                    if (labelToBB[j] >= 0) {
                        target = labelToBB[j];
                    } else {
                        // 标签未映射，尝试直接查找包含该标签的基本块
                        int labelTACIdx = labelPos[j];
                        for (int k = 0; k < bbCount; ++k) {
                            if (labelTACIdx >= bbs[k].start && labelTACIdx <= bbs[k].end) {
                                target = k;
                                labelToBB[j] = k;  // 更新映射
                                break;
                            }
                        }
                    }
                    break;
                }
            }
            if (target >= 0 && cfgCount[i] < 16) {
                cfg[i][cfgCount[i]++] = target;
            }
        } else if (strcmp(last->op, "if") == 0) {
            // 条件跳转：两个后继（真分支和假分支）
            // 真分支（跳转目标）
            int trueTarget = -1;
            for (int j = 0; j < labelCount; ++j) {
                if (strcmp(labelNames[j], last->res) == 0) {
                    if (labelToBB[j] >= 0) {
                        trueTarget = labelToBB[j];
                    } else {
                        // 标签未映射，尝试直接查找包含该标签的基本块
                        int labelTACIdx = labelPos[j];
                        for (int k = 0; k < bbCount; ++k) {
                            if (labelTACIdx >= bbs[k].start && labelTACIdx <= bbs[k].end) {
                                trueTarget = k;
                                labelToBB[j] = k;  // 更新映射
                                break;
                            }
                        }
                    }
                    break;
                }
            }
            if (trueTarget >= 0 && cfgCount[i] < 16) {
                cfg[i][cfgCount[i]++] = trueTarget;
            }
            
            // 假分支（顺序执行的下一个基本块）
            // 需要找到TAC序列中紧跟当前基本块的下一个基本块
            int falseTarget = -1;
            for (int j = 0; j < bbCount; ++j) {
                if (bbs[j].start > bbs[i].end) {
                    falseTarget = j;
                    break;
                }
            }
            if (falseTarget >= 0 && cfgCount[i] < 16) {
                cfg[i][cfgCount[i]++] = falseTarget;
            }
        } else {
            // 顺序执行：下一个基本块
            // 找到TAC序列中紧跟当前基本块的下一个基本块
            int nextTarget = -1;
            for (int j = 0; j < bbCount; ++j) {
                if (bbs[j].start > bbs[i].end) {
                    nextTarget = j;
                    break;
                }
            }
            if (nextTarget >= 0 && cfgCount[i] < 16) {
                cfg[i][cfgCount[i]++] = nextTarget;
            }
        }
    }
    
    // 步骤3: 计算必经结点集 D(n)
    // D[i][j] = 1 表示基本块i支配基本块j（i DOM j）
    int D[256][256];
    memset(D, 0, sizeof(D));
    
    // 使用临时数组存储每个基本块的必经节点集：domSet[n][k] = 1 表示 k 是 n 的必经节点（k DOM n）
    int domSet[256][256];
    memset(domSet, 0, sizeof(domSet));
    
    // (1) 置初值：D(n0) = {n0}，对于n∈(N-{n0})，D(n) = N
    for (int i = 0; i < bbCount; ++i) {
        for (int j = 0; j < bbCount; ++j) {
            domSet[i][j] = 1;  // 初始假设所有结点都是必经的
        }
    }
    // D(0) = {0}
    memset(domSet[0], 0, sizeof(domSet[0]));
    domSet[0][0] = 1;
    
    // (2) 迭代计算必经结点集
    int changed = 1;
    while (changed) {
        changed = 0;
        for (int n = 1; n < bbCount; ++n) {
            // 计算 NEW_D = {n} ∪ (∩ D(p) for p in P(n))
            int newD[256];
            // 初始化为全1（表示所有结点）
            for (int k = 0; k < bbCount; ++k) {
                newD[k] = 1;
            }
            newD[n] = 1;  // 确保n在集合中
            
            // 找到n的所有前驱
            int predCount = 0;
            int preds[64];
            
            for (int p = 0; p < bbCount; ++p) {
                for (int j = 0; j < cfgCount[p]; ++j) {
                    if (cfg[p][j] == n) {
                        // p是n的前驱
                        if (predCount < 64) {
                            preds[predCount++] = p;
                        }
                    }
                }
            }
            
            if (predCount > 0) {
                // 计算所有前驱必经结点集的交集
                // newD = ∩ D(p) for p in preds
                for (int k = 0; k < bbCount; ++k) {
                    for (int pi = 0; pi < predCount; ++pi) {
                        int p = preds[pi];
                        if (!domSet[p][k]) {
                            newD[k] = 0;  // 如果某个前驱的必经结点集中没有k，则k不在交集中
                            break;
                        }
                    }
                }
                // 然后加上n本身：newD = {n} ∪ newD
                newD[n] = 1;
            } else {
                // 如果没有前驱，D(n) = {n}
                for (int k = 0; k < bbCount; ++k) {
                    newD[k] = 0;
                }
                newD[n] = 1;
            }
            
            // 检查是否变化
            for (int k = 0; k < bbCount; ++k) {
                if (newD[k] != domSet[n][k]) {
                    changed = 1;
                    break;
                }
            }
            
            // 更新domSet(n)
            for (int k = 0; k < bbCount; ++k) {
                domSet[n][k] = newD[k];
            }
        }
    }
    
    // 将domSet转换为标准支配矩阵D：D[i][j] = 1 表示 i DOM j
    for (int n = 0; n < bbCount; ++n) {
        for (int k = 0; k < bbCount; ++k) {
            if (domSet[n][k]) {
                // k 是 n 的必经节点（k DOM n），所以 D[k][n] = 1
                D[k][n] = 1;
            }
        }
    }
    
    // 步骤4: 识别回边
    // 回边：n→m 是回边，如果 m DOM n（m是n的必经节点）
    // 注意：D[i][j] = 1 表示基本块i是基本块j的必经节点（i DOM j）
    typedef struct {
        int from;  // 回边的起点（基本块ID）
        int to;    // 回边的终点（基本块ID，入口）
    } BackEdge;
    BackEdge backEdges[64];
    int backEdgeCount = 0;
    
    // 调试输出：打印必经节点关系
    printf("\n===== 必经节点关系 (D[i][j] = 1 表示 Bi DOM Bj) =====\n");
    for (int i = 0; i < bbCount; ++i) {
        printf("B%d 的必经节点: ", i);
        int first = 1;
        for (int j = 0; j < bbCount; ++j) {
            if (D[j][i]) {  // j DOM i（j 支配 i）
                if (!first) printf(", ");
                printf("B%d", j);
                first = 0;
            }
        }
        printf("\n");
    }
    printf("====================\n");
    
    // 验证：打印 D[0][1] 和 D[1][0] 用于调试
    if (bbCount > 1) {
        printf("调试信息: D[0][1] = %d (B0 DOM B1), D[1][0] = %d (B1 DOM B0)\n", 
               D[0][1], D[1][0]);
    }
    
    for (int n = 0; n < bbCount; ++n) {
        for (int j = 0; j < cfgCount[n]; ++j) {
            int m = cfg[n][j];
            // 检查 m DOM n：D[m][n] = 1 表示 m 是 n 的必经节点
            if (m >= 0 && D[m][n]) {  // m DOM n，则n→m是回边
                if (backEdgeCount < 64) {
                    backEdges[backEdgeCount].from = n;
                    backEdges[backEdgeCount].to = m;
                    backEdgeCount++;
                }
            }
        }
    }
    
    // 步骤5: 对每条回边，找到循环
    Loop loops[32];
    int loopCount = 0;
    
    for (int be = 0; be < backEdgeCount; ++be) {
        int m = backEdges[be].to;   // 循环入口
        int n = backEdges[be].from; // 回边起点
        
        // 循环查找算法：从回边n→m开始，找到所有能到达n且不经过m的结点
        // loop = {m, n}, S = {n}
        int loop[256];  // 基本块ID集合
        int loopSize = 0;
        loop[loopSize++] = m;  // 入口
        loop[loopSize++] = n;  // 回边起点
        
        int S[256];  // 待处理的基本块集合
        int SSize = 0;
        S[SSize++] = n;
        
        // 迭代查找：S ← (∪ P(q) for q in S) - loop, loop ← loop ∪ S
        int changed2 = 1;
        while (changed2) {
            changed2 = 0;
            int newS[256];
            int newSSize = 0;
            
            // S ← (∪ P(q) for q in S) - loop
            for (int i = 0; i < SSize; ++i) {
                int q = S[i];
                // 找到q的所有前驱
                for (int p = 0; p < bbCount; ++p) {
                    for (int j = 0; j < cfgCount[p]; ++j) {
                        if (cfg[p][j] == q) {
                            // p是q的前驱
                            // 检查p是否已在loop中（排除loop中的结点）
                            int inLoop = 0;
                            for (int k = 0; k < loopSize; ++k) {
                                if (loop[k] == p) {
                                    inLoop = 1;
                                    break;
                                }
                            }
                            if (!inLoop) {
                                // 检查p是否已在newS中（去重）
                                int inNewS = 0;
                                for (int k = 0; k < newSSize; ++k) {
                                    if (newS[k] == p) {
                                        inNewS = 1;
                                        break;
                                    }
                                }
                                if (!inNewS && newSSize < 256) {
                                    newS[newSSize++] = p;
                                }
                            }
                        }
                    }
                }
            }
            
            // 更新S和loop：loop ← loop ∪ S
            if (newSSize > 0) {
                changed2 = 1;
                // 将newS中的结点加入loop和S
                for (int i = 0; i < newSSize; ++i) {
                    // 检查是否已在loop中（虽然理论上不应该，但安全起见）
                    int alreadyInLoop = 0;
                    for (int k = 0; k < loopSize; ++k) {
                        if (loop[k] == newS[i]) {
                            alreadyInLoop = 1;
                            break;
                        }
                    }
                    if (!alreadyInLoop && loopSize < 256) {
                        loop[loopSize++] = newS[i];
                    }
                    // 更新S（清空旧S，用newS替换）
                }
                // 更新S为newS
                SSize = newSSize;
                for (int i = 0; i < newSSize; ++i) {
                    S[i] = newS[i];
                }
            } else {
                // newS为空，结束迭代
                changed2 = 0;
            }
        }
        
        // 将循环中的基本块转换为TAC索引范围
        if (loopCount < 32) {
            loops[loopCount].nodeCount = 0;
            loops[loopCount].bbCount = 0;
            int minTAC = in->sz, maxTAC = -1;
            
            // 保存基本块ID
            for (int i = 0; i < loopSize; ++i) {
                int bb_id = loop[i];
                if (loops[loopCount].bbCount < 64) {
                    loops[loopCount].bbs[loops[loopCount].bbCount++] = bb_id;
                }
            }
            
            // 找到循环中所有基本块的TAC范围
            for (int i = 0; i < loopSize; ++i) {
                int bb_id = loop[i];
                if (bbs[bb_id].start < minTAC) minTAC = bbs[bb_id].start;
                if (bbs[bb_id].end > maxTAC) maxTAC = bbs[bb_id].end;
                
                // 记录所有TAC索引
                for (int j = bbs[bb_id].start; j <= bbs[bb_id].end; ++j) {
                    if (loops[loopCount].nodeCount < 256) {
                        loops[loopCount].nodes[loops[loopCount].nodeCount++] = j;
                    }
                }
            }
            
            loops[loopCount].loopStart = minTAC;
            loops[loopCount].loopEnd = maxTAC;
            
            // 找到入口标签
            const TAC* entryTAC = &in->data[bbs[m].start];
            if (entryTAC->isLabel && entryTAC->label[0]) {
                strcpy(loops[loopCount].startLabel, entryTAC->label);
            } else {
                snprintf(loops[loopCount].startLabel, sizeof(loops[loopCount].startLabel), "BB%d", m);
            }
            
            // 保存对应的回边索引
            loops[loopCount].backEdgeIdx = be;
            
            loopCount++;
        }
    }
    
    // ========== 输出基本块、程序流图和循环信息 ==========
    printf("\n===== 基本块信息 =====\n");
    for (int i = 0; i < bbCount; ++i) {
        printf("基本块 B%d (TAC索引: %d-%d):\n", i, bbs[i].start, bbs[i].end);
        for (int j = bbs[i].start; j <= bbs[i].end; ++j) {
            const TAC* t = &in->data[j];
            if (t->isLabel) {
                printf("  [%d] %s:\n", j, t->label);
            } else if (strcmp(t->op, "=") == 0) {
                printf("  [%d] %s = %s\n", j, t->res, t->arg1);
            } else if (strcmp(t->op, "if") == 0) {
                printf("  [%d] if %s goto %s\n", j, t->arg1, t->res);
            } else if (strcmp(t->op, "goto") == 0) {
                printf("  [%d] goto %s\n", j, t->res);
            } else if (strcmp(t->op, "scan") == 0 || strcmp(t->op, "print") == 0) {
                printf("  [%d] %s %s\n", j, t->op, t->res);
            } else if (strcmp(t->op, "//") == 0) {
                printf("  [%d] // %s %s\n", j, t->res, t->arg1);
            } else {
                printf("  [%d] %s = %s %s %s\n", j, t->res, t->arg1, t->op, t->arg2);
            }
        }
        printf("\n");
    }
    printf("====================\n");
    
    printf("\n===== 程序流图 (控制流图) =====\n");
    for (int i = 0; i < bbCount; ++i) {
        printf("B%d", i);
        if (cfgCount[i] > 0) {
            printf(" -> ");
            for (int j = 0; j < cfgCount[i]; ++j) {
                if (j > 0) printf(", ");
                printf("B%d", cfg[i][j]);
            }
        } else {
            printf(" -> (结束)");
        }
        printf("\n");
    }
    printf("====================\n");
    
    printf("\n===== 回边信息 =====\n");
    for (int i = 0; i < backEdgeCount; ++i) {
        printf("回边 %d: B%d -> B%d\n", i + 1, backEdges[i].from, backEdges[i].to);
    }
    if (backEdgeCount == 0) {
        printf("(无回边)\n");
    }
    printf("====================\n");
    
    printf("\n===== 循环信息 =====\n");
    if (loopCount == 0) {
        printf("(未发现循环)\n");
    } else {
        for (int i = 0; i < loopCount; ++i) {
            printf("循环 %d:\n", i + 1);
            printf("  入口标签: %s\n", loops[i].startLabel);
            printf("  TAC范围: [%d-%d]\n", loops[i].loopStart, loops[i].loopEnd);
            printf("  包含的基本块: ");
            for (int j = 0; j < loops[i].bbCount; ++j) {
                if (j > 0) printf(", ");
                printf("B%d", loops[i].bbs[j]);
            }
            printf("\n");
            printf("  包含的TAC指令数: %d\n", loops[i].nodeCount);
            printf("  回边: B%d -> B%d\n", 
                   backEdges[loops[i].backEdgeIdx].from, 
                   backEdges[loops[i].backEdgeIdx].to);
        }
    }
    printf("====================\n\n");
    
    // 对每个循环进行标准的LICM分析
    for (int loopIdx = 0; loopIdx < loopCount; ++loopIdx) {
        Loop* loop = &loops[loopIdx];
        
        // ⑴ 求出循环的所有不变运算
        // 首先，找出循环中所有被定值的变量
        char loopDefs[256][64];
        int defCount = 0;
        
        // 使用循环中的所有TAC索引
        for (int idx = 0; idx < loop->nodeCount; ++idx) {
            int i = loop->nodes[idx];
            const TAC* t = &in->data[i];
            if (t->res[0] && !t->isLabel && 
                strcmp(t->op, "if") != 0 && strcmp(t->op, "goto") != 0 &&
                strcmp(t->op, "scan") != 0 && strcmp(t->op, "print") != 0) {
                int exists = 0;
                for (int k = 0; k < defCount; ++k) {
                    if (strcmp(loopDefs[k], t->res) == 0) {
                        exists = 1;
                        break;
                    }
                }
                if (!exists && defCount < 256) {
                    strcpy(loopDefs[defCount++], t->res);
                }
            }
        }
        
        // 识别不变运算：操作数都不在循环中定值（或者是常量/外部变量）
        typedef struct {
            int idx;
            TAC instr;
            int canHoist; // 是否满足外提条件
        } InvariantInstr;
        InvariantInstr invariants[64];
        int invCount = 0;
        
        // 使用循环中的所有TAC索引
        for (int idx = 0; idx < loop->nodeCount; ++idx) {
            int i = loop->nodes[idx];
            const TAC* t = &in->data[i];
            
            // 只考虑纯计算指令：A := B op C, A := op B, A := B
            if (t->isLabel || strcmp(t->op, "if") == 0 || strcmp(t->op, "goto") == 0 ||
                strcmp(t->op, "scan") == 0 || strcmp(t->op, "print") == 0 ||
                strcmp(t->op, "++") == 0 || strcmp(t->op, "--") == 0 ||
                strcmp(t->op, "+=") == 0 || strcmp(t->op, "-=") == 0 ||
                strcmp(t->op, "*=") == 0 || strcmp(t->op, "/=") == 0 ||
                strcmp(t->op, "//") == 0) {
                continue;
            }
            
            // 检查操作数是否在循环中定值
            int arg1InLoop = 0, arg2InLoop = 0;
            if (t->arg1[0]) {
                for (int k = 0; k < defCount; ++k) {
                    if (strcmp(loopDefs[k], t->arg1) == 0) {
                        arg1InLoop = 1;
                        break;
                    }
                }
            }
            if (t->arg2[0]) {
                for (int k = 0; k < defCount; ++k) {
                    if (strcmp(loopDefs[k], t->arg2) == 0) {
                        arg2InLoop = 1;
                        break;
                    }
                }
            }
            
            // 不变运算：操作数都不在循环中定值
            // 排除数组访问（别名问题）
            if (!arg1InLoop && !arg2InLoop &&
                strstr(t->arg1, "[") == NULL && strstr(t->arg2, "[") == NULL &&
                strstr(t->res, "[") == NULL && invCount < 64) {
                invariants[invCount].idx = i;
                invariants[invCount].instr = *t;
                invariants[invCount].canHoist = 0; // 待检查
                invCount++;
            }
        }
        
        // ⑵ 对每个不变运算，检查是否满足外提条件
        // 首先找到循环的所有出口结点
        int exitBBs[64];
        int exitCount = 0;
        findLoopExits_licm(loop, cfg, cfgCount, bbCount, exitBBs, &exitCount);
        
        for (int k = 0; k < invCount; ++k) {
            InvariantInstr* inv = &invariants[k];
            const char* A = inv->instr.res;
            int instrPos = inv->idx;
            int defPos = instrPos;
            
            // 找到指令所在的基本块
            int instrBB = findBasicBlockForTAC_licm(instrPos, bbs, bbCount);
            if (instrBB < 0) {
                // 无法找到基本块，保守跳过
                continue;
            }
            
            // 条件(a)① s所在结点是所有出口结点的必经结点
            int cond_a1 = isDominatingAllExits_licm(instrBB, exitBBs, exitCount, D);
            
            // 条件(a)② A在L中其它地方不再定值
            int cond_a2 = 1;  // 初始假设唯一定值
            for (int idx = 0; idx < loop->nodeCount; ++idx) {
                int i = loop->nodes[idx];
                if (i == instrPos) continue;
                const TAC* t = &in->data[i];
                if (t->res[0] && strcmp(t->res, A) == 0 && !t->isLabel &&
                    strcmp(t->op, "if") != 0 && strcmp(t->op, "goto") != 0) {
                    cond_a2 = 0;  // 有其他定值点
                    break;
                }
            }
            
            // 条件(a)③ L中所有A的引用点只有s中的A的定值才能达到
            int cond_a3 = 0;
            if (cond_a2) {
                // 如果唯一定值，检查所有使用点是否只能由此到达
                cond_a3 = allUsesReachByDef_licm(in, loop, A, defPos);
            }
            
            // 条件(b) A在离开L之后不再活跃，且条件①和②成立
            int liveAfter = isLiveAfterLoop_licm(in, loop, A);
            int cond_b = (!liveAfter && cond_a1 && cond_a2);
            
            // 满足条件(a)或(b)
            // 条件(a)需要满足：① 或 ② 或 ③
            int cond_a = (cond_a1 || cond_a2 || cond_a3);
            
            if (cond_a || cond_b) {
                inv->canHoist = 1;
                
                // 记录判断依据
                char reason[512];
                if (cond_a1) {
                    snprintf(reason, sizeof(reason), 
                             "  外提 %s = %s%s%s (条件a①: 必经所有出口结点)", 
                             A, inv->instr.arg1, 
                             inv->instr.arg2[0] ? " " : "",
                             inv->instr.arg2[0] ? inv->instr.op : "");
                } else if (cond_a2) {
                    snprintf(reason, sizeof(reason),
                             "  外提 %s = %s%s%s (条件a②: 循环内唯一定值)",
                             A, inv->instr.arg1,
                             inv->instr.arg2[0] ? " " : "",
                             inv->instr.arg2[0] ? inv->instr.op : "");
                } else if (cond_a3) {
                    snprintf(reason, sizeof(reason),
                             "  外提 %s = %s%s%s (条件a③: 所有引用仅由此定值到达)",
                             A, inv->instr.arg1,
                             inv->instr.arg2[0] ? " " : "",
                             inv->instr.arg2[0] ? inv->instr.op : "");
                } else if (cond_b) {
                    snprintf(reason, sizeof(reason),
                             "  外提 %s = %s%s%s (条件b: 循环后不活跃且满足①和②)",
                             A, inv->instr.arg1,
                             inv->instr.arg2[0] ? " " : "",
                             inv->instr.arg2[0] ? inv->instr.op : "");
                }
                logOpt(reason);
            }
        }
        
        // 统计可外提的指令数
        int hoistableCount = 0;
        for (int k = 0; k < invCount; ++k) {
            if (invariants[k].canHoist) hoistableCount++;
        }
        
        if (hoistableCount > 0) {
            char logbuf[256];
            snprintf(logbuf, sizeof(logbuf),
                     "循环不变代码外提: 从循环 %s 中识别 %d 条不变运算，满足外提条件 %d 条",
                     loop->startLabel, invCount, hoistableCount);
            logOpt(logbuf);
            *hoistCount += hoistableCount;
        }
        
        // ⑶ 将满足条件的不变运算外提到前置结点
        TACList temp = {0};
        for (int i = 0; i < in->sz; ++i) {
            if (i == loop->loopStart && hoistableCount > 0) {
                // 输出循环开始标签
                emitTo(&temp, in->data[i].op, in->data[i].res, in->data[i].arg1,
                       in->data[i].arg2, in->data[i].label, in->data[i].isLabel);
                // 外提满足条件的指令
                for (int k = 0; k < invCount; ++k) {
                    if (invariants[k].canHoist) {
                        TAC* ins = &invariants[k].instr;
                        emitTo(&temp, ins->op, ins->res, ins->arg1, ins->arg2, ins->label, ins->isLabel);
                    }
                }
                continue;
            }
            
            // 检查当前指令是否已被外提
            int isHoisted = 0;
            for (int k = 0; k < invCount; ++k) {
                if (invariants[k].canHoist && invariants[k].idx == i) {
                    isHoisted = 1;
                    break;
                }
            }
            
            if (!isHoisted) {
                const TAC* t = &in->data[i];
                emitTo(&temp, t->op, t->res, t->arg1, t->arg2, t->label, t->isLabel);
            }
        }
        
        // 更新输入
        free((void*)in->data);
        *(TACList*)in = temp;
    }
    
    // 最终输出
    for (int i = 0; i < in->sz; ++i) {
        const TAC* t = &in->data[i];
        emitTo(out, t->op, t->res, t->arg1, t->arg2, t->label, t->isLabel);
    }
}

static void optimizeTAC(const TACList* in, TACList* out) {
    out->sz = 0;
    clearOptLog(); // 清空优化日志
    
    // 先进行循环不变代码外提
    TACList afterHoist = {0};
    int hoistCount = 0;
    loopInvariantCodeMotion(in, &afterHoist, &hoistCount);
    
    ExprEntry exprs[256];
    int exprCount = 0;

    int constFoldCount = 0;
    int cseCount = 0;
    int peepholeCount = 0;

    for (int i = 0; i < afterHoist.sz; ++i) {
        const TAC* t = &afterHoist.data[i];
        TAC cur = *t;

        if (cur.isLabel) { // 遇到标签，清空可用表达式（跨基本块不做 CSE）
            exprCount = 0;
            emitTo(out, cur.op, cur.res, cur.arg1, cur.arg2, cur.label, cur.isLabel);
            continue;
        }

        // 处理赋值/运算的目标定义，先杀掉相关表达式
        if (cur.res[0]) killExprs(exprs, &exprCount, cur.res);

        // 常量折叠
        if (isPureBinary(cur.op) && isNumber(cur.arg1) && isNumber(cur.arg2)) {
            char folded[64];
            if (foldBinary(cur.op, cur.arg1, cur.arg2, folded, sizeof(folded))) {
                char logbuf[256];
                snprintf(logbuf, sizeof(logbuf), "常量折叠: %s = %s %s %s => %s = %s", 
                         cur.res, cur.arg1, cur.op, cur.arg2, cur.res, folded);
                logOpt(logbuf);
                constFoldCount++;
                strcpy(cur.op, "=");
                strcpy(cur.arg1, folded);
                cur.arg2[0] = '\0';
            }
        }

        // CSE：纯二元且非赋值，尝试复用
        if (isPureBinary(cur.op)) {
            char key[200];
            snprintf(key, sizeof(key), "%s|%s|%s", cur.op, cur.arg1, cur.arg2);
            int hit = -1;
            for (int k = 0; k < exprCount; ++k) {
                if (strcmp(exprs[k].key, key) == 0) { hit = k; break; }
            }
            if (hit >= 0) {
                // 复用已有结果：res = old_res
                char logbuf[256];
                snprintf(logbuf, sizeof(logbuf), "公共子表达式消除(CSE): %s = %s %s %s 复用为 %s = %s",
                         cur.res, cur.arg1, cur.op, cur.arg2, cur.res, exprs[hit].res);
                logOpt(logbuf);
                cseCount++;
                strcpy(cur.op, "=");
                strcpy(cur.arg1, exprs[hit].res);
                cur.arg2[0] = '\0';
            } else if (exprCount < (int)(sizeof(exprs)/sizeof(exprs[0]))) {
                strcpy(exprs[exprCount].key, key);
                strcpy(exprs[exprCount].res, cur.res);
                exprCount++;
            }
        }

        // 简单窥孔：goto L; L: => 移除 goto
        if (strcmp(cur.op, "goto") == 0 && i + 1 < afterHoist.sz) {
            const TAC* nxt = &afterHoist.data[i + 1];
            if (nxt->isLabel && strcmp(nxt->label, cur.res) == 0) {
                char logbuf[256];
                snprintf(logbuf, sizeof(logbuf), "窥孔优化: 移除冗余跳转 goto %s (紧跟标签)", cur.res);
                logOpt(logbuf);
                peepholeCount++;
                continue; // 跳过冗余 goto
            }
        }

        emitTo(out, cur.op, cur.res, cur.arg1, cur.arg2, cur.label, cur.isLabel);
    }

    // 添加优化统计摘要
    char summary[256];
    snprintf(summary, sizeof(summary), "\n优化统计: 循环不变代码外提 %d 处, 常量折叠 %d 处, CSE %d 处",
             hoistCount, constFoldCount, cseCount);
    logOpt(summary);
    
    // 清理临时列表
    free(afterHoist.data);
}

// 打印 TAC
static void printTAC(const TACList* l, FILE* out) {
    for (int i = 0; i < l->sz; ++i) {
        const TAC* t = &l->data[i];
        if (t->isLabel) {
            fprintf(out, "%s:\n", t->label);
            continue;
        }
        if (strcmp(t->op, "label") == 0) {
            fprintf(out, "%s:\n", t->label);
        } else if (strcmp(t->op, "=") == 0) {
            fprintf(out, "%s = %s\n", t->res, t->arg1);
        } else if (strcmp(t->op, "+=") == 0 || strcmp(t->op, "-=") == 0 ||
                   strcmp(t->op, "*=") == 0 || strcmp(t->op, "/=") == 0) {
            fprintf(out, "%s %s %s\n", t->res, t->op, t->arg1);
        } else if (strcmp(t->op, "++") == 0 || strcmp(t->op, "--") == 0) {
            fprintf(out, "%s %s\n", t->res, t->op);
        } else if (strcmp(t->op, "if") == 0) {
            fprintf(out, "if %s goto %s\n", t->arg1, t->res);
        } else if (strcmp(t->op, "goto") == 0) {
            fprintf(out, "goto %s\n", t->res);
        } else if (strcmp(t->op, "scan") == 0 || strcmp(t->op, "print") == 0) {
            fprintf(out, "%s %s\n", t->op, t->res);
        } else if (strcmp(t->op, "//") == 0) {
            fprintf(out, "// %s %s\n", t->res, t->arg1);
        } else if (strcmp(t->op, "uminus") == 0) {
            fprintf(out, "%s = - %s\n", t->res, t->arg1);
        } else {
            fprintf(out, "%s = %s %s %s\n", t->res, t->arg1, t->op, t->arg2);
        }
    }
}

// 写优化后的 TAC 到文件
static void writeOptimizedToFile(const char* path, const TACList* l) {
    FILE* fp = fopen(path, "w");
    if (!fp) {
        fprintf(stderr, "[TAC] 无法写入优化文件: %s\n", path);
        return;
    }
    
    // 写入优化详情
    fprintf(fp, "===== 优化详情 =====\n");
    for (int i = 0; i < g_optLog.count; ++i) {
        fprintf(fp, "%s\n", g_optLog.logs[i]);
    }
    fprintf(fp, "====================\n\n");
    
    // 写入优化后的TAC
    fprintf(fp, "===== Optimized TAC =====\n");
    printTAC(l, fp);
    fprintf(fp, "===== TAC End =====\n");
    fclose(fp);
}

void generateTAC(ASTNode* root) {
    // 重置全局状态
    temp_id = 0;
    label_id = 0;
    g_tac.sz = 0;
    g_loop_stack_top = -1;  // 重置标签栈

    // 生成原始 TAC
    genStmtList(root->child); // root 应是 NODE_PROGRAM，child 是语句列表

    // 打印原始 TAC
    printf("===== TAC (三地址码，原始) =====\n");
    printTAC(&g_tac, stdout);
    printf("===== TAC End =====\n");

    // 生成优化 TAC
    TACList opt = {0};
    optimizeTAC(&g_tac, &opt);

    // 打印优化详情
    printf("\n===== 优化详情 =====\n");
    for (int i = 0; i < g_optLog.count; ++i) {
        printf("%s\n", g_optLog.logs[i]);
    }
    printf("====================\n");

    // 打印优化 TAC
    printf("\n===== TAC (三地址码，优化后) =====\n");
    printTAC(&opt, stdout);
    printf("===== TAC End =====\n");

    // 输出优化 TAC 到文件
    writeOptimizedToFile("optimized.tac", &opt);
    
    // 保存优化后的TAC供MIPS生成使用
    g_optimized_tac = opt;  // 浅拷贝（注意：后续不要释放opt）
    
    // 清理优化日志
    clearOptLog();
}

// 导出优化后的TAC（供mips_gen.c使用）
TACList* getOptimizedTAC() {
    return &g_optimized_tac;
}



