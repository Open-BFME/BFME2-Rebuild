// BoneFXUpdate.h: plays FX, OCLs and particle systems at named bones per body damage state.
// Zero Hour's BoneFXUpdate.h layout, which BFME2 keeps (old repo
// reference/shims/bfme2_bonefxupdate/GameLogic/Module/BoneFXUpdate.h and the
// BoneFXUpdate.cpp unit's layout notes):
//  - module data: damage-type words at +0x08, +0x48C and +0x910 (the constructor
//    0x00487797 stores -1 there), each followed by a [4][8] array of 0x24-byte
//    bone infos at +0x0C, +0x490 and +0x914;
//  - module: UpdateModule (0x20), the particle-system id vector at +0x20, next
//    frames at +0x2C/+0xAC/+0x12C, bone positions at +0x1AC/+0x32C/+0x4AC, the
//    body state at +0x62C, bonesResolved at +0x630, active at +0x634; sizeof
//    0x638 (friend_newModuleInstance).
// vftable 0x00C4B170: loadPostProcess, the name getter and xfer in slots 1-3; no crc.
#pragma once
#ifndef BFME2_BONEFXUPDATE_H
#define BFME2_BONEFXUPDATE_H

#include "GameLogic/Module/UpdateModule.h"
#include "GameLogic/Module/ModuleData.h"
#include "Common/AsciiString.h"
#include "Common/Coord3D.h"
#include "GameLogic/Module/BoneFXBonePosition.h"
#include "Common/RandomValue.h"
#include "Common/Vector.h"

class FXList;
class INI;
class ObjectCreationList;
class ParticleSystemTemplate;

enum { BONE_FX_MAX_BONES = 8 };

struct BoneLocInfo
{
	AsciiString boneName;							// 0x00
};
// SIZE 0x4

struct BaseBoneListInfo
{
	BoneLocInfo locInfo;							// 0x00
	GameClientRandomVariable gameClientDelay;		// 0x04
	GameLogicRandomVariable gameLogicDelay;			// 0x10
	Bool onlyOnce;									// 0x1C
};
// SIZE 0x20

struct BoneFXListInfo : public BaseBoneListInfo
{
	const FXList *fx;								// 0x20
};
// SIZE 0x24

struct BoneOCLInfo : public BaseBoneListInfo
{
	const ObjectCreationList *ocl;					// 0x20
};
// SIZE 0x24

struct BoneParticleSystemInfo : public BaseBoneListInfo
{
	const ParticleSystemTemplate *particleSysTemplate;	// 0x20
};
// SIZE 0x24

class BoneFXUpdateModuleData : public ModuleData
{
public:
	BoneFXUpdateModuleData();
	// No user-declared destructor: retail's (0x00487877) is the implicit one.

	static void parseFXList(INI *ini, void *instance, void *store, const void *userData);
	static void parseObjectCreationList(INI *ini, void *instance, void *store, const void *userData);
	static void parseParticleSystem(INI *ini, void *instance, void *store, const void *userData);

	UnsignedInt m_damageFXTypes;														// 0x008
	BoneFXListInfo m_fxList[BODYDAMAGETYPE_COUNT][BONE_FX_MAX_BONES];					// 0x00C
	UnsignedInt m_damageOCLTypes;														// 0x48C
	BoneOCLInfo m_OCL[BODYDAMAGETYPE_COUNT][BONE_FX_MAX_BONES];							// 0x490
	UnsignedInt m_damageParticleTypes;													// 0x910
	BoneParticleSystemInfo m_particleSystem[BODYDAMAGETYPE_COUNT][BONE_FX_MAX_BONES];	// 0x914
};
// SIZE 0xD94

class BoneFXUpdate : public UpdateModule
{
public:
	BoneFXUpdate(Thing *thing, const ModuleData *moduleData);

	virtual NameKeyType getModuleNameKey() const;
	virtual UpdateSleepTime update();

	void changeBodyDamageState(BodyDamageType oldState, BodyDamageType newState);
	void stopAllBoneFX();

	const BoneFXUpdateModuleData *getBoneFXUpdateModuleData() const
	{
		return (const BoneFXUpdateModuleData *)m_moduleData;
	}

protected:
	virtual ~BoneFXUpdate();
	virtual const char *GetSnapshotName() const;
	virtual void xfer(Xfer *xfer);
	virtual void loadPostProcess();
	virtual void onObjectCreated();
	virtual void resolveBoneLocations();

	void doFXListAtBone(const FXList *fxList, const Coord3D *bonePosition);
	void doOCLAtBone(const ObjectCreationList *ocl, const Coord3D *bonePosition);
	void doParticleSystemAtBone(const ParticleSystemTemplate *particleSystemTemplate, const Coord3D *bonePosition);
	void killRunningParticleSystems();
	void computeNextClientFXTime(const BaseBoneListInfo *info, Int &nextFrame);
	void computeNextLogicFXTime(const BaseBoneListInfo *info, Int &nextFrame);
	void initTimes();

	typedef _STL::vector<ParticleSystemID> ParticleSystemIDVec;

	ParticleSystemIDVec m_particleSystemIDs;										// 0x020
	Int m_nextFXFrame[BODYDAMAGETYPE_COUNT][BONE_FX_MAX_BONES];						// 0x02C
	Int m_nextOCLFrame[BODYDAMAGETYPE_COUNT][BONE_FX_MAX_BONES];					// 0x0AC
	Int m_nextParticleSystemFrame[BODYDAMAGETYPE_COUNT][BONE_FX_MAX_BONES];			// 0x12C
	BoneFXBonePosition m_FXBonePositions[BODYDAMAGETYPE_COUNT][BONE_FX_MAX_BONES];				// 0x1AC
	BoneFXBonePosition m_OCLBonePositions[BODYDAMAGETYPE_COUNT][BONE_FX_MAX_BONES];			// 0x32C
	BoneFXBonePosition m_PSBonePositions[BODYDAMAGETYPE_COUNT][BONE_FX_MAX_BONES];				// 0x4AC
	BodyDamageType m_curBodyState;													// 0x62C
	Bool m_bonesResolved[BODYDAMAGETYPE_COUNT];										// 0x630
	Bool m_active;																	// 0x634
};
// SIZE 0x638

void parseGameClientRandomDelay(INI *ini, void *instance, GameClientRandomVariable *delay);


#endif
