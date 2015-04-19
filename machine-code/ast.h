#include <iostream>
#include <string>
#include <stack>
#include <vector>
using namespace std;

string nextLabel();

string fallthroughinstr(string);
string notfallthroughinstr(string);

class Register
{
public:
	string name;
	Register(string name){
		this->name;
	}
};

class Instruction{
public:
	string name, first, second;
	bool isLabel;
	bool isGoto;

	Instruction(string name, string first, string second){
		this->name = name;
		this->first = first;
		this->second = second;
		isLabel = false;
		isGoto = false;
	}

	Instruction(string name){
		this->name = name;
	}

	void setLabel(){
		if (isLabel) return;
		name = nextLabel();
		isLabel = true;
	}

	void backpatch(Instruction* instr){
		instr->setLabel();
		name = instr->name;
	}
};

class InstrList{
public:
	std::vector<Instruction*> instrList;

	void backpatch(Instruction* instr){
		for (int i=0; i< instrList.size(); i++){
			instrList[i]->backpatch(instr);
		}
	}

	void merge(InstrList* instList){
		this->instrList.insert(this->instrList.end(), instList->instrList.begin(), instList->instrList.end());
	}
};



/*
Extern Variable
*/

extern int labelNumber;
extern stack<Register*> registers;
extern vector<Instruction*> instructions;

