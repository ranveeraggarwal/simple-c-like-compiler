#include <iostream>
#include <string>
#include <vector>
#include <typeinfo>
#include <unordered_map>
#include <stack>
using namespace std;

extern string typeArray[3];
struct Type{
	int size;
	int base;
	Type* child;
	bool isCasted;
	int cast;

	Type();
	Type(int);
	Type(Type*, int);
	void castTo(Type*);
	void print();

};

class Variable
{
public:
	string varname;
	Type* type;
	int scope;
	int size;
	int offset;

	Variable();
	Variable(string, Type*);

};

class LocalSymbolTable
{
public:
	string funcName;
	Type* returnType;
	bool checkParam = true;
	unordered_map<string, Variable*> variables;
	vector<Variable*> params;

	LocalSymbolTable(string);
	LocalSymbolTable();
	void print();

};

class GlobalSymbolTable
{
public:
	vector<LocalSymbolTable*> lstList;

	GlobalSymbolTable();
	LocalSymbolTable* getLst(string);
	void print();

};

class Register
{
public:
	string name;
	Register(string);
};

class Instruction{
public:
	string name, first, second;
	bool isLabel;
	bool isGoto;

	Instruction(string name, string first, string second);
	Instruction(string name);
	Instruction(string name, string first);
	void setLabel();
	void backpatch(Instruction* instr);
};

class InstrList{
public:
	std::vector<Instruction*> instrList;

	void backpatch(Instruction*);
	void merge(InstrList*);
};



/*
Extern Variable
*/
/*
extern int labelNumber;
extern stack<Register*> registers;
extern vector<Instruction*> instructions;
*/


class abstract_astnode
{
public:
	Type* type;
	virtual void print () = 0;
	//virtual std::string generate_code(const symbolTable&) = 0;
	virtual void generate_code() = 0;
	//virtual basic_types getType() = 0;
	//virtual bool checkTypeofAST() = 0;
protected:
	//virtual void setType(basic_types) = 0;
private:
	//typeExp astnode_type;
};

class expAst: public abstract_astnode
{
public:
	InstrList *truelist, *falselist, *nextlist;
	bool fallthrough;

	virtual void print() = 0;
	virtual void generate_code() = 0;
	expAst();
	
};

class int_constant: public expAst
{
public:
	int value;
	int_constant(){}

	void generate_code();
	void print();
	
};

class arrayRef: public expAst
{
public:
	virtual void print() = 0;
	virtual void generate_code() = 0;
	arrayRef(){}
	
};

class stmtAst: public abstract_astnode
{
public:
	InstrList* nextlist;
	virtual void print() = 0;
	virtual void generate_code() = 0;
	stmtAst(){}
	
};

class op: public expAst
{
public:
	expAst *exp1, *exp2;
	string opcode;
	bool optype;
	op(){}

	void  generate_code();

	void print();
};

class block_ast: public stmtAst
{
public:
	std::vector<stmtAst*> v;
	block_ast(){};

	void generate_code();
	
	void print();
};

class exp_list
{
public:
	std::vector<expAst*> v;
	exp_list(){};

	void generate_code(){
		
	}

	void print()
	{
		for (int i = 0; i < v.size(); i++)
		{
			cout << " ";
			v[i]->print();
		}
	}
};

class fun_call: public expAst
{
public:
	string fun_name;
	exp_list* expList;
	fun_call(){}

	void generate_code();
	
	void print()
	{
		cout<<"(";
		type->print();
		cout<<" ";
		cout << fun_name;
		expList->print();
		cout<<")";
	}
};

class fun_call_stmt: public stmtAst
{
public:
	string fun_name;
	exp_list* expList;
	fun_call_stmt(){}

	void generate_code(){
		
	}
	
	void print()
	{
		cout<<"(";
		cout << fun_name;
		expList->print();
		cout<<")";
	}
};

class float_constant: public expAst
{
public:
	float value;
	float_constant(){}
	
	void generate_code();

	void print()
	{
		cout<<"(";
		type->print();
		cout<<") ";
		cout << "(FloatConst \"";
		cout << value;
		cout << "\")";
	}
};

class string_constant: public expAst
{
public:
	string value;
	string_constant(){}
	
	void generate_code(){
		
	}

	void print()
	{
		cout << "(StringCont \"";
		cout << value;
		cout << "\")";
	}
};

class identifier_exp: public expAst
{
public:
	string value;
	identifier_exp(){}

	void generate_code(){
		
	}	

	void print()
	{
		cout<<"(";
		type->print();
		cout<<") ";
		cout << "(Id \"";
		cout << value;
		cout << "\")";
	}
};

class identifier: public arrayRef
{
public:
	string value;
	Variable* var;
	identifier(){}
	void generate_code();

	void print()
	{
		cout<<"(";
		type->print();
		cout<<") ";
		cout << "(Id \"";
		cout << value;
		cout << "\")";
	}
};

class index: public arrayRef
{
public:
	arrayRef *arr;
	expAst *exp;
	index(){}

	void generate_code();

	void generate_code_for_lhs();

	void generate_code_helper();

	void print()
	{
		cout << "(Ref ";
		arr->print();
		cout << "[";
		exp->print();
		cout << "]";
		cout << ")";
	}
};

class seq: public stmtAst
{
public:
	stmtAst *stmt1, *stmt2;
	seq(){}

	void generate_code(){
		
	}

	void print()
	{
		stmt1->print();
		stmt2->print();
	}
};

class ass: public stmtAst
{
public:
	expAst *exp1, *exp2;
	ass(){}

	void generate_code();
	
	void print()
	{
		if (exp1 == 0 && exp2==0)
		{
			cout << "(Empty)";
		}
		else
		{
			cout << "(Ass (";
			exp1->print();
			cout<<") (";
			exp2->print();
			cout << "))";
		}
	}
};

class return_stmt: public stmtAst
{
public:
	expAst *exp;
	int offset;
	return_stmt(){}
	
	void generate_code();

	void print()
	{
		cout << "(Return ";
		exp->print();
		cout << ")";
	}
};

class if_stmt: public stmtAst
{
public:
	expAst *exp;
	stmtAst *stmt1, *stmt2;
	if_stmt(){}

	void generate_code();
	
	void print()
	{
		cout << "(If ";
		exp->print();
		cout<<endl;
		stmt1->print();
		cout<<endl;
		stmt2->print();
		cout << ")";
	}
};

class while_stmt: public stmtAst
{
public:
	expAst *exp;
	stmtAst *stmt;
	while_stmt(){}

	void generate_code();

	void print(){
		cout << "(While ";
		exp->print();
		cout<<endl;
		stmt->print();
		cout << ")";
	}
};

class for_stmt: public stmtAst
{
public:
	expAst *exp1, *exp2, *exp3;
	stmtAst *stmt;
	for_stmt(){}
	
	void generate_code();

	void print(){
		cout << "(For ";
		exp1->print();
		cout<<endl;
		exp2->print();
		cout<<endl;
		exp3->print();
		cout<<endl;
		stmt->print();
		cout << ")";
	}
};