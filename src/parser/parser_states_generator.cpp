#include "parser_states_generator.hpp"
#include "parser_states.hpp"
#include "parsers.hpp"

#include <functional>
#include <algorithm>
#include <stack>
#include <stdexcept>
#include <cstdint>


enum class EExpressionLevelType
{
	Global,
	Class,
	Function,
	Local,
};

class ExpressionStack :
	public std::enable_shared_from_this<ExpressionStack>
{
public:
	bool canOpen(EExpressionLevelType type) const
	{

	}
	size_t open(EExpressionLevelType type)
	{
		if (!canOpen(type))
			throw std::runtime_error("Cannot open expression!");
		stack_.push(type);
		return stack_.size() - 1;
	}
	bool canClose(EExpressionLevelType type, size_t num)
	{
		if (stack_.top() == type && num == stack_.size() - 1)
			return true;
		return false;
	}
	void close(EExpressionLevelType type, size_t num)
	{
		if (!canClose(type, num))
			throw std::runtime_error("Cannot close expression!");
		stack_.pop();
	}
private:
	std::stack<EExpressionLevelType> stack_;
};

//TODO: [OOKAMI] Refactor with new enum

class BodyExpressionLevel :
	public std::enable_shared_from_this<BodyExpressionLevel>
{
public:
	uint64_t current() const
	{
		return level_;
	}
	void operator++()
	{
		level_++;
	}
	void operator--()
	{
		level_--;
	}
private:
	uint64_t level_ = 0;
};

class FunctionBodyBorderState :
	public ConditionalParserState
{
public:
	enum class EBorderType
	{
		Open,
		Close
	};
public:
	FunctionBodyBorderState(
		EBorderType borderType, std::shared_ptr<BodyExpressionLevel> bodyExpressionLevel,
		std::shared_ptr<uint64_t> levelOpen, std::shared_ptr<CommonParser> parser) :
			ConditionalParserState(borderType == EBorderType::Close ? EParserState::FunctionBodyStart : EParserState::FunctionBodyEnd),
			borderType_(borderType),
			bodyExpressionLevel_(bodyExpressionLevel),
			levelOpen_(levelOpen),
			parser_(parser)
	{

	}
	bool canActivate() const override
	{
		if (borderType_ == EBorderType::Open)
			return true;
		if (*levelOpen_ == bodyExpressionLevel_->current())
			return true;
		return false;
	}
	void activate() override
	{
		switch (borderType_)
		{
		case EBorderType::Open:
			(*bodyExpressionLevel_)++;
			*levelOpen_ = bodyExpressionLevel_->current();
			break;
		case EBorderType::Close:
			*levelOpen_ = 0;
			(*bodyExpressionLevel_)--;
			break;
		}
	}
	std::vector<std::shared_ptr<CommonParser>> getParsers() const override
	{
		return {parser_};
	}
	std::optional<EParserState> getNextJumpState() const override
	{
		if (borderType_ == EBorderType::Close)
			return EParserState::Global;
	}
	std::vector<EParserState> getNextAvailableStates() const override
	{

	}
private:
	const EBorderType borderType_;
	const std::shared_ptr<BodyExpressionLevel> bodyExpressionLevel_;
	const std::shared_ptr<uint64_t> levelOpen_;
	const std::shared_ptr<CommonParser> parser_;
};

std::vector<std::shared_ptr<IParserState>> generateStates()
{
	std::vector<std::shared_ptr<IParserState>> states;

	auto bodyExpressionLevel = std::make_shared<BodyExpressionLevel>();
	auto functionBodyLevel = std::make_shared<uint64_t>(0);

	auto basicTypeParser = std::make_shared<BasicBlastTypeParser>();
	auto nameParser = std::make_shared<BlastNameParser>();
	auto expressionEndParser = std::make_shared<BlastExpressionEndParser>();

	auto functionParamtersStart = std::make_shared<BlastFunctionParametersStartParser>();
	auto functionParamtersSeparator = std::make_shared<BlastFunctionParametersSeparatorParser>();
	auto functionParamtersEnd = std::make_shared<BlastFunctionParametersEndParser>();

	auto expressionBodyStart = std::make_shared<BlastExpressionBodyStartParser>();
	auto expressionBodyEnd = std::make_shared<BlastExpressionBodyEndParser>();

	auto simpleStates = std::move(std::vector<SimpleParserState>
	{
		{ EParserState::Global, { expressionEndParser },
		{ EParserState::GlobalType }
		},
		{ EParserState::GlobalType, { basicTypeParser },
			{EParserState::GlobalName}
		},
		{ EParserState::GlobalName, { nameParser },
			{EParserState::Global, EParserState::FunctionParametersStart}
		},
		{ EParserState::FunctionParametersStart, {functionParamtersStart},
			{ EParserState::FunctionParameterType, EParserState::FunctionParametersEnd}
		},
		{ EParserState::FunctionParameterType, { basicTypeParser },
			{ EParserState::FunctionParameterName, EParserState::FunctionParametersSeparator, EParserState::FunctionParametersEnd }
		},
		{ EParserState::FunctionParameterName, {nameParser},
			{ EParserState::FunctionParametersSeparator, EParserState::FunctionParametersEnd }
		},
		{ EParserState::FunctionParametersSeparator, {functionParamtersSeparator},
			{EParserState::FunctionParameterType}
		},
		{ EParserState::FunctionParametersEnd, {functionParamtersEnd},
			{ EParserState::Global, EParserState::FunctionBodyStart }
		},
		{ EParserState::FunctionBodyStart, {expressionBodyStart},
			{EParserState::FunctionBodyEnd}
		},
		{ EParserState::FunctionBodyEnd, {expressionBodyEnd},
			{EParserState::Global}
		}
	});

	std::transform(simpleStates.begin(), simpleStates.end(), std::back_inserter(states), [](SimpleParserState &simpleState) -> std::shared_ptr<IParserState>
	{
		return std::make_shared<SimpleParserState>(simpleState);
	});

	return std::move(states);
}