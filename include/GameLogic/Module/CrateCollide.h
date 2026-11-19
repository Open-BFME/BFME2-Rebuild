#pragma once
#include "GameLogic/Module/CollideModule.h"
#include "GameLogic/Module/CrateCollideModuleData.h"
// Native ctor4BC523 calls CollideModule4BB702 and initializes bool14.
// Primary tableC5A618 adds executeCrateBehavior/isValidToExecute at30/34;
// interface tableC5A600 invokes onCollide with the secondary receiver+10.
class Object;
class UnicodeString;
class CrateCollide : public CollideModule {
public:
    CrateCollide(Thing *,const ModuleData *);
    virtual ~CrateCollide();
    virtual void onCollide(Object *,const Coord3D *,const Coord3D *);
protected:
    virtual void loadPostProcess();
    virtual const char *GetSnapshotName() const;
    virtual void xfer(Xfer *);
    virtual bool executeCrateBehavior(Object *)=0;
    virtual bool isValidToExecute(const Object *) const;
    virtual bool wouldLikeToCollideWith(const Object *other) const { return isValidToExecute(other); }	// CollideModuleInterface slot 1: retail 0x004BC51B, an adjusting tail jump into slot 13
    const CrateCollideModuleData *getCrateCollideModuleData() const
        { return static_cast<const CrateCollideModuleData *>(getModuleData()); }
    // 0x004BCF92 (f14): floating text over the collecting module's object (+10 up), in the colour of the given object's player
    // (0xE6 alpha); VeterancyCrateCollide::executeCrateBehavior 0x004BCFF0 calls it for every promoted unit.
    void showFloatingText(const Object *colorSource, const UnicodeString &text);
    bool m_everExecuted;
};
typedef char CrateCollideSize[sizeof(CrateCollide)==0x18 ? 1 : -1];
