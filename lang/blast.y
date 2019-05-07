// Use the default C++ style
%skeleton "lalr1.cc"
// Define the ParserClass
%define api.parser.class {BlastParser}
// Define namespace
%define api.namespace {gen}
// Request that symbols be handled as a whole (type, value, and possibly location) in the scanner.
%define api.token.constructor
// Use the default variant type as semantic type
%define api.value.type variant
// Enable locations!
%locations

%code requires
{
#include <map>
#include <unordered_map>
#include <any>
#include <list>
#include <vector>
#include <string>
#include <memory>
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
	PreIncrement,
	PostIncrement,
	PreDecrement,
	PostDecrement,
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
	std::size_t id;
};

struct Type
{

};

struct BaseVariable
{
	struct Type type;
	struct Identifier ident;
};

struct Literal
{
	LiteralType type;
	uint64_t unum;
	int64_t inum;
	double dnum;
	std::string string;
};

struct Attribute
{
	struct Identifier ident {};
	AttributeParamType type;
	struct Identifier paramIdent {};
	struct Literal paramLiteral {};
};

struct Attributable
{
	std::vector<struct Attribute> attributes;
};

typedef std::list<struct Expression> ExpressionParams;

struct Expression
{
	ExpressionType type;
	Identifier ident {};
	Literal literal {};
	ExpressionParams params;
};

typedef std::vector<struct FunctionParameter> FunctionParameters;

struct FunctionDeclaration
{
	std::string name;
	FunctionParameters parameters;
};

struct Function : Attributable
{
	std::string name;
	struct Expression body;
};

struct FunctionParameter : BaseVariable
{
	std::size_t position; //position in parameters list
};

struct Struct : Attributable
{
	std::vector<struct StructMember> members;
};

struct StructMember : Attributable, BaseVariable
{
	std::size_t position; //member position in struct
};

class LexContext
{
public:
	virtual IdentifierType getIdentifierType(std::string name) = 0;
	virtual Identifier getIdentifier(std::string name) = 0;
	virtual Type getUserDefinedType(std::string name) = 0;
protected:
  LexContext() = default;
	virtual ~LexContext() = default;
};

class Context;

} //%code requires

%lex-param { ctx }
%parse-param { Context &ctx }

%code
{

struct scope
{
	std::unordered_map<std::string, Identifier> identifiers;
};

class Context :
	public virtual LexContext
{
public:
	void addTempAttribute(const std::string &name, AttributeParamType type, std::any param);
	void operator++();
	void operator--();
private:
	char *cursor = nullptr;
	std::vector<Attribute> tempAttributes;
	std::vector<scope> scopes;
private:

};

} //%code

%token END 0
%token DO "do" WHILE "while" IF "if" SWITCH "switch" FOR "for"
%token DEFAULT "default" CASE "case"
%token RETURN "return" BREAK "break" CONTINUE "continue"
%token CONST "const" SPECCONST "specconst" NOVAR "novar" RUNTIME "runtime"
%token VOID "void" INT "int" FLOAT "float" BOOL "bool"
%token MATRIX "mat" VECTOR "vec"
%token IMAGE "img" SAMPLER "smpl" SAMPLED_IMAGE "simg"
%token STRUCT "struct"
%token IDENTIFIER USER_DEFINED_TYPE
%token NUMLITERAL STRINGLITERAL
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

%type<Literal> NUMLITERAL STRINGLITERAL
%type<std::string> IDENTIFIER

%%

shader: shader_unit_rec;

shader_unit_rec: shader_unit_rec shader_unit
| %empty;

shader_unit: function_a
| function_prototype_a
| struct_a ;

/* FUNCTIONS */

function_a: function
| attribute_rec function;

function_prototype_a: function_prototype
| attribute_rec function_prototype;

function: function_decl function_body {--ctx;};

function_prototype: function_decl {--ctx;} ';' ;

function_decl: type IDENTIFIER {++ctx;} function_parameters;

function_parameters: '(' function_parameter_list ')'
| '(' ')';

function_parameter_list: function_parameter
| function_parameter_list ',' function_parameter;

function_parameter: type IDENTIFIER
| type;

function_body: braced_body;

/* BODY AND STATEMENTS */

braced_body: '{' {++ctx;} body {--ctx;} '}';

body: statement_rec
| %empty;

statement_rec: statement_rec statement
| statement;

statement: statement_nb
| braced_body
| var_def ';' ;

statement_nb_rec: statement_nb_rec statement_nb
| braced_body
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

struct_a: attribute_rec struct
| struct;

struct: STRUCT '{' struct_body '}';

struct_body: struct_members_continious ;

struct_members_continious: struct_members_continious struct_member_a
| struct_member_a;

struct_member_a: attribute_rec struct_member
| struct_member;

struct_member: type struct_member_continious ';' ;

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
| expression "->" expression
| expression "." expression
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
| expression "%=" expression
| expression "&=" expression
| expression "|=" expression
| expression "^=" expression
| expression '^' expression
| expression '|' expression
| expression '&' expression
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
| '!' expression
| '~' expression
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
| SPECCONST
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
| attribute;

attribute: "[[" attribute_body "]]" ;

attribute_body: IDENTIFIER
| IDENTIFIER ':' NUMLITERAL
| IDENTIFIER ':' STRINGLITERAL
| IDENTIFIER ':' IDENTIFIER;

%%

// Realization of context
void Context::addTempAttribute(const std::string &name, AttributeParamType type, std::any param)
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

void Context::operator++()
{
	scopes.emplace_back();
}

void Context::operator--()
{
	scopes.pop_back();
}

// TODO: [OOKAMI] Use re2c and use context to determine whenether the token is ident or user defined type.
// TODO: [OOKAMI] Maybe I should use function name as like user defined types? user defined functions xD
