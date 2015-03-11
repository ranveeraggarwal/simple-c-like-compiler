#include <string>
#include <vector>
#include <unordered_map>
using namespace std;

class Type{

};

class BasicType: public Type{
public:
	string identifier;
};

class ArrayType: public Type{

public:
	int size;
	Type type;

};

class Variable
{
private:
	string varname;
	Type type;
public:
	Variable(){};

	Variable(string varname, Type type)
	{
		this->varname = varname;
		this->type = type;
	}
};

class LocalSymbolTable
{
public:
	string funcName;
	vector<Variable> parameters;
	string returnType;
	unordered_map<string, Variable> variables;

	LocalSymbolTable(string funcName){
		this->funcName = funcName;
	}

	LocalSymbolTable(){};
};

class GlobalSymbolTable
{
public:
	vector<LocalSymbolTable*> lstList;
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

int main(){
	return 0;
}
