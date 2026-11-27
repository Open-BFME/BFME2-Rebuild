// Generated from the ledger; map snapshot c591f7146f88c8a8248ebb8ae22f0c978c22aed463db64e05ef57ff0dfae99cc
// object=G004C999D name-evidence=invented
// Extents and edges are hypotheses; byte verification alone does not establish original TU boundaries.
#include "Common/INI.h"
#include "Common/INI/FieldParse.h"
#include "GameLogic/Module/EvaAnnounceClientCreateModuleData.h"
#include "GameClient/Eva.h"
// FUNCTION: BFME2 0x004C999D size=38 name=??0EvaAnnounceClientCreateModuleData@@QAE@XZ evidence=N:ledger tier=B hypothesis:mm1-gap-unit
EvaAnnounceClientCreateModuleData::EvaAnnounceClientCreateModuleData()
{
	m_announcementEventEnemy = -1;
	m_announcementEventAlly = -1;
	m_announcementEventOwner = -1;
	m_delayBeforeAnnouncementMS = 0;
	m_onlyIfVisible = false;
	m_countAsFirstSightedAnnoucement = false;
	m_useObjectsPosition = false;
	m_createFakeRadarEvent = false;
}

static const FieldParse s_evaAnnounceClientCreateModuleDataFieldParse[] =
{
	{ "AnnouncementEventEnemy", Eva::parseEvaEventID, 0, 0x8 },
	{ "AnnouncementEventAlly", Eva::parseEvaEventID, 0, 0xC },
	{ "AnnouncementEventOwner", Eva::parseEvaEventID, 0, 0x10 },
	{ "DelayBeforeAnnouncementMS", INI::parseDurationUnsignedInt, 0, 0x14 },
	{ "OnlyIfVisible", INI::parseBool, 0, 0x18 },
	{ "CountAsFirstSightedAnnoucement", INI::parseBool, 0, 0x19 },
	{ "UseObjectsPosition", INI::parseBool, 0, 0x1A },
	{ "CreateFakeRadarEvent", INI::parseBool, 0, 0x1B },
	{ 0, 0, 0, 0 }
};

// FUNCTION: BFME2 0x004C99C3 size=17 name=?buildFieldParse@EvaAnnounceClientCreateModuleData@@SAXAAVMultiIniFieldParse@@@Z evidence=N:ledger tier=B hypothesis:mm1-gap-unit
void EvaAnnounceClientCreateModuleData::buildFieldParse(MultiIniFieldParse &parse)
{
	parse.add(s_evaAnnounceClientCreateModuleDataFieldParse, 0);
}

// FUNCTION: BFME2 0x004C99D4 size=50 name=??0EvaAnnounceClientCreate@@QAE@PAVThing@@PBVModuleData@@@Z evidence=N:ledger tier=B hypothesis:mm1-gap-unit
