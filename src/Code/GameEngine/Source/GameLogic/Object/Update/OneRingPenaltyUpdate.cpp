// Portions derived from Command & Conquer Generals and Command & Conquer Generals Zero Hour, (c) Electronic Arts Inc., GPL-3.0-or-later; see LICENSE and NOTICE.
// Diagnostic pilot import; object boundaries may be unproven.
// FUNCTION: BFME2 0x00499954 size=6 name=?name@Rva00499954Named@@QBEPBDXZ evidence=?:placeholder tier=B hypothesis:mm1-held-object-same-path
// FUNCTION: BFME2 0x0049995A size=82 name=?xfer@OneRingPenaltyUpdate@@MAEXPAVXfer@@@Z evidence=N:ledger tier=B hypothesis:mm1-held-object-same-path
// FUNCTION: BFME2 0x004999AC size=69 name=?getModuleNameKey@OneRingPenaltyUpdate@@UBE?AW4NameKeyType@@XZ evidence=N:ledger tier=B hypothesis:mm1-held-object-same-path
// FUNCTION: BFME2 0x004999F1 size=37 name=??0OneRingPenaltyUpdateModuleData@@QAE@XZ evidence=N:ledger tier=B hypothesis:mm1-held-object-same-path
// FUNCTION: BFME2 0x00499A16 size=73 name=??1OneRingPenaltyUpdateModuleData@@UAE@XZ evidence=N:ledger tier=B hypothesis:mm1-held-object-same-path
// FUNCTION: BFME2 0x00499A5F size=17 name=?buildFieldParse@OneRingPenaltyUpdateModuleData@@SAXAAVMultiIniFieldParse@@@Z evidence=N:ledger tier=B hypothesis:mm1-held-object-same-path
// FUNCTION: BFME2 0x00499AFD size=284 name=?rva00499AFD@OneRingPenaltyUpdate@@AAEXXZ evidence=?:placeholder
// FUNCTION: BFME2 0x00499C35 size=17 name=?rva00499C35@OneRingPenaltyUpdate@@AAEXXZ evidence=?:placeholder
// FUNCTION: BFME2 0x00499C46 size=38 name=?bfmeGo987C@BfmeC987@@QAEXXZ evidence=N:ledger
// FUNCTION: BFME2 0x00499C6C size=172 name=?rva00499C6C@OneRingPenaltyUpdate@@AAEXXZ evidence=?:placeholder
// FUNCTION: BFME2 0x00499D18 size=11 name=?rva00499D18@OneRingPenaltyUpdate@@UAEXPAX@Z evidence=?:placeholder
// FUNCTION: BFME2 0x00499D23 size=373 name=?rva00499D23@OneRingPenaltyUpdate@@AAEXXZ evidence=?:placeholder
#include "GameLogic/Module/OneRingPenaltyUpdate.h"
#include "GameLogic/Module/OneRingPenaltyUpdateModuleData.h"
#include "GameLogic/Module/OneRingMaskPosition.h"
#include "GameLogic/Module/Rva00373EC6.h"
#include "GameLogic/Module/AIUpdate.h"
#include "GameLogic/Object.h"
#include "GameLogic/GameLogic.h"
#include "GameLogic/TerrainLogic.h"
#include "Common/Player.h"
#include "Common/PlayerList.h"
#include "Common/ThingFactory.h"
#include "Common/NameKeyGenerator.h"
#include "Common/INI.h"
#include "Common/Xfer.h"
#include "GameLogic/TerrainGroundCall.h"
#include "Common/GameAudio.h"
#include <string.h>
#include <stddef.h>
// The module data field table (retail VA 0x00C503B0, seven fields and the terminator).
static const FieldParse OneRingPenaltyFields[] =
{
	{ "SpecialObjectName", &INI::parseAsciiString, 0, offsetof(OneRingPenaltyUpdateModuleData, m_specialObjectName) },
	{ "RingTimeBeforeSpawning", &INI::parseDurationUnsignedInt, 0, offsetof(OneRingPenaltyUpdateModuleData, m_ringTimeBeforeSpawning) },
	{ "TimeSpentRoamingAround", &INI::parseDurationUnsignedInt, 0, offsetof(OneRingPenaltyUpdateModuleData, m_timeSpentRoamingAround) },
	{ "TimeRingPowerSuppressed", &INI::parseDurationUnsignedInt, 0, offsetof(OneRingPenaltyUpdateModuleData, m_timeRingPowerSuppressed) },
	{ "StartingDistanceFromMe", &INI::parseReal, 0, offsetof(OneRingPenaltyUpdateModuleData, m_startingDistanceFromMe) },
	{ "TimeFrozenFromPenalty", &INI::parseDurationUnsignedInt, 0, offsetof(OneRingPenaltyUpdateModuleData, m_timeFrozenFromPenalty) },
	{ "DiscoveredSound", &INI::parseAudioEventRTS, 0, offsetof(OneRingPenaltyUpdateModuleData, m_discoveredSound) },
	{ 0, 0, 0, 0 }
};
extern int BfmeLogicFramesPerSecond;
float GetGameLogicRandomValueReal(float,float,char*,int);
float Cos(float);
float Sin(float);
#define PI 3.14159265359f
// GameLogic/Object/Update/OneRingPenaltyUpdate.cpp: retail translation unit (pilot skeleton).
// Write each function below its FUNCTION annotation, in this (retail) order.
// Bodies matched in the old repo (read-only reference) are listed per function as old=<source>.
// Shared types belong in include/; no class or struct with a body in this file.
// cl: 


OneRingPenaltyUpdate::~OneRingPenaltyUpdate() {}


const char *OneRingPenaltyUpdate::GetSnapshotName() const { return "OneRingPenaltyUpdate"; }


void OneRingPenaltyUpdate::xfer(Xfer *xfer)
{
	UpdateModule::xfer(xfer);
	if (xfer->IsLightCRC())
		return;
	xfer->Version1();
	XferObjectID(xfer, &m_24);
	*xfer == m_28;
	*xfer == m_2C;
	*xfer == m_30;
}


NameKeyType OneRingPenaltyUpdate::getModuleNameKey() const
{
	static NameKeyType TheOneRingPenaltyUpdatePoolKey =
		TheNameKeyGenerator->nameToKey("OneRingPenaltyUpdate");
	return TheOneRingPenaltyUpdatePoolKey;
}


OneRingPenaltyUpdateModuleData::OneRingPenaltyUpdateModuleData()
{
	m_timeSpentRoamingAround = 0;
	m_timeRingPowerSuppressed = 0;
	m_startingDistanceFromMe = 0.0f;
	m_ringTimeBeforeSpawning = 0;
	m_timeFrozenFromPenalty = 0;
}


OneRingPenaltyUpdateModuleData::~OneRingPenaltyUpdateModuleData()
{
}


void OneRingPenaltyUpdateModuleData::buildFieldParse(MultiIniFieldParse &parse) { parse.add(OneRingPenaltyFields,0); }


OneRingPenaltyUpdate::OneRingPenaltyUpdate(Thing *thing,const ModuleData *moduleData)
 : UpdateModule(thing,moduleData)
{
 float fzero=0.0f;
 int zero=0;
 m_24=(ObjectID)zero; m_28=zero; m_2C=fzero; m_30=zero;
 setWakeFrame(m_object,UPDATE_SLEEP_NONE);
}



void OneRingPenaltyUpdate::rva00499AFD()
{
	const OneRingPenaltyUpdateModuleData *data = (const OneRingPenaltyUpdateModuleData *)m_moduleData;
	Object *obj = m_object;
	const ThingTemplate *thingTemplate = TheThingFactory->findTemplate(data->m_specialObjectName);
	if (!thingTemplate)
		return;
	else
	{
		Team *team;
		Player *neutral = ThePlayerList->getNeutralPlayer();
		if (neutral)
			team = neutral->getDefaultTeam();
		else
			team = obj->m_team;
		OneRingMaskPosition u;
		memset(&u.mask, 0, sizeof(u.mask));
		Object *newObj = TheThingFactory->newObject(thingTemplate, team, &u.mask, (ObjectID)0);
		if (!newObj)
			return;
		m_24 = newObj->m_id;
		const Coord3D *objPos = obj->getPosition();
		u.overlay.pos.x = objPos->x;
		u.overlay.pos.y = objPos->y;
		u.overlay.pos.z = objPos->z;
		Real angle = GetGameLogicRandomValueReal(-PI, PI, "C:\\projects\\bfme2patch103\\bfme2\\Code\\GameEngine\\Source\\GameLogic\\Object\\Update\\OneRingPenaltyUpdate.cpp", 0xDA);
		u.overlay.pos.x += Cos(angle) * data->m_startingDistanceFromMe;
		u.overlay.pos.y += Sin(angle) * data->m_startingDistanceFromMe;
		u.overlay.pos.z = TheTerrainLogic->getGroundHeight(u.overlay.pos.x, u.overlay.pos.y, 0);
		newObj->setPosition(static_cast<const Coord3D *>(&u.overlay.pos));
		m_2C = angle;
	}
}



void OneRingPenaltyUpdate::rva00499C35()
{
	m_30 = 0;
	m_object->setStatus((ObjectStatusTypes)0x12, true);
}


void OneRingPenaltyUpdate::rva00499C46()
{
 Object *obj=TheGameLogic->findObjectByID(m_24);
 if(obj) { obj->kill((DamageType)8,(DeathType)0);m_24=INVALID_ID; }
}


void OneRingPenaltyUpdate::rva00499C6C()
{
	UnsignedInt frame = TheGameLogic->getFrame();
	const OneRingPenaltyUpdateModuleData *data = reinterpret_cast<const OneRingPenaltyUpdateModuleData *>(getModuleData());
	m_28 = 0;
	Object *obj = m_object;
	m_30 = frame;
	Rva00373EC6 *ring = obj->rva0028F4BC();
	if (ring != 0 && ring->m_31)
	{
		ring->rva00374815();
		obj->setStatus((ObjectStatusTypes)0x12, false);
		obj->rva0028EC68(5, 0, 1);
		const OpaqueRefElement4 *ref = &data->m_discoveredSound;
		if (ref->referent != 0)
		{
			BfmeAudioEventPrefix136 evt(*ref, obj->m_id);
			TheAudio->addAudioEvent(&evt);
		}
	}
	rva00499C46();
}


void OneRingPenaltyUpdate::onDie(const DamageInfo *) { rva00499C46(); }


void OneRingPenaltyUpdate::rva00499D23()
{
	const OneRingPenaltyUpdateModuleData *oneRingPenaltyUpdateModuleData = reinterpret_cast<const OneRingPenaltyUpdateModuleData *>(getModuleData());
	Object *obj = m_object;
	GameLogic *logic = TheGameLogic;
	Object *ringer = logic->findObjectByID(m_24);
	UnsignedInt now = logic->getFrame();
	UnsignedInt start = m_28 + oneRingPenaltyUpdateModuleData->m_ringTimeBeforeSpawning;
	UnsignedInt end = start + oneRingPenaltyUpdateModuleData->m_timeSpentRoamingAround;

	if (now >= end)
	{
		ringer->setPosition(obj->getPosition());
		rva00499C6C();
		return;
	}

	if (ringer && ringer->getAI() && ringer->getAI()->isIdle())
	{
		Coord3D pos;
		pos.x = obj->getPosition()->x;
		pos.y = obj->getPosition()->y;
		m_2C += GetGameLogicRandomValueReal(-1.5707964f, 1.5707964f,
			"C:\\projects\\bfme2patch103\\bfme2\\Code\\GameEngine\\Source\\GameLogic\\Object\\Update\\OneRingPenaltyUpdate.cpp",
			184);
		Real radius;
		if (now - start == 0)
		{
			radius = oneRingPenaltyUpdateModuleData->m_startingDistanceFromMe;
		}
		else
		{
			Real startRadius = oneRingPenaltyUpdateModuleData->m_startingDistanceFromMe;
			Real ratio = (Real)(now - start) / (Real)(end - start);
			radius = startRadius + ratio * (oneRingPenaltyUpdateModuleData->m_startingDistanceFromMe / 2.0f - startRadius);
		}
		pos.x += Cos(m_2C) * radius;
		Real sine = Sin(m_2C);
		TerrainLogic *terrain = TheTerrainLogic;
        const TerrainGroundMethods &ground=**reinterpret_cast<TerrainGroundMethods **>(terrain);

		pos.y += sine * radius;
		pos.z = (terrain->*ground[6])(pos.x,pos.y,0);
		ringer->getAI()->aiMoveToPosition(&pos, (CommandSourceType)2);
	}
}


UpdateSleepTime OneRingPenaltyUpdate::update()
{
	const OneRingPenaltyUpdateModuleData *oneRingPenaltyUpdateModuleData = reinterpret_cast<const OneRingPenaltyUpdateModuleData *>(getModuleData());
	Object *obj = m_object;
	Bool wearing = false;
	if (obj->rva0028F518() || obj->testStatus((ObjectStatusTypes)0xF))
	{
		Rva00373EC6 *ring = obj->rva0028F4BC();
		if (ring && ring->m_31)
			wearing = true;
	}
	UnsignedInt now = TheGameLogic->getFrame();
	if (m_30 != 0 && now > oneRingPenaltyUpdateModuleData->m_timeRingPowerSuppressed + m_30)
		rva00499C35();
	else if (m_28 == 0)
	{
		if (wearing)
			m_28 = now;
	}
	else if (!wearing)
	{
		m_28 = 0;
		if (m_24)
			rva00499C46();
	}
	else if (m_24)
		rva00499D23();
	else if (now > oneRingPenaltyUpdateModuleData->m_ringTimeBeforeSpawning + m_28)
		rva00499AFD();
	return (UpdateSleepTime)BfmeLogicFramesPerSecond;
}





// FUNCTION: BFME2 0x00499E98 size=173 name=?update@OneRingPenaltyUpdate@@UAE?AW4UpdateSleepTime@@XZ evidence=N:ledger tier=B hypothesis:mm1-held-object-same-path
