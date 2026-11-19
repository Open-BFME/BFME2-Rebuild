// Portions derived from Command & Conquer Generals and Command & Conquer Generals Zero Hour, (c) Electronic Arts Inc., GPL-3.0-or-later; see LICENSE and NOTICE.
// W3DGameClient: the Direct3D game client (TheGameClient's concrete class; Zero Hour's W3DGameClient.h).
// Target facts: it adds no data members (the factories 0x0004C45D.. allocate the subsystems it creates; the table
// VA 0x00BC5AE8-equivalent overrides the GameClient slots 37..51 with 0x0004C45D .. 0x0004C8DD and slots 21, 28, 30
// and 32 with createRayEffectByTemplate 0x0004C7F0, friend_createDrawable 0x0004C75F, setTimeOfDay 0x0004C884 and
// setTeamColor 0x0004C8BC). Only the overrides whose rows are written carry their slot comment.
#pragma once
#ifndef BFME2_W3DGAMECLIENT_H
#define BFME2_W3DGAMECLIENT_H

#include "GameClient/GameClient.h"

class W3DGameClient : public GameClient
{
public:
	virtual void *rva0004C45D();								// slot 37: new W3DDisplay
	virtual void *createInGameUI();								// slot 38
	virtual void *rva0004C4C7();								// slot 39
	virtual W3DDisplayStringManager *rva0004C553();				// slot 41: new W3DDisplayStringManager
	virtual Keyboard *rva0004C6D4();							// slot 44: new DirectInputKeyboard
protected:
	virtual FontLibrary *createFontLibrary();			// slot 40, 0x0004C4F9: new W3DFontLibrary (the map spells it protected)
public:
protected:
	virtual SnowManager *createSnowManager();						// slot 46, 0x0004C5F8: new W3DSnowManager (the map spells it protected)
public:
	virtual void setFrameRate(Real msecsPerFrame);			// slot 51, 0x0004C6C6
	virtual void createRayEffectByTemplate(const Coord3D *start, const Coord3D *end, const ThingTemplate *tmpl);	// slot 21, 0x0004C7F0
	virtual Drawable *friend_createDrawable(const ThingTemplate *thing, DrawableStatus statusBits, Int value);	// slot 28, 0x0004C75F
	virtual void setTeamColor(Int red, Int green, Int blue);	// slot 32, 0x0004C8BC
};

typedef char W3DGameClientSize[(sizeof(W3DGameClient) == 0x140) ? 1 : -1];

#endif
