#ifndef PARSER_EXPRESSION_STACK
#define PARSER_EXPRESSION_STACK

#include <stack>
#include <memory>
#include "parser_expression_level_enum.hpp"

class ExpressionStack :
	public std::enable_shared_from_this<ExpressionStack>
{
public:
	bool canOpen(EExpressionLevelType type) const;
	void open(EExpressionLevelType type);
	bool canClose(EExpressionLevelType type) const;
	void close(EExpressionLevelType type);
	EExpressionLevelType top() const;
private:
	std::stack<EExpressionLevelType> stack_;
};

#endif