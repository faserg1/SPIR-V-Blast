#ifndef PARSER_EXPRESSION_BORDER_STATE
#define PARSER_EXPRESSION_BORDER_STATE

#include <memory>
#include "parser_states.hpp"
#include "parser_expression_level_enum.hpp"

enum class EExpressionBorderType
{
	Open,
	Close
};

class CommonParser;
class ExpressionStack;

class ExpressionBorderState :
	public std::enable_shared_from_this<ExpressionBorderState>,
	public ConditionalParserState
{
public:
	ExpressionBorderState(EExpressionLevelType expressionLevelType, EExpressionBorderType borderType,
		std::shared_ptr<CommonParser> parser, std::shared_ptr<ExpressionStack> stack);
	bool canActivate() const override;
	void activate() override;
	std::vector<std::shared_ptr<CommonParser>> getParsers() const override;
	std::optional<EParserState> getNextJumpState() const override;
	std::vector<EParserState> getNextAvailableStates() const override;
private:
	const EExpressionLevelType expressionLevelType_;
	const EExpressionBorderType expressionBorderType_;
	const std::shared_ptr<CommonParser> parser_;
	const std::shared_ptr<ExpressionStack> stack_;
private:
	friend EParserState getStateByBorderType(EExpressionLevelType expressionLevelType, EExpressionBorderType borderType);
};

#endif