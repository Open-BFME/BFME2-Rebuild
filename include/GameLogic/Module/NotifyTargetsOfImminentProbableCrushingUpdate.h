// The two update modules share a native scan implementation and INI data.
// The factory at 0x253D37 allocates 0x18 and passes the data to the same
// parse routine at 0x4CEE09.
#pragma once

#include <stddef.h>
#include "GameLogic/Module/ModuleData.h"
#include "GameLogic/Module/UpdateModule.h"

class Xfer;

struct CrushingScanParameters
{
	UnsignedInt m_updateInterval;
	UnsignedInt m_scanAheadTime;
	Real m_scanHeight;
	Real m_scanWidth;
};

class NotifyTargetsOfImminentProbableCrushingUpdateModuleData : public ModuleData
{
public:
	NotifyTargetsOfImminentProbableCrushingUpdateModuleData();
	static void buildFieldParse(MultiIniFieldParse &parse);

	CrushingScanParameters m_parameters;
};

// The native worker receives this subobject at +0x20 and stores the
// next delay through its output argument. Its snapshot is version-only.
class CrushingScan
{
public:
	void update(Object *object, const CrushingScanParameters *parameters, UpdateSleepTime *delay);
	void xfer(Xfer *xfer);
};

class NotifyTargetsOfImminentProbableCrushingUpdate : public UpdateModule
{
public:
	NotifyTargetsOfImminentProbableCrushingUpdate(Thing *thing, const ModuleData *data);
	static Module *friend_newModuleInstance(Thing *thing, const ModuleData *moduleData);
	virtual ~NotifyTargetsOfImminentProbableCrushingUpdate();
	virtual NameKeyType getModuleNameKey() const;
	virtual UpdateSleepTime update();

protected:
	virtual void loadPostProcess() { UpdateModule::loadPostProcess(); }
	virtual const char *GetSnapshotName() const;
	virtual void xfer(Xfer *xfer);

	CrushingScan m_scan;
};

class HordeNotifyTargetsOfImminentProbableCrushingUpdate : public UpdateModule
{
public:
	HordeNotifyTargetsOfImminentProbableCrushingUpdate(Thing *thing, const ModuleData *data);
	static Module *friend_newModuleInstance(Thing *thing, const ModuleData *moduleData);
	virtual ~HordeNotifyTargetsOfImminentProbableCrushingUpdate();
	virtual NameKeyType getModuleNameKey() const;
	virtual UpdateSleepTime update();

protected:
	virtual void loadPostProcess() { UpdateModule::loadPostProcess(); }
	virtual const char *GetSnapshotName() const;
	virtual void xfer(Xfer *xfer);

	CrushingScan m_scan;
};

typedef char CrushingScanDataSizeCheck[sizeof(NotifyTargetsOfImminentProbableCrushingUpdateModuleData) == 0x18 ? 1 : -1];
typedef char CrushingScanDataOffset[offsetof(NotifyTargetsOfImminentProbableCrushingUpdateModuleData, m_parameters) == 0x8 ? 1 : -1];
typedef char CrushingScanUpdateSizeCheck[sizeof(NotifyTargetsOfImminentProbableCrushingUpdate) == 0x24 ? 1 : -1];
typedef char HordeCrushingScanUpdateSizeCheck[sizeof(HordeNotifyTargetsOfImminentProbableCrushingUpdate) == 0x24 ? 1 : -1];
