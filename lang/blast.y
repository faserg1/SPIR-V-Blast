// Use the default C++ style
%skeleton "lalr1.cc"
//Set the C++ language
%language "c++"
// Define the ParserClass
%define api.parser.class {BlastParser}
// Define namespace
%define api.namespace {gen}
// Request that symbols be handled as a whole (type, value, and possibly location) in the scanner.
// Used only when `api.value.type` is `variant`
// Generate methods MAKE_<symbol>... but... it wont work in GLR
%define api.token.constructor
// Use the default variant type as semantic type
%define api.value.type variant
// Enable locations!
%locations
//Enabling the GLR parser
//%glr-parser

%code top
{

} // %code top

%code requires
{
#include <functional>
#include "../parser/parser_types.hpp"
#include "../parser/parser_lex_context.hpp"
#include "../parser/parser_abstract_syntax_tree_container.hpp"

namespace gen
{
	class location;
	class BlastScanner;
}
class Context;

typedef std::function<void(const gen::location &, const std::string &)> ErrorCallback;
} //%code requires

%param { gen::BlastScanner *scanner }
%lex-param { LexContext &ctx }
%parse-param { ErrorCallback callback }
%parse-param { Context &ctx }

%code
{
gen::BlastParser::symbol_type yylex(gen::BlastScanner *scanner, LexContext &ctx);

#include "../parser/parser_helper.hpp"
#include "../parser/parser_context.hpp"

} //%code

%token END 0
%token DO "do" WHILE "while" IF "if" ELSE "else" SWITCH "switch" FOR "for"
%token DEFAULT "default" CASE "case"
%token RETURN "return" BREAK "break" CONTINUE "continue"
%token CONST "const"
%token C_TRUE "true" C_FALSE "false"
%token BITCAST "bit_cast"
%token VOID "void" INT "int" UINT "uint" FLOAT "float" BOOL "bool"
%token MATRIX "mat" VECTOR "vec"
%token IMAGE "img" SAMPLER "smpl" SAMPLED_IMAGE "simg"
%token STRUCT "struct" ENUM "enum" CLASS "class"
%token IDENTIFIER STRUCT_TYPE CLASS_TYPE ENUM_TYPE
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

%type<Literal> literal enum_use NUMLITERAL STRINGLITERAL
%type<std::string> IDENTIFIER STRUCT_TYPE ENUM_TYPE
%type<Struct> struct_a struct
%type<std::vector<struct StructMember>> struct_body struct_members_continious struct_member_a struct_member
%type<std::vector<std::string>> struct_member_continious
%type<Type> type
%type<spv::Dim> image_dim
%type<spv::ImageFormat> image_format
%type<spv::AccessQualifier> image_access_qualifier
%type<Attribute> attribute attribute_body
%type<AttributeParam> attribute_param
%type<std::vector<AttributeParam>> attribute_params_rec
%type<uint8_t> type_pointer_suffix
%type<uint64_t> image_depth image_arrayed image_ms image_sampled
%type<bool> boolean_const_expr
%type<TypeModification> type_mod
%type<TypeArraySuffix> type_array_suffix
%type<TypeInner> type_variant user_defined_type simple_type complex_type_variant image_sampled_type
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
%type<TypeInner> enum_base_opt
%type<EnumDecls> enum_body enum_ident_rec
%type<EnumDecl> enum_ident
%type<VariableInitialization> var_init
%type<CompositeInitialization> var_init_rec

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

enum: ENUM IDENTIFIER enum_base_opt '{' enum_body '}' ';' {ctx.defineEnum($3, $2, $5);};

enum_base_opt: ':' int_type {$$ = TypeInner {EType::Int, $2};}
| %empty {$$ = TypeInner {EType::Int, ConstantHelper::createIntType(32, true)};};

enum_body: enum_ident_rec
| %empty {$$ = {};};

enum_ident_rec: enum_ident_rec ',' enum_ident {auto map = $1; map.insert($3); $$ = map; ctx.incrementEnumCounter();}
| {ctx.resetEnumCounter();} enum_ident {$$ = {$2}; ctx.incrementEnumCounter();};

enum_ident: IDENTIFIER {$$ = std::make_pair($1, ctx.getEnumCounter());}
| IDENTIFIER '=' NUMLITERAL {$$ = std::make_pair($1, $3); ctx.setEnumCounter($3);};

/* EXPRESSIONS */

comma_expression: expression {$$ = Op::simple(ExpressionType::Comma, {$1});}
| comma_expression ',' expression {auto c = $1; c.params.push_back($3); $$ = c;};

expression: IDENTIFIER {$$ = Op::ident(ctx.use($1));}
| literal {$$ = Op::literal($1);}
| enum_use {$$ = Op::literal($1);}
| '(' comma_expression ')' {$$ = $2;}
| expression '(' ')' {$$ = Op::simple(ExpressionType::FunctionCall, {$1});}
| expression '(' comma_expression ')' {$$ = Op::simple(ExpressionType::FunctionCall, {$1, $3});}
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

enum_use: SCOPE_RESOLVE ENUM_TYPE SCOPE_RESOLVE IDENTIFIER {$$ = ctx.resolveEnum($2, $4);};

/* VARIABLE DECLARATION */

var_def_a: var_def {ctx.defineGlobalVariables($1.first, $1.second);}
| attribute_rec var_def {ctx.defineGlobalVariables($2.first, $2.second);};

var_def: type var_def_continious {$$ = std::make_pair($1, $2);};

var_def_continious: var_def_continious ',' IDENTIFIER {auto prev = $1; prev.push_back(std::make_pair($3, std::optional<VariableInitialization>())); $$ = prev;}
| var_def_continious ',' IDENTIFIER '=' var_init {auto prev = $1; prev.push_back(std::make_pair($3, $5)); $$ = prev;}
| IDENTIFIER {$$ = {std::make_pair($1, std::optional<VariableInitialization>())};}
| IDENTIFIER '=' var_init {$$ = {std::make_pair($1, $3)};};

var_init: expression { $$ = {$1};}
| '{' '}' {$$ = {};}
| '{' var_init_rec '}' {$$ = {$2};};

var_init_rec: var_init {$$ = {$1};}
| var_init_rec ',' var_init {auto vec = $1; vec.push_back($3); $$ = std::move(vec);};

/* TYPES */

type: type_mod type_variant type_array_suffix type_pointer_suffix {$$ = ConstantHelper::createType($1, $2, $3, $4);};

type_pointer_suffix: type_pointer_suffix '*' {$$ = $1 + 1;}
| %empty {$$ = 0;};

type_array_suffix: '[' NUMLITERAL ']' {$$ = TypeArraySuffix{true, false, ConstantHelper::uintFromLiteral($2), {}};}
| '[' IDENTIFIER ']' {$$ = TypeArraySuffix{true, false, 0, ctx.use($2)};}
| '[' ']' {$$ = TypeArraySuffix{true, true, 0, {}};}
| %empty {$$ = TypeArraySuffix{false, false, 0, {}};};

type_mod: CONST {$$ = {true};}
| %empty {$$ = {};};

type_variant: simple_type
| user_defined_type
| complex_type_variant;

user_defined_type: STRUCT_TYPE {$$ = TypeInner {EType::Struct, TypeStruct{$1}};}
| ENUM_TYPE {$$ = TypeInner {EType::Enum, TypeEnum{$1}};};

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
| UINT {$$ = ConstantHelper::createIntType(32, false);}
| INT '<' '>'{$$ = ConstantHelper::createIntType(32, true);}
| UINT '<' '>'{$$ = ConstantHelper::createIntType(32, false);}
| INT '<' NUMLITERAL '>'{$$ = ConstantHelper::createIntType((uint8_t) ConstantHelper::uintFromLiteral($3), true);}
| UINT '<' NUMLITERAL '>' {$$ = ConstantHelper::createIntType((uint8_t) ConstantHelper::uintFromLiteral($3), false);}
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

attribute_body: IDENTIFIER {$$ = Attributes::makeAttribute($1, {});}
| IDENTIFIER ':' attribute_params_rec {$$ = Attributes::makeAttribute($1, std::move($3));};

attribute_params_rec: attribute_params_rec ',' attribute_param {auto params = $1; params.push_back($3); $$ = params;}
| attribute_param {$$ = {$1};};

attribute_param: IDENTIFIER {$$ = Attributes::paramFromIdentifier($1);}
| STRINGLITERAL {$$ = Attributes::paramFromLiteral($1);}
| NUMLITERAL {$$ = Attributes::paramFromLiteral($1);};

%%

void gen::BlastParser::error(const gen::location &loc, const std::string &msg)
{
	callback(loc, msg);
}
