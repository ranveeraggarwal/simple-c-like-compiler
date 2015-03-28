#include <string>
#include <vector>
#include <unordered_map>
using namespace std;

class Type{
	public:
		int size;
		Type(){};
};

class BasicType: public Type{
public:
	string identifier;
	BasicType(){
		size = 0;
	};
};

class ArrayType: public Type{

public:
	Type* type;
	ArrayType(){};

};

class Variable
{
public:
	string varname;
	Type* type;
	int scope;
	int size;
	int offset;
	Variable(){
		//cout<<"Kuch to hai"<<endl;
		size = 0;
		offset = 0;
		type = NULL;
	};

	Variable(string varname, Type* type)
	{
		cout<<varname<<" "<<((BasicType*)type)->identifier<<endl;
		this->varname = varname;
		this->type = type;
	}
};

class LocalSymbolTable
{
public:
	string funcName;
	string returnType;
	unordered_map<string, Variable*> variables;

	LocalSymbolTable(string funcName){
		this->funcName = funcName;
	}

	LocalSymbolTable(){};
};

class GlobalSymbolTable
{
public:
	vector<LocalSymbolTable*> lstList;

	GlobalSymbolTable(){

	}

	LocalSymbolTable* getLst(string funcName)
	{
		for (unsigned int i = 0; i < lstList.size(); i++)
		{
			if (lstList[i]->funcName == funcName)
			{
				return lstList[i];
			}
			return nullptr;
		}
	}
};

/*
	0: global
	1: local to function
	2: param
 */
extern int scope;
extern Type* type;
extern int offset;
extern GlobalSymbolTable* gst;
extern LocalSymbolTable* currentLst;