/*
	0: global
	1: local to function
	2: param
 */
int scope = 0;
Type* type = NULL;
int offset = 0;
GlobalSymbolTable* gst = new GlobalSymbolTable();
LocalSymbolTable* currentLst = new LocalSymbolTable();