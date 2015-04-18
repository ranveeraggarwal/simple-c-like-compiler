#include <iostream>
#include <string>
#include <vector>
#include <typeinfo>
#include "ast.h"
using namespace std;

class abstract_astnode
{
public:
	Type* type;
	virtual void print () = 0;
	//virtual std::string generate_code(const symbolTable&) = 0;
	void generate_code();
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
	expAst(){
		fallthrough = false;
		truelist = new InstrList();
		falselist = new InstrList();
		nextlist = new InstrList();
	}
	
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
	InstrList* nextlist;
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
	
	void generate_code()
	{
		if (optype == 0)
		{
			if (opcode == "++")
			{
				if (type->base == 1)
				{
					Register* top = registers.top();
					instructions.push_back(new Instruction("addi", "1", top->name));
				}
				else if (type->base == 2)
				{
					Register* top = registers.top();
					instructions.push_back(new Instruction("addf", "1", top->name));
				}
			}
			else if (opcode == "-")
			{
				if (type->base == 1)
				{
					Register* top = registers.top();
					instructions.push_back(new Instruction("muli", "-1", top->name));
				}
				else if (type->base == 2)
				{
					Register* top = registers.top();
					instructions.push_back(new Instruction("mulf", "-1", top->name));
				}
			}
		}
		else
		{
			
		}
	}

	void print()
	{
		cout << nextLabel();
		if (!optype)
		{
			cout << "(" << opcode << " ";
			exp1->print();
			cout << ")";
		}
		else{
			cout<<"( "<<opcode<<" ";

			exp1->print();
			exp2->print();
			cout<<" )";
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

class exp_list
{
public:
	std::vector<expAst*> v;
	exp_list(){};

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

class int_constant: public expAst
{
public:
	int value;
	int_constant(){}
	

	void print()
	{
		cout<<"(";
		type->print();
		cout<<") ";
		cout << "(IntConst \"";
		cout << value;
		cout << "\")" ;
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
		cout << "\")";
	}
};

class identifier_exp: public expAst
{
public:
	string value;
	identifier_exp(){}
	

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
	void generate_code(){
		if (type->base == 1){
	    Register* top = registers.top();
	    instructions.push_back(new Instruction("loadi", value, top->name));
		}
		else if (type->base == 2){
		    Register* top = registers.top();
		    instructions.push_back(new Instruction("loadf", value, top->name));  
		}
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
		cout << ")";
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
	
	void generate_code(){
		if (exp->type->base == 1) { //expression is of type int
			if (int_constant* i = dynamic_cast<int_constant*>(exp)){
				int temp = ((int_constant*)exp)->value;
				instructions.push_back(new Instruction("storei", to_string(temp), "ind(ebp," + to_string(offset) + ")"));
			}
			else {
				exp->generate_code();
			    Register* reg = registers.top();
			    instructions.push_back(new Instruction("storei", reg->name, "ind(ebp, "  + to_string(offset) + ")"));
			}
		}
		else { //float
			if (float_constant* i = dynamic_cast<float_constant*>(exp)){
				int temp = ((float_constant*)exp)->value;
				instructions.push_back(new Instruction("storef", to_string(temp), "ind(ebp," + to_string(offset) + ")"));
			}
			else {
				exp->generate_code();
			    Register* reg = registers.top();
			    instructions.push_back(new Instruction("storef", reg->name, "ind(ebp, "  + to_string(offset) + ")"));
			}
		}
	}

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

	void generate_code(){
	  exp->fallthrough = 1;               // expression.fall = 1
	  exp->generate_code();      
	  int stmt1M = instructions.size();
	  stmt1->generate_code();
	  Instruction* code = new Instruction("jl");
	  instructions.push_back(code);
	  nextlist->instrList.push_back(code);
	  int stmt2M = instructions.size();
	  stmt2->generate_code();
	  (exp->truelist)->backpatch(instructions[stmt1M]);
	  (exp->falselist)->backpatch(instructions[stmt2M]);
	  nextlist->merge(stmt1->nextlist);
	  nextlist->merge(stmt2->nextlist);
	}
	

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

	void generate_code(){
	  exp->fallthrough = true;
	  int expM = instructions.size();
	  exp->generate_code();
	  int stmtM = instructions.size();
	  stmt->generate_code();
	  (exp->truelist)->backpatch(instructions[expM]);
	  (exp->nextlist)->backpatch(instructions[stmtM]);
	  nextlist = exp->falselist;
	  Instruction* code = new Instruction("jl");
	  instructions.push_back(code);
	  code->backpatch(instructions[expM]);
	}
	

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
	
	void generate_code(){
		exp2->fallthrough = 1;
		exp1->generate_code();
		int exp2M = instructions.size();
		exp2->generate_code();
		int stmtM = instructions.size();
		stmt->generate_code();
		int exp3M = instructions.size();
		exp3->generate_code();
		Instruction* code = new Instruction("jl");
		instructions.push_back(code);
		(exp2->truelist)->backpatch(instructions[stmtM]);
		(stmt->nextlist)->backpatch(instructions[exp3M]);
		code->backpatch(instructions[exp2M]);
		nextlist = exp2->falselist;
	}

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