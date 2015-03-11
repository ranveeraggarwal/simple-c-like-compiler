#include <string>
#include <vector>
using namespace std;

class Var
{
private:
	string varname;
	string type;
public:
	Var(string varname, string type)
	{
		varname = varname;
		type = type;
	}
};

class SymbolTable
{
	
};

class Gst: public symbolTable
{
private:
	vector<Lst> lstList;
public:
	lst* getLst(string funcName)
	{
		for (unsigned int i = 0; i < lstList.size(); i++)
		{
			if (lstList[i].funcName == funcName)
			{
				return lstList[i];
			}
			return nullLst;
		}
	}
};

class Lst: public symbolTable
{
private:
	lst* parentSt;
	string funcName;
	vector<Var> scopeVars;
};