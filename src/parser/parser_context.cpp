#include "parser_context.hpp"
#include <algorithm>

// AST Container methods

std::vector<struct Struct> Context::getStructs() const
{
	return structs;
}

std::vector<struct GlobalVariable> Context::getGlobalVariables() const
{
	return globalVariables;
}

std::vector<struct Function> Context::getFunctions() const
{
	return functions;
}

std::vector<struct Enum> Context::getEnums() const
{
	return enums;
}

// Realization of context
void Context::addTempAttribute(Attribute attr)
{
	tempAttributes.push_back(attr);
}

std::vector<Attribute> Context::getAndClearTempAttributes()
{
	return std::move(tempAttributes);
}

IdentifierType Context::getIdentifierType(std::string name) const
{
	for (auto it = scopes.rbegin(); it != scopes.rend(); it++)
	{
		auto findResult = it->identifiers.find(name);
		if (findResult == it->identifiers.end())
			continue;
		return findResult->second.type;
	}
	return IdentifierType::Undefined;
}

Identifier Context::use(std::string name)
{
	for (auto it = scopes.rbegin(); it != scopes.rend(); it++)
	{
		auto findResult = it->identifiers.find(name);
		if (findResult == it->identifiers.end())
			continue;
		return findResult->second;
	}
	return {};
}

Struct &Context::defineStruct(const std::string &name)
{
	if (getIdentifierType(name) != IdentifierType::Undefined)
		throw 0; // TODO: [OOKAMI] throw error
	Identifier t;
	t.type = IdentifierType::Structure;
	t.name = name;
	auto id = pushIdentifierToScope(t);
	Struct s;
	s.name = name;
	s.attributes = getAndClearTempAttributes();
	s.id = id;
	structs.push_back(s);
	return structs.back();
}

Struct &Context::getStruct(const std::string &name)
{
	auto it = std::find_if(structs.begin(), structs.end(),
		[&name](const Struct &s) -> bool
	{
		return s.name == name;
	});
	if (it == structs.end())
		throw 0;
	return *it;
}

std::vector<struct StructMember> Context::defineStructMembers(Type t, const std::vector<std::string> &names)
{
	std::vector<struct StructMember> members;
	std::transform(names.begin(), names.end(), std::back_inserter(members),
		[&t](const std::string &name)
	{
		StructMember member;
		member.type = t;
		member.name = name;
		member.position = 0;
		return member;
	});
	return std::move(members);
}

void Context::indexStructMembers(Struct &s)
{
	size_t pos = 0;
	for (auto &member : s.members)
		member.position = pos++;
}

void Context::defineGlobalVariables(Type t, const VarNamesAndInits &vars)
{
	for (auto &var : vars)
	{
		Identifier ident;
		ident.name = var.first;
		ident.type = IdentifierType::Variable;
		auto id = pushIdentifierToScope(ident);
		GlobalVariable v;
		v.id = id;
		v.type = t;
		v.name = var.first;
		v.attributes = getAndClearTempAttributes();
		v.initialization = var.second;
		globalVariables.push_back(v);
	}
}

Expression Context::defineLocalVariables(Type t, const VarNamesAndInits &vars)
{
	Expression ex;
	ex.type = ExpressionType::VariableDeclaration;
	for (auto &var : vars)
	{
		Identifier ident;
		ident.name = var.first;
		ident.type = IdentifierType::Variable;
		auto id = pushIdentifierToScope(ident);
		LocalVariable v;
		v.id = id;
		v.type = t;
		v.name = var.first;
		v.initialization = var.second;
		ex.declaredVariables.push_back(v);
	}
	return ex;
}

FunctionParameter Context::defineFunctionParameter(Type t, const std::string &name)
{
	int64_t id = -1;
	if (!name.empty())
	{
		Identifier ident;
		ident.name = name;
		ident.type = IdentifierType::Variable;
		id = pushIdentifierToScope(ident);
	}
	FunctionParameter p;
	p.id = id;
	p.type = t;
	p.name = name;
	return p;
}

void Context::defineFunctionPrototype(const FunctionDeclaration &decl)
{
	Identifier ident;
	ident.name = decl.name;
	ident.type = IdentifierType::Function;
	pushIdentifierToScope(ident);
	getOrDefineFunction(decl);
}

void Context::defineFunction(const FunctionDeclaration &decl, const Expression &body)
{
	Identifier ident;
	ident.name = decl.name;
	ident.type = IdentifierType::Function;
	pushIdentifierToScope(ident);
	auto &func = getOrDefineFunction(decl);
	//TODO: [OOKAMI] if body already exists - exception
	func.body = body;
}

void Context::defineEnum(TypeInner baseType, std::string name, EnumDecls defines)
{
	Identifier ident;
	ident.name = name;
	ident.type = IdentifierType::Enumeration;
	pushIdentifierToScope(ident);
	Enum e;
	e.baseType.innerType = baseType;
	e.name = name;
	e.enumDeclarations = defines;
	enums.push_back(e);
}

void Context::resetEnumCounter()
{
	enumCounter.unum = 0;
}

void Context::setEnumCounter(Literal num)
{
	enumCounter = num;
}

void Context::incrementEnumCounter()
{
	enumCounter.unum++;
}

Literal Context::getEnumCounter()
{
	return enumCounter;
}

Literal Context::resolveEnum(std::string enumName, std::string name)
{
	auto itEnum = std::find_if(enums.begin(), enums.end(),
		[&enumName](Enum &e) -> bool
	{
		return e.name == enumName;
	});
	auto itName = itEnum->enumDeclarations.find(name);
	if (itName == itEnum->enumDeclarations.end())
		throw 0; // TODO: [OOKAMI] Throw exception
	return itName->second;
}

void Context::operator++()
{
	scopes.emplace_back();
}

void Context::operator--()
{
	scopes.pop_back();
}

int64_t Context::pushIdentifierToScope(Identifier id)
{
	static int counter = 0;
	// Functions can be overloaded.
	//So compiler decide which function to call
	if (id.type != IdentifierType::Function)
		id.id = counter++;
	else
		id.id = -1;
	auto &topScope = scopes.back();
	if (id.type == IdentifierType::Function)
	{
		if (topScope.identifiers.find(id.name) == topScope.identifiers.end())
			topScope.identifiers.insert(std::make_pair(id.name, id));
	}
	else
		topScope.identifiers.insert(std::make_pair(id.name, id));
	return id.id;
}

std::vector<struct Function*> Context::getFunctionsByName(std::string name)
{
	std::vector<struct Function*> findedFunctions;
	for (auto &func : functions)
	{
		if (func.name == name)
			findedFunctions.push_back(&func);
	}
	return findedFunctions;
}

Function &Context::getOrDefineFunction(const FunctionDeclaration &decl)
{
	auto functionsByName = getFunctionsByName(decl.name);
	bool createNew = false;
	if (functionsByName.empty())
		createNew = true;
	else
	{
		// Try to find exists function
		auto lambdaRemove = [&decl](Function *func) -> bool
		{
			if (func->parameters.size() != decl.parameters.size())
				return true;
			for (size_t i = 0; i < func->parameters.size(); i++)
			{
				const auto &fParam = func->parameters[i];
				const auto &dParam = decl.parameters[i];
				if (fParam.type != dParam.type)
					return true;
			}
			return false;
		};
		//remove with different params
		auto iterDelete = std::remove_if(functionsByName.begin(), functionsByName.end(), lambdaRemove);
		functionsByName.erase(iterDelete, functionsByName.end());
		if (functionsByName.size() > 1)
			throw 0; // TODO: [OOKAMI] Throw normal exception
		else if (functionsByName.size() == 1)
		{
			auto findedFunc = functionsByName.back();
			if (findedFunc->returnType == decl.returnType)
				return *findedFunc;
			// TODO: [OOKAMI] Throw normal exception
			throw 0;
		}
		else
			createNew = true;
	}
	if (createNew)
	{
		Function f;
		f.returnType = decl.returnType;
		f.name = decl.name;
		f.parameters = decl.parameters;
		f.attributes = getAndClearTempAttributes();
		functions.push_back(f);
		return functions.back();
	}
	// TODO: [OOKAMI] Throw normal exception
	throw 0;
}