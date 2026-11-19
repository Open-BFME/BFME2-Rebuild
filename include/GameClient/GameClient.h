// GameClient.h: the client subsystem (TheGameClient). SIZE 0x140.
// Target facts (code RVAs, tables VAs):
//  - Bases SubsystemInterface +0x00 and Snapshot +0x0C: ctor 0x0023BC36 calls
//    SubsystemInterface(), stores Snapshot's table at +0x0C, then 0x00BED850
//    (+0x00, 52 slots) and 0x00BED840 (+0x0C, 4 slots).
//  - Size 0x140: Win32GameEngine 0x00041D62 allocates 0x140 for W3DGameClient,
//    whose ctor 0x0004C43E only replaces the two vptrs.
//  - Slots 14..34, in retail order. Names: the map's rows for GameClient's own
//    bodies and W3DGameClient's overrides of the __purecall slots (21, 22, 28,
//    32; W3DGameClient table 0x00BC4738); the order is Zero Hour's GameClient.
//    Folded bodies: slot 14 0x005F69CE stores +0x10 (setFrame), slot 17
//    0x0057E556 returns +0x14 (firstDrawable), slot 31 0x001DB09D returns +0x10
//    (getFrame); these are inline, as Zero Hour has them. Slot 35 is a second
//    +0x14 getter (0x0057E556 again; the map's other accepted name for that
//    address is getDrawableList), slot 36 is 0x00239911 (frees the deferred
//    destroy list at +0xE4; update's last call and GameLogic::
//    destroyAllObjectsImmediate's tail call), and slots 37..51 are __purecall in
//    this table: the factories W3DGameClient 0x007C4738 overrides, in Zero
//    Hour's declaration order (display, in-game UI, window manager, font
//    library, display strings, video player, terrain visual, keyboard, mouse,
//    snow manager) then four more BFME 2 factories and setFrameRate. Names
//    only where the map's row for the override carries one; the rest keep the
//    address spelling until the map names them.
//  - Members: +0x10 the frame, +0x14 the drawable list (registerDrawable
//    0x00238E25 prepends to it), +0x18 the drawable hash findDrawableByID
//    0x00239C21 searches (hash_map ctor 0x0023BC17), +0x2C the next drawable
//    id (1 in the ctor; registerDrawable post-increments it).
// Module units also reach findDrawableByID at +0x40, destroyDrawable at +0x74
// and getFrame at +0x7C. The frames-per-millisecond global is the float at
// 0x00DBA500.
#pragma once
#ifndef BFME2_GAMECLIENT_H
#define BFME2_GAMECLIENT_H

#include <stddef.h>
#include "Common/GameType.h"
#include "Common/AsciiString.h"
#include "Common/Snapshot.h"
#include "Common/SubsystemInterface.h"
#include "Common/STLTypedefs.h"
#include "Common/StlHashMap.h"
#include "Common/GameMessageTranslator.h"
#include "GameClient/CommandXlat.h"

class Drawable;
class FrustumClass;
class FontLibrary;
class Keyboard;
class Mouse;
class SnowManager;
class W3DDisplayStringManager;
class ThingTemplate;
class Xfer;
class Coord3D;
struct RayEffectData;
struct Region3D;
enum DrawableStatus;
enum Scorches;
enum TimeOfDay;

typedef void (*GameClientFuncPtr)(Drawable *draw, void *userData);

class Drawable;
// Zero Hour's name: DrawableID -> Drawable * (STLport hash_map, SIZE 0x14).
typedef _STL::hash_map<DrawableID, Drawable *, rts::hash<DrawableID>, _STL::equal_to<DrawableID> > DrawablePtrHash;

class GameClient : public SubsystemInterface, public Snapshot
{
public:
	// One drawable TOC record, the list value (node +8): name at +0, id at +4.
	struct DrawableTOCEntry
	{
		AsciiString name;
		UnsignedShort id;
	};

	GameClient();												// 0x0023BC36
	virtual ~GameClient();										// 0x0023AE08

	virtual void init();										// slot 1, 0x0023A1BB
	virtual void reset();										// slot 9, 0x0023B278
	virtual void update();										// slot 10, 0x0023BEE8

	virtual void setFrame(UnsignedInt frame) { m_frame = frame; }	// slot 14
	virtual void registerDrawable(Drawable *draw);				// slot 15, 0x00238E25
	virtual Drawable *findDrawableByID(DrawableID id);			// slot 16, 0x00239C21
	virtual Drawable *firstDrawable() { return m_drawableList; }	// slot 17
	virtual GameMessage::Type evaluateContextCommand(Drawable *draw, const Coord3D *pos, CommandTranslator::CommandEvaluateType cmdType);	// slot 18, 0x00238F15
	virtual void removeFromRayEffects(Drawable *draw);			// slot 19, 0x00238F34
	virtual void getRayEffectData(Drawable *draw, RayEffectData *effectData);	// slot 20, 0x00238F29
	virtual void createRayEffectByTemplate(const Coord3D *start, const Coord3D *end, const ThingTemplate *tmpl) = 0;	// slot 21
	virtual void addScorch(const Coord3D *pos, Real radius, Scorches type) = 0;	// slot 22
	virtual Bool loadMap(AsciiString mapName);					// slot 23, 0x00239949
	virtual void unloadMap(AsciiString mapName);				// slot 24, 0x00239982
	virtual void iterateDrawablesInRegion(Region3D *region, GameClientFuncPtr userFunc, void *userData);	// slot 25, 0x00238E4C
	virtual void rva002391AA(const FrustumClass *frustum, Real value, GameClientFuncPtr userFunc, void *userData);	// slot 26, 0x002391AA
	virtual void rva00238EBF(GameClientFuncPtr userFunc, void *userData);	// slot 27, 0x00238EBF
	virtual Drawable *friend_createDrawable(const ThingTemplate *thing, DrawableStatus statusBits, Int value) = 0;	// slot 28
	virtual void destroyDrawable(Drawable *draw);				// slot 29, 0x00239F40
	void removeDrawableFromLookupTable(const Drawable *draw);	// 0x00239C38 (map: a placeholder on Rva00239C38): erases the drawable's ID from the hash at +0x18
	virtual void setTimeOfDay(TimeOfDay tod);					// slot 30, 0x00238EF2
	virtual UnsignedInt getFrame() { return m_frame; }			// slot 31
	virtual void setTeamColor(Int red, Int green, Int blue) = 0;	// slot 32
	virtual void releaseShadows();								// slot 33, 0x00238F3F
	virtual void allocateShadows();								// slot 34, 0x00238F5C
	virtual Drawable *getDrawableList() { return m_drawableList; }	// slot 35, 0x0057E556 (folded with firstDrawable)
	virtual void rva00239911();									// slot 36, 0x00239911

	virtual void *rva0004C45D() = 0;							// slot 37, W3DGameClient 0x0004C45D
	virtual void *createInGameUI() = 0;							// slot 38, 0x0004C492
	virtual void *rva0004C4C7() = 0;							// slot 39, 0x0004C4C7
	virtual FontLibrary *createFontLibrary() = 0;				// slot 40, 0x0004C4F9 (the map spells it protected)
	virtual W3DDisplayStringManager *rva0004C553() = 0;			// slot 41, 0x0004C553
	virtual void *rva0004C585() = 0;							// slot 42, 0x0004C585
	virtual void *rva0004C5C6() = 0;							// slot 43, 0x0004C5C6
	virtual Keyboard *rva0004C6D4() = 0;						// slot 44, 0x0004C6D4
	virtual Mouse *createMouse() = 0;							// slot 45, 0x0004C709
	virtual SnowManager *createSnowManager() = 0;				// slot 46, 0x0004C5F8 (the map spells it protected)
	virtual void *rva0004C62D() = 0;							// slot 47, 0x0004C62D
	virtual void *rva0004C662() = 0;							// slot 48, 0x0004C662
	virtual void *rva0004C694() = 0;							// slot 49, 0x0004C694
	virtual void *rva0004C8DD() = 0;							// slot 50, 0x0004C8DD
	virtual void setFrameRate(Real msecsPerFrame) = 0;			// slot 51, 0x0004C6C6

	DrawableTOCEntry *findTOCEntryByName(AsciiString name);		// 0x00239C5E
	void addIconLayer(int layer, Drawable *drawable);	// 0x00239FCC
	DrawableTOCEntry *findTOCEntryById(unsigned short id);		// 0x002399EB
	void addTOCEntry(AsciiString name, unsigned short id);			// 0x0023AB78
	void restorePriorDisplayMode();								// 0x00238FC0 (role name: copies the saved display-mode fields at +0xD8/+0xDC/+0xE0 back over +0xCC/+0xD0/+0xD4)

protected:
	virtual void loadPostProcess();								// Snapshot slot 1, 0x00239A9D
	virtual const char *GetSnapshotName() const;				// Snapshot slot 2, 0x0004C457
	virtual void xfer(Xfer *xfer);								// Snapshot slot 3, 0x0023B509

public:
	UnsignedInt m_frame;					// +0x10
	Drawable *m_drawableList;				// +0x14
	DrawablePtrHash m_drawableHash;			// +0x18 findDrawableByID 0x00239C21 calls _M_find on it (hash_map ctor 0x0023BC17)
	UnsignedInt m_nextDrawableID;			// +0x2C
	unsigned char m_unk0x30[0x80];			// +0x30
	Int m_unk0xB0;							// +0xB0
	CommandTranslator *m_commandTranslator;	// +0xB4, evaluateContextCommand 0x00238F15 forwards to it
	Int m_unk0xB8;							// +0xB8
	AsciiString m_unk0xBC;					// +0xBC
	Bool m_unk0xC0[2];						// +0xC0
	Int m_unk0xC4;							// +0xC4
	Bool m_unk0xC8[3];						// +0xC8
	Int m_unk0xCC[6];						// +0xCC
	_STL::list<Drawable *> m_unk0xE4;		// +0xE4 (destroyDrawable 0x00239F40 pushes onto it; role not recovered)
	unsigned char m_unk0xE8[0xC];			// +0xE8 a vector
	_STL::list<DrawableTOCEntry> m_drawableTOC;	// +0xF4, walked by findTOCEntryById 0x002399EB and findTOCEntryByName 0x00239C5E
	_STL::list<Drawable *> m_iconLayers[10];	// +0xF8 (addIconLayer 0x00239FCC pushes the drawable onto the list at index layer; the constructor builds the ten lists with an eh vector)
	unsigned char m_unk0x120[0xC];			// +0x120 a vector
	unsigned char m_unk0x12C[0xC];			// +0x12C a vector
	Int m_unk0x138;							// +0x138
	Int m_unk0x13C;							// +0x13C
};
// SIZE 0x140
typedef char GameClientSize[(sizeof(GameClient) == 0x140) ? 1 : -1];
typedef char GameClientListOffset[(offsetof(GameClient, m_drawableList) == 0x14) ? 1 : -1];
typedef char GameClientNextIDOffset[(offsetof(GameClient, m_nextDrawableID) == 0x2C) ? 1 : -1];

extern GameClient *TheGameClient;

// Client frames per second (30 at load; VA 0x00DBA4E8). BfmeLogicFramesPerSecond (VA 0x00DBA4E4) is the logic rate.
extern int BfmeClientFramesPerSecond;
extern Real BfmeClientFramesPerMillisecond;					// 0x00DBA500


// GameClientMessageDispatcher: the client end of the message stream (Zero Hour keeps it
// in this header). Target facts: table 0x00BED670 holds translateGameMessage
// 0x00428D66 and the deleting destructor 0x004296EE, the GameMessageTranslator
// order; the vptr is stored at 0x00238E12. No data members.
class GameClientMessageDispatcher : public GameMessageTranslator
{
public:
	virtual GameMessageDisposition translateGameMessage(const GameMessage *msg);	// 0x00428D66
	virtual ~GameClientMessageDispatcher() {}
};

#endif
