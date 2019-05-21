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
#include <optional>
#include <list>
#include <vector>
#include <string>
#include <memory>
#include <functional>
#include <algorithm>
#include <type_traits>

#include <spirv.hpp11>

enum class IdentifierType
{
	Undefined,
	Attribute,
	Structure,
	Enumeration,
	Function,
	Parameter,
	Variable
};

enum class LiteralType
{
	String,
	UNumber,
	INumber,
	DNumber,
	Boolean
};

enum class EType
{
	Int,
	Float,
	Bool,
	Void,
	Matrix,
	Vector,
	Image,
	Sampler,
	SampledImage,
	Pointer,
	Array,
	Struct
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
	Statement,
	StatementGroup,
	VariableDeclaration,
	/*Basic*/
	Literal,
	Identifier,
	Type,
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
	UnaryPlus,
	UnaryMinus,
	/*Bit instructions*/
	BitShiftLogicalLeft,
	BitShiftLogicalRight,
	BitShiftArithmeticLeft,
	BitShiftArithmeticRight,
	BitAnd,
	BitOr,
	BitXor,
	BitInvert,
	/*Logical*/
	Equal,
	NonEqual,
	Not,
	And,
	Or,
	Less,
	More,
	LessOrEqual,
	MoreOrEqual,
	/*Control Switch - condition and loops */
	If,
	IfElse,
	Select, // ternary op
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
	ArrayAccess,
	PointerAccess,
	ObjectAccess,
	Reference,
	Dereference,
	/*Cast*/
	BitCast,
	/*Misc*/
	Comma,
	Assignment,
};

struct Identifier
{
	IdentifierType type;
	std::string name;
	int64_t id;
};

struct ScalarType
{

};

struct VoidType
{

};

struct BooleanType : ScalarType
{

};

struct IntType : ScalarType
{
	uint8_t width;
	bool signedness;
};

struct FloatType : ScalarType
{
	uint8_t width;
};

struct TypeInner
{
	EType etype;
	std::any innerType;
};

struct VectorType
{
	TypeInner componentType;
	uint8_t componentCount;
};

struct MatrixType
{
	TypeInner componentType;
	uint8_t rowsCount;
	uint8_t columnsCount;
};

struct ImageType
{
	TypeInner sampledType;
	spv::Dim dimension;
	uint8_t isDepth;
	uint8_t isArrayed;
	uint8_t isMultisampled;
	uint8_t isSampled;
	spv::ImageFormat imageFormat;
	std::optional<spv::AccessQualifier> accessQualifier;
};

struct SamplerType
{

};

struct SampledImageType
{
	TypeInner innerType;
};

struct TypeStruct
{
	std::string name;
};

struct PointerType
{
	TypeInner innerType;
};

struct ArrayType
{
	bool isRuntime;
	uint64_t length;
	TypeInner innerType;
};

struct TypeArraySuffix
{
	bool isArray;
	bool isRuntime;
	uint64_t length;
};

struct TypeModification
{
	bool isConst;
};

struct Type
{
	bool isConst;
	TypeInner innerType;
};

struct BaseVariable
{
	struct Type type;
	std::string name;
	int64_t id;
};

struct Literal
{
	LiteralType type;
	union
	{
		uint64_t unum;
		int64_t inum;
		long double dnum;
		bool boolean;
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
typedef std::list<struct LocalVariable> LocalVariables;

struct Expression
{
	ExpressionType type;
	Identifier ident {};
	Literal literal {};
	Type castToType {};
	ExpressionParams params;
	LocalVariables declaredVariables;
};

struct LocalVariable : BaseVariable
{
	std::optional<struct Expression> initialization;
};

typedef std::vector<struct FunctionParameter> FunctionParameters;

struct FunctionDeclaration
{
	Type returnType;
	std::string name;
	FunctionParameters parameters;
};

struct Function : FunctionDeclaration, Attributable
{
	std::optional<struct Expression> body;
};

struct FunctionParameter : BaseVariable
{
	std::size_t position; //position in parameters list
};

struct Enum
{
	struct Type baseType;
	bool isEnumClass;
	std::map<uint64_t, std::string> enumDeclarations;
};

struct Struct : Attributable
{
	std::string name;
	std::vector<struct StructMember> members;
	int64_t id;
};

struct StructMember : Attributable, BaseVariable
{
	std::size_t position; //member position in struct
};

struct GlobalVariable : Attributable, BaseVariable
{
	std::optional<struct Expression> initialization;
};

using VarNamesAndInits =
	std::vector<std::pair<std::string, std::optional<Expression>>>;
using Vars = std::pair<Type, VarNamesAndInits>;

bool operator==(const Type &t1, const Type &t2);
bool operator!=(const Type &t1, const Type &t2);

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
	virtual std::vector<struct GlobalVariable> getGlobalVariables() const = 0;
	virtual std::vector<struct Function> getFunctions() const = 0;
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
	static uint64_t uintFromLiteral(const Literal &l);
	static int64_t intFromLiteral(const Literal &l);
	static spv::Dim dimFromLiteral(const Literal &l);
	static spv::Dim dimFromIdent(const std::string &ident);
	static spv::ImageFormat imageFormatFromIdent(const std::string &ident);
	static spv::AccessQualifier accessQualifierFromIdent(const std::string &ident);
	static IntType createIntType(uint8_t width, bool signedness);
	static FloatType createFloatType(uint8_t width);
	static ImageType createImageType(TypeInner sampledType, spv::Dim dim,
		uint8_t isDepth, uint8_t isArrayed, uint8_t isMultisampled, uint8_t isSampled,
		spv::ImageFormat imageFormat, std::optional<spv::AccessQualifier> accessQualifier = {});
	static Type createType(const TypeModification &mod, TypeInner innerType,
		const TypeArraySuffix &arraySuffix, uint64_t pointerSuffixCount);
private:
	ConstantHelper() = delete;
	~ConstantHelper() = delete;
};

class Op
{
public:
	static Expression simple(ExpressionType type, const ExpressionParams &params);
	static Expression ident(const Identifier &ident);
	static Expression literal(const Literal &literal);
	static Expression type(const Type &type);
	static Expression group(const ExpressionParams &params);
	static Expression nop();
	static Expression makeBreak();
	static Expression makeContinue();
	static Expression makeReturn();
	static Expression makeReturn(const Expression &expression);
	static Expression makeIf(const Expression &expression,
		const Expression &statement);
	static Expression makeIf(const Expression &varDecl,
		const Expression &expression, const Expression &statement);
	static Expression makeIfElse(const Expression &expression,
		const Expression &statement, const Expression &statementElse);
	static Expression makeIfElse(const Expression &varDecl,
		const Expression &expression, const Expression &statement, const Expression &statementElse);
	static Expression makeFor(const Expression &varDecl,
		const Expression &expression, const Expression &action,
		const Expression &statement);
	static Expression makeWhile(const Expression &expression,
		const Expression &statement);
	static Expression makeDoWhile(const Expression &expression,
		const Expression &statement);
	static Expression makeSwitchCase(const Literal &literal, const Expression &expression);
	static Expression makeSwitchCaseDefault(const Expression &expression);
	static Expression makeSwitch(const Expression &expression, const std::vector<Expression> &body);
	static Expression makeSwitch(const Expression &varDecl, const Expression &expression, const std::vector<Expression> &body);
private:
	Op() = delete;
	~Op() = delete;
};

class Context :
	public virtual LexContext,
	public virtual AbstractSyntaxTreeContainer
{
public:
	//AST Container methods
	std::vector<struct Struct> getStructs() const override;
	std::vector<struct GlobalVariable> getGlobalVariables() const override;
	std::vector<struct Function> getFunctions() const override;
	//Context methods
	void addTempAttribute(Attribute attr);
	std::vector<Attribute> getAndClearTempAttributes();
	IdentifierType getIdentifierType(std::string name) const override;
	Identifier use(std::string name);
	Struct &defineStruct(const std::string &name);
	Struct &getStruct(const std::string &name);
	std::vector<struct StructMember> defineStructMembers(Type t, const std::vector<std::string> &names);
	void indexStructMembers(Struct &s);
	void defineGlobalVariables(Type t, const VarNamesAndInits &vars);
	Expression defineLocalVariables(Type t, const VarNamesAndInits &vars);
	FunctionParameter defineFunctionParameter(Type t, const std::string &name);
	void defineFunctionPrototype(const FunctionDeclaration &decl);
	void defineFunction(const FunctionDeclaration &decl, const Expression &body);
	void defineEnum(std::optional<Type> baseType, bool isEnumClass, std::map<uint64_t, std::string> defines);
	void operator++();
	void operator--();
private:
	char *cursor = nullptr;
	std::vector<Attribute> tempAttributes;
	std::vector<scope> scopes;
	std::vector<Struct> structs;
	std::vector<GlobalVariable> globalVariables;
	std::vector<Function> functions;
	std::vector<Enum> enums;
private:
	int64_t pushIdentifierToScope(Identifier id);
	std::vector<struct Function*> getFunctionsByName(std::string name);
	Function &getOrDefineFunction(const FunctionDeclaration &decl);
};

} //%code

%token END 0
%token DO "do" WHILE "while" IF "if" ELSE "else" SWITCH "switch" FOR "for"
%token DEFAULT "default" CASE "case"
%token RETURN "return" BREAK "break" CONTINUE "continue"
%token CONST "const"
%token C_TRUE "true" C_FALSE "false"
%token BITCAST "bit_cast"
%token VOID "void" INT "int" FLOAT "float" BOOL "bool"
%token MATRIX "mat" VECTOR "vec"
%token IMAGE "img" SAMPLER "smpl" SAMPLED_IMAGE "simg"
%token NAMESPACE "namespace" STRUCT "struct" ENUM "enum" CLASS "class"
%token IDENTIFIER USER_DEFINED_TYPE
%token NUMLITERAL STRINGLITERAL
%token MOD "/%"
%token OR "||" AND "&&" EQ "==" NE "!="
%token LESS_EQ "<=" MORE_EQ ">="
%token INC "++" DEC "--"
%token PL_EQ "+=" MI_EQ "-=" MUL_EQ "*=" DIV_EQ "/=" MOD_EQ "/%=" REM_EQ "%="
%token SHIFT_LEFT_A_EQ "<<=" SHIFT_RIGHT_A_EQ ">>=" SHIFT_LEFT_L_EQ "!<<=" SHIFT_RIGHT_L_EQ "!>>="
%token SHIFT_LEFT_A "<<" SHIFT_RIGHT_A ">>" SHIFT_LEFT_L "!<<" SHIFT_RIGHT_L "!>>"
%token AND_EQ "&=" OR_EQ "|=" XOR_EQ "^="
%token PTR_ACCESS "->"
%token ATTR_OPEN "[[" ATTR_CLOSE "]]"
%token SCOPE_RESOLVE "::"

%left ','
%right '?' ':' '=' "+=" "-=" "*=" "/=" "/%=" "%=" ">>=" "<<=" "!>>=" "!<<=" "&=" "|=" "^="
%left "||"
%left "&&"
%left '|'
%left '^'
%left '&'
%left "==" "!="
%left '<' '>' "<=" ">="
%left "<<"  ">>" "!<<" "!>>"
%left '+' '-'
%left '*' '/' "/%" '%'
%right "++" "--" '!' '~' UMINUS UPLUS PTR_DR ADDR
%left '(' '[' '.' "->" POST_INC POST_DEC
%left "::"

%nonassoc NO_ELSE
%nonassoc ELSE

%type<Literal> literal NUMLITERAL STRINGLITERAL
%type<std::string> IDENTIFIER USER_DEFINED_TYPE
%type<Struct> struct_a struct
%type<std::vector<struct StructMember>> struct_body struct_members_continious struct_member_a struct_member
%type<std::vector<std::string>> struct_member_continious
%type<Type> type
%type<spv::Dim> image_dim
%type<spv::ImageFormat> image_format
%type<spv::AccessQualifier> image_access_qualifier
%type<Attribute> attribute attribute_body
%type<uint8_t> type_pointer_suffix
%type<uint64_t> image_depth image_arrayed image_ms image_sampled
%type<bool> boolean_const_expr
%type<TypeModification> type_mod
%type<TypeArraySuffix> type_array_suffix
%type<TypeInner> type_variant simple_type custom_type complex_type_variant image_sampled_type
%type<VoidType> void_type
%type<IntType> int_type
%type<FloatType> float_type
%type<BooleanType> bool_type
%type<VectorType> vector_type
%type<MatrixType> matrix_type
%type<ImageType> image_type
%type<SamplerType> sampler_type
%type<SampledImageType> sampled_image_type
%type<Expression> function_body braced_body body
%type<Expression> expression comma_expression
%type<Expression> statement_rec statement statement_nb statement_nb_rec
%type<std::vector<Expression>> switch_body switch_case_rec
%type<Expression> if_statement while_statement for_statement switch_statement do_while_statement
%type<std::optional<Expression>> else_statement
%type<Expression> for_init for_condition for_action
%type<Expression> switch_case switch_default_case case_body
%type<VarNamesAndInits> var_def_continious
%type<Vars> var_def
%type<FunctionParameter> function_parameter
%type<FunctionParameters> function_parameter_list function_parameters
%type<FunctionDeclaration> function_decl

%%

shader: {++ctx;} shader_unit_rec {--ctx;};

shader_unit_rec: shader_unit_rec shader_unit
| %empty;

shader_unit: function_a
| function_prototype_a
| struct_a
| enum
| var_def_a ';';

/* FUNCTIONS */

function_a: function
| attribute_rec function;

function_prototype_a: function_prototype
| attribute_rec function_prototype;

function: function_decl function_body {--ctx;} {ctx.defineFunction($1, $2);};

function_prototype: function_decl {--ctx;} ';' {ctx.defineFunctionPrototype($1);};

function_decl: type IDENTIFIER {++ctx;} function_parameters {$$ = {$1, $2, $4};};

function_parameters: '(' function_parameter_list ')' {$$ = $2;}
| '(' ')' {$$ = {};};

function_parameter_list: function_parameter {auto p = $1; p.position = 0; $$ = {p};}
| function_parameter_list ',' function_parameter {auto list = $1; auto p = $3; p.position = list.back().position + 1; list.push_back(p); $$ = list; };

function_parameter: type IDENTIFIER {$$ = ctx.defineFunctionParameter($1, $2);}
| type {$$ = ctx.defineFunctionParameter($1, "");};

function_body: braced_body;

/* BODY AND STATEMENTS */

braced_body: '{' {++ctx;} body {--ctx;} '}' {$$ = $3;};

body: statement_rec
| %empty {$$ = Op::nop();};

statement_rec: statement_rec statement {auto e = $1; e.params.push_back($2); $$ = e;}
| statement {$$ = Op::group({$1});};

statement: statement_nb
| braced_body
| var_def ';' {$$ = ctx.defineLocalVariables($1.first, $1.second);};

statement_nb_rec: statement_nb_rec statement_nb {auto e = $1; e.params.push_back($2); $$ = e;}
| braced_body {$$ = Op::group({$1});}
| statement_nb {$$ = Op::group({$1});};

statement_nb: if_statement
| while_statement
| for_statement
| switch_statement
| do_while_statement
| expression ';'
| RETURN expression ';' {$$ = Op::makeReturn($2);}
| RETURN ';' {$$ = Op::makeReturn();}
| CONTINUE ';' {$$ = Op::makeContinue();}
| BREAK ';' {$$ = Op::makeBreak();}
| ';' {$$ = Op::nop();};

/* CONTROL SWITCHS*/

if_statement: IF '(' {++ctx;} expression ')' statement else_statement {std::optional<Expression> es = $7; $$ = es ? Op::makeIfElse($4, $6, es.value()) : Op::makeIf($4, $6); --ctx;}
| IF '(' {++ctx;} var_def ';' expression ')' statement else_statement {std::optional<Expression> es = $9; auto vars = ctx.defineLocalVariables($4.first, $4.second); $$ = es ? Op::makeIfElse(vars, $6, $8, es.value()) : Op::makeIf(vars, $6, $8); --ctx;};
while_statement: WHILE  '(' {++ctx;} expression ')' statement {$$ = Op::makeWhile($4, $6); --ctx;};
for_statement: FOR '(' {++ctx;} for_init ';' for_condition ';' for_action ')' statement {$$ = Op::makeFor($4, $6, $8, $10); --ctx;};
switch_statement: SWITCH '(' {++ctx;} expression ')' switch_body {$$ = Op::makeSwitch($4, $6); --ctx;}
| SWITCH '(' {++ctx;} var_def ';' expression ')' switch_body {auto vars = ctx.defineLocalVariables($4.first, $4.second); $$ = Op::makeSwitch(vars, $6, $8); --ctx;};
do_while_statement: DO {++ctx;} statement {--ctx;} WHILE '(' expression ')' ';' {$$ = Op::makeDoWhile($3, $7);};

else_statement: ELSE statement {$$ = $2;}
| %empty %prec NO_ELSE {$$ = std::optional<Expression>();};

for_init: var_def {$$ = ctx.defineLocalVariables($1.first, $1.second);}
| %empty {$$ = Op::nop();};

for_condition: expression
| %empty {$$ = Op::nop();};

for_action: expression
| %empty {$$ = Op::nop();};

switch_body: '{' switch_case_rec '}' {$$ = $2;};

switch_case_rec: switch_case_rec switch_case {auto vec = $1; vec.push_back($2);}
| switch_case_rec switch_default_case {auto vec = $1; vec.push_back($2);}
| %empty {$$ = {};};

switch_case: CASE NUMLITERAL ':' case_body {$$ = Op::makeSwitchCase($2, $4);};
switch_default_case: DEFAULT ':' case_body {$$ = Op::makeSwitchCaseDefault($3);};

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

/* Enumeration */

enum: ENUM class_opt IDENTIFIER '{' enum_body '}' ';';

class_opt: CLASS
| %empty;

enum_body: enum_ident_rec
| %empty;

enum_ident_rec: enum_ident_rec ',' enum_ident
| enum_ident;

enum_ident: IDENTIFIER
| IDENTIFIER '=' NUMLITERAL;

/* EXPRESSIONS */

comma_expression: expression {$$ = Op::simple(ExpressionType::Comma, {$1});}
| comma_expression ',' expression {auto c = $1; c.params.push_back($3); $$ = c;};

expression: IDENTIFIER {$$ = Op::ident(ctx.use($1));}
| literal {$$ = Op::literal($1);}
| '(' comma_expression ')' {$$ = $2;}
| expression '(' ')'
| expression '(' comma_expression ')'
| expression '[' expression ']' {$$ = Op::simple(ExpressionType::ArrayAccess, {$1, $3});}
| expression "->" expression {$$ = Op::simple(ExpressionType::PointerAccess, {$1, $3});}
| expression '.' expression {$$ = Op::simple(ExpressionType::ObjectAccess, {$1, $3});}
| expression '=' expression {$$ = Op::simple(ExpressionType::Assignment, {$1, $3});}
| expression '+' expression {$$ = Op::simple(ExpressionType::Add, {$1, $3});}
| expression '-' expression {$$ = Op::simple(ExpressionType::Negate, {$1, $3});}
| expression '*' expression {$$ = Op::simple(ExpressionType::Multiply, {$1, $3});}
| expression '/' expression {$$ = Op::simple(ExpressionType::Divide, {$1, $3});}
| expression "/%" expression {$$ = Op::simple(ExpressionType::Modulo, {$1, $3});}
| expression '%' expression {$$ = Op::simple(ExpressionType::Remainder, {$1, $3});}
| expression "+=" expression {$$ = Op::simple(ExpressionType::Assignment, {$1, Op::simple(ExpressionType::Add, {$1, $3})});}
| expression "-=" expression {$$ = Op::simple(ExpressionType::Assignment, {$1, Op::simple(ExpressionType::Negate, {$1, $3})});}
| expression "*=" expression {$$ = Op::simple(ExpressionType::Assignment, {$1, Op::simple(ExpressionType::Multiply, {$1, $3})});}
| expression "/=" expression {$$ = Op::simple(ExpressionType::Assignment, {$1, Op::simple(ExpressionType::Divide, {$1, $3})});}
| expression "/%=" expression {$$ = Op::simple(ExpressionType::Assignment, {$1, Op::simple(ExpressionType::Modulo, {$1, $3})});}
| expression "%=" expression {$$ = Op::simple(ExpressionType::Assignment, {$1, Op::simple(ExpressionType::Remainder, {$1, $3})});}
| expression ">>=" expression {$$ = Op::simple(ExpressionType::Assignment, {$1, Op::simple(ExpressionType::BitShiftLogicalRight, {$1, $3})});}
| expression "<<=" expression {$$ = Op::simple(ExpressionType::Assignment, {$1, Op::simple(ExpressionType::BitShiftLogicalLeft, {$1, $3})});}
| expression "!>>=" expression {$$ = Op::simple(ExpressionType::Assignment, {$1, Op::simple(ExpressionType::BitShiftArithmeticRight, {$1, $3})});}
| expression "!<<=" expression {$$ = Op::simple(ExpressionType::Assignment, {$1, Op::simple(ExpressionType::BitShiftArithmeticLeft, {$1, $3})});}
| expression "&=" expression {$$ = Op::simple(ExpressionType::Assignment, {$1, Op::simple(ExpressionType::BitAnd, {$1, $3})});}
| expression "|=" expression {$$ = Op::simple(ExpressionType::Assignment, {$1, Op::simple(ExpressionType::BitOr, {$1, $3})});}
| expression "^=" expression {$$ = Op::simple(ExpressionType::Assignment, {$1, Op::simple(ExpressionType::BitXor, {$1, $3})});}
| expression '^' expression {$$ = Op::simple(ExpressionType::BitXor, {$1, $3});}
| expression '|' expression {$$ = Op::simple(ExpressionType::BitOr, {$1, $3});}
| expression '&' expression {$$ = Op::simple(ExpressionType::BitAnd, {$1, $3});}
| expression "||" expression {$$ = Op::simple(ExpressionType::Or, {$1, $3});}
| expression "&&" expression {$$ = Op::simple(ExpressionType::And, {$1, $3});}
| expression "==" expression {$$ = Op::simple(ExpressionType::Equal, {$1, $3});}
| expression "!=" expression {$$ = Op::simple(ExpressionType::NonEqual, {$1, $3});}
| expression '<' expression {$$ = Op::simple(ExpressionType::Less, {$1, $3});}
| expression '>' expression {$$ = Op::simple(ExpressionType::More, {$1, $3});}
| expression "<=" expression {$$ = Op::simple(ExpressionType::LessOrEqual, {$1, $3});}
| expression ">=" expression {$$ = Op::simple(ExpressionType::MoreOrEqual, {$1, $3});}
| '-' expression %prec UMINUS {$$ = Op::simple(ExpressionType::UnaryMinus, {$2});}
| '+' expression %prec UPLUS {$$ = Op::simple(ExpressionType::UnaryPlus, {$2});}
| '*' expression %prec PTR_DR {$$ = Op::simple(ExpressionType::Dereference, {$2});}
| '&' expression %prec ADDR {$$ = Op::simple(ExpressionType::Reference, {$2});}
| '!' expression {$$ = Op::simple(ExpressionType::Not, {$2});}
| '~' expression {$$ = Op::simple(ExpressionType::BitInvert, {$2});}
| "++" expression {$$ = Op::simple(ExpressionType::PreIncrement, {$2});}
| "--" expression {$$ = Op::simple(ExpressionType::PreDecrement, {$2});}
| expression "++" %prec POST_INC {$$ = Op::simple(ExpressionType::PostIncrement, {$1});}
| expression "--" %prec POST_DEC {$$ = Op::simple(ExpressionType::PostDecrement, {$1});}
| expression '?' expression ':' expression {$$ = Op::simple(ExpressionType::Select, {$1, $3, $5});}
| BITCAST '<' type '>' '(' expression ')' {$$ = Op::simple(ExpressionType::BitCast, {Op::type($3), $6});}
| expression "<<" expression {$$ = Op::simple(ExpressionType::BitShiftArithmeticLeft, {$1, $3});}
| expression ">>" expression {$$ = Op::simple(ExpressionType::BitShiftArithmeticRight, {$1, $3});}
| expression "!<<" expression {$$ = Op::simple(ExpressionType::BitShiftLogicalLeft, {$1, $3});}
| expression "!>>" expression {$$ = Op::simple(ExpressionType::BitShiftLogicalRight, {$1, $3});};

/* Literals */

literal: NUMLITERAL
| STRINGLITERAL
| boolean_const_expr {Literal l; l.type = LiteralType::Boolean; l.boolean = $1; $$ = l;};

boolean_const_expr: C_TRUE {$$ = true;}
| C_FALSE {$$ = false;};

inner_use: USER_DEFINED_TYPE SCOPE_RESOLVE IDENTIFIER;

/* VARIABLE DECLARATION */

var_def_a: var_def {ctx.defineGlobalVariables($1.first, $1.second);}
| attribute_rec var_def {ctx.defineGlobalVariables($2.first, $2.second);};

var_def: type var_def_continious {$$ = std::make_pair($1, $2);};

var_def_continious: var_def_continious ',' IDENTIFIER {auto prev = $1; prev.push_back(std::make_pair($3, std::optional<Expression>())); $$ = prev;}
| var_def_continious ',' IDENTIFIER '=' expression {auto prev = $1; prev.push_back(std::make_pair($3, $5)); $$ = prev;}
| IDENTIFIER {$$ = {std::make_pair($1, std::optional<Expression>())};}
| IDENTIFIER '=' expression {$$ = {std::make_pair($1, $3)};};

/* TYPES */

type: type_mod type_variant type_array_suffix type_pointer_suffix {$$ = ConstantHelper::createType($1, $2, $3, $4);};

type_pointer_suffix: type_pointer_suffix '*' {$$ = $1 + 1;}
| %empty {$$ = 0;};

type_array_suffix: '[' NUMLITERAL ']' {$$ = TypeArraySuffix{true, false, ConstantHelper::uintFromLiteral($2)};}
| '[' ']' {$$ = TypeArraySuffix{true, true, 0};}
| %empty {$$ = TypeArraySuffix{false, false, 0};};

type_mod: CONST {$$ = {true};}
| %empty {$$ = {};};

type_variant: simple_type
| custom_type
| complex_type_variant;

custom_type: USER_DEFINED_TYPE {$$ = TypeInner {EType::Struct, TypeStruct{$1}};};

simple_type: void_type {$$ = TypeInner {EType::Void, $1};}
| int_type {$$ = TypeInner {EType::Int, $1};}
| float_type {$$ = TypeInner {EType::Float, $1};}
| bool_type {$$ = TypeInner {EType::Bool, $1};};

complex_type_variant: matrix_type {$$ = TypeInner {EType::Matrix, $1};}
| vector_type {$$ = TypeInner {EType::Vector, $1};}
| image_type {$$ = TypeInner {EType::Image, $1};}
| sampler_type {$$ = TypeInner {EType::Sampler, $1};}
| sampled_image_type {$$ = TypeInner {EType::SampledImage, $1};};

void_type: VOID {$$ = VoidType {};};
int_type: INT {$$ = ConstantHelper::createIntType(32, true);}
| INT '<' '>'{$$ = ConstantHelper::createIntType(32, true);}
| INT '<' NUMLITERAL '>'{$$ = ConstantHelper::createIntType((uint8_t) ConstantHelper::uintFromLiteral($3), true);}
| INT '<' NUMLITERAL ',' NUMLITERAL '>' {$$ = ConstantHelper::createIntType((uint8_t) ConstantHelper::uintFromLiteral($3), ConstantHelper::uintFromLiteral($5) != 0);}
| INT '<' NUMLITERAL ',' boolean_const_expr '>' {$$ = ConstantHelper::createIntType((uint8_t) ConstantHelper::uintFromLiteral($3), $5);};
float_type: FLOAT {$$ = ConstantHelper::createFloatType(32);}
| FLOAT '<' '>' {$$ = ConstantHelper::createFloatType(32);}
| FLOAT '<' NUMLITERAL '>' {$$ = ConstantHelper::createFloatType((uint8_t) ConstantHelper::uintFromLiteral($3));};
bool_type: BOOL {$$ = BooleanType{};};

vector_type: VECTOR '<' simple_type ',' NUMLITERAL '>' {$$ = VectorType{$3, (uint8_t) ConstantHelper::uintFromLiteral($5)};};

matrix_type: MATRIX '<' simple_type ',' NUMLITERAL ',' NUMLITERAL '>' {$$ = MatrixType{$3, (uint8_t) ConstantHelper::uintFromLiteral($5), (uint8_t) ConstantHelper::uintFromLiteral($7)};};

image_type: IMAGE '<' image_sampled_type ',' image_dim ',' image_depth ',' image_arrayed ',' image_ms ',' image_sampled ',' image_format '>' {$$ = ConstantHelper::createImageType($3, $5, (uint8_t) $7, (uint8_t) $9, (uint8_t) $11, (uint8_t) $13, $15);}
| IMAGE '<' image_sampled_type ',' image_dim ',' image_depth ',' image_arrayed ',' image_ms ',' image_sampled ',' image_format ',' image_access_qualifier '>' {$$ = ConstantHelper::createImageType($3, $5, (uint8_t) $7, (uint8_t) $9, (uint8_t) $11, (uint8_t) $13, $15, $17);};
image_sampled_type: int_type {$$ = TypeInner {EType::Int, $1};}
| void_type {$$ = TypeInner {EType::Void, $1};};
image_dim: NUMLITERAL {$$ = ConstantHelper::dimFromLiteral($1);}
| IDENTIFIER {$$ = ConstantHelper::dimFromIdent($1);};
image_depth: NUMLITERAL {$$ = ConstantHelper::uintFromLiteral($1);};
image_arrayed: NUMLITERAL {$$ = ConstantHelper::uintFromLiteral($1);};
image_ms: NUMLITERAL {$$ = ConstantHelper::uintFromLiteral($1);};
image_sampled: NUMLITERAL {$$ = ConstantHelper::uintFromLiteral($1);};
image_format: IDENTIFIER {$$ = ConstantHelper::imageFormatFromIdent($1);};
image_access_qualifier: IDENTIFIER {$$ = ConstantHelper::accessQualifierFromIdent ($1);};

sampler_type: SAMPLER {$$ = SamplerType{};};

sampled_image_type: SAMPLED_IMAGE '<' image_type '>' {$$ = SampledImageType{TypeInner{EType::Image, $3}};};

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

uint64_t ConstantHelper::uintFromLiteral(const Literal &l)
{
	switch (l.type)
	{
		case LiteralType::INumber:
			return static_cast<uint64_t>(l.inum);
		case LiteralType::UNumber:
			return static_cast<uint64_t>(l.unum);
		default:
			// TODO: [OOKAMI] Throw error
			throw 0;
	}
}

int64_t ConstantHelper::intFromLiteral(const Literal &l)
{
	switch (l.type)
	{
		case LiteralType::INumber:
			return static_cast<int64_t>(l.inum);
		case LiteralType::UNumber:
			return static_cast<int64_t>(l.unum);
		default:
			// TODO: [OOKAMI] Throw error
			throw 0;
	}
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
				default:
					break;
			}
		default:
			break;
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
	if (ident == "Unknown")
		return spv::ImageFormat::Unknown;
	else if (ident == "Rgba32f")
		return spv::ImageFormat::Rgba32f;
	else if (ident == "Rgba16f")
		return spv::ImageFormat::Rgba16f;
	else if (ident == "R32f")
		return spv::ImageFormat::R32f;
	else if (ident == "Rgba8")
		return spv::ImageFormat::Rgba8;
	else if (ident == "Rgba8Snorm")
		return spv::ImageFormat::Rgba8Snorm;
	else if (ident == "Rg32f")
		return spv::ImageFormat::Rg32f;
	else if (ident == "Rg16f")
		return spv::ImageFormat::Rg16f;
	else if (ident == "R11fG11fB10f")
		return spv::ImageFormat::R11fG11fB10f;
	else if (ident == "R16f")
		return spv::ImageFormat::R16f;
	else if (ident == "Rgba16")
		return spv::ImageFormat::Rgba16;
	else if (ident == "Rgb10A2")
		return spv::ImageFormat::Rgb10A2;
	else if (ident == "Rg16")
		return spv::ImageFormat::Rg16;
	else if (ident == "Rg8")
		return spv::ImageFormat::Rg8;
	else if (ident == "R16")
		return spv::ImageFormat::R16;
	else if (ident == "R8")
		return spv::ImageFormat::R8;
	else if (ident == "Rgba16Snorm")
		return spv::ImageFormat::Rgba16Snorm;
	else if (ident == "Rg16Snorm")
		return spv::ImageFormat::Rg16Snorm;
	else if (ident == "Rg8Snorm")
		return spv::ImageFormat::Rg8Snorm;
	else if (ident == "R16Snorm")
		return spv::ImageFormat::R16Snorm;
	else if (ident == "R8Snorm")
		return spv::ImageFormat::R8Snorm;
	else if (ident == "Rgba32i")
		return spv::ImageFormat::Rgba32i;
	else if (ident == "Rgba16i")
		return spv::ImageFormat::Rgba16i;
	else if (ident == "Rgba8i")
		return spv::ImageFormat::Rgba8i;
	else if (ident == "R32i")
		return spv::ImageFormat::R32i;
	else if (ident == "Rg32i")
		return spv::ImageFormat::Rg32i;
	else if (ident == "Rg16i")
		return spv::ImageFormat::Rg16i;
	else if (ident == "Rg8i")
		return spv::ImageFormat::Rg8i;
	else if (ident == "R16i")
		return spv::ImageFormat::R16i;
	else if (ident == "R8i")
		return spv::ImageFormat::R8i;
	else if (ident == "Rgba32ui")
		return spv::ImageFormat::Rgba32ui;
	else if (ident == "Rgba16ui")
		return spv::ImageFormat::Rgba16ui;
	else if (ident == "Rgba8ui")
		return spv::ImageFormat::Rgba8ui;
	else if (ident == "R32ui")
		return spv::ImageFormat::R32ui;
	else if (ident == "Rgb10a2ui")
		return spv::ImageFormat::Rgb10a2ui;
	else if (ident == "Rg32ui")
		return spv::ImageFormat::Rg32ui;
	else if (ident == "Rg16ui")
		return spv::ImageFormat::Rg16ui;
	else if (ident == "Rg8ui")
		return spv::ImageFormat::Rg8ui;
	else if (ident == "R16ui")
		return spv::ImageFormat::R16ui;
	else if (ident == "R8ui")
		return spv::ImageFormat::R8ui;
	// TODO: [OOKAMI] Throw exception
	throw 0;
}

spv::AccessQualifier ConstantHelper::accessQualifierFromIdent(const std::string &ident)
{
	if (ident == "ReadOnly")
		return spv::AccessQualifier::ReadOnly;
	else if (ident == "WriteOnly")
		return spv::AccessQualifier::WriteOnly;
	if (ident == "ReadWrite")
		return spv::AccessQualifier::ReadWrite;
	// TODO: [OOKAMI] Throw exception
	throw 0;
}

IntType ConstantHelper::createIntType(uint8_t width, bool signedness)
{
	IntType t;
	t.width = width;
	t.signedness = signedness;
	return t;
}

FloatType ConstantHelper::createFloatType(uint8_t width)
{
	FloatType t;
	t.width = width;
	return t;
}

ImageType ConstantHelper::createImageType(TypeInner sampledType, spv::Dim dim,
	uint8_t isDepth, uint8_t isArrayed, uint8_t isMultisampled, uint8_t isSampled,
	spv::ImageFormat imageFormat, std::optional<spv::AccessQualifier> accessQualifier)
{
		ImageType itype;
		itype.sampledType = sampledType;
		itype.dimension = dim;
		itype.isDepth = isDepth;
		itype.isArrayed = isArrayed;
		itype.isMultisampled = isMultisampled;
		itype.isSampled = isSampled;
		itype.imageFormat = imageFormat;
		itype.accessQualifier = accessQualifier;
		return itype;
}

Type ConstantHelper::createType(const TypeModification &mod, TypeInner innerType, const TypeArraySuffix &arraySuffix, uint64_t pointerSuffixCount)
{
	Type t;
	t.isConst = mod.isConst;
	while (pointerSuffixCount)
	{
		innerType = TypeInner {EType::Pointer, PointerType{innerType}};
		pointerSuffixCount--;
	}
	if (arraySuffix.isArray)
		innerType = TypeInner {EType::Array, ArrayType{arraySuffix.isRuntime, arraySuffix.length, innerType}};
	t.innerType = innerType;
	return t;
}

Expression Op::simple(ExpressionType type, const ExpressionParams &params)
{
	Expression e;
	e.type = type;
	e.params = params;
	return e;
}

Expression Op::ident(const Identifier &ident)
{
	Expression e;
	e.type = ExpressionType::Identifier;
	e.ident = ident;
	return e;
}

Expression Op::literal(const Literal &literal)
{
	Expression e;
	e.type = ExpressionType::Literal;
	e.literal = literal;
	return e;
}

Expression Op::type(const Type &type)
{
	Expression e;
	e.type = ExpressionType::Type;
	e.castToType = type;
	return e;
}

Expression Op::group(const ExpressionParams &params)
{
	Expression e;
	e.type = ExpressionType::StatementGroup;
	e.params = params;
	return e;
}

Expression Op::nop()
{
	Expression e;
	e.type = ExpressionType::None;
	return e;
}

Expression Op::makeBreak()
{
	Expression e;
	e.type = ExpressionType::Break;
	return e;
}

Expression Op::makeContinue()
{
	Expression e;
	e.type = ExpressionType::Continue;
	return e;
}

Expression Op::makeReturn()
{
	Expression e;
	e.type = ExpressionType::Return;
	return e;
}

Expression Op::makeReturn(const Expression &expression)
{
	Expression e;
	e.type = ExpressionType::Return;
	e.params.push_back(expression);
	return e;
}

Expression Op::makeIf(const Expression &expression,
	const Expression &statement)
{
	Expression e;
	e.type = ExpressionType::If;
	e.params.push_back(expression);
	e.params.push_back(statement);
	return e;
}

Expression Op::makeIf(const Expression &varDecl,
	const Expression &expression, const Expression &statement)
{
	Expression e;
	e.type = ExpressionType::If;
	e.params.push_back(varDecl);
	e.params.push_back(expression);
	e.params.push_back(statement);
	return e;
}

Expression Op::makeIfElse(const Expression &expression,
	const Expression &statement, const Expression &statementElse)
{
	Expression e;
	e.type = ExpressionType::IfElse;
	e.params.push_back(expression);
	e.params.push_back(statement);
	e.params.push_back(statementElse);
	return e;
}

Expression Op::makeIfElse(const Expression &varDecl, const Expression &expression,
	const Expression &statement, const Expression &statementElse)
{
	Expression e;
	e.type = ExpressionType::IfElse;
	e.params.push_back(varDecl);
	e.params.push_back(expression);
	e.params.push_back(statement);
	e.params.push_back(statementElse);
	return e;
}

Expression Op::makeFor(const Expression &varDecl,
	const Expression &expression, const Expression &action,
	const Expression &statement)
{
	Expression e;
	e.type = ExpressionType::For;
	e.params.push_back(varDecl);
	e.params.push_back(expression);
	e.params.push_back(action);
	e.params.push_back(statement);
	return e;
}

Expression Op::makeWhile(const Expression &expression,
	const Expression &statement)
{
	Expression e;
	e.type = ExpressionType::While;
	e.params.push_back(expression);
	e.params.push_back(statement);
	return e;
}

Expression Op::makeDoWhile(const Expression &expression,
	const Expression &statement)
{
	Expression e;
	e.type = ExpressionType::DoWhile;
	e.params.push_back(statement);
	e.params.push_back(expression);
	return e;
}

Expression Op::makeSwitchCase(const Literal &literal, const Expression &expression)
{
	Expression e;
	e.type = ExpressionType::SwitchCase;
	e.literal = literal;
	e.params.push_back(expression);
	return e;
}

Expression Op::makeSwitchCaseDefault(const Expression &expression)
{
	Expression e;
	e.type = ExpressionType::SwitchDefault;
	e.params.push_back(expression);
	return e;
}

Expression Op::makeSwitch(const Expression &expression, const std::vector<Expression> &body)
{
	Expression e;
	e.type = ExpressionType::Switch;
	e.params.push_back(expression);
	e.params.insert(e.params.end(), body.begin(), body.end());
	return e;
}
Expression Op::makeSwitch(const Expression &varDecl, const Expression &expression, const std::vector<Expression> &body)
{
	Expression e;
	e.type = ExpressionType::Switch;
	e.params.push_back(varDecl);
	e.params.push_back(expression);
	e.params.insert(e.params.end(), body.begin(), body.end());
	return e;
}

// AST Container methods

std::vector<struct Struct> Context::getStructs() const
{
	return structs;
}

std::vector<struct GlobalVariable> Context::getGlobalVariables() const
{
	return globalVariables;
}

std::vector<struct Function> Context::getFunctions() const
{
	return functions;
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

Identifier Context::use(std::string name)
{
	for (auto it = scopes.rbegin(); it != scopes.rend(); it++)
	{
		auto findResult = it->identifiers.find(name);
		if (findResult == it->identifiers.end())
			continue;
		return findResult->second;
	}
	return {};
}

Struct &Context::defineStruct(const std::string &name)
{
	if (getIdentifierType(name) != IdentifierType::Undefined)
		throw 0; // TODO: [OOKAMI] throw error
	Identifier t;
	t.type = IdentifierType::Structure;
	t.name = name;
	auto id = pushIdentifierToScope(t);
	Struct s;
	s.name = name;
	s.attributes = getAndClearTempAttributes();
	s.id = id;
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

void Context::defineGlobalVariables(Type t, const VarNamesAndInits &vars)
{
	for (auto &var : vars)
	{
		Identifier ident;
		ident.name = var.first;
		ident.type = IdentifierType::Variable;
		auto id = pushIdentifierToScope(ident);
		GlobalVariable v;
		v.id = id;
		v.type = t;
		v.name = var.first;
		v.attributes = getAndClearTempAttributes();
		v.initialization = var.second;
		globalVariables.push_back(v);
	}
}

Expression Context::defineLocalVariables(Type t, const VarNamesAndInits &vars)
{
	Expression ex;
	ex.type = ExpressionType::VariableDeclaration;
	for (auto &var : vars)
	{
		Identifier ident;
		ident.name = var.first;
		ident.type = IdentifierType::Variable;
		auto id = pushIdentifierToScope(ident);
		LocalVariable v;
		v.id = id;
		v.type = t;
		v.name = var.first;
		v.initialization = var.second;
		ex.declaredVariables.push_back(v);
	}
	return ex;
}

FunctionParameter Context::defineFunctionParameter(Type t, const std::string &name)
{
	int64_t id = -1;
	if (!name.empty())
	{
		Identifier ident;
		ident.name = name;
		ident.type = IdentifierType::Variable;
		id = pushIdentifierToScope(ident);
	}
	FunctionParameter p;
	p.id = id;
	p.type = t;
	p.name = name;
	return p;
}

void Context::defineFunctionPrototype(const FunctionDeclaration &decl)
{
	Identifier ident;
	ident.name = decl.name;
	ident.type = IdentifierType::Function;
	pushIdentifierToScope(ident);
	getOrDefineFunction(decl);
}

void Context::defineFunction(const FunctionDeclaration &decl, const Expression &body)
{
	Identifier ident;
	ident.name = decl.name;
	ident.type = IdentifierType::Function;
	pushIdentifierToScope(ident);
	auto &func = getOrDefineFunction(decl);
	//TODO: [OOKAMI] if body already exists - exception
	func.body = body;
}

void Context::operator++()
{
	scopes.emplace_back();
}

void Context::operator--()
{
	scopes.pop_back();
}

int64_t Context::pushIdentifierToScope(Identifier id)
{
	static int counter = 0;
	// Functions can be overloaded.
	//So compiler decide which function to call
	if (id.type != IdentifierType::Function)
		id.id = counter++;
	else
		id.id = -1;
	auto &topScope = scopes.back();
	if (id.type == IdentifierType::Function)
	{
		if (topScope.identifiers.find(id.name) == topScope.identifiers.end())
			topScope.identifiers.insert(std::make_pair(id.name, id));
	}
	else
		topScope.identifiers.insert(std::make_pair(id.name, id));
	return id.id;
}

std::vector<struct Function*> Context::getFunctionsByName(std::string name)
{
	std::vector<struct Function*> findedFunctions;
	for (auto &func : functions)
	{
		if (func.name == name)
			findedFunctions.push_back(&func);
	}
	return findedFunctions;
}

Function &Context::getOrDefineFunction(const FunctionDeclaration &decl)
{
	auto functionsByName = getFunctionsByName(decl.name);
	bool createNew = false;
	if (functionsByName.empty())
		createNew = true;
	else
	{
		auto lambdaRemove = [&decl](Function *func) -> bool
		{
			if (func->parameters.size() != decl.parameters.size())
				return true;
			for (size_t i = 0; i < func->parameters.size(); i++)
			{
				const auto &fParam = func->parameters[i];
				const auto &dParam = decl.parameters[i];
				if (fParam.type != dParam.type)
					return true;
			}
			return false;
		};
		//remove with different params
		auto iterDelete = std::remove_if(functionsByName.begin(), functionsByName.end(), lambdaRemove);
		functionsByName.erase(iterDelete, functionsByName.end());
		if (functionsByName.size() > 1)
			throw 0; // TODO: [OOKAMI] Throw normal exception
		else if (functionsByName.size() == 1)
		{
			auto findedFunc = functionsByName.back();
			if (findedFunc->returnType == decl.returnType)
				return *findedFunc;
			// TODO: [OOKAMI] Throw normal exception
			throw 0;
		}
		else
			createNew = true;
	}
	if (createNew)
	{
		Function f;
		f.returnType = decl.returnType;
		f.name = decl.name;
		f.parameters = decl.parameters;
		functions.push_back(f);
		return functions.back();
	}
	// TODO: [OOKAMI] Throw normal exception
	throw 0;
}

void gen::BlastParser::error(const gen::location &loc, const std::string &msg)
{
	callback(loc, msg);
}

bool operator==(const TypeInner &t1, const TypeInner &t2)
{
	if (t1.etype != t2.etype)
		return false;
	switch (t1.etype)
	{
		case EType::Int:
		{
			auto i1 = std::any_cast<IntType>(t1.innerType);
			auto i2 = std::any_cast<IntType>(t2.innerType);
			return i1.width == i2.width && i1.signedness == i2.signedness;
		}
		case EType::Float:
		{
			auto f1 = std::any_cast<FloatType>(t1.innerType);
			auto f2 = std::any_cast<FloatType>(t2.innerType);
			return f1.width == f2.width;
		}
		case EType::Bool:
		case EType::Void:
			return true;
		case EType::Matrix:
		{
			auto m1 = std::any_cast<MatrixType>(t1.innerType);
			auto m2 = std::any_cast<MatrixType>(t2.innerType);
			return m1.rowsCount == m2.rowsCount &&
				m1.columnsCount == m2.columnsCount &&
				m1.componentType == m2.componentType;
		}
		case EType::Vector:
		{
			auto v1 = std::any_cast<VectorType>(t1.innerType);
			auto v2 = std::any_cast<VectorType>(t2.innerType);
			return v1.componentCount == v2.componentCount &&
				v1.componentType == v2.componentType;
		}
		case EType::Image:
		{
			auto i1 = std::any_cast<ImageType>(t1.innerType);
			auto i2 = std::any_cast<ImageType>(t2.innerType);
			return i1.sampledType == i2.sampledType &&
				i1.dimension == i2.dimension &&
				i1.isDepth == i2.isDepth &&
				i1.isArrayed == i2.isArrayed &&
				i1.isMultisampled == i2.isMultisampled &&
				i1.isSampled == i2.isSampled &&
				i1.imageFormat == i2.imageFormat &&
				i1.accessQualifier == i2.accessQualifier;
		}
		case EType::Sampler:
			return true;
		case EType::SampledImage:
		{
			auto si1 = std::any_cast<SampledImageType>(t1.innerType);
			auto si2 = std::any_cast<SampledImageType>(t2.innerType);
			return si1.innerType == si2.innerType;
		}
		case EType::Pointer:
		{
			auto p1 = std::any_cast<PointerType>(t1.innerType);
			auto p2 = std::any_cast<PointerType>(t2.innerType);
			return p1.innerType == p2.innerType;
		}
		case EType::Array:
		{
			auto a1 = std::any_cast<ArrayType>(t1.innerType);
			auto a2 = std::any_cast<ArrayType>(t2.innerType);
			return a1.isRuntime == a2.isRuntime &&
				a1.length == a2.length &&
				a1.innerType == a2.innerType;
		}
		case EType::Struct:
		{
			auto s1 = std::any_cast<TypeStruct>(t1.innerType);
			auto s2 = std::any_cast<TypeStruct>(t2.innerType);
			return s1.name == s2.name;
		}
	}
	return false;
}

bool operator!=(const TypeInner &t1, const TypeInner &t2)
{
	return !(t1 == t2);
}

bool operator==(const Type &t1, const Type &t2)
{
	return t1.isConst == t2.isConst && t1.innerType == t2.innerType;
}

bool operator!=(const Type &t1, const Type &t2)
{
	return !(t1 == t2);
}
