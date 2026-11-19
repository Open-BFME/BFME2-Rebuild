// DockUpdate.h: SAGE docking behaviour and its module data.
// Target facts (old repo SupplyWarehouseDockUpdateNative.h and DockUpdateCtor.cpp):
// ??0DockUpdate 0x0058A290 installs four tables (+0 primary, +0x0C behaviour,
// +0x10 update, +0x20 dock interface) and the factory allocates 0x8C bytes for
// SupplyWarehouseDockUpdate (0x88 for DockUpdate). Layout after UpdateModule
// (0x20): DockUpdateInterface vptr +0x20, enter/dock/exit +0x24/+0x30/+0x3C,
// number/bones/loaded +0x48/+0x4C/+0x50, approach positions +0x54, approach
// owners +0x60, approach reached bits +0x6C, active docker +0x80, inside +0x84,
// crippled +0x85, open +0x86. DockUpdateInterface's callbacks receive `this` at
// the +0x20 subobject, so their bytes read interface-relative offsets.
#pragma once
#ifndef BFME2_DOCKUPDATE_H
#define BFME2_DOCKUPDATE_H

#include <vector>
#include "Common/GameType.h"
#include "Common/Coord3D.h"
#include "GameLogic/Module/UpdateModule.h"
#include "GameLogic/Module/ModuleData.h"

class Object;
class Thing;
class Xfer;
struct FieldParse;
class MultiIniFieldParse;

enum
{
	DEFAULT_APPROACH_VECTOR_SIZE = 10,
	DYNAMIC_APPROACH_VECTOR_FLAG = -1
};

class DockUpdateModuleData : public ModuleData
{
public:
	DockUpdateModuleData();
	static void buildFieldParse(MultiIniFieldParse &parse);

	Int m_numberApproachPositionsData;	// 0x08
	Bool m_isAllowPassthrough;			// 0x0C
};
// SIZE 0x10

// Retail VA 0x00C702CC: NumberApproachPositions at +8 and AllowsPassthrough
// at +0xC, then the zero sentinel.
extern const FieldParse BfmeDockUpdateFields[];

// Seven-byte disp8 byte chase: the holder sits at a negative displacement from
// `this`, so the getter is a member of the subobject at DockUpdate+0x1C.
struct Rva005896D2Inner
{
	char m_lead[0x0C];
	unsigned char m_value;				// 0x0C
};
struct Rva005896D2Sub
{
	Rva005896D2Inner *m_holder;			// 0x00
};
class Rva005896D2ByteChaseField
{
public:
	unsigned char get() const;
};

class DockUpdateInterface
{
public:
	virtual Bool isClearToApproach(const Object *docker) const = 0;			// 0
	virtual Bool reserveApproachPosition(Object *docker, Coord3D *position, Int *index) = 0;
	virtual Bool advanceApproachPosition(Object *docker, Coord3D *position, Int *index) = 0;
	virtual Bool isClearToEnter(const Object *docker) const = 0;
	virtual Bool isClearToAdvance(const Object *docker, Int index) const = 0;
	virtual void getEnterPosition(Object *docker, Coord3D *position) = 0;
	virtual void getDockPosition(Object *docker, Coord3D *position) = 0;
	virtual void getExitPosition(Object *docker, Coord3D *position) = 0;
	virtual void onApproachReached(Object *docker) = 0;
	virtual void onEnterReached(Object *docker) = 0;
	virtual void onDockReached(Object *docker) = 0;
	virtual void onExitReached(Object *docker) = 0;
	virtual Bool action(Object *docker, Object *drone) = 0;	// 12: the map names RepairDockUpdate/SupplyCenterDockUpdate/MonsterDockUpdate overrides `action(Object *, Object *)`; AIDockProcessDockState::update 0x00544B96 passes the drone
	virtual void cancelDock(Object *docker) = 0;
	virtual Bool isDockOpen() = 0;
	virtual void setDockOpen(Bool open) = 0;
	virtual Bool dockSlot16(Object *docker) = 0;
	virtual void setDockCrippled(Bool setting) = 0;
	virtual Bool isAllowPassthroughType() = 0;
	virtual Bool isRallyPointAfterDockType() = 0;
};

// The approach-bone scratch array's lifetime adapter: retail's unwind graph
// witnesses the 10-element array's construction and destruction, whose bodies
// are byte twins of the native Coord3D constructor (8b c1 c3) and destructor (c3).
struct Rva005897FBCoordLifetime : Coord3D
{
	Rva005897FBCoordLifetime();
	~Rva005897FBCoordLifetime();
};

class DockUpdate : public UpdateModule, public DockUpdateInterface
{
public:
	DockUpdate(Thing *thing, const ModuleData *moduleData);
	virtual ~DockUpdate();

	virtual UpdateSleepTime update();

	virtual Bool isClearToApproach(const Object *docker) const;
	virtual Bool reserveApproachPosition(Object *docker, Coord3D *position, Int *index);
	virtual Bool advanceApproachPosition(Object *docker, Coord3D *position, Int *index);
	virtual Bool isClearToEnter(const Object *docker) const;
	virtual Bool isClearToAdvance(const Object *docker, Int index) const;
	virtual void getEnterPosition(Object *docker, Coord3D *position);
	virtual void getDockPosition(Object *docker, Coord3D *position);
	virtual void getExitPosition(Object *docker, Coord3D *position);
	virtual void onApproachReached(Object *docker);
	virtual void onEnterReached(Object *docker);
	virtual void onDockReached(Object *docker);
	virtual void onExitReached(Object *docker);
	virtual Bool action(Object *docker, Object *drone) = 0;
	virtual void cancelDock(Object *docker);
	virtual Bool isDockOpen() { return m_dockOpen; }
	virtual void setDockOpen(Bool open) { m_dockOpen = open; }
	virtual Bool dockSlot16(Object *docker) { return false; }
	virtual void setDockCrippled(Bool setting);
	virtual Bool isAllowPassthroughType()
	{
		return ((const DockUpdateModuleData *)m_moduleData)->m_isAllowPassthrough;
	}
	virtual Bool isRallyPointAfterDockType() { return false; }

	void rva00589AFF(Int index);

protected:
	Coord3D computeApproachPosition(Int index, Object *docker);	// protected in retail (IAE), 0x00589B7D
	virtual void loadPostProcess();
	virtual void xfer(Xfer *xfer);
	void loadDockPositions();

	Coord3D m_enterPosition;					// 0x24
	Coord3D m_dockPosition;						// 0x30
	Coord3D m_exitPosition;						// 0x3C
	Int m_numberApproachPositions;				// 0x48
	Int m_numberApproachPositionBones;			// 0x4C
	Bool m_positionsLoaded;						// 0x50
	_STL::vector<Coord3D> m_approachPositions;			// 0x54
	_STL::vector<ObjectID> m_approachPositionOwners;	// 0x60
	_STL::vector<bool> m_approachPositionReached;		// 0x6C
	ObjectID m_activeDocker;					// 0x80
	Bool m_dockerInside;						// 0x84
	Bool m_dockCrippled;						// 0x85
	Bool m_dockOpen;							// 0x86
protected:
	virtual const char *GetSnapshotName() const;	// 0x0058A171 (retail slot 2 of the class table)
};
// SIZE 0x88

// 0x0058A193, 14 bytes: vector<ObjectID>'s resize(count, INVALID_ID) reached
// through a derived spelling.
class Rva0058A193 : public _STL::vector<ObjectID>
{
public:
	void rva0058A193(UnsignedInt count);
};

#endif
