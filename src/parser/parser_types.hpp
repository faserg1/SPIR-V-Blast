#ifndef PARSER_TYPES
#define PARSER_TYPES

#include <string>
#include <vector>
#include <optional>
#include <any>
#include <map>

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
	Struct,
	Enum
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

struct TypeEnum
{
	std::string name;
};

struct PointerType
{
	TypeInner innerType;
	uint32_t storageClass;
};

struct ArrayType
{
	bool isRuntime;
	uint64_t length;
	Identifier lengthId;
	TypeInner innerType;
};

struct TypeArraySuffix
{
	bool isArray;
	bool isRuntime;
	uint64_t length;
	Identifier lengthId;
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
	uint32_t storageClass;
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

struct AttributeParam
{
	AttributeParamType type;
	std::string paramIdent{};
	struct Literal paramLiteral {};
};

struct Attribute
{
	std::string name;
	std::vector<AttributeParam> params;
};

struct Attributable
{
	std::vector<struct Attribute> attributes;
};

typedef std::vector<struct Expression> ExpressionParams;
typedef std::vector<struct LocalVariable> LocalVariables;

struct Expression
{
	ExpressionType type;
	Identifier ident{};
	Literal literal{};
	Type castToType{};
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
	int64_t id;
};

struct Function : FunctionDeclaration, Attributable
{
	std::optional<struct Expression> body;
};

struct FunctionParameter : BaseVariable
{
	std::size_t position; //position in parameters list
};

using EnumDecls = std::map<std::string, Literal>;
using EnumDecl = std::pair<std::string, Literal>;

struct Enum
{
	struct Type baseType;
	std::string name;
	EnumDecls enumDeclarations;
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

struct ConstExpression
{
	struct Type type;
	struct Literal literal;
};

using VarNamesAndInits =
std::vector<std::pair<std::string, std::optional<Expression>>>;
using Vars = std::pair<Type, VarNamesAndInits>;

bool operator==(const Type &t1, const Type &t2);
bool operator!=(const Type &t1, const Type &t2);
bool operator==(const TypeInner &t1, const TypeInner &t2);
bool operator!=(const TypeInner &t1, const TypeInner &t2);
bool operator<(const TypeInner &t1, const TypeInner &t2);
bool operator<(const Literal &l1, const Literal &l2);
bool operator<(const ConstExpression &e1, const ConstExpression &e2);

#endif