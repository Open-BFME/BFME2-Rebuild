// Portions derived from Command & Conquer Generals and Command & Conquer Generals Zero Hour, (c) Electronic Arts Inc., GPL-3.0-or-later; see LICENSE and NOTICE.
// IOGridParametersStruct.h: the record GridCullSystemClass::Save 0x0071AD30 and Load 0x0071CA00 exchange with a chunk file.
// Target facts: Save clears 0x34 bytes of a stack record, fills it from the grid's members and writes it with a
// 0x34-byte chunk write; the three float vectors come first, then the three cell counts, then the object extent.
#pragma once
#ifndef BFME2_IOGRIDPARAMETERSSTRUCT_H
#define BFME2_IOGRIDPARAMETERSSTRUCT_H

#include "WWLib/iostruct.h"

struct IOGridParametersStruct
{
	IOVector3Struct MinCellSize;	// +0x00
	IOVector3Struct Origin;		// +0x0C
	IOVector3Struct CellDim;	// +0x18
	unsigned long CellCount[3];	// +0x24
	float MaxObjExtent;		// +0x30
};

typedef char IOGridParametersStructSize[(sizeof(IOGridParametersStruct) == 0x34) ? 1 : -1];

#endif
