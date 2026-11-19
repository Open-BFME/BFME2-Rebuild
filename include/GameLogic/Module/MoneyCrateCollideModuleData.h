// MoneyCrateCollideModuleData.h: the INI data of MoneyCrateCollide. SIZE 0x60.
// Target facts: ModuleFactory registers the module with operator new 0x60 and
// constructor RVA 0x002562FA; buildFieldParse RVA 0x00251168 calls CrateCollideModuleData::buildFieldParse, then adds VA 0x00BEFABC at +0x0.
// Offsets are the table entries'; each type is what the entry's parse function stores
// (h05 fpgen: INI::parseReal -> Real, parseDurationUnsignedInt -> UnsignedInt, ...);
// names are the INI tokens. Byte gaps are bytes no table entry writes.
#pragma once
#ifndef BFME2_MONEY_CRATE_COLLIDE_MODULE_DATA_H
#define BFME2_MONEY_CRATE_COLLIDE_MODULE_DATA_H

#include <stddef.h>
#include "Common/GameType.h"
#include "GameLogic/Module/CrateCollideModuleData.h"

class INI;
class MultiIniFieldParse;

// Base CrateCollideModuleData: the constructor calls its constructor on this. It ends at 0x5C (its own operator-new size);
// this class's members follow it.
typedef char MoneyCrateCollideModuleData_base_size[(sizeof(CrateCollideModuleData) == 0x5C) ? 1 : -1];

class MoneyCrateCollideModuleData : public CrateCollideModuleData
{
public:
	MoneyCrateCollideModuleData();
	static void buildFieldParse(MultiIniFieldParse &parse);
	static ModuleData *friend_newModuleData(INI *ini);	// 0x00256310

	UnsignedInt m_moneyProvided;                           // +0x5C MoneyProvided
};
// SIZE 0x60
typedef char MoneyCrateCollideModuleData_size[(sizeof(MoneyCrateCollideModuleData) == 0x60) ? 1 : -1];
typedef char MoneyCrateCollideModuleData_off_m_moneyProvided[(offsetof(MoneyCrateCollideModuleData, m_moneyProvided) == 0x5C) ? 1 : -1];

#endif
