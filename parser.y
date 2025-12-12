%define parse.error verbose
%locations

%code requires {
#include "def.h"
}

%{
#include "stdio.h"
#include "math.h"
#include "string.h"
#include "def.h"
extern int yylineno;
extern char *yytext;
extern FILE *yyin;
void yyerror(const char* fmt, ...);
int yylex();
%}

/*********************请在下面完成parser.y*******************/

%union {
    ASTNode* type_node;
    int type_int;
    float type_float;
    char type_id[32];
}

%token LC RC LP RP SEMI LB RB COMMA
%token <type_int> INT CHAR
%token <type_float> FLOAT
%token <type_id> ID
%token IF ELSE WHILE FOR BREAK CONTINUE SCAN PRINT
%token TYPE_INT TYPE_FLOAT TYPE_CHAR
%token ASSIGNOP PLUS MINUS STAR DIV
%token PLUS_ASSIGN MINUS_ASSIGN STAR_ASSIGN DIV_ASSIGN
%token INCREMENT DECREMENT
%token EQ NE GT GE LT LE

%type <type_node> program StmList Stmt CompSt Exp Var VarDecl ArrayDecl ArrayDims

%expect 1

%right ASSIGNOP PLUS_ASSIGN MINUS_ASSIGN STAR_ASSIGN DIV_ASSIGN
%left EQ NE
%left LT LE GT GE
%left PLUS MINUS
%left STAR DIV
%right UMINUS INCREMENT DECREMENT

%%

program:
    StmList {
        $$ = createNode(NODE_PROGRAM);
        $$->child = $1;
        displayAST($$, 0);
        checkSemantics($$);
        if (getSemanticErrorCount() == 0) {
            generateTAC($$);        // 语义通过后生成三地址码
            generateMIPSFromTAC();  // 从TAC生成MIPS汇编代码
        } else {
            printf("[跳过代码生成] 语义错误数: %d\n", getSemanticErrorCount());
        }
    }
    ;

StmList:
    /* 空 */
    {
        $$ = NULL;
    }
    | Stmt StmList
    {
        $$ = createNode(NODE_STMLIST);
        $$->child = $1;
        $$->right = $2;
    }
    ;

CompSt:
    LC StmList RC
    {
        $$ = createNode(NODE_COMP_STMT);
        $$->child = $2;
    }
    ;

Stmt:
    Exp SEMI
    {
        $$ = createNode(NODE_EXP_STMT);
        $$->child = $1;
    }
    | CompSt
    {
        $$ = $1;
    }
    | SCAN ID SEMI
    {
        $$ = createNode(NODE_SCAN_STMT);
        strcpy($$->value.var_name, $2);
    }
    | PRINT Exp SEMI
    {
        $$ = createNode(NODE_PRINT_STMT);
        $$->child = $2;
    }
    | IF LP Exp RP Stmt
    {
        $$ = createNode(NODE_IF_STMT);
        $$->child = $3;
        $$->left = $5;
    }
    | IF LP Exp RP Stmt ELSE Stmt
    {
        $$ = createNode(NODE_IF_ELSE_STMT);
        $$->child = $3;
        $$->left = $5;
        $$->right = $7;
    }
    | WHILE LP Exp RP Stmt
    {
        $$ = createNode(NODE_WHILE_STMT);
        $$->child = $3;
        $$->left = $5;
    }
    | FOR LP Stmt Exp SEMI Stmt RP Stmt
    {
        $$ = createNode(NODE_FOR_STMT);
        $$->child = $3;  // 初始化语句
        $$->left = $4;   // 条件表达式
        $$->right = createNode(NODE_STMLIST);
        $$->right->child = $6;  // 更新语句
        $$->right->right = $8;  // 循环体
    }
    | BREAK SEMI
    {
        $$ = createNode(NODE_BREAK_STMT);
    }
    | CONTINUE SEMI
    {
        $$ = createNode(NODE_CONTINUE_STMT);
    }
    | VarDecl
    {
        $$ = $1;
    }
    | ArrayDecl
    {
        $$ = $1;
    }
    ;

Var:
    ID
    {
        $$ = createVarNode($1);
    }
    | Var LB Exp RB
    {
        $$ = createNode(NODE_ARRAY_ACCESS);
        $$->left = $1;
        $$->right = $3;
    }
    ;

VarDecl:
    TYPE_INT ID SEMI
    {
        $$ = createNode(NODE_VAR_DECL);
        $$->decl_type = VAL_INT;
        strcpy($$->value.var_name, $2);
    }
    | TYPE_FLOAT ID SEMI
    {
        $$ = createNode(NODE_VAR_DECL);
        $$->decl_type = VAL_FLOAT;
        strcpy($$->value.var_name, $2);
    }
    | TYPE_CHAR ID SEMI
    {
        $$ = createNode(NODE_VAR_DECL);
        $$->decl_type = VAL_CHAR;
        strcpy($$->value.var_name, $2);
    }
    ;

ArrayDecl:
    TYPE_INT ID ArrayDims SEMI
    {
        $$ = createNode(NODE_ARRAY_DECL);
        $$->decl_type = VAL_INT;
        strcpy($$->value.var_name, $2);
        $$->child = $3;  // 多维数组维度信息
    }
    | TYPE_FLOAT ID ArrayDims SEMI
    {
        $$ = createNode(NODE_ARRAY_DECL);
        $$->decl_type = VAL_FLOAT;
        strcpy($$->value.var_name, $2);
        $$->child = $3;  // 多维数组维度信息
    }
    | TYPE_CHAR ID ArrayDims SEMI
    {
        $$ = createNode(NODE_ARRAY_DECL);
        $$->decl_type = VAL_CHAR;
        strcpy($$->value.var_name, $2);
        $$->child = $3;  // 多维数组维度信息
    }
    ;

ArrayDims:
    LB INT RB
    {
        $$ = createIntNode($2);
    }
    | ArrayDims LB INT RB
    {
        $$ = createNode(NODE_STMLIST);  // 复用STMLIST节点结构来存储多个维度
        $$->child = $1;
        $$->right = createIntNode($3);
    }
    ;

Exp:
    INT
    {
        $$ = createIntNode($1);
    }
    | FLOAT
    {
        $$ = createFloatNode($1);
    }
    | CHAR
    {
        $$ = createNode(NODE_CHAR_CONST);
        $$->value.int_val = $1;
    }
    | Var
    {
        $$ = $1;
    }
    | Var ASSIGNOP Exp
    {
        $$ = createNode(NODE_ASSIGN);
        $$->left = $1;
        $$->right = $3;
    }
    | Var PLUS_ASSIGN Exp
    {
        $$ = createNode(NODE_PLUS_ASSIGN);
        $$->left = $1;
        $$->right = $3;
    }
    | Var MINUS_ASSIGN Exp
    {
        $$ = createNode(NODE_MINUS_ASSIGN);
        $$->left = $1;
        $$->right = $3;
    }
    | Var STAR_ASSIGN Exp
    {
        $$ = createNode(NODE_STAR_ASSIGN);
        $$->left = $1;
        $$->right = $3;
    }
    | Var DIV_ASSIGN Exp
    {
        $$ = createNode(NODE_DIV_ASSIGN);
        $$->left = $1;
        $$->right = $3;
    }
    | INCREMENT Var
    {
        $$ = createNode(NODE_INCREMENT);
        $$->left = $2;
    }
    | DECREMENT Var
    {
        $$ = createNode(NODE_DECREMENT);
        $$->left = $2;
    }
    | Var INCREMENT
    {
        $$ = createNode(NODE_INCREMENT);
        $$->left = $1;
    }
    | Var DECREMENT
    {
        $$ = createNode(NODE_DECREMENT);
        $$->left = $1;
    }
    | Exp PLUS Exp
    {
        $$ = createNode(NODE_PLUS);
        $$->left = $1;
        $$->right = $3;
    }
    | Exp MINUS Exp
    {
        $$ = createNode(NODE_MINUS);
        $$->left = $1;
        $$->right = $3;
    }
    | Exp STAR Exp
    {
        $$ = createNode(NODE_STAR);
        $$->left = $1;
        $$->right = $3;
    }
    | Exp DIV Exp
    {
        $$ = createNode(NODE_DIV);
        $$->left = $1;
        $$->right = $3;
    }
    | Exp EQ Exp
    {
        $$ = createNode(NODE_EQ);
        $$->left = $1;
        $$->right = $3;
    }
    | Exp NE Exp
    {
        $$ = createNode(NODE_NE);
        $$->left = $1;
        $$->right = $3;
    }
    | Exp GT Exp
    {
        $$ = createNode(NODE_GT);
        $$->left = $1;
        $$->right = $3;
    }
    | Exp GE Exp
    {
        $$ = createNode(NODE_GE);
        $$->left = $1;
        $$->right = $3;
    }
    | Exp LT Exp
    {
        $$ = createNode(NODE_LT);
        $$->left = $1;
        $$->right = $3;
    }
    | Exp LE Exp
    {
        $$ = createNode(NODE_LE);
        $$->left = $1;
        $$->right = $3;
    }
    | MINUS Exp %prec UMINUS
    {
        $$ = createNode(NODE_UMINUS);
        $$->left = $2;
    }
    | LP Exp RP
    {
        $$ = createNode(NODE_PAREN_EXP);
        $$->child = $2;
    }
    ;






/************************************************************/
%%

// 将token类型转换为可读字符串
const char* getTokenName(int token) {
	switch(token) {
		case LC: return "LC(左大括号)";
		case RC: return "RC(右大括号)";
		case LP: return "LP(左小括号)";
		case RP: return "RP(右小括号)";
		case LB: return "LB(左中括号)";
		case RB: return "RB(右中括号)";
		case SEMI: return "SEMI(分号)";
		case COMMA: return "COMMA(逗号)";
		case IF: return "IF(关键字)";
		case ELSE: return "ELSE(关键字)";
		case WHILE: return "WHILE(关键字)";
		case FOR: return "FOR(关键字)";
		case BREAK: return "BREAK(关键字)";
		case CONTINUE: return "CONTINUE(关键字)";
		case TYPE_INT: return "TYPE_INT(类型)";
		case TYPE_FLOAT: return "TYPE_FLOAT(类型)";
		case TYPE_CHAR: return "TYPE_CHAR(类型)";
		case SCAN: return "SCAN(关键字)";
		case PRINT: return "PRINT(关键字)";
		case ASSIGNOP: return "ASSIGNOP(赋值)";
		case PLUS_ASSIGN: return "PLUS_ASSIGN(+=)";
		case MINUS_ASSIGN: return "MINUS_ASSIGN(-=)";
		case STAR_ASSIGN: return "STAR_ASSIGN(*=)";
		case DIV_ASSIGN: return "DIV_ASSIGN(/=)";
		case INCREMENT: return "INCREMENT(++)";
		case DECREMENT: return "DECREMENT(--)";
		case PLUS: return "PLUS(+)";
		case MINUS: return "MINUS(-)";
		case STAR: return "STAR(*)";
		case DIV: return "DIV(/)";
		case EQ: return "EQ(==)";
		case NE: return "NE(!=)";
		case GE: return "GE(>=)";
		case LE: return "LE(<=)";
		case GT: return "GT(>)";
		case LT: return "LT(<)";
		case INT: return "INT(整数常量)";
		case FLOAT: return "FLOAT(浮点常量)";
		case CHAR: return "CHAR(字符常量)";
		case ID: return "ID(标识符)";
		default: return "UNKNOWN";
	}
}

// 词法分析：扫描并输出所有token
void performLexicalAnalysis(const char* filename) {
	FILE* fp = fopen(filename, "r");
	if (!fp) return;
	
	// 使用flex进行词法分析
	yyin = fp;
	yylineno = 1;
	
	printf("===== 词法分析结果 =====\n");
	printf("%-6s %-25s %-20s %s\n", "行号", "Token类型", "Token值", "词素");
	printf("-------------------------------------------------------------------\n");
	
	int token;
	int tokenCount = 0;
	while ((token = yylex()) != 0) {
		tokenCount++;
		printf("%-6d %-25s ", yylineno, getTokenName(token));
		
		// 根据token类型输出值
		switch(token) {
			case INT:
				printf("%-20d %s\n", yylval.type_int, yytext);
				break;
			case FLOAT:
				printf("%-20.2f %s\n", yylval.type_float, yytext);
				break;
			case CHAR:
				printf("%-20c %s\n", (char)yylval.type_int, yytext);
				break;
			case ID:
				printf("%-20s %s\n", yylval.type_id, yytext);
				break;
			default:
				printf("%-20s %s\n", "-", yytext);
				break;
		}
	}
	
	printf("-------------------------------------------------------------------\n");
	printf("共识别 %d 个token\n", tokenCount);
	printf("========================\n\n");
	
	fclose(fp);
}

int main(int argc, char *argv[]){
	// 如果开启源代码显示功能，先输出源代码
	#if SHOW_SOURCE_CODE
	printSourceCode(argv[1]);
	#endif
	
	// 词法分析：输出所有token
	performLexicalAnalysis(argv[1]);
	
	// 语法分析
	yyin=fopen(argv[1],"r");
	if (!yyin) return 0;
	yylineno=1;
	yyparse();
	return 0;
	}

#include<stdarg.h>
void yyerror(const char* fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    fprintf(stderr, "Grammar Error at Line %d Column %d: ", yylloc.first_line,yylloc.first_column);
    vfprintf(stderr, fmt, ap);
    fprintf(stderr, ".\n");
}
	

