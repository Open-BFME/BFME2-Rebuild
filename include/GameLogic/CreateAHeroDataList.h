// CreateAHeroDataList.h: the Create-a-Hero manager's hero list (the member at CreateAHeroManager +0x174, SIZE 0x10).
// The map names the class's methods under the placeholder Rva0040A3F9 (0x0040A32F, 0x0040A3B1, 0x0040A3F9,
// 0x0040A441, 0x0040A5CF); its constructor 0x0040A530 and clear 0x0040A59B carry other placeholder names for
// the same object (Rva0040A530, Rva0040A59B), so only the methods the manager's rows call are declared.
//
// Target facts: the object holds a vector of hero-data pointers (begin +0x0, end +0x4, capacity end +0x8; the
// manager's GetDefaultHero 0x0021A6C8 reads begin and end itself and indexes through 0x0040A32F, which returns
// null past the end) and one flag byte at +0xC (the constructor 0x0040A530 stores its argument there; the
// clear 0x0040A59B deletes the pointed-to heroes only when it is set).
#pragma once
#ifndef BFME2_CREATEAHERODATALIST_H
#define BFME2_CREATEAHERODATALIST_H

#include <stddef.h>
#include "Common/GameType.h"
#include "Common/AsciiString.h"

class CreateAHeroData;

class Rva0040A3F9
{
public:
	CreateAHeroData *rva0040A32F(Int index);					// 0x0040A32F: the hero at index, or null
	CreateAHeroData *findHeroData(Int id);								// 0x0040A3B1: the hero with this id, or null
	Int findIndex(CreateAHeroData *hero) const;							// 0x0040A3F9: byte offset of hero (the end when null)
	Bool Add(CreateAHeroData *hero);									// 0x0040A384 (the map spells it Add@Rva005B5C02List: AptCreateAHero::Manager::replaceSelectedHero 0x005B5C02 files the replaced hero)
	Bool rva0040A441(CreateAHeroData *hero);							// 0x0040A441
	CreateAHeroData *rva0040A5CF(const AsciiString &name);				// 0x0040A5CF

	CreateAHeroData **m_begin;											// +0x00
	CreateAHeroData **m_end;											// +0x04
	CreateAHeroData **m_capacityEnd;									// +0x08
	Bool m_ownsHeroes;													// +0x0C
	unsigned char m_pad0D[3];											// +0x0D..+0x0F
	UnsignedInt size() const { return (UnsignedInt)(m_end - m_begin); }
};
typedef char Rva0040A3F9_size[(sizeof(Rva0040A3F9) == 0x10) ? 1 : -1];

#endif
