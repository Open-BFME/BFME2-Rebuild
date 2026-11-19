#pragma once
#include "GameLogic/Module/UpdateModule.h"
#include "GameLogic/Module/FloodUpdateModuleData.h"
#include "GameLogic/Module/FloodMember.h"
#include "GameLogic/Module/FloodUpdateRecord.h"
class FloodUpdate : public UpdateModule
{
public:
 FloodUpdate(Thing *,const ModuleData *);
 virtual ~FloodUpdate();
 virtual UpdateSleepTime update();
 virtual NameKeyType getModuleNameKey() const;
 void rva0048DC51(const FloodMember *,FloodUpdateRecord *);
 void rva0048E189();
protected:
 virtual const char *GetSnapshotName() const;
 virtual void xfer(Xfer *);
 const FloodUpdateModuleData *getFloodUpdateModuleData() const { return (const FloodUpdateModuleData *)m_moduleData; }
private:
 _STL::list<int> m_records; // 0x20
 bool m_membersCreated; // 0x24
protected:
	virtual void loadPostProcess() { UpdateModule::loadPostProcess(); }	// retail slot 1 0x00497805: a jmp to UpdateModule's (h05)
};
// SIZE 0x28
