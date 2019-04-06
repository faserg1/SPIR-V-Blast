#include "compiler_state_generator.hpp"
#include "compiler_state.hpp"
#include "../parser/parser_nodes.hpp"
#include <stdexcept>

class CompilerStateNone :
	public CompilerState
{
public:
	CompilerStateNone() :
		CompilerState(ECompilerState::None)
	{

	}
	ECompilerState getNextState(std::shared_ptr<ParserNode> node) const override
	{
		switch (node->getNodeType())
		{
		case EParserNodeType::Type:
			return ECompilerState::TypeDeclaration;
		}
		CompilerState::getNextState(node);
	}
};

class CompilerStateTypeDelaration :
	public CompilerState
{
public:
	CompilerStateTypeDelaration() :
		CompilerState(ECompilerState::TypeDeclaration)
	{

	}
	ECompilerState getNextState(std::shared_ptr<ParserNode> node) const override
	{
		switch (node->getNodeType())
		{
		case EParserNodeType::Name:
			return ECompilerState::NameDeclaration;
		}
		CompilerState::getNextState(node);
	}
};

class CompilerStateNameDelaration :
	public CompilerState
{
public:
	CompilerStateNameDelaration() :
		CompilerState(ECompilerState::TypeDeclaration)
	{

	}
	ECompilerState getNextState(std::shared_ptr<ParserNode> node) const override
	{
		switch (node->getNodeType())
		{
		case EParserNodeType::Expression:
			switch (node->getParserState())
			{
			case EParserState::FunctionParametersStart:
				return ECompilerState::FunctionDeclaration;
			}
		}
		CompilerState::getNextState(node);
	}
};

std::vector<std::shared_ptr<CompilerState>> CompilerStateGenerator::generateStates()
{
	return std::vector<std::shared_ptr<CompilerState>>();
}
