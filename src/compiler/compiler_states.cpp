#include "compiler_states.hpp"

CompilerStateNone::CompilerStateNone() :
	CompilerState(ECompilerState::None)
{

}

ECompilerState CompilerStateNone::getNextState(std::shared_ptr<ParserNode> node) const
{
	switch (node->getNodeType())
	{
	case EParserNodeType::Type:
		return ECompilerState::TypeDeclaration;
	}
	CompilerState::getNextState(node);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

CompilerStateTypeDelaration::CompilerStateTypeDelaration() :
	CompilerState(ECompilerState::TypeDeclaration)
{

}

ECompilerState CompilerStateTypeDelaration::getNextState(std::shared_ptr<ParserNode> node) const
{
	switch (node->getNodeType())
	{
	case EParserNodeType::Name:
		return ECompilerState::NameDeclaration;
	}
	return CompilerState::getNextState(node);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

CompilerStateNameDelaration::CompilerStateNameDelaration() :
	CompilerState(ECompilerState::NameDeclaration)
{

}

ECompilerState CompilerStateNameDelaration::getNextState(std::shared_ptr<ParserNode> node) const
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
	return CompilerState::getNextState(node);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

CompilerStateFunctionDelaration::CompilerStateFunctionDelaration() :
	CompilerState(ECompilerState::FunctionDeclaration)
{
}

ECompilerState CompilerStateFunctionDelaration::getNextState(std::shared_ptr<ParserNode> node) const
{
	return CompilerState::getNextState(node);
}
