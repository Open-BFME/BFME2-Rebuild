#pragma once
#include "GameLogic/Module/SpecialAbilityUpdateModuleData.h"
class ArrowStormUpdateModuleData : public SpecialAbilityUpdateModuleData
{
public:
    ArrowStormUpdateModuleData();
    AsciiString m_weaponTemplate; // 0xC8
    float m_targetRadius; // 0xCC
    int m_shotsPerTarget; // 0xD0
    int m_shotsPerBurst; // 0xD4
    int m_maxShots; // 0xD8
    bool m_canShootEmptyGround; // 0xDC
};
// SIZE 0xE0
