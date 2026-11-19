// CreateObjectDieIfEldestKindof.h: CreateObjectDie that fires only when the dead
// object is the eldest of its kind. Target facts: the pool key getter is
// 0x00485A2F (slot 4 of the Module table), onDie 0x00485D11 is slot 0 of the
// die interface table 0x00C4A810 and gates on the eldest-of-kind helper before
// running CreateObjectDie::onDie 0x00485C86.
#pragma once
#ifndef BFME2_CREATEOBJECTDIEIFELDESTKINDOF_H
#define BFME2_CREATEOBJECTDIEIFELDESTKINDOF_H

#include "GameLogic/Module/CreateObjectDie.h"
class Xfer;

// The eldest-of-kind check the die module runs first (rva00485C19, retail 0x00485C19); retail declares it a struct. Its one
// word at IfEldest +0x14 is not named yet; the filter argument is the map's placeholder type.
struct Rva00485C19Data;
class ObjectFilter;

// The callback StatusBitsEldestFrame::rva00485C19 runs over every team's members (TeamPrototype::rva0039ED9C 0x0039ED9C) to find the
// eldest object that carries the module: CreateObjectDieIfEldestKindof's table slot 40 of the module interface at +0xC answers
// with the module's StatusBitsEldestFrame, whose cached frame the search compares.
struct findEldest
{
	struct Search
	{
		Object *m_eldest;						// +0x0 the eldest object found so far
		UnsignedInt m_eldestFrame;				// +0x4 its cached frame; -1 before the first hit
		const ObjectFilter *m_filter;			// +0x8 what a candidate has to pass
		NameKeyType m_key;						// +0xC the module's name key
	};
	static Int func(Object *object, void *userData);	// 0x004859A2
};
struct StatusBitsEldestFrame
{
public:
	Bool rva00485C19(Object *object, const Rva00485C19Data *filter, const NameKeyType *key);	// 0x00485C19
	void xfer(Xfer *xfer);	// 0x00485BB8: Version1, then the cached frame
	StatusBitsEldestFrame *cacheGameLogicFrame();	// 0x00485975: stores the logic frame in m_unk0x0 and returns this
	UnsignedInt m_unk0x0;	// +0x0 of the frame; 4 bytes (IfEldest SIZE 0x18 less the CreateObjectDie base 0x14)
};

class CreateObjectDieIfEldestKindof : public CreateObjectDie
{
public:
	CreateObjectDieIfEldestKindof(Thing *thing, const ModuleData *moduleData);

	virtual NameKeyType getModuleNameKey() const;
	virtual void onDie(const DamageInfo *damageInfo);

protected:
	virtual ~CreateObjectDieIfEldestKindof();

public:
	StatusBitsEldestFrame m_eldest;	// +0x14
protected:
	virtual const char *GetSnapshotName() const;	// slot 2 of the class table, 0x0048599C (h05 r9)
};
// SIZE 0x18

#endif
