whitespaces													[\t\n\r ]

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

STRING_LITERAL												\"([^"\\]|\\.)*\"


/* Operators */
NOT_OP														\!
ASS_OP														=
ADD_OP														\+
SUB_OP														\-
MUL_OP														\*
DIV_OP														\/
LT_OP														\<
GL_OP														\>
LE_OP														\<=
GE_OP														\>=
EQ_OP														==
NE_OP														\!=
AND_OP														\&\&
OR_OP														\|\|
INC_OP														\+\+

//others
OTHERS 														\(|\)|\[|\]|;|,|\{|\}

%%
{whitespaces}												//return WHITESPACE; Do nothing
{NOT_OP}													return NOT_OP;
{ASS_OP}													return ASS_OP;
{ADD_OP}													return ADD_OP;
{SUB_OP}													return SUB_OP;
{MUL_OP}													return MUL_OP;
{DIV_OP}													return DIV_OP;
{LT_OP}														return LT_OP;
{GL_OP}														return GL_OP;
{LE_OP}														return LE_OP;
{GE_OP}														return GE_OP;
{EQ_OP}														return EQ_OP;
{NE_OP}														return NE_OP;
{AND_OP}													return AND_OP;
{OR_OP}														return OR_OP;
{INC_OP}													return INC_OP;
{STRING_LITERAL}											return STRING_LITERAL;
{void}														return VOID;
{int}														return INT;
{float}														return FLOAT;
{return}													return RETURN;
{if}														return IF;
{else}														return ELSE;
{while}														return WHILE;
{for}														return FOR;
{identifiers}												return IDENTIFIER;
{float_constant}											return FLOAT_CONSTANT;
{int_constant}												return INT_CONSTANT;
.															return matched()[0];
