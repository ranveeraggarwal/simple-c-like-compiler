#include "ast.h"


int labelNumber = 0;

string nextLabel()
{
	string newLabel = "l" + to_string(labelNumber);
	labelNumber++;
	return newLabel;
}