// MultiplayerSettings: the MultiplayerSettings INI block and the player colour list
// (TheMultiplayerSettings). A SubsystemInterface. SIZE 0xC4.
// Retail evidence: the constructor 0x003811D8 builds SubsystemInterface, stores the
// table 0x00C191B8, builds the colour map at +0x34 (map ctor 0x003811A2) and two
// MultiplayerColorDefinitions at +0x44 and +0x84, then zeroes +0x0C, +0x14 and
// +0x40, sets +0x10 to 10000, +0x18 to 3, the four Bools at +0x1C..+0x1F and the
// five starting-credit choices +0x20..+0x30 (500..2500). The implicit destructor
// 0x00381293 tears down the same members. Table 0x00C191B8 (14 slots) holds the shared empty
// body 0x000B3FD0 at init, reset and update (overridden, empty) and SubsystemInterface's
// own entries elsewhere, so the class overrides nothing else (slot 6, getFieldParse,
// is the base's NULL return). Field names follow the INI tokens of the parse table
// 0x00C18FB0 (h05's Draft); findMultiplayerColorDefinitionByName 0x00380E34 walks
// the map at +0x34 and compares each value's tooltip name.
#pragma once
#ifndef BFME2_MULTIPLAYERSETTINGS_H
#define BFME2_MULTIPLAYERSETTINGS_H

#include <stddef.h>
#include "Common/GameType.h"
#include "Common/AsciiString.h"
#include "Common/SubsystemInterface.h"
#include "Common/StlMap.h"
#include "Common/MultiplayerColorDefinition.h"

struct FieldParse;

typedef _STL::map<Int, MultiplayerColorDefinition> MultiplayerColorList;

class MultiplayerSettings : public SubsystemInterface
{
public:
	MultiplayerSettings();																// 0x003811D8
	// No user-declared destructor: retail's 0x00381293 stores no table of its own before
	// tearing down the members, so it is the implicit one (emitted with the table).

	virtual void init() {}
	virtual void reset() {}
	virtual void update() {}

	MultiplayerColorDefinition *findMultiplayerColorDefinitionByName(AsciiString name);	// 0x00380E34
	MultiplayerColorDefinition *getColor(Int index);									// 0x00381373
	Int getNumColors() { if (m_numColors == 0) m_numColors = (Int)m_colorList.size(); return m_numColors; }	// inline (AptMpGameSetup::InitAvailableColors 0x004419FA)

	static const FieldParse m_multiplayerSettingsFieldParseTable[];

	Int m_unk0x0C;									// +0x0C
	Int m_unk0x10;									// +0x10, 10000 in the constructor
	Int m_startCountdownTimer;						// +0x14 StartCountdownTimer
	Int m_maxBeaconsPerPlayer;						// +0x18 MaxBeaconsPerPlayer
	Bool m_useShroud;								// +0x1C UseShroud
	Bool m_showRandomPlayerTemplate;				// +0x1D ShowRandomPlayerTemplate
	Bool m_showRandomStartPos;						// +0x1E ShowRandomStartPos
	Bool m_showRandomColor;							// +0x1F ShowRandomColor
	Int m_initialCreditsVeryLow;					// +0x20 InitialCreditsVeryLow
	Int m_initialCreditsLow;						// +0x24 InitialCreditsLow
	Int m_initialCreditsMedium;						// +0x28 InitialCreditsMedium
	Int m_initialCreditsHigh;						// +0x2C InitialCreditsHigh
	Int m_initialCreditsVeryHigh;					// +0x30 InitialCreditsVeryHigh
	MultiplayerColorList m_colorList;				// +0x34
	Int m_numColors;								// +0x40
	MultiplayerColorDefinition m_observerColor;			// getColor uses the observer selection index.
	MultiplayerColorDefinition m_randomColor;			// getColor uses the random selection index.
};
// SIZE 0xC4
typedef char MultiplayerSettingsSize[(sizeof(MultiplayerSettings) == 0xC4) ? 1 : -1];
extern MultiplayerSettings *TheMultiplayerSettings;							// VA 0x00E022F4

#endif
