#include "parser_expression_border_state.hpp"
#include "parser_expression_stack.hpp"
#include <stdexcept>

ExpressionBorderState::ExpressionBorderState(EExpressionLevelType expressionLevelType, EExpressionBorderType borderType,
	std::shared_ptr<CommonParser> parser, std::shared_ptr<ExpressionStack> stack) :
	ConditionalParserState(getStateByBorderType(expressionLevelType, borderType)),
	expressionLevelType_(expressionLevelType), expressionBorderType_(borderType), parser_(parser), stack_(stack)
{
}

bool ExpressionBorderState::canActivate() const
{
	switch (expressionBorderType_)
	{
	case EExpressionBorderType::Open:
		return stack_->canOpen(expressionLevelType_);
	case EExpressionBorderType::Close:
		return stack_->canClose(expressionLevelType_);
	};
	return false;
}

void ExpressionBorderState::activate()
{
	switch (expressionBorderType_)
	{
	case EExpressionBorderType::Open:
		stack_->open(expressionLevelType_);
		break;
	case EExpressionBorderType::Close:
		stack_->close(expressionLevelType_);
		break;
	};
}

std::vector<std::shared_ptr<CommonParser>> ExpressionBorderState::getParsers() const
{
	return { parser_ };
}

std::optional<EParserState> ExpressionBorderState::getNextJumpState() const
{
	if (expressionBorderType_ == EExpressionBorderType::Close)
	{
		auto levelTop = stack_->top();
		switch (levelTop)
		{
		case EExpressionLevelType::None:
			return EParserState::Global;
		}
		throw std::runtime_error("Cannot determinate the next jump state");
	}
		
	return {};
}

std::vector<EParserState> ExpressionBorderState::getNextAvailableStates() const
{
	if (expressionBorderType_ == EExpressionBorderType::Close)
		return {};
	switch (expressionLevelType_)
	{
	case EExpressionLevelType::Function:
		return {EParserState::FunctionBodyEnd};
	}
	return {};
}

EParserState getStateByBorderType(EExpressionLevelType expressionLevelType, EExpressionBorderType borderType)
{
	switch (expressionLevelType)
	{
	case EExpressionLevelType::Function:
		switch (borderType)
		{
		case EExpressionBorderType::Open:
			return EParserState::FunctionBodyStart;
		case EExpressionBorderType::Close:
			return EParserState::FunctionBodyEnd;
		}
	}
	throw std::runtime_error("Unknow expression type");
}