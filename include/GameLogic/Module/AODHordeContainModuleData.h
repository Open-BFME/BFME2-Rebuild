// The Army of the Dead formation adds oscillation and scatter configuration.
#pragma once

#include <stddef.h>
#include "GameLogic/Module/HordeContainModuleData.h"

class AODHordeContainModuleData : public HordeContainModuleData
{
public:
  AODHordeContainModuleData();
  static void buildFieldParse(MultiIniFieldParse &parse);

  float FrequencyScale;
  float FrequencyRandomness;
  float AmplitudeScale;
  float AmplitudeRandomness;
  float StillAmplitude;
  float FrequencyScaleZ;
  float FrequencyRandomnessZ;
  float AmplitudeScaleZ;
  float AmplitudeRandomnessZ;
  float StillAmplitudeZ;
  float OathFulfilledZFactor;
  float LargeUnitHeightFactor;
  float LargeUnitMinHeight;
  float LargeUnitMaxHeight;
  unsigned int LargeUnitTimeout;
  float LargeUnitTailOff;
  float ScatterSpeedFactor;
  float ScatterRandomness;
};

typedef char check_size_AODHordeContainModuleData[sizeof(AODHordeContainModuleData)==0x2BC?1:-1];
typedef char check_AODHordeContainModuleData_FrequencyScale[offsetof(AODHordeContainModuleData,FrequencyScale)==0x274?1:-1];
typedef char check_AODHordeContainModuleData_FrequencyRandomness[offsetof(AODHordeContainModuleData,FrequencyRandomness)==0x278?1:-1];
typedef char check_AODHordeContainModuleData_AmplitudeScale[offsetof(AODHordeContainModuleData,AmplitudeScale)==0x27C?1:-1];
typedef char check_AODHordeContainModuleData_AmplitudeRandomness[offsetof(AODHordeContainModuleData,AmplitudeRandomness)==0x280?1:-1];
typedef char check_AODHordeContainModuleData_StillAmplitude[offsetof(AODHordeContainModuleData,StillAmplitude)==0x284?1:-1];
typedef char check_AODHordeContainModuleData_FrequencyScaleZ[offsetof(AODHordeContainModuleData,FrequencyScaleZ)==0x288?1:-1];
typedef char check_AODHordeContainModuleData_FrequencyRandomnessZ[offsetof(AODHordeContainModuleData,FrequencyRandomnessZ)==0x28C?1:-1];
typedef char check_AODHordeContainModuleData_AmplitudeScaleZ[offsetof(AODHordeContainModuleData,AmplitudeScaleZ)==0x290?1:-1];
typedef char check_AODHordeContainModuleData_AmplitudeRandomnessZ[offsetof(AODHordeContainModuleData,AmplitudeRandomnessZ)==0x294?1:-1];
typedef char check_AODHordeContainModuleData_StillAmplitudeZ[offsetof(AODHordeContainModuleData,StillAmplitudeZ)==0x298?1:-1];
typedef char check_AODHordeContainModuleData_OathFulfilledZFactor[offsetof(AODHordeContainModuleData,OathFulfilledZFactor)==0x29C?1:-1];
typedef char check_AODHordeContainModuleData_LargeUnitHeightFactor[offsetof(AODHordeContainModuleData,LargeUnitHeightFactor)==0x2A0?1:-1];
typedef char check_AODHordeContainModuleData_LargeUnitMinHeight[offsetof(AODHordeContainModuleData,LargeUnitMinHeight)==0x2A4?1:-1];
typedef char check_AODHordeContainModuleData_LargeUnitMaxHeight[offsetof(AODHordeContainModuleData,LargeUnitMaxHeight)==0x2A8?1:-1];
typedef char check_AODHordeContainModuleData_LargeUnitTimeout[offsetof(AODHordeContainModuleData,LargeUnitTimeout)==0x2AC?1:-1];
typedef char check_AODHordeContainModuleData_LargeUnitTailOff[offsetof(AODHordeContainModuleData,LargeUnitTailOff)==0x2B0?1:-1];
typedef char check_AODHordeContainModuleData_ScatterSpeedFactor[offsetof(AODHordeContainModuleData,ScatterSpeedFactor)==0x2B4?1:-1];
typedef char check_AODHordeContainModuleData_ScatterRandomness[offsetof(AODHordeContainModuleData,ScatterRandomness)==0x2B8?1:-1];
