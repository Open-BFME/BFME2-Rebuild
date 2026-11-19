// TeamFactory.h: the subsystem that owns every TeamPrototype. SIZE 0xC4.
//
// Target facts:
//  - ctor 0x003A25B1: SubsystemInterface at +0, Snapshot's vptr at +0xC; tables
//    0x00C1AE2C (14 slots) and 0x00C1AE1C (4 slots: ??_E adjustor 0x003A2624,
//    loadPostProcess 0x0039F7A5, GetSnapshotName 0x003A261E, xfer 0x003A308A); the
//    map at +0xB0 is built out of line, +0xBC and +0xC0 are zeroed and the 0xA0
//    bytes at +0x10 are memset.
//  - primary slots: init 0x003A3899 (a jump to clear), reset 0x003A389E (zeroes +0xBC
//    and +0xC0, then clear), update the folded empty body 0x000B3FD0; the others are
//    SubsystemInterface's.
//  - the map: retail's operator[] 0x003A2943 is map<pair<int,int>, TeamPrototype*>;
//    addTeamPrototypeToList 0x003A2B78 keys a prototype by the name keys of its
//    owner name (+0x10, first) and its name (+0x14, second). Iterating callers read
//    the prototype from node+0x18 and step with _Rb_global<bool>::_M_increment.
//  - dtor 0x003A383A: clear(), then the map's destructor, then SubsystemInterface's.
#pragma once
#ifndef BFME2_TEAMFACTORY_H
#define BFME2_TEAMFACTORY_H

#include <stddef.h>
#include <map>
#include "Common/GameType.h"
#include "Common/SubsystemInterface.h"
#include "Common/Snapshot.h"

class AsciiString;
class Team;
class TeamPrototype;
class SidesList;

class TeamFactory : public SubsystemInterface, public Snapshot
{
public:
	typedef _STL::map<_STL::pair<Int, Int>, TeamPrototype *> TeamPrototypeMap;

	TeamFactory();													// 0x003A25B1
	virtual ~TeamFactory();											// 0x003A383A

	virtual void init();											// 0x003A3899
	virtual void reset();											// 0x003A389E
	virtual void update() {}										// folded empty 0x000B3FD0

protected:
	virtual void loadPostProcess();									// 0x0039F7A5
	virtual const char *GetSnapshotName() const;					// 0x003A261E
	virtual void xfer(Xfer *xfer);									// 0x003A308A

public:
	void clear();													// 0x003A2F4C
	void initTeam(const AsciiString &name, const AsciiString &owner, Bool isSingleton, Dict *dict);	// 0x003A404D
	void initFromSides(SidesList *sides);							// 0x003A4182 (PlayerList::newGame calls it after the players exist)
	TeamPrototype *findTeamPrototypeByID(UnsignedInt id);			// 0x0039F72C
	TeamPrototype *findTeamPrototype(const AsciiString &qualifiedName);	// 0x003A2659 (N:ledger; splits the qualified name and forwards to the two-argument overload; ScriptConditions::evaluateHasUnits 0x003E9373)
	Team *findTeamByID(UnsignedInt id);								// 0x0039F761
	TeamPrototype *findTeamPrototype(const AsciiString &owner, const AsciiString &name);	// 0x0039FE6C
	void removeTeamPrototypeFromList(TeamPrototype *team);			// 0x0039FE11
	void addTeamPrototypeToList(TeamPrototype *team);				// 0x003A2B78
	Team *createEmptyTeam(Player *owner);	// 0x003A3CD9
	Team *createInactiveTeam(const AsciiString &owner, const AsciiString &name);	// 0x003A3B7E
	Team *findTeam(const AsciiString &owner, const AsciiString &name);	// 0x003A3E37
	void teamAboutToBeDeleted(Team *team);							// 0x003A3048
	void rva003A262C();												// 0x003A262C (runs each prototype's 0x003A0D0A)
	Team *createTeamOnPrototype(TeamPrototype *prototype, Bool activate);	// 0x003A3DBE (TeamPrototype::xfer calls it)

	Int m_unk0x10[40];												// +0x10, zeroed by the ctor and clear(); role not recovered
	TeamPrototypeMap m_prototypes;									// +0xB0
	UnsignedInt m_uniqueTeamPrototypeID;							// +0xBC, zeroed by the ctor and reset()
	UnsignedInt m_uniqueTeamID;										// +0xC0, zeroed by the ctor and reset()
};
// SIZE 0xC4
typedef char TeamFactorySize[(sizeof(TeamFactory) == 0xC4) ? 1 : -1];
typedef char TeamFactoryPrototypesOffset[(offsetof(TeamFactory, m_prototypes) == 0xB0) ? 1 : -1];

#endif
