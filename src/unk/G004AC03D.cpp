// cl: /O1 /arch:SSE
// stlport46
#include "Common/INI.h"
#include "GameLogic/Module/RainOfFireUpdate.h"
#include "GameLogic/GameLogic.h"
// Generated from the ledger; map snapshot c591f7146f88c8a8248ebb8ae22f0c978c22aed463db64e05ef57ff0dfae99cc
// object=G004AC03D name-evidence=invented
// Extents and edges are hypotheses; byte verification alone does not establish original TU boundaries.
// FUNCTION: BFME2 0x004AC03D size=90 name=??0RainOfFireUpdateModuleData@@QAE@XZ evidence=N:ledger tier=B hypothesis:mm1-gap-unit
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

// Field parse table for RainOfFireUpdateModuleData (decoded from retail image).
static const FieldParse s_rainOfFireUpdateModuleDataFieldParse[] =
{
	{ "StartRainTime", INI::parseDurationUnsignedInt, 0, 0x8 },
	{ "DarknessFadeTime", INI::parseDurationUnsignedInt, 0, 0xC },
	{ "RainEmitterHeight", INI::parsePositiveNonZeroReal, 0, 0x10 },
	{ "DarknessLevel", INI::parsePositiveNonZeroReal, 0, 0x14 },
	{ "JitterRadius", INI::parseReal, 0, 0x18 },
	{ "DPSMin", INI::parsePositiveNonZeroReal, 0, 0x1C },
	{ "DPSMax", INI::parsePositiveNonZeroReal, 0, 0x20 },
	{ "DPSRampupTime", INI::parseDurationReal, 0, 0x24 },
	{ "RainOffset", INI::parseCoord2D, 0, 0x28 },
	{ 0, 0, 0, 0 }
};

// FUNCTION: BFME2 0x004AC097 size=17 name=?buildFieldParse@RainOfFireUpdateModuleData@@SAXAAVMultiIniFieldParse@@@Z evidence=N:ledger tier=B hypothesis:mm1-gap-unit
void RainOfFireUpdateModuleData::buildFieldParse(MultiIniFieldParse &parse)
{
	parse.add(s_rainOfFireUpdateModuleDataFieldParse, 0);
}

// FUNCTION: BFME2 0x004AC0A8 size=135 name=??0RainOfFireUpdate@@QAE@PAVThing@@PBVModuleData@@@Z evidence=N:ledger tier=B hypothesis:mm1-gap-unit
RainOfFireUpdate::RainOfFireUpdate(Thing *thing, const ModuleData *moduleData) :
	UpdateModule(thing, moduleData)
{
	m_startFrame = 0x3FFFFFFF;
	m_darkness = 0.0f;
	m_scale = 1.0f;
	m_pending = 0.0f;
	m_ramp = 0.0f;
	const RainOfFireUpdateModuleData *data = getRainOfFireUpdateModuleData();
	if (data) {
		m_startFrame = TheGameLogic->getFrame();
		m_ramp = (data->m_rampup == 0.0f) ? 1.0f : 0.0f;
	}
}
