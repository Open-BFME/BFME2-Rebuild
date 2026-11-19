// ThreatFinderUpdate.h: SAGE threat-finder update module and its INI data.
// Target facts (old repo ThreatFinderUpdate.cpp / ThreatFinderUpdateDtor.cpp /
// ThreatFinderUpdateXfer.cpp): ??0ThreatFinderUpdate 0x003ECCA2 is 46 bytes and
// stores a null heap pointer after the UpdateModule base (0x20), so the class is
// 0x24 with the heap at +0x20. The dtor 0x003ECF64 deletes that heap object and
// calls ~UpdateModule 0x0024A797. getModuleNameKey 0x003ECCD6 caches the pool key
// for "ThreatFinderUpdate", and the INI table registers DefaultRadius at +8.
#pragma once
#ifndef BFME2_THREATFINDERUPDATE_H
#define BFME2_THREATFINDERUPDATE_H

#include "Common/GameType.h"
#include "GameLogic/Module/ModuleData.h"
#include "GameLogic/Module/UpdateModule.h"

class Thing;
class Xfer;
class Dict;
class MultiIniFieldParse;
struct FieldParse;
class Rva003ECDB7Object;

class ThreatFinderUpdateModuleData : public ModuleData
{
public:
	static void buildFieldParse(MultiIniFieldParse &parse);

	Real m_defaultRadius;				// 0x08
};
// SIZE 0x0C

// Retail VA 0x00C35FE0: DefaultRadius at +8, then the zero sentinel.
extern const FieldParse BfmeThreatFinderUpdateFields[];

class ThreatFinderUpdate : public UpdateModule
{
public:
	ThreatFinderUpdate(Thing *thing, const ModuleData *moduleData);
	virtual ~ThreatFinderUpdate();

	virtual NameKeyType getModuleNameKey() const;
	virtual void mapLoadPostProcess(const Dict *properties);

protected:
	virtual void xfer(Xfer *xfer);
	virtual const char *GetSnapshotName() const;

	Rva003ECDB7Object *m_heap;			// 0x20
protected:
	virtual void loadPostProcess() { UpdateModule::loadPostProcess(); }	// retail slot 1 0x00497805: a jmp to UpdateModule's (h05)
};
// SIZE 0x24

#endif
