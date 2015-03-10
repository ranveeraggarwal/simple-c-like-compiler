%debug
%scanner Scanner.h
%scanner-token-function d_scanner.lex()
%token id intconst floatconst 
%polymorphic idAttr: std::string; opAttr: char; typeAttr: Type;
%type <typeAttr> statement lhs term factor
%type <opAttr> unop
%type <idAttr> id

%%
statement 
       : lhs '=' term
       {
	 ($1).Type::printType();
	 ($3).Type::printType();

	 if (($1).tag == Type::Error ||  // error propagation
	     ($3).tag == Type::Error)
	   $$ = Type(Type::Error);

	 else if (compatible ($1, $3)) 
	   $$ = Type(Type::Ok);
	 else $$ = Type(Type::Error);

	     ($$).Type::printType();}
       ;

lhs    
       : unop lhs
         {  
	   ($2).Type::printType();

	   if (($2).tag == Type::Error) // error propagation
	     $$ = Type(Type::Error);

	   else if (($2).tag == Type::Pointer)
	     $$ = *($2).pointed;
	   else $$ = Type(Type::Error);
	   //
	   ($$).Type::printType();
          }

       | id
         {
	   // Add error condition here
           $$ = Symboltable[$1];
	   std::cout <<  $1 << std::endl;
	   //
	   ($$).Type::printType();
	 } 
       ;

term
       : term  '*' factor 
        {

	  ($1).Type::printType();
	  ($3).Type::printType();

	  if (($1).tag == Type::Error ||     // error propagation
	      ($3).tag == Type::Error)
	    $$ = Type(Type::Error);

          else if (($1).tag == Type::Pointer || 
		   ($3).tag == Type::Pointer)
            $$ = Type(Type::Error);
          else if (($1).basetype == Type::Int && 
                   ($3).basetype == Type::Int)
            $$ = Type(Type::Base, Type::Int);
          else $$ = Type(Type::Base, Type::Float);

	  ($$).Type::printType();

	}
       | factor
         {
	  ($1).Type::printType();

	  $$ = $1;  // includes error propagation

	  ($$).Type::printType();

         }
       ;

factor
       : intconst
         {
           $$= Type(Type::Base, Type::Int);
	  
	   ($$).Type::printType();
	 }
           
       | floatconst
         {
           $$= Type(Type::Base, Type::Float);
      
	   ($$).Type::printType();
	 }

       | '(' term ')'
         {
	   ($2).Type::printType();
           $$ = $2;   // includes error propagation 
 
	   ($$).Type::printType();
	 }

       | unop factor
         {
	   ($2).Type::printType();

           $$ = $2;   // includes error propagation
       
	   ($$).Type::printType();
	 }

       | id
         {
	   // Add error condition here
           $$ = Symboltable[$1];
	   std::cout <<  $1 << std::endl;
	   //
           ($$).Type::printType();
           }
       ;

unop   
       : '-'
         {
           $$ = '-';
	 }
                   
       | '*'
         {
           $$ = '*';}
       ; 
