# ---------- Makefile ----------

parser: parser.tab.c lex.yy.c displayAST.c semantics.c codegen.c mips_gen.c
	gcc parser.tab.c lex.yy.c displayAST.c semantics.c codegen.c mips_gen.c -o parser

parser.tab.c parser.tab.h: parser.y
	bison -d parser.y

lex.yy.c: lex.l
	flex lex.l

clean:
	rm -f parser.tab.c parser.tab.h lex.yy.c parser

# ---------- End of Makefile ----------