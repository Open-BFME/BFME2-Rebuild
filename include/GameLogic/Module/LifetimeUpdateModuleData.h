#pragma once
#include "GameLogic/Module/ModuleData.h"
class MultiIniFieldParse;
struct FieldParse;
enum DeathType;
// Target factory allocates0x18; INI table at0xC1B0F0 gives all five offsets.
class LifetimeUpdateModuleData : public ModuleData
{
public:
    LifetimeUpdateModuleData();
    static void buildFieldParse(MultiIniFieldParse &parse);
    static const FieldParse m_fieldParseTable[];
    UnsignedInt m_minFrames; // +08
    UnsignedInt m_maxFrames; // +0C
    Bool m_startDisabled; // +10 WaitForWakeUp
    Bool m_creditKiller; // +11 ScoreKill
    DeathType m_deathType; // +14
};
typedef char check_LifetimeUpdateModuleData_size[sizeof(LifetimeUpdateModuleData)==0x18?1:-1];
