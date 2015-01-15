whitespaces 																[ \t\n]
identifier 																	[[:alpha:]_][[:alpha:][:digit:]_]*
type_specifier																VOID|INT|FLOAT
exponent-part 																[eE][+-][0-9]*|[eE][0-9]*
fractional-part 															[0-9]*
integer-part 																[0-9]*
floating-point-constant 													{integer-part}.{fractional-part}({exponent-part}?)
c-integer-constant 															[1-9][0-9]*|0[bB][01]*|0[0-7]*|0[xX][0-9a-fA-F]*
character-constant 															\'.\'
c-constant 																	{c-integer-constant}|{floating-point-constant}|{character-constant}
%%
{identifier}																{std::cout << "IDENTIFIER " << matched() << std::endl;}
{whitespaces}																{std::cout << "WHITESPACE" << matched() << std::endl;}
{c-constant}																{std::cout << "C-CONSTANT" << matched() << std::endl;}
