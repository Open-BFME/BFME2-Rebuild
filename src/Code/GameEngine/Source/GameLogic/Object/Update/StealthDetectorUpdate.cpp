// Portions derived from Command & Conquer Generals and Command & Conquer Generals Zero Hour, (c) Electronic Arts Inc., GPL-3.0-or-later; see LICENSE and NOTICE.
// FUNCTION: BFME2 0x004A2BD1 size=25 name=??1StealthDetectorUpdate@@UAE@XZ evidence=N:ledger
// FUNCTION: BFME2 0x004A2BEA size=6 name=?name@StealthDetectorUpdate@@QBEPBDXZ evidence=N:ledger
// FUNCTION: BFME2 0x004A2BF0 size=64 name=?allow@Rva004A2BF0@@QAE_NPAVObject@@@Z evidence=?:placeholder
// FUNCTION: BFME2 0x004A2C30 size=68 name=?xfer@StealthDetectorUpdate@@MAEXPAVXfer@@@Z evidence=N:ledger
// FUNCTION: BFME2 0x004A2C74 size=69 name=?getModuleNameKey@StealthDetectorUpdate@@UBE?AW4NameKeyType@@XZ evidence=N:ledger
// FUNCTION: BFME2 0x004A2CB9 size=116 name=??0StealthDetectorUpdate@@QAE@PAVThing@@PBVModuleData@@@Z evidence=N:ledger
// FUNCTION: BFME2 0x004A2D49 size=30 name=?rva004A2D49@Rva004A2D49@@QAEX_N@Z evidence=?:placeholder
// FUNCTION: BFME2 0x004A2D82 size=17 name=?buildFieldParse@StealthDetectorUpdateModuleData@@SAXAAVMultiIniFieldParse@@@Z evidence=N:ledger
// FUNCTION: BFME2 0x004A2D93 size=1579 name=?update@StealthDetectorUpdate@@UAE?AW4UpdateSleepTime@@XZ evidence=N:ledger
// cl: /D_STLP_USE_MALLOC /D_STLP_USE_STATIC_LIB /D_STLP_NO_EXCEPTIONS
// stlport46
#include <list>
// GameLogic/Object/Update/StealthDetectorUpdate.cpp: retail translation unit.
// Bodies are written against the shared headers; see StealthDetectorUpdate.h,
// StealthDetectorUpdateModuleData.h, Rva004A2BF0.h and Rva004A2D49.h.

#include <string.h>

#include "Common/GameType.h"
#include "Common/UpgradeCenter.h"
#include "Common/Player.h"
#include "Common/PlayerList.h"
#include "Common/GameAudio.h"
#include "GameClient/ParticleSys.h"
#include "GameClient/Drawable.h"
#include "GameLogic/FXList.h"
#include "GameLogic/GameLogic.h"
#include "GameLogic/InvisibilityManager.h"
#include "GameLogic/PartitionManager.h"
#include "GameLogic/PartitionFilterRelationship.h"
#include "GameLogic/PartitionFilterAcceptByKindOf.h"
#include "GameLogic/PartitionFilterSameMapStatus.h"
#include "GameLogic/Rva00261513Filter.h"
#include "GameLogic/Module/Rva00373EC6.h"
#include "GameLogic/Module/SpecialDisguiseUpdate.h"
#include "Common/INI.h"
#include "Common/INI/FieldParse.h"
#include "Common/BitFlags.h"
#include <stddef.h>
#include "Common/NameKeyGenerator.h"
#include "Common/Xfer.h"
#include "GameLogic/Module/ContainModule.h"
#include "GameLogic/Module/StealthDetectorUpdate.h"
#include "GameLogic/Module/StealthDetectorUpdateModuleData.h"
#include "GameLogic/Object.h"
#include "GameLogic/Rva004A2BF0.h"
#include "GameLogic/Rva004A2D49.h"

StealthDetectorUpdate::~StealthDetectorUpdate()
{
}

DisabledMaskType StealthDetectorUpdate::getDisabledTypesToProcess() const
{
	return DisabledMaskType(DisabledMaskType::kInit, 3);
}


Bool Rva004A2BF0::allow(Object *obj)
{
	if (obj->rva002943B2(m_player))
		return true;
	ContainModuleInterface *contain = obj->getContain();
	if (contain && contain->isGarrisonable() && contain->getStealthUnitsContained())
		return true;
	return false;
}

// Version 3 dropped the enabled flag; older saves still carry it, so it is read
// into a local and discarded.
void StealthDetectorUpdate::xfer(Xfer *xfer)
{
	Xfer::Version version(1, 3);
	*xfer == version;
	UpdateModule::xfer(xfer);
	if (version.m_minimum < 3)
	{
		Bool enabled = false;
		*xfer == enabled;
	}
}

NameKeyType StealthDetectorUpdate::getModuleNameKey() const
{
	static NameKeyType TheStealthDetectorUpdatePoolKey =
		TheNameKeyGenerator->nameToKey("StealthDetectorUpdate");
	return TheStealthDetectorUpdatePoolKey;
}

// Parks the module with a random wake over the data's update rate, or forever
// when the data starts disabled. The roll is the rowed 0x00233FF4, whose file
// argument is the source path (without its extension) and whose line is 79.
Int __cdecl GetGameLogicRandomValue(Int low, Int high, const char *file, Int line);

StealthDetectorUpdate::StealthDetectorUpdate(Thing *thing, const ModuleData *moduleData)
	: UpdateModule(thing, moduleData)
{
	const StealthDetectorUpdateModuleData *data = (const StealthDetectorUpdateModuleData *)getModuleData();
	UnsignedInt wake;
	if (data->m_initiallyDisabled == 0)
		wake = GetGameLogicRandomValue(1, data->m_updateRate, "C:\\projects\\bfme2patch103\\bfme2\\Code\\GameEngine\\Source\\GameLogic\\Object\\Update\\StealthDetectorUpdate", 79);
	else
		wake = UPDATE_SLEEP_FOREVER;
	setWakeFrame(getObject(), (UpdateSleepTime)wake);
}


void Rva004A2D49::rva004A2D49(Bool enabled)
{
	UpdateSleepTime wakeTime = enabled ? UPDATE_SLEEP_NONE : UPDATE_SLEEP_FOREVER;
	setWakeFrame(m_object, wakeTime);
}

// The INI field table buildFieldParse registers (retail .rdata 0x00852188: sixteen entries and the terminator; tokens,
// parsers and offsets read from game.dat).
const FieldParse StealthDetectorUpdateModuleDataFieldParse[] =
{
	{ "DetectionRate", INI::parseDurationUnsignedInt, 0, offsetof(StealthDetectorUpdateModuleData, m_updateRate) },
	{ "DetectionRange", INI::parseReal, 0, offsetof(StealthDetectorUpdateModuleData, m_detectionRange) },
	{ "InitiallyDisabled", INI::parseBool, 0, offsetof(StealthDetectorUpdateModuleData, m_initiallyDisabled) },
	{ "PingSound", INI::parseAudioEventRTS, 0, offsetof(StealthDetectorUpdateModuleData, m_pingSound) },
	{ "LoudPingSound", INI::parseAudioEventRTS, 0, offsetof(StealthDetectorUpdateModuleData, m_loudPingSound) },
	{ "IRBeaconParticleSysName", INI::parseParticleSystemTemplate, 0, offsetof(StealthDetectorUpdateModuleData, m_IRBeaconParticleSysTmpl) },
	{ "IRParticleSysName", INI::parseParticleSystemTemplate, 0, offsetof(StealthDetectorUpdateModuleData, m_IRParticleSysTmpl) },
	{ "IRBrightParticleSysName", INI::parseParticleSystemTemplate, 0, offsetof(StealthDetectorUpdateModuleData, m_IRBrightParticleSysTmpl) },
	{ "IRGridParticleSysName", INI::parseParticleSystemTemplate, 0, offsetof(StealthDetectorUpdateModuleData, m_IRGridParticleSysTmpl) },
	{ "IRParticleSysBone", INI::parseAsciiString, 0, offsetof(StealthDetectorUpdateModuleData, m_IRParticleSysBone) },
	{ "ExtraRequiredKindOf", BitFlags<218>::parseFromINI, 0, offsetof(StealthDetectorUpdateModuleData, m_extraDetectKindof) },
	{ "ExtraForbiddenKindOf", BitFlags<218>::parseFromINI, 0, offsetof(StealthDetectorUpdateModuleData, m_extraDetectKindofNot) },
	{ "CanDetectWhileGarrisoned", INI::parseBool, 0, offsetof(StealthDetectorUpdateModuleData, m_garrison) },
	{ "CanDetectWhileContained", INI::parseBool, 0, offsetof(StealthDetectorUpdateModuleData, m_transport) },
	{ "CancelOneRingEffect", INI::parseBool, 0, offsetof(StealthDetectorUpdateModuleData, m_cancelRing) },
	{ "RequiredUpgrade", INI::parseAsciiString, 0, offsetof(StealthDetectorUpdateModuleData, m_upgradeName) },
	{ 0, 0, 0, 0 }
};

void StealthDetectorUpdateModuleData::buildFieldParse(MultiIniFieldParse &parse)
{
	parse.add(StealthDetectorUpdateModuleDataFieldParse, 0);
}



void StealthDetectorUpdate::loadPostProcess()
{
	UpdateModule::loadPostProcess();
}

// 0x004A2BEA
const char *StealthDetectorUpdate::GetSnapshotName() const { return "StealthDetectorUpdate"; }
