#pragma once
// GameWindowRenderer: the per-window renderer object a GameWindow keeps at +4 (created by
// GameWindowManager::createWindowRenderer, slot 14). Only the slots the window code calls are declared; the
// class's SIZE and the slot at +4 are not recovered here.
// Evidence (retail): GameWindow::winDrawBorder 0x00313D03 calls slot 0 with the window; winSetPosition
// 0x00313A9E calls slot 2 with the old position then the new one; GameWindow::winSetText 0x0031484A passes the
// string by address to slot 3; winSetFont 0x00313D73 calls slot 4 with the font.
class GameWindow;
class GameFont;
class UnicodeString;
class WinInstanceData;

class GameWindowRenderer {
public:
	virtual void drawBorder(GameWindow *window) = 0;							// slot 0
	virtual void draw(GameWindow *window, WinInstanceData *instData) = 0;		// slot 1: GameWindowManager::drawWindow 0x002C0FC9 calls it after the window's own draw, unless that returned 1
	virtual void positionChanged(int oldX, int oldY, int newX, int newY) = 0;	// slot 2
	virtual void setText(const UnicodeString *text) = 0;						// slot 3
	virtual void setFont(GameFont *font) = 0;									// slot 4
};
