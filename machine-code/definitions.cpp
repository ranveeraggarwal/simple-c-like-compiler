#include <map>

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

map<string,string> fallThrough;
int labelNumber = 0;

stack<Register*> registers;
vector<Instruction*> instructions;

void initializeStack(){
	registers.push(new Register("eax"));
	registers.push(new Register("ebx"));
	registers.push(new Register("ecx"));
	registers.push(new Register("edx"));
	registers.push(new Register("eex"));
	registers.push(new Register("efx"));
}


string nextLabel()
{
	string newLabel = "l" + to_string(labelNumber);
	labelNumber++;
	return newLabel;
}

void swapRegisters(){
  if (registers.size() >= 2){
    Register* first = registers.top();
    registers.pop();
    Register* second = registers.top();
    registers.pop();
    registers.push(first);
    registers.push(second);
  }
}

	
string fallthroughinstr(string op){
  if (op == "<")
    return "jge";
  else if (op == ">")
    return "jle";
  else if (op == "<=")
    return "jl";
  else if (op == ">=")
    return "jg";
  else if (op == "==")
    return "jne";
  else if (op == "!=")
    return "je";
  else return "j";
}

string notfallthroughinstr(string op){
  if (op == "<")
    return "jl";
  else if (op == ">")
    return "jg";
  else if (op == ">=")
    return "jge";
  else if (op == "<=")
    return "jle";
  else if (op == "==")
    return "je";
  else if (op == "!=")
    return "jne";
  else return "j";
}

Type::Type(){size = -1; child=NULL; isCasted = 0;}
Type::Type(int tp){
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
	isCasted = 0;
}
Type::Type(Type* c,int s){
	child = c;
	size = s;
	isCasted = 0;
}

void Type::castTo(Type* type){
	isCasted = 1;
	cast = type->base;
}

void Type::print(){
	if(child != NULL){
		cout<<"array("<<size<<",";
			child->print();
		cout<<")";
	}
	else{
		if (isCasted){
		cout<<" (to_"<<typeArray[cast]<<") ";
		}
		if(base == 0) cout<<"void";
		if(base == 1) cout<<"int";
		if(base == 2) cout<<"float";
	}
	
	
}


Variable::Variable(){
	//cout<<"Kuch to hai"<<endl;
	size = 0;
	offset = 0;
	type = NULL;
};

Variable::Variable(string varname, Type* type)
{
	this->varname = varname;
	this->type = type;
}

LocalSymbolTable::LocalSymbolTable(string funcName){
	this->funcName = funcName;
}

LocalSymbolTable::LocalSymbolTable(){};

void LocalSymbolTable::print(){
	cout<<"\n\tLocal Symbol Table for function: "<<funcName<<endl;
	cout<<"\tReturn Type: ";
	returnType->print();
	cout<<endl;
	cout<<"\tArguments:"<<endl;
	cout<<"\t\tName\tsize\toffset\ttype"<<endl;
	for (int i=0; i< params.size(); i++){
		Variable* temp = params[i];
		cout<<"\t\t"<<temp->varname;
		cout<<"\t"<<temp->size<<"\t"<<temp->offset<<"\t";
		temp->type->print();
		cout<<endl;
	}
	cout<<endl;
	cout<<"\tLocal Variables:"<<endl;
	cout<<"\t\tName\tsize\toffset\ttype"<<endl;
	for ( auto it = variables.begin(); it != variables.end(); ++it ){
		Variable* temp = it->second;
		if (temp->scope == 2) continue;
		cout<<"\t\t"<<temp->varname;
		cout<<"\t"<<temp->size<<"\t"<<temp->offset<<"\t";
		temp->type->print();
		cout<<endl;
	}
}

GlobalSymbolTable::GlobalSymbolTable(){
	
	LocalSymbolTable* tempLst = new LocalSymbolTable();
	tempLst->funcName = "printf";
	tempLst->returnType = new Type(0);
	tempLst->checkParam = false;
	this->lstList.push_back(tempLst);
}

LocalSymbolTable* GlobalSymbolTable::getLst(string funcName)
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

void GlobalSymbolTable::print(){
	cout<<"\n\nPrinting SymbolTables:"<<endl;
	for (int i = 0; i< lstList.size(); i++){
		lstList[i]->print();
	}
}

Register::Register(string name){
		this->name;
	}

Instruction::Instruction(string name, string first, string second){
	this->name = name;
	this->first = first;
	this->second = second;
	isLabel = false;
	isGoto = false;
}

Instruction::Instruction(string name){
	this->name = name;
}

Instruction::Instruction(string name, string first){
	this->name = name;
	this->first = first;
	this->second = "";
}

void Instruction::setLabel(){
	if (isLabel) return;
	name = nextLabel();
	isLabel = true;
}

void Instruction::backpatch(Instruction* instr){
	instr->setLabel();
	name = instr->name;
}

void InstrList::backpatch(Instruction* instr){
	for (int i=0; i< instrList.size(); i++){
		instrList[i]->backpatch(instr);
	}
}

void InstrList::merge(InstrList* instList){
	this->instrList.insert(this->instrList.end(), instList->instrList.begin(), instList->instrList.end());
}

expAst::expAst(){
	fallthrough = false;
	truelist = new InstrList();
	falselist = new InstrList();
	nextlist = new InstrList();
}

void int_constant::print()
{
    cout<<"(";
	type->print();
	cout<<") ";
	cout << "(IntConstant \"";
	cout << value;
	cout << "\")";
}


void op :: generate_code()
{
	if (optype == 0)
	{
		if (opcode == "++")
		{
			if (exp1->type->base == 1)
			{
				exp1->generate_code();
				Register* top = registers.top();
				instructions.push_back(new Instruction("addi", "1", top->name));
			}
			else if (exp1->type->base == 2)
			{
				exp1->generate_code();
				Register* top = registers.top();
				instructions.push_back(new Instruction("addf", "1", top->name));
			}
		}
		else if (opcode == "-")
		{
			if (exp1->type->base == 1)
			{
				exp1->generate_code();
				Register* top = registers.top();
				instructions.push_back(new Instruction("muli", "-1", top->name));
			}
			else if (exp1->type->base == 2)
			{
				exp1->generate_code();
				Register* top = registers.top();
				instructions.push_back(new Instruction("mulf", "-1", top->name));
			}
		}
	}
	else
	{
		if (opcode == "||"){
			exp1->fallthrough = false;
		    exp2->fallthrough = fallthrough;
		    exp1->generate_code();
		    int exp2M = instructions.size();
		    exp2->generate_code();
		    (exp1->falselist)->backpatch(instructions[exp2M]);
		    truelist->merge(exp1->truelist);
		    truelist->merge(exp2->truelist);
		    falselist = exp2->falselist;
		}

		if (opcode == "&&"){
			exp1->fallthrough = true;
		    exp2->fallthrough = fallthrough;
		    exp1->generate_code();
		    int exp2M = instructions.size();
		    exp2->generate_code();
		    (exp1->truelist)->backpatch(instructions[exp2M]);
		    falselist->merge(exp1->falselist);
		    falselist->merge(exp2->falselist);
		    truelist = exp2->truelist;
		}

		else if (opcode == "="){
			if (exp1->type->base = 1 && exp2->type->base == 1){

				bool isExp2Const = false;
				int_constant* tempI2;
				if (tempI2 = dynamic_cast<int_constant*>(exp2)) isExp2Const = true;

				if (isExp2Const){

					int val = tempI2->value;
					if(identifier* id = dynamic_cast<identifier*> (exp1)){
						// Left side is an identifier
						int offset = id->var->offset;
						Register* top = registers.top();
						instructions.push_back(new Instruction("movei", to_string(val), top->name));
						instructions.push_back(new Instruction("storei", top->name, "ind(ebp+"+to_string(offset)+")"));

					}
					else {
						index* ind = dynamic_cast<index*> (exp1);
						ind->generate_code_for_lhs();
						Register* top = registers.top();
						string topName = top->name;
						instructions.push_back(new Instruction("storei", to_string(val), "ind("+topName+")"));
						instructions.push_back(new Instruction("movei" , to_string(val), topName));
					}
				}
				else {

					if(identifier* id = dynamic_cast<identifier*> (exp1)){
						// Left side is an identifier
						exp2->generate_code();
						Register* top = registers.top();
						string topName = top->name;
						int offset = id->var->offset;
						instructions.push_back(new Instruction("storei", topName, "ind(ebp, " + to_string(offset) + ")"));

					}
					else {
						index* ind = dynamic_cast<index*> (exp1);
						exp2->generate_code();
						Register* top = registers.top();
						string topName = top->name;
						instructions.push_back(new Instruction("pushi", topName));

						swapRegisters();
						ind->generate_code_for_lhs();
						swapRegisters();

						top = registers.top();
						topName = top->name;

						registers.pop();

						instructions.push_back(new Instruction("loadi", "ind(esp)", topName));
						instructions.push_back(new Instruction("popi", "1"));
						Register* top2 = registers.top();

						string top2Name = top2->name;

						instructions.push_back(new Instruction("storei", topName, "ind("+ top2Name + ")"));

						registers.push(top);
					}

				}
			}
			else {
				//TODO for float with proper casting
			}
		}

		else{
			if (exp1->type->base == 1 && exp2->type->base == 1){
				bool isExp1Const = false;
				int_constant* tempI1;
				if (tempI1 = dynamic_cast<int_constant*>(exp1)){
					isExp1Const = true;
				}
				bool isExp2Const = false;
				int_constant* tempI2;
				if (tempI2 = dynamic_cast<int_constant*>(exp2)) isExp2Const = true;
				if (isExp1Const && isExp2Const){
					int exp1val = tempI1->value;
					int exp2val = tempI2->value;

					Register* top = registers.top();
					if (opcode == "+"){
						instructions.push_back(new Instruction("movei", to_string(exp1val), top->name));
						instructions.push_back(new Instruction("addi", to_string(exp2val), top->name));
					}

					else if (opcode == "-"){
						exp2val *= -1;
						instructions.push_back(new Instruction("movei", to_string(exp1val), top->name));
						instructions.push_back(new Instruction("addi", to_string(exp2val), top->name));
					}

					else if (opcode == "*"){
						instructions.push_back(new Instruction("movei", to_string(exp1val), top->name));
						instructions.push_back(new Instruction("muli", to_string(exp2val), top->name));
					}

					else if (opcode == "/"){
						instructions.push_back(new Instruction("movei", to_string(exp1val), top->name));
						instructions.push_back(new Instruction("divi", to_string(exp2val), top->name));
					}

					else if (opcode == "<" || opcode == ">" || opcode == "<=" 
						|| opcode == ">=" || opcode == "==" || opcode == "!="){
						instructions.push_back(new Instruction("movei", to_string(exp1val), top->name));
						instructions.push_back(new Instruction("compi", to_string(exp2val), top->name));

						if (fallthrough){
							Instruction* code = new Instruction(fallthroughinstr(opcode));
							instructions.push_back(code);
							falselist->instrList.push_back(code);
						}
						else{
							Instruction* code = new Instruction(notfallthroughinstr(opcode));
							instructions.push_back(code);
							truelist->instrList.push_back(code);
						}
					}
				}
				else if (isExp2Const || isExp1Const){
					expAst* constant_exp;
					expAst* nonconstant_exp;
					if (isExp1Const){
						constant_exp = exp1;
						nonconstant_exp = exp2;
					}
					else {
						constant_exp = exp2;
						nonconstant_exp = exp1;
					}
					nonconstant_exp->generate_code();
					int val = ((int_constant*)constant_exp)->value;
					Register* top = registers.top();
					string regName = top->name;
					if (opcode == "+"){
						instructions.push_back(new Instruction("addi", to_string(val), regName));
					}
					else if (opcode == "*"){
						instructions.push_back(new Instruction("muli", to_string(val), regName));
					}
					else if (opcode == "-"){
						instructions.push_back(new Instruction("muli", to_string(-1), regName));
						instructions.push_back(new Instruction("addi", to_string(val), regName));

						if (constant_exp == exp2){
							instructions.push_back(new Instruction("muli", to_string(-1), regName));
						}
					}
					else if (opcode == "/"){
						if (constant_exp == exp1){
							instructions.push_back(new Instruction("divi", to_string(val), regName));
						}
						else {
							registers.pop();
							Register* top2 = registers.top();
							instructions.push_back(new Instruction("movei", to_string(val), top2->name));
							instructions.push_back(new Instruction("divi", regName, top2->name));
							registers.pop();
							registers.push(top);
							registers.push(top2);
						}
					}
					else if (opcode == "<" || opcode == ">" || opcode == "<=" 
						|| opcode == ">=" || opcode == "==" || opcode == "!="){

						registers.pop();
						Register* top2 = registers.top();


						instructions.push_back(new Instruction("movei", to_string(val), regName));
						if (constant_exp == exp1){
							instructions.push_back(new Instruction("compi", regName, top2->name));
							registers.pop();
							registers.push(top);
							registers.push(top2);
						}
						else {
							instructions.push_back(new Instruction("compi", top2->name, regName));
							registers.push(top);
						}

						if (fallthrough){
							Instruction* code = new Instruction(fallthroughinstr(opcode));
							instructions.push_back(code);
							falselist->instrList.push_back(code);
						}
						else{
							Instruction* code = new Instruction(notfallthroughinstr(opcode));
							instructions.push_back(code);
							truelist->instrList.push_back(code);
						}
					}

				}
				else { // Both expressions are not constant
					exp1->generate_code();
					Register* top = registers.top();

					instructions.push_back(new Instruction("pushi", top->name, ""));

					exp2->generate_code();
					top = registers.top();
					string reg2Name = top->name;
					registers.pop();
					Register* top2 = registers.top();
					string reg1Name = top2->name;

					instructions.push_back(new Instruction("loadi", "ind(esp)", reg1Name));

					if (opcode == "+"){
						instructions.push_back(new Instruction("addi", reg1Name, reg2Name));
					}
					else if (opcode == "-"){
						instructions.push_back(new Instruction("addi", reg2Name, reg1Name));
						instructions.push_back(new Instruction("movei", "-1", reg2Name));
						instructions.push_back(new Instruction("muli", reg1Name, reg2Name));
					}
					else if (opcode == "*"){
						instructions.push_back(new Instruction("muli", reg1Name, reg2Name));
					}
					else if (opcode == "/"){
						instructions.push_back(new Instruction("divi", reg1Name, reg2Name));
					}

					else if (opcode == "<" || opcode == ">" || opcode == "<=" 
						|| opcode == ">=" || opcode == "==" || opcode == "!="){

						instructions.push_back(new Instruction("cmpi", reg1Name, reg2Name));
			
						if (fallthrough){
							Instruction* code = new Instruction(fallthroughinstr(opcode));
							instructions.push_back(code);
							falselist->instrList.push_back(code);
						}
						else{
							Instruction* code = new Instruction(notfallthroughinstr(opcode));
							instructions.push_back(code);
							truelist->instrList.push_back(code);
						}
					}
					
					registers.push(top);

				}

			} // HERE ends op for integer
			else {
				//TODO :: for float with proper casting 
			}
		}
		
	}
}

void op::print()
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

void block_ast::print()
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

void identifier::generate_code(){
	if (type->base == 1){
    Register* top = registers.top();
    instructions.push_back(new Instruction("loadi", value, top->name));
	}
	else if (type->base == 2){
	    Register* top = registers.top();
	    instructions.push_back(new Instruction("loadf", value, top->name));  
	}
}


void return_stmt::generate_code(){
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

void if_stmt::generate_code(){
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

void while_stmt::generate_code(){
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

void for_stmt::generate_code(){
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

void index::generate_code_helper(){
	if (identifier* id = dynamic_cast<identifier*>(arr)){
		//LHS is an identifier
		swapRegisters();
		exp->generate_code();
		Register* top = registers.top();
		string name = top->name;
		instructions.push_back(new Instruction("muli", to_string(type->size), name));
		swapRegisters();

		top = registers.top();
		instructions.push_back(new Instruction("loadi", to_string(id->var->offset), top->name));

	}
	else {
		index* ind = dynamic_cast<index*> (arr);
		ind->generate_code_helper();
		Register* top = registers.top();
		instructions.push_back(new Instruction("pushi", top->name));

		swapRegisters();
		exp->generate_code();
		Register* top2 = registers.top();
		string top2Name = top2->name;
		swapRegisters();
		top = registers.top();
		registers.pop();
		string topName = top->name;
		instructions.push_back(new Instruction("loadi", "ind(esp)", topName));
		instructions.push_back(new Instruction("popi", "1"));

		int size = type->size;
		instructions.push_back(new Instruction("muli", to_string(size), top2Name));
		instructions.push_back(new Instruction("addi", top2Name, topName));
		registers.push(top);
			
	}
}

void index::generate_code(){
	generate_code_helper();
	Register* top = registers.top();

	if (type->base == 1){
		instructions.push_back(new Instruction("loadi", "ind(" + top->name + ")", top->name));
	}
	else {
		instructions.push_back(new Instruction("loadf", "ind(" + top->name + ")", top->name));
	}
}

void index::generate_code_for_lhs(){
	generate_code_helper();
	Register* top = registers.top();
	string name = top->name;
	//TODO: which instruction
}


void block_ast::generate_code(){
	int l = v.size();
	if (l > 0){
		v[0]->generate_code();

		for (int i = 1; i < l; i++){
			int M = instructions.size();
			v[i]->generate_code();
			(v[i - 1]->nextlist)->backpatch(instructions[M]);
		}

		nextlist = v[l - 1]->nextlist;
	}
}

void ass::generate_code(){
	op* temp = new op();
	temp->exp1 = exp1;
	temp->exp2 = exp2;
	temp->opcode = "=";
	temp->optype = "1";
	temp->generate_code();
}

/*
For ranveer: generate code for float constant, int constant and try for funcall
*/

void float_constant::generate_code(){
	Register* top = registers.top();
	instructions.push_back(new Instruction("loadf", to_string(value), top->name));
}

void int_constant::generate_code(){
	Register* top = registers.top();
	instructions.push_back(new Instruction("loadi", to_string(value), top->name));
}