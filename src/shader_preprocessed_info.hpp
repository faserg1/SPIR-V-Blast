#ifndef SHADER_PREPROCESSED_INFO
#define SHADER_PREPROCESSED_INFO

#include <spirv.hpp11>
#include <string>
#include <vector>

struct EntryPoint
{
	std::string id;
	std::string functionId;
	spv::ExecutionModel executionModel;
};

class ShaderPreprocessedInfo
{
	friend class Preprocessor;
public:
	const std::string &text() const;
private:
	std::string text_;

	std::vector<EntryPoint> entryPoints_;
	spv::MemoryModel memoryModel_;
	spv::AddressingModel adressingModel_;
private:
	EntryPoint &getEntryPoint(const std::string &id);
	EntryPoint &addEntryPoint(const std::string &id, const std::string &functionId, const std::string &executionModel);

	void setMemoryModel(std::string addressingModel, std::string memoryModel);
};

#endif // SHADER_PREPROCESSED_INFO
