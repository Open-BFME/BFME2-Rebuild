// AptFileTransferPopup.h: the map transfer popup ("FileTransfer" Apt callbacks, one status row per player while a map is sent).
// Target facts (retail game.dat, RVAs): an AptCallbackHost screen (0x58 bytes) whose first own member is the game being set up at
// +0x58 (FileTransfer::PlayerColor 0x00583034 reads the slot's apparent colour through it); AptFileTransferPopup's rows read the
// eight display-slot ints at +0x5C (ProcessProgress 0x00583121 maps a player to the status row it shows, -1 for none).
// The ledger spells PlayerColor FileTransfer:: and the rest AptFileTransferPopup::, so FileTransfer holds the game pointer and
// AptFileTransferPopup derives it.
#pragma once
#ifndef BFME2_APTFILETRANSFERPOPUP_H
#define BFME2_APTFILETRANSFERPOPUP_H

#include "GameClient/AptCallbackHost.h"
#include "Common/UnicodeString.h"

class GameInfo;

class FileTransfer : public AptCallbackHost
{
public:
	void PlayerColor(int slot, char *result, bool skip);			// 0x00583034: the apparent colour of a slot as a decimal ("0" out of range)

	GameInfo *m_game;												// +0x58
};

class AptFileTransferPopup : public FileTransfer
{
public:
	void ProcessProgress(int playerIndex, int completionPercent, UnicodeString statusText);	// 0x00583121: shows the player's status text and bar
	void rva00583202(int totalSeconds);								// 0x00583202: shows the remaining time as minutes and seconds

	int m_displaySlot[8];											// +0x5C
};

#endif
