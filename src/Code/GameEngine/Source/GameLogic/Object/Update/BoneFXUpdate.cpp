// Portions derived from Command & Conquer Generals and Command & Conquer Generals Zero Hour, (c) Electronic Arts Inc., GPL-3.0-or-later; see LICENSE and NOTICE.
// Diagnostic pilot import; object boundaries may be unproven.
// FUNCTION: BFME2 0x0048707A size=124 name=?onObjectCreated@BoneFXUpdate@@MAEXXZ evidence=N:ledger
// FUNCTION: BFME2 0x004870F6 size=69 name=?parseGameClientRandomDelay@@YAXPAVINI@@PAXPAVGameClientRandomVariable@@@Z evidence=N:ledger
// FUNCTION: BFME2 0x0048713B size=54 name=?computeNextClientFXTime@BoneFXUpdate@@IAEXPBUBaseBoneListInfo@@AAH@Z evidence=N:ledger
// FUNCTION: BFME2 0x00487171 size=54 name=?computeNextLogicFXTime@BoneFXUpdate@@IAEXPBUBaseBoneListInfo@@AAH@Z evidence=N:ledger
// FUNCTION: BFME2 0x004871A7 size=88 name=?parseFXLocInfo@@YAXPAVINI@@PAXPAUBoneLocInfo@@@Z evidence=N:ledger
// FUNCTION: BFME2 0x00487200 size=175 name=?parseFXList@BoneFXUpdateModuleData@@SAXPAVINI@@PAX1PBX@Z evidence=N:ledger
// FUNCTION: BFME2 0x004872AF size=175 name=?parseObjectCreationList@BoneFXUpdateModuleData@@SAXPAVINI@@PAX1PBX@Z evidence=N:ledger
// FUNCTION: BFME2 0x0048735E size=175 name=?parseParticleSystem@BoneFXUpdateModuleData@@SAXPAVINI@@PAX1PBX@Z evidence=N:ledger
// FUNCTION: BFME2 0x0048740D size=115 name=?doFXListAtBone@BoneFXUpdate@@IAEXPBVFXList@@PBUCoord3D@@@Z evidence=N:ledger
// FUNCTION: BFME2 0x00487480 size=116 name=?doOCLAtBone@BoneFXUpdate@@IAEXPBVObjectCreationList@@PBUCoord3D@@@Z evidence=N:ledger
// FUNCTION: BFME2 0x0048750B size=297 name=?initTimes@BoneFXUpdate@@IAEXXZ evidence=N:ledger
// FUNCTION: BFME2 0x00487634 size=355 name=?resolveBoneLocations@BoneFXUpdate@@MAEXXZ evidence=N:ledger
// FUNCTION: BFME2 0x00487797 size=196 name=??0BoneFXUpdateModuleData@@QAE@XZ evidence=N:ledger
// FUNCTION: BFME2 0x00487877 size=111 name=??1BoneFXUpdateModuleData@@UAE@XZ evidence=N:ledger
// FUNCTION: BFME2 0x004878E6 size=307 name=??0BoneFXUpdate@@QAE@PAVThing@@PBVModuleData@@@Z evidence=N:ledger
// FUNCTION: BFME2 0x00487A19 size=69 name=?getModuleNameKey@BoneFXUpdate@@UBE?AW4NameKeyType@@XZ evidence=N:ledger
// FUNCTION: BFME2 0x00487A5E size=6 name=?name@Rva00487A5ENamed@@QBEPBDXZ evidence=?:placeholder
// FUNCTION: BFME2 0x00487A64 size=49 name=?killRunningParticleSystems@BoneFXUpdate@@IAEXXZ evidence=N:ledger
// FUNCTION: BFME2 0x00487A95 size=42 name=?stopAllBoneFX@BoneFXUpdate@@QAEXXZ evidence=N:ledger
// FUNCTION: BFME2 0x00487ABF size=161 name=??1BoneFXUpdate@@MAE@XZ evidence=N:ledger
// FUNCTION: BFME2 0x00487B60 size=29 name=?changeBodyDamageState@BoneFXUpdate@@QAEXW4BodyDamageType@@0@Z evidence=N:ledger
// FUNCTION: BFME2 0x00487B7D size=248 name=?doParticleSystemAtBone@BoneFXUpdate@@IAEXPBVParticleSystemTemplate@@PBUCoord3D@@@Z evidence=N:ledger
// FUNCTION: BFME2 0x00487C75 size=349 name=?xfer@BoneFXUpdate@@MAEXPAVXfer@@@Z evidence=N:ledger
// FUNCTION: BFME2 0x00487DEE size=332 name=?update@BoneFXUpdate@@UAE?AW4UpdateSleepTime@@XZ evidence=N:ledger
// GameLogic/Object/Update/BoneFXUpdate.cpp: retail translation unit (pilot skeleton).
// Write each function below its FUNCTION annotation, in this (retail) order.
// Bodies matched in the old repo (read-only reference) are listed per function as old=<source>.
// Shared types belong in include/; no class or struct with a body in this file.
// cl: /EHsc- /D_BFME_RETAIL_TREE_INSERT_LAYOUT /D_STLP_USE_MALLOC /D_STLP_USE_STATIC_LIB /D_CRTIMP=
// stlport46
#include <vector>
#include "Common/Rva004874F4.h"
#include "GameClient/Drawable.h"
#include "GameLogic/Module/BodyModule.h"
#include "GameLogic/Damage.h"
#include "GameLogic/FXList.h"
#include "GameLogic/ObjectCreationList.h"
#include "Common/INIException.h"
#include <string.h>
namespace BoneFXCRT { extern "C" __declspec(dllimport) int __cdecl _strcmpi(const char *,const char *); }
#include "GameLogic/Module/BoneFXUpdate.h"
#include "GameLogic/Object.h"
#include "GameLogic/GameLogic.h"
#include "GameClient/ParticleSys.h"
#include "Common/INI.h"
#include "Common/Xfer.h"
#include "Common/XferException.h"
#include "Common/NameKeyGenerator.h"

BoneFXBonePosition::BoneFXBonePosition() {}
BoneFXBonePosition::~BoneFXBonePosition() {}

void BoneFXUpdate::onObjectCreated()
{
	static NameKeyType key_BoneFXDamage = TheNameKeyGenerator->nameToKey("BoneFXDamage");
	DamageModule *bfxd = getObject()->findDamageModule(key_BoneFXDamage);
	if (bfxd == NULL)
	{
		throw INIException(3, "BoneFXUpdate requires BoneFXDamage");
	}
}


void parseGameClientRandomDelay(INI *ini, void *instance, GameClientRandomVariable *delay)
{
	Real min, max;
	INI::parseDurationReal(ini, instance, &min, NULL);
	INI::parseDurationReal(ini, instance, &max, NULL);
	delay->setRange(min, max, GameClientRandomVariable::UNIFORM);
}

void BoneFXUpdate::computeNextClientFXTime(const BaseBoneListInfo *info, Int &nextFrame)
{
	if (info->onlyOnce)
	{
		nextFrame = -1;
		return;
	}
	Int frame = TheGameLogic->getFrame();
	nextFrame = frame + (Int)info->gameClientDelay.getValue();
}

void BoneFXUpdate::computeNextLogicFXTime(const BaseBoneListInfo *info, Int &nextFrame)
{
	if (info->onlyOnce)
	{
		nextFrame = -1;
		return;
	}
	Int frame = TheGameLogic->getFrame();
	nextFrame = frame + (Int)info->gameLogicDelay.getValue();
}

static void parseFXLocInfo(INI *ini, void *instance, BoneLocInfo *locInfo)
{
	const char *token = ini->getNextToken(ini->getSepsColon());
	if (BoneFXCRT::_strcmpi(token, "bone") == 0)
	{
		locInfo->boneName.set(ini->getNextToken());
	}
	else
	{
		throw INIException(3, "'bone' expected");
	}
}

void BoneFXUpdateModuleData::parseFXList(INI *ini, void *instance, void *store, const void *userData)
{
	BoneFXListInfo *info = (BoneFXListInfo *)store;
	parseFXLocInfo(ini, instance, &info->locInfo);
	const char *token = ini->getNextToken(ini->getSepsColon());
	if (BoneFXCRT::_strcmpi(token, "onlyonce") != 0)
		throw INIException(3, "'onlyonce' expected");
	INI::parseBool(ini, instance, &info->onlyOnce, NULL);
	parseGameClientRandomDelay(ini, instance, (GameClientRandomVariable *)&info->gameLogicDelay);
	token = ini->getNextToken(ini->getSepsColon());
	if (BoneFXCRT::_strcmpi(token, "fxlist") != 0)
		throw INIException(3, "'fxlist' expected");
	INI::parseFXList(ini, instance, (void *)&info->fx, NULL);
}

void BoneFXUpdateModuleData::parseObjectCreationList(INI *ini, void *instance, void *store, const void *userData)
{
	BoneOCLInfo *info = (BoneOCLInfo *)store;
	parseFXLocInfo(ini, instance, &info->locInfo);
	const char *token = ini->getNextToken(ini->getSepsColon());
	if (BoneFXCRT::_strcmpi(token, "onlyonce") != 0)
		throw INIException(3, "'onlyonce' expected");
	INI::parseBool(ini, instance, &info->onlyOnce, NULL);
	parseGameClientRandomDelay(ini, instance, (GameClientRandomVariable *)&info->gameLogicDelay);
	token = ini->getNextToken(ini->getSepsColon());
	if (BoneFXCRT::_strcmpi(token, "ocl") != 0)
		throw INIException(3, "'ocl' expected");
	INI::parseObjectCreationList(ini, instance, (void *)&info->ocl, NULL);
}

void BoneFXUpdateModuleData::parseParticleSystem(INI *ini, void *instance, void *store, const void *userData)
{
	BoneParticleSystemInfo *info = (BoneParticleSystemInfo *)store;
	parseFXLocInfo(ini, instance, &info->locInfo);
	const char *token = ini->getNextToken(ini->getSepsColon());
	if (BoneFXCRT::_strcmpi(token, "onlyonce") != 0)
		throw INIException(3, "'onlyonce' expected");
	INI::parseBool(ini, instance, &info->onlyOnce, NULL);
	parseGameClientRandomDelay(ini, instance, &info->gameClientDelay);
	token = ini->getNextToken(ini->getSepsColon());
	if (BoneFXCRT::_strcmpi(token, "psys") != 0)
		throw INIException(3, "'psys' expected");
	INI::parseParticleSystemTemplate(ini, instance, (void *)&info->particleSysTemplate, NULL);
}


void BoneFXUpdate::doFXListAtBone(const FXList *fxList, const Coord3D *bonePosition)
{
	if (m_bonesResolved[m_curBodyState] == FALSE)
		resolveBoneLocations();

	const BoneFXUpdateModuleData *boneFXUpdateModuleData = getBoneFXUpdateModuleData();
	const DamageInfo *lastDamageInfo = getObject()->getBodyModule()->getLastDamageInfo();
	if (lastDamageInfo && getDamageTypeFlag(boneFXUpdateModuleData->m_damageFXTypes, lastDamageInfo->in.m_damageType) == FALSE)
		return;

	Object *building = getObject();
	Coord3D newPos;
	building->convertBonePosToWorldPos(bonePosition, NULL, &newPos, NULL);
	FXList::doFXPos(fxList, &newPos, NULL);
}

void BoneFXUpdate::doOCLAtBone(const ObjectCreationList *ocl, const Coord3D *bonePosition)
{
	if (m_bonesResolved[m_curBodyState] == FALSE)
		resolveBoneLocations();

	const BoneFXUpdateModuleData *boneFXUpdateModuleData = getBoneFXUpdateModuleData();
	const DamageInfo *lastDamageInfo = getObject()->getBodyModule()->getLastDamageInfo();
	if (lastDamageInfo && getDamageTypeFlag(boneFXUpdateModuleData->m_damageOCLTypes, lastDamageInfo->in.m_damageType) == FALSE)
		return;

	Object *building = getObject();
	Coord3D newPos;
	building->convertBonePosToWorldPos(bonePosition, NULL, &newPos, NULL);
	ObjectCreationList::create(ocl, building, &newPos, NULL);
}


Rva004874F4::Rva004874F4() : m_unk0x00(0), m_unk0x04(0), m_unk0x08(0.0f), m_unk0x0c(0.0f) {}

void BoneFXUpdate::initTimes()
{
	int i;
	const BoneFXUpdateModuleData *boneFXUpdateModuleData = getBoneFXUpdateModuleData();
	int now = TheGameLogic->getFrame();
	for (i = 0; i < BONE_FX_MAX_BONES; ++i)
	{
		if (boneFXUpdateModuleData->m_fxList[m_curBodyState][i].locInfo.boneName.compare(AsciiString::TheEmptyString) != 0)
			m_nextFXFrame[m_curBodyState][i] = now + (int)boneFXUpdateModuleData->m_fxList[m_curBodyState][i].gameLogicDelay.getValue();
		else
			m_nextFXFrame[m_curBodyState][i] = -1;
		if (boneFXUpdateModuleData->m_OCL[m_curBodyState][i].locInfo.boneName.compare(AsciiString::TheEmptyString) != 0)
			m_nextOCLFrame[m_curBodyState][i] = now + (int)boneFXUpdateModuleData->m_OCL[m_curBodyState][i].gameLogicDelay.getValue();
		else
			m_nextOCLFrame[m_curBodyState][i] = -1;
		if (boneFXUpdateModuleData->m_particleSystem[m_curBodyState][i].locInfo.boneName.compare(AsciiString::TheEmptyString) != 0)
			m_nextParticleSystemFrame[m_curBodyState][i] = now + (int)boneFXUpdateModuleData->m_particleSystem[m_curBodyState][i].gameClientDelay.getValue();
		else
			m_nextParticleSystemFrame[m_curBodyState][i] = -1;
	}
}

void BoneFXUpdate::resolveBoneLocations()
{
	int i;
	Object *building = getObject();
	const BoneFXUpdateModuleData *boneFXUpdateModuleData = getBoneFXUpdateModuleData();
	if (building == NULL)
		return;
	Drawable *drawable = building->getDrawable();
	if (boneFXUpdateModuleData == NULL)
		return;

	for (i = 0; i < BONE_FX_MAX_BONES; ++i)
	{
		if (boneFXUpdateModuleData->m_fxList[m_curBodyState][i].locInfo.boneName.compare(AsciiString::TheEmptyString) != 0)
		{
			const BoneFXListInfo *info = &(boneFXUpdateModuleData->m_fxList[m_curBodyState][i]);
			drawable->getPristineBonePositions(info->locInfo.boneName.str(), 0, &m_FXBonePositions[m_curBodyState][i], NULL, 1, 0);
		}
		if (boneFXUpdateModuleData->m_OCL[m_curBodyState][i].locInfo.boneName.compare(AsciiString::TheEmptyString) != 0)
		{
			const BoneOCLInfo *info = &(boneFXUpdateModuleData->m_OCL[m_curBodyState][i]);
			drawable->getPristineBonePositions(info->locInfo.boneName.str(), 0, &m_OCLBonePositions[m_curBodyState][i], NULL, 1, 0);
		}
		if (boneFXUpdateModuleData->m_particleSystem[m_curBodyState][i].locInfo.boneName.compare(AsciiString::TheEmptyString) != 0)
		{
			const BoneParticleSystemInfo *info = &(boneFXUpdateModuleData->m_particleSystem[m_curBodyState][i]);
			drawable->getPristineBonePositions(info->locInfo.boneName.str(), 0, &m_PSBonePositions[m_curBodyState][i], NULL, 1, 0);
		}
	}
	m_bonesResolved[m_curBodyState] = true;
}


BoneFXUpdateModuleData::BoneFXUpdateModuleData()
{
	Int i, j;
	for (i = 0; i < BODYDAMAGETYPE_COUNT; ++i)
	{
		for (j = 0; j < BONE_FX_MAX_BONES; ++j)
		{
			m_fxList[i][j].fx = NULL;
			m_fxList[i][j].onlyOnce = true;
			m_OCL[i][j].ocl = NULL;
			m_OCL[i][j].onlyOnce = true;
			m_particleSystem[i][j].particleSysTemplate = NULL;
			m_particleSystem[i][j].onlyOnce = true;
		}
	}
	m_damageFXTypes = 0xFFFFFFFF;
	m_damageOCLTypes = 0xFFFFFFFF;
	m_damageParticleTypes = 0xFFFFFFFF;
}



BoneFXUpdate::BoneFXUpdate(Thing *thing, const ModuleData *moduleData)
	: UpdateModule(thing, moduleData)
{
	int i, j;
	for (i = 0; i < BODYDAMAGETYPE_COUNT; ++i)
	{
		for (j = 0; j < BONE_FX_MAX_BONES; ++j)
		{
			m_nextFXFrame[i][j] = -1;
			m_nextOCLFrame[i][j] = -1;
			m_nextParticleSystemFrame[i][j] = -1;
			m_FXBonePositions[i][j].zero();
			m_OCLBonePositions[i][j].zero();
			m_PSBonePositions[i][j].zero();
		}
		m_bonesResolved[i] = false;
	}
	m_particleSystemIDs.clear();
	m_curBodyState = BODY_PRISTINE;
	m_active = false;
}

NameKeyType BoneFXUpdate::getModuleNameKey() const
{
	static NameKeyType TheBoneFXUpdatePoolKey = TheNameKeyGenerator->nameToKey("BoneFXUpdate");
	return TheBoneFXUpdatePoolKey;
}

const char *BoneFXUpdate::GetSnapshotName() const
{
	return "BoneFXUpdate";
}

void BoneFXUpdate::killRunningParticleSystems()
{
	for (ParticleSystemIDVec::iterator it = m_particleSystemIDs.begin(); it != m_particleSystemIDs.end(); ++it)
	{
		TheParticleSystemManager->destroyParticleSystemByID(*it);
	}
	m_particleSystemIDs.clear();
}

void BoneFXUpdate::stopAllBoneFX()
{
	int i, j;
	for (i = 0; i < BODYDAMAGETYPE_COUNT; ++i)
	{
		for (j = 0; j < BONE_FX_MAX_BONES; ++j)
		{
			m_nextFXFrame[i][j] = -1;
			m_nextOCLFrame[i][j] = -1;
			m_nextParticleSystemFrame[i][j] = -1;
		}
	}
	killRunningParticleSystems();
}

BoneFXUpdate::~BoneFXUpdate()
{
	killRunningParticleSystems();
}


void BoneFXUpdate::changeBodyDamageState(BodyDamageType oldState, BodyDamageType newState)
{
	m_curBodyState = newState;
	killRunningParticleSystems();
	initTimes();
}


void BoneFXUpdate::doParticleSystemAtBone(const ParticleSystemTemplate *particleSystemTemplate, const Coord3D *bonePosition)
{
	if (m_bonesResolved[m_curBodyState] == FALSE) {
		resolveBoneLocations();
	}

	// if we are restricted by the damage type executing effect, bail out of here
	const BoneFXUpdateModuleData *boneFXUpdateModuleData = getBoneFXUpdateModuleData();
	const DamageInfo *lastDamageInfo = getObject()->getBodyModule()->getLastDamageInfo();
	if( lastDamageInfo && getDamageTypeFlag( boneFXUpdateModuleData->m_damageParticleTypes, lastDamageInfo->in.m_damageType ) == FALSE )
		return;

	Object *building = getObject();

	ParticleSystemHandle psys = TheParticleSystemManager->createParticleSystem(particleSystemTemplate, true);
	if (psys)
	{
		ParticleSystemID systemID = psys->m_systemID;
		m_particleSystemIDs.push_back(systemID);
		psys->setPosition(bonePosition);
		psys->attachToObject(building);
		Drawable *drawable = building->getDrawable();
		if (drawable && drawable->isDrawableEffectivelyHidden())
		{
			psys->stop();
		}
	}
}


void BoneFXUpdate::xfer(Xfer *xfer)
{
	UpdateModule::xfer(xfer);
	if (xfer->IsLightCRC())
		return;
	xfer->Version1();

	UnsignedShort particleSystemCount = m_particleSystemIDs.size();
	*xfer == particleSystemCount;
	ParticleSystemID systemID;
	if (xfer->IsStoring())
	{
		ParticleSystemIDVec::const_iterator it;
		for (it = m_particleSystemIDs.begin(); it != m_particleSystemIDs.end(); ++it)
		{
			systemID = *it;
			XferParticleSystemID(xfer, (Int *)&systemID);
		}
	}
	else
	{
		if (m_particleSystemIDs.empty() == false)
			throw XferException(5, 0);
		for (UnsignedShort i = 0; i < particleSystemCount; ++i)
		{
			XferParticleSystemID(xfer, (Int *)&systemID);
			m_particleSystemIDs.push_back(systemID);
		}
	}

	xfer->XferRawBytes(m_nextFXFrame, sizeof(Int) * BODYDAMAGETYPE_COUNT * BONE_FX_MAX_BONES);
	xfer->XferRawBytes(m_nextOCLFrame, sizeof(Int) * BODYDAMAGETYPE_COUNT * BONE_FX_MAX_BONES);
	xfer->XferRawBytes(m_nextParticleSystemFrame, sizeof(Int) * BODYDAMAGETYPE_COUNT * BONE_FX_MAX_BONES);
	xfer->XferRawBytes(m_FXBonePositions, sizeof(Coord3D) * BODYDAMAGETYPE_COUNT * BONE_FX_MAX_BONES);
	xfer->XferRawBytes(m_OCLBonePositions, sizeof(Coord3D) * BODYDAMAGETYPE_COUNT * BONE_FX_MAX_BONES);
	xfer->XferRawBytes(m_PSBonePositions, sizeof(Coord3D) * BODYDAMAGETYPE_COUNT * BONE_FX_MAX_BONES);
	xfer->XferRawBytes(&m_curBodyState, sizeof(BodyDamageType));
	xfer->XferRawBytes(m_bonesResolved, sizeof(Bool) * BODYDAMAGETYPE_COUNT);
	*xfer == m_active;
}



UpdateSleepTime BoneFXUpdate::update()
{
	const BoneFXUpdateModuleData *boneFXUpdateModuleData = getBoneFXUpdateModuleData();
	Int now = TheGameLogic->getFrame();
	if (m_active == FALSE)
	{
		initTimes();
		m_active = TRUE;
	}

	for (Int i = 0; i < BONE_FX_MAX_BONES; ++i)
	{
		if ((m_nextFXFrame[m_curBodyState][i] != -1) && (m_nextFXFrame[m_curBodyState][i] <= now))
		{
			doFXListAtBone(boneFXUpdateModuleData->m_fxList[m_curBodyState][i].fx, &(m_FXBonePositions[m_curBodyState][i]));
			computeNextLogicFXTime(&(boneFXUpdateModuleData->m_fxList[m_curBodyState][i]), m_nextFXFrame[m_curBodyState][i]);
		}
		if ((m_nextOCLFrame[m_curBodyState][i] != -1) && (m_nextOCLFrame[m_curBodyState][i] <= now))
		{
			doOCLAtBone(boneFXUpdateModuleData->m_OCL[m_curBodyState][i].ocl, &(m_OCLBonePositions[m_curBodyState][i]));
			computeNextLogicFXTime(&(boneFXUpdateModuleData->m_OCL[m_curBodyState][i]), m_nextOCLFrame[m_curBodyState][i]);
		}
		if ((m_nextParticleSystemFrame[m_curBodyState][i] != -1) && (m_nextParticleSystemFrame[m_curBodyState][i] <= now))
		{
			doParticleSystemAtBone(boneFXUpdateModuleData->m_particleSystem[m_curBodyState][i].particleSysTemplate, &(m_PSBonePositions[m_curBodyState][i]));
			computeNextClientFXTime(&(boneFXUpdateModuleData->m_particleSystem[m_curBodyState][i]), m_nextParticleSystemFrame[m_curBodyState][i]);
		}
	}
	return UPDATE_SLEEP_NONE;
}







