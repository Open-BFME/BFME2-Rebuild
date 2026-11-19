// ThingFactory.h: the subsystem that owns every ThingTemplate and creates Objects and Drawables.
// Target facts (retail RVAs): a SubsystemInterface subclass. Constructor 0x002D0E14 stores table
// 0x00C0233C, clears the template list head at +0x0C, builds the template hash map at +0x14 and
// reserves 0x3000 buckets. Destructor 0x002D0588 runs freeDatabase (0x002D0469), destroys the map and
// the base. Table (14 slots): 0 deleting dtor 0x002D09F5, 1 empty 0x000B3FD0, 2 the base
// loadIniFilesFromLegend, 3 postProcessLoad 0x002CF5FF, 4 0x002CF172, 5 0x002CEF14, 6 and 7 the base
// defaults, 8 empty, 9 reset 0x002D05CF, 10 empty, 11 0x002CEF6C, 12 empty, 13 the base default.
// findTemplate 0x002D06CA and the existence test 0x002D06AA look a name up in the map through the
// out-of-line hash_map::find 0x0041534B and return the mapped pointer (node + 8). SIZE 0x28.
// Zero Hour's ThingFactory.h is the name donor.
#pragma once
#ifndef BFME2_THINGFACTORY_H
#define BFME2_THINGFACTORY_H

#include "Common/GameType.h"
#include "Common/AsciiString.h"
#include "Common/SubsystemInterface.h"
#include "Common/STLTypedefs.h"

class INI;
class ThingTemplate;
class Object;
class Team;
class Drawable;
struct CreateMask;

#if defined(_STLPORT_VERSION)
#include <hash_map>
typedef _STL::hash_map<AsciiString, ThingTemplate *, rts::hash<AsciiString>, _STL::equal_to<AsciiString> > ThingTemplateHashMap;
#endif

class ThingFactory : public SubsystemInterface
{
public:
	ThingFactory();																// 0x002D0E14
	virtual ~ThingFactory();													// slot 0: 0x002D0588 (deleting 0x002D09F5)

	virtual void init() {}														// slot 1 (0x000B3FD0, empty)
	virtual void postProcessLoad();												// slot 3, 0x002CF5FF
	// Retail's table 0x0080233C (and W3DThingFactory's 0x007C4858) holds the class's own bodies in
	// slots 4, 5 and 11, not SubsystemInterface's inline defaults: 0x002CF172 (the map's
	// ThingFactory::attemptReload), 0x002CEF14 and 0x002CEF6C.
	virtual Bool attemptReload(Bool *needsRestart);								// slot 4, 0x002CF172
	virtual Bool attemptRefresh();											// slot 5, 0x002CEF14
	virtual void reset();														// slot 9, 0x002D05CF
	virtual void update() {}													// slot 10 (0x000B3FD0, empty)
	virtual Bool dumpToCSV(const char *fileName);								// slot 11, 0x002CEF6C

	// Target 0x002D1C01, old shared ThingFactory.h and parser call sites.
	static void parseObjectDefinition(INI *, const AsciiString &, const AsciiString &, const AsciiString &);
	ThingTemplate *newOverride(ThingTemplate *thingTemplate);					// 0x002D1AB4
	const ThingTemplate *findTemplate(const AsciiString &name);					// 0x002D06CA
	Bool rva002D06AA(const AsciiString &name);									// 0x002D06AA (is the name in the map)
	const ThingTemplate *findByTemplateID(UnsignedShort id);					// 0x002CF1FB (Zero Hour name; callers pass the factory in ecx, the walk reads +0x0C, +0x484 and +0x5D8)
	Drawable *newDrawable(const ThingTemplate *,Int,Int);						// 0x002CF21B
	Object *newObject(const ThingTemplate *,Team *,const CreateMask *,ObjectID);	// 0x002D0A23
	ThingTemplate *firstTemplate() const { return m_firstTemplate; }			// inline in retail: ScriptEngine::setPriorityKind 0x0020611D reads +0x0C and walks ThingTemplate::friend_getNextTemplate

private:
	void freeDatabase();														// 0x002D0469

	ThingTemplate *m_firstTemplate;												// +0x0C
	UnsignedInt m_nextTemplateID;												// +0x10
#if defined(_STLPORT_VERSION)
	ThingTemplateHashMap m_templateHashMap;										// +0x14
#else
	unsigned char m_templateHashMapStorage[0x14];								// +0x14
#endif
};

extern ThingFactory *TheThingFactory;

#endif
