// cl: /Oy-
#include "Common/INI/FieldParse.h"
#include "Common/INI.h"
#include "Common/INIException.h"
#include "Draft/LivingWorldAutoResolveResourceBonus.h"
// Tier B; object=O00413DCC evidence=Z flags=align=A;ehform=EH_prolog
// Extent is a hypothesis; only Object VERIFIED establishes recovery.
// Edges: left=unknown-rows right=unknown-rows
// The resource bonus INI block: three fields at +0/+4/+8 and a negative
// minimum rejected with INIException 3.
// FUNCTION: BFME2 0x00413DCC size=65 name=?parseBonusIniSubBlock@LivingWorldAutoResolveResourceBonus@@QAEXPAVINI@@@Z evidence=N:ledger

const FieldParse ResourceBonusFieldParse[] =
{
	{ "MinResourceBonus",  &INI::parseInt,  0, 0x0 },
	{ "WeaponMultiplier",  &INI::parseReal, 0, 0x4 },
	{ "ArmorMultiplier",   &INI::parseReal, 0, 0x8 },
	{ 0, 0, 0, 0 }
};

void LivingWorldAutoResolveResourceBonus::parseBonusIniSubBlock(INI *ini)
{
	ini->initFromINI(this, ResourceBonusFieldParse);

	if (minimum < 0)
		throw INIException(3, "Must provide a MinResourceBonus, and it must be >= 0");
}

// FUNCTION: BFME2 0x00413E27 size=46 name=?rva00413E27@Rva00413E27@@QAEPAURva00413E27Bucket@@H@Z evidence=N:ledger

// FUNCTION: BFME2 0x00413FC1 size=54 name=?getBonusForResourceAmount@LivingWorldAutoResolveResourceBonusSchedule@@QAEPAHH@Z evidence=N:ledger

// FUNCTION: BFME2 0x00414009 size=120 name=?Rva00414009Parse@@YAXPAVINI@@PAX1PBX@Z evidence=N:ledger

// FUNCTION: BFME2 0x00414081 size=18 name=?rva00414081@Rva00414081@@QAEXPAVINI@@@Z evidence=N:ledger

// FUNCTION: BFME2 0x00414148 size=30 name=?rva00414148@Rva00414148@@QAEXXZ evidence=N:ledger

// FUNCTION: BFME2 0x0041428F size=120 name=?parseLivingWorldAutoResolveResourceBonusBlock@LivingWorldAutoResolveResourceBonusScheduleStore@@SAXPAVINI@@@Z evidence=N:ledger
