%token END 0
%token RETURN "return" DO "do" WHILE "while" IF "if" SWITCH "switch" FOR "for"
%token CONST "const"
%token INT "int" FLOAT "float" BOOL "bool"
%token IDENTIFIER NUMLITERAL STRINGLITERAL
%token OR "||" AND "&&" EQ "==" NE "!="
%token INC "++" DEC "--" PL_EQ "+=" MI_EQ "-="

%left ','
%left '=' "+=" "-=" "*=" "/=" "%=" "&=" "^=" "|="
%left "||"
%left "&&"
%left '|'
%left '^'
%left '&'
%left "==" "!="
%left '>' '<' ">=" "<="
%left '+' '-'
%left '*' '/' '%'
%right "++" "--" '!' '~'
%left '(' '[' "." "->"

%%

shader: functions
| %empty;



functions: function_decl function_body functions // recursion
| function_decl function_body // function
| function_decl ';'  // function prototype
;

function_decl: type IDENTIFIER function_parameters

function_parameters: '(' function_parameter_list ')' 
| '(' ')';

function_parameter_list: function_parameter
| function_parameter_list ',' function_parameter;

function_parameter: type IDENTIFIER
| type;

function_body: '{' body '}';

body: statement_rec
| %empty;

statement_rec: statement_rec statement
| statement;

statement: '{' body '}'
| if_statement
| while_statement
| expression ';'
;

if_statement: IF '(' expression ')' statement;
while_statement: WHILE '(' expression ')' statement;

expression: IDENTIFIER
| NUMLITERAL
| STRINGLITERAL
| '(' expression ')'
| access
| expression ',' expression
| expression OR expression
| expression AND expression;

access: expression '[' expression ']';

type: int_type
| float_type
| bool_type ;

int_type: INT;
float_type: FLOAT;
bool_type: BOOL;

%%