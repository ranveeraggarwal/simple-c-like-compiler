

// There may be some flexer directives here

%%
[ \t]                                    //skip
(push|mov|and|sub)l|call|leave|ret          return Parser::MNEMONIC;
(-?)[[:digit:]]+                            return Parser::NUMBER;
"$"(-?)[[:digit:]]+                         return Parser::IMMNUMBER;  
"%"(ebp|esp|edx|eax|esp)                    return Parser::REGISTER;     
[[:alpha:]][[:alnum:]]*                     return Parser::SYMBOL;
("."?)[[:alpha:]][[:alnum:]]*               return Parser::IMMSYMBOL;
"+"                                         return '+';
","                                         return ',';
"("                                         return '(';
")"                                         return ')';
":"                                         return ':';
"\n"                                        return '\n';
.                                           

