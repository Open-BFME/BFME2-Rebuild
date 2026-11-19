#pragma once
#include "GameLogic/Module/UpdateModule.h"
#include "GameLogic/Module/DieModuleInterface.h"
#include "GameLogic/Module/RubbleRiseUpdateModuleData.h"
#include "Common/Coord3D.h"
class RubbleRiseUpdate : public UpdateModule,public DieModuleInterface
{
public:
 RubbleRiseUpdate(Thing *,const ModuleData *);
 virtual ~RubbleRiseUpdate();
 virtual const char *GetSnapshotName() const;
 virtual NameKeyType getModuleNameKey() const;
 virtual UpdateSleepTime update();
 virtual void onDie(const DamageInfo *);
 virtual DieModuleInterface *getDie() { return this; }
 void rva004A4D70();
protected:
 // Own override: the table slot is the fold 0x00497805 of the per-module
 // `{ UpdateModule::loadPostProcess(); }` bodies, not UpdateModule's 0x0058B03E.
 virtual void loadPostProcess();
 virtual void xfer(Xfer *);
 void doPhaseStuff(RubbleRisePhaseType,const Coord3D *);
 void doRubbleDoneStuff();
 const RubbleRiseUpdateModuleData *getRubbleRiseUpdateModuleData() const { return (const RubbleRiseUpdateModuleData *)getModuleData(); }
 unsigned m_unused24; // 0x24
 unsigned m_nextBurstFrame; // 0x28
 RubbleRiseStateType m_riseState; // 0x2C
 float m_riseVelocity; // 0x30
 float m_currentHeight; // 0x34
 float m_rubbleHeight; // 0x38
 Coord3D m_risePosition; // 0x3C
};
// SIZE 0x48
