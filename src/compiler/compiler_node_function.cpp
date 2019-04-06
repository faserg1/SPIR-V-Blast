#include "compiler_node_function.hpp"

std::shared_ptr<CompilerNodeType> CompilerNodeFunction::getType() const
{
	return type_;
}

std::string CompilerNodeFunction::getName() const
{
	return name_;
}

std::vector<std::shared_ptr<CompilerNodeFunctionParameter>> CompilerNodeFunction::getParameters() const
{
	return parameters_;
}

std::shared_ptr<CompilerNodeFunctionBody> CompilerNodeFunction::getBody() const
{
	return body_;
}
