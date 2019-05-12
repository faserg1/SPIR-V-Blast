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
#include <functional>
#include <algorithm>

#include <spirv.hpp11>

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
	Remainder,
	PreIncrement,
	PreDecrement,
	PostIncrement,
	PostDecrement,
	/*Bit instructions*/
	BitShiftLogicalLeft,
	BitShiftLogicalRight,
	BitShiftArithmeticLeft,
	BitShiftArithmeticRight,
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
	/*Cast*/
	BitCast,
	/*Misc*/
	Comma,
};

struct Identifier
{
	IdentifierType type;
	std::string name;
};

struct Type
{

};

struct BaseVariable
{
	struct Type type;
	std::string name;
};

struct Literal
{
	LiteralType type;
	union
	{
		uint64_t unum;
		int64_t inum;
		long double dnum;
	};
	std::string string;
};

struct Attribute
{
	std::string name;
	AttributeParamType type;
	std::string paramIdent {};
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
	std::string name;
	std::vector<struct StructMember> members;
};

struct StructMember : Attributable, BaseVariable
{
	std::size_t position; //member position in struct
};

class LexContext
{
public:
	virtual IdentifierType getIdentifierType(std::string name) const = 0;
protected:
  LexContext() = default;
	virtual ~LexContext() = default;
};

class AbstractSyntaxTreeContainer
{
public:
	virtual std::vector<struct Struct> getStructs() const = 0;
protected:
	AbstractSyntaxTreeContainer() = default;
	virtual ~AbstractSyntaxTreeContainer() = default;
};

namespace gen
{
	class location;
}

typedef std::function<void(const gen::location &, const std::string &)> ErrorCallback;

class Context;
namespace gen
{
	class BlastScanner;
}

} //%code requires

%param { gen::BlastScanner *scanner }
%lex-param { LexContext &ctx }
%parse-param { ErrorCallback callback }
%parse-param { Context &ctx }

%code
{
gen::BlastParser::symbol_type yylex(gen::BlastScanner *scanner, LexContext &ctx);

struct scope
{
	std::unordered_map<std::string, Identifier> identifiers;
};

class Attributes
{
public:
	static Attribute fromIdentifiers(const std::string &ident, const std::string &ident2);
	static Attribute fromIdentifier(const std::string &ident);
	static Attribute fromLiteral(const std::string &ident, const Literal &l);
private:
	Attributes() = delete;
	~Attributes() = delete;
};

class ConstantHelper
{
public:
	static spv::Dim dimFromLiteral(const Literal &l);
	static spv::Dim dimFromIdent(const std::string &ident);
	static spv::ImageFormat imageFormatFromIdent(const std::string &ident);
	static spv::AccessQualifier accessQualifierFromIdent(const std::string &ident);
private:
	ConstantHelper() = delete;
	~ConstantHelper() = delete;
};

class Context :
	public virtual LexContext,
	public virtual AbstractSyntaxTreeContainer
{
public:
	//AST Container methods
	std::vector<struct Struct> getStructs() const override;
	//Context methods
	void addTempAttribute(Attribute attr);
	std::vector<Attribute> getAndClearTempAttributes();
	IdentifierType getIdentifierType(std::string name) const override;
	Struct &defineStruct(const std::string &name);
	Struct &getStruct(const std::string &name);
	std::vector<struct StructMember> defineStructMembers(Type t, const std::vector<std::string> &names);
	void indexStructMembers(Struct &s);
	void operator++();
	void operator--();
private:
	char *cursor = nullptr;
	std::vector<Attribute> tempAttributes;
	std::vector<scope> scopes;
	std::vector<Struct> structs;
private:
	void pushIdentifierToScope(const Identifier &id);
};

} //%code

%token END 0
%token DO "do" WHILE "while" IF "if" SWITCH "switch" FOR "for"
%token DEFAULT "default" CASE "case"
%token RETURN "return" BREAK "break" CONTINUE "continue"
%token CONST "const"
%token C_TRUE "true" C_FALSE "false"
%token BITCAST "bit_cast"
%token VOID "void" INT "int" FLOAT "float" BOOL "bool"
%token MATRIX "mat" VECTOR "vec"
%token IMAGE "img" SAMPLER "smpl" SAMPLED_IMAGE "simg"
%token STRUCT "struct"
%token IDENTIFIER USER_DEFINED_TYPE
%token NUMLITERAL STRINGLITERAL
%token MOD "/%"
%token OR "||" AND "&&" EQ "==" NE "!="
%token LESS '<' MORE '>' LESS_EQ "<=" MORE_EQ ">="
%token INC "++" DEC "--"
%token PL_EQ "+=" MI_EQ "-=" MUL_EQ "*=" DIV_EQ "/=" MOD_EQ "/%=" REM_EQ "%="
%token SHIFT_LEFT_A_EQ "<<=" SHIFT_RIGHT_A_EQ ">>=" SHIFT_LEFT_L_EQ "!<<=" SHIFT_RIGHT_L_EQ "!>>="
%token SHIFT_LEFT_A "<<" SHIFT_RIGHT_A ">>" SHIFT_LEFT_L "!<<" SHIFT_RIGHT_L "!>>"
%token AND_EQ "&=" OR_EQ "|=" XOR_EQ "^="
%token PTR_ACCESS "->"
%token ATTR_OPEN "[[" ATTR_CLOSE "]]"

%left ','
%right '?' ':' '=' "+=" "-=" "*=" "/=" "/%=" "%=" ">>=" "<<=" "!>>=" "!<<=" "&=" "|=" "^="
%left "||"
%left "&&"
%left '|'
%left '^'
%left '&'
%left EQ NE
%left LESS MORE LESS_EQ MORE_EQ
%left SHIFT_LEFT_A SHIFT_RIGHT_A SHIFT_LEFT_L SHIFT_RIGHT_L
%left '+' '-'
%left '*' '/' MOD '%'
%right INC DEC '!' '~' UMINUS UPLUS PTR_DR ADDR
%left '(' '[' '.' PTR_ACCESS POST_INC POST_DEC

%type<Literal> NUMLITERAL STRINGLITERAL
%type<std::string> IDENTIFIER USER_DEFINED_TYPE
%type<Struct> struct_a struct
%type<std::vector<struct StructMember>> struct_body struct_members_continious struct_member_a struct_member
%type<std::vector<std::string>> struct_member_continious
%type<Type> type
%type<spv::Dim> image_dim
%type<spv::ImageFormat> image_format
%type<spv::AccessQualifier> image_access_qualifier
%type<Attribute> attribute attribute_body

%%

shader: {++ctx;} shader_unit_rec {--ctx;};

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

struct_a: attribute_rec struct {$$ = $2;}
| struct;

struct: STRUCT IDENTIFIER { ctx.defineStruct($2); } '{' struct_body '}' ';' {auto &s = ctx.getStruct($2); s.members = $5; ctx.indexStructMembers(s); $$ = s;};

struct_body: struct_members_continious {$$ = std::move($1);}
| %empty {$$ = {};};

struct_members_continious: struct_members_continious struct_member_a {$$ = std::move($1); std::move($2.begin(), $2.end(), std::back_inserter($$));}
| struct_member_a {$$ = std::move($1); };

struct_member_a: attribute_rec struct_member {$$ = $2; auto attrs = ctx.getAndClearTempAttributes(); std::for_each($$.begin(), $$.end(), [&attrs](StructMember &member){member.attributes = attrs;});}
| struct_member {$$ = std::move($1);};

struct_member: type struct_member_continious ';' {$$ = ctx.defineStructMembers($1, std::move($2));};

struct_member_continious: struct_member_continious ',' IDENTIFIER {$$ = std::move($1); $$.push_back($3);}
| IDENTIFIER {$$ = {$1};};

/* EXPRESSIONS */

comma_expression: expression
| comma_expression ',' expression;

expression: IDENTIFIER
| literal
| '(' comma_expression ')'
| expression '(' ')'
| expression '(' comma_expression ')'
| expression '[' expression ']'
| expression PTR_ACCESS expression
| expression '.' expression
| expression '=' expression
| expression '+' expression
| expression '-' expression
| expression '*' expression
| expression '/' expression
| expression MOD expression
| expression '%' expression
| expression "+=" expression
| expression "-=" expression
| expression "*=" expression
| expression "/=" expression
| expression "/%=" expression
| expression "%=" expression
| expression ">>=" expression
| expression "<<=" expression
| expression "!>>=" expression
| expression "!<<=" expression
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
| expression '?' expression ':' expression
| BITCAST '<' type '>' '(' expression ')'
| expression SHIFT_LEFT_A expression
| expression SHIFT_RIGHT_A expression
| expression SHIFT_LEFT_L expression
| expression SHIFT_RIGHT_L expression;

/* Literals */

literal: NUMLITERAL
| STRINGLITERAL
| boolean_const_expr;

boolean_const_expr: C_TRUE
| C_FALSE;

/* VARIABLE DECLARATION */

var_def: attribute_rec type var_def_continious;

var_def_continious: var_def_continious ',' IDENTIFIER
| var_def_continious ',' IDENTIFIER '=' expression
| IDENTIFIER
| IDENTIFIER '=' expression;

/* TYPES */

type: type_mod type_variant type_suffix {$$ = {};};

type_suffix: type_suffix type_suffix_variant
| %empty ;

type_suffix_variant: '*'
| "[]";

type_mod: CONST
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
int_type: INT
| INT '<' '>'
| INT '<' NUMLITERAL '>'
| INT '<' NUMLITERAL ',' NUMLITERAL '>';
float_type: FLOAT
| FLOAT '<' '>'
| FLOAT '<' NUMLITERAL '>';
bool_type: BOOL;

vector_type: VECTOR '<' simple_type ',' NUMLITERAL '>';

matrix_type: MATRIX '<' simple_type ',' NUMLITERAL ',' NUMLITERAL '>';

image_type: IMAGE '<' image_sampled_type ',' image_dim ',' image_depth ',' image_ms ',' image_sampled ',' image_format ',' image_access_qualifier '>'
| IMAGE '<' image_sampled_type ',' image_dim ',' image_depth ',' image_ms ',' image_sampled ',' image_format '>';
image_sampled_type: int_type
| void_type;
image_dim: NUMLITERAL {$$ = ConstantHelper::dimFromLiteral($1);}
| IDENTIFIER {$$ = ConstantHelper::dimFromIdent($1);};
image_depth: NUMLITERAL;
image_ms: NUMLITERAL;
image_sampled: NUMLITERAL;
image_format: IDENTIFIER;
image_access_qualifier: IDENTIFIER;

sampler_type: SAMPLER;

sampled_image_type: SAMPLED_IMAGE;

/* Attributes */
attribute_rec: attribute_rec attribute {ctx.addTempAttribute($2);}
| attribute {ctx.addTempAttribute($1);};

attribute: "[[" attribute_body "]]" {$$ = std::move($2);};

attribute_body: IDENTIFIER  {$$ = Attributes::fromIdentifier($1);}
| IDENTIFIER ':' NUMLITERAL {$$ = Attributes::fromLiteral($1, $3);}
| IDENTIFIER ':' STRINGLITERAL {$$ = Attributes::fromLiteral($1, $3);}
| IDENTIFIER ':' IDENTIFIER {$$ = Attributes::fromIdentifiers($1, $3);};

%%

Attribute Attributes::fromIdentifiers(const std::string &ident, const std::string &ident2)
{
	Attribute attr;
	attr.name = ident;
	attr.paramIdent = ident2;
	attr.type = AttributeParamType::Identifier;
	return attr;
}

Attribute Attributes::fromIdentifier(const std::string &ident)
{
	Attribute attr;
	attr.name = ident;
	attr.type = AttributeParamType::None;
	return attr;
}

Attribute Attributes::fromLiteral(const std::string &ident, const Literal &l)
{
	Attribute attr;
	attr.name = ident;
	attr.type = AttributeParamType::Literal;
	attr.paramLiteral = l;
	return attr;
}

spv::Dim ConstantHelper::dimFromLiteral(const Literal &l)
{
	switch (l.type)
	{
		case LiteralType::INumber:
			switch (l.inum)
			{
				case 1:
					return spv::Dim::Dim1D;
				case 2:
					return spv::Dim::Dim2D;
				case 3:
					return spv::Dim::Dim3D;
			}
	}
	// TODO: [OOKAMI] Throw exception
	throw 0;
}

spv::Dim ConstantHelper::dimFromIdent(const std::string &ident)
{
	if (ident == "Cube")
		return spv::Dim::Cube;
	else if (ident == "Rect")
		return spv::Dim::Rect;
	else if (ident == "Buffer")
		return spv::Dim::Buffer;
	else if (ident == "SubpassData")
		return spv::Dim::SubpassData;
	// TODO: [OOKAMI] Throw exception
	throw 0;
}

spv::ImageFormat ConstantHelper::imageFormatFromIdent(const std::string &ident)
{
	// TODO: [OOKAMI] Realization
	return {};
}

spv::AccessQualifier ConstantHelper::accessQualifierFromIdent(const std::string &ident)
{
	// TODO: [OOKAMI] Realization
	return {};
}

// AST Container methods

std::vector<struct Struct> Context::getStructs() const
{
	return structs;
}

// Realization of context
void Context::addTempAttribute(Attribute attr)
{
	tempAttributes.push_back(attr);
}

std::vector<Attribute> Context::getAndClearTempAttributes()
{
	return std::move(tempAttributes);
}

IdentifierType Context::getIdentifierType(std::string name) const
{
	for (auto it = scopes.rbegin(); it != scopes.rend(); it++)
	{
		auto findResult = it->identifiers.find(name);
		if (findResult == it->identifiers.end())
			continue;
		return findResult->second.type;
	}
	return IdentifierType::Undefined;
}

Struct &Context::defineStruct(const std::string &name)
{
	if (getIdentifierType(name) != IdentifierType::Undefined)
		throw 0; // TODO: [OOKAMI] throw error
	Identifier t;
	t.type = IdentifierType::Structure;
	t.name = name;
	pushIdentifierToScope(t);
	Struct s;
	s.name = name;
	s.attributes = getAndClearTempAttributes();
	structs.push_back(s);
	return structs.back();
}

Struct &Context::getStruct(const std::string &name)
{
	auto it = std::find_if(structs.begin(), structs.end(),
		[&name](const Struct &s) -> bool
		{
			return s.name == name;
		});
	if (it == structs.end())
		throw 0;
	return *it;
}

std::vector<struct StructMember> Context::defineStructMembers(Type t, const std::vector<std::string> &names)
{
	std::vector<struct StructMember> members;
	std::transform(names.begin(), names.end(), std::back_inserter(members),
		[&t](const std::string &name)
		{
			StructMember member;
			member.type = t;
			member.name = name;
			member.position = 0;
			return member;
		});
	return std::move(members);
}

void Context::indexStructMembers(Struct &s)
{
	size_t pos = 0;
	for (auto &member : s.members)
		member.position = pos++;
}

void Context::operator++()
{
	scopes.emplace_back();
}

void Context::operator--()
{
	scopes.pop_back();
}

void Context::pushIdentifierToScope(const Identifier &id)
{
	auto &topScope = scopes.back();
	topScope.identifiers.insert(std::make_pair(id.name, id));
}

void gen::BlastParser::error(const gen::location &loc, const std::string &msg)
{
	callback(loc, msg);
}
