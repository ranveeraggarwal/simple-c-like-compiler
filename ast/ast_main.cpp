#include <iostream>
#include <string>
#include <vector>
using namespace std;

class abstract_astnode
{
public:
	//virtual void print () = 0;
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
	expAst();
	~expAst();
};

class arrayRef: public abstract_astnode
{
public:
	arrayRef();
	~arrayRef();
};

class stmtAst: public abstract_astnode
{
public:
	stmtAst();
	~stmtAst();
};

class op: public expAst
{
public:
	expAst exp1, exp2;
	string opcode;
	bool optype;
	op();
	~op();
	
};

class fun_call: public expAst
{
public:
	std::vector<expAst> v;
	fun_call();
	~fun_call();
};

class float_constant: public expAst
{
public:
	float value;
	float_constant();
	~float_constant();
};

class int_constant: public expAst
{
public:
	int value;
	int_constant();
	~int_constant();
};

class string_constant: public expAst
{
public:
	string value;
	string_constant();
	~string_constant();
};

class identifier_exp: public expAst
{
public:
	string value;
	identifier_exp();
	~identifier_exp();
};

class identifier: public arrayRef
{
public:
	string value;
	identifier();
	~identifier();
};

class index: public arrayRef
{
public:
	arrayRef arr;
	expAst exp;
	index();
	~index();
};

class seq: public stmtAst
{
public:
	stmtAst stmt1, stmt2;
	seq();
	~seq();
};

class ass: public stmtAst
{
public:
	expAst exp1, exp2;
	ass();
	~ass();
};

class return_stmt: public stmtAst
{
public:
	expAst exp;
	return_stmt();
	~return_stmt();
};

class if_stmt
{
public:
	expAst exp;
	stmtAst stmt1, stmt2;
	if_stmt();
	~if_stmt();
};

class while_stmt
{
public:
	expAst exp;
	stmtAst stmt;
	while_stmt();
	~while_stmt();
};

class for_stmt
{
public:
	expAst exp1, exp2, exp3;
	stmtAst stmt;
	for_stmt();
	~for_stmt();
};

int main(int argc, char const *argv[])
{
	/* code */
	return 0;
}