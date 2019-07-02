#ifndef COMPILER_CONTEXT
#define COMPILER_CONTEXT

#include <stack>
#include "compiler_data.hpp"
#include "../gen/blast_tokens.hpp"
#include "compiler_identifiers.hpp"

class CompilerContext
{
public:
	bool hasType(const TypeInner &type) const;
	bool hasType(const StructureType &type) const;
	bool hasType(const FunctionType &type) const;
	bool hasConstant(const Type &t, Literal &value) const;

	Id getTypeId(const TypeInner &type);
	Id getTypeId(const StructureType &type);
	Id getTypeId(const FunctionType &funcType);
	Id getConstantId(const Type &t, Literal &value);
	Id getVariableId(const BaseVariable &var);
	Id getFunctionId(const Function &func);

	Id findIdentifier(const Identifier &ident);

	void addConstantIdAssociation(const BaseVariable &var, const Id &id);

	Id createId(const std::string debugName);

	void addHeader(SpirVOp headerInstruction);
	void addDecorate(SpirVOp decorateInstruction);
	void addType(SpirVOp typeInstruction);
	void addDebug(SpirVOp debugInstruction);
	void addConst(SpirVOp constInstruction);
	void addSpecConst(SpirVOp specConstInstruction);
	void addGlobal(SpirVOp globalInstruction);

	std::vector<SpirVOp> getHeaderOps() const;
	std::vector<SpirVOp> getDebugNameOps() const;
	std::vector<SpirVOp> getDecorateOps() const;
	std::vector<SpirVOp> getTypeOps() const;
	std::vector<SpirVOp> getConstOps() const;
	std::vector<SpirVOp> getSpecConstOps() const;
	std::vector<SpirVOp> getGlobalOps() const;
private:
	CompilerIdentifiers ids_;
	
	//header operations
	std::vector<SpirVOp> headerOps_;
	//debug name operations
	std::vector<SpirVOp> debugNameOps_;
	//decorate operations
	std::vector<SpirVOp> decorateOps_;
	//type operations
	std::vector<SpirVOp> typeOps_;
	//const declarations operations
	std::vector<SpirVOp> constOps_;
	//specconst declarations operations
	std::vector<SpirVOp> specConstOps_;
	//global operations
	std::vector<SpirVOp> globalOps_;
	
	//stack -> instruction set ?
	//operation list to insert after or before some position of stack
};

#endif // COMPILER_CONTEXT