#include <iostream>
#include "Scanner.h"
using namespace std;

int main(int argc, char const *argv[])
{
        Scanner scanner;
        while(scanner.lex());
        return 0;
}

