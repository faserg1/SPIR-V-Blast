%skeleton "lalr1.cc"
%define api.parser.class {BlastParser}
%define api.namespace {gen}
%locations

%code requires
{
#include <map>
#include <any>
#include <list>
#include <vector>
#include <string>
#include <iostream>
#include <algorithm>

enum class IdentifierType
{
	Undefined,
	Attribute,
	Structure,
	Function,
	Parameter,
	Variable
};

enum class LiteralType
{
	String,
	UNumber,
	INumber,
	DNumber
};

enum class AttributeParamType
{
	None,
	Identifier,
	Literal,
};

enum class ExpressionType
{
	None,
	/*Basic*/
	Literal,
	Identifier,
	/*Ariphmetic*/
	Add,
	Negate,
	Multiply,
	Divide,
	Modulo,
	Increment,
	Decrement,
	/*Logical*/
	Equal,
	NonEqual,
	And,
	Or,
	Less,
	More,
	LessOrEqual,
	MoreOrEqual,
	/*Control Switch - condition and loops */
	If,
	For,
	While,
	DoWhile,
	Switch,
	/*Control Switch - switch cases*/
	SwitchCase,
	SwitchDefault,
	/*Control Switch - common*/
	FunctionCall,
	Return,
	Break,
	Continue,
	/*References*/
	Reference, Dereference,
	/*Misc*/
	Comma,
};

struct Identifier
{
	IdentifierType type;
	std::string name;
};

struct Attribute
{
	Identifier ident {};
	AttributeParamType type;
	Identifier paramIdent {};
	Literal paramLiteral {};
};

struct Function
{
	std::string name;
	struct Expression body;
};

struct Struct
{
	
};

struct Literal
{
	LiteralType type;
	union
	{
		uint64_t unum;
		int64_t inum;
		double dnum; 
		std::string string;
	} literal;
};

typedef  std::list<struct Expression> ExpressionParams;

struct Expression
{
	ExpressionType type;
	Identifier ident {};
	Literal literal {};
	ExpressionParams params;
};

struct lexcontext;

} //%code requires

%param { lexcontext &ctx }

%code
{
	struct lexcontext
	{
		std::vector<Attribute> tempAttributes;
	public:
	private:
		void addTempAttribute(const std::string &name, AttributeParamType type, std::any param)
		{
			Attribute attr;
			attr.ident = { IdentifierType::Attribute, name };
			attr.type = type;
			switch (type)
			{
			case AttributeParamType::Identifier:
				attr.paramIdent = std::any_cast<Identifier>(param);
				attr.paramIdent.type = IdentifierType::Variable;
				break;
			case AttributeParamType::Literal:
				attr.paramLiteral = std::any_cast<Literal>(param);
				break;
			}
			tempAttributes.push_back(attr);
		}
	};
} //%code

%token END 0
%token DO "do" WHILE "while" IF "if" SWITCH "switch" FOR "for"
%token DEFAULT "default" CASE "case"
%token RETURN "return" BREAK "break" CONTINUE "continue"
%token CONST "const" NOVAR "novar" RUNTIME "runtime"
%token VOID "void" INT "int" FLOAT "float" BOOL "bool"
%token MATRIX "mat" VECTOR "vec"
%token IMAGE "img" SAMPLER "smpl" SAMPLED_IMAGE "simg"
%token STRUCT "struct"
%token IDENTIFIER USER_DEFINED_TYPE NUMLITERAL STRINGLITERAL
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
%left EQ NE
%left LESS MORE LESS_EQ MORE_EQ
%left '+' '-'
%left '*' '/' '%'
%right INC DEC '!' '~' UMINUS UPLUS PTR_DR ADDR
%left '(' '[' "." "->" POST_INC POST_DEC

%%

shader: shader_unit_rec;

shader_unit_rec: shader_unit_rec shader_unit
| %empty;

shader_unit: function
| function_prototype
| struct ;

/* FUNCTIONS */

function: function_decl function_body ;

function_prototype: function_decl ';' ;

function_decl: attribute_rec type IDENTIFIER function_parameters

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

/* Structs */

struct: attribute_rec STRUCT '{' struct_body '}' ;

struct_body: struct_members_continious ;

struct_members_continious: struct_members_continious struct_member
| struct_member;

struct_member: attribute_rec type struct_member_continious ';' ;

struct_member_continious: struct_member_continious ',' IDENTIFIER
| IDENTIFIER;

/* EXPRESSIONS */

comma_expression: expression
| comma_expression ',' expression;

expression: IDENTIFIER
| literal
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
| expression LESS expression
| expression MORE expression
| expression LESS_EQ expression
| expression MORE_EQ expression
| '-' expression %prec UMINUS
| '+' expression %prec UPLUS
| '*' expression %prec PTR_DR
| '&' expression %prec ADDR
| INC expression
| DEC expression
| expression INC %prec POST_INC
| expression DEC %prec POST_DEC
| expression '?' expression ':' expression;

/* Literals */

literal: NUMLITERAL
| STRINGLITERAL;

/* VARIABLE DECLARATION */

var_def: attribute_rec type var_def_continious;

var_def_continious: var_def_continious ',' IDENTIFIER
| var_def_continious ',' IDENTIFIER '=' expression
| IDENTIFIER
| IDENTIFIER '=' expression;

/* TYPES */

type: type_mod type_variant type_suffix ;

type_suffix: type_suffix type_suffix_variant
| %empty ;

type_suffix_variant: '*'
| "[]";

type_mod: CONST
| NOVAR
| RUNTIME
| %empty;

type_variant: simple_type
| custom_type
| complex_type_variant;

custom_type: USER_DEFINED_TYPE;

simple_type: void_type
| int_type
| float_type
| bool_type ;

complex_type_variant: matrix_type
| vector_type
| image_type
| sampler_type
| sampled_image_type;

void_type: VOID;
int_type: INT;
float_type: FLOAT;
bool_type: BOOL;

vector_type: VECTOR '<' simple_type ',' NUMLITERAL '>';
matrix_type: MATRIX '<' simple_type ',' NUMLITERAL ',' NUMLITERAL '>';
image_type: IMAGE;
sampler_type: SAMPLER;
sampled_image_type: SAMPLED_IMAGE;

/* Attributes */
attribute_rec: attribute_rec attribute
| %empty;

attribute: "[[" attribute_body "]]" ;

attribute_body: IDENTIFIER
| IDENTIFIER ':' NUMLITERAL
| IDENTIFIER ':' STRINGLITERAL
| IDENTIFIER ':' IDENTIFIER;

%%

// TODO: [OOKAMI] Use re2c and use context to determine whenether the token is ident or user defined type.
// TODO: [OOKAMI] Maybe I should use function name as like user defined types? user defined functions xD