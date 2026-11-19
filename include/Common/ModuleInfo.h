// ModuleInfo: one of a ThingTemplate's module lists (Behavior, Draw, ClientUpdate, ...):
// a vector of 0x14-byte Nuggets, one per module block the template's INI added.
// Retail evidence: getNthData 0x0033ACE8 divides the vector span by 0x14 and returns
// the nugget's +8; setCopiedFromDefault 0x0033B18A writes the byte at +0x10 of each;
// getNuggetWithTag 0x0033C21C and clearModuleDataWithTag 0x0033C807 compare the tag at
// +4; clearModuleDataWithTag copies +0 (the module name) out; clearAiModuleInfo
// 0x0033C882 asks the ModuleData at +8 isAiModuleData (slot 4); 0x0033C84A tests the
// interface mask at +0xC and the bytes at +0x10/+0x11. The nugget's destructor is the
// implicit one (0x002CF51B, two AsciiStrings) and so is its assignment (0x002CEFCD copies
// through +0x11, so +0x12/+0x13 are padding); it is not trivial
// (vector::erase 0x0033C3BC copies through __copy_ptrs with __false_type).
// ThingTemplate keeps four of these (+0x2E4, +0x2F0, +0x2FC, +0x308).
#pragma once
#ifndef BFME2_MODULEINFO_H
#define BFME2_MODULEINFO_H

#include "Common/GameType.h"
#include "Common/AsciiString.h"
#include "Common/Vector.h"

class ModuleData;

class ModuleInfo
{
public:
	struct Nugget
	{
		AsciiString first;					// +0x00 the module's name
		AsciiString m_moduleTag;			// +0x04
		const ModuleData *second;			// +0x08
		Int interfaceMask;					// +0x0C
		Bool copiedFromDefault;				// +0x10
		Bool inheritable;					// +0x11
	};

	Int getCount() const { return m_info.size(); }			// Zero Hour's
	const ModuleData *getNthData(Int i) const;				// 0x0033ACE8
	void setCopiedFromDefault(Bool copied);					// 0x0033B18A
	const Nugget *getNuggetWithTag(const AsciiString &tag) const;	// 0x0033C21C
	Bool clearModuleDataWithTag(const AsciiString &tagToClear, AsciiString &clearedModuleNameOut);	// 0x0033C807
	Bool clearAiModuleInfo();								// 0x0033C882

private:
	_STL::vector<Nugget> m_info;
};
// SIZE 0xC

#endif
