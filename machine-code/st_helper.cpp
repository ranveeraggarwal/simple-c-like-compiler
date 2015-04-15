/*
	0: global
	1: local to function
	2: param
 */
#include <iostream>
#include <fstream>
using namespace std;

int scope = 0;
Type* type = NULL;
int offset = 0;
GlobalSymbolTable* gst = new GlobalSymbolTable();
LocalSymbolTable* currentLst = new LocalSymbolTable();
LocalSymbolTable* currentLstCalled = new LocalSymbolTable();
int paramCount = 0;

Type* arithmetic_check(Type* first, Type* second){
	if (first->base == 1 && second->base == 1){
		return first;
	}
	if (first->base == 2 && (second->base == 1 || second->base == 2)){
		return first;
	}
	if (second->base == 2 && (first->base == 2 || first->base == 1)){
		return second;
	}
	return NULL;
}

Type* compatibility_check(Type* first , Type* second){
	if(first->base == 2 && (second->base == 2 || second->base == 1))
		return first;
	if(first->base == 1 && (second->base == 2 || second->base == 1))
		return first;
	if(first->base == 1 &&  second->base == 1)
		return first;
	else return NULL;
}

Type* extended_compatibility_check(Type* first, Type* second){
	if(first->base == 2 && (second->base == 2 || second->base == 1))
		return first;
	if(first->base == 1 && (second->base == 2 || second->base == 1))
		return first;
	if(first->base == 1 &&  second->base == 1)
		return first;
	if (first->base == 0 && second->base == 0)
		return first;
	else return NULL;
}
