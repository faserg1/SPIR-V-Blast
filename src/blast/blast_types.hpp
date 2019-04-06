#ifndef BLAST_TYPES
#define BLAST_TYPES

// TODO: There is types that will be used only in blast.
// Reduse some types, as function - function will be... just a function

enum class EBlastType
{
	Void,
	Boolean,
	Integer,
	Float,
	Vector,
	Matrix,
	Image,
	Sampler,
	SampledImage,
	Array,
	RuntimeArray,
	Struct,
	Opaque,
	Pointer, //< ?
	Function, //< ?

	Event,
	DeviceEvent,
	ReserveId,
	Queue,
	Pipe,
	ForwardPointer,
	PipeStorage,
	NamedBarrier,

	AccelerationStructureNV,
	OpTypeCooperativeMatrixNV,
};

#endif