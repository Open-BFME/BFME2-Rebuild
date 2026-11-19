#pragma once
#include "Common/ModelConditionMask.h"
class ObjectCreationList;
class RiderInfo
{
public:
 RiderInfo();
 RiderInfo(const RiderInfo &);
 ModelConditionMask m_conditions; // 0x00
 int m_animationTime; // 0x4C
 ObjectCreationList *m_creation; // 0x50
};
// SIZE 0x54
