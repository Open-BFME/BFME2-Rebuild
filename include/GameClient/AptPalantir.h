// Apt tactical sidebar. The live owner allocates 0x154 bytes before
// constructor 0x2D57B6, which installs the table over AptCallbackHost.
#pragma once

#include <stddef.h>
#include "Common/AsciiString.h"
#include "Common/StlMap.h"
#include "GameClient/AptCallbackHost.h"

class GameWindow;
class RadarWindowOverrideSource;
class Xfer;

class AptPalantir : public AptCallbackHost
{
public:
	AptPalantir(RadarWindowOverrideSource *owner, int level);
	virtual ~AptPalantir();

	void OnBttnSpellStore(const char *name);
	void OnBttnOptions(const char *name);
	void OnBttnObserveNextPlayer(const char *name);
	void OnBttnObservePriorPlayer(const char *name);
	void OnBttnMessenger(const char *name);
	void OnClosed(const char *name);	// 0x002D2FE3: hides the sidebar's Apt level and flags it closed
	void update();
	void xfer(Xfer *xfer);

private:
	RadarWindowOverrideSource *m_owner;  // +0x58, forwarded to hasOverrideWindow
	int m_level;
	unsigned char m_pendingFlags;
	GameWindow *m_inputWindow;  // +0x64, released through the window manager
	unsigned char m_unrecovered68[0x10];
	GameWindow *m_radarWindow;  // +0x78, updated by its window slot
	bool m_unk0x7C;
	bool m_unk0x7D;
	unsigned char m_unk0x7E;
	unsigned char m_unk0x7F;
	int m_unk0x80;
	int m_unk0x84;
	int m_unk0x88;
	unsigned char m_unrecovered8C[0x58];
	bool m_unk0xE4;
	bool m_unk0xE5;
	int m_frameState;  // +0xE8, compared with ComputeFrameState
	unsigned char m_unrecoveredEC[0x0C];
	AsciiString m_displayedResourceText;  // +0xF8, lifetime and comparison at 0x2D6A03
	unsigned char m_unrecoveredFC[0x20];
	_STL::map<int, bool> m_flashStates;  // +0x11C, snapshot records are integer/boolean pairs
	bool m_unk0x128;
	unsigned char m_unrecovered129[0x27];
	bool m_unk0x150;

	friend struct AptPalantirLayout;
};

struct AptPalantirLayout
{
	typedef char Owner[offsetof(AptPalantir, m_owner) == 0x58 ? 1 : -1];
	typedef char Windows[offsetof(AptPalantir, m_radarWindow) == 0x78 ? 1 : -1];
	typedef char Flash[offsetof(AptPalantir, m_flashStates) == 0x11C ? 1 : -1];
	typedef char Size[sizeof(AptPalantir) == 0x154 ? 1 : -1];
};
