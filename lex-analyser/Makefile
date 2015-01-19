all:   clean lex.cc  main.cc
	g++   --std=c++0x lex.cc main.cc -o lex-analyser

lex.cc: lex.l
	flexc++ lex.l; 
	sed -i '/int lex()/r enums' Scanner.h

clean:
	rm -f lex.cc Scannerbase.h Scanner.ih Scanner.h lex-analyser
