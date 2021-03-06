whitespaces													[\t\r ]

/* keywords */

void														void
int															int
float 														float
return														return
if 															if
else														else
while														while
for 														for


/* identifiers */
identifiers													[a-zA-Z_][a-zA-Z0-9_]*

/* contants */
int_constant												([1-9][0-9]*)|(0[bB][01]*)|(0[xX][0-9a-fA-F]*)|(0[0-7]*)

// float contants
integer_part												[0-9]*
fractional_part												[0-9]*
exponent_part												([eE][+-][0-9]*)|([eE][0-9]*)
float_constant												({integer_part}\.{fractional_part}{exponent_part})|({integer_part}\.{fractional_part})

STRING_LITERAL												\"(\\.|[^"\\])*\"


/* Operators */
NOT_OP														\!
ADD_OP														\+
SUB_OP														\-
MUL_OP														\*
DIV_OP														\/
LT_OP														\<
GT_OP														\>
LE_OP														\<=
GE_OP														\>=
EQ_OP														==
NE_OP														\!=
AND_OP														\&\&
OR_OP														\|\|
INC_OP														\+\+

//others
OTHERS 														\(|\)|\[|\]|;|,|\{|\}

new_line													\n

%%
{new_line}													{
																Parser::lineCount++;
															}
{whitespaces}												//return Parser::WHITESPACE; Do nothing
{NOT_OP}													return Parser::NOT_OP;
{INC_OP}													return Parser::INC_OP;
{LE_OP}														return Parser::LE_OP;
{GE_OP}														return Parser::GE_OP;
{LE_OP}														return Parser::LE_OP;
{GE_OP}														return Parser::GE_OP;
{EQ_OP}														return Parser::EQ_OP;
{NE_OP}														return Parser::NE_OP;
{AND_OP}													return Parser::AND_OP;
{OR_OP}														return Parser::OR_OP;
{STRING_LITERAL}											{Parser::d_val__ = std::string(matched());
															return Parser::STRING_LITERAL;}
{void}														return Parser::VOID;
{int}														return Parser::INT;
{float}														return Parser::FLOAT;
{return}													return Parser::RETURN;
{if}														return Parser::IF;
{else}														return Parser::ELSE;
{while}														return Parser::WHILE;
{for}														return Parser::FOR;
{identifiers}												{Parser::d_val__ = std::string(matched());
															return Parser::IDENTIFIER;}
{float_constant}											{
															float v;
															istringstream(std::string(matched()))>>v;
															Parser::d_val__ = v;
															return Parser::FLOAT_CONSTANT;}
{int_constant}												{
															int v;
															istringstream(std::string(matched()))>>v;
															Parser::d_val__ = v;
															return Parser::INT_CONSTANT;}
.															return matched()[0];
