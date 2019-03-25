#ifndef SHADER_PREPROCESSED_INFO
#define SHADER_PREPROCESSED_INFO

#include <spirv.hpp11>
#include <string>
#include <vector>
#include <set>

struct EntryPoint
{
	std::string functionId;
	spv::ExecutionModel executionModel;
};

class ShaderPreprocessedInfo
{
	friend class Preprocessor;
public:
	const std::string &text() const;
	std::set<spv::Capability> capabilities() const;
	std::vector<EntryPoint> entryPoints() const;
	spv::MemoryModel memoryModel() const;
	spv::AddressingModel addressingModel() const;
private:
	std::string text_;

	std::set<spv::Capability> capabilities_;
	std::vector<EntryPoint> entryPoints_;
	spv::MemoryModel memoryModel_;
	spv::AddressingModel addressingModel_;
private:
	EntryPoint &getEntryPoint(const std::string &functionId);
	EntryPoint &addEntryPoint(const std::string &functionId, const std::string &executionModel);

	void setMemoryModel(std::string addressingModel, std::string memoryModel);

	void addCapability(std::string capability);
};

#endif // SHADER_PREPROCESSED_INFO
