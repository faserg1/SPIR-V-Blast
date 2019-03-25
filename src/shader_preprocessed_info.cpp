#include "shader_preprocessed_info.hpp"
#include <stdexcept>
#include <algorithm>

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
