#include "shader_preprocessed_info.hpp"
#include <stdexcept>
#include <algorithm>

const std::string &ShaderPreprocessedInfo::text() const
{
	return text_;
}

std::vector<DebugRowInfo> ShaderPreprocessedInfo::debugRowsInfo() const
{
	return debugRowsInfo_;
}

std::vector<std::string> ShaderPreprocessedInfo::sourceFiles() const
{
	return sourceFiles_;
}

std::set<spv::Capability> ShaderPreprocessedInfo::capabilities() const
{
	return capabilities_;
}

std::vector<EntryPoint> ShaderPreprocessedInfo::entryPoints() const
{
	return entryPoints_;
}

spv::MemoryModel ShaderPreprocessedInfo::memoryModel() const
{
	return memoryModel_;
}

spv::AddressingModel ShaderPreprocessedInfo::addressingModel() const
{
	return addressingModel_;
}

EntryPoint &ShaderPreprocessedInfo::getEntryPoint(const std::string &functionId)
{
	auto iter = std::find_if(entryPoints_.begin(), entryPoints_.end(), [&functionId](const EntryPoint &entryPoint) -> bool
	{
		return entryPoint.functionId == functionId;
	});
	if (iter != entryPoints_.end())
		return *iter;
	throw std::runtime_error("Unable to find entry point \"" + functionId + "\"!");
}

EntryPoint &ShaderPreprocessedInfo::addEntryPoint(const std::string &functionId, const std::string &executionModel)
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

	entryPoints_.push_back({ functionId, model });

	return entryPoints_.back();
}

void ShaderPreprocessedInfo::setMemoryModel(std::string addressingModel, std::string memoryModel)
{
	if (addressingModel == "Logical")
		addressingModel_ = spv::AddressingModel::Logical;
	else if (addressingModel == "Physical32")
		addressingModel_ = spv::AddressingModel::Physical32;
	else if (addressingModel == "Physical64")
		addressingModel_ = spv::AddressingModel::Physical64;
	else if (addressingModel == "PhysicalStorageBuffer64EXT")
		addressingModel_ = spv::AddressingModel::PhysicalStorageBuffer64EXT;
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

void ShaderPreprocessedInfo::addCapability(std::string capability)
{
	spv::Capability capabilitySpv = spv::Capability::Max;
	if (capability == "Matrix")
		capabilitySpv = spv::Capability::Matrix;
	if (capability == "Shader")
		capabilitySpv = spv::Capability::Shader;
	if (capability == "Geometry")
		capabilitySpv = spv::Capability::Geometry;
	if (capability == "Tessellation")
		capabilitySpv = spv::Capability::Tessellation;
	if (capability == "Addresses")
		capabilitySpv = spv::Capability::Addresses;
	if (capability == "Linkage")
		capabilitySpv = spv::Capability::Linkage;
	if (capability == "Kernel")
		capabilitySpv = spv::Capability::Kernel;
	if (capability == "Vector16")
		capabilitySpv = spv::Capability::Vector16;
	if (capability == "Float16Buffer")
		capabilitySpv = spv::Capability::Float16Buffer;
	if (capability == "Float16")
		capabilitySpv = spv::Capability::Float16;
	if (capability == "Float64")
		capabilitySpv = spv::Capability::Float64;
	if (capability == "Int64")
		capabilitySpv = spv::Capability::Int64;
	if (capability == "Int64Atomics")
		capabilitySpv = spv::Capability::Int64Atomics;
	if (capability == "ImageBasic")
		capabilitySpv = spv::Capability::ImageBasic;
	if (capability == "ImageReadWrite")
		capabilitySpv = spv::Capability::ImageReadWrite;
	if (capability == "ImageMipmap")
		capabilitySpv = spv::Capability::ImageMipmap;
	if (capability == "Pipes")
		capabilitySpv = spv::Capability::Pipes;
	if (capability == "Groups")
		capabilitySpv = spv::Capability::Groups;
	if (capability == "DeviceEnqueue")
		capabilitySpv = spv::Capability::DeviceEnqueue;
	if (capability == "LiteralSampler")
		capabilitySpv = spv::Capability::LiteralSampler;
	if (capability == "AtomicStorage")
		capabilitySpv = spv::Capability::AtomicStorage;
	if (capability == "Int16")
		capabilitySpv = spv::Capability::Int16;
	if (capability == "TessellationPointSize")
		capabilitySpv = spv::Capability::TessellationPointSize;
	if (capability == "GeometryPointSize")
		capabilitySpv = spv::Capability::GeometryPointSize;
	if (capability == "ImageGatherExtended")
		capabilitySpv = spv::Capability::ImageGatherExtended;
	if (capability == "StorageImageMultisample")
		capabilitySpv = spv::Capability::StorageImageMultisample;
	if (capability == "UniformBufferArrayDynamicIndexing")
		capabilitySpv = spv::Capability::UniformBufferArrayDynamicIndexing;
	if (capability == "SampledImageArrayDynamicIndexing")
		capabilitySpv = spv::Capability::SampledImageArrayDynamicIndexing;
	if (capability == "StorageBufferArrayDynamicIndexing")
		capabilitySpv = spv::Capability::StorageBufferArrayDynamicIndexing;
	if (capability == "StorageImageArrayDynamicIndexing")
		capabilitySpv = spv::Capability::StorageImageArrayDynamicIndexing;
	if (capability == "ClipDistance")
		capabilitySpv = spv::Capability::ClipDistance;
	if (capability == "CullDistance")
		capabilitySpv = spv::Capability::CullDistance;
	if (capability == "ImageCubeArray")
		capabilitySpv = spv::Capability::ImageCubeArray;
	if (capability == "SampleRateShading")
		capabilitySpv = spv::Capability::SampleRateShading;
	if (capability == "ImageRect")
		capabilitySpv = spv::Capability::ImageRect;
	if (capability == "SampledRect")
		capabilitySpv = spv::Capability::SampledRect;
	if (capability == "GenericPointer")
		capabilitySpv = spv::Capability::GenericPointer;
	if (capability == "Int8")
		capabilitySpv = spv::Capability::Int8;
	if (capability == "InputAttachment")
		capabilitySpv = spv::Capability::InputAttachment;
	if (capability == "SparseResidency")
		capabilitySpv = spv::Capability::SparseResidency;
	if (capability == "MinLod")
		capabilitySpv = spv::Capability::MinLod;
	if (capability == "Sampled1D")
		capabilitySpv = spv::Capability::Sampled1D;
	if (capability == "Image1D")
		capabilitySpv = spv::Capability::Image1D;
	if (capability == "SampledCubeArray")
		capabilitySpv = spv::Capability::SampledCubeArray;
	if (capability == "SampledBuffer")
		capabilitySpv = spv::Capability::SampledBuffer;
	if (capability == "ImageBuffer")
		capabilitySpv = spv::Capability::ImageBuffer;
	if (capability == "ImageMSArray")
		capabilitySpv = spv::Capability::ImageMSArray;
	if (capability == "StorageImageExtendedFormats")
		capabilitySpv = spv::Capability::StorageImageExtendedFormats;
	if (capability == "ImageQuery")
		capabilitySpv = spv::Capability::ImageQuery;
	if (capability == "DerivativeControl")
		capabilitySpv = spv::Capability::DerivativeControl;
	if (capability == "InterpolationFunction")
		capabilitySpv = spv::Capability::InterpolationFunction;
	if (capability == "TransformFeedback")
		capabilitySpv = spv::Capability::TransformFeedback;
	if (capability == "GeometryStreams")
		capabilitySpv = spv::Capability::GeometryStreams;
	if (capability == "StorageImageReadWithoutFormat")
		capabilitySpv = spv::Capability::StorageImageReadWithoutFormat;
	if (capability == "StorageImageWriteWithoutFormat")
		capabilitySpv = spv::Capability::StorageImageWriteWithoutFormat;
	if (capability == "MultiViewport")
		capabilitySpv = spv::Capability::MultiViewport;
	if (capability == "SubgroupDispatch")
		capabilitySpv = spv::Capability::SubgroupDispatch;
	if (capability == "NamedBarrier")
		capabilitySpv = spv::Capability::NamedBarrier;
	if (capability == "PipeStorage")
		capabilitySpv = spv::Capability::PipeStorage;
	if (capability == "GroupNonUniform")
		capabilitySpv = spv::Capability::GroupNonUniform;
	if (capability == "GroupNonUniformVote")
		capabilitySpv = spv::Capability::GroupNonUniformVote;
	if (capability == "GroupNonUniformArithmetic")
		capabilitySpv = spv::Capability::GroupNonUniformArithmetic;
	if (capability == "GroupNonUniformBallot")
		capabilitySpv = spv::Capability::GroupNonUniformBallot;
	if (capability == "GroupNonUniformShuffle")
		capabilitySpv = spv::Capability::GroupNonUniformShuffle;
	if (capability == "GroupNonUniformShuffleRelative")
		capabilitySpv = spv::Capability::GroupNonUniformShuffleRelative;
	if (capability == "GroupNonUniformClustered")
		capabilitySpv = spv::Capability::GroupNonUniformClustered;
	if (capability == "GroupNonUniformQuad")
		capabilitySpv = spv::Capability::GroupNonUniformQuad;
	if (capability == "SubgroupBallotKHR")
		capabilitySpv = spv::Capability::SubgroupBallotKHR;
	if (capability == "DrawParameters")
		capabilitySpv = spv::Capability::DrawParameters;
	if (capability == "SubgroupVoteKHR")
		capabilitySpv = spv::Capability::SubgroupVoteKHR;
	if (capability == "StorageBuffer16BitAccess")
		capabilitySpv = spv::Capability::StorageBuffer16BitAccess;
	if (capability == "StorageUniformBufferBlock16")
		capabilitySpv = spv::Capability::StorageUniformBufferBlock16;
	if (capability == "StorageUniform16")
		capabilitySpv = spv::Capability::StorageUniform16;
	if (capability == "UniformAndStorageBuffer16BitAccess")
		capabilitySpv = spv::Capability::UniformAndStorageBuffer16BitAccess;
	if (capability == "StoragePushConstant16")
		capabilitySpv = spv::Capability::StoragePushConstant16;
	if (capability == "StorageInputOutput16")
		capabilitySpv = spv::Capability::StorageInputOutput16;
	if (capability == "DeviceGroup")
		capabilitySpv = spv::Capability::DeviceGroup;
	if (capability == "MultiView")
		capabilitySpv = spv::Capability::MultiView;
	if (capability == "VariablePointersStorageBuffer")
		capabilitySpv = spv::Capability::VariablePointersStorageBuffer;
	if (capability == "VariablePointers")
		capabilitySpv = spv::Capability::VariablePointers;
	if (capability == "AtomicStorageOps")
		capabilitySpv = spv::Capability::AtomicStorageOps;
	if (capability == "SampleMaskPostDepthCoverage")
		capabilitySpv = spv::Capability::SampleMaskPostDepthCoverage;
	if (capability == "StorageBuffer8BitAccess")
		capabilitySpv = spv::Capability::StorageBuffer8BitAccess;
	if (capability == "UniformAndStorageBuffer8BitAccess")
		capabilitySpv = spv::Capability::UniformAndStorageBuffer8BitAccess;
	if (capability == "StoragePushConstant8")
		capabilitySpv = spv::Capability::StoragePushConstant8;
	if (capability == "DenormPreserve")
		capabilitySpv = spv::Capability::DenormPreserve;
	if (capability == "DenormFlushToZero")
		capabilitySpv = spv::Capability::DenormFlushToZero;
	if (capability == "SignedZeroInfNanPreserve")
		capabilitySpv = spv::Capability::SignedZeroInfNanPreserve;
	if (capability == "RoundingModeRTE")
		capabilitySpv = spv::Capability::RoundingModeRTE;
	if (capability == "RoundingModeRTZ")
		capabilitySpv = spv::Capability::RoundingModeRTZ;
	if (capability == "Float16ImageAMD")
		capabilitySpv = spv::Capability::Float16ImageAMD;
	if (capability == "ImageGatherBiasLodAMD")
		capabilitySpv = spv::Capability::ImageGatherBiasLodAMD;
	if (capability == "FragmentMaskAMD")
		capabilitySpv = spv::Capability::FragmentMaskAMD;
	if (capability == "StencilExportEXT")
		capabilitySpv = spv::Capability::StencilExportEXT;
	if (capability == "ImageReadWriteLodAMD")
		capabilitySpv = spv::Capability::ImageReadWriteLodAMD;
	if (capability == "SampleMaskOverrideCoverageNV")
		capabilitySpv = spv::Capability::SampleMaskOverrideCoverageNV;
	if (capability == "GeometryShaderPassthroughNV")
		capabilitySpv = spv::Capability::GeometryShaderPassthroughNV;
	if (capability == "ShaderViewportIndexLayerEXT")
		capabilitySpv = spv::Capability::ShaderViewportIndexLayerEXT;
	if (capability == "ShaderViewportIndexLayerNV")
		capabilitySpv = spv::Capability::ShaderViewportIndexLayerNV;
	if (capability == "ShaderViewportMaskNV")
		capabilitySpv = spv::Capability::ShaderViewportMaskNV;
	if (capability == "ShaderStereoViewNV")
		capabilitySpv = spv::Capability::ShaderStereoViewNV;
	if (capability == "PerViewAttributesNV")
		capabilitySpv = spv::Capability::PerViewAttributesNV;
	if (capability == "FragmentFullyCoveredEXT")
		capabilitySpv = spv::Capability::FragmentFullyCoveredEXT;
	if (capability == "MeshShadingNV")
		capabilitySpv = spv::Capability::MeshShadingNV;
	if (capability == "ImageFootprintNV")
		capabilitySpv = spv::Capability::ImageFootprintNV;
	if (capability == "FragmentBarycentricNV")
		capabilitySpv = spv::Capability::FragmentBarycentricNV;
	if (capability == "ComputeDerivativeGroupQuadsNV")
		capabilitySpv = spv::Capability::ComputeDerivativeGroupQuadsNV;
	if (capability == "FragmentDensityEXT")
		capabilitySpv = spv::Capability::FragmentDensityEXT;
	if (capability == "ShadingRateNV")
		capabilitySpv = spv::Capability::ShadingRateNV;
	if (capability == "GroupNonUniformPartitionedNV")
		capabilitySpv = spv::Capability::GroupNonUniformPartitionedNV;
	if (capability == "ShaderNonUniformEXT")
		capabilitySpv = spv::Capability::ShaderNonUniformEXT;
	if (capability == "RuntimeDescriptorArrayEXT")
		capabilitySpv = spv::Capability::RuntimeDescriptorArrayEXT;
	if (capability == "InputAttachmentArrayDynamicIndexingEXT")
		capabilitySpv = spv::Capability::InputAttachmentArrayDynamicIndexingEXT;
	if (capability == "UniformTexelBufferArrayDynamicIndexingEXT")
		capabilitySpv = spv::Capability::UniformTexelBufferArrayDynamicIndexingEXT;
	if (capability == "StorageTexelBufferArrayDynamicIndexingEXT")
		capabilitySpv = spv::Capability::StorageTexelBufferArrayDynamicIndexingEXT;
	if (capability == "UniformBufferArrayNonUniformIndexingEXT")
		capabilitySpv = spv::Capability::UniformBufferArrayNonUniformIndexingEXT;
	if (capability == "SampledImageArrayNonUniformIndexingEXT")
		capabilitySpv = spv::Capability::SampledImageArrayNonUniformIndexingEXT;
	if (capability == "StorageBufferArrayNonUniformIndexingEXT")
		capabilitySpv = spv::Capability::StorageBufferArrayNonUniformIndexingEXT;
	if (capability == "StorageImageArrayNonUniformIndexingEXT")
		capabilitySpv = spv::Capability::StorageImageArrayNonUniformIndexingEXT;
	if (capability == "InputAttachmentArrayNonUniformIndexingEXT")
		capabilitySpv = spv::Capability::InputAttachmentArrayNonUniformIndexingEXT;
	if (capability == "UniformTexelBufferArrayNonUniformIndexingEXT")
		capabilitySpv = spv::Capability::UniformTexelBufferArrayNonUniformIndexingEXT;
	if (capability == "StorageTexelBufferArrayNonUniformIndexingEXT")
		capabilitySpv = spv::Capability::StorageTexelBufferArrayNonUniformIndexingEXT;
	if (capability == "RayTracingNV")
		capabilitySpv = spv::Capability::RayTracingNV;
	if (capability == "VulkanMemoryModelKHR")
		capabilitySpv = spv::Capability::VulkanMemoryModelKHR;
	if (capability == "VulkanMemoryModelDeviceScopeKHR")
		capabilitySpv = spv::Capability::VulkanMemoryModelDeviceScopeKHR;
	if (capability == "PhysicalStorageBufferAddressesEXT")
		capabilitySpv = spv::Capability::PhysicalStorageBufferAddressesEXT;
	if (capability == "ComputeDerivativeGroupLinearNV")
		capabilitySpv = spv::Capability::ComputeDerivativeGroupLinearNV;
	if (capability == "CooperativeMatrixNV")
		capabilitySpv = spv::Capability::CooperativeMatrixNV;
	if (capability == "SubgroupShuffleINTEL")
		capabilitySpv = spv::Capability::SubgroupShuffleINTEL;
	if (capability == "SubgroupBufferBlockIOINTEL")
		capabilitySpv = spv::Capability::SubgroupBufferBlockIOINTEL;
	if (capability == "SubgroupImageBlockIOINTEL")
		capabilitySpv = spv::Capability::SubgroupImageBlockIOINTEL;
	if (capability == "SubgroupImageMediaBlockIOINTEL")
		capabilitySpv = spv::Capability::SubgroupImageMediaBlockIOINTEL;
	if (capability == "SubgroupAvcMotionEstimationINTEL")
		capabilitySpv = spv::Capability::SubgroupAvcMotionEstimationINTEL;
	if (capability == "SubgroupAvcMotionEstimationIntraINTEL")
		capabilitySpv = spv::Capability::SubgroupAvcMotionEstimationIntraINTEL;
	if (capability == "SubgroupAvcMotionEstimationChromaINTEL")
		capabilitySpv = spv::Capability::SubgroupAvcMotionEstimationChromaINTEL;

	if (capabilitySpv == spv::Capability::Max)
		throw std::runtime_error("Unknown capability \"" + capability + "\"!");

	capabilities_.insert(capabilitySpv);
}
