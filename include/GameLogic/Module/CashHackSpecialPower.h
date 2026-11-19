// CashHackSpecialPower: SpecialPowerModule, no added members; SIZE 0x34.
// Factory25191F allocates34; ctor4C2626 stores C5C63C at0 and C5C5D8 at10.
// The primary table has14 inherited slots; the secondary has25, with action
// 4C26D0 at11 and an empty location action at12. WB125B700 corroborates
// CashHackSpecialPower's action and its science-selected money amount.
#pragma once
#ifndef BFME2_CASHHACKSPECIALPOWER_H
#define BFME2_CASHHACKSPECIALPOWER_H
#include "GameLogic/Module/SpecialPowerModule.h"
#include "Common/Vector.h"
enum ScienceType;
class CashHackSpecialPowerModuleData : public SpecialPowerModuleData
{
public:
    // Parser4C2918 appends an8-byte science/amount pair; native vectors are
    // at7C..87 and fallback88. Upgrades/field names are donor name leads.
    struct Upgrades {
        ScienceType m_science;
        int m_amountToSteal;
        Upgrades();
        Upgrades(int,const int &);
        Upgrades(const Upgrades &);
    };
    CashHackSpecialPowerModuleData();
    static void buildFieldParse(MultiIniFieldParse &);
    _STL::vector<Upgrades> m_upgrades; //7C
    int m_defaultAmountToSteal; //88
};
// SIZE 0x8C
typedef char CashHackDataSize[sizeof(CashHackSpecialPowerModuleData)==0x8C?1:-1];
class CashHackSpecialPower : public SpecialPowerModule
{
public:
    CashHackSpecialPower(Thing *,const ModuleData *);
    virtual ~CashHackSpecialPower();
    virtual NameKeyType getModuleNameKey() const;
    virtual void doSpecialPowerAtObject(Object *,unsigned int);
    virtual void doSpecialPowerAtLocation(const Coord3D *,unsigned int) {}
protected:
    virtual void loadPostProcess();
    virtual const char *GetSnapshotName() const;
    virtual void xfer(Xfer *);
    int findAmountToSteal() const;
    const CashHackSpecialPowerModuleData *getCashHackSpecialPowerModuleData() const {
        return static_cast<const CashHackSpecialPowerModuleData *>(getModuleData());
    }
};
typedef char CashHackSize[sizeof(CashHackSpecialPower)==0x34?1:-1];
#endif
