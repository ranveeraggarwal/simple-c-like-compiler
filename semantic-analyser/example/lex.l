// There may be some flexer directives here

%%
[ \t\n]                                    //skip
([+-]?([1-9][0-9]*)|(0[bB][01]+)|(0[0-7]*)|(0[xX][0-9a-fA-F]+)) {return Parser::intconst;}
([+-]?[0-9]+\.[0-9]*(([eE][+-][0-9]*)|([eE][0-9]*))?)        {return ParserBase::floatconst;}
([+-]?\.[0-9]+(([eE][+-][0-9]*)|([eE][0-9]*))?)              {return ParserBase::floatconst;}
([+-]?\.(([eE][+-][0-9]*)|([eE][0-9]*)))                     {return ParserBase::floatconst;}
[[:alpha:]][[:alnum:]]*                     { Parser::d_val__ = std::string(matched());
                                              return Parser::id;}
"="                                         return '=';
"*"                                         return '*';
"("                                         return '(';
")"                                         return ')';
"-"                                         return '-';
.                                           {std::cout << "Error" << std::endl;}

