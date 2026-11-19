#pragma once
#include "GameLogic/Module/UpdateModule.h"
#include "Common/BitFlags11.h"
class DetachableRiderUpdate : public UpdateModule
{
public:
 DetachableRiderUpdate(Thing *,const ModuleData *);
 virtual ~DetachableRiderUpdate();
 virtual NameKeyType getModuleNameKey() const;
 virtual UpdateSleepTime update();
 virtual DisabledMaskType getDisabledTypesToProcess() const;
 void rva004AE8FA();
 void killRider();
protected:
 virtual const char *GetSnapshotName() const;
 virtual void xfer(Xfer *);
private:
 bool m_active; // 0x20
 unsigned char m_riderIndex; // 0x21
protected:
	virtual void loadPostProcess() { UpdateModule::loadPostProcess(); }	// retail slot 1 0x00497805: a jmp to UpdateModule's (h05)
};
// SIZE 0x24
