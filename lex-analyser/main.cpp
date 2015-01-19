#include <iostream>
#include "Scanner.h"
using namespace std;
int main()
{
	Scanner scanner;   // define a Scanner object
	while (int token = scanner.lex()) // get all tokens
	{
		string const &text = scanner.matched();
		switch (token)
		{
			case Scanner::WHITESPACE:
				break;
			case Scanner::IDENTIFIER:
				cout << "IDENTIFIER: " << text << '\n';
				break;
			case Scanner::NUMBER:
				cout << "NUMBER: " << text << '\n';
				break;
			case Scanner::NOT_OP:
				cout << "NOT_OP: " << text << '\n';
				break;
			case Scanner::ASS_OP:
				cout << "ASS_OP: " << text << '\n';
				break;
			case Scanner::ADD_OP:
				cout << "ADD_OP: " << text << '\n';
				break;
			case Scanner::SUB_OP:
				cout << "SUB_OP: " << text << '\n';
				break;
			case Scanner::MUL_OP:
				cout << "MUL_OP: " << text << '\n';
				break;
			case Scanner::DIV_OP:
				cout << "DIV_OP: " << text << '\n';
				break;
			case Scanner::LT_OP:
				cout << "LT_OP: " << text << '\n';
				break;
			case Scanner::GL_OP:
				cout << "GL_OP: " << text << '\n';
				break;
			case Scanner::LE_OP:
				cout << "LE_OP: " << text << '\n';
				break;
			case Scanner::GE_OP:
				cout << "GE_OP: " << text << '\n';
				break;
			case Scanner::EQ_OP:
				cout << "EQ_OP: " << text << '\n';
				break;
			case Scanner::NE_OP:
				cout << "NE_OP: " << text << '\n';
				break;
			case Scanner::AND_OP:
				cout << "AND_OP: " << text << '\n';
				break;
			case Scanner::OR_OP:
				cout << "OR_OP: " << text << '\n';
				break;
			case Scanner::INC_OP:
				cout << "INC_OP: " << text << '\n';
				break;
			case Scanner::STRING_LITERAL:
				cout << "STRING_LITERAL: " << text << '\n';
				break;
			case Scanner::VOID:
				cout << "VOID: " << text << '\n';
				break;
			case Scanner::INT:
				cout << "INT: " << text << '\n';
				break;
			case Scanner::FLOAT:
				cout << "FLOAT: " << text << '\n';
				break;
			case Scanner::RETURN:
				cout << "RETURN: " << text << '\n';
				break;
			case Scanner::IF:
				cout << "IF: " << text << '\n';
				break;
			case Scanner::ELSE:
				cout << "ELSE: " << text << '\n';
				break;
			case Scanner::WHILE:
				cout << "WHILE: " << text << '\n';
				break;
			case Scanner::FOR:
				cout << "FOR: " << text << '\n';
				break;
			case Scanner::INT_CONSTANT:
				cout << "INT_CONSTANT: " << text << '\n';
				break;
			case Scanner::FLOAT_CONSTANT:
				cout << "FLOAT_CONSTANT: " << text << '\n';
				break;
			default:
				cout << "char. token: '" << text << "'\n";
		}
	}
	return 0;
}
