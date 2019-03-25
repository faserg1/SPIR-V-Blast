#include "shader_preprocessed_info.hpp"
#include <stdexcept>
#include <algorithm>

const std::string &ShaderPreprocessedInfo::text() const
{
	return text_;
}

EntryPoint &ShaderPreprocessedInfo::getEntryPoint(const std::string &id)
{
	auto iter = std::find_if(entryPoints_.begin(), entryPoints_.end(), [&id](const EntryPoint &entryPoint) -> bool
	{
		return entryPoint.id == id;
	});
	if (iter != entryPoints_.end())
		return *iter;
	throw std::runtime_error("Unable to find entry point \"" + id + "\"!");
}

EntryPoint &ShaderPreprocessedInfo::addEntryPoint(const std::string &id, const std::string &functionId, const std::string &executionModel)
{
	spv::ExecutionModel model;
	if (executionModel == "Vertex")
		model = spv::ExecutionModel::Vertex;
	else if (executionModel == "TessellationControl")
		model = spv::ExecutionModel::TessellationControl;
	else if (executionModel == "TessellationEvaluation")
		model = spv::ExecutionModel::TessellationEvaluation;
	else if (executionModel == "Geometry")
		model = spv::ExecutionModel::Geometry;
	else if (executionModel == "Fragment")
		model = spv::ExecutionModel::Fragment;
	else if (executionModel == "GLCompute")
		model = spv::ExecutionModel::GLCompute;
	else if (executionModel == "Kernel")
		model = spv::ExecutionModel::Kernel;
	else if (executionModel == "TaskNV")
		model = spv::ExecutionModel::TaskNV;
	else if (executionModel == "MeshNV")
		model = spv::ExecutionModel::MeshNV;
	else if (executionModel == "RayGenerationNV")
		model = spv::ExecutionModel::RayGenerationNV;
	else if (executionModel == "IntersectionNV")
		model = spv::ExecutionModel::IntersectionNV;
	else if (executionModel == "AnyHitNV")
		model = spv::ExecutionModel::AnyHitNV;
	else if (executionModel == "ClosestHitNV")
		model = spv::ExecutionModel::ClosestHitNV;
	else if (executionModel == "MissNV")
		model = spv::ExecutionModel::MissNV;
	else if (executionModel == "CallableNV")
		model = spv::ExecutionModel::CallableNV;
	else
		throw std::runtime_error("Unknown execution model \"" + executionModel + "\"!");

	entryPoints_.push_back({ id, functionId, model });

	return entryPoints_.back();
}

void ShaderPreprocessedInfo::setMemoryModel(std::string addressingModel, std::string memoryModel)
{
	if (addressingModel == "Logical")
		adressingModel_ = spv::AddressingModel::Logical;
	else if (addressingModel == "Physical32")
		adressingModel_ = spv::AddressingModel::Physical32;
	else if (addressingModel == "Physical64")
		adressingModel_ = spv::AddressingModel::Physical64;
	else if (addressingModel == "PhysicalStorageBuffer64EXT")
		adressingModel_ = spv::AddressingModel::PhysicalStorageBuffer64EXT;
	else
		throw std::runtime_error("Unknown addressing model \"" + addressingModel + "\"!");

	if (memoryModel == "Simple")
		memoryModel_ = spv::MemoryModel::Simple;
	else if (memoryModel == "GLSL450")
		memoryModel_ = spv::MemoryModel::GLSL450;
	else if (memoryModel == "OpenCL")
		memoryModel_ = spv::MemoryModel::OpenCL;
	else if (memoryModel == "VulkanKHR")
		memoryModel_ = spv::MemoryModel::VulkanKHR;
	else
		throw std::runtime_error("Unknown memory model \"" + memoryModel + "\"!");
}
