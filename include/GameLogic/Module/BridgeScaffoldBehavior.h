#pragma once
#include "GameLogic/Module/UpdateModule.h"
#include "Common/Coord3D.h"

enum ScaffoldTargetMotion { STM_STILL, STM_RISE, STM_BUILD_ACROSS, STM_TEAR_DOWN_ACROSS, STM_SINK };
// Six slots confirmed by the scaffold setters, motion query and bridge calls.
// Names and order agree with the ZH BridgeScaffoldBehavior interface.
class BridgeScaffoldBehaviorInterface {
public:
    virtual void setPositions(const Coord3D *, const Coord3D *, const Coord3D *) = 0;
    virtual void setMotion(ScaffoldTargetMotion) = 0;
    virtual ScaffoldTargetMotion getCurrentMotion() = 0;
    virtual void reverseMotion() = 0;
    virtual void setLateralSpeed(float) = 0;
    virtual void setVerticalSpeed(float) = 0;
};
// Native ctor45831C and xfer establish UpdateModule0..1F, interface20,
// motion24, positions28/34/40/54, and speeds4C/50; complete size60.
// Method declarations preserve the target's external call boundaries.
class BridgeScaffoldBehavior : public UpdateModule, public BridgeScaffoldBehaviorInterface {
public:
    BridgeScaffoldBehavior(Thing *, const ModuleData *);
    virtual ~BridgeScaffoldBehavior();
    virtual UpdateSleepTime update();
    virtual NameKeyType getModuleNameKey() const;     // 0x004583BD (h05: written in BridgeScaffoldBehavior.cpp)
    virtual BridgeScaffoldBehaviorInterface *getBridgeScaffoldBehaviorInterface() { return this; }
    static BridgeScaffoldBehaviorInterface *getBridgeScaffoldBehaviorInterfaceFromObject(Object *);
    virtual void setPositions(const Coord3D *, const Coord3D *, const Coord3D *);
    virtual void setMotion(ScaffoldTargetMotion);
    virtual ScaffoldTargetMotion getCurrentMotion();
    virtual void reverseMotion();
    virtual void setLateralSpeed(float);
    virtual void setVerticalSpeed(float);
protected:
    virtual const char *GetSnapshotName() const;      // 0x004583A9
    virtual void xfer(Xfer *);
    virtual void loadPostProcess();
    ScaffoldTargetMotion m_targetMotion;
    Coord3D m_createPos, m_riseToPos, m_buildPos;
    float m_lateralSpeed, m_verticalSpeed;
    Coord3D m_targetPos;
};
typedef char BridgeScaffoldBehaviorSize[sizeof(BridgeScaffoldBehavior)==0x60 ? 1 : -1];
