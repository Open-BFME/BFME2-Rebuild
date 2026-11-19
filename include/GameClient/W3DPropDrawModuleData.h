// W3DPropDrawModuleData.h: the INI data of the prop draw module. SIZE 0x10.
// Target facts (retail game.dat 1.06):
// - Layout: ModuleData (0x8, Snapshot vptr at +0), then ModelName (AsciiString at +0x8)
//   and DistanceFog (bool at +0xC).
// - Constructor 0x000CEF2B does not call a base constructor: it stores its own vtable,
//   clears ModelName and sets DistanceFog to true.
// - Destructor 0x000CEF3C releases ModelName, then the base chain.
// - buildFieldParse 0x000CEF72 (static) adds the two-field table once.
#pragma once
#ifndef BFME2_W3DPROPDRAWMODULEDATA_H
#define BFME2_W3DPROPDRAWMODULEDATA_H

#include "Common/AsciiString.h"
#include "GameLogic/Module/ModuleData.h"

class MultiIniFieldParse;

class W3DPropDrawModuleData : public ModuleData
{
public:
	W3DPropDrawModuleData();
	virtual ~W3DPropDrawModuleData();
	static void buildFieldParse(MultiIniFieldParse &parse);
	virtual const W3DPropDrawModuleData *getAsW3DPropDrawModuleData() const { return this; }	// slot 19: retail's table holds the folded `mov eax,ecx; ret` 0x0047A6A9
	virtual AsciiString getBestModelNameForWB(const ModelConditionFlags &) const { return ModelName; }	// slot 14: retail's table points at the body shared with W3DTreeDrawModuleData (0x000CED14: a copy of the string at +0x8)

	AsciiString ModelName;														// 0x8
	Bool DistanceFog;															// 0xC
};
// SIZE 0x10
typedef char W3DPropDrawModuleData_size[(sizeof(W3DPropDrawModuleData) == 0x10) ? 1 : -1];

#endif
