#ifndef COMPILER_CONTEXT
#define COMPILER_CONTEXT

#include "compiler_data.hpp"
#include "../gen/blast_tokens.hpp"
#include "compiler_identifiers.hpp"

class CompilerContext
{
public:
	bool hasType(const TypeInner &type);
	Id getTypeId(const TypeInner &type);

	void addType(SpirVOp typeInstruction);
	void addDebug(SpirVOp debugInstruction);
private:
	CompilerIdentifiers ids_;
	
	//debug name operations
	std::vector<SpirVOp> debugNameOps_;
	//decorate operations
	std::vector<SpirVOp> decorateOps_;
	//type operations
	std::vector<SpirVOp> typeOps_;
	//global operations
	std::vector<SpirVOp> globalOps_;
	
	//stack -> instruction set ?
	//operation list to insert after or before some position of stack
};

#endif // COMPILER_CONTEXT