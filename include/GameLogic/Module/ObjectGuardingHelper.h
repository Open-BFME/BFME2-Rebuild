// ObjectGuardingHelper.h: the helper module that tracks which objects guard an object and which object this
// object guards (Object+0x23C points at it; Object::rva0028BC05 / rva0028BC17 forward to addGuarder /
// removeGuarder). Target facts (f14, 2026-11-18): the constructor 0x004DF418 calls ObjectHelper's
// 0x0028C8DF, stores the tables 0x00861588 at +0x0, 0x007EFF90 at +0xC and 0x0086157C at +0x10, builds the guarder
// list at +0x24 (0x0029FB3B), clears the guarded ID at +0x20 and sets the wake frame to 0x3FFFFFFF; the destructor
// 0x004DF324 resets the tables, destroys the list (0x00268902) and runs ObjectHelper's. The +0x10 table's slot 0
// (update) is retail's __purecall thunk, so the class is abstract here. SIZE 0x28.
#pragma once
#ifndef BFME2_OBJECTGUARDINGHELPER_H
#define BFME2_OBJECTGUARDINGHELPER_H

#include "GameLogic/Module/ObjectHelper.h"
#include "Common/ObjectIDList.h"

class Object;
class Thing;

class ObjectGuardingHelper : public ObjectHelper
{
public:
	ObjectGuardingHelper(Thing *thing, const ModuleData *moduleData);	// 0x004DF418 (emitted)
	virtual ~ObjectGuardingHelper();									// 0x004DF324 (emitted)
	virtual NameKeyType getModuleNameKey() const;						// slot 4, 0x004DF351

	void setGuardStatus();												// 0x004DF2E2: status GUARD_SELECTION on
	void clearGuardStatus();											// 0x004DF2EF: status GUARD_SELECTION off
	void stopGuarding(Object *guarded);									// 0x004DF2FC
	void setGuardedObject(Object *guarded);								// 0x004DF3B0
	void removeGuarder(Object *guarder);								// 0x004DF3F8 (Object::rva0028BC17 forwards here)
	Bool hasGuarders() const;											// 0x004DF49C
	void addGuarder(Object *guarder);									// 0x004DF4B5 (Object::rva0028BC05 forwards here)
	void disbandAllGuarders();											// 0x004DF4D5

protected:
	virtual const char *GetSnapshotName() const;						// slot 2, 0x004DF34B

private:
	ObjectID m_guardedID;												// +0x20 the object this one guards (0 for none)
	ObjectIDList m_guarders;											// +0x24 the objects guarding this one
};
typedef char ObjectGuardingHelper_size[(sizeof(ObjectGuardingHelper) == 0x28) ? 1 : -1];

#endif
