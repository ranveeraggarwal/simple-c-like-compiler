#include <iostream>
#include "Scanner.h"
using namespace std;
int main()
{
  Scanner scanner;   // define a Scanner object
  while(scanner.lex());
  /*while (int token = scanner.lex()) // get all tokens
    {
      string const &text = scanner.matched();
      switch (token)
	{
	case Scanner::IDENTIFIER:
	  cout << "identifier: " << text << '\n';
	  break;
	case Scanner::NUMBER:
	  cout << "number: " << text << '\n';
	  break;
	default:
	  cout << "char. token: `" << text << "'\n";
	}
    }
   */
    return 0;
}
