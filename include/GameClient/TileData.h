// Portions derived from Command & Conquer Generals and Command & Conquer Generals Zero Hour, (c) Electronic Arts Inc., GPL-3.0-or-later; see LICENSE and NOTICE.
// TileData.h: the terrain tile bitmap (W3DDevice/GameClient/TileData.h in Zero Hour). Retail's tile is 16 bits per pixel
// (RGB555): the 64 x 64 base level is 0x2000 bytes at +0x08 and the six mip levels follow it (constructor 0x00111850,
// updateMips 0x00111883, the table of offsets in 0x00111A05).
// Target facts:
//  - base RefCountClass (vftable 0x00BC650C: Delete_This, scalar deleting destructor); TileData's own vftable is 0x00BCFAC0
//    (Delete_This 0x0006CE57 then the scalar deleting destructor 0x00111867); the destructor 0x0011172A stores TileData's
//    vptr, releases the counted pointer at +0x2AB4 and puts RefCountClass's vptr back.
//  - the pointer at +0x2AB4 is a reference-counted object (count at +4, Delete_This in slot 0): 0x00111772 returns it after an
//    Add_Ref, 0x0011178B replaces it (Add_Ref on the new one, Release_Ref on the old one), 0x00111784 returns it as it is.
//    The map spells its class GapCounted.
//  - sizes: the mip levels are 32, 16, 8, 4, 2 and 1 pixels wide, at +0x2008, +0x2808, +0x2A08, +0x2A88, +0x2AA8, +0x2AB0.
#pragma once
#ifndef BFME2_TILEDATA_H
#define BFME2_TILEDATA_H

#include "Common/GameType.h"
#include "WWLib/RefCountClass.h"

class TileData : public RefCountClass
{
protected:
	UnsignedShort m_tileData[64 * 64];			// +0x08 RGB555, the 64 x 64 base level
	UnsignedShort m_tileDataMip32[32 * 32];		// +0x2008
	UnsignedShort m_tileDataMip16[16 * 16];		// +0x2808
	UnsignedShort m_tileDataMip8[8 * 8];		// +0x2A08
	UnsignedShort m_tileDataMip4[4 * 4];		// +0x2A88
	UnsignedShort m_tileDataMip2[2 * 2];		// +0x2AA8
	UnsignedShort m_tileDataMip1[1 * 2];		// +0x2AB0 (one pixel, padded to the next dword)
	RefCountClass *m_gapCounted;				// +0x2AB4

public:
	TileData();									// 0x00111850
	virtual ~TileData();						// 0x0011172A

	RefCountClass *getGapCountedRef();			// 0x00111772: the counted pointer, with a reference added
	RefCountClass *getGapCounted() const;		// 0x00111784
	void setGapCounted(RefCountClass *object);	// 0x0011178B
	void updateMips();							// 0x00111883
	void setFromRgba(const UnsignedByte *src, Int srcPitch);	// 0x00111A05: fills every level from 64 x 64 BGRA pixels, srcPitch bytes a row
	bool hasRGBDataForWidth(int width);			// 0x001117B4 (Zero Hour's name; the width of one of the seven mip levels, 64 down to 1)
};
// SIZE 0x2AB8
typedef char TileDataSize[(sizeof(TileData) == 0x2AB8) ? 1 : -1];

#endif
