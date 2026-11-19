// StrafeAreaUpdateModuleData.h: the strafe run's INI data. SIZE 0x20.
// Target facts: ctor 0x003A4E5A stores vtable 0x00C1B268, clears the weapon-name
// string at +0x08 and sets the four sweep reals at +0x0C/+0x10/+0x14/+0x18
// (150.0, 0.4, 100.0, 50.0) plus the zero phase at +0x1C. Destructor
// 0x003A4ECF releases +0x08. buildFieldParse 0x003A4D12 registers the retail
// table at 0x00C1B1F8 (WeaponName, StrafeAreaRadius, Sweepfrequency,
// SweepAmplitude, Slope) with offset 0.
#pragma once
#ifndef BFME2_STRAFEAREAUPDATEMODULEDATA_H
#define BFME2_STRAFEAREAUPDATEMODULEDATA_H

#include "Common/AsciiString.h"
#include "GameLogic/Module/ModuleData.h"

class MultiIniFieldParse;
struct FieldParse;

class StrafeAreaUpdateModuleData : public ModuleData
{
public:
	StrafeAreaUpdateModuleData();
	virtual ~StrafeAreaUpdateModuleData();

	static void buildFieldParse(MultiIniFieldParse &parse);

	AsciiString m_weaponName;							// 0x08
	Real m_strafeAreaRadius;							// 0x0C
	Real m_sweepFrequency;								// 0x10
	Real m_sweepAmplitude;								// 0x14
	Real m_slope;										// 0x18
	Real m_initialSweepPhase;							// 0x1C
};
// SIZE 0x20

extern const FieldParse StrafeAreaUpdateModuleDataFieldParse[];

#endif
