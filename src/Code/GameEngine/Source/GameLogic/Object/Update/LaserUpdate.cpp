// Portions derived from Command & Conquer Generals and Command & Conquer Generals Zero Hour, (c) Electronic Arts Inc., GPL-3.0-or-later; see LICENSE and NOTICE.
// Portions derived from Command & Conquer Generals Zero Hour, (c) Electronic Arts Inc., GPL-3.0-or-later; see LICENSE and NOTICE.
// Diagnostic pilot import; object boundaries may be unproven.
// FUNCTION: BFME2 0x00362EEE size=112 name=??0LaserUpdate@@QAE@PAVThing@@PBVModuleData@@@Z evidence=N:ledger
// FUNCTION: BFME2 0x00362F5E size=6 name=?Rva00362F5EGet@@YAHXZ evidence=?:placeholder
// FUNCTION: BFME2 0x00362F64 size=95 name=??1LaserUpdate@@UAE@XZ evidence=N:ledger
// FUNCTION: BFME2 0x00362FC3 size=53 name=?rva00362FC3@Rva00362FC3@@QAEXI@Z evidence=?:placeholder
// FUNCTION: BFME2 0x00362FF8 size=238 name=?xfer@LaserUpdate@@MAEXPAVXfer@@@Z evidence=N:ledger
// FUNCTION: BFME2 0x00363102 size=69 name=?rva000363102@LaserUpdate@@SA?AW4NameKeyType@@XZ evidence=?:placeholder
// FUNCTION: BFME2 0x00363147 size=31 name=??0LaserUpdateModuleData@@QAE@XZ evidence=N:ledger
// FUNCTION: BFME2 0x00363166 size=17 name=?buildFieldParse@LaserUpdateModuleData@@SAXAAVMultiIniFieldParse@@@Z evidence=N:ledger
// FUNCTION: BFME2 0x00363193 size=57 name=?getCurrentLaserRadius@LaserUpdate@@QBEMXZ evidence=N:ledger
// FUNCTION: BFME2 0x00363233 size=809 name=?clientUpdate@LaserUpdate@@UAEXXZ evidence=N:ledger
// FUNCTION: BFME2 0x0036355C size=759 name=?initLaser@LaserUpdate@@QAEXPBVObject@@PBUCoord3D@@1H@Z evidence=N:ledger
// FUNCTION: BFME2 0x00363853 size=76 name=?initFromDrawables@LaserUpdate@@QAEXPAXPAVDrawable@@10@Z evidence=N:ledger
// GameLogic/Object/Update/LaserUpdate.cpp: retail translation unit.
// Every body below is written against the shared headers; see LaserUpdate.h and
// LaserUpdateModuleData.h for the layouts and their target evidence.

#include "Common/GameType.h"
#include "Common/Coord3D.h"
#include "Common/AsciiString.h"
#include "Common/Geometry.h"
#include "Common/INI.h"
#include "Common/NameKeyGenerator.h"
#include "Common/Rva00362EC7.h"
#include "Common/Xfer.h"
#include "GameClient/DrawModule.h"
#include "GameClient/Drawable.h"
#include "GameClient/GameClient.h"
#include "GameClient/ParticleSys.h"
#include "GameLogic/Module/LaserUpdate.h"
#include "GameLogic/Module/LaserUpdateModuleData.h"
#include "GameLogic/Object.h"
#include "WWMath/Matrix3D.h"


// Retail 0x00362F5E (6 bytes): LaserUpdate's table 0x008170E0 carries it in
// slot 2, so it is GetSnapshotName (the class-name literal at 0x00BCB950).
const char *LaserUpdate::GetSnapshotName() const
{
	return "LaserUpdate";
}

// Retail table 0x00C17208: five INI fields in this order and the zero sentinel.
static const FieldParse LaserUpdateModuleDataFieldParse[] =
{
	{ "MuzzleParticleSystem", &INI::parseAsciiString, 0, offsetof(LaserUpdateModuleData, m_muzzleParticleSystemName) },
	{ "TargetParticleSystem", &INI::parseAsciiString, 0, offsetof(LaserUpdateModuleData, m_targetParticleSystemName) },
	{ "ParentFireBoneName", &INI::parseAsciiString, 0, offsetof(LaserUpdateModuleData, m_parentFireBoneName) },
	{ "ParentFireBoneOnTurret", &INI::parseBool, 0, offsetof(LaserUpdateModuleData, m_parentFireBoneOnTurret) },
	{ "LaserLifetime", &INI::parseReal, 0, offsetof(LaserUpdateModuleData, m_lifetimeMsec) },
	{ 0, 0, 0, 0 }
};

Rva00362EC7::~Rva00362EC7()
{
}

LaserUpdate::LaserUpdate(Thing *thing, const ModuleData *moduleData)
	: Rva00362EC7(thing, moduleData)
{
	m_dirty = false;
	m_endPos.zero();
	m_startPos.zero();
	m_particleSystemID = INVALID_PARTICLE_SYSTEM_ID;
	m_targetParticleSystemID = INVALID_PARTICLE_SYSTEM_ID;
	m_widening = false;
	m_widenStartFrame = 0;
	m_widenFinishFrame = 0;
	m_currentWidthScalar = 1.0f;
	m_decaying = false;
	m_decayStartFrame = 0;
	m_decayFinishFrame = 0;
	m_expiryFrame = 0;
	m_parentID = INVALID_DRAWABLE_ID;
	m_targetID = INVALID_DRAWABLE_ID;
}


LaserUpdate::~LaserUpdate()
{
	if (m_particleSystemID != INVALID_PARTICLE_SYSTEM_ID)
		TheParticleSystemManager->destroyParticleSystemByID(m_particleSystemID);
	if (m_targetParticleSystemID != INVALID_PARTICLE_SYSTEM_ID)
		TheParticleSystemManager->destroyParticleSystemByID(m_targetParticleSystemID);
}

void LaserUpdate::decayLaser(UnsignedInt frames)
{
	if (frames > 0)
	{
		m_decaying = true;
		UnsignedInt now = TheGameClient->getFrame();
		m_decayStartFrame = now;
		m_decayFinishFrame = now + frames;
		m_currentWidthScalar = 1.0f;
	}
}

void LaserUpdate::xfer(Xfer *xfer)
{
	Rva00362EC7::xfer(xfer);
	if (xfer->IsLightCRC())
		return;
	Xfer::Version version(1, 2);
	*xfer == version;
	*xfer == *(Coord3DBase *)&m_startPos;
	*xfer == *(Coord3DBase *)&m_endPos;
	*xfer == m_dirty;
	XferParticleSystemID(xfer, (int *)&m_particleSystemID);
	XferParticleSystemID(xfer, (int *)&m_targetParticleSystemID);
	*xfer == m_widening;
	*xfer == m_decaying;
	*xfer == m_widenStartFrame;
	*xfer == m_widenFinishFrame;
	*xfer == m_currentWidthScalar;
	*xfer == m_decayStartFrame;
	*xfer == m_decayFinishFrame;
	XferDrawableID(xfer, (int *)&m_parentID);
	XferDrawableID(xfer, (int *)&m_targetID);
	if (version.m_minimum >= 2)
		*xfer == m_expiryFrame;
}


// Slot 4 of the table 0x008170E0.
NameKeyType LaserUpdate::getModuleNameKey() const
{
	static NameKeyType TheLaserUpdatePoolKey =
		TheNameKeyGenerator->nameToKey("LaserUpdate");
	return TheLaserUpdatePoolKey;
}

LaserUpdateModuleData::LaserUpdateModuleData()
{
	m_parentFireBoneOnTurret = false;
	m_lifetimeMsec = 0.0f;
}

void LaserUpdateModuleData::buildFieldParse(MultiIniFieldParse &parse)
{
	parse.add(LaserUpdateModuleDataFieldParse, 0);
}


Real LaserUpdate::getCurrentLaserRadius() const
{
	// A const member: the const getter (DrawModule slot 47) answers.
	const DrawModule *const *drawModule = m_drawable->getDrawModules();
	for (; *drawModule; ++drawModule)
	{
		const LaserDrawInterface *laserInterface = (*drawModule)->getLaserDrawInterface();
		if (laserInterface)
			return laserInterface->getLaserTemplateWidth() * m_currentWidthScalar;
	}
	return 0.0f;
}




void LaserUpdate::clientUpdate()
{
	const LaserUpdateModuleData *data = getLaserUpdateModuleData();
	UnsignedInt now = TheGameClient->getFrame();
	if (now > m_expiryFrame)
		goto expired;
	if (m_decaying)
	{
		m_currentWidthScalar = 1.0f - (Real)(now - m_decayStartFrame) / (Real)(m_decayFinishFrame - m_decayStartFrame);
		m_dirty = true;
		if (m_currentWidthScalar <= 0.0f)
		{
			m_currentWidthScalar = 0.0f;
expired:
			TheGameClient->destroyDrawable(m_drawable);
			return;
		}
	}
	else if (m_widening)
	{
		m_currentWidthScalar = (Real)(now - m_widenStartFrame) / (Real)(m_widenFinishFrame - m_widenStartFrame);
		m_dirty = true;
		if (m_currentWidthScalar >= 1.0f)
		{
			m_currentWidthScalar = 1.0f;
			m_widening = false;
		}
	}
	m_dirty = true;
	if (m_parentID && m_targetID)
	{
		Drawable *parent = TheGameClient->findDrawableByID(m_parentID);
		Drawable *target = TheGameClient->findDrawableByID(m_targetID);
		if (parent && target)
		{
			if (!data->m_parentFireBoneName.isEmpty())
			{
				Matrix3D matrix;
				matrix.Make_Identity();
				parent->getCurrentWorldspaceClientBonePositions(data->m_parentFireBoneName.str(), matrix);
				m_startPos.set(matrix.Get_X_Translation(), matrix.Get_Y_Translation(), matrix.Get_Z_Translation());
			}
			else
			{
				m_startPos.set(parent->getPosition());
				m_startPos.z += parent->getObject()->getGeometryInfo()->getMaxHeightAbovePosition() * 0.5f;
			}
			m_endPos.set(target->getPosition());
			m_endPos.z += target->getObject()->getGeometryInfo()->getMaxHeightAbovePosition() * 0.83f;
			ParticleSystemHandle system;
			if (m_particleSystemID)
			{
				system = TheParticleSystemManager->findParticleSystemByID(m_particleSystemID);
				if (system.m_system)
					system.m_system->setPosition(&m_startPos);
			}
			if (m_targetParticleSystemID)
			{
				system = TheParticleSystemManager->findParticleSystemByID(m_targetParticleSystemID);
				if (system.m_system)
					system.m_system->setPosition(&m_endPos);
			}
			Coord3D pos;
			pos.set(&m_startPos);
			pos.add(&m_endPos);
			pos.x *= 0.5f;
			pos.y *= 0.5f;
			pos.z *= 0.5f;
			getDrawable()->setPosition(&pos);
		}
	}
}

void LaserUpdate::initLaser(const Object *parent, const Coord3D *startPos, const Coord3D *endPos, Int sizeDeltaFrames)
{
	const LaserUpdateModuleData *data = getLaserUpdateModuleData();
	ParticleSystemHandle system;
	if (sizeDeltaFrames > 0)
	{
		m_widening = true;
		m_widenStartFrame = TheGameClient->getFrame();
		m_widenFinishFrame = m_widenStartFrame + sizeDeltaFrames;
		m_currentWidthScalar = 0.0f;
	}
	else if (sizeDeltaFrames < 0)
	{
		m_decaying = true;
		m_decayStartFrame = TheGameClient->getFrame();
		m_decayFinishFrame = m_decayStartFrame - sizeDeltaFrames;
		m_currentWidthScalar = 1.0f;
	}

	m_expiryFrame = (UnsignedInt)((Real)TheGameClient->getFrame() + BfmeClientFramesPerMillisecond * data->m_lifetimeMsec);

	if (parent && !data->m_parentFireBoneName.isEmpty())
	{
		if (data->m_parentFireBoneOnTurret)
		{
			if (!parent->getSingleLogicalBonePositionOnTurret(TURRET_MAIN, data->m_parentFireBoneName.str(), &m_startPos, 0))
			{
				TheGameClient->destroyDrawable(getDrawable());
				return;
			}
		}
		else if (!parent->getSingleLogicalBonePosition(data->m_parentFireBoneName.str(), &m_startPos, 0))
		{
			TheGameClient->destroyDrawable(getDrawable());
			return;
		}
	}
	else if (startPos)
	{
		m_startPos = *startPos;
	}
	else
	{
		TheGameClient->destroyDrawable(getDrawable());
		return;
	}

	if (endPos)
	{
		m_endPos = *endPos;
	}
	else
	{
		TheGameClient->destroyDrawable(getDrawable());
		return;
	}

	if (!m_particleSystemID)
	{
		if (!data->m_muzzleParticleSystemName.isEmpty())
		{
			const ParticleSystemTemplate *particleSystemTemplate = TheParticleSystemManager->findTemplate(data->m_muzzleParticleSystemName);
			if (particleSystemTemplate)
			{
				system = TheParticleSystemManager->createParticleSystem(particleSystemTemplate, true);
				if (system.m_system)
					m_particleSystemID = system.m_system->m_systemID;
			}
		}

		if (!data->m_targetParticleSystemName.isEmpty())
		{
			const ParticleSystemTemplate *particleSystemTemplate = TheParticleSystemManager->findTemplate(data->m_targetParticleSystemName);
			if (particleSystemTemplate)
			{
				system = TheParticleSystemManager->createParticleSystem(particleSystemTemplate, true);
				if (system.m_system)
					m_targetParticleSystemID = system.m_system->m_systemID;
			}
		}
	}

	if (m_particleSystemID)
	{
		system = TheParticleSystemManager->findParticleSystemByID(m_particleSystemID);
		if (system.m_system)
			system.m_system->setPosition(&m_startPos);
	}

	if (m_targetParticleSystemID)
	{
		system = TheParticleSystemManager->findParticleSystemByID(m_targetParticleSystemID);
		if (system.m_system)
			system.m_system->setPosition(&m_endPos);
	}

	Coord3D posToUse;
	posToUse.set(startPos);
	posToUse.add(endPos);
	posToUse.x *= 0.5f;
	posToUse.y *= 0.5f;
	posToUse.z *= 0.5f;
	getDrawable()->setPosition(&posToUse);
	m_dirty = true;
}

void LaserUpdate::initFromDrawables(void *primary, Drawable *parent, Drawable *target, void *d)
{
	if (parent)
	{
		if (target)
		{
			m_parentID = parent->getID();
			m_targetID = target->getID();
			initLaser((const Object *)primary, parent->getPosition(), target->getPosition(), (Int)d);
		}
	}
}



