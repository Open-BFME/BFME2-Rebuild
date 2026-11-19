// ObjectModule.h: SAGE base of every object module. SIZE 0xC.
// Target facts (old repo Code/GameEngine/Include/Common/ObjectModule.h):
// ??0ObjectModule@@QAE@PAVThing@@PBVModuleData@@@Z (0x00306AA7) installs
// 0x00C07E1C, stores the ModuleData at +4 and the object at +8. The table has
// Module's 9 slots plus slot 9, an empty virtual with two stack arguments
// (0x0050B238 `ret 8`; Zero Hour's onCapture). The class-name literal at
// 0x00C07E44 bounds it at 10 slots.
// This is the +0x08-to-+0x0C step of the chain Module.h describes: a class that
// adds a behavior interface puts its vptr at +0x0C and one that adds an update
// interface as well puts that at +0x10 (??0UpdateModule 0x00253390).
// Its destructor is out of line (Module.cpp; see Module.h): ~UpdateModule
// (0x0024A797) and every behavior-module destructor end in a call or jump to it.
#pragma once
#ifndef BFME2_OBJECTMODULE_H
#define BFME2_OBJECTMODULE_H

#include "GameLogic/Module/Module.h"

class Object;
class Player;
class Thing;

class ObjectModule : public Module
{
public:
	ObjectModule(Thing *thing, const ModuleData *moduleData);
	virtual ~ObjectModule();

	virtual void onCapture(Player *oldOwner, Player *newOwner) {}			// slot 9

protected:
	virtual void xfer(Xfer *xfer) { xfer->Version1(); Module::xfer(xfer); }	// slot 3, the fold 0x00560AE1 (map: ?xfer@ObjectModule@@QAEXPAVXfer@@@Z)
	virtual const char *GetSnapshotName() const;							// slot 2 of 0x00807E1C, 0x00306B13 (needs the map rename from ?name@ObjectModule@@QBEPBDXZ)
	Object *getObject() { return m_object; }
	const Object *getObject() const { return m_object; }

	Object *m_object;														// 0x08
};
// SIZE 0xC

#endif
