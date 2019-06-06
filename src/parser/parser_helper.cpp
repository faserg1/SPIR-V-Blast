#include "parser_helper.hpp"
#include "../gen/spirv_enums.hpp"

Attribute Attributes::makeAttribute(const std::string &ident, std::vector<AttributeParam> params)
{
	Attribute attr;
	attr.name = ident;
	attr.params = params;
	return attr;
}

AttributeParam Attributes::paramFromIdentifier(const std::string &ident)
{
	AttributeParam param;
	param.paramIdent = ident;
	param.type = AttributeParamType::Identifier;
	return param;
}

AttributeParam Attributes::paramFromLiteral(const Literal &l)
{
	AttributeParam param;
	param.type = AttributeParamType::Literal;
	param.paramLiteral = l;
	return param;
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
	return fromString<spv::Dim>(ident);
}

spv::ImageFormat ConstantHelper::imageFormatFromIdent(const std::string &ident)
{
	return fromString<spv::ImageFormat>(ident);
}

spv::AccessQualifier ConstantHelper::accessQualifierFromIdent(const std::string &ident)
{
	return fromString<spv::AccessQualifier>(ident);
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
		innerType = TypeInner{ EType::Pointer, PointerType{innerType} };
		pointerSuffixCount--;
	}
	if (arraySuffix.isArray)
		innerType = TypeInner{ EType::Array, ArrayType{arraySuffix.isRuntime, arraySuffix.length, innerType} };
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