// Portions derived from Command & Conquer Generals and Command & Conquer Generals Zero Hour, (c) Electronic Arts Inc., GPL-3.0-or-later; see LICENSE and NOTICE.
// Diagnostic pilot import; object boundaries may be unproven.
// FUNCTION: BFME2 0x00495C48 size=32 name=??1AutoPickUpUpdate@@UAE@XZ evidence=N:ledger tier=B hypothesis:mm1-held-object-same-path
// FUNCTION: BFME2 0x00495C68 size=6 name=?GetSnapshotName@AutoPickUpUpdate@@MBEPBDXZ evidence=N:ledger
// FUNCTION: BFME2 0x00495C6E size=9 name=?rva00495C6E@AutoPickUpUpdate@@UAEXXZ evidence=?:placeholder
// FUNCTION: BFME2 0x00495C77 size=89 name=?xfer@AutoPickUpUpdate@@MAEXPAVXfer@@@Z evidence=N:ledger
// FUNCTION: BFME2 0x00495CEC size=69 name=?getModuleNameKey@AutoPickUpUpdate@@UBE?AW4NameKeyType@@XZ evidence=N:ledger
// FUNCTION: BFME2 0x00495D31 size=121 name=??0AutoPickUpUpdate@@QAE@PAVThing@@PBVModuleData@@@Z evidence=N:ledger
// FUNCTION: BFME2 0x00495DC6 size=7 name=?get@Rva00495DC6PtrChaseField@@QBEHXZ evidence=?:placeholder
// FUNCTION: BFME2 0x00495DCD size=7 name=?get@Rva00495DCDByteChaseField@@QBEEXZ evidence=?:placeholder
#include <new>
#define _CRTIMP
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#undef _CRTIMP
#include <string.h>
#include "GameLogic/PartitionManager.h"
#include "GameLogic/PartitionFilterAlive.h"
#include "GameLogic/PartitionFilterPlayerAffiliation.h"
#include "GameLogic/Rva00261058.h"
#include "GameLogic/Rva00261513Filter.h"
#include "GameLogic/Rva002614ECFilter.h"
#include "GameLogic/Rva003959FA.h"
#include "GameLogic/Module/SpecialPowerModule.h"
#include "GameLogic/Module/ContainModule.h"
#include "GameLogic/Module/BodyModule.h"
#include "GameLogic/TerrainLogic.h"
#include <new>
#include "GameLogic/Module/AutoPickUpEatObjectEntry.h"
#include "GameLogic/Module/AutoPickUpEatObjectEntryVectorBase.h"
#include "Common/INIException.h"
#include "Common/BitFlags.h"
#include "Common/FalseType.h"
#include <stdlib.h>
#include <string.h>
extern KindOfMaskType KINDOFMASK_NONE;
extern int BfmeLogicFramesPerSecond;
void *AllocateRawBytes(unsigned,const void *);
void destroyRange(void *,void *);
AutoPickUpEatObjectEntry *CopyAutoPickUpEntries(AutoPickUpEatObjectEntry *,AutoPickUpEatObjectEntry *,AutoPickUpEatObjectEntry *,const FalseType &);
AutoPickUpEatObjectEntry *FillAutoPickUpEntries(AutoPickUpEatObjectEntry *,unsigned,const AutoPickUpEatObjectEntry &,const FalseType &);
inline const unsigned &PickUpVectorMax(const unsigned &a,const unsigned &b) { return a<b?b:a; }
inline void ConstructAutoPickUpEntry(AutoPickUpEatObjectEntry *p,const AutoPickUpEatObjectEntry &v) { if(p) new(p) AutoPickUpEatObjectEntry(v); }
inline AutoPickUpEatObjectEntry *CopyAutoPickUpEntries(AutoPickUpEatObjectEntry *first,AutoPickUpEatObjectEntry *last,AutoPickUpEatObjectEntry *result,const FalseType &)
{
 AutoPickUpEatObjectEntry *cur=result;
 for(;first!=last;++first,++cur) ConstructAutoPickUpEntry(cur,*first);
 return cur;
}
inline AutoPickUpEatObjectEntry *FillAutoPickUpEntries(AutoPickUpEatObjectEntry *first,unsigned count,const AutoPickUpEatObjectEntry &value,const FalseType &)
{
 AutoPickUpEatObjectEntry *cur=first;
 for(;count>0;--count,++cur) ConstructAutoPickUpEntry(cur,value);
 return cur;
}
inline AutoPickUpEatObjectEntry *AutoPickUpEntryAllocator::allocate(unsigned count,const void *) const
{ return count ? (AutoPickUpEatObjectEntry *)AllocateRawBytes(count*sizeof(AutoPickUpEatObjectEntry),0) : 0; }


// GameLogic/Object/Update/AutoPickUpUpdate.cpp: retail translation unit (pilot skeleton).
// Write each function below its FUNCTION annotation, in this (retail) order.
// Bodies matched in the old repo (read-only reference) are listed per function as old=<source>.
// Shared types belong in include/; no class or struct with a body in this file.
// cl: 
#include "GameLogic/Module/AutoPickUpUpdate.h"
#include "GameLogic/Object.h"
#include "Common/Xfer.h"
#include "Common/NameKeyGenerator.h"
#include "Common/RandomValue.h"
#include "Common/INI.h"
#include <stddef.h>

AutoPickUpUpdate::~AutoPickUpUpdate()
{
}

const char *AutoPickUpUpdate::GetSnapshotName() const
{
	return "AutoPickUpUpdate";
}

void AutoPickUpUpdate::rva00495C6E()
{
	m_unk0x28 = true;
	m_unk0x29 = true;
}

void AutoPickUpUpdate::xfer(Xfer *xfer)
{
	Xfer::Version version(1, 2);
	*xfer == version;
	UpdateModule::xfer(xfer);
	*xfer == m_scanCountdown;
	*xfer == m_unk0x28;
	if (version.m_minimum >= 2)
	{
		*xfer == m_unk0x29;
	}
}

NameKeyType AutoPickUpUpdate::getModuleNameKey() const
{
	static NameKeyType TheAutoPickUpUpdatePoolKey = TheNameKeyGenerator->nameToKey("AutoPickUpUpdate");
	return TheAutoPickUpUpdatePoolKey;
}

AutoPickUpUpdate::AutoPickUpUpdate(Thing *thing, const ModuleData *moduleData)
	: UpdateModule(thing, moduleData)
{
	const AutoPickUpUpdateModuleData *data = getAutoPickUpUpdateModuleData();
	int zero = 0;
	m_scanCountdown = zero;
	m_unk0x28 = zero;
	m_unk0x29 = zero;
	m_scanCountdown += GetGameLogicRandomValue(0, data->m_scanDelayTime, "C:\\projects\\bfme2patch103\\bfme2\\Code\\GameEngine\\Source\\GameLogic\\Object\\Update\\AutoPickUpUpdate.cpp", 0x83);
}



int AutoPickUpUpdate::getRunFromButtonNumber() const { return getAutoPickUpUpdateModuleData()->m_runFromButtonNumber; }


unsigned char AutoPickUpUpdate::isRunFromButton() const { return getAutoPickUpUpdateModuleData()->m_runFromButton; }

Bool AutoPickUpUpdate::rva00495DD4()
{
	const AutoPickUpUpdateModuleData *data = getAutoPickUpUpdateModuleData();
	if (data->m_runFromButton)
		return m_unk0x28;
	if (m_scanCountdown == 0)
	{
		m_scanCountdown = data->m_scanDelayTime;
		return true;
	}
	m_scanCountdown--;
	return false;
}



void DestroyAutoPickUpEntry(AutoPickUpEatObjectEntry *p) { p->~AutoPickUpEatObjectEntry(); }


UpdateSleepTime AutoPickUpUpdate::update()
{
	Object *object = getObject();
	const AutoPickUpUpdateModuleData *data = getAutoPickUpUpdateModuleData();
	if (object->isEffectivelyDead())
		return UPDATE_SLEEP_NONE;

	bool allowed;
	if (!data->m_canScanWhileAttackingOrMoving
		&& (object->testModelConditionState((ModelConditionFlagType)0x25) || object->testModelConditionState((ModelConditionFlagType)0x3D)))
		allowed = false;
	else
		allowed = true;

	if (!rva00495DD4())
		return UPDATE_SLEEP_NONE;
	if (!allowed)
		return UPDATE_SLEEP_NONE;

	m_unk0x28 = false;

	if (data->m_autoThrowObject && object->getContainedBy()
		&& object->getContain()->getCount(false) == 1)
	{
		Coord3D position = *object->getPosition();
		Object *found = ThePartitionManager->getClosestObject(&position,
			object->getVisionRange(), 0,
			PartitionFilterPlayerAffiliation(object->getControllingPlayer(), ALLOW_ENEMIES, true).link(
				PartitionFilterAlive().link(
					Rva00261513Filter(object, true, -1.0f).link(
						&Rva00261058(object->getControllingPlayer(), false)))));
		if (found)
		{
			object->fireCurrentWeapon(found, found->getID());
			return UPDATE_SLEEP_NONE;
		}
	}

	if (object->getContainedBy() && object->getContain()->getCount(false) == 0)
	{
		Coord3D position = *object->getPosition();
		const KindOfMaskType mask(KindOfMaskType::kInit, 0x6C);
		Object *found = ThePartitionManager->getClosestObject(&position,
			getAutoPickUpUpdateModuleData()->m_scanDistance, 0,
			Rva002614ECFilter(&data->m_pickUpFilter, object->getControllingPlayer(), true).link(
				&Rva003959FA(mask)));
		if (found)
		{
			SpecialPowerModuleInterface *power = object->findSpecialPowerModuleInterface((SpecialPowerType)0x27);
			if (power)
			{
				power->doSpecialPowerAtObject(found, 0x2000);
				return UPDATE_SLEEP_NONE;
			}
		}
	}

	if (!object->getContain())
		return UPDATE_SLEEP_FOREVER;

	if (object->getContain()->getCount(false) == 0)
	{
		unsigned int i = 0;
		if (i < data->m_eatObjectEntries.size())
		{
			do
			{
				const AutoPickUpEatObjectEntry *entry = &data->m_eatObjectEntries.m_start[i];
				if (object->getBodyModule()->getHealthRatio() <= entry->m_myHealth)
				{
					Object *found = ThePartitionManager->getClosestObject(object->getPosition(),
						data->m_scanDistance, 0,
						Rva002614ECFilter(entry, object->getControllingPlayer(), true).link(
							&PartitionFilterAlive()));
					if (found && found->getBodyModule()->getHealthRatio() <= entry->m_targetHealth)
					{
						SpecialPowerModuleInterface *power = object->findSpecialPowerModuleInterface((SpecialPowerType)0x27);
						if (power)
						{
							power->doSpecialPowerAtObject(found, 0x4000);
							return UPDATE_SLEEP_NONE;
						}
					}
				}
				++i;
			} while (i < data->m_eatObjectEntries.size());
		}
	}

	if (object->getContain()->getCount(false) != 0)
		return UPDATE_SLEEP_NONE;

	{
		Coord3D position = *object->getPosition();
		if (const_cast<ObjectFilter&>(data->m_pickUpFilter).testKindOf(&KindOfMaskType(KindOfMaskType::kInit, 0x5E),0,0))
		{
			const Coord3D *point = (const Coord3D *)TheTerrainLogic->rva0027F108(&position,
				getAutoPickUpUpdateModuleData()->m_scanDistance, 1, 1);
			SpecialPowerModuleInterface *power = m_object->findSpecialPowerModuleInterface((SpecialPowerType)0x27);
			if (power && point)
			{
				Coord3D target;
				target.x = point->x;
				target.y = point->y;
				target.z = point->z;
				power->doSpecialPowerAtLocation(&target, 0x2000);
				return UPDATE_SLEEP_NONE;
			}
		}
		Object *found = ThePartitionManager->getClosestObject(&position,
			getScanRange(), 0,
			&Rva002614ECFilter(&data->m_pickUpFilter, object->getControllingPlayer(), true));
		if (found)
		{
			SpecialPowerModuleInterface *power = object->findSpecialPowerModuleInterface((SpecialPowerType)0x27);
			if (power)
				power->doSpecialPowerAtObject(found, 0x2000);
		}
	}
	return UPDATE_SLEEP_NONE;
}


void destroyRange(void *first,void *last)
{ AutoPickUpEatObjectEntry *cur=(AutoPickUpEatObjectEntry *)first; AutoPickUpEatObjectEntry *end=(AutoPickUpEatObjectEntry *)last;
 while(cur!=end) { cur->~AutoPickUpEatObjectEntry(); ++cur; } }



AutoPickUpEatObjectEntryVector::~AutoPickUpEatObjectEntryVector() { destroyRange(m_start,m_finish); }


void AutoPickUpEatObjectEntryVector::clearStorage()
{ destroyRange(m_start,m_finish); void *block=m_start; if(block) free(block); }



AutoPickUpUpdateModuleData::AutoPickUpUpdateModuleData()
 : m_scanDelayTime(BfmeLogicFramesPerSecond),m_scanDistance(300.0f)
{
 m_runFromButtonNumber=0;
 m_autoThrowObject=0; m_runFromButton=0; m_canScanWhileAttackingOrMoving=0;
 m_pickUpFilter.initFromStorages(KindOfMaskType(KINDOFMASK_NONE),KindOfMaskType(KINDOFMASK_NONE));
}



void AutoPickUpEatObjectEntryVector::insertOverflow(AutoPickUpEatObjectEntry *position,const AutoPickUpEatObjectEntry &value,const FalseType &,unsigned count,bool atEnd)
{
 const unsigned oldSize=size();
 const unsigned length=oldSize+PickUpVectorMax(oldSize,count);
 AutoPickUpEatObjectEntry *start=((AutoPickUpEntryAllocator *)&m_end)->allocate(length);
 AutoPickUpEatObjectEntry *finish=start;
 finish=CopyAutoPickUpEntries(m_start,position,start,FalseType());
 if(count==1) { ConstructAutoPickUpEntry(finish,value); ++finish; }
 else finish=FillAutoPickUpEntries(finish,count,value,FalseType());
 if(!atEnd) finish=CopyAutoPickUpEntries(position,m_finish,finish,FalseType());
 clearStorage();
 m_start=start; m_finish=finish; m_end=start+length;
}




AutoPickUpUpdateModuleData::~AutoPickUpUpdateModuleData() {}


void AutoPickUpEatObjectEntryVector::push_back(const AutoPickUpEatObjectEntry &value)
{
 if(m_finish!=m_end) { ConstructAutoPickUpEntry(m_finish,value); ++m_finish; }
 else insertOverflow(m_finish,value,FalseType(),1,true);
}



void AutoPickUpUpdateModuleData::iniParseEatObjectEntry(INI *ini, void *instance, void *store, const void *)
{
	AutoPickUpEatObjectEntry entry;

	const char *token = ini->getNextTokenOrNull(ini->getSepsColon());
	if (token == 0 || _strcmpi(token, "MyHealth") != 0)
		throw INIException(3,
			"AutoPickUpUpdateModuleData::iniParseEatObjectEntry -- Expecting 'MyHealth' entry. You specified %s.",
			token);
	if (strcmp(token, "MyHealth") != 0)
		throw INIException(3,
			"AutoPickUpUpdateModuleData::iniParseEatObjectEntry -- Entry for 'MyHealth' is case sensitive. You specified %s.",
			token);
	INI::parsePercentToReal(ini, instance, &entry.m_myHealth, 0);

	token = ini->getNextTokenOrNull(ini->getSepsColon());
	if (token == 0 || _strcmpi(token, "TargetHealth") != 0)
		throw INIException(3,
			"AutoPickUpUpdateModuleData::iniParseEatObjectEntry -- Expecting 'TargetHealth' entry. You specified %s.",
			token);
	if (strcmp(token, "TargetHealth") != 0)
		throw INIException(3,
			"AutoPickUpUpdateModuleData::iniParseEatObjectEntry -- Entry for 'TargetHealth' is case sensitive. You specified %s.",
			token);
	INI::parsePercentToReal(ini, instance, &entry.m_targetHealth, 0);

	token = ini->getNextTokenOrNull(ini->getSepsColon());
	if (token == 0 || _strcmpi(token, "Filter") != 0)
		throw INIException(3,
			"AutoPickUpUpdateModuleData::iniParseEatObjectEntry -- Expecting 'Filter' entry. You specified %s.",
			token);
	if (strcmp(token, "Filter") != 0)
		throw INIException(3,
			"AutoPickUpUpdateModuleData::iniParseEatObjectEntry -- Entry for 'Filter' is case sensitive. You specified %s.",
			token);
	iniParseObjectFilter(ini, instance, &entry.m_filter, 0);

	((AutoPickUpEatObjectEntryVector *)store)->push_back(entry);
}

void AutoPickUpUpdateModuleData::buildFieldParse(MultiIniFieldParse &p)
{
	static const FieldParse dataFieldParse[] =
	{
		{ "ScanDelayTime", INI::parseDurationUnsignedInt, 0, offsetof(AutoPickUpUpdateModuleData, m_scanDelayTime) },
		{ "PickUpFilter", iniParseObjectFilter, 0, offsetof(AutoPickUpUpdateModuleData, m_pickUpFilter) },
		{ "ScanDistance", INI::parseReal, 0, offsetof(AutoPickUpUpdateModuleData, m_scanDistance) },
		{ "EatObjectEntry", AutoPickUpUpdateModuleData::iniParseEatObjectEntry, 0, offsetof(AutoPickUpUpdateModuleData, m_eatObjectEntries) },
		{ "AutoThrowObject", INI::parseBool, 0, offsetof(AutoPickUpUpdateModuleData, m_autoThrowObject) },
		{ "RunFromButton", INI::parseBool, 0, offsetof(AutoPickUpUpdateModuleData, m_runFromButton) },
		{ "RunFromButtonNumber", INI::parseInt, 0, offsetof(AutoPickUpUpdateModuleData, m_runFromButtonNumber) },
		{ "CanScanWhileAttackingOrMoving", INI::parseBool, 0, offsetof(AutoPickUpUpdateModuleData, m_canScanWhileAttackingOrMoving) },
		{ 0, 0, 0, 0 }
	};
	p.add(dataFieldParse);
}










AutoPickUpEatObjectEntry::~AutoPickUpEatObjectEntry() {}

void AutoPickUpUpdate::loadPostProcess()
{
	UpdateModule::loadPostProcess();
}
