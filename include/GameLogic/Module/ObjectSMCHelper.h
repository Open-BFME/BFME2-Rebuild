// ObjectSMCHelper.h: the helper module that times an object's special model conditions (Object::m_smcHelper,
// Object::setSpecialModelConditionState 0x0028AEB2 forwards to setModelConditionState). Target facts (f14,
// 2026-11-18): the destructor 0x004DE767 resets the tables 0x007FC088 at +0x0, 0x007EFF90 at +0xC and
// 0x007FC07C at +0x10 and destroys the std::list at +0x20 (clear 0x0023DAA5, base 0x004EC395); the list's
// nodes hold two dwords at +8/+0xC (the condition and the frame it ends: update 0x004DE7C2 ends every entry whose
// frame has passed, framesUntilNext 0x004DE700 takes the smallest frame), xfer 0x004DE906 stores the count, then each
// (condition, frame) pair. SIZE 0x24.
#pragma once
#ifndef BFME2_OBJECTSMCHELPER_H
#define BFME2_OBJECTSMCHELPER_H

#include <list>
#include "GameLogic/Module/ObjectHelper.h"

class Object;
class Thing;
class Xfer;
enum ModelConditionFlagType;

class ObjectSMCHelper : public ObjectHelper
{
public:
	ObjectSMCHelper(Thing *thing, const ModuleData *moduleData);
	virtual ~ObjectSMCHelper();											// 0x004DE767
	virtual NameKeyType getModuleNameKey() const;						// slot 4, 0x00292849
	virtual UpdateSleepTime update();									// 0x004DE7C2
	void setModelConditionState(ModelConditionFlagType flag, UnsignedInt frames);	// 0x004DE85F

protected:
	virtual const char *GetSnapshotName() const;						// slot 2, 0x0029288E
	virtual void xfer(Xfer *xfer);										// 0x004DE906

private:
	struct Entry
	{
		Int m_flag;														// +0x8 of the list node: the model condition
		UnsignedInt m_frame;											// +0xC the frame it ends
	};
	Int framesUntilNext();												// 0x004DE700
	_STL::list<Entry> m_entries;										// +0x20
};
typedef char ObjectSMCHelper_size[(sizeof(ObjectSMCHelper) == 0x24) ? 1 : -1];

#endif
