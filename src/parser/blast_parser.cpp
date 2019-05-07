#include "../gen/blast_parser.hpp"

std::shared_ptr<Context> buildContext()
{
	return std::make_shared<Context>();
}

std::shared_ptr<AbstractSyntaxTreeContainer> getContainer(std::shared_ptr<Context> context)
{
	return std::dynamic_pointer_cast<AbstractSyntaxTreeContainer>(context);
}