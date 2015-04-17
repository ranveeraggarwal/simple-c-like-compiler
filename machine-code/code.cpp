#include "ast.h"


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
