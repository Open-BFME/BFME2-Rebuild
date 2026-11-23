// cl: /O1
#include "Common/INI/FieldParse.h"
#include "Common/INI.h"
#include "Common/NameKeyGenerator.h"
#include "Common/Xfer.h"
#include "GameLogic/Module/HeightDieUpdate.h"
#include "GameLogic/Module/HeightDieUpdateModuleData.h"
#include "GameClient/ParticleSys.h"
#include "GameLogic/AIPathfind.h"
#include "GameLogic/GameLogic.h"
#include "GameLogic/Object.h"
#include "GameLogic/PartitionFilterAcceptByKindOf.h"
#include "GameLogic/PartitionManager.h"
#include "GameLogic/TerrainLogic.h"

extern KindOfMaskType KINDOFMASK_NONE;
// Tier B; object=O0048FBD3 evidence=Z flags=align=A
// Extent is a hypothesis; only Object VERIFIED establishes recovery.
// Edges: left=unknown-rows right=unknown-rows
// The height-die module and its INI data. The field table is the one retail
// passes to MultiIniFieldParse::add at 0x0048FCA1 (0x00C4CFC8), which names
// every member and its parser.
// FUNCTION: BFME2 0x0048FBBA size=25 name=??1HeightDieUpdate@@UAE@XZ evidence=N:ledger tier=B hypothesis:mm1-held-object-same-path
HeightDieUpdate::~HeightDieUpdate()
{
}

// FUNCTION: BFME2 0x0048FBD3 size=6 name=?GetSnapshotName@HeightDieUpdate@@MBEPBDXZ evidence=N:ledger

const char *HeightDieUpdate::GetSnapshotName(void) const
{
	return "HeightDieUpdate";
}

// FUNCTION: BFME2 0x0048FBD9 size=87 name=?xfer@HeightDieUpdate@@MAEXPAVXfer@@@Z evidence=N:ledger

void HeightDieUpdate::xfer(Xfer *xfer)
{
	UpdateModule::xfer(xfer);

	if (xfer->IsLightCRC())
		return;

	xfer->Version1();

	*xfer == m_triggeredHeightDeath;
	*xfer == m_destroyedAttachedParticles;
	*xfer == m_lastPosition;
	*xfer == m_firstCheckFrame;
}

// FUNCTION: BFME2 0x0048FC30 size=69 name=?getModuleNameKey@HeightDieUpdate@@UBE?AW4NameKeyType@@XZ evidence=N:ledger

NameKeyType HeightDieUpdate::getModuleNameKey(void) const
{
	static NameKeyType key = TheNameKeyGenerator->nameToKey("HeightDieUpdate");
	return key;
}

// FUNCTION: BFME2 0x0048FC75 size=44 name=??0HeightDieUpdateModuleData@@QAE@XZ evidence=N:ledger

HeightDieUpdateModuleData::HeightDieUpdateModuleData()
{
	m_targetHeight = 0.0f;
	m_targetHeightIncludesStructures = false;
	m_onlyWhenMovingDown = false;
	m_destroyAttachedParticlesAtHeight = -1.0f;
	m_snapToGroundOnDeath = false;
	m_initialDelay = 0;
}

// FUNCTION: BFME2 0x0048FCA1 size=17 name=?buildFieldParse@HeightDieUpdateModuleData@@SAXAAVMultiIniFieldParse@@@Z evidence=N:ledger

const FieldParse HeightDieUpdateModuleDataFieldParseTable[] =
{
	{ "TargetHeight", &INI::parseReal, NULL, offsetof(HeightDieUpdateModuleData, m_targetHeight) },
	{ "TargetHeightIncludesStructures", &INI::parseBool, NULL, offsetof(HeightDieUpdateModuleData, m_targetHeightIncludesStructures) },
	{ "OnlyWhenMovingDown", &INI::parseBool, NULL, offsetof(HeightDieUpdateModuleData, m_onlyWhenMovingDown) },
	{ "DestroyAttachedParticlesAtHeight", &INI::parseReal, NULL, offsetof(HeightDieUpdateModuleData, m_destroyAttachedParticlesAtHeight) },
	{ "SnapToGroundOnDeath", &INI::parseBool, NULL, offsetof(HeightDieUpdateModuleData, m_snapToGroundOnDeath) },
	{ "InitialDelay", &INI::parseDurationUnsignedInt, NULL, offsetof(HeightDieUpdateModuleData, m_initialDelay) },
	{ NULL, NULL, NULL, 0 }
};

void HeightDieUpdateModuleData::buildFieldParse(MultiIniFieldParse &p)
{
	p.add(HeightDieUpdateModuleDataFieldParseTable, 0);
}

// FUNCTION: BFME2 0x0048FCB2 size=77 name=??0HeightDieUpdate@@QAE@PAVThing@@PBVModuleData@@@Z evidence=N:ledger

HeightDieUpdate::HeightDieUpdate(Thing *thing, const ModuleData *moduleData) :
	UpdateModule(thing, moduleData)
{
	m_firstCheckFrame = 0xFFFFFFFF;
	m_triggeredHeightDeath = false;
	m_destroyedAttachedParticles = false;
	m_lastPosition.x = -1.0f;
	m_lastPosition.y = -1.0f;
	m_lastPosition.z = -1.0f;
}

// FUNCTION: BFME2 0x0048FD1B size=604 name=?update@HeightDieUpdate@@UAE?AW4UpdateSleepTime@@XZ evidence=N:ledger
UpdateSleepTime HeightDieUpdate::update()
{
	UnsignedInt now = TheGameLogic->getFrame();
	if (m_firstCheckFrame == 0xFFFFFFFF)
		m_firstCheckFrame = now + getHeightDieUpdateModuleData()->m_initialDelay;

	if (m_firstCheckFrame > now)
		return UPDATE_SLEEP_NONE;

	if (getObject()->getContainedBy() != NULL)
	{
		m_lastPosition = *getObject()->getPosition();
		return UPDATE_SLEEP_NONE;
	}

	const HeightDieUpdateModuleData *modData = getHeightDieUpdateModuleData();
	const Coord3D *pos = getObject()->getPosition();

	Bool directionOK = TRUE;
	if (m_triggeredHeightDeath == FALSE)
	{
		if (modData->m_onlyWhenMovingDown)
		{
			if (pos->z >= m_lastPosition.z)
				directionOK = FALSE;
		}

		Real terrainHeightAtPos = TheTerrainLogic->getGroundHeight(pos->x, pos->y);

		if (modData->m_targetHeightIncludesStructures)
		{
			PathfindLayerEnum layer = TheTerrainLogic->getHighestLayerForDestination(pos);
			if (layer != LAYER_GROUND)
			{
				Real layerHeight = TheTerrainLogic->getLayerHeight(pos->x, pos->y, layer);
				if (layerHeight > terrainHeightAtPos)
					terrainHeightAtPos = layerHeight;
			}
		}

		Real targetHeight = terrainHeightAtPos + modData->m_targetHeight;

		if (modData->m_targetHeightIncludesStructures == TRUE)
		{
			Real tallestHeight = 0.0f;
			Real range = getObject()->getMajorRadius();
			PartitionFilterAcceptByKindOf filter(KindOfMaskType(KindOfMaskType::kInit, KINDOF_STRUCTURE), KINDOFMASK_NONE);
			ObjectIterator iter = ThePartitionManager->iterateObjectsInRange(getObject()->getPosition(), range, 3, &filter, 0);
			for (Object *obj = iter.next(); obj; obj = iter.next())
			{
				if (obj == getObject())
					continue;

				Real thisHeight = obj->getGeometryInfo()->getMaxHeightAbovePosition();
				if (thisHeight > tallestHeight)
					tallestHeight = thisHeight;
			}

			if (tallestHeight > modData->m_targetHeight)
				targetHeight = tallestHeight + terrainHeightAtPos;
		}

		if (pos->z < targetHeight && directionOK)
		{
			if (modData->m_snapToGroundOnDeath || pos->z < terrainHeightAtPos)
			{
				Coord3D ground;
				ground.x = pos->x;
				ground.y = pos->y;
				ground.z = terrainHeightAtPos;
				getObject()->setPosition(&ground);
			}

			getObject()->kill(DAMAGE_UNRESISTABLE, DEATH_NORMAL);
			m_triggeredHeightDeath = TRUE;
		}
	}

	if (m_destroyedAttachedParticles == FALSE && pos->z < modData->m_destroyAttachedParticlesAtHeight && (m_triggeredHeightDeath || directionOK))
	{
		TheParticleSystemManager->destroyAttachedSystems(getObject());
		m_destroyedAttachedParticles = TRUE;
	}

	m_lastPosition = *pos;
	return UPDATE_SLEEP_NONE;
}
