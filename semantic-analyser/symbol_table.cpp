#include <string>
#include <vector>
#include <unordered_map>
using namespace std;

struct Type{
	int size;
	int base;
	Type* child;
	Type(){size = -1; child=NULL;}
	Type(int tp){
		child = NULL;
		switch(tp){
			case 0: // VOID
				size = 0;
				base = 0;
				break;
			case 1: // INT
				size = 4;
				base = 1;
				break;
			case 2: // FLOAT
				size = 4;
				base = 2;
				break;
		}
	}
	Type(Type* c,int s){
		child = c;
		size = s;
	}

	void print(){
		if(child != NULL){
			cout<<"array("<<size<<",";
				child->print();
			cout<<")";
		}
		else if(base == 0) cout<<"void";
		else if(base == 1) cout<<"int";
		else if(base == 2) cout<<"float";
	}
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
		this->varname = varname;
		this->type = type;
	}
};

class LocalSymbolTable
{
public:
	string funcName;
	Type* returnType;
	unordered_map<string, Variable*> variables;

	LocalSymbolTable(string funcName){
		this->funcName = funcName;
	}

	LocalSymbolTable(){};

	void print(){
		cout<<"\n\tLocal Symbol Table for function: "<<funcName<<endl;
		cout<<"\tReturn Type: ";
		returnType->print();
		cout<<endl;
		cout<<"\t\tName\ttype\tscope\tsize\toffset\t"<<endl;
		for ( auto it = variables.begin(); it != variables.end(); ++it ){
			Variable* temp = it->second;
			cout<<"\t\t"<<temp->varname<<"\t";
			temp->type->print();
			cout<<"\t"<<temp->scope<<"\t"<<temp->size<<"\t"<<temp->offset<<endl;
		}
	}
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
			
		}
		return nullptr;
	}

	void print(){
		cout<<"\n\nPrinting SymbolTables:"<<endl;
		for (int i = 0; i< lstList.size(); i++){
			lstList[i]->print();
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