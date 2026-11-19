#pragma once
#include "Common/GameType.h"
#include "GameLogic/Module/FloodPath.h"
class FloodUpdateRecord
{
public:
 FloodUpdateRecord() throw();
 ObjectID m_id; // 0x00
 FloodPath m_path; // 0x04
 int m_index; // 0x10
};
// SIZE 0x14
