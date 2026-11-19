// Diagnostic pilot import; object boundaries may be unproven.
// FUNCTION: BFME2 0x004AC24D size=91 name=??1RainOfFireUpdate@@UAE@XZ evidence=N:ledger tier=B hypothesis:mm1-held-object-same-path
// FUNCTION: BFME2 0x004AC2A8 size=817 name=?update@RainOfFireUpdate@@UAE?AW4UpdateSleepTime@@XZ evidence=N:ledger
// GameLogic/Object/Update/RainOfFireUpdate.cpp: retail translation unit (pilot skeleton).
// Write each function below its FUNCTION annotation, in this (retail) order.
// Bodies matched in the old repo (read-only reference) are listed per function as old=<source>.
// Shared types belong in include/; no class or struct with a body in this file.
// cl: 
#include "GameLogic/Module/RainOfFireUpdate.h"
#include "GameLogic/Object.h"
#include "GameLogic/GameLogic.h"
#include "GameClient/Display.h"
#include "Common/INI.h"
#include "Common/Xfer.h"
#include "Common/NameKeyGenerator.h"
#include <stddef.h>

RainOfFireUpdateModuleData::RainOfFireUpdateModuleData()
{
	m_height = 100.0f;
	m_darkness = 0.25f;
	m_startRainTime = 150;
	m_darknessFadeTime = 120;
	m_jitter = 0.0f;
	m_dpsMin = 1.0f;
	m_dpsMax = 1.0f;
	m_rampup = 0.0f;
	m_offset.x = 0.0f;
	m_offset.y = 0.0f;
}

void RainOfFireUpdateModuleData::buildFieldParse(MultiIniFieldParse &p)
{
	static const FieldParse dataFieldParse[] =
	{
		{ "StartRainTime", INI::parseDurationUnsignedInt, NULL, offsetof(RainOfFireUpdateModuleData, m_startRainTime) },
		{ "DarknessFadeTime", INI::parseDurationUnsignedInt, NULL, offsetof(RainOfFireUpdateModuleData, m_darknessFadeTime) },
		{ "RainEmitterHeight", INI::parsePositiveNonZeroReal, NULL, offsetof(RainOfFireUpdateModuleData, m_height) },
		{ "DarknessLevel", INI::parsePositiveNonZeroReal, NULL, offsetof(RainOfFireUpdateModuleData, m_darkness) },
		{ "JitterRadius", INI::parseReal, NULL, offsetof(RainOfFireUpdateModuleData, m_jitter) },
		{ "DPSMin", INI::parsePositiveNonZeroReal, NULL, offsetof(RainOfFireUpdateModuleData, m_dpsMin) },
		{ "DPSMax", INI::parsePositiveNonZeroReal, NULL, offsetof(RainOfFireUpdateModuleData, m_dpsMax) },
		{ "DPSRampupTime", INI::parseDurationReal, NULL, offsetof(RainOfFireUpdateModuleData, m_rampup) },
		{ "RainOffset", INI::parseCoord2D, NULL, offsetof(RainOfFireUpdateModuleData, m_offset) },
		{ 0, 0, 0, 0 }
	};
	p.add(dataFieldParse);
}

RainOfFireUpdate::RainOfFireUpdate(Thing *thing, const ModuleData *moduleData)
	: UpdateModule(thing, moduleData)
{
	const RainOfFireUpdateModuleData *data = getRainOfFireUpdateModuleData();
	m_startFrame = UPDATE_SLEEP_FOREVER;
	m_darkness = 0.0f;
	m_scale = 1.0f;
	m_pending = 0.0f;
	m_ramp = 0.0f;
	if (data)
	{
		m_startFrame = TheGameLogic->getFrame();
		m_ramp = data->m_rampup == 0.0f ? 1.0f : 0.0f;
	}
}

const char *RainOfFireUpdate::GetSnapshotName() const
{
	return "RainOfFireUpdate";
}

void RainOfFireUpdate::rva004AC135(Real delta)
{
	m_scale += delta;
}

NameKeyType RainOfFireUpdate::getModuleNameKey() const
{
	static NameKeyType TheRainOfFireUpdatePoolKey = TheNameKeyGenerator->nameToKey("RainOfFireUpdate");
	return TheRainOfFireUpdatePoolKey;
}

void RainOfFireUpdate::rva004AC18D(Real fadeFraction)
{
	m_darkness = fadeFraction;
	if (m_moduleData == NULL)
		return;
	if (TheDisplay == NULL)
		return;
	TheDisplay->slot48(1.0 - (getRainOfFireUpdateModuleData()->m_darkness * m_scale * fadeFraction));
}

void RainOfFireUpdate::xfer(Xfer *xfer)
{
	UpdateModule::xfer(xfer);
	if (xfer->IsLightCRC())
		return;
	xfer->Version1();
	*xfer == m_startFrame;
	*xfer == m_darkness;
	*xfer == m_scale;
	*xfer == m_pending;
	*xfer == m_ramp;
}

void RainOfFireUpdate::loadPostProcess()
{
	UpdateModule::loadPostProcess();
	setScale(m_scale);
	rva004AC18D(m_darkness);
}

RainOfFireUpdate::~RainOfFireUpdate()
{
	m_scale = 0.0f;
	rva004AC18D(0.0f);
}







// land smoke test RainOfFireUpdate
