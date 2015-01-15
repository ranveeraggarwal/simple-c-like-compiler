all:    lex.cc  main.cpp
	g++   --std=c++0x lex.cc main.cpp -o lex-analyser

lex.cc: lex.l
	flexc++ lex.l; 
	#sed -i '/int lex()/r enums' Scanner.h

clean:
	rm lex.cc Scannerbase.h Scanner.ih Scanner.h lex-analyser
