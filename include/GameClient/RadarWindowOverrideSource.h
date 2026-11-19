#pragma once
// RadarWindowOverrideSource: what the game's radar window lookups consult before falling back to
// "ControlBar.wnd:LeftHUD" (Radar::newMap 0x002D7B6B, ControlBar::reset and update test the global too).
// Only the two accessors the map names are declared; the layout is not recovered (both read the
// record at +0x10: hasOverrideWindow 0x002D35D6 tests its flags byte at +0x60 against 7, the window
// is its pointer at +0x64). TheRadarWindowOverrideSource's VA is 0x00DFF028.
#include "Common/GameType.h"
class GameWindow;

class RadarWindowOverrideSource
{
public:
	bool hasOverrideWindow() const;			// 0x002D35D6
	GameWindow *getOverrideWindow() const;	// 0x002D35CF
	// The script actions' forwarders (ScriptActions 0x003BD270..0x003BD459: each calls one of these through TheRadarWindowOverrideSource;
	// the map spells them on one placeholder class each). Names are roles from the callers (the control-bar buttons they flash).
	void setPlanningModeEnabled(unsigned char enable);	// 0x002D37BD (map: ?SetPlanningModeEnabled@Palantir@@QAEXE@Z)
	void flashSpellStoreButton(Int frames);	// 0x002D381D
	void flashObjectivesButton(Int frames);	// 0x002D382E
	void flashPlanningModeButton(Int frames);	// 0x002D383F
	void flashHeroSelectButton(Int slot, Int frames);	// 0x002D36AE
	void flashSelectBuilderButton(Int frames);	// 0x002D36C3 (the map types its parameter void *)
};

extern RadarWindowOverrideSource *theRadarWindowOverrideSource;
