
#pragma once
#ifndef __BFME2_SPECIALABILITYUPDATE_H_
#define __BFME2_SPECIALABILITYUPDATE_H_

#include "UpdateModule.h"
#include "SpecialPowerUpdateInterface.h"
#include "../../Common/BitFlags11.h"
#include "Common/Coord3D.h"
#include "Common/ObjectIDList.h"

class SpecialPowerTemplate;
class SpecialPowerModuleInterface;
class Waypoint;
class CommandButton;
enum SpecialPowerType;
enum ObjectID;

class SpecialAbilityUpdate : public UpdateModule, public SpecialPowerUpdateInterface
{
public:
    SpecialAbilityUpdate(Thing *, const ModuleData *);
    virtual ~SpecialAbilityUpdate();
    static Module *friend_newModuleInstance(Thing *, const ModuleData *);

    virtual NameKeyType getModuleNameKey() const;
    virtual UpdateSleepTime update();
    virtual DisabledMaskType getDisabledTypesToProcess() const
    {
        return DisabledMaskType(DisabledMaskType::kInit, 3);
    }
    virtual SpecialPowerUpdateInterface *getSpecialPowerUpdateInterface() { return this; }
    virtual void initiateIntentToDoSpecialPower(const SpecialPowerTemplate *, const Object *,
        const Coord3D *, unsigned int, const Waypoint *);
    virtual bool isSpecialAbility() const { return true; }
    virtual bool isActive() const { return m_active; }
    virtual CommandOption getCommandOption() const { return (CommandOption)0; }
    virtual bool doesSpecialPowerHaveOverridableDestinationActive() const { return false; }
    virtual void setSpecialPowerOverridableDestination(const Coord3D *) {}
    virtual bool isPowerCurrentlyInUse(const CommandButton *) const;
    virtual bool vslot07(int);
    virtual bool vslot08(int);

protected:
    virtual void onExit(bool, bool);
public:
    virtual bool rva00450AE9(); // slot 14, 0x00450AE9
    virtual void startPreparation();
    virtual bool continuePreparation();
    virtual void triggerAbilityEffect();
    virtual void vslot18() { m_2C = 0; } // 0x004C12CB: and [ecx+0x2c],0
    virtual void finishAbility();
protected:
    virtual bool handlePackingProcessing(); // slot 20, 0x0044F0B3
public:
    virtual void startPacking(bool);
    virtual void startUnpacking();

    SpecialPowerType getSpecialPowerType() const;
    SpecialPowerModuleInterface *rva0044E633();
    bool rva0044E85E(Coord3D *, float);
    bool rva0044E7A8(int); // 0x0044E7A8 (map: placeholder Rva0044E7A8, in no source file): vslot08 calls it directly on the module with its argument
    void startFacing(); // 0x00451EAC (map: placeholder Rva00451EAC; ecx is the module: idles the AI, sets +0x7E, faces the target object or position)
    unsigned int getPreparationFrames(); // 0x0044F633 (map: placeholder Rva0044F633; ecx is the module: reads the target ID +0x40 and the data)
    void rva0044EE07();
    bool rva0044E5E7(); // 0x0044E5E7 (map: placeholder class Rva0044E5E7; ecx is the module): state 4 and the pack time, update's pack gate
    bool rva0044E60D(); // 0x0044E60D: state 3 and the unpack time, update's unpack gate
    bool rva0044E655(); // 0x0044E655, bool rva0044E689(): the facing checks update's start-range branch makes
    bool rva0044E689(); // 0x0044E689: needs an AI and one of +0x7E/+0x7F clear
    void rva0044E6AE(); // 0x0044E6AE (map: placeholder Rva0044E6AE; callers pass the module): `onExit(false, true)` through the vtable (slot 13), called out of line by GiveUpgradeUpdate::update 0x0049C93F and 0x0049C8FC
    void rva0044EE80();
    void killSpecialObjects();
    Object *findSpecialObjectWithProducerID(const Object *target); // 0x0044F7EA (map: placeholder rva0044F7EA; its callers pass the module in ecx and it walks the list at +0x64)
    Object *rva0044FEB6();
    bool rva0044F679();
    void rva0044F6D0();
    UpdateSleepTime rva0044F881();

protected:
    virtual void loadPostProcess(); // 0x00497805, a jmp to UpdateModule's; out of line, so its subclasses' own forwarders (0x00483CEF) jump here (h05)
    virtual const char *GetSnapshotName() const; // slot 2, 0x0044F099 (retail table 0x0083FBA8)
    virtual void xfer(Xfer *);
    void validateSpecialObjects();
    void endPreparation();
    bool initLaser(Object *, Object *);
    bool isWithinStartAbilityRange();
    bool isWithinAbilityAbortRange() const;

    int m_24;
    unsigned int m_animFrames;
    unsigned int m_2C;
    unsigned int m_packingState;
    int m_34;   // AudioHandle (xfer 0x0044F996, endPreparation 0x0044F81E)
    int m_38;   // AudioHandle (xfer 0x0044F996)
    unsigned int m_prepFrames;
    ObjectID m_targetID;
    Coord3D m_targetPos;
    Coord3D m_50;
    int m_5C;
    int m_60;
    _STL::list<int> m_specialObjectIDList; // list<int>: killSpecialObjects 0x0044F72E clears it through _List_base<int, allocator<int>> 0x0043DAA5, xfer 0x0044F996 reaches Rva0036ABAFXfer
    unsigned int m_specialObjectEntries;
    unsigned int m_6C;
    float m_70;
    bool m_active;
    unsigned int m_78;
    bool m_7C;
    bool m_noTargetCommand;
    bool m_7E;
    bool m_7F;
    bool m_withinStartAbilityRange;
    bool m_81;
    bool m_82;
    bool m_83;
    unsigned int m_84;
};
// SIZE 0x88 (h05): ModuleFactory allocates 0x88 for the SpecialAbilityUpdate module itself
// (operator new before its constructor 0x0044EF5E), and the members above end with m_84.
typedef char SpecialAbilityUpdate_size[(sizeof(SpecialAbilityUpdate) == 0x88) ? 1 : -1];

#endif
