#ifndef PARSER_TYPES
#define PARSER_TYPES

#include <cstdint>
#include <memory>

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

class BlastType :
	std::enable_shared_from_this<BlastType>
{
public:
	virtual ~BlastType() = default;
	EBlastType type() const;
	bool operator==(const BlastType &otherType) const;
protected:
	BlastType(EBlastType type);
	virtual bool isEqual(const BlastType &otherType) const = 0;
private:
	const EBlastType type_;
};

class BasicBlastType :
	public BlastType
{
public:
	virtual ~BasicBlastType() = default;
protected:
	BasicBlastType(EBlastType type);
};

class VoidBlastType :
	public BasicBlastType
{
public:
	VoidBlastType();
protected:
	bool isEqual(const BlastType &otherType) const override;
};

class BooleanBlastType :
	public BasicBlastType
{
public:
	BooleanBlastType();
protected:
	bool isEqual(const BlastType &otherType) const override;
};


class IntegerBlastType :
	public BasicBlastType
{
public:
	/** 
	@param width number of bits
	@param signedness 0 — unsigned or no signedness semantic, 1 — signed
	**/
	IntegerBlastType(int16_t width, int16_t signedness);

	/* Number of bits */
	int16_t width() const;
	/* 0 — unsigned or no signedness semantic, 1 — signed */
	int16_t signedness() const;
protected:
	bool isEqual(const BlastType &otherType) const override;
private:
	const int16_t width_;
	const int16_t signedness_;
};

class FloatBlastType :
	public BasicBlastType
{
public:
	/**
	@param width number of bits
	**/
	FloatBlastType(int16_t width);

	int16_t width() const;
protected:
	bool isEqual(const BlastType &otherType) const override;
private:
	const int16_t width_;
};

#endif