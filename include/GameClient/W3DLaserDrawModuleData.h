// W3DLaserDrawModuleData.h: the INI data of the laser draw module (promoted from the Draft).
// Target facts (retail game.dat 1.06):
// - Derives from ModuleData and only overrides slot 0 (table 0x007CB960). Constructor 0x000C9A6F
//   sets the two colours to -1, NumBeams and Segments to 1, OuterBeamWidth and TilingScalar to 1
//   and the other numbers to 0, then builds the texture list (+0x2C, an empty vector) and
//   initialises the 0x28-byte Envelope at +0x4C. Destructor 0x000C9AE7 destroys the texture list.
//   buildFieldParse 0x000C928F adds the 15-entry table at 0x007CB840 (INI tokens NumBeams ..
//   Envelope, offsets below).
#pragma once
#ifndef BFME2_W3DLASERDRAWMODULEDATA_H
#define BFME2_W3DLASERDRAWMODULEDATA_H

#include <stddef.h>
#include "Common/GameType.h"
#include "Common/AsciiString.h"
#include "Common/Vector.h"
#include "GameLogic/Module/ModuleData.h"

class MultiIniFieldParse;

class W3DLaserDrawModuleData : public ModuleData
{
public:
	W3DLaserDrawModuleData();													// 0x000C9A6F
	virtual ~W3DLaserDrawModuleData();											// 0x000C9AE7
	static void buildFieldParse(MultiIniFieldParse &parse);					// 0x000C928F

	Int InnerColor;																// 0x08 InnerColor (parseColorInt)
	Int OuterColor;																// 0x0C OuterColor
	Real InnerBeamWidth;														// 0x10
	Real OuterBeamWidth;														// 0x14
	Real ScrollRate;															// 0x18
	Bool Tile;																	// 0x1C
	UnsignedInt NumBeams;														// 0x20
	UnsignedInt MaxIntensityLifetime;											// 0x24 duration, frames
	UnsignedInt FadeLifetime;													// 0x28 duration, frames
	_STL::vector<AsciiString> Textures;										// 0x2C INI token Texture (parseAsciiStringVectorAppend)
	UnsignedInt Segments;														// 0x38
	Real ArcHeight;																// 0x3C
	Real SegmentOverlapRatio;													// 0x40
	Real TilingScalar;															// 0x44
	Real FanWidth;																// 0x48
	UnsignedByte Envelope[0x28];												// 0x4C INI token Envelope: initialised by 0x000C9251, parsed by 0x00338E52 (layout not recovered)
};
// SIZE 0x74
typedef char W3DLaserDrawModuleData_size[(sizeof(W3DLaserDrawModuleData) == 0x74) ? 1 : -1];
typedef char W3DLaserDrawModuleData_off_Textures[(offsetof(W3DLaserDrawModuleData, Textures) == 0x2C) ? 1 : -1];
typedef char W3DLaserDrawModuleData_off_Envelope[(offsetof(W3DLaserDrawModuleData, Envelope) == 0x4C) ? 1 : -1];

#endif
