all: clean lex.cc parse.cc main.cc Scanner.h Scannerbase.h Scanner.ih Parser.h Parserbase.h Parser.ih
	./sedscript
	g++  --std=c++0x lex.cc parse.cc main.cc;
	./a.out < testcase_1 > junk.scm;        

lex.cc: lex.l Scanner.ih 
	./cond_remove_scannerih; 
	flexc++ lex.l; 
	sed -i '/include/a #include "Parserbase.h"' Scanner.ih; 

parse.cc: parse.y Parser.ih Parser.h Parserbase.h
	bisonc++ parse.y; 
#	bisonc++   --construction -V parse.y; 
#	sed -i '/ifndef/a #include "tree_util.hh"' Parserbase.h;
#	sed -i '/ifndef/a #include "tree.hh"' Parserbase.h;
#	./sedscript
     
clean:
	rm -f Parser.h Parser.ih Parserbase.h Scanner* a.out lex.cc parse.cc junk.scm

Parser.ih: parse.y
Parser.h:  parse.y
Parserbase.h: parse.y
Scanner.ih: lex.l
Scanner.h: lex.l
Scannerbase.h: lex.l
