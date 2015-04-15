#include <iostream>
#include <string>
#include <map>
using namespace std;

extern int labelNumber;
extern map<string,string> fallThrough;

string nextLabel();

class Instruction
{
public:
	Instruction();
	~Instruction();
};