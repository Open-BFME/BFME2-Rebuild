// AnimateWindowManager.h: slides windows on and off screen. AnimateWindow is one
// window's animation record; AnimateWindowManager owns the lists of them and one
// ProcessAnimateWindow per animation style (GameClient/ProcessAnimateWindow.h).
//
// Layout evidence (retail game.dat): AnimateWindow's constructor 0x0053B2CC clears
// +0x04..+0x3C, the two floats at +0x2C/+0x30 and the bytes at +0x40/+0x41, so it is
// 0x44 bytes with a vftable (0x00C692AC, one slot, the deleting destructor 0x0053B36B).
// setAnimData 0x0053B31C copies its twelve arguments to +0x08..+0x38 in that order
// (start, end, current and rest positions, the velocity, then the start and end
// frames); +0x28 (the window) and +0x3C (the style) are not part of it. The
// process classes read +0x04 as the delay, +0x34 as the start time and write
// +0x41 (finished). AnimateWindowManager's constructor 0x0053B550 builds a
// SubsystemInterface, two lists at +0x0C/+0x10, two flag bytes at +0x14/+0x15 and
// news one process object per style into +0x18..+0x34 (size 0x38); its table
// 0x008692B0 overrides init, reset and update. Names are Zero Hour's where the
// retail row names agree (the map carries them).
#pragma once
#ifndef BFME2_ANIMATEWINDOWMANAGER_H
#define BFME2_ANIMATEWINDOWMANAGER_H

#include "Common/SubsystemInterface.h"
#include "Common/ICoord2D.h"
#include "Common/Coord2D.h"
#include "Common/StlList.h"

class GameWindow;
class ProcessAnimateWindow;
class ProcessAnimateWindowSlideFromRight;
class ProcessAnimateWindowSlideFromRightFast;
class ProcessAnimateWindowSlideFromTop;
class ProcessAnimateWindowSlideFromTopFast;
class ProcessAnimateWindowSlideFromLeft;
class ProcessAnimateWindowSlideFromBottom;
class ProcessAnimateWindowSpiral;
class ProcessAnimateWindowSlideFromBottomTimed;

enum AnimTypes;

class AnimateWindow
{
public:
	AnimateWindow();													// 0x0053B2CC
	virtual ~AnimateWindow();											// 0x0053B311

	void setAnimData(ICoord2D startPos, ICoord2D endPos, ICoord2D curPos, ICoord2D restPos,
		Coord2D vel, unsigned int startTime, unsigned int endTime);		// 0x0053B31C

	// Inline: their out-of-line copies (getStartPos 0x005C4FA5, getEndPos
	// 0x005C4FB7, setStartPos 0x005C4FC9, setEndPos 0x005C4FDA, getCurPos
	// 0x005F8376) have no callers; ProcessAnimateWindow reads +0x08/+0x10/+0x18
	// in place. getRestPos stays out of line (its row matches).
	ICoord2D getStartPos() { return m_startPos; }
	ICoord2D getEndPos() { return m_endPos; }
	ICoord2D getCurPos() { return m_curPos; }
	void setCurPos(ICoord2D pos) { m_curPos = pos; }
	unsigned int getStartTime() { return m_startTime; }
	ICoord2D getRestPos();																		// 0x0053B25C
	void setStartPos(ICoord2D pos) { m_startPos = pos; }
	void setEndPos(ICoord2D pos) { m_endPos = pos; }
	Coord2D getVel();													// 0x005C5046
	// Inline: ProcessAnimateWindow's bodies store +0x2C/+0x30, +0x34 and +0x41
	// in place and read +0x04, +0x28 and +0x41 in place (e.g. Spiral
	// initReverseAnimateWindow 0x005C5BE2, SlideFromRight reverseAnimateWindow
	// 0x005C51E5); the out-of-line setVel copy 0x005C5058 is the COMDAT a
	// non-inlined call leaves.
	void setVel(Coord2D vel) { m_vel = vel; }
	void setStartTime(unsigned int t) { m_startTime = t; }
	unsigned int getDelay() { return m_delay; }
	GameWindow *getGameWindow() { return m_win; }
	bool isFinished() { return m_isFinished; }
	void setNeedsToFinish(bool needsToFinish);							// 0x00203574 (stores +0x40; its row sits in ScriptEngine.cpp's extent)
	void setFinished(bool finished) { m_isFinished = finished; }

	unsigned int m_delay;												// +0x04
	ICoord2D m_startPos;												// +0x08
	ICoord2D m_endPos;													// +0x10
	ICoord2D m_curPos;													// +0x18
	ICoord2D m_restPos;													// +0x20
	GameWindow *m_win;													// +0x28
	Coord2D m_vel;														// +0x2C
	unsigned int m_startTime;											// +0x34
	unsigned int m_endTime;												// +0x38
	int m_animType;														// +0x3C (AnimTypes)
	bool m_needsToFinish;												// +0x40
	bool m_isFinished;													// +0x41
};
// SIZE 0x44

class AnimateWindowManager : public SubsystemInterface
{
public:
	AnimateWindowManager();												// 0x0053B550
	virtual ~AnimateWindowManager();									// 0x0053B6E0
	virtual void init();												// 0x0053B7E4
	virtual void reset();												// 0x0053B803
	virtual void update();												// 0x0053B387

	void reverseAnimateWindow();										// 0x0053B417
	bool isFinished() { return m_needsUpdate == false; }					// inline: Shell::isAnimFinished 0x0035BE55 reads +0x14 against 0
	void resetToRestPosition();											// 0x0053B4B7
	void registerGameWindow(GameWindow *win, AnimTypes animType, bool needsToFinish,
		unsigned int millisecondsToFinish, unsigned int delayMs);		// 0x0053B843

	typedef _STL::list<AnimateWindow *> AnimateWindowList;

	AnimateWindowList m_winList;										// +0x0C
	AnimateWindowList m_winMustFinishList;								// +0x10
	bool m_needsUpdate;													// +0x14
	bool m_reverse;														// +0x15
	ProcessAnimateWindowSlideFromRight *m_slideFromRight;				// +0x18
	ProcessAnimateWindowSlideFromRightFast *m_slideFromRightFast;		// +0x1C
	ProcessAnimateWindowSlideFromTop *m_slideFromTop;					// +0x20
	ProcessAnimateWindowSlideFromLeft *m_slideFromLeft;					// +0x24
	ProcessAnimateWindowSlideFromBottom *m_slideFromBottom;				// +0x28
	ProcessAnimateWindowSpiral *m_spiral;								// +0x2C
	ProcessAnimateWindowSlideFromBottomTimed *m_slideFromBottomTimed;	// +0x30
	ProcessAnimateWindowSlideFromTopFast *m_slideFromTopFast;			// +0x34

private:
	ProcessAnimateWindow *getProcessAnimate(AnimTypes animType);		// 0x0053B26E: one process object per style 1..8
};
// SIZE 0x38

#endif
