// ModuleFactory.h: the module registry (name+type -> create procs).
// Target facts: SubsystemInterface at +0 (0xC) and Snapshot at +0xC (0x4) put
// the template map at +0x10 and the created-data list at +0x1C; the ctor
// 0x00256E19 builds both, addModuleInternal 0x002573EE writes a record's
// +0/+4/+8 callbacks and its mask at +0x0C, and mask getter 0x00256F5C reads
// +0x0C. Method signatures and access come from the matched ledger rows
// 0x00256783 (newModule), 0x002571F9 (newModuleDataFromINI), 0x00256F39
// (findModuleInterfaceMask), 0x002567B9 (GetAssetList), 0x0025674F
// (findModuleTemplate) and 0x00255C5E (makeDecoratedNameKey).
#pragma once
#ifndef BFME2_MODULEFACTORY_H
#define BFME2_MODULEFACTORY_H

#include <stddef.h>
#include <map>
#include <vector>
#include "Common/GameType.h"
#include "Common/SubsystemInterface.h"
#include "Common/Snapshot.h"

class INI;
class Module;
class ModuleData;
class Thing;
class AsciiString;

// Zero Hour's Module.h order; only the enum's spelling reaches the ledger.
enum ModuleType
{
	MODULETYPE_FIRST = 0,
	MODULETYPE_BEHAVIOR = MODULETYPE_FIRST,
	MODULETYPE_CLIENT_BEHAVIOR,
	MODULETYPE_CLIENT_UPDATE,
	MODULETYPE_COLLIDE,
	MODULETYPE_CONTAIN,
	MODULETYPE_CREATE,
	MODULETYPE_DIE,
	MODULETYPE_DRAW,
	MODULETYPE_UPDATE,
	MODULETYPE_UPGRADE,
	MODULETYPE_DEFAULTAI,
	NUM_MODULE_TYPES
};

class ModuleFactory : public SubsystemInterface, public Snapshot
{
public:
	class ModuleTemplate
	{
	public:
		Module *(__cdecl *m_createProc)(Thing *, const ModuleData *);
		ModuleData *(__cdecl *m_createDataProc)(INI *);
		void (__cdecl *m_create08)(int, int, int);
		int m_tag0C;
	};
	typedef std::map<NameKeyType, ModuleTemplate> ModuleTemplateMap;
	typedef std::vector<const ModuleData *> ModuleDataList;

	Module *newModule(Thing *, const AsciiString &, const ModuleData *, ModuleType);	// 0x00256783
	ModuleData *newModuleDataFromINI(INI *, const AsciiString &, ModuleType, const AsciiString &);	// 0x002571F9
	int findModuleInterfaceMask(const AsciiString &, ModuleType);	// 0x00256F39
	void GetAssetList(const AsciiString &, int, ModuleType, int, int);	// 0x002567B9

protected:
	const ModuleTemplate *findModuleTemplate(const AsciiString &, ModuleType);	// 0x0025674F
	static NameKeyType makeDecoratedNameKey(const AsciiString &, ModuleType);	// 0x00255C5E

private:
	ModuleTemplateMap m_moduleTemplateMap;	// +0x10
	ModuleDataList m_moduleDataList;		// +0x1C
};

extern ModuleFactory *TheModuleFactory;

#endif
