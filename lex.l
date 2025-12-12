%option yylineno
%{
#include "parser.tab.h"
#include "string.h"
#include "stdlib.h"
#include "def.h"

int yycolumn=1;
#define YY_USER_ACTION    \
	yylloc.first_line=yylloc.last_line=yylineno; \
	yylloc.first_column=yycolumn;\
	yylloc.last_column=yycolumn+yyleng-1;\
	yycolumn+=yyleng;

%}
id   [A-Za-z][A-Za-z0-9]*  
int    [0-9]+
float ([0-9]*\.[0-9]+)|([0-9]+\.)
char_const  '([^'\\]|\\.)'
%%
"{" { return LC; }
"}" { return RC; }
"(" { return LP; }
")" { return RP; }
"[" { return LB; }
"]" { return RB; }
";" { return SEMI; }
"," { return COMMA; }
"if" { return IF; }
"else" { return ELSE; }
"while" { return WHILE; }
"for" { return FOR; }
"break" { return BREAK; }
"continue" { return CONTINUE; }
"int" { return TYPE_INT; }
"float" { return TYPE_FLOAT; }
"char" { return TYPE_CHAR; }
"scan" { return SCAN; }
"print" { return PRINT; }
"=" { return ASSIGNOP; }
"+=" { return PLUS_ASSIGN; }
"-=" { return MINUS_ASSIGN; }
"*=" { return STAR_ASSIGN; }
"/=" { return DIV_ASSIGN; }
"++" { return INCREMENT; }
"--" { return DECREMENT; }
"+" { return PLUS; }
"-" { return MINUS; }
"*" { return STAR; }
"/" { return DIV; }
"==" { return EQ; }
"!=" { return NE; }
">=" { return GE; }
"<=" { return LE; }
">" { return GT; }
"<" { return LT; }
{int} { 
    yylval.type_int = atoi(yytext); 
    return INT; 
}
{float} { 
    yylval.type_float = atof(yytext); 
    return FLOAT; 
}
{char_const} {
    // 提取字符值，处理转义字符
    char ch = yytext[1];
    if (ch == '\\') {
        switch(yytext[2]) {
            case 'n': ch = '\n'; break;
            case 't': ch = '\t'; break;
            case '\\': ch = '\\'; break;
            case '\'': ch = '\''; break;
            default: ch = yytext[2]; break;
        }
    }
    yylval.type_int = (int)ch;
    return CHAR;
}
{id} { 
    strcpy(yylval.type_id, yytext); 
    return ID; 
}
"//".* { /* 行注释，忽略 */ }
"/*" { 
    int c;
    while ((c = input()) != 0) {
        if (c == '*') {
            c = input();
            if (c == '/') break;
            else unput(c);
        }
    }
}
[ \t\r] { /* 忽略空白字符（包括回车符\r） */ }
\n { yycolumn = 1; }
. { 
    fprintf(stderr, "[词法错误] 第%d行第%d列: 无法识别的字符 '%c' (ASCII: %d)\n", 
            yylineno, yycolumn, yytext[0], (unsigned char)yytext[0]); 
    yycolumn++; 
}
%%

int yywrap()
{
return 1;
}

