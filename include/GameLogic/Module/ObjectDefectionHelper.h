// ObjectDefectionHelper.h: the helper that runs an undetected defector's detection
// window and its flash and tick effects. SIZE 0x30.
// Target facts (retail game.dat 1.06):
// - Layout: ObjectHelper (0x20, UpdateModule with the update interface at +0x10),
//   then the four members below. The constructor is at 0x0028C955.
// - update 0x004DF547 is the UpdateModuleInterface slot 0 entry (this at +0x10).
// - startDefectionTimer 0x004DF725 (public, non-virtual) takes (uint duration, bool fx).
// - xfer 0x004DF76E (protected virtual, slot 3) calls the ObjectHelper xfer first,
//   then transfers the four members in this order.
#pragma once
#ifndef BFME2_OBJECTDEFECTIONHELPER_H
#define BFME2_OBJECTDEFECTIONHELPER_H

#include "GameLogic/Module/ObjectHelper.h"

class Xfer;

class ObjectDefectionHelper : public ObjectHelper
{
public:
	ObjectDefectionHelper(Thing *thing, const ModuleData *moduleData);	// 0x0028C955
	virtual NameKeyType getModuleNameKey() const;				// slot 4, 0x0028C992
	virtual UpdateSleepTime update();							// 0x004DF547
	void startDefectionTimer(UnsignedInt duration, Bool doFX);	// 0x004DF725

protected:
	virtual void loadPostProcess();								// slot 1 (0x00483CEF: tail-jumps to ObjectHelper's)
	virtual const char *GetSnapshotName() const;				// slot 2, 0x0028C9D7
	virtual void xfer(Xfer *xfer);								// 0x004DF76E

private:
	UnsignedInt m_windowStartFrame;								// 0x20
	UnsignedInt m_windowEndFrame;								// 0x24
	float m_flashPhase;											// 0x28
	Bool m_doFX;												// 0x2C
};
// SIZE 0x30
typedef char ObjectDefectionHelper_size[(sizeof(ObjectDefectionHelper) == 0x30) ? 1 : -1];

#endif
