#pragma once
#include "GameLogic/Module/ModuleData.h"
#include "GameLogic/Module/DieMuxData.h"
#include "GameLogic/Module/RubbleRiseTypes.h"
#include <vector>
class INI;
class FXList;
class ObjectCreationList;
typedef std::vector<const FXList *> FXVec;
typedef std::vector<const ObjectCreationList *> OCLVec;
class RubbleRiseUpdateModuleData : public ModuleData
{
public:
 RubbleRiseUpdateModuleData(); // 0x00256653
 static void parseFXList(INI *,void *,void *,const void *);
 static void parseOCL(INI *,void *,void *,const void *);
 static void buildFieldParse(MultiIniFieldParse &);
 static ModuleData *friend_newModuleData(INI *ini); // 0x002566F6
 DieMuxData m_dieMuxData; // 0x08
 int m_minRubbleRiseDelay; // 0x38 MinRubbleRiseDelay
 int m_maxRubbleRiseDelay; // 0x3C MaxRubbleRiseDelay
 unsigned m_minBurstDelay; // 0x40
 unsigned m_maxBurstDelay; // 0x44
 int m_bigBurstFrequency; // 0x48
 float m_rubbleRiseDamping; // 0x4C
 float m_rubbleHeight; // 0x50
 float m_maxShudder; // 0x54
 OCLVec m_ocls[RR_PHASE_COUNT]; // 0x58
 FXVec m_fxs[RR_PHASE_COUNT]; // 0x88
 int m_oclCount[RR_PHASE_COUNT]; // 0xB8
 int m_fxCount[RR_PHASE_COUNT]; // 0xC8
};
// SIZE 0xD8
