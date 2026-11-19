// GameSpyConfig.h: the interface to the GameSpy configuration object (Zero Hour's GameSpyConfigInterface by role and slot order,
// GPL-3.0-or-later; the slot numbers and the global are retail's).
// Portions derived from Command & Conquer Generals Zero Hour, (c) Electronic Arts Inc., GPL-3.0-or-later; see LICENSE and NOTICE.
// Target facts (code RVAs, global VA 0x00E05FB4 = RVA 0x00A05FB4):
//  - SetUpGameSpy 0x00386F7F stores the object GameSpyConfigInterface::create 0x0054F4AF builds in the global and TearDownGameSpy
//    0x00385956 clears it.
//  - The slots callers use: 3 (+0x0C) is getPingTimeoutInMs (GameSpyInfo::getPingValue 0x0038277F: `mov ecx,[0xE05FB4]; mov eax,[ecx];
//    call [eax+0xC]`), 8 (+0x20) is getQMChannel (joinBestGroupRoom 0x00385678). The concrete table (GameSpyConfig, GSConfig.cpp
//    rows) holds getPingServers 0x0054F3E2 at slot 1, getQMMaps 0x0054F3FD at slot 6, isPlayerVIP 0x0054E86D at slot 10,
//    getManglerLocation 0x0054E7EA at slot 11 and getLeftoverConfig 0x0054F3C7 at slot 12, with the one-line getters retail folds
//    (slots 2, 4, 5, 7, 9 and 13..19) in between; Zero Hour's interface has the same order up to slot 12 except that BFME 2 has no
//    getPointsForRank. The BFME 2 slots past 12 are not named here.
#pragma once
#ifndef BFME2_GAMESPYCONFIG_H
#define BFME2_GAMESPYCONFIG_H

#include <list>
#include "Common/GameType.h"
#include "Common/AsciiString.h"

class GameSpyConfigInterface
{
public:
	virtual ~GameSpyConfigInterface() {}											// slot 0x00
	virtual _STL::list<AsciiString> getPingServers() = 0;							// slot 0x04, 0x0054F3E2
	virtual Int getNumPingRepetitions() = 0;										// slot 0x08
	virtual Int getPingTimeoutInMs() = 0;											// slot 0x0C
	virtual Int getPingCutoffGood() = 0;											// slot 0x10
	virtual Int getPingCutoffBad() = 0;												// slot 0x14
	virtual _STL::list<AsciiString> getQMMaps() = 0;								// slot 0x18, 0x0054F3FD
	virtual Int getQMBotID() = 0;													// slot 0x1C
	virtual Int getQMChannel() = 0;													// slot 0x20
	virtual void setQMChannel(Int channel) = 0;										// slot 0x24 (retail's target 0x003FF328 is the shared one-store body)
	virtual Bool isPlayerVIP(Int id) = 0;											// slot 0x28, 0x0054E86D
	virtual Bool getManglerLocation(Int index, AsciiString &host, UnsignedShort &port) = 0;	// slot 0x2C, 0x0054E7EA
	virtual AsciiString getLeftoverConfig() = 0;									// slot 0x30, 0x0054F3C7
};

extern GameSpyConfigInterface *TheGameSpyConfig;		// VA 0x00E05FB4 (the map's name ?TheGameSpyConfig@@3PAVGameSpyConfigInterface@@A)

#endif
