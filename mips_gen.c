#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "stdarg.h"   // 可变参数宏（va_start, va_end等）
#include "def.h"

/*******************************************************************************
 * MIPS 目标代码生成器
 * 
 * 功能：将优化后的TAC（三地址码）转换为MIPS汇编代码
 * 目标：可在MARS/QTSPIM模拟器上运行
 * 
 * 寄存器分配策略：
 * - $t0-$t9: 临时寄存器（用于表达式计算和临时变量）
 * - $s0-$s7: 保存寄存器（用于频繁使用的变量）
 * - $sp:      栈指针（用于溢出变量和数组）
 * - $ra:      返回地址（预留，当前未使用函数）
 * - $v0:      系统调用号和返回值
 * - $a0:      系统调用参数
 ******************************************************************************/

// TAC结构（与codegen.c保持一致）
typedef struct {
    char op[8];
    char arg1[64];
    char arg2[64];
    char res[64];
    char label[32];
    int isLabel;
} TAC;

typedef struct {
    TAC* data;
    int sz;
    int cap;
} TACList;

/*******************************************************************************
 * 寄存器分配数据结构
 ******************************************************************************/

// 寄存器类型
typedef enum {
    REG_T0 = 0, REG_T1, REG_T2, REG_T3, REG_T4, REG_T5, REG_T6, REG_T7, REG_T8, REG_T9,  // $t0-$t9
    REG_S0, REG_S1, REG_S2, REG_S3, REG_S4, REG_S5, REG_S6, REG_S7,                      // $s0-$s7
    REG_COUNT
} RegType;

// 寄存器名称表
static const char* REG_NAMES[] = {
    "$t0", "$t1", "$t2", "$t3", "$t4", "$t5", "$t6", "$t7", "$t8", "$t9",
    "$s0", "$s1", "$s2", "$s3", "$s4", "$s5", "$s6", "$s7"
};

// 寄存器描述符（记录寄存器中存储的变量）
typedef struct {
    char var[64];       // 当前存储的变量名（空串表示未使用）
    int dirty;          // 脏位：1表示需要写回内存
    int last_use;       // 最后使用时间（用于LRU替换）
} RegDescriptor;

// 变量描述符（记录变量的位置）
typedef struct {
    char name[64];      // 变量名
    int in_reg;         // 是否在寄存器中：1表示在，0表示在内存
    RegType reg;        // 所在寄存器（如果in_reg=1）
    int offset;         // 在栈中的偏移（如果in_reg=0或需要溢出）
    int is_temp;        // 是否为临时变量
    int is_array;       // 是否为数组
} VarDescriptor;

// 全局寄存器和变量描述符表
static RegDescriptor reg_desc[REG_COUNT];
static VarDescriptor* var_desc = NULL;
static int var_desc_count = 0;
static int var_desc_cap = 0;

// 栈帧管理
static int stack_offset = 0;       // 当前栈偏移
static int max_stack_size = 0;     // 最大栈大小

// 输出文件
static FILE* mips_output = NULL;

// 指令计数（用于last_use）
static int instr_count = 0;

/*******************************************************************************
 * 辅助函数声明
 ******************************************************************************/
static void initRegAlloc();
static void addVariable(const char* name, int is_temp, int is_array);
static VarDescriptor* findVar(const char* name);
static RegType allocReg(const char* var);
static void spillReg(RegType reg);
static const char* getReg(const char* var);
static void freeReg(const char* var);
static void writeBack(const char* var);
static int isNumber(const char* str);
static int isTemp(const char* name);
static void emitMIPS(const char* fmt, ...);

/*******************************************************************************
 * 初始化寄存器分配器
 ******************************************************************************/
static void initRegAlloc() {
    // 清空寄存器描述符
    for (int i = 0; i < REG_COUNT; i++) {
        reg_desc[i].var[0] = '\0';
        reg_desc[i].dirty = 0;
        reg_desc[i].last_use = -1;
    }
    
    // 清空变量描述符
    var_desc_count = 0;
    stack_offset = 0;
    max_stack_size = 0;
    instr_count = 0;
}

/*******************************************************************************
 * 添加变量到描述符表
 ******************************************************************************/
static void addVariable(const char* name, int is_temp, int is_array) {
    // 检查是否已存在
    if (findVar(name)) return;
    
    // 扩展容量
    if (var_desc_count >= var_desc_cap) {
        int new_cap = var_desc_cap ? var_desc_cap * 2 : 64;
        var_desc = (VarDescriptor*)realloc(var_desc, new_cap * sizeof(VarDescriptor));
        var_desc_cap = new_cap;
    }
    
    // 添加变量
    VarDescriptor* vd = &var_desc[var_desc_count++];
    strncpy(vd->name, name, sizeof(vd->name) - 1);
    vd->in_reg = 0;
    vd->reg = REG_T0;
    vd->offset = stack_offset;
    vd->is_temp = is_temp;
    vd->is_array = is_array;
    
    // 为非临时变量分配栈空间
    if (!is_temp && !is_array) {
        stack_offset += 4;
        if (stack_offset > max_stack_size) {
            max_stack_size = stack_offset;
        }
    }
}

/*******************************************************************************
 * 查找变量描述符
 ******************************************************************************/
static VarDescriptor* findVar(const char* name) {
    for (int i = 0; i < var_desc_count; i++) {
        if (strcmp(var_desc[i].name, name) == 0) {
            return &var_desc[i];
        }
    }
    return NULL;
}

/*******************************************************************************
 * 判断是否为数字常量
 ******************************************************************************/
static int isNumber(const char* str) {
    if (!str || !str[0]) return 0;
    int i = 0;
    if (str[i] == '-' || str[i] == '+') i++;
    int has_digit = 0;
    int has_dot = 0;
    while (str[i]) {
        if (str[i] >= '0' && str[i] <= '9') {
            has_digit = 1;
        } else if (str[i] == '.') {
            if (has_dot) return 0;
            has_dot = 1;
        } else {
            return 0;
        }
        i++;
    }
    return has_digit;
}

/*******************************************************************************
 * 判断是否为临时变量（以t开头的变量）
 ******************************************************************************/
static int isTemp(const char* name) {
    return name && name[0] == 't' && name[1] >= '0' && name[1] <= '9';
}

/*******************************************************************************
 * 寄存器分配算法（改进的LRU策略）
 * 返回：分配给变量的寄存器编号
 ******************************************************************************/
static RegType allocReg(const char* var) {
    VarDescriptor* vd = findVar(var);
    if (!vd) {
        addVariable(var, isTemp(var), 0);
        vd = findVar(var);
    }
    
    // 如果变量已在寄存器中，直接返回
    if (vd->in_reg) {
        reg_desc[vd->reg].last_use = instr_count;
        return vd->reg;
    }
    
    // 选择临时寄存器范围（$t0-$t9优先用于临时变量）
    int start_reg = vd->is_temp ? REG_T0 : REG_S0;
    int end_reg = vd->is_temp ? REG_T9 : REG_S7;
    
    // 首先尝试找一个空闲寄存器
    for (int i = start_reg; i <= end_reg; i++) {
        if (reg_desc[i].var[0] == '\0') {
            // 找到空闲寄存器
            strcpy(reg_desc[i].var, var);
            reg_desc[i].dirty = 0;
            reg_desc[i].last_use = instr_count;
            vd->in_reg = 1;
            vd->reg = i;
            return i;
        }
    }
    
    // 如果临时变量找不到$t寄存器，扩展到$s寄存器
    if (vd->is_temp && end_reg < REG_S7) {
        for (int i = REG_S0; i <= REG_S7; i++) {
            if (reg_desc[i].var[0] == '\0') {
                strcpy(reg_desc[i].var, var);
                reg_desc[i].dirty = 0;
                reg_desc[i].last_use = instr_count;
                vd->in_reg = 1;
                vd->reg = i;
                return i;
            }
        }
    }
    
    // 没有空闲寄存器，使用LRU策略替换
    int lru_reg = start_reg;
    int min_use = reg_desc[start_reg].last_use;
    
    for (int i = start_reg; i <= end_reg; i++) {
        if (reg_desc[i].last_use < min_use) {
            min_use = reg_desc[i].last_use;
            lru_reg = i;
        }
    }
    
    // 如果临时变量仍未找到，扩展LRU搜索范围
    if (vd->is_temp && end_reg < REG_S7) {
        for (int i = REG_S0; i <= REG_S7; i++) {
            if (reg_desc[i].last_use < min_use) {
                min_use = reg_desc[i].last_use;
                lru_reg = i;
            }
        }
    }
    
    // 溢出LRU寄存器
    spillReg(lru_reg);
    
    // 分配给新变量
    strcpy(reg_desc[lru_reg].var, var);
    reg_desc[lru_reg].dirty = 0;
    reg_desc[lru_reg].last_use = instr_count;
    vd->in_reg = 1;
    vd->reg = lru_reg;
    
    return lru_reg;
}

/*******************************************************************************
 * 溢出寄存器（将寄存器内容写回内存）
 ******************************************************************************/
static void spillReg(RegType reg) {
    if (reg_desc[reg].var[0] == '\0') return;  // 寄存器为空
    
    const char* var = reg_desc[reg].var;
    VarDescriptor* vd = findVar(var);
    
    if (!vd) return;
    
    // 如果是脏的（被修改过），需要写回内存
    if (reg_desc[reg].dirty) {
        emitMIPS("    sw %s, %d($sp)   # spill %s", 
                 REG_NAMES[reg], vd->offset, var);
    }
    
    // 更新描述符
    vd->in_reg = 0;
    reg_desc[reg].var[0] = '\0';
    reg_desc[reg].dirty = 0;
}

/*******************************************************************************
 * 获取变量所在的寄存器名
 * 如果变量不在寄存器中，分配一个并加载
 ******************************************************************************/
static const char* getReg(const char* var) {
    // 如果是数字常量，加载到$t9
    if (isNumber(var)) {
        emitMIPS("    li $t9, %s       # load constant", var);
        return "$t9";
    }
    
    VarDescriptor* vd = findVar(var);
    if (!vd) {
        addVariable(var, isTemp(var), 0);
        vd = findVar(var);
    }
    
    // 如果变量在寄存器中，直接返回
    if (vd->in_reg) {
        reg_desc[vd->reg].last_use = instr_count;
        return REG_NAMES[vd->reg];
    }
    
    // 分配寄存器并加载变量
    RegType reg = allocReg(var);
    emitMIPS("    lw %s, %d($sp)    # load %s", 
             REG_NAMES[reg], vd->offset, var);
    
    return REG_NAMES[reg];
}

/*******************************************************************************
 * 释放变量占用的寄存器
 ******************************************************************************/
static void freeReg(const char* var) {
    VarDescriptor* vd = findVar(var);
    if (!vd || !vd->in_reg) return;
    
    RegType reg = vd->reg;
    
    // 如果是脏的，写回内存
    if (reg_desc[reg].dirty) {
        emitMIPS("    sw %s, %d($sp)   # free %s", 
                 REG_NAMES[reg], vd->offset, var);
    }
    
    // 清空寄存器
    reg_desc[reg].var[0] = '\0';
    reg_desc[reg].dirty = 0;
    vd->in_reg = 0;
}

/*******************************************************************************
 * 将变量写回内存（不释放寄存器）
 ******************************************************************************/
static void writeBack(const char* var) {
    VarDescriptor* vd = findVar(var);
    if (!vd || !vd->in_reg) return;
    
    RegType reg = vd->reg;
    
    if (reg_desc[reg].dirty) {
        emitMIPS("    sw %s, %d($sp)   # write back %s", 
                 REG_NAMES[reg], vd->offset, var);
        reg_desc[reg].dirty = 0;
    }
}

/*******************************************************************************
 * 输出MIPS指令（格式化输出）
 ******************************************************************************/
static void emitMIPS(const char* fmt, ...) {
    va_list args;
    va_start(args, fmt);
    vfprintf(mips_output, fmt, args);
    va_end(args);
    fprintf(mips_output, "\n");
}

/*******************************************************************************
 * 扫描TAC，收集所有变量（预处理阶段）
 ******************************************************************************/
static void collectVariables(TACList* tac_list) {
    for (int i = 0; i < tac_list->sz; i++) {
        TAC* tac = &tac_list->data[i];
        
        if (tac->isLabel) continue;
        
        // 处理结果变量
        if (tac->res[0] && !isNumber(tac->res)) {
            addVariable(tac->res, isTemp(tac->res), 0);
        }
        
        // 处理操作数1
        if (tac->arg1[0] && !isNumber(tac->arg1)) {
            addVariable(tac->arg1, isTemp(tac->arg1), 0);
        }
        
        // 处理操作数2
        if (tac->arg2[0] && !isNumber(tac->arg2)) {
            addVariable(tac->arg2, isTemp(tac->arg2), 0);
        }
    }
}

/*******************************************************************************
 * 生成MIPS代码：算术运算
 ******************************************************************************/
static void genArithmeticMIPS(TAC* tac) {
    const char* op = tac->op;
    const char* res = tac->res;
    const char* arg1 = tac->arg1;
    const char* arg2 = tac->arg2;
    
    // 获取操作数寄存器
    const char* reg1 = getReg(arg1);
    const char* reg2 = getReg(arg2);
    
    // 分配结果寄存器
    RegType res_reg = allocReg(res);
    const char* res_reg_name = REG_NAMES[res_reg];
    
    // 生成对应的MIPS指令
    if (strcmp(op, "+") == 0) {
        emitMIPS("    add %s, %s, %s   # %s = %s + %s", 
                 res_reg_name, reg1, reg2, res, arg1, arg2);
    } else if (strcmp(op, "-") == 0) {
        emitMIPS("    sub %s, %s, %s   # %s = %s - %s", 
                 res_reg_name, reg1, reg2, res, arg1, arg2);
    } else if (strcmp(op, "*") == 0) {
        emitMIPS("    mul %s, %s, %s   # %s = %s * %s", 
                 res_reg_name, reg1, reg2, res, arg1, arg2);
    } else if (strcmp(op, "/") == 0) {
        emitMIPS("    div %s, %s, %s   # %s = %s / %s", 
                 res_reg_name, reg1, reg2, res, arg1, arg2);
    }
    
    // 标记结果寄存器为脏
    reg_desc[res_reg].dirty = 1;
}

/*******************************************************************************
 * 生成MIPS代码：关系运算
 ******************************************************************************/
static void genRelationalMIPS(TAC* tac) {
    const char* op = tac->op;
    const char* res = tac->res;
    const char* arg1 = tac->arg1;
    const char* arg2 = tac->arg2;
    
    const char* reg1 = getReg(arg1);
    const char* reg2 = getReg(arg2);
    
    RegType res_reg = allocReg(res);
    const char* res_reg_name = REG_NAMES[res_reg];
    
    if (strcmp(op, "==") == 0) {
        emitMIPS("    seq %s, %s, %s   # %s = (%s == %s)", 
                 res_reg_name, reg1, reg2, res, arg1, arg2);
    } else if (strcmp(op, "!=") == 0) {
        emitMIPS("    sne %s, %s, %s   # %s = (%s != %s)", 
                 res_reg_name, reg1, reg2, res, arg1, arg2);
    } else if (strcmp(op, "<") == 0) {
        emitMIPS("    slt %s, %s, %s   # %s = (%s < %s)", 
                 res_reg_name, reg1, reg2, res, arg1, arg2);
    } else if (strcmp(op, "<=") == 0) {
        emitMIPS("    sle %s, %s, %s   # %s = (%s <= %s)", 
                 res_reg_name, reg1, reg2, res, arg1, arg2);
    } else if (strcmp(op, ">") == 0) {
        emitMIPS("    sgt %s, %s, %s   # %s = (%s > %s)", 
                 res_reg_name, reg1, reg2, res, arg1, arg2);
    } else if (strcmp(op, ">=") == 0) {
        emitMIPS("    sge %s, %s, %s   # %s = (%s >= %s)", 
                 res_reg_name, reg1, reg2, res, arg1, arg2);
    }
    
    reg_desc[res_reg].dirty = 1;
}

/*******************************************************************************
 * 生成MIPS代码：赋值
 ******************************************************************************/
static void genAssignMIPS(TAC* tac) {
    const char* res = tac->res;
    const char* arg1 = tac->arg1;
    
    const char* src_reg = getReg(arg1);
    RegType dst_reg = allocReg(res);
    const char* dst_reg_name = REG_NAMES[dst_reg];
    
    if (strcmp(src_reg, dst_reg_name) != 0) {
        emitMIPS("    move %s, %s       # %s = %s", 
                 dst_reg_name, src_reg, res, arg1);
    }
    
    reg_desc[dst_reg].dirty = 1;
}

/*******************************************************************************
 * 生成MIPS代码：条件跳转
 ******************************************************************************/
static void genCondJumpMIPS(TAC* tac) {
    const char* op = tac->op;
    const char* arg1 = tac->arg1;
    const char* label = tac->res;
    
    const char* reg1 = getReg(arg1);
    
    if (strcmp(op, "ifz") == 0) {
        emitMIPS("    beqz %s, %s       # if %s == 0 goto %s", 
                 reg1, label, arg1, label);
    } else if (strcmp(op, "ifnz") == 0) {
        emitMIPS("    bnez %s, %s       # if %s != 0 goto %s", 
                 reg1, label, arg1, label);
    }
}

/*******************************************************************************
 * 生成MIPS代码：无条件跳转
 ******************************************************************************/
static void genGotoMIPS(TAC* tac) {
    emitMIPS("    j %s              # goto %s", tac->res, tac->res);
}

/*******************************************************************************
 * 生成MIPS代码：输入（scan）
 ******************************************************************************/
static void genScanMIPS(TAC* tac) {
    const char* var = tac->res;
    
    // 系统调用：读取整数
    emitMIPS("    li $v0, 5         # syscall: read_int");
    emitMIPS("    syscall");
    
    // 分配寄存器并保存结果
    RegType reg = allocReg(var);
    emitMIPS("    move %s, $v0      # %s = input", REG_NAMES[reg], var);
    reg_desc[reg].dirty = 1;
}

/*******************************************************************************
 * 生成MIPS代码：输出（print）
 ******************************************************************************/
static void genPrintMIPS(TAC* tac) {
    const char* var = tac->res;
    const char* reg = getReg(var);
    
    // 将要打印的值移到$a0
    emitMIPS("    move $a0, %s      # prepare to print %s", reg, var);
    
    // 系统调用：打印整数
    emitMIPS("    li $v0, 1         # syscall: print_int");
    emitMIPS("    syscall");
    
    // 打印换行
    emitMIPS("    li $v0, 11        # syscall: print_char");
    emitMIPS("    li $a0, 10        # newline");
    emitMIPS("    syscall");
}

/*******************************************************************************
 * 生成MIPS代码：主函数
 ******************************************************************************/
void generateMIPS(TACList* optimized_tac, const char* filename) {
    // 打开输出文件
    mips_output = fopen(filename, "w");
    if (!mips_output) {
        fprintf(stderr, "[MIPS生成] 无法创建文件: %s\n", filename);
        return;
    }
    
    printf("\n========================================\n");
    printf("生成MIPS汇编代码\n");
    printf("========================================\n");
    
    // 初始化寄存器分配器
    initRegAlloc();
    
    // 收集所有变量
    collectVariables(optimized_tac);
    
    // 输出MIPS文件头
    emitMIPS("# MIPS Assembly Code");
    emitMIPS("# Generated from optimized TAC");
    emitMIPS("# Target: MARS/QTSPIM simulator");
    emitMIPS("");
    emitMIPS(".data");
    emitMIPS("    newline: .asciiz \"\\n\"");
    emitMIPS("");
    emitMIPS(".text");
    emitMIPS(".globl main");
    emitMIPS("");
    emitMIPS("main:");
    emitMIPS("    # Prologue: allocate stack frame");
    emitMIPS("    addiu $sp, $sp, -%d   # allocate %d bytes", max_stack_size, max_stack_size);
    emitMIPS("");
    
    // 遍历TAC生成MIPS代码
    for (int i = 0; i < optimized_tac->sz; i++) {
        TAC* tac = &optimized_tac->data[i];
        instr_count = i;
        
        // 标签
        if (tac->isLabel) {
            emitMIPS("");
            emitMIPS("%s:", tac->label);
            continue;
        }
        
        const char* op = tac->op;
        
        // 算术运算
        if (strcmp(op, "+") == 0 || strcmp(op, "-") == 0 || 
            strcmp(op, "*") == 0 || strcmp(op, "/") == 0) {
            genArithmeticMIPS(tac);
        }
        // 关系运算
        else if (strcmp(op, "==") == 0 || strcmp(op, "!=") == 0 ||
                 strcmp(op, "<") == 0 || strcmp(op, "<=") == 0 ||
                 strcmp(op, ">") == 0 || strcmp(op, ">=") == 0) {
            genRelationalMIPS(tac);
        }
        // 赋值
        else if (strcmp(op, "=") == 0) {
            genAssignMIPS(tac);
        }
        // 条件跳转
        else if (strcmp(op, "ifz") == 0 || strcmp(op, "ifnz") == 0) {
            genCondJumpMIPS(tac);
        }
        // 无条件跳转
        else if (strcmp(op, "goto") == 0) {
            genGotoMIPS(tac);
        }
        // 输入
        else if (strcmp(op, "scan") == 0) {
            genScanMIPS(tac);
        }
        // 输出
        else if (strcmp(op, "print") == 0) {
            genPrintMIPS(tac);
        }
    }
    
    // 输出MIPS文件尾（程序退出）
    emitMIPS("");
    emitMIPS("    # Epilogue: restore stack and exit");
    emitMIPS("    addiu $sp, $sp, %d    # deallocate stack", max_stack_size);
    emitMIPS("    li $v0, 10            # syscall: exit");
    emitMIPS("    syscall");
    
    fclose(mips_output);
    
    printf("\n[MIPS生成] 代码已写入: %s\n", filename);
    printf("[寄存器分配] 使用了 %d 个变量\n", var_desc_count);
    printf("[栈管理] 最大栈大小: %d 字节\n", max_stack_size);
    printf("========================================\n");
}

/*******************************************************************************
 * 导出TAC列表（用于从codegen.c调用）
 ******************************************************************************/
extern TACList* getOptimizedTAC();  // 在codegen.c中实现

void generateMIPSFromTAC() {
    TACList* opt_tac = getOptimizedTAC();
    if (!opt_tac || opt_tac->sz == 0) {
        fprintf(stderr, "[MIPS生成] 警告: 没有TAC可生成\n");
        return;
    }
    
    generateMIPS(opt_tac, "output.asm");
}

