#include "parser_expression_stack.hpp"

bool ExpressionStack::canOpen(EExpressionLevelType type) const
{
	if (stack_.empty())
	{
		switch (type)
		{
		case EExpressionLevelType::Class:
		case EExpressionLevelType::Function:
			return true;
		}
	}
	const auto top = stack_.top();
	// TODO: [OOKAMI] Add more
	switch (top)
	{
	case EExpressionLevelType::Global:
		break;
	}
	return false;
}

void ExpressionStack::open(EExpressionLevelType type)
{
	if (!canOpen(type))
		throw std::runtime_error("Cannot open expression!");
	stack_.push(type);
}

bool ExpressionStack::canClose(EExpressionLevelType type) const
{
	if (stack_.empty())
		return false;
	if (stack_.top() == type)
		return true;
	return false;
}

void ExpressionStack::close(EExpressionLevelType type)
{
	if (!canClose(type))
		throw std::runtime_error("Cannot close expression!");
	stack_.pop();
}

EExpressionLevelType ExpressionStack::top() const
{
	if (stack_.empty())
		return EExpressionLevelType::None;
	return stack_.top();
}
