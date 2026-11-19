// Portions derived from Command & Conquer Generals and Command & Conquer Generals Zero Hour, (c) Electronic Arts Inc., GPL-3.0-or-later; see LICENSE and NOTICE.
// TempIndexStruct.h: the sorting renderer's temporary index record (Zero Hour's sortingrenderer.cpp): three short
// indices, the vertex index and the view-space depth the polygons are sorted by. Target facts: the comparison
// operators 0x0012D2F0..0x0012D370 compare the float at +8, and Get_Temp_Index_Array 0x0012D390 allocates 0xC
// bytes per element.
#pragma once
#ifndef BFME2_TEMPINDEXSTRUCT_H
#define BFME2_TEMPINDEXSTRUCT_H

struct TempIndexStruct
{
	unsigned short tri[3];	// +0x00
	unsigned short idx;		// +0x06
	float z;				// +0x08
};
// SIZE 0xC
typedef char TempIndexStructSize[(sizeof(TempIndexStruct) == 0xC) ? 1 : -1];

bool operator <(const TempIndexStruct &l, const TempIndexStruct &r);	// 0x0012D2F0
bool operator <=(const TempIndexStruct &l, const TempIndexStruct &r);	// 0x0012D310
bool operator >(const TempIndexStruct &l, const TempIndexStruct &r);	// 0x0012D330
bool operator >=(const TempIndexStruct &l, const TempIndexStruct &r);	// 0x0012D350
bool operator ==(const TempIndexStruct &l, const TempIndexStruct &r);	// 0x0012D370

#endif
