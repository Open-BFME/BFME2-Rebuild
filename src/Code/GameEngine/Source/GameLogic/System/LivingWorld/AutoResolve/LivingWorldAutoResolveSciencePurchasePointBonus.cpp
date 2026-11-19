// cl: /Oy-
#include "Common/INI/FieldParse.h"
#include "Common/INI.h"
#include "Common/INIException.h"
#include "Draft/LivingWorldAutoResolveSciencePurchasePointBonus.h"
// Tier B; object=O004135CD evidence=Z flags=align=A;ehform=EH_prolog
// Extent is a hypothesis; only Object VERIFIED establishes recovery.
// Edges: left=unknown-rows right=unknown-rows
// The science-point bonus INI block. The field table at 0x00C39A78 is three
// records and a terminator; a negative minimum is rejected with INIException 3.
// FUNCTION: BFME2 0x004135CD size=65 name=?parseBonusIniSubBlock@LivingWorldAutoResolveSciencePurchasePointBonus@@QAEXPAVINI@@@Z evidence=N:ledger

const FieldParse SciencePurchasePointBonusFieldParse[] =
{
	{ "MinSciencePurchasePointsForBonus", &INI::parseInt,  0, 0x0 },
	{ "WeaponMultiplier",                 &INI::parseReal, 0, 0x4 },
	{ "ArmorMultiplier",                  &INI::parseReal, 0, 0x8 },
	{ 0, 0, 0, 0 }
};

void LivingWorldAutoResolveSciencePurchasePointBonus::parseBonusIniSubBlock(INI *ini)
{
	ini->initFromINI(this, SciencePurchasePointBonusFieldParse);

	if (minimum < 0)
		throw INIException(3, "Must provide a MinSciencePurchasePointsForBonus, and it must be >= 0");
}

// FUNCTION: BFME2 0x004138C7 size=120 name=?Rva004138C7Parse@@YAXPAVINI@@PAX1PBX@Z evidence=N:ledger

// FUNCTION: BFME2 0x0041393F size=18 name=?rva0041393F@Rva0041386B@@QAEXPAVINI@@@Z evidence=N:ledger

// FUNCTION: BFME2 0x00413B4D size=120 name=?parseLivingWorldAutoResolveSciencePurchasePointBonusBlock@LivingWorldAutoResolveSciencePurchasePointBonusScheduleStore@@SAXPAVINI@@@Z evidence=N:ledger
