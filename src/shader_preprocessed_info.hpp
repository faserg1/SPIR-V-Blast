#ifndef SHADER_PREPROCESSED_INFO
#define SHADER_PREPROCESSED_INFO

#include <spirv.hpp11>
#include <string>
#include <vector>
#include <set>

struct DebugRowInfo
{
	size_t sourceFileId;
	size_t sourceRowNumber;
};

class ShaderPreprocessedInfo
{
	friend class Preprocessor;
public:
	const std::string &text() const;
	std::vector<DebugRowInfo> debugRowsInfo() const;
	std::vector<std::string> sourceFiles() const;

	std::set<spv::Capability> capabilities() const;
	spv::MemoryModel memoryModel() const;
	spv::AddressingModel addressingModel() const;
private:
	std::string text_;
	std::vector<std::string> sourceFiles_;
	std::vector<DebugRowInfo> debugRowsInfo_;

	std::set<spv::Capability> capabilities_;
	spv::MemoryModel memoryModel_;
	spv::AddressingModel addressingModel_;
private:
	void setMemoryModel(std::string addressingModel, std::string memoryModel);

	void addCapability(std::string capability);
};

#endif // SHADER_PREPROCESSED_INFO
