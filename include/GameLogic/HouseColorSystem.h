// HouseColorSystem.h: the house-colour texture pair subsystem (TheHouseColorSystem, VA 0x00E030A0).
// Target facts: ctor 0x00229557 (26 bytes) and dtor 0x0022958D are rows elsewhere; the class is a SubsystemInterface (0xC) plus two
// AsciiStrings: +0x0C is read as the second argument and +0x10 as the first argument of Rva00136B5E by 0x00417A6B. The INI table at
// VA 0x00C3A600 parses "BaseTexture" into +0x10 and "HouseTexture" into +0x0C (both INI::parseAsciiString).
#pragma once
#ifndef BFME2_HOUSECOLORSYSTEM_H
#define BFME2_HOUSECOLORSYSTEM_H

#include "Common/GameType.h"
#include "Common/AsciiString.h"
#include "Common/SubsystemInterface.h"

class INI;

// 0x00136B5E: registers the house-colour recolour pair (base texture name, house texture name); the map names it
// ?Rva00136B5E@@YAXPBD0@Z and the owner is not recovered.
void Rva00136B5E(const char *baseTexture, const char *houseTexture);

class HouseColorSystem : public SubsystemInterface
{
public:
	HouseColorSystem();											// 0x00229557
	virtual ~HouseColorSystem();								// 0x0022958D (deleting 0x00229571)
	virtual void init() {}										// 0x00417A66 (5 bytes)
	virtual void reset() {}										// slot 9, the shared empty body 0x000B3FD0
	virtual void update() {}									// slot 10, the shared empty body 0x000B3FD0
	void rva00417A6B();											// 0x00417A6B: hands the two texture names to Rva00136B5E

	AsciiString m_houseTexture;									// +0x0C HouseTexture
	AsciiString m_baseTexture;									// +0x10 BaseTexture
};

extern HouseColorSystem *TheHouseColorSystem;					// VA 0x00E030A0

void parseHouseColorDefinition(INI *ini);						// 0x00417A94

#endif
