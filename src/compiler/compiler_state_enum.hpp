#ifndef COMPILER_STATE_ENUM
#define COMPILER_STATE_ENUM

enum class ECompilerState
{
	None,
	TypeDeclaration,
	NameDeclaration,
	FunctionDeclaration,
	FunctionDefinition,
};

#endif // COMPILER_STATE_ENUM