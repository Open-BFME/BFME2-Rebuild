#pragma once
#ifndef __BFME2_WALLUPGRADEUPDATE_H_
#define __BFME2_WALLUPGRADEUPDATE_H_

#include "UpdateModule.h"
#include "GameLogic/Module/DamageModule.h"
#include "WallUpgradeUpdateInterface24.h"

class DamageInfo;
enum BodyDamageType;
enum ObjectID;


class WallUpgradeUpdate : public UpdateModule,
    public DamageModuleInterface, public WallUpgradeUpdateInterface24
{
public:
    WallUpgradeUpdate(Thing *thing, const ModuleData *moduleData);
    virtual ~WallUpgradeUpdate();

    static Module *friend_newModuleInstance(Thing *thing, const ModuleData *moduleData);
    static WallUpgradeUpdate *rva004AB1F5(Object *object);	// Object::getAbleToAttackSpecificObject 0x0028D051 asks the result's interface at +0x24
    virtual NameKeyType getModuleNameKey() const;
    virtual DamageModuleInterface *getDamage() { return this; }
    virtual UpdateSleepTime update();
    virtual void onDamage(DamageInfo *damageInfo) { }
    virtual void onHealing(DamageInfo *damageInfo) { }
    virtual void onBodyDamageStateChange(const DamageInfo *damageInfo,
        BodyDamageType oldState, BodyDamageType newState);
    virtual bool rva004AB3AE();

    bool rva004AB4AB() const;
    void scanForBuildingAndPossess();
    void rva004AB57E();

protected:
    virtual void loadPostProcess();
    virtual const char *GetSnapshotName() const;

    virtual void xfer(Xfer *xfer);

private:
    ObjectID m_28;
    int m_2C;
    bool m_30;
    bool m_31;
};

#endif
