/* Changes:  */

/* 1. Character constants removed */
/* 2. Changed INTCONSTANT to INT_CONSTANT */
/* 3. Changed the production for constant_expression to include FLOAT_CONSTANT */
/* 4. Added examples of FLOAT_CONSTANTS */
/* 5. Added the description of STRING_LITERAL */
/* 6. Changed primary_expression and FOR */
/* 7. The grammar permits a empty statement. This should be  */
/*    explicitly represented in the AST. */
/* 8. To avoid local decl inside blocks, a rule for statement  */
/*    has been changed. */

/* ----------------------------------------------------------------------- */

/* start symbol is translation_unit */

/* ---------------------------------------------------- */

//%debug
%scanner Scanner.h
%scanner-token-function d_scanner.lex()
%polymorphic ExpAst : expAst* ; StmAst : stmtAst*; Int : int; Float : float; String : string;

%type <ExpAst> expression logical_and_expression equality_expression relational_expression additive_expression multiplicative_expression unary_expression postfix_expression primary_expression l_expression constant_expression expression_list 
%type <StmAst> selection_statement iteration_statement assignment_statement translation_unit function_definition compound_statement statement statement_list
%type <Int> INT_CONSTANT
%type <Float> FLOAT_CONSTANT
%type <String> STRING_LITERAL IDENTIFIER unary_operator

%token NOT_OP ADD_OP SUB_OP MUL_OP DIV_OP LT_OP GL_OP LE_OP GE_OP EQ_OP NE_OP AND_OP OR_OP INC_OP VOID INT FLOAT RETURN IF ELSE WHILE FOR 

%%

translation_unit
	: function_definition 
	{
		$$ = $1;
		$$->print();
	}
	| translation_unit function_definition 
    ;

function_definition
	: type_specifier fun_declarator compound_statement 
		{
			$$ = $3;
		}
	;

type_specifier
	: VOID 	
    | INT   
	| FLOAT 
    ;

fun_declarator
	: IDENTIFIER '(' parameter_list ')' 
    | IDENTIFIER '(' ')' 
	;

parameter_list
	: parameter_declaration 
	| parameter_list ',' parameter_declaration 
	;

parameter_declaration
	: type_specifier declarator 
    ;

declarator
	: IDENTIFIER 
	| declarator '[' constant_expression ']' 
    ;

constant_expression 
    : INT_CONSTANT
    {
        ($$) = new int_constant();
        ((int_constant*)$$)->value = $1;

    }
    | FLOAT_CONSTANT 
    {
        ($$) = new float_constant();
        ((float_constant*)$$)->value = $1;
    }
    ;

compound_statement
	: '{' '}' 
		{
			$$ = new block_ast();
		}
	| '{' statement_list '}' 
		{
			$$ = $2;
		}
    | '{' declaration_list statement_list '}' 
    	{
    		$$ = $3;
    	}
	;

statement_list
	: statement		
		{
			stmtAst* temp = $1;
			$$ = new block_ast();
			((block_ast*)$$)->v.push_back(temp);
		}
    | statement_list statement	
    	{
            $$ = $1;
    		((block_ast*)$$)->v.push_back($2);
    	}
	;

statement
	: '{' statement_list '}'
	{
		$$ = $2;
	}
	| selection_statement 	
	{
		$$ = $1;
		
	}
	| iteration_statement 	
	{
		$$ = $1;
		
	}
	| assignment_statement	
	{
		$$ = $1;
		
	}
	| RETURN expression ';'	
	{
		$$ = new return_stmt();
		((return_stmt*)$$)->exp = ($<ExpAst>2);
		
	}
	;

assignment_statement
	: ';'
	{
		$$ = new ass();
		((ass*)$$)->exp1 = 0;
		((ass*)$$)->exp2 = 0;
	} 								
	|  l_expression '=' expression ';'
	{
		$$ = new ass();
		((ass*)$$)->exp1 = $1;
		((ass*)$$)->exp2 = $3;
	}	
	;

expression
    : logical_and_expression 
    {
    	($$) = ($1);
    }
    | expression OR_OP logical_and_expression
    {
        expAst* temp = $1;
    	($$) = new op();
    	((op*)$$)->exp1 = temp;
    	((op*)$$)->exp2 = ($<ExpAst>3);
    	((op*)$$)->opcode = "||";
    	((op*)$$)->optype = 1;
    }
	;

logical_and_expression
    : equality_expression
    {
    	($$) = ($1);
    }
    | logical_and_expression AND_OP equality_expression 
    {
        expAst* temp = $1;
    	($$) = new op();
    	((op*)$$)->exp1 = temp;
    	((op*)$$)->exp2 = ($<ExpAst>3);
    	((op*)$$)->opcode = "&&";
    	((op*)$$)->optype = 1;
    }
	;

equality_expression
	: relational_expression 
	{
    	($$) = ($1);
    }
    | equality_expression EQ_OP relational_expression 	
    {
        expAst* temp = $1;
    	($$) = new op();
    	((op*)$$)->exp1 = temp;
    	((op*)$$)->exp2 = ($<ExpAst>3);
    	((op*)$$)->opcode = "==";
    	((op*)$$)->optype = 1;
    }
	| equality_expression NE_OP relational_expression
	{
        expAst* temp = $1;
    	($$) = new op();
    	((op*)$$)->exp1 = temp;
    	((op*)$$)->exp2 = ($<ExpAst>3);
    	((op*)$$)->opcode = "!=";
    	((op*)$$)->optype = 1;
    }
	;

relational_expression
	: additive_expression
	{
    	($$) = ($1);
    }
    | relational_expression '<' additive_expression 
    {
        expAst* temp = $1;
    	($$) = new op();
    	((op*)$$)->exp1 = temp;
    	((op*)$$)->exp2 = ($<ExpAst>3);
    	((op*)$$)->opcode = "<";
    	((op*)$$)->optype = 1;
    }
	| relational_expression '>' additive_expression 
	{
        expAst* temp = $1;
    	($$) = new op();
    	((op*)$$)->exp1 = temp;
    	((op*)$$)->exp2 = ($<ExpAst>3);
    	((op*)$$)->opcode = ">";
    	((op*)$$)->optype = 1;
    }
	| relational_expression LE_OP additive_expression 
	{
        expAst* temp = $1;
    	($$) = new op();
    	((op*)$$)->exp1 = temp;
    	((op*)$$)->exp2 = ($<ExpAst>3);
    	((op*)$$)->opcode = "<=";
    	((op*)$$)->optype = 1;
    }
    | relational_expression GE_OP additive_expression 
    {
        expAst* temp = $1;
    	($$) = new op();
    	((op*)$$)->exp1 = temp;
    	((op*)$$)->exp2 = ($<ExpAst>3);
    	((op*)$$)->opcode = ">=";
    	((op*)$$)->optype = 1;
    }
	;

additive_expression 
	: multiplicative_expression
	{
    	($$) = ($1);
    }
	| additive_expression '+' multiplicative_expression
	{
        expAst* temp = $1;
    	($$) = new op();
    	((op*)$$)->exp1 = temp;
    	((op*)$$)->exp2 = ($<ExpAst>3);
    	((op*)$$)->opcode = "+";
    	((op*)$$)->optype = 1;
    } 
	| additive_expression '-' multiplicative_expression 
	{
        expAst* temp = $1;
    	($$) = new op();
    	((op*)$$)->exp1 = temp;
    	((op*)$$)->exp2 = ($<ExpAst>3);
    	((op*)$$)->opcode = "-";
    	((op*)$$)->optype = 1;
    }
	;

multiplicative_expression
	: unary_expression
	{
    	($$) = ($1);
    }
	| multiplicative_expression '*' unary_expression 
	{
        expAst* temp = $1;
    	($$) = new op();
    	((op*)$$)->exp1 = temp;
    	((op*)$$)->exp2 = ($<ExpAst>3);
    	((op*)$$)->opcode = "*";
    	((op*)$$)->optype = 1;
    }
	| multiplicative_expression '/' unary_expression 
	{
        expAst* temp = $1;
    	($$) = new op();
    	((op*)$$)->exp1 = temp;
    	((op*)$$)->exp2 = ($<ExpAst>3);
    	((op*)$$)->opcode = "/";
    	((op*)$$)->optype = 1;
    }
	;

unary_expression
	: postfix_expression  
	{
    	($$) = ($1);
    }				
	| unary_operator postfix_expression 
	{
        //String* temp = $1;
    	($$) = new op();
    	((op*)$$)->exp1 = ($2);
    	((op*)$$)->opcode = $<String>1;
    	((op*)$$)->optype = 0;
    }
	;

postfix_expression
	: primary_expression
	{
    	($$) = ($1);
    }
    | IDENTIFIER '(' ')'
	    {
	    	$$ = new fun_call();
	    	((fun_call*)$$)->fun_name = $1;
	    }
	| IDENTIFIER '(' expression_list ')' 
		{
			((fun_call*)$$)->fun_name = $1;
			$$=$3;
		}
	| l_expression INC_OP
	{
        expAst* temp = $1;
    	($$) = new op();
    	((op*)$$)->exp1 = temp;
    	((op*)$$)->opcode = "++";
    	((op*)$$)->optype = 0;
    }
	;

primary_expression
	: l_expression
	{
    	($$) = ($1);
    }
    | l_expression '=' expression // added this production
    {
        expAst* temp = $1;
    	($$) = new op();
    	((op*)$$)->exp1 = temp;
    	((op*)$$)->exp2 = ($<ExpAst>3);
    	((op*)$$)->opcode = "=";
    	((op*)$$)->optype = 1;
    }
	| INT_CONSTANT
	{
    	($$) = new int_constant();
    	((int_constant*)$$)->value = $1;
    }
	| FLOAT_CONSTANT
		{
    	($$) = new float_constant();
    	((float_constant*)$$)->value = $1;
    	}	
    | STRING_LITERAL
    {
    	($$) = new string_constant();
    	((string_constant*)$$)->value = $1;
    }
	| '(' expression ')'
	{
		$$ = $2;
	}	
	;

l_expression
    : IDENTIFIER
    {   
        $$ = new identifier();
    	((identifier*)$$)->value = $1;
    }
    | l_expression '[' expression ']'
    {
    	$$ = new index();
    	((index*)$$)->arr = ((arrayRef*)$<ExpAst>1);
    	((index*)$$)->exp = ($<ExpAst>3);
    }
    ;

expression_list
    : expression
    	{
    		expAst *temp = $1;
    		$$ = new fun_call();
    		((fun_call*)$$)->v.push_back(temp);
    	}
    | expression_list ',' expression
	    {
	    	$$ = $1;
	    	((fun_call*)$$)->v.push_back($3);
	    }
    ;

unary_operator
    : '-'
    	{
    		$$ = "-";
    	}	
	| '!'
		{
    		$$ = "!";
    	}	
	;

selection_statement
    : IF '(' expression ')' statement ELSE statement 
    {
    	($$) = new if_stmt();
    	((if_stmt*)$$)->exp = ($3);
    	((if_stmt*)$$)->stmt1 = ($5);
    	((if_stmt*)$$)->stmt2 = ($7);
    }
	;

iteration_statement
	: WHILE '(' expression ')' statement
	{
    	($$) = new while_stmt();
    	((while_stmt*)$$)->exp = ($3);
    	((while_stmt*)$$)->stmt = ($5);
    }
    | FOR '(' expression ';' expression ';' expression ')' statement
    {
        //cout<<"ok"<<$3<<" "<<endl;
    	($$) = new for_stmt();
    	((for_stmt*)$$)->exp1 = ($3);
    	((for_stmt*)$$)->exp2 = ($5); 
    	((for_stmt*)$$)->exp3 = ($7);
    	((for_stmt*)$$)->stmt = ($9);

    }
    ;

declaration_list
    : declaration  					
    | declaration_list declaration
	;

declaration
	: type_specifier declarator_list';'
	;

declarator_list
	: declarator
	| declarator_list ',' declarator 
	;