#include <map>

string typeArray[] = {"void", "int", "float"};
int scope = 0;
Type* type = NULL;
int offset = 0;
GlobalSymbolTable* gst = new GlobalSymbolTable();
LocalSymbolTable* currentLst = new LocalSymbolTable();
LocalSymbolTable* currentLstCalled = new LocalSymbolTable();
int paramCount = 0;
bool paramDone = false;

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
	//cerr<<"comp check"<<endl;
	//cerr<<"first: "<<first->base<<endl;
	//cerr<<"second: "<<second->base<<endl;
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
	registers.push(new Register("esi"));
	registers.push(new Register("edi"));
	registers.push(new Register("edx"));
	registers.push(new Register("ecx"));
	registers.push(new Register("ebx"));
	registers.push(new Register("eax"));
	
	//cerr<<"Init Regis:"<<registers.top()->name<<endl;
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
		this->name = name;
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
	isGoto = true;
	isLabel = false;
}

Instruction::Instruction(string name, string first){
	this->name = name;
	this->first = first;
	this->second = "";
	isLabel = false;
	isGoto = false;
}

void Instruction::setLabel(){
	if (isLabel) return;
	label = nextLabel();
	isLabel = true;
}

void Instruction::backpatch(Instruction* instr){
	instr->setLabel();
	first = instr->label;
}

void Instruction::print(){

	if (isReturn){
		cout<<"\treturn;"<<endl;
		return;
	}

	if (isFunctionStart){
		cout<<"void "<<name<<"() {"<<endl;
		return;
	}

	if (isFunctionEnd){
		cout<<"}\n"<<endl;
		return;
	}

	if (isLabel){
		cout<<label<<":"<<endl;
	}

	if (second == ""){
		cout<<"\t"<<name<<"("<<first<<");"<<endl;
	}
	else {
		cout<<"\t"<<name<<"("<<first<<","<<second<<");"<<endl;
	}
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


void op::generate_code()
{
	//cerr<<"opcode: "<<optype<<endl;
	if (optype == 0)
	{
		if (opcode == "++")
		{
			if (exp1->type->base == 1)
			{
				exp1->generate_code();
				Register* top = registers.top();
				instructions.push_back(new Instruction("addi", "1", top->name));
				if (identifier* temp = dynamic_cast<identifier*>(exp1)){
					int offset = temp->var->offset;
					instructions.push_back(new Instruction("storei", top->name, "ind(ebp,"+to_string(offset)+")"));
				}
				else {
					if(index* temp = dynamic_cast<index*>(exp1)){
						int offset = ((identifier*)temp->arr)->var->offset;
						instructions.push_back(new Instruction("storei", top->name, "ind(ebp,"+to_string(offset)+")"));
					}
				}
			}
			else if (exp1->type->base == 2)
			{
				exp1->generate_code();
				Register* top = registers.top();
				instructions.push_back(new Instruction("addf", "1", top->name));
				if (identifier* temp = dynamic_cast<identifier*>(exp1)){
					int offset = temp->var->offset;
					instructions.push_back(new Instruction("storef", top->name, "ind(ebp,"+to_string(offset)+")"));
				}
				else if(index* temp = dynamic_cast<index*>(exp1)){
					int offset = ((identifier*)temp->arr)->var->offset;
					instructions.push_back(new Instruction("storef", top->name, "ind(ebp,"+to_string(offset)+")"));
					
				}
			}
		}
		else if (opcode == "-")
		{
			if (exp1->type->base == 1)
			{
				exp1->generate_code();
				Register* top = registers.top();
				instructions.push_back(new Instruction("muli", "-1", top->name));
				if (identifier* temp = dynamic_cast<identifier*>(exp1)){
					int offset = temp->var->offset;
					instructions.push_back(new Instruction("storei", top->name, "ind(ebp,"+to_string(offset)+")"));
				}
				else {
					if(index* temp = dynamic_cast<index*>(exp1)){
						int offset = ((identifier*)temp->arr)->var->offset;
						instructions.push_back(new Instruction("storef", top->name, "ind(ebp,"+to_string(offset)+")"));
					}
				}
			}
			else if (exp1->type->base == 2)
			{
				exp1->generate_code();
				Register* top = registers.top();
				instructions.push_back(new Instruction("mulf", "-1", top->name));
				if (identifier* temp = dynamic_cast<identifier*>(exp1)){
					int offset = temp->var->offset;
					instructions.push_back(new Instruction("storef", top->name, "ind(ebp,"+to_string(offset)+")"));
				}
				else {
					if(index* temp = dynamic_cast<index*>(exp1)){
						int offset = ((identifier*)temp->arr)->var->offset;
						instructions.push_back(new Instruction("storef", top->name, "ind(ebp,"+to_string(offset)+")"));
					}
				}
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

		else if (opcode == "&&"){
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
			if ((exp1->type->base == 1 && exp2->type->base == 1) || (exp1->type->base == 2 && exp2->type->base == 2)){

				string instrType;
				if (exp1->type->base == 1) instrType = "i";
				else instrType = "f";

				bool isExp2Const = false;
				
				int_constant* tempI2i;
				float_constant* tempI2f;

				if (instrType == "i"){
					if (tempI2i = dynamic_cast<int_constant*>(exp2)) isExp2Const = true;
				}
				else {
					if (tempI2f = dynamic_cast<float_constant*>(exp2)) isExp2Const = true;
				}

				if (isExp2Const){



					string arg1;
					if (instrType == "i") arg1 = to_string(tempI2i->value);
					else arg1 = to_string(tempI2f->value);
					//cerr<<"Arg for array: "<<arg1<<endl;

					if(identifier* id = dynamic_cast<identifier*> (exp1)){
						// Left side is an identifier
						int offset = id->var->offset;
						Register* top = registers.top();
						////cerr<<"Register top:"<<top->name<<endl;
						instructions.push_back(new Instruction("move", arg1, top->name));
						instructions.push_back(new Instruction("store" + instrType, top->name, "ind(ebp,"+to_string(offset)+")"));

					}
					else {
						index* ind = dynamic_cast<index*> (exp1);
						ind->generate_code_for_lhs();
						//cerr<<"Here Arra"<<endl;
						Register* top = registers.top();
						
						string topName = top->name;
						instructions.push_back(new Instruction("store" + instrType, arg1, "ind("+topName+")"));
						instructions.push_back(new Instruction("move" , arg1, topName));
					}
				}
				else {

					if(identifier* id = dynamic_cast<identifier*> (exp1)){
						// Left side is an identifier
						exp2->generate_code();
						Register* top = registers.top();
						string topName = top->name;
						int offset = id->var->offset;
						instructions.push_back(new Instruction("store" + instrType, topName, "ind(ebp, " + to_string(offset) + ")"));

					}
					else {
						index* ind = dynamic_cast<index*> (exp1);
						exp2->generate_code();
						Register* top = registers.top();
						string topName = top->name;
						instructions.push_back(new Instruction("push" + instrType, topName));

						swapRegisters();
						ind->generate_code_for_lhs();
						swapRegisters();

						top = registers.top();
						topName = top->name;

						registers.pop();

						instructions.push_back(new Instruction("load" + instrType, "ind(esp)", topName));
						instructions.push_back(new Instruction("pop" + instrType, "1"));
						Register* top2 = registers.top();

						string top2Name = top2->name;

						instructions.push_back(new Instruction("store" + instrType, topName, "ind("+ top2Name + ")"));

						registers.push(top);
					}

				}
			}
			else {
				//cerr<<"HERE for '='"<<endl;

				//TODO for float with proper casting
				string instrType;
				if (exp1->type->base == 1) instrType = "i";
				else instrType = "f";

				//cerr << "HERE for '='"<<instrType<<endl;

				bool isExp2Const = false;
				
				int_constant* tempI2i;
				float_constant* tempI2f;
				string exp2Type;

				if (exp2->type->base == 2) exp2Type = "f";
				else exp2Type = "i";

				
				if (tempI2i = dynamic_cast<int_constant*>(exp2)){
					isExp2Const = true;
				}
				
				
				else if (tempI2f = dynamic_cast<float_constant*>(exp2)){
					isExp2Const = true;
				}
				

				if (isExp2Const){

					string arg1;
					if (exp2Type == "i") arg1 = to_string(tempI2i->value);
					else arg1 = to_string(tempI2f->value);


					if(identifier* id = dynamic_cast<identifier*> (exp1)){
						// Left side is an identifier
						int offset = id->var->offset;
						Register* top = registers.top();
						//cerr<<"Register top:"<<top->name<<endl;
						instructions.push_back(new Instruction("move", arg1, top->name));
						if (instrType == "f" && exp2Type == "i"){
							instructions.push_back(new Instruction("intTofloat", top->name));
						}
						else if (instrType == "i" && exp2Type == "f"){
							instructions.push_back(new Instruction("floatToint", top->name));
						}
						instructions.push_back(new Instruction("store" + instrType, top->name, "ind(ebp,"+to_string(offset)+")"));

					}
					else {
						index* ind = dynamic_cast<index*> (exp1);
						ind->generate_code_for_lhs();
						Register* top = registers.top();
						
						string topName = top->name;
						registers.pop();
						Register* top2 = registers.top();

						instructions.push_back(new Instruction("move", arg1, top2->name));

						if (instrType == "f" && exp2Type == "i"){
							instructions.push_back(new Instruction("intTofloat", top2->name));
						}
						else if (instrType == "i" && exp2Type == "f"){
							instructions.push_back(new Instruction("floatToint", top2->name));
						}

						instructions.push_back(new Instruction("store" + instrType, top2->name, "ind("+topName+")"));
						instructions.push_back(new Instruction("move" , top2->name, topName));
						registers.push(top);
					}
				}
				else {

					if(identifier* id = dynamic_cast<identifier*> (exp1)){
						// Left side is an identifier
						exp2->generate_code();
						Register* top = registers.top();
						string topName = top->name;
						int offset = id->var->offset;
						if (exp2->type->base == 2 && exp1->type->base == 1){
							instructions.push_back(new Instruction("floatToint", topName));
						}
						else if (exp2->type->base == 1 && exp1->type->base == 2){
							instructions.push_back(new Instruction("intTofloat", topName));
						}
						instructions.push_back(new Instruction("store" + instrType, topName, "ind(ebp, " + to_string(offset) + ")"));

					}
					else {
						index* ind = dynamic_cast<index*> (exp1);
						exp2->generate_code();
						Register* top = registers.top();
						string topName = top->name;

						if (exp2->type->base == 2 && exp1->type->base == 1){
							instructions.push_back(new Instruction("floatToint", topName));
						}
						else if (exp2->type->base == 1 && exp1->type->base == 2){
							instructions.push_back(new Instruction("intTofloat", topName));
						}

						instructions.push_back(new Instruction("push" + instrType, topName));

						swapRegisters();
						ind->generate_code_for_lhs();
						swapRegisters();

						top = registers.top();
						topName = top->name;

						registers.pop();

						instructions.push_back(new Instruction("load" + instrType, "ind(esp)", topName));
						instructions.push_back(new Instruction("pop" + instrType, "1"));
						Register* top2 = registers.top();

						string top2Name = top2->name;

						instructions.push_back(new Instruction("store" + instrType, topName, "ind("+ top2Name + ")"));

						registers.push(top);
					}

				}
			}
		}

		else{
			//cerr<<"opcode: "<<opcode<<endl;
			if ((exp1->type->base == 1 && exp2->type->base == 1) || (exp1->type->base == 2 && exp2->type->base == 2)){
				bool isExp1Const = false;
				string instrType;
				if (exp1->type->base == 1) instrType = "i";
				else instrType = "f";

				int_constant* tempI1;
				float_constant* tempI1f;

				if (instrType == "i"){
					if (tempI1 = dynamic_cast<int_constant*>(exp1)){
						isExp1Const = true;
					}
				}
				else {
					if (tempI1f = dynamic_cast<float_constant*>(exp1)){
						isExp1Const = true;
					}
				}

				bool isExp2Const = false;
				int_constant* tempI2;
				float_constant* tempI2f;

				if (instrType == "i"){
					if (tempI2 = dynamic_cast<int_constant*>(exp2)) isExp2Const = true;
				}
				else {
					if (tempI2f = dynamic_cast<float_constant*>(exp2)) isExp2Const = true;
				}

				//cerr<<"Exp1: "<<isExp1Const<<" Exp2: "<<isExp2Const<<endl;

				if (isExp1Const && isExp2Const){
					string val1, val2;
					if (instrType == "i"){
						val1 = to_string(tempI1->value);
						val2 = to_string(tempI2->value);
					}
					else {
						val1 = to_string(tempI1f->value);
						val2 = to_string(tempI2f->value);
					}

					Register* top = registers.top();
					if (opcode == "+"){
						instructions.push_back(new Instruction("move", val1, top->name));
						instructions.push_back(new Instruction("add" + instrType, val2, top->name));
					}

					else if (opcode == "-"){
						val2 = "-" + val2;
						instructions.push_back(new Instruction("move", val1, top->name));
						instructions.push_back(new Instruction("add" + instrType, val2, top->name));
					}

					else if (opcode == "*"){
						instructions.push_back(new Instruction("move", val1, top->name));
						instructions.push_back(new Instruction("mul" + instrType, val2, top->name));
					}

					else if (opcode == "/"){
						instructions.push_back(new Instruction("move" , val1, top->name));
						instructions.push_back(new Instruction("div" + instrType, val2, top->name));
					}

					else if (opcode == "<" || opcode == ">" || opcode == "<=" 
						|| opcode == ">=" || opcode == "==" || opcode == "!="){
						instructions.push_back(new Instruction("move", val1, top->name));
						instructions.push_back(new Instruction("cmp" + instrType, val2, top->name));

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
					//cerr<<"code generated for nonconst"<<endl;
					string val;
					if(instrType == "i") val = to_string(((int_constant*)constant_exp)->value);
					else val = to_string(((float_constant*)constant_exp)->value);
					//cerr<<"Constant Value: "<<val<<endl;
					Register* top = registers.top();
					string regName = top->name;
					if (opcode == "+"){
						instructions.push_back(new Instruction("add" + instrType, val, regName));
					}
					else if (opcode == "*"){
						instructions.push_back(new Instruction("mul" + instrType, val, regName));
					}
					else if (opcode == "-"){
						instructions.push_back(new Instruction("mul" + instrType, "-1", regName));
						instructions.push_back(new Instruction("add" + instrType, val, regName));

						if (constant_exp == exp2){
							instructions.push_back(new Instruction("mul" + instrType, "-1", regName));
						}
					}
					else if (opcode == "/"){
						if (constant_exp == exp1){
							instructions.push_back(new Instruction("div" + instrType, val, regName));
						}
						else {
							registers.pop();
							Register* top2 = registers.top();
							instructions.push_back(new Instruction("move", val, top2->name));
							instructions.push_back(new Instruction("div" + instrType, regName, top2->name));
							registers.pop();
							registers.push(top);
							registers.push(top2);
						}
					}
					else if (opcode == "<" || opcode == ">" || opcode == "<=" 
						|| opcode == ">=" || opcode == "==" || opcode == "!="){

						registers.pop();
						Register* top2 = registers.top();


						instructions.push_back(new Instruction("move", val, top2->name));

						//cerr<<"Inside <"<<endl;
						if (constant_exp == exp1){
							instructions.push_back(new Instruction("cmp" + instrType, regName, top2->name));
							registers.pop();
							registers.push(top);
							registers.push(top2);
						}
						else {
							//cerr<<"coming inside exp2cnst"<<endl;
							instructions.push_back(new Instruction("cmp" + instrType, regName, top2->name));
							registers.push(top);
						}

						//cerr<<"fallthrough: "<<fallthrough<<endl;
						if (fallthrough){
							Instruction* code = new Instruction(fallthroughinstr(opcode));
							//cerr<<code->name<<endl;
							instructions.push_back(code);
							falselist->instrList.push_back(code);
							//cerr<<"Ab kaha hai?"<<endl;
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

					instructions.push_back(new Instruction("push" + instrType, top->name, ""));

					exp2->generate_code();
					top = registers.top();
					string reg2Name = top->name;
					registers.pop();
					Register* top2 = registers.top();
					string reg1Name = top2->name;

					instructions.push_back(new Instruction("load" + instrType, "ind(esp)", reg1Name));
					instructions.push_back(new Instruction("pop" + instrType, "1"));

					if (opcode == "+"){
						instructions.push_back(new Instruction("add" + instrType, reg1Name, reg2Name));
					}
					else if (opcode == "-"){
						instructions.push_back(new Instruction("add" + instrType, reg2Name, reg1Name));
						instructions.push_back(new Instruction("move", "-1", reg2Name));
						instructions.push_back(new Instruction("mul" + instrType, reg1Name, reg2Name));
					}
					else if (opcode == "*"){
						instructions.push_back(new Instruction("mul" + instrType, reg1Name, reg2Name));
					}
					else if (opcode == "/"){
						instructions.push_back(new Instruction("div" + instrType, reg1Name, reg2Name));
					}

					else if (opcode == "<" || opcode == ">" || opcode == "<=" 
						|| opcode == ">=" || opcode == "==" || opcode == "!="){

						instructions.push_back(new Instruction("cmp" + instrType, reg1Name, reg2Name));
			
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
			//Doing for INT OP FLOAT and FLOAT OP INT
			else {
				bool isExp1Int;
				if (exp1->type->base == 1) isExp1Int = 1;
				else isExp1Int = 0;

				bool isExp1Const = false;

				bool isExp2Const = false;

				int_constant* intC;
				float_constant* floatC;
				if (isExp1Int){
					if (intC = dynamic_cast<int_constant*>(exp1)){
						isExp1Const = true;
					}
					if (floatC = dynamic_cast<float_constant*>(exp2)){
						isExp2Const = true;
					}
				}
				else {
					if (intC = dynamic_cast<int_constant*>(exp2)){
						isExp2Const = true;
					}
					if (floatC = dynamic_cast<float_constant*>(exp1)){
						isExp1Const = true;
					}
				}

				if (isExp1Const && isExp2Const){
					string val1, val2;
					if (isExp1Int){
						val1 = to_string(intC->value);
						val2 = to_string(floatC->value);
					}
					else {
						val1 = to_string(floatC->value);
						val2 = to_string(intC->value);
					}

					Register* top = registers.top();
					registers.pop();
					Register* top2 = registers.top();
					if (opcode == "+"){
						instructions.push_back(new Instruction("move", val1, top->name));
						if (isExp1Int) {
							instructions.push_back(new Instruction("intTofloat", top->name));
							instructions.push_back(new Instruction("addf", val2, top->name));
						}
						else {
							instructions.push_back(new Instruction("move", val2, top2->name));
							instructions.push_back(new Instruction("intTofloat", top2->name));
							instructions.push_back(new Instruction("addf", top2->name, top->name));
						}
					}

					else if (opcode == "-"){
						val2 = "-" + val2;
						instructions.push_back(new Instruction("move", val1, top->name));
						if (isExp1Int) {
							instructions.push_back(new Instruction("intTofloat", top->name));
							instructions.push_back(new Instruction("addf", val2, top->name));
						}
						else {
							instructions.push_back(new Instruction("move", val2, top2->name));
							instructions.push_back(new Instruction("intTofloat", top2->name));
							instructions.push_back(new Instruction("addf", top2->name, top->name));
						}
					}

					else if (opcode == "*"){
						instructions.push_back(new Instruction("move", val1, top->name));
						if (isExp1Int) {
							instructions.push_back(new Instruction("intTofloat", top->name));
							instructions.push_back(new Instruction("addf", val2, top->name));
						}
						else {
							instructions.push_back(new Instruction("move", val2, top2->name));
							instructions.push_back(new Instruction("intTofloat", top2->name));
							instructions.push_back(new Instruction("addf", top2->name, top->name));
						}
					}

					else if (opcode == "/"){
						instructions.push_back(new Instruction("move", val1, top->name));
						if (isExp1Int) {
							instructions.push_back(new Instruction("intTofloat", top->name));
							instructions.push_back(new Instruction("addf", val2, top->name));
						}
						else {
							instructions.push_back(new Instruction("move", val2, top2->name));
							instructions.push_back(new Instruction("intTofloat", top2->name));
							instructions.push_back(new Instruction("addf", top2->name, top->name));
						}
					}

					else if (opcode == "<" || opcode == ">" || opcode == "<=" 
						|| opcode == ">=" || opcode == "==" || opcode == "!="){
						instructions.push_back(new Instruction("move", val1, top->name));
						if (isExp1Int){
							instructions.push_back(new Instruction("intTofloat", top->name));
							instructions.push_back(new Instruction("cmpf", val2, top->name));
						}
						else {
							instructions.push_back(new Instruction("move", val2, top2->name));
							instructions.push_back(new Instruction("intTofloat", top2->name));
							instructions.push_back(new Instruction("cmpf", top2->name, top->name));
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
					registers.push(top);
				}
				else if (isExp2Const || isExp1Const){
					expAst* constant_exp;
					expAst* nonconstant_exp;

					bool isConstExpInt;
					if (isExp1Const){
						if (isExp1Int) isConstExpInt = true;
						else isConstExpInt = false;
						constant_exp = exp1;
						nonconstant_exp = exp2;
					}
					else {
						if (isExp1Int) isConstExpInt = false;
						else isConstExpInt = true;
						constant_exp = exp2;
						nonconstant_exp = exp1;
					}
					nonconstant_exp->generate_code();
					string val;
					if(isConstExpInt) val = to_string(((int_constant*)constant_exp)->value);
					else val = to_string(((float_constant*)constant_exp)->value);
					Register* top = registers.top();
					string regName = top->name;

					if (!isConstExpInt){
						instructions.push_back(new Instruction("intTofloat", regName));
					}

					if (opcode == "+"){
						if (isConstExpInt){
							registers.pop();
							Register* top2 = registers.top();
							instructions.push_back(new Instruction("move", val, top2->name));
							instructions.push_back(new Instruction("intTofloat", top2->name));
							instructions.push_back(new Instruction("addf", top2->name, regName));
							registers.push(top);
						}
						else instructions.push_back(new Instruction("addf", val, regName));
					}
					else if (opcode == "*"){
						if (isConstExpInt){
							registers.pop();
							Register* top2 = registers.top();
							instructions.push_back(new Instruction("move", val, top2->name));
							instructions.push_back(new Instruction("intTofloat", top2->name));
							instructions.push_back(new Instruction("mulf", top2->name, regName));
							registers.push(top);
						}
						else instructions.push_back(new Instruction("mulf", val, regName));
					}
					else if (opcode == "-"){
						instructions.push_back(new Instruction("mulf", "-1", regName));
						if (isConstExpInt){
							registers.pop();
							Register* top2 = registers.top();
							instructions.push_back(new Instruction("move", val, top2->name));
							instructions.push_back(new Instruction("intTofloat", top2->name));
							instructions.push_back(new Instruction("addf", top2->name, regName));
							registers.push(top);
						}
						else instructions.push_back(new Instruction("addf", val, regName));

						if (constant_exp == exp2){
							instructions.push_back(new Instruction("mulf", "-1", regName));
						}
					}
					else if (opcode == "/"){
						if (constant_exp == exp1){
							if (isConstExpInt){
								registers.pop();
								Register* top2 = registers.top();
								instructions.push_back(new Instruction("move", val, top2->name));
								instructions.push_back(new Instruction("intTofloat", top2->name));
								instructions.push_back(new Instruction("divf", top2->name, regName));
								registers.push(top);
							}
							else instructions.push_back(new Instruction("divf", val, regName));
						}
						else {
							registers.pop();
							Register* top2 = registers.top();
							instructions.push_back(new Instruction("move", val, top2->name));
							if (isConstExpInt) instructions.push_back(new Instruction("intTofloat", top2->name));
							instructions.push_back(new Instruction("divf", regName, top2->name));
							registers.pop();
							registers.push(top);
							registers.push(top2);
						}
					}
					else if (opcode == "<" || opcode == ">" || opcode == "<=" 
						|| opcode == ">=" || opcode == "==" || opcode == "!="){

						registers.pop();
						Register* top2 = registers.top();


						instructions.push_back(new Instruction("move", val, regName));
						if (isConstExpInt) instructions.push_back(new Instruction("intTofloat", regName));
						if (constant_exp == exp1){
							instructions.push_back(new Instruction("cmpf", regName, top2->name));
							registers.pop();
							registers.push(top);
							registers.push(top2);
						}
						else {
							instructions.push_back(new Instruction("cmpf", top2->name, regName));
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

					if (isExp1Int) {
						instructions.push_back(new Instruction("intTofloat", top->name));
					}
					instructions.push_back(new Instruction("pushf", top->name));

					exp2->generate_code();
					top = registers.top();
					string reg2Name = top->name;
					if (!isExp1Int) instructions.push_back(new Instruction("intTofloat", reg2Name));
					registers.pop();
					Register* top2 = registers.top();
					string reg1Name = top2->name;

					instructions.push_back(new Instruction("loadf", "ind(esp)", reg1Name));

					if (opcode == "+"){
						instructions.push_back(new Instruction("addf", reg1Name, reg2Name));
					}
					else if (opcode == "-"){
						instructions.push_back(new Instruction("addf", reg2Name, reg1Name));
						instructions.push_back(new Instruction("move", "-1", reg2Name));
						instructions.push_back(new Instruction("mulf", reg1Name, reg2Name));
					}
					else if (opcode == "*"){
						instructions.push_back(new Instruction("mulf", reg1Name, reg2Name));
					}
					else if (opcode == "/"){
						instructions.push_back(new Instruction("divf", reg1Name, reg2Name));
					}

					else if (opcode == "<" || opcode == ">" || opcode == "<=" 
						|| opcode == ">=" || opcode == "==" || opcode == "!="){

						instructions.push_back(new Instruction("cmpf", reg1Name, reg2Name));
			
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

			} // HERE ends op for int op float
			
		}
		
	}
}

void op::print()
{
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
    //cerr<<"identifier: "<<var->offset<<endl;
    instructions.push_back(new Instruction("loadi", "ind(ebp," + to_string(var->offset) + ")", top->name));
	}
	else if (type->base == 2){
	    Register* top = registers.top();
	    instructions.push_back(new Instruction("loadf", "ind(ebp," + to_string(var->offset) + ")", top->name));  
	}
}


void return_stmt::generate_code(){
	Type* returnType = lst->returnType;
	string instrType;
	if (returnType->base == 1) instrType = "i";
	else instrType = "f";

	if (exp->type->base == 1) { //expression is of type int
		if (int_constant* i = dynamic_cast<int_constant*>(exp)){
			int temp = ((int_constant*)exp)->value;
			if (instrType == "f"){
				Register* top = registers.top();
				instructions.push_back(new Instruction("move", to_string(temp), top->name));
				instructions.push_back(new Instruction("intTofloat", top->name));
				instructions.push_back(new Instruction("storef", top->name, "ind(ebp,"+to_string(offset) + ")"));
			}
			else instructions.push_back(new Instruction("storei", to_string(temp), "ind(ebp," + to_string(offset) + ")"));

		}
		else {
			exp->generate_code();
		    Register* reg = registers.top();
		    if (instrType == "f") instructions.push_back(new Instruction("intTofloat", reg->name));
		    instructions.push_back(new Instruction("store" + instrType, reg->name, "ind(ebp, "  + to_string(offset) + ")"));
		}
	}
	else { //float
		if (float_constant* i = dynamic_cast<float_constant*>(exp)){
			float temp = ((float_constant*)exp)->value;
			if (instrType == "i"){
				Register* top = registers.top();
				instructions.push_back(new Instruction("move", to_string(temp), top->name));
				instructions.push_back(new Instruction("floatToint", top->name));
				instructions.push_back(new Instruction("storei", top->name, "ind(ebp,"+to_string(offset) + ")"));
			}
			else instructions.push_back(new Instruction("storef", to_string(temp), "ind(ebp," + to_string(offset) + ")"));
		}
		else {
			exp->generate_code();
		    Register* reg = registers.top();
		    if (instrType == "i") instructions.push_back(new Instruction("floatToint", reg->name));
		    instructions.push_back(new Instruction("store" + instrType, reg->name, "ind(ebp, "  + to_string(offset) + ")"));
		}
	}

	for (auto kv: lst->variables){
		if (kv.second->scope == 2) continue;
		instructions.push_back(new Instruction("addi", to_string(kv.second->size), "esp"));
	}
	instructions.push_back(new Instruction("popi", "1"));

	Instruction* returnInstr = new Instruction();
	returnInstr->name = "return";
	returnInstr->isReturn = true;
	instructions.push_back(returnInstr);

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
	//cerr<<"exp2 generated"<<endl;
	int stmtM = instructions.size();
	//cerr<<"stmt code generation called"<<endl;
	//stmt->print();
	////cerr<<"stmt printed"<<endl;
	////cerr<<stmt<<endl;
	stmt->generate_code();
	//cerr<<"stmt code generated"<<endl;
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
		instructions.push_back(new Instruction("loadi", "ind(ebp,"+to_string(id->var->offset)+")", top->name));

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
	//cerr<<"For Loop yaha aya"<<endl;
	if (isFunction){
		Instruction* funcStart = new Instruction();
		funcStart->name = lst->funcName;
		funcStart->isFunctionStart = true;
		instructions.push_back(funcStart);

		instructions.push_back(new Instruction("pushi", "ebp"));
		instructions.push_back(new Instruction("move", "esp", "ebp"));

		for (auto kv: lst->variables){
			if (kv.second->scope == 2) continue;
			instructions.push_back(new Instruction("addi", "-" + to_string(kv.second->size), "esp"));
		}
	}

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

	if (isFunction){

		//instructions.push_back(new Instruction("popi", "1"));

		Instruction* funEnd = new Instruction();
		funEnd->isFunctionEnd = true;
		instructions.push_back(funEnd);
	}
}

void ass::generate_code(){
	//cerr<<"for ass"<<endl;
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
	instructions.push_back(new Instruction("move", to_string(value), top->name));
}

void int_constant::generate_code(){
	Register* top = registers.top();
	instructions.push_back(new Instruction("move", to_string(value), top->name));
}

void  fun_call::generate_code(){
	if (fun_name == "printf"){
		instructions.push_back(new Instruction("pushi", "0"));
		for (auto exp: expList->v){
			if (int_constant* temp = dynamic_cast<int_constant*>(exp)){
				instructions.push_back(new Instruction("print_int", to_string(temp->value)));
			}
			else if (float_constant* temp = dynamic_cast<float_constant*>(exp)){
				instructions.push_back(new Instruction("print_float", to_string(temp->value)));
			}
			else if (string_constant* temp = dynamic_cast<string_constant*>(exp)){
				instructions.push_back(new Instruction("print_string", temp->value));
			}
			else{
				exp->generate_code();
				Register* top = registers.top();
				Type* t = exp->type;
				if (t->base == 1){
					instructions.push_back(new Instruction("print_int", top->name));

				}
				else {
					instructions.push_back(new Instruction("print_float", top->name));
				}
			}
		}
		Register* top = registers.top();
		instructions.push_back(new Instruction("loadi", "ind(esp)", top->name));
		instructions.push_back(new Instruction("popi", "1"));
	}
	else {
		LocalSymbolTable* lst = gst->getLst(fun_name);
		Type* returnType = lst->returnType;
		if (returnType->base == 1){
			instructions.push_back(new Instruction("pushi", "0"));
		}
		else if (returnType->base == 2){
			instructions.push_back(new Instruction("pushf", "0"));
		}

		for (int i=0; i< expList->v.size(); i++){
			Type* t = expList->v[i]->type;
			Type* t2 = lst->params[i]->type;
			if (t->base == 1){
				if (t2->base == 1){
					if (int_constant* temp = dynamic_cast<int_constant*>(expList->v[i])){
						instructions.push_back(new Instruction("pushi", to_string(temp->value)));
					}
					else {
						expList->v[i]->generate_code();
						Register* top = registers.top();
						instructions.push_back(new Instruction("pushi", top->name));
					}
				}
				else {
					if (int_constant* temp = dynamic_cast<int_constant*>(expList->v[i])){
						Register* top = registers.top();
						instructions.push_back(new Instruction("move", to_string(temp->value), top->name));
						instructions.push_back(new Instruction("intTofloat", top->name));
						instructions.push_back(new Instruction("pushi", top->name));
					}
					else {
						expList->v[i]->generate_code();
						Register* top = registers.top();
						instructions.push_back(new Instruction("intTofloat", top->name));
						instructions.push_back(new Instruction("pushi", top->name));
					}
				}
			}
			else {
				if (t2->base == 2){
					if (float_constant* temp = dynamic_cast<float_constant*>(expList->v[i])){
						instructions.push_back(new Instruction("pushf", to_string(temp->value)));
					}
					else {
						expList->v[i]->generate_code();
						Register* top = registers.top();
						instructions.push_back(new Instruction("pushf", top->name));
					}
				}
				else {
					if (float_constant* temp = dynamic_cast<float_constant*>(expList->v[i])){
						Register* top = registers.top();
						instructions.push_back(new Instruction("move", to_string(temp->value), top->name));
						instructions.push_back(new Instruction("floatToint", top->name));
						instructions.push_back(new Instruction("pushf", top->name));
					}
					else {
						expList->v[i]->generate_code();
						Register* top = registers.top();
						instructions.push_back(new Instruction("floatToint", top->name));
						instructions.push_back(new Instruction("pushf", top->name));
					}
				}
			}
		}

		instructions.push_back(new Instruction(fun_name, ""));

		for (auto var: lst->params){
			if (var->type->base == 1){
				instructions.push_back(new Instruction("popi", "1"));
			}
			else {
				instructions.push_back(new Instruction("popf", "1"));
			}
		}

		Register* top = registers.top();

		if (returnType->base == 1){
			instructions.push_back(new Instruction("loadi", "ind(esp)", top->name));
			instructions.push_back(new Instruction("popi", "1"));
		}
		else {
			instructions.push_back(new Instruction("loadf", "ind(esp)", top->name));
			instructions.push_back(new Instruction("popf", "1"));
		}
	}
}

void fun_call_stmt::generate_code(){
	fun_call* temp = new fun_call();
	temp->fun_name = fun_name;
	temp->expList = expList;
	temp->generate_code();
}