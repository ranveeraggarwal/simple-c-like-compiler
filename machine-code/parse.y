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
%polymorphic exp_list: exp_list*; ExpAst : expAst* ; StmAst : stmtAst*; Int : int; Float : float; String : string; Variable : Variable*; Type : Type*;
%lsp-needed

%type <ExpAst> expression logical_and_expression equality_expression relational_expression additive_expression multiplicative_expression unary_expression postfix_expression primary_expression l_expression constant_expression 
%type <StmAst> selection_statement iteration_statement assignment_statement translation_unit function_definition compound_statement statement statement_list
%type <Int> INT_CONSTANT
%type <Float> FLOAT_CONSTANT
%type <String> STRING_LITERAL IDENTIFIER unary_operator
%type <Type> type_specifier
%type <Variable> parameter_declaration declarator
%type <exp_list> expression_list

%token NOT_OP ADD_OP SUB_OP MUL_OP DIV_OP LT_OP GL_OP LE_OP GE_OP EQ_OP NE_OP AND_OP OR_OP INC_OP VOID INT FLOAT RETURN IF ELSE WHILE FOR 

%%

program
    : translation_unit
    {
        gst->print();
    }
    ;

translation_unit
	: function_definition 
	{
		$$ = $1;
		$$->print();
        cout<<endl;
	}
	| translation_unit function_definition 
    {
        $$ = $2;
        $$->print();
        cout<<endl;
    }
    ;

function_definition
	: type_specifier fun_declarator compound_statement 
		{
			$$ = $3;
            scope = 0;
		}
	;

type_specifier
	: VOID 	
    {
        $$ = new Type(0);
        type = $$;
    }
    | INT  
    {
        $$ = new Type(1);
        type = $$;
    } 
	| FLOAT 
    {
        $$ = new Type(2);
        type = $$;
    }
    ;

fun_declarator
	: IDENTIFIER '('
    {
        currentLst = new LocalSymbolTable($1);
        currentLst->returnType = type;
        gst->lstList.push_back(currentLst);
        offset = 0;
        scope = 2;
    } 
    parameter_list ')' 
    {
        scope = 1;
    }
    | IDENTIFIER '('  ')'
    {
        currentLst = new LocalSymbolTable($1);
        currentLst->returnType = type;
        gst->lstList.push_back(currentLst);
        offset = 0;
        scope = 1;
    } 
	;

parameter_list
	: parameter_declaration 
    {
        scope = 2;
    }
	| parameter_list ',' parameter_declaration 
    {
        scope = 2;
    }
	;

parameter_declaration
	: type_specifier declarator
    {
        unordered_map<string, Variable*>::iterator it= currentLst->variables.find($2->varname);
        if (it != currentLst->variables.end()){
            cout<<"error: Variable "<<$2->varname<<" in parameter Already defined in function "
            <<currentLst->funcName<<" at line number "<<lineCount<<endl;
            exit(0);
        }
        else{
            currentLst->variables[$2->varname] = $2;
            currentLst->params.push_back($2);
        }
    }
    ;

declarator
	: IDENTIFIER 
    {
        $$ = new Variable();
        if (type->base == 0){
            cout<<"Variable or field '"<<$1<<"' declared void at line number "<<lineCount<<endl;
            exit(0);
        }
        $$->varname = $1;
        $$->scope = scope;
        $$->type = type;
        $$->size = type->size;
        $$->offset = offset;
        offset += $$->size;
    
    }
    
	| declarator '[' constant_expression ']'
    {
        $$ = $1;
        Type *temp =  new Type($1->type,((int_constant*)$3)->value);
        $$->type = temp;
        offset -= $$->size;
        $$->size *=  temp->size;
        offset += $$->size;
    }
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
        Type* tempType = extended_compatibility_check($2->type, currentLst->returnType);
        if (tempType == 0){
            cout<<"return type of function "<<currentLst->funcName<<" is not same as return statement type at line "
            <<lineCount<<endl;
            exit(0);
        }
        if (currentLst->returnType->base != $2->type->base){
            $2->type->castTo(currentLst->returnType);
        }
		$$ = new return_stmt();
		((return_stmt*)$$)->exp = ($<ExpAst>2);
        int tempOffset = 0;
        for (int i=0; i< currentLst->params; i++){
            int size = currentLst->params[i]->size;
            tempOffset += size;
        }
        tempOffset += $2->type->size;
        ((return_stmt*)$$)->offset = tempOffset;
		
	}
    | IDENTIFIER '(' 
    {
        LocalSymbolTable* tempLst = gst->getLst($1);
        if (tempLst == 0){
            cout<<"error: Function "<<$1<<" not defined yet"<<" at line number "<<lineCount<<endl;
            exit(0);
        }
        currentLstCalled = tempLst;
        paramCount = 0;
    }
    expression_list ')' ';' 
    {

        $$ = new fun_call_stmt();
        ((fun_call_stmt*)$$)->fun_name = $1;
        ((fun_call_stmt*)$$)->expList = $4;
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
        Type* tempType = compatibility_check($1->type, $3->type);
        if (tempType == 0){
            cout<<"Incompatible expressions"<<" at line number "<<lineCount<<endl;
            exit(0);
        }
        if (tempType->base != $1->type->base) $1->type->castTo(tempType);
        if (tempType->base != $3->type->base) $3->type->castTo(tempType);

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
        Type* tempType = arithmetic_check($1->type, $3->type);
        if(tempType==0){
            cout<<"Incompatible expressions"<<" at line number "<<lineCount<<endl;
            exit(0);
        };

        expAst* temp = $1;
    	($$) = new op();
    	((op*)$$)->exp1 = temp;
    	((op*)$$)->exp2 = ($<ExpAst>3);
    	((op*)$$)->opcode = "||";
    	((op*)$$)->optype = 1;

        $$->type = new Type(1);
    }
	;

logical_and_expression
    : equality_expression
    {
    	($$) = ($1);
    }
    | logical_and_expression AND_OP equality_expression 
    {
        Type* tempType = arithmetic_check($1->type, $3->type);
        if(tempType==0){
            cout<<"Incompatible expressions"<<" at line number "<<lineCount<<endl;
            exit(0);
        };

        expAst* temp = $1;
    	($$) = new op();
    	((op*)$$)->exp1 = temp;
    	((op*)$$)->exp2 = ($<ExpAst>3);
    	((op*)$$)->opcode = "&&";
    	((op*)$$)->optype = 1;

        $$->type = new Type(1);
    }
	;

equality_expression
	: relational_expression 
	{
    	($$) = ($1);
    }
    | equality_expression EQ_OP relational_expression 	
    {
        Type* tempType = arithmetic_check($1->type, $3->type);
        if(tempType==0){
            cout<<"Incompatible expressions"<<" at line number "<<lineCount<<endl;
            exit(0);
        };

        expAst* temp = $1;
    	($$) = new op();
    	((op*)$$)->exp1 = temp;
    	((op*)$$)->exp2 = ($<ExpAst>3);
    	((op*)$$)->opcode = "==";
    	((op*)$$)->optype = 1;

        $$->type = new Type(1);
    }
	| equality_expression NE_OP relational_expression
	{
        Type* tempType = arithmetic_check($1->type, $3->type);
        if(tempType==0){
            cout<<"Incompatible expressions"<<" at line number "<<lineCount<<endl;
            exit(0);
        };

        expAst* temp = $1;
    	($$) = new op();
    	((op*)$$)->exp1 = temp;
    	((op*)$$)->exp2 = ($<ExpAst>3);
    	((op*)$$)->opcode = "!=";
    	((op*)$$)->optype = 1;

        $$->type = new Type(1);
    }
	;

relational_expression
	: additive_expression
	{
    	($$) = ($1);
    }
    | relational_expression '<' additive_expression 
    {
        Type* tempType = arithmetic_check($1->type, $3->type);
        if(tempType==0){
            cout<<"Incompatible expressions"<<" at line number "<<lineCount<<endl;
            exit(0);
        };

        if (tempType->base != $1->type->base) $1->type->castTo(tempType);
        if (tempType->base != $3->type->base) $3->type->castTo(tempType);

        expAst* temp = $1;
    	($$) = new op();
    	((op*)$$)->exp1 = temp;
    	((op*)$$)->exp2 = ($<ExpAst>3);
    	((op*)$$)->opcode = "<";
    	((op*)$$)->optype = 1;

        $$->type = new Type(1);
    }
	| relational_expression '>' additive_expression 
	{
        Type* tempType = arithmetic_check($1->type, $3->type);
        if(tempType==0){
            cout<<"Incompatible expressions"<<" at line number "<<lineCount<<endl;
            exit(0);
        };
        if (tempType->base != $1->type->base) $1->type->castTo(tempType);
        if (tempType->base != $3->type->base) $3->type->castTo(tempType);

        expAst* temp = $1;
    	($$) = new op();
    	((op*)$$)->exp1 = temp;
    	((op*)$$)->exp2 = ($<ExpAst>3);
    	((op*)$$)->opcode = ">";
    	((op*)$$)->optype = 1;

        $$->type = new Type(1);
    }
	| relational_expression LE_OP additive_expression 
	{
        Type* tempType = arithmetic_check($1->type, $3->type);
        if(tempType==0){
            cout<<"Incompatible expressions"<<" at line number "<<lineCount<<endl;
            exit(0);
        };
        if (tempType->base != $1->type->base) $1->type->castTo(tempType);
        if (tempType->base != $3->type->base) $3->type->castTo(tempType);

        expAst* temp = $1;
    	($$) = new op();
    	((op*)$$)->exp1 = temp;
    	((op*)$$)->exp2 = ($<ExpAst>3);
    	((op*)$$)->opcode = "<=";
    	((op*)$$)->optype = 1;

        $$->type = new Type(1);
    }
    | relational_expression GE_OP additive_expression 
    {
        Type* tempType = arithmetic_check($1->type, $3->type);
        if(tempType==0){
            cout<<"Incompatible expressions"<<" at line number "<<lineCount<<endl;
            exit(0);
        };
        if (tempType->base != $1->type->base) $1->type->castTo(tempType);
        if (tempType->base != $3->type->base) $3->type->castTo(tempType);

        expAst* temp = $1;
    	($$) = new op();
    	((op*)$$)->exp1 = temp;
    	((op*)$$)->exp2 = ($<ExpAst>3);
    	((op*)$$)->opcode = ">=";
    	((op*)$$)->optype = 1;

        $$->type = new Type(1);
    }
	;

additive_expression 
	: multiplicative_expression
	{
    	($$) = ($1);
    }
	| additive_expression '+' multiplicative_expression
	{
        Type* tempType = arithmetic_check($1->type, $3->type);
        if(tempType==0){
            cout<<"Incompatible expressions"<<" at line number "<<lineCount<<endl;
            exit(0);
        };
        if (tempType->base != $1->type->base) $1->type->castTo(tempType);
        if (tempType->base != $3->type->base) $3->type->castTo(tempType);

        expAst* temp = $1;
    	($$) = new op();
    	((op*)$$)->exp1 = temp;
    	((op*)$$)->exp2 = ($<ExpAst>3);
    	((op*)$$)->opcode = "+";
    	((op*)$$)->optype = 1;

        $$->type = tempType;
    } 
	| additive_expression '-' multiplicative_expression 
	{
        Type* tempType = arithmetic_check($1->type, $3->type);
        if(tempType==0){
            cout<<"Incompatible expressions"<<" at line number "<<lineCount<<endl;
            exit(0);
        };
        if (tempType->base != $1->type->base) $1->type->castTo(tempType);
        if (tempType->base != $3->type->base) $3->type->castTo(tempType);

        expAst* temp = $1;
    	($$) = new op();
    	((op*)$$)->exp1 = temp;
    	((op*)$$)->exp2 = ($<ExpAst>3);
    	((op*)$$)->opcode = "-";
    	((op*)$$)->optype = 1;

        $$->type = tempType;
    }
	;

multiplicative_expression
	: unary_expression
	{
    	($$) = ($1);
    }
	| multiplicative_expression '*' unary_expression 
	{
        Type* tempType = arithmetic_check($1->type, $3->type);
        if(tempType==0){
            cout<<"Incompatible expressions"<<" at line number "<<lineCount<<endl;
            exit(0);
        };
        if (tempType->base != $1->type->base) $1->type->castTo(tempType);
        if (tempType->base != $3->type->base) $3->type->castTo(tempType);

        expAst* temp = $1;
    	($$) = new op();
    	((op*)$$)->exp1 = temp;
    	((op*)$$)->exp2 = ($<ExpAst>3);
    	((op*)$$)->opcode = "*";
    	((op*)$$)->optype = 1;

        $$->type = tempType;
    }
	| multiplicative_expression '/' unary_expression 
	{
        Type* tempType = arithmetic_check($1->type, $3->type);
        if(tempType==0){
            cout<<"Incompatible expressions"<<" at line number "<<lineCount<<endl;
            exit(0);
        };
        if (tempType->base != $1->type->base) $1->type->castTo(tempType);
        if (tempType->base != $3->type->base) $3->type->castTo(tempType);

        expAst* temp = $1;
    	($$) = new op();
    	((op*)$$)->exp1 = temp;
    	((op*)$$)->exp2 = ($<ExpAst>3);
    	((op*)$$)->opcode = "/";
    	((op*)$$)->optype = 1;

        $$->type = tempType;
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
            LocalSymbolTable* tempLst = gst->getLst($1);

            if (tempLst == 0){
                cout<<"error: Function "<<$1<<" not defined yet"<<" at line number "<<lineCount<<endl;
                exit(0);
            }

	    	$$ = new fun_call();
	    	((fun_call*)$$)->fun_name = $1;
            $$->type = tempLst->returnType;

	    }
    | IDENTIFIER '(' 
    {
        LocalSymbolTable* tempLst = gst->getLst($1);
        if (tempLst == 0){
            cout<<"error: Function "<<$1<<" not defined yet"<<" at line number "<<lineCount<<endl;
            exit(0);
        }
        currentLstCalled = tempLst;
        paramCount = 0;
    }
    expression_list ')'
    {
    
        LocalSymbolTable* tempLst = gst->getLst($1);

        $$ = new fun_call();
        ((fun_call*)$$)->fun_name = $1;
        $$->type = tempLst->returnType;
        ((fun_call*)$$)->expList = $4;
    }

	| l_expression INC_OP
	{
        if ($1->type->base == 0){
            cout<<"++ operator is not allowed on void type"<<" at line number "<<lineCount<<endl;
            exit(0);
        }

        expAst* temp = $1;
    	($$) = new op();
    	((op*)$$)->exp1 = temp;
    	((op*)$$)->opcode = "++";
    	((op*)$$)->optype = 0;
        $$->type = $1->type;
    }
	;

primary_expression
	: l_expression
	{
    	($$) = ($1);
    }
    | l_expression '=' expression // added this production
    {
        Type* typeTemp = compatibility_check($1->type, $3->type);
        if (typeTemp == 0){
            cout<<"Incompatible tyeps ";
            $1->type->print();
            cout<<", ";
            $3->type->print();
            cout<<" at line number "<<lineCount<<endl;
            exit(0);
        }

        if ($1->type->base != $3->type->base){
            $3->type->castTo($1->type);
        }

        expAst* temp = $1;
    	($$) = new op();
    	((op*)$$)->exp1 = temp;
    	((op*)$$)->exp2 = ($<ExpAst>3);
    	((op*)$$)->opcode = "=";
    	((op*)$$)->optype = 1;

        $$->type = typeTemp;
    }
	| INT_CONSTANT
	{
    	($$) = new int_constant();
    	((int_constant*)$$)->value = $1;
        $$->type = new Type(1);
    }
	| FLOAT_CONSTANT
		{
    	($$) = new float_constant();
    	((float_constant*)$$)->value = $1;
        $$->type = new Type(2);
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
        Variable* temp = currentLst->variables[$1];
        if (temp == 0){
            cout<<"error: ‘"<<$1<<"’ was not declared in this scope "<<" at line number "<<lineCount<<endl;
            exit(0);
        }
        else{
            $$->type = copyType(temp->type);
        }
        ((identifier*)$$)->var = temp;
    }
    | l_expression '[' expression ']'
    {
        Type* isArray = $1->type->child;

        if (isArray == 0){
            cout<<"error: in function "<<currentLst->funcName<<endl;
            cout<<"Not an array "<<" at line number "<<lineCount<<endl;
            exit(0);
        }

        if($3->type->base != 1){
            cout<<"error: in function "<<currentLst->funcName<<endl;
            cout<<"Array Indices Not an Integer"<<" at line number "<<lineCount<<endl;
            exit(0);
        }

        arrayRef* temp = (arrayRef*)$<ExpAst>1;
    	$$ = new index();
    	((index*)$$)->arr = temp;
    	((index*)$$)->exp = ($<ExpAst>3);

        $$->type = isArray;
    }
    ;

expression_list
    : expression
    	{
            
            if (currentLstCalled->checkParam) {
                if (paramCount >= currentLstCalled->params.size()){
                    cout<<"function "<<currentLstCalled->funcName<<" is defined with "<<currentLstCalled->params.size()
                    <<" but is called with more number of arguments at line number "<<lineCount<<endl;
                    exit(0);
                }

                Variable* tempVar = currentLstCalled->params[paramCount];

                Type* tempType = compatibility_check($1->type, tempVar->type);
                if (tempType  == 0){
                    cout<<"Param types defined is not same as called in line number "<<lineCount<<endl;
                    exit(0);
                }
                paramCount++;
                
            }

    		expAst *temp = $1;
    		$$ = new exp_list();
    		((exp_list*)$$)->v.push_back(temp);
    	}
    | expression_list ',' expression
	    {
            if (currentLstCalled->checkParam) {
                if (paramCount >= currentLstCalled->params.size()){
                    cout<<"function "<<currentLstCalled->funcName<<" is defined with "<<currentLstCalled->params.size()
                    <<" arguments only but is called with more number of arguments at line number "<<lineCount<<endl;
                    exit(0);
                }
                Variable* tempVar = currentLstCalled->params[paramCount];
                Type* tempType = compatibility_check($3->type, tempVar->type);
                if (tempType  == 0){
                    cout<<"Param types defined is not same as called in line number "<<lineCount<<endl;
                    exit(0);
                }
                paramCount++;
            }

	    	$$ = $1;
	    	((exp_list*)$$)->v.push_back($3);
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
    {
        unordered_map<string, Variable*>::iterator it= currentLst->variables.find($1->varname);
        if (it != currentLst->variables.end()){
            cout<<"Variable "<<$1->varname<<" in declarations Already defined in function "
            <<currentLst->funcName<<" at line number "<<lineCount<<endl;
            exit(0);
        }
        else{
            currentLst->variables[$1->varname] = $1;
        }
    }
	| declarator_list ',' declarator 
    {
        unordered_map<string, Variable*>::iterator it= currentLst->variables.find($3->varname);
        if (it != currentLst->variables.end()){
            cout<<"Variable "<<$3->varname<<" in declarations Already defined in function "
            <<currentLst->funcName<<" at line number "<<lineCount<<endl;
            exit(0);
        }
        else{
            currentLst->variables[$3->varname] = $3;
        }
    }
	;