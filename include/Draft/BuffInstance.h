// BuffInstance.h: one buff slot of BuffManager. SIZE 0x44.
// Layout, proven by the rows below: the constructor 0x00362D25 writes the vptr at +0, a bool at +4,
// ints at +8, +0xC, +0x10 and +0x14, pointers at +0x18 (glow) and +0x1C (thing), three floats at
// +0x20..+0x28, the intensity float at +0x2C, and the embedded GlowMaterial at +0x30.
// TurnOff 0x00362609 reads +4, +0xC and +0x18; the glow object's state is at +8 of the glow.
// The vptr's table is 0x00C17068: destructor, LoadPostProcess (the shared empty body 0x000B3FD0),
// GetSnapshotName, DoXfer 0x003628D3. The destructor 0x00362D6C ends with a store of Snapshot's
// table (0x00BBB554), so the class is a Snapshot; the map spells the transfer DoXfer, which the Snapshot
// header names xfer, so the base is not declared yet and the destructor is held.
// +0x08 type and +0x0C sub type are written by MakeNormalBuff 0x00362518 and transferred as raw
// words by DoXfer; +0x10 duration, +0x14 frame limit (the logic frame at start plus the duration),
// +0x18 glow effect, +0x1C the ThingTemplate whose name DoXfer saves (+0x64), +0x20 a colour,
// +0x2C the intensity.
#pragma once
#ifndef BFME2_BUFF_INSTANCE_H
#define BFME2_BUFF_INSTANCE_H

#include <stddef.h>
#include "Common/GameType.h"
#include "Common/RGBColor.h"
#include "Draft/GlowMaterial.h"

class Xfer;
class ThingTemplate;

// The glow effect a buff holds at +0x18. Only the release call and the state byte at +8 are used here.
// DoXfer is slot 3 (BuffInstance::DoXfer 0x003628D3 calls [vptr+0xC] with the Xfer); the other
// slots are unproven and only hold the place.
class Rva00419BA5
{
public:
	virtual void vfunc_0x00();								// slot 0
	virtual void vfunc_0x04();								// slot 1
	virtual void vfunc_0x08();								// slot 2
	virtual void DoXfer(Xfer *xfer);						// slot 3
	void rva00419BA5();
	unsigned char m_pad04[0x4];
	int m_state;											// +0x08
	float m_intensity;										// +0x0C (MakeNormalBuff 0x00362518 resets it to 1.0)
};

class BuffInstance
{
	friend class BuffManager;
public:
	BuffInstance();											// 0x00362D25
	virtual ~BuffInstance();								// 0x00362D6C (held)
	void TurnOff(bool immediate);							// 0x00362609
	void Update(unsigned int frame);						// 0x003627B8
	bool MakeNormalBuff(int type, int subType, void *thingTemplate, void *drawable, int duration);	// 0x00362518
	bool MakeGlowOutlineBuff(int type, int subType, void *drawable, int duration, int color, float intensity);	// 0x003625C3 (unsupported: asserts)

protected:
	virtual void LoadPostProcess() {}						// slot 1; the shared empty body 0x000B3FD0
	virtual const char *GetSnapshotName() const;			// 0x00362D66 (slot 2)
	virtual void DoXfer(Xfer *xfer);						// 0x003628D3 (slot 3)

public:
	bool m_active;											// +0x04
	Int m_type;												// +0x08
	Int m_subType;											// +0x0C
	Int m_duration;											// +0x10
	UnsignedInt m_frameLimit;								// +0x14 (compared unsigned in Update 0x003627B8)
	Rva00419BA5 *m_glow;									// +0x18
	const ThingTemplate *m_thing;							// +0x1C
	RGBColor m_color;										// +0x20 (DoXfer transfers it as an RGBColor)
	Real m_intensity;										// +0x2C
	GlowMaterial m_material;								// +0x30
};
// SIZE 0x44
typedef char BuffInstance_size[(sizeof(BuffInstance) == 0x44) ? 1 : -1];

#endif
