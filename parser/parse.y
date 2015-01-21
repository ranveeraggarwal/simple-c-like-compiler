%debug
%scanner Scanner.h
%scanner-token-function d_scanner.lex()

%token MNEMONIC SYMBOL IMMSYMBOL NUMBER 
%token IMMNUMBER REGISTER
 


%%
program: 
       instruction_list
       ;

instruction_list: 
         instruction 
       | instruction_list '\n' instruction
       ;

instruction:
         optional_label MNEMONIC 
       | optional_label MNEMONIC opnd
       | optional_label MNEMONIC opnd ',' opnd
       ;

optional_label:
       | SYMBOL':' '\n'
       ;

opnd:
         register_exp
       | exp
       ;
       
register_exp:
         REGISTER
       | '(' REGISTER ')'
       | NUMBER '(' REGISTER ')'
       ;

exp:
         SYMBOL
       | IMMSYMBOL
       | NUMBER
       | IMMNUMBER
       | SYMBOL '+' NUMBER
       | IMMSYMBOL '+' NUMBER
       ;

