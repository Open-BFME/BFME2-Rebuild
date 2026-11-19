// MpGameSetupOwner.h: the interface a multiplayer lobby screen gives its embedded AptMpGameSetup panel.
// The map spells the class Rva004444D2 (constructor 0x004444AE, 23-slot table VA 0x0083E020).
//
// Target facts (retail game.dat; AptLanLobby's table VA 0x0083E098 overrides every slot and the lobby embeds this base
// at +0x27C, SIZE 0xC):
//  - +0x00 vptr; +0x04 two 4-byte handles (the constructor builds them with the vector constructor 0x00629512: count 2,
//    element size 4, constructor 0x00326BE6, destructor 0x000B3FD0); getTarget() 0x0043F103 returns the handle at +0x04 +
//    index*4 for index 0 and 1 (AptMpGameSetup's constructor reads both).
//  - The table's 23 slots, with what the lobby's overrides do (AptLanLobby 0x004441B0..0x004456B8; the bodies read the
//    full object through `this - 0x27C`, which is why the map spells them AptLanLobby members):
//     0 destructor (the lobby's adjustor thunk 0x00444691)   1 isHost (TheLAN->AmIHost)     2 saveRules
//     3 requestAccept(int)    4 selectColor    5 selectHandicap    6 selectHero    7 mapChanged(AsciiString*)    8 setScenario(int)
//     9 selectPlayer(slot, state, name)   10 selectPlayerTemplate   11 selectStartPosition   12 selectTeam   13 stub
//    14 getLocalPlayerName(UnicodeString&)   15 enableSetupUI   16 setBusy(bool)   17 printMessage(UnicodeString&, int)
//    18 (bool, int) start/ready update   19 returns 0   20 creates a 12-byte object   21 getOwnerId (returns the +0x274 value
//       of the lobby)   22 stub.
// The names below are roles; the map spells the lobby's overrides non-virtual (QAE), so the lobby declares its own
// non-virtual members under the map's names and this interface only fixes the slot order.
#pragma once
#ifndef BFME2_MPGAMESETUPOWNER_H
#define BFME2_MPGAMESETUPOWNER_H

#include "GameNetwork/GameSlot.h"
class AsciiString;
class UnicodeString;
class AptRefTarget;

class MpGameSetupOwner
{
public:
	MpGameSetupOwner();															// 0x004444AE
	virtual ~MpGameSetupOwner();												// slot 0
	virtual bool isHost();														// slot 1
	virtual void saveRules();													// slot 2
	virtual bool MpOwnerOnReadyChecked(bool);									// slot 3
	virtual bool MpOwnerSelectColor(GameSlot *, int);							// slot 4
	virtual bool MpOwnerSelectHandicap(GameSlot *, int);						// slot 5
	virtual bool MpOwnerSelectHero(GameSlot *);									// slot 6
	virtual bool MpOwnerSelectMap(const AsciiString &);							// slot 7
	virtual bool MpOwnerSelectStrategicScenario(int);								// slot 8
	virtual bool MpOwnerSelectPlayer(GameSlot *, SlotState, const UnicodeString &);	// slot 9 (retail's Skirmish 0x00521B16 passes arg 3 to a UnicodeString copy)
	virtual bool MpOwnerSelectPlayerTemplate(GameSlot *, int);					// slot 10
	virtual bool MpOwnerSelectStartPosition(GameSlot *, int);					// slot 11
	virtual bool MpOwnerSelectTeam(GameSlot *, int);								// slot 12
	virtual bool MpOwnerSetClanID(GameSlot *, const UnicodeString &);			// slot 13 (HandleClanChange 0x004409D2 passes the local slot and the new clan text)
	virtual void MpOwnerGetLocalPlayerName(UnicodeString &);					// slot 14
	virtual void enableSetupUI();												// slot 15
	virtual void setBusy(bool);													// slot 16
	virtual void MpOwnerPrintMessage(const UnicodeString &, int);					// slot 17
	virtual void slot18(bool, int);												// slot 18
	virtual int slot19(GameSlot *);											// slot 19 (PlayerTooltip 0x0043DF71 passes the slot under the mouse)
	virtual void *slot20();														// slot 20
	virtual void *getOwnerId();												// slot 21
	virtual void slot22();														// slot 22

	AptRefTarget *getTarget(int index);											// 0x0043F103: m_targets[index]

	AptRefTarget *m_targets[2];													// +0x04
};
typedef char MpGameSetupOwnerSizeCheck[sizeof(MpGameSetupOwner)==0xC ? 1 : -1];

#endif
