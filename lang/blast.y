%define api.parser.class {BlastParser}

%code requires
{
#include <map>
#include <list>
#include <vector>
#include <string>
#include <iostream>
#include <algorithm>
} //%code requires

%token END 0
%token RETURN "return" DO "do" WHILE "while" IF "if" SWITCH "switch" FOR "for"
%token DEFAULT "default" CASE "case"
%token BREAK "break" CONTINUE "continue"
%token CONST "const" NOVAR "novar"
%token INT "int" FLOAT "float" BOOL "bool"
%token IDENTIFIER NUMLITERAL STRINGLITERAL
%token OR "||" AND "&&" EQ "==" NE "!="
%token LESS '<' MORE '>' LESS_EQ "<=" MORE_EQ ">="
%token INC "++" DEC "--" PL_EQ "+=" MI_EQ "-="

%left ','
%right '?' ':' '=' "+=" "-=" "*=" "/=" "%=" "&=" "^=" "|="
%left "||"
%left "&&"
%left '|'
%left '^'
%left '&'
%left "==" "!="
%left '>' '<' ">=" "<="
%left '+' '-'
%left '*' '/' '%'
%right "++" "--" '!' '~' UMINUS UPLUS PTR_DR ADDR
%left '(' '[' "." "->" POST_INC POST_DEC

%%

shader: functions
| %empty;

/* FUNCTIONS */

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

/* BODY AND STATEMENTS */

body: statement_rec
| %empty;

statement_rec: statement_rec statement
| statement;

statement: statement_nb
| '{' body '}'
| var_def ';' ;

statement_nb_rec: statement_nb_rec statement_nb
| '{' body '}'
| statement_nb;

statement_nb: if_statement
| while_statement
| for_statement
| switch_statement
| do_while_statement
| expression ';'
| RETURN expression ';'
| RETURN ';'
| CONTINUE ';'
| BREAK ';'
| ';'
;

/* CONTROL SWITCHS*/

if_statement: IF '(' expression ')' statement
| IF '(' var_def ';' expression ')' statement;
while_statement: WHILE '(' expression ')' statement;
for_statement: FOR '(' for_init ';' for_condition ';' for_action ')' statement;
switch_statement: SWITCH switch_init switch_body;
do_while_statement: DO statement WHILE '(' expression ')' ';'

for_init: var_def
| %empty;

for_condition: expression
| %empty;

for_action: expression
| %empty;

switch_init: '(' expression ')'
| '(' var_def ';' expression ')';

switch_body: '{' switch_case_rec '}';

switch_case_rec: switch_case_rec switch_case
| switch_case_rec switch_default_case
| %empty;

switch_case: CASE NUMLITERAL ':' case_body ;
switch_default_case: DEFAULT ':' case_body ;

case_body: statement_nb_rec;

/* EXPRESSIONS */

comma_expression: expression
| comma_expression ',' expression;

expression: IDENTIFIER
| NUMLITERAL
| STRINGLITERAL
| '(' comma_expression ')'
| expression '(' ')'
| expression '(' comma_expression ')'
| expression '[' expression ']'
| expression '=' expression
| expression '+' expression
| expression '-' expression
| expression '*' expression
| expression '/' expression
| expression '%' expression
| expression "+=" expression
| expression "-=" expression
| expression "*=" expression
| expression "/=" expression
| expression OR expression
| expression AND expression
| expression EQ expression
| expression NE expression
| '-' expression %prec UMINUS
| '+' expression %prec UPLUS
| '*' expression %prec PTR_DR
| '&' expression %prec ADDR
| "++" expression
| "--" expression
| expression "++" %prec POST_INC
| expression "--" %prec POST_DEC
| expression '?' expression ':' expression;

/* VARIABLE DECLARATION */

var_def: type var_def_continious;

var_def_continious: var_def_continious ',' IDENTIFIER
| var_def_continious ',' IDENTIFIER '=' expression
| IDENTIFIER
| IDENTIFIER '=' expression;

/* TYPES */

type: type_mod simple_type type_ptr_suffix ;

type_ptr_suffix: type_ptr_suffix '*'
| %empty ;

type_mod: CONST
| NOVAR
| %empty;

simple_type: int_type
| float_type
| bool_type ;

int_type: INT;
float_type: FLOAT;
bool_type: BOOL;

%%