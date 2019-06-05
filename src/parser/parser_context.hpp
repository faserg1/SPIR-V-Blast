#ifndef PARSER_CONTEXT
#define PARSER_CONTEXT

#include <unordered_map>

#include "parser_types.hpp"
#include "parser_abstract_syntax_tree_container.hpp"
#include "parser_lex_context.hpp"

struct scope
{
	std::unordered_map<std::string, Identifier> identifiers;
};

class Context :
	public virtual LexContext,
	public virtual AbstractSyntaxTreeContainer
{
public:
	//AST Container methods
	std::vector<struct Struct> getStructs() const override;
	std::vector<struct GlobalVariable> getGlobalVariables() const override;
	std::vector<struct Function> getFunctions() const override;
	std::vector<struct Enum> getEnums() const override;
	//Context methods
	void addTempAttribute(Attribute attr);
	std::vector<Attribute> getAndClearTempAttributes();
	IdentifierType getIdentifierType(std::string name) const override;
	Identifier use(std::string name);
	Struct &defineStruct(const std::string &name);
	Struct &getStruct(const std::string &name);
	std::vector<struct StructMember> defineStructMembers(Type t, const std::vector<std::string> &names);
	void indexStructMembers(Struct &s);
	void defineGlobalVariables(Type t, const VarNamesAndInits &vars);
	Expression defineLocalVariables(Type t, const VarNamesAndInits &vars);
	FunctionParameter defineFunctionParameter(Type t, const std::string &name);
	void defineFunctionPrototype(const FunctionDeclaration &decl);
	void defineFunction(const FunctionDeclaration &decl, const Expression &body);
	void defineEnum(TypeInner baseType, std::string name, EnumDecls defines);
	void resetEnumCounter();
	void setEnumCounter(Literal num);
	void incrementEnumCounter();
	Literal getEnumCounter();
	Literal resolveEnum(std::string enumName, std::string name);
	void operator++();
	void operator--();
private:
	std::vector<Attribute> tempAttributes;
	std::vector<scope> scopes;
	std::vector<Struct> structs;
	std::vector<GlobalVariable> globalVariables;
	std::vector<Function> functions;
	std::vector<Enum> enums;

	Literal enumCounter;
private:
	int64_t pushIdentifierToScope(Identifier id);
	std::vector<struct Function*> getFunctionsByName(std::string name);
	Function &getOrDefineFunction(const FunctionDeclaration &decl);
};

#endif