// Generated from the ledger; map snapshot c591f7146f88c8a8248ebb8ae22f0c978c22aed463db64e05ef57ff0dfae99cc
// object=G004C1F71 name-evidence=invented
// Extents and edges are hypotheses; byte verification alone does not establish original TU boundaries.
#include "Common/INI.h"
#include "GameLogic/Module/OathbreakerBody.h"
#include "GameLogic/GameLogic.h"
#include "GameLogic/Object.h"
#include "Common/ThingTemplate.h"
#include "Common/KindOf.h"
#include "GameLogic/Damage.h"
#include "GameLogic/Module/PorcupineFormationBodyModuleData.h"

// FUNCTION: BFME2 0x004C1F71 size=90 name=?attemptDamage@OathbreakerBody@@UAEXPAVDamageInfo@@@Z evidence=N:ledger tier=B hypothesis:mm1-gap-unit
void OathbreakerBody::attemptDamage(DamageInfo *damageInfo)
{
	Object *source = TheGameLogic->findObjectByID(damageInfo->in.m_sourceID);
	if (!source)
		return;
	Bool fromHero;
	if (source->getTemplate()->isKindOf(KINDOF_PROJECTILE))
	{
		Object *launcher = TheGameLogic->findObjectByID(source->getProducerID());
		if (!launcher)
			return;
		fromHero = launcher->getTemplate()->isKindOf(KINDOF_HERO);
	}
	else
		fromHero = source->getTemplate()->isKindOf(KINDOF_HERO);
	if (fromHero)
		ActiveBody::attemptDamage(damageInfo);
}

static const FieldParse s_formationBodyFields[] =
{
	{ "DamageWeaponTemplate", INI::parseWeaponTemplate, 0, offsetof(PorcupineFormationBodyModuleData, m_damageWeaponTemplate) },
	{ "CrushDamageWeaponTemplate", INI::parseWeaponTemplate, 0, offsetof(PorcupineFormationBodyModuleData, m_crushDamageWeaponTemplate) },
	{ "CrusherLevelResisted", INI::parseByte, 0, offsetof(PorcupineFormationBodyModuleData, m_crusherLevelResisted) },
	{ 0, 0, 0, 0 }
};

// FUNCTION: BFME2 0x004C1FCB size=27 name=?buildFieldParse@PorcupineFormationBodyModuleData@@SAXAAVMultiIniFieldParse@@@Z evidence=N:ledger tier=B hypothesis:mm1-gap-unit
void PorcupineFormationBodyModuleData::buildFieldParse(MultiIniFieldParse &parse)
{
	ActiveBodyModuleData::buildFieldParse(parse);
	parse.add(s_formationBodyFields, 0);
}
