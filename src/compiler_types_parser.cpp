#include "compiler_types_parser.hpp"
#include <stdexcept>
#include <regex>

CompilerBasicTypeParser::CompilerBasicTypeParser() : data({})
{
}

bool CompilerBasicTypeParser::tryVisit(const std::string &expression)
{
	if (data.type_.empty() && isType(expression))
	{
		data.type_ = expression;
		return true;
	}
	else if (!data.type_.empty())
	{
		if (data.parametersRecord_)
		{
			if (expression == ">")
			{
				data.parametersRecord_ = false;
				data.recordEnd_ = true;
				return false;
			}
			else
			{
				if (data.type_ == "float" && data.parameters_.size() >= 1)
					throw std::logic_error("Ivalid syntax! Float has only one parameter");
				if (data.type_ == "int" && data.parameters_.size() >= 2)
					throw std::logic_error("Ivalid syntax! Float has only one parameter");
				if (data.separatorAwait_ && expression != ",")
					throw std::logic_error("Ivalid syntax! Type arguments must be separeted by \'.\'");
				else if (data.separatorAwait_ && expression == ",")
				{
					data.separatorAwait_ = false;
					return true;
				}
				std::regex literalRegex("^(\\d{1,5})$", std::regex_constants::ECMAScript);
				std::sregex_iterator end;
				auto literalMatchIter = std::sregex_iterator(expression.begin(), expression.end(), literalRegex);
				if (literalMatchIter == end)
					throw std::logic_error("Ivalid syntax! Type arguments must be integers from 0 to 65565");
				data.parameters_.push_back(expression);
				data.separatorAwait_ = true;
				return true;
			}
		}
		else if(expression == "<")
		{
			data.parametersRecord_ = true;
			return true;
		}
	}
	return false;
}

bool CompilerBasicTypeParser::next()
{
	if (data.type_ == "void" ||
		data.type_ == "bool")
		return false;
	if (data.recordEnd_)
		return false;
	return true;
}

std::shared_ptr<CompilerNode> CompilerBasicTypeParser::end()
{
	std::shared_ptr<BasicType> type;
	if (data.type_ == "void")
		type = std::make_shared<VoidType>();
	else if (data.type_ == "bool")
		type = std::make_shared<BooleanType>();
	else if (data.type_ == "float")
	{
		int16_t width = 32;
		if (data.parameters_.size() > 0)
			width = static_cast<int16_t>(std::stoi(data.parameters_[0]));
		type = std::make_shared<FloatType>(width);
	}
	else if (data.type_ == "int")
	{
		int16_t width = 32;
		int16_t signedness = 1;
		if (data.parameters_.size() > 0)
			width = static_cast<int16_t>(std::stoi(data.parameters_[0]));
		if (data.parameters_.size() > 1)
			signedness = static_cast<int16_t>(std::stoi(data.parameters_[1]));
		type = std::make_shared<IntegerType>(width, signedness);
	}
	data = {};
	return std::shared_ptr<CompilerNode>();
}

bool CompilerBasicTypeParser::isType(std::string expression) const
{
	// Base type
	if (expression == "void" ||
		expression == "bool" ||
		expression == "int" ||
		expression == "float")
		return true;
	return false;
}
