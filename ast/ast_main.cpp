#include <iostream>
#include <string>
#include <vector>
using namespace std;

class abstract_astnode
{
public:
	virtual void print () = 0;
	//virtual std::string generate_code(const symbolTable&) = 0;
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
	virtual void print() = 0;
	expAst(){}
	
};

class arrayRef: public expAst
{
public:
	virtual void print() = 0;
	arrayRef(){}
	
};

class stmtAst: public abstract_astnode
{
public:
	virtual void print() = 0;
	stmtAst(){}
	
};

class op: public expAst
{
public:
	expAst *exp1, *exp2;
	string opcode;
	bool optype;
	op(){}
	

	void print()
	{
		if (!optype)
		{
			cout << "(" << opcode << " ";
			exp1->print();
			cout << ")" << endl;
		}	
	}
};

class block_ast: public stmtAst
{
public:
	std::vector<stmtAst*> v;
	block_ast(){}
	

	void print()
	{
		cout << "(Block[";
		for (int i = 0; i < v.size(); i++)
		{
			cout << " ";
			v[i]->print();
			if (i != v.size()-1)cout << endl;
		}
		cout << "])";
	}
};

class fun_call: public expAst
{
public:
	string fun_name;
	std::vector<expAst*> v;
	fun_call(){}
	

	void print()
	{
		cout << fun_name;
		for (int i = 0; i < v.size(); i++)
		{
			cout << " ";
			v[i]->print();
		}
		cout << ")" << endl;
	}
};

class float_constant: public expAst
{
public:
	float value;
	float_constant(){}
	

	void print()
	{
		cout << "(FloatConst \"";
		cout << value;
		cout << "\")" << endl;
	}
};

class int_constant: public expAst
{
public:
	int value;
	int_constant(){}
	

	void print()
	{
		cout << "(IntConst \"";
		cout << value;
		cout << "\")" << endl;
	}
};

class string_constant: public expAst
{
public:
	string value;
	string_constant(){}
	

	void print()
	{
		cout << "(StringCont \"";
		cout << value;
		cout << "\")" << endl;
	}
};

class identifier_exp: public expAst
{
public:
	string value;
	identifier_exp(){}
	

	void print()
	{
		cout << "(Id \"";
		cout << value;
		cout << "\")" << endl;
	}
};

class identifier: public arrayRef
{
public:
	string value;
	identifier(){}
	

	void print()
	{
		cout << "(Id \"";
		cout << value;
		cout << "\")" << endl;
	}
};

class index: public arrayRef
{
public:
	arrayRef *arr;
	expAst *exp;
	index(){}
	

	void print()
	{
		cout << "(Ref ";
		arr->print();
		cout << "[";
		exp->print();
		cout << "]";
		cout << ")" << endl;
	}
};

class seq: public stmtAst
{
public:
	stmtAst *stmt1, *stmt2;
	seq(){}
	

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
	

	void print()
	{
		if (exp1 == 0 && exp2==0)
		{
			cout << "(Empty)";
		}
		else
		{
			cout << "(Ass ";
			exp1->print();
			exp2->print();
			cout << ")" << endl;
		}
	}
};

class return_stmt: public stmtAst
{
public:
	expAst *exp;
	return_stmt(){}
	

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
	

	void print()
	{
		cout << "(If ";
		exp->print();
		stmt1->print();
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
	

	void print(){
		cout << "(While ";
		exp->print();
		stmt->print();
		cout << ")" << endl;
	}
};

class for_stmt: public stmtAst
{
public:
	expAst *exp1, *exp2, *exp3;
	stmtAst *stmt;
	for_stmt(){}
	

	void print(){
		cout << "(For ";
		exp1->print();
		exp2->print();
		exp3->print();
		stmt->print();
		cout << ")" << endl;
	}
};

/*
int main(int argc, char const *argv[])
{
	
	return 0;
}
*/