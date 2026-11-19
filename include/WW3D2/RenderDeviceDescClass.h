// Portions derived from Command & Conquer Generals and Command & Conquer Generals Zero Hour, (c) Electronic Arts Inc., GPL-3.0-or-later; see LICENSE and NOTICE.
// RenderDeviceDescClass.h: one render device's description (Zero Hour's WWVegas/WW3D2/rddesc.h).
// Target facts (retail game.dat): DX8Wrapper::Get_Render_Device_Desc 0x00120140 indexes the table's array (0x00DEE5AC, behind the
// vector's table pointer at 0x00DEE5A8) with a 0x5B8-byte stride. The members are kept as opaque storage until a body reads them: Zero
// Hour's members (device, vendor and driver names, the resolution list) are followed here by a Direct3D 9 adapter identifier and
// capability block (the old views size those as 0x44C and 0x130 bytes).
// SIZE 0x5B8.
#pragma once
#ifndef BFME2_RENDERDEVICEDESCCLASS_H
#define BFME2_RENDERDEVICEDESCCLASS_H

class RenderDeviceDescClass
{
public:
	// Zero Hour's inline comparisons (a description never compares equal; the vector template needs them).
	bool operator==(const RenderDeviceDescClass &src) { return false; }
	bool operator!=(const RenderDeviceDescClass &src) { return true; }

private:
	unsigned char m_opaque[0x5B8];		// +0x000
};

typedef char RenderDeviceDescClassSize[(sizeof(RenderDeviceDescClass) == 0x5B8) ? 1 : -1];

#endif
