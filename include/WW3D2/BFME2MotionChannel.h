// The W3D compressed animation channels used by BFME 2's chunk 0x284.
// The class and the sampling names describe roles recovered from retail.
// 0x1A465B and 0x1A466C install the seven-slot table at 0x7D6CB4, whose
// only concrete entry is the deleting destructor 0x1A4697. The factory
// 0x1A46B4 fills the four fields below; 0x190210 and 0x190218 confirm the
// pivot and type, and 0x1B2168 and 0x1B2E69 use the final two for payload size.
#pragma once
#ifndef BFME2_MOTION_CHANNEL_H
#define BFME2_MOTION_CHANNEL_H

class ChunkLoadClass;
class Quaternion;
class Vector3;

// The 8-byte header that precedes a motion channel chunk (Load_BFME2MotionChannel reads it first).
struct BFME2MotionChannelHeader
{
	unsigned char reserved;			// must be zero
	unsigned char encoding;			// 0, 1 or 2: selects the channel class
	unsigned char components;
	unsigned char type;
	unsigned short count;
	unsigned short pivot;
};

class BFME2MotionChannel
{
public:
	BFME2MotionChannel();
	virtual bool Load(ChunkLoadClass &chunk) = 0;
	virtual ~BFME2MotionChannel();
	virtual int Get_Cache_Size() = 0;
	// VC7.1 reverses this overload group in the vtable. Native slots 3-5
	// evaluate float, Vector3, and Quaternion in that order.
	virtual void Evaluate(float frame, Quaternion *value, unsigned char **cursor) = 0;
	virtual void Evaluate(float frame, Vector3 *value, unsigned char **cursor) = 0;
	virtual void Evaluate(float frame, float *value, unsigned char **cursor) = 0;
	virtual int Get_Channel_Memory_Usage() = 0;

	int Type;
	int Pivot;
	int Count;
	int Components;
};

typedef char BFME2MotionChannelSize[(sizeof(BFME2MotionChannel) == 0x14) ? 1 : -1];

// The stream header holds one scale and up to four initial components. The
// serialized payload follows separately. The Stream loader 0x1B2168 starts
// its two reads at +0x14 and +0x18 and owns the allocation at +0x28.
class BFME2StreamMotionChannel : public BFME2MotionChannel
{
public:
	BFME2StreamMotionChannel();
	virtual ~BFME2StreamMotionChannel();
	virtual bool Load(ChunkLoadClass &chunk);
	virtual int Get_Cache_Size();

protected:
	float Scale;
	float Initial[4];
	unsigned char *Data;
};

class BFME2Encoding1MotionChannel : public BFME2StreamMotionChannel
{
public:
	virtual void Evaluate(float frame, float *value, unsigned char **cursor);
	virtual void Evaluate(float frame, Vector3 *value, unsigned char **cursor);
	virtual void Evaluate(float frame, Quaternion *value, unsigned char **cursor);
	virtual int Get_Channel_Memory_Usage();
};

class BFME2Encoding2MotionChannel : public BFME2StreamMotionChannel
{
public:
	virtual void Evaluate(float frame, float *value, unsigned char **cursor);
	virtual void Evaluate(float frame, Vector3 *value, unsigned char **cursor);
	virtual void Evaluate(float frame, Quaternion *value, unsigned char **cursor);
	virtual int Get_Channel_Memory_Usage();
};

class BFME2Encoding0MotionChannel : public BFME2MotionChannel
{
public:
	BFME2Encoding0MotionChannel();
	virtual ~BFME2Encoding0MotionChannel();
	virtual bool Load(ChunkLoadClass &chunk);
	virtual int Get_Cache_Size() { return sizeof(int); }
	virtual void Evaluate(float frame, float *value, unsigned char **cursor);
	virtual void Evaluate(float frame, Vector3 *value, unsigned char **cursor);
	virtual void Evaluate(float frame, Quaternion *value, unsigned char **cursor);
	virtual int Get_Channel_Memory_Usage();

private:
	unsigned short *TimeCodes;
	float *Samples;
};

typedef char BFME2StreamMotionChannelSize[(sizeof(BFME2StreamMotionChannel) == 0x2C) ? 1 : -1];
typedef char BFME2Encoding0MotionChannelSize[(sizeof(BFME2Encoding0MotionChannel) == 0x1C) ? 1 : -1];
typedef char BFME2Encoding1MotionChannelSize[(sizeof(BFME2Encoding1MotionChannel) == 0x2C) ? 1 : -1];
typedef char BFME2Encoding2MotionChannelSize[(sizeof(BFME2Encoding2MotionChannel) == 0x2C) ? 1 : -1];

#endif
