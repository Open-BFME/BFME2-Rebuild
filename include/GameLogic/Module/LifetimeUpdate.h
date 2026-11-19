#pragma once
#include "GameLogic/Module/UpdateModule.h"
// Primary, behavior and update subobjects belong to UpdateModule. Native
// calcSleepDelay and xfer establish the frame fields+20/+24 and flag+28.
class LifetimeUpdate : public UpdateModule
{
public:
    LifetimeUpdate(Thing *thing, const ModuleData *moduleData);
    virtual NameKeyType getModuleNameKey() const;
    virtual UpdateSleepTime update();
    virtual void rva0044DF8D(UpdateSleepTime wakeDelay);
    void setLifetimeRange(UnsignedInt minFrames, UnsignedInt maxFrames);
    void rva003A4AD2();
protected:
    virtual const char *GetSnapshotName() const;
    virtual void xfer(Xfer *xfer);
private:
    UnsignedInt calcSleepDelay(UnsignedInt minFrames, UnsignedInt maxFrames);
    UnsignedInt m_dieFrame;
    UnsignedInt m_birthFrame;
    Bool m_disabled;
protected:
	virtual void loadPostProcess() { UpdateModule::loadPostProcess(); }	// retail slot 0x00497805: a jmp to UpdateModule's, ICF-folded across classes (h05)
};
typedef char check_LifetimeUpdate_size[sizeof(LifetimeUpdate)==0x2C?1:-1];
