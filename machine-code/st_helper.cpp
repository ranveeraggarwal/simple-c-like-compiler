/*
	0: global
	1: local to function
	2: param
 */
#include <iostream>
#include <fstream>
#include <string>
using namespace std;

string typeArray[] = {"void", "int", "float"};
int scope = 0;
Type* type = NULL;
int offset = 0;
GlobalSymbolTable* gst = new GlobalSymbolTable();
LocalSymbolTable* currentLst = new LocalSymbolTable();
LocalSymbolTable* currentLstCalled = new LocalSymbolTable();
int paramCount = 0;

Type* copyType(Type* type){
	Type *temp = new Type();
	if (type->child != NULL){
		temp->child = copyType(type->child);
	}

	temp->base = type->base;
	temp->size = type->size;
	temp->isCasted = false;
	return temp;
}

Type* arithmetic_check(Type* first, Type* second){
	if (first->child  != NULL || second->child != NULL) return NULL;
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
