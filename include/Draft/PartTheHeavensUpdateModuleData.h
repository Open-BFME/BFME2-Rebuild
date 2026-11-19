// DRAFT (h05 fpgen): generated from retail's field-parse table (VA 0x00C54DD8).
// Offsets and size are compile-checked below; no function matches against it yet,
// so it stays in Draft until one does. Unresolved members: m_color, m_radius, m_opacity, m_angle.
// PartTheHeavensUpdateModuleData.h: the INI data of PartTheHeavensUpdate. SIZE 0x94.
// Target facts: ModuleFactory registers the module with operator new 0x94 and
// constructor RVA 0x004ACBF8; buildFieldParse RVA 0x004AC987 adds VA 0x00C54DD8 at +0x0.
// Offsets are the table entries'; each type is what the entry's parse function stores
// (h05 fpgen: INI::parseReal -> Real, parseDurationUnsignedInt -> UnsignedInt, ...);
// names are the INI tokens. Byte gaps are bytes no table entry writes.
// Unresolved (byte storage up to the next member, named after the token):
//   m_color +0xC: INI::parseColorInt 0x0002F3FE
//   m_radius +0x10: Rva00339D74ParseCurve 0x00339D74
//   m_opacity +0x3C: Rva00339D74ParseCurve 0x00339D74
//   m_angle +0x68: Rva00339D74ParseCurve 0x00339D74
#pragma once
#ifndef BFME2_PART_THE_HEAVENS_UPDATE_MODULE_DATA_H
#define BFME2_PART_THE_HEAVENS_UPDATE_MODULE_DATA_H

#include <stddef.h>
#include "Common/GameType.h"
#include "GameLogic/Module/ModuleData.h"
#include "Common/AsciiString.h"

class INI;
class MultiIniFieldParse;

class PartTheHeavensUpdateModuleData : public ModuleData
{
public:
	PartTheHeavensUpdateModuleData();
	static void buildFieldParse(MultiIniFieldParse &parse);

	AsciiString m_texture;                                 // +0x8 Texture
	UnsignedByte m_color[0x4];                             // +0xC Color; unresolved: INI::parseColorInt
	UnsignedByte m_radius[0x2C];                           // +0x10 Radius; unresolved: Rva00339D74ParseCurve
	UnsignedByte m_opacity[0x2C];                          // +0x3C Opacity; unresolved: Rva00339D74ParseCurve
	UnsignedByte m_angle[0x2C];                            // +0x68 Angle; unresolved: Rva00339D74ParseCurve
};
// SIZE 0x94
typedef char PartTheHeavensUpdateModuleData_size[(sizeof(PartTheHeavensUpdateModuleData) == 0x94) ? 1 : -1];
typedef char PartTheHeavensUpdateModuleData_off_m_texture[(offsetof(PartTheHeavensUpdateModuleData, m_texture) == 0x8) ? 1 : -1];
typedef char PartTheHeavensUpdateModuleData_off_m_color[(offsetof(PartTheHeavensUpdateModuleData, m_color) == 0xC) ? 1 : -1];
typedef char PartTheHeavensUpdateModuleData_off_m_radius[(offsetof(PartTheHeavensUpdateModuleData, m_radius) == 0x10) ? 1 : -1];
typedef char PartTheHeavensUpdateModuleData_off_m_opacity[(offsetof(PartTheHeavensUpdateModuleData, m_opacity) == 0x3C) ? 1 : -1];
typedef char PartTheHeavensUpdateModuleData_off_m_angle[(offsetof(PartTheHeavensUpdateModuleData, m_angle) == 0x68) ? 1 : -1];

#endif
