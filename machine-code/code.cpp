#include "ast.h"
#include <map>

map<string,string> fallThrough;
int labelNumber = 0;

string nextLabel()
{
	string newLabel = "l" + to_string(labelNumber);
	labelNumber++;
	return newLabel;
}