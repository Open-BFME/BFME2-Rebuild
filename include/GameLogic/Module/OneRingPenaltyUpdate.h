#pragma once

#ifndef __BFME2_ONERINGPENALTYUPDATE_H_
#define __BFME2_ONERINGPENALTYUPDATE_H_

#include "UpdateModule.h"
#include "DieModuleInterface.h"

enum ObjectID;

class OneRingPenaltyUpdate : public UpdateModule, public DieModuleInterface
{
public:
    OneRingPenaltyUpdate(Thing *thing, const ModuleData *moduleData);
    virtual ~OneRingPenaltyUpdate();

    static Module *friend_newModuleInstance(Thing *thing, const ModuleData *moduleData);
    virtual NameKeyType getModuleNameKey() const;
    virtual UpdateSleepTime update();
    virtual void onDie(const DamageInfo *damageInfo);
    virtual DieModuleInterface *getDie() { return this; }

protected:
    virtual void loadPostProcess();
    virtual const char *GetSnapshotName() const;

    virtual void xfer(Xfer *xfer);

private:
    void rva00499AFD();
    void rva00499C35();
    void rva00499C46();
    void rva00499C6C();
    void rva00499D23();

    ObjectID m_24; // +0x24
    unsigned int m_28; // +0x28
    float m_2C; // +0x2C
    unsigned int m_30; // +0x30
};

#endif
