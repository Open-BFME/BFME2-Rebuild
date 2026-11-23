// RadialWindowController.h: subsystem managing radial HUD windows.
#pragma once
#ifndef BFME2_RADIALWINDOWCONTROLLER_H
#define BFME2_RADIALWINDOWCONTROLLER_H

#include <stddef.h>
#include <vector>
#include "Common/SubsystemInterface.h"
#include "GameClient/RadialMenuLayoutController.h"

class GameWindow;

class RadialWindowController : public SubsystemInterface, public RadialMenuLayoutController
{
public:
	RadialWindowController();
	virtual ~RadialWindowController();

	virtual void reset();
	virtual void update();

	virtual Bool ShouldButtonFlash(Int index);
	virtual void PositionButton(Int index, const Coord2D &pos, const Coord2D &size);
	virtual void EnableButtonInput(Int index, Bool enable);
	virtual ICoord2D getCenterScreenPos();
	virtual void onLayoutComplete();

	std::vector<GameWindow *> m_windows;
	Int m_unk0x54;
	Int m_unk0x58;
};

typedef char RadialWindowControllerSize[(sizeof(RadialWindowController) == 0x5C) ? 1 : -1];
typedef char RadialWindowControllerWindowsOffset[(offsetof(RadialWindowController, m_windows) == 0x48) ? 1 : -1];

#endif
