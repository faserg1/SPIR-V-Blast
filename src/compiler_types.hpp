#ifndef COMPILER_TYPES
#define COMPILER_TYPES

#include <cstdint>
#include <memory>

enum class ECompilerType
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
	Pointer,
	Function,

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

class CompilerType :
	std::enable_shared_from_this<CompilerType>
{
public:
	virtual ~CompilerType() = default;
	ECompilerType type() const;
	bool operator==(const CompilerType &otherType) const;
protected:
	CompilerType(ECompilerType type);
	virtual bool isEqual(const CompilerType &otherType) const = 0;
private:
	const ECompilerType type_;
};

class BasicType :
	public CompilerType
{
public:
	virtual ~BasicType() = default;
protected:
	BasicType(ECompilerType type);
};

class VoidType :
	public BasicType
{
public:
	VoidType();
protected:
	bool isEqual(const CompilerType &otherType) const override;
};

class BooleanType :
	public BasicType
{
public:
	BooleanType();
protected:
	bool isEqual(const CompilerType &otherType) const override;
};


class IntegerType :
	public BasicType
{
public:
	/** 
	@param width number of bits
	@param signedness 0 — unsigned or no signedness semantic, 1 — signed
	**/
	IntegerType(int16_t width, int16_t signedness);

	/* Number of bits */
	int16_t width() const;
	/* 0 — unsigned or no signedness semantic, 1 — signed */
	int16_t signedness() const;
protected:
	bool isEqual(const CompilerType &otherType) const override;
private:
	const int16_t width_;
	const int16_t signedness_;
};

class FloatType :
	public BasicType
{
public:
	/**
	@param width number of bits
	**/
	FloatType(int16_t width);

	int16_t width() const;
protected:
	bool isEqual(const CompilerType &otherType) const override;
private:
	const int16_t width_;
};

#endif