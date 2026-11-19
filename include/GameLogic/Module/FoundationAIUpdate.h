// FoundationAIUpdate.h: the update module every builder/foundation AI derives.
// SIZE 0x30. Target facts: ctor 0x004551B3 calls UpdateModule's, then stores the
// interface table 0x00BFBC9C at +0x20 and its own 0x00C1A690 at +0x20, so the
// class takes an interface at +0x20 and its members start at +0x24 (the ctor
// zeroes +0x24/+0x28 and the byte +0x2C). CastleBehavior 0x003983D4 derives
// from it and re-stores all five tables.
#pragma once
#ifndef BFME2_FOUNDATIONAIUPDATE_H
#define BFME2_FOUNDATIONAIUPDATE_H

#include <stddef.h>
#include "Common/GameType.h"
#include "Common/NameKeyGenerator.h"
#include "GameLogic/Module/UpdateModule.h"

class Thing;
class ModuleData;
class Xfer;
class Player;
class Object;
class ThingTemplate;
class Coord3D;

// The interface at +0x20. Target facts: the constructor 0x004551B3 stores the folded all-pure table
// of VA 0x00BFBC9C (eight slots) and then this class's own table 0x00C1A690, whose eight entries are:
// 0 and 1 shared bodies that return the interface pointer after a null-checked adjustment (0x00452D78;
// slot 1 is a thunk to 0x004543A9), 2 a thunk chain to 0x002FECBC, 3 the shared `[this+8] != 0` getter
// (0x00354EB0: the +0x28 ObjectID is set), 4 rva00456134, 5 rva00455374, 6 rva00455B33 and 7 construct
// (ledger names). Slots 0..3 have no ledger name of their own; their names here are role labels.
class FoundationAIUpdateInterface
{
public:
	virtual FoundationAIUpdateInterface *vslot00() = 0;		// slot 0 (PROVISIONAL name)
	virtual FoundationAIUpdateInterface *vslot01() = 0;		// slot 1 (PROVISIONAL name)
	virtual void vslot02(ObjectID objectID) = 0;
	virtual Bool hasBuilder() const = 0;						// slot 3 (PROVISIONAL name): the ObjectID at module +0x28 is set
	virtual Object *rva00456134(void *context, const ThingTemplate *tmpl, const Coord3D *pos, Real angle, Player *owner, Bool flag) = 0;	// slot 4, 0x00456134
	virtual void rva00455374() = 0;							// slot 5, 0x00455374
	virtual Object *rva00455B33() = 0;						// slot 6, 0x00455B33
	virtual Object *construct(const ThingTemplate *tmpl, const Coord3D *pos, Real angle, Player *owner, Int flags, Bool flag) = 0;	// slot 7, 0x0045569D
};

struct OpaqueRefElement4;

class FoundationAIUpdate : public UpdateModule, public FoundationAIUpdateInterface
{
public:
	FoundationAIUpdate(Thing *thing, const ModuleData *moduleData);

	// Behavior slot 38 returns the foundation interface at +0x20 for Foundation and Castle.
	virtual FoundationAIUpdateInterface *slot38() { return this; }

	virtual NameKeyType getModuleNameKey() const;				// 0x00455167
	virtual void onCapture(Player *oldOwner, Player *newOwner);	// override: primary slot 9, 0x00455B67 (the ledger calls it rva00455B67)
	virtual void startBuildingSound(const OpaqueRefElement4 &sound, Int objectID);	// primary slot 13, 0x00455076 (the map files it as CastleBehavior::rva00455076) (starts the sound: m_24 = TheAudio->addAudioEvent)
	virtual void finishBuildingSound();							// primary slot 14, 0x004550DE (ends it: TheAudio slot 27 on m_24, then m_24 = 1)
	virtual Real rva0045523A() const;							// primary slot 15, 0x0045523A (folded getter of a Real at module data +8)
	virtual void rva0045537F();									// primary slot 16, 0x0045537F

	// FoundationAIUpdateInterface at +0x20 (retail table 0x00C1A690)
	virtual FoundationAIUpdateInterface *vslot00() { return this; }	// slot 0: the shared 0x00452D78 (lea eax,[ecx-20h]; neg; sbb; and eax,ecx: this back to the interface, null-checked)
	virtual FoundationAIUpdateInterface *vslot01();
	virtual void vslot02(ObjectID objectID);
	virtual Bool hasBuilder() const { return m_28 != 0; }	// slot 3: the shared `[this+8] != 0` getter 0x00354EB0 (the interface this at +0x20, so +0x28)
	virtual Object *rva00456134(void *context, const ThingTemplate *tmpl, const Coord3D *pos, Real angle, Player *owner, Bool flag);	// 0x00456134
	virtual void rva00455374();									// 0x00455374
	virtual Object *rva00455B33();								// 0x00455B33
	virtual Object *construct(const ThingTemplate *tmpl, const Coord3D *pos, Real angle, Player *owner, Int flags, Bool flag);	// 0x0045569D

	virtual void slot46(Int);									// override: BehaviorModuleInterface slot 46, 0x00455B42 (the ledger calls it rva00455B42)

	Bool isRemovableForConstruction(Object *obj);				// 0x00455241

protected:
	virtual const char *GetSnapshotName() const;				// 0x00455070
	virtual void xfer(Xfer *xfer);								// 0x004550F8

public:
	Int m_24;									// 0x24
	ObjectID m_28;								// 0x28 (XferObjectID takes an ObjectID *)
	Bool m_2C;									// 0x2C
public:
	virtual UpdateSleepTime update();	// 0x00455477, UpdateModuleInterface slot 0; retail's table has this class's own (h05, gen3 vtable-content list)

private:
	void rva00455BDD(const ThingTemplate *tmpl, const Coord3D *position, Real angle);	// 0x00455BDD: clears the objects in the way
	Bool rva00455C98(const ThingTemplate *tmpl, const Coord3D *position, Real angle, Player *owner);	// 0x00455C98
	void rva0045527A(ObjectID id);								// 0x0045527A: sets m_28 and shows or hides the owner object
protected:
	virtual void loadPostProcess() { UpdateModule::loadPostProcess(); }	// retail slot 0x00497805: a jmp to UpdateModule's, ICF-folded across classes (h05)
};
// SIZE 0x30
typedef char FoundationAIUpdate_size[(sizeof(FoundationAIUpdate) == 0x30) ? 1 : -1];

#endif
