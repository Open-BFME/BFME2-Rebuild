// PhysicsBehavior.h: the physics behaviour module. Partial layout; SIZE 0x68.
// Target facts: ctor 0x003907A6 (factory 0x0024E73C) builds UpdateModule
// (0x00253390), an empty 12-byte element vector at +0x20, the coordinate
// triples at +0x2C and +0x38, and the scalars up to +0x64; it copies the module
// data's +0x58 byte to +0x5D and calls setWakeFrame(m_object, FOREVER).
// rva0039051E 0x0039051E answers (size() > 0) from the vector's +0x20/+0x24
// pointers divided by 12, rva00390601 0x00390601 selects the sleep time from the
// same size plus +0x5C/+0x58, and the destructor 0x003908C2 destroys the vector.
// The vector element type is opaque; only its 12-byte stride is witnessed.
#pragma once
#ifndef BFME2_PHYSICSBEHAVIOR_H
#define BFME2_PHYSICSBEHAVIOR_H

#include "Common/GameType.h"
#include "Common/Coord3D.h"
#include "Common/Vector.h"
#include "GameLogic/Module/UpdateModule.h"
#include "Common/BitFlags11.h"

class ModuleData;
class Object;
class Thing;
class WeaponTemplate;

// Retail uses a 12-byte coordinate vector at +0x20.
typedef _STL::vector<Coord3D> PhysicsElementVector;

class PhysicsBehavior : public UpdateModule
{
public:
	PhysicsBehavior(Thing *thing, const ModuleData *moduleData);
	virtual ~PhysicsBehavior();

	virtual UpdateSleepTime update();					// 0x0039114B
	virtual DisabledMaskType getDisabledTypesToProcess() const;	// slot 1, 0x003908A7: the mask with bit 4 set
	virtual NameKeyType getModuleNameKey() const;		// slot 4 of the class table 0x00819F74: 0x0039085C
	// Own override: the table slot is the fold 0x00497805 of the per-module
	// `{ UpdateModule::loadPostProcess(); }` bodies, not UpdateModule's 0x0058B03E.
	virtual void loadPostProcess();

	Bool rva0039051E() const;							// 0x0039051E
	Bool calcFlightPath(Bool flag, Real strength);		// 0x003901DA
	void rva00390557(const Coord3D *where, Real strength, Real value44, Int value60, Int value64);	// 0x00390557
	UpdateSleepTime rva00390601();						// 0x00390601
	void rva00390629(Bool enable);						// 0x00390629
	void rva003906BF();									// 0x003906BF
	void rva00390CCA(Bool arg);							// 0x00390CCA
	void rva00390E61(Bool skipWake);					// 0x00390E61
	void rva00390FAF();									// 0x00390FAF, the end of a flight
	void rva00390E36(Int source, Int weapon);			// 0x00390E36
	void consume(void *force, Int source, Int weapon);	// 0x003909FA

	PhysicsElementVector m_elements;					// 0x20
	Coord3D m_bfme2C;									// 0x2C
	Coord3D m_bfme38;									// 0x38
	Real m_bfme44;										// 0x44
	Real m_bfme48;										// 0x48
	Int m_bfme4C;										// 0x4C
	Int m_bfme50;										// 0x50
	Int m_bfme54;										// 0x54
	Int m_bfme58;										// 0x58
	Bool m_bfme5C;										// 0x5C
	Bool m_bfme5D;									// 0x5D
	Bool m_bfme5E;										// 0x5E
	Bool m_bfme5F;										// 0x5F
	const Object *m_bfme60;								// 0x60
	const WeaponTemplate *m_bfme64;						// 0x64
protected:
	virtual const char *GetSnapshotName() const;	// 0x003908A1 (retail slot 2 of the class table)
	virtual void xfer(Xfer *xfer);						// 0x00390D50 (protected: the ledger spells it MAE)
};
// SIZE 0x68

#endif
