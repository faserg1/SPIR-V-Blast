#include "compiler_data.hpp"

bool operator<(const FunctionType &t1, const FunctionType &t2)
{
	if (t1.paramTypes.size() > t2.paramTypes.size())
		return false;
	if (t1.paramTypes.size() < t2.paramTypes.size())
		return true;
	for (size_t idxParamType = 0; idxParamType < t1.paramTypes.size(); idxParamType++)
	{
		if (t1.paramTypes[idxParamType].id > t2.paramTypes[idxParamType].id)
			return false;
		if (t2.paramTypes[idxParamType].id < t2.paramTypes[idxParamType].id)
			return true;
	}
	if (t1.returnType.id > t2.returnType.id)
		return false;
	if (t1.returnType.id < t2.returnType.id)
		return true;
	return true;
}

bool operator<(const StructureType &t1, const StructureType &t2)
{
	if (t1.memberTypes.size() > t2.memberTypes.size())
		return false;
	if (t1.memberTypes.size() < t2.memberTypes.size())
		return true;
	for (size_t idxParamType = 0; idxParamType < t1.memberTypes.size(); idxParamType++)
	{
		if (t1.memberTypes[idxParamType].id > t2.memberTypes[idxParamType].id)
			return false;
		if (t2.memberTypes[idxParamType].id < t2.memberTypes[idxParamType].id)
			return true;
	}
	return true;
}