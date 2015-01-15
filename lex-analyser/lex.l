whitespaces 																[ \t\n]
identifier 																	[[:alpha:]_][[:alpha:][:digit:]_]*
type_specifier																void|int|float
condition_specifier															if|else
loop_specifier																for|while
bool_specifier																true|false
return_specifier															return
keywords																	{type_specifier}|{condition_specifier}|{loop_specifier}|{return_specifier}|{bool_specifier}
exponent-part 																[eE][+-][0-9]*|[eE][0-9]*
fractional-part 															[0-9]*
integer-part 																[0-9]*
floating-point-constant 													{integer-part}.{fractional-part}({exponent-part}?)
c-integer-constant 															[1-9][0-9]*|0[bB][01]*|0[0-7]*|0[xX][0-9a-fA-F]*
character-constant 															\'.\'
c-constant 																	{c-integer-constant}|{floating-point-constant}|{character-constant}
%%
{whitespaces}																//skip
{keywords}																	{std::cout << "Keyword " << matched() << std::endl;}
{identifier}																{std::cout << "Identifier " << matched() << std::endl;}
{c-constant}																{std::cout << "C-Constant " << matched() << std::endl;}
.																			{std::cout << "Others " << matched() << std::endl;};
