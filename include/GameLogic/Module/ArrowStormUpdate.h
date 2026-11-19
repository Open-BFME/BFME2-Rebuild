#pragma once
#include "Common/NameKeyGenerator.h"
#include "GameLogic/Module/SpecialAbilityUpdate.h"
#include "GameLogic/Module/ArrowStormUpdateModuleData.h"
class ArrowStormUpdate : public SpecialAbilityUpdate
{
public:
	static Module *friend_newModuleInstance(Thing *thing, const ModuleData *moduleData);
    ArrowStormUpdate(Thing *thing, const ModuleData *moduleData);
    virtual ~ArrowStormUpdate();
    virtual NameKeyType getModuleNameKey() const;
    virtual void startPreparation();
    virtual void triggerAbilityEffect();
    virtual bool continuePreparation();
    virtual bool vslot08(int value);
    void rva00490AC5();
protected:
    virtual const char *GetSnapshotName() const;	// slot 2, 0x004907F1 (the map spells it ?name@ArrowStormUpdate@@QBEPBDXZ)
    // Own override: the table slot is the fold 0x00497805 of the per-module
    // `{ UpdateModule::loadPostProcess(); }` bodies, not UpdateModule's 0x0058B03E.
    virtual void loadPostProcess();
    virtual void xfer(Xfer *xfer);
private:
    bool rva0049083C();
    ObjectIDList m_targets; // 0x88
    ObjectID m_currentTarget; // 0x8C
    int m_shotsAtCurrent; // 0x90
    int m_shotsTotal; // 0x94
    bool m_finished; // 0x98
};
// SIZE 0x9C
