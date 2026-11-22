// Portions derived from Command & Conquer Generals and Command & Conquer Generals Zero Hour, (c) Electronic Arts Inc., GPL-3.0-or-later; see LICENSE and NOTICE.
// RenderDeviceDescClass.h: one render device's description (Zero Hour's WWVegas/WW3D2/rddesc.h).
// Target facts (retail game.dat): DX8Wrapper::Get_Render_Device_Desc 0x00120140 indexes the table's array (0x00DEE5AC, behind the
// vector's table pointer at 0x00DEE5A8) with a 0x5B8-byte stride. The members before the resolution list are kept as opaque storage
// until a body reads them: Zero Hour's nine name pointers (0x24 bytes), then a Direct3D 9 adapter identifier and capability block
// (0x44C and 0x130 bytes). The display-mode count and getter in W3DDisplay.cpp read the resolution list at +0x5A0 (its element
// pointer at +0x5A4, count at +0x5B0) with a 0x10-byte element stride: width, height and bit depth at +0, +4 and +8.
// SIZE 0x5B8.
#pragma once
#ifndef BFME2_RENDERDEVICEDESCCLASS_H
#define BFME2_RENDERDEVICEDESCCLASS_H

#include "WWLib/vector.h"

// One display mode the device offers (Zero Hour's rddesc.h). Zero Hour's constructors are left out: no body here calls
// them, and a definition here is emitted as code in every unit that includes this header. The comparisons stay because
// DynamicVectorClass<T>::ID instantiates them.
class ResolutionDescClass
{
public:
	bool operator==(const ResolutionDescClass &src) { return ((Width == src.Width) && (Height == src.Height) && (BitDepth == src.BitDepth)); }
	bool operator!=(const ResolutionDescClass &src) { return ((Width != src.Width) || (Height != src.Height) || (BitDepth != src.BitDepth)); }

	int Width;				// +0x0
	int Height;				// +0x4
	int BitDepth;			// +0x8
	int RefreshRate;		// +0xC
};

typedef char ResolutionDescClassSize[(sizeof(ResolutionDescClass) == 0x10) ? 1 : -1];

class RenderDeviceDescClass
{
public:
	// Zero Hour's inline comparisons (a description never compares equal; the vector template needs them).
	bool operator==(const RenderDeviceDescClass &src) { return false; }
	bool operator!=(const RenderDeviceDescClass &src) { return true; }

private:
	unsigned char m_opaque[0x5A0];		// +0x000

public:
	// Zero Hour reaches this through Enumerate_Resolutions; the accessor is left out for the same reason as above.
	DynamicVectorClass<ResolutionDescClass> ResArray;	// +0x5A0
};

typedef char RenderDeviceDescClassSize[(sizeof(RenderDeviceDescClass) == 0x5B8) ? 1 : -1];

#endif
