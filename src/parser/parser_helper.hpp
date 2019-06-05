#ifndef PARSER_HELPER
#define PARSER_HELPER

#include "parser_types.hpp"

class Attributes
{
public:
	static Attribute makeAttribute(const std::string &ident, std::vector<AttributeParam> params);
	static AttributeParam paramFromIdentifier(const std::string &ident);
	static AttributeParam paramFromLiteral(const Literal &l);
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

#endif