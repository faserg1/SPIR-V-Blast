#ifndef COMPILER_NODE_FUNCTION
#define COMPILER_NODE_FUNCTION

#include "compiler_node.hpp"
#include <string>
#include <vector>

class CompilerNodeType;
class CompilerNodeFunctionParameter;
class CompilerNodeFunctionBody;

class CompilerNodeFunction :
	public CompilerNode
{
public:
	std::shared_ptr<CompilerNodeType> getType() const;
	std::string getName() const;
	std::vector<std::shared_ptr<CompilerNodeFunctionParameter>> getParameters() const;
	std::shared_ptr<CompilerNodeFunctionBody> getBody() const;
private:
	std::shared_ptr<CompilerNodeType> type_;
	std::string name_;
	std::vector<std::shared_ptr<CompilerNodeFunctionParameter>> parameters_;
	std::shared_ptr<CompilerNodeFunctionBody> body_;
};

#endif // COMPILER_NODE_FUNCTION