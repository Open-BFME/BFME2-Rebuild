// AIGroup.h: SAGE AI group (Zero Hour AIGroup.h names).
// Target facts: AI::createGroup 0x002FEC4B allocates 0x3C bytes and calls the
// out-of-line constructor 0x0036E4C7, which stores table 0x00C17D00, constructs
// the list at +4 and the vector at +0x30, zeroes +0x08 (Real), +0x0C (Bool),
// +0x14 and the six Reals +0x18..+0x2C except +0x24 (constant 0x00BC2428), and
// takes the next group ID from TheAI (+0x1C, pre-incremented) into +0x10.
// Table 0x00C17D00 is Snapshot's four slots: deleting destructor 0x0036E8EA
// (protected, ??_GAIGroup@@MAEPAXI@Z), the shared empty loadPostProcess,
// GetSnapshotName 0x0036E55E and xfer 0x0036E3C6. AI::destroyGroup 0x002FE712
// destroys groups, so AI is a friend.
#pragma once
#ifndef BFME2_AIGROUP_H
#define BFME2_AIGROUP_H

#include "Common/Coord3D.h"

#include "Common/GameType.h"
#include "Common/Snapshot.h"
#include "Common/StlList.h"
#include "Common/Vector.h"

class Object;
class Path;
class CommandButton;
class PolygonTrigger;
class Team;
class Waypoint;
class SequentialScript;
enum WeaponLockType;
enum SpecialPowerType;
class Rva003427DD;	// the map's spelling of the DamageInfo record aiGoProne takes (AICommandInterface.h)
enum GUICommandType;
enum WeaponSlotType;
enum WeaponSetType;
class Coord3D;
class Coord2D;
class Player;
enum AttitudeType;
class PolygonTrigger;
class Team;
// Zero Hour's guard modes; AIPlayer::guardSupplyCenter 0x004F2ABC passes 0 for the normal guard.
enum GuardMode
{
	GUARDMODE_NORMAL,
	GUARDMODE_GUARD_WITHOUT_PURSUIT,
	GUARDMODE_GUARD_FLYING_UNITS_ONLY
};

class AIGroup : public Snapshot
{
public:
	Bool isIdle() const;								// 0x0036DF4D: every living member with an AI is idle
	const _STL::vector<ObjectID> &getAllIDs();				// 0x0036F710: rebuilds the +0x30 vector from the members' object IDs and returns it (Zero Hour's AIGroup::getAllIDs)
	static void rotateOffset(const Coord3D *from, const Coord3D *to, Coord2D *offset);	// 0x0036CBF9
	Real getSpeed();								// 0x0036E333: recomputes the group speed first when the group is dirty
	void add(Object *obj);							// 0x0036E5F1: joins the group when the object has an AI or is a structure or always selectable
	Bool remove(Object *obj);				// 0x0036CF07 (Object::leaveGroup calls it)
	Bool isMember(Object *obj);				// 0x0036CED0
	void cohereAttackState();				// 0x003705C2
	void rva0036DBBA(Int unused, Int command, UnsignedInt frames);	// 0x0036DBBA (the map's name; the first argument is not read): commands 0x438 and 0x44C..0x44F set special model conditions 0x72 and 0xB8..0xBB on every member
	Bool rva0036CFE5(const Player *player);	// 0x0036CFE5 (the map's name): removeAnyObjectsNotOwnedByPlayer's twin keyed on the member's contain slot 56
	void groupSetPositionInFormation(const Coord3D *pos, const Coord3D *facing, Int mode);	// 0x0036D6F4
	void rva00370554(Object *target, CommandSourceType cmdSource);	// 0x00370554 (the map's name): the first member's AI gets 0x0036EC1D on the target
	Bool rva0036E071() const;				// 0x0036E071 (the map's name): false when a live member's AI answers slot 113 (rva00262DA3) true
	void rva0036D6B4();						// 0x0036D6B4 (map: placeholder class Rva0036ListOwner; groupSetPositionInFormation 0x0036D6F4 calls it on the group): each member's 0x0028AD32, then its horde contain's slot 52
	Bool rva0036E357();						// 0x0036E357 (the map's name): walks the members' AIs; groupFollowWaypointPathInFormation and 0x003724B8 call it before forming up
	void createFormation(Int cmdSource, Int mode);	// 0x005494A0 (map ?createFormation@AIGroup@@QAEXHH@Z, 1997 B)
	void setWeaponSetFlag(WeaponSetType wst);	// 0x0036DE17
	void setSequentialScriptOnMembers(SequentialScript *script, Int flag);	// 0x0036E2A7 (public: runs setCurScript(script, true, flag) on the script tracker of every member whose object has an AI update)
	void rva00372C05();						// 0x00372C05 (the map's name; AITactic::cohereTeams 0x004ED08A runs it on a team's group)
	Bool containsAnyObjectsNotOwnedByPlayer(const Player *player);	// 0x0036CF73
	Bool removeAnyObjectsNotOwnedByPlayer(const Player *player);	// 0x0036CFA4
	AIGroup();											// 0x0036E4C7
	void groupChangeStance(Int stance);				// 0x0036E269: every member's StancesBehavior::changeStance 0x0045F084
	void clearGroundPath();							// 0x0036CE87: deletes the ground path and resets the cached path state (the destructor runs it)

	// Group wrappers: each sends one command to every member's AI command interface (retail
	// 0x0037008E..0x00370283 and 0x0036FA8C, 0x0036FAC2).
	void groupHunt(CommandSourceType cmdSource);
	void groupIdle(CommandSourceType cmdSource);	// map: ?groupIdle@AIGroup@@QAEXW4CommandSourceType@@@Z
	Bool getCenter(Coord3D *center);	// map: ?getCenter@AIGroup@@QAE_NPAVCoord3D@@@Z
	void groupEvacuate(CommandSourceType cmdSource);	// map: ?groupEvacuate@AIGroup@@QAEXW4CommandSourceType@@@Z
	void groupAttackPosition(const Coord3D *pos, Int maxShotsToFire, CommandSourceType cmdSource);	// 0x0036FF74 (Zero Hour's): passengers allowed to fire, slaves, then the member attack pos (a null pos: each member's own position)
	void groupAttackTeam(const Team *team, Int maxShots, CommandSourceType cmdSource);	// map: ?groupAttackTeam@AIGroup@@QAEXPBVTeam@@HW4CommandSourceType@@@Z
	void groupAttackArea(PolygonTrigger *area, CommandSourceType cmdSource);	// map: ?groupAttackArea@AIGroup@@QAEXPAVPolygonTrigger@@W4CommandSourceType@@@Z
	void groupMoveToAndEvacuate(const Coord3D *pos, CommandSourceType cmdSource);
	void groupMoveToAndEvacuateAndExit(const Coord3D *pos, CommandSourceType cmdSource);
	void groupRepair(Object *target, CommandSourceType cmdSource);
	void groupResumeConstruction(Object *target, CommandSourceType cmdSource);
	void groupGetHealed(Object *target, CommandSourceType cmdSource);
	void groupGetRepaired(Object *target, CommandSourceType cmdSource);
	void groupDock(Object *target, CommandSourceType cmdSource);
	void groupHarvest(const Coord3D *pos, CommandSourceType cmdSource);
	void groupExit(Object *target, CommandSourceType cmdSource);
	void setAttitude(AttitudeType attitude);
	void groupDoCommandButton(const CommandButton *button, CommandSourceType cmdSource);
	void groupDoCommandButtonAtPosition(const CommandButton *button, const Coord3D *pos, CommandSourceType cmdSource);	// 0x0036DCB8
	void groupDoCommandButtonAtObject(const CommandButton *button, Object *obj, CommandSourceType cmdSource);	// 0x0036DCE7
	void groupGuardObject(Object *target, Int guardMode, CommandSourceType cmdSource);	// 0x00370410
	void groupGuardPosition(const Coord3D *pos, GuardMode mode, CommandSourceType cmdSource);	// 0x003703CF
	void groupEnter(Object *target, CommandSourceType cmdSource);	// 0x00370198
	void groupFollowWaypointPath(const Waypoint *way, CommandSourceType cmdSource, Int mode);	// 0x0036F9FE: mode 0 each member's aiFollowWaypointPath, 1 its aiAttackFollowWaypointPath with no shot limit
	void groupFollowWaypointPathAsTeam(const Waypoint *way, CommandSourceType cmdSource, Int mode);	// 0x0036FAF8: clears each member's Object +0x410, then mode 0 aiFollowWaypointPathAsTeam, 1 aiAttackFollowWaypointPathAsTeam
	void groupFollowWaypointPathInFormation(const Waypoint *way, CommandSourceType cmdSource, Int mode);	// 0x0036FB57: forms the group up first (createFormation 0x005494A0), then each member follows the path (ScriptActions::doTeamFollowWaypoints 0x003BF39F calls it with mode 0, doTeamAttackMoveFollowWaypoints 0x003BF4CF with mode 1; map: ?rva0036FB57@AIGroup@@QAEXPBVWaypoint@@W4CommandSourceType@@H@Z, placeholder)
	void groupFollowWaypointPathExact(const Waypoint *way, CommandSourceType cmdSource);	// 0x0036FA56 (Zero Hour name): each member's aiFollowWaypointPathExact 0x0036ECE7
	void groupFollowWaypointPathAsTeamExact(const Waypoint *way, CommandSourceType cmdSource);	// 0x0036FBC5 (Zero Hour name): clears each member's Object +0x410, then its command 0x33 0x0036EDB1 (Zero Hour's AICMD_FOLLOW_WAYPOINT_PATH_AS_TEAM_EXACT follows _EXACT 0x32)
	void groupGuardTeam(const Team *team, Int guardMode, CommandSourceType cmdSource);	// 0x00370451: each member's AICommandInterface 0x0036F54D (command 0x20, between guard-object 0x1F and guard-area 0x21)
	void groupGuardArea(const PolygonTrigger *area, Int guardMode, CommandSourceType cmdSource);	// 0x00370492: each member's aiGuardArea 0x0036F5BB
	void groupGuardAreaFromPosition(const PolygonTrigger *area, Int guardMode, CommandSourceType cmdSource, const Coord3D *pos);	// 0x003704D3: each member's aiGuardAreaFromPosition 0x0036F629
	void groupGoProne(const Rva003427DD *damageInfo, CommandSourceType cmdSource);	// 0x00370399 (Zero Hour's name; the map spells it rva00370399): every member's aiGoProne 0x0036F400
	void groupOneRing();	// 0x00370680 (the map's name): members with a One Ring module (Object 0x0028F4BC) go idle and update it
	Object *getSpecialPowerSourceObject(UnsignedInt specialPowerID);	// 0x0036E111
	Object *getCommandButtonSourceObject(GUICommandType commandType);	// 0x0036E158
	Bool setWeaponLockForGroup(WeaponSlotType weaponSlot, WeaponLockType lockType);	// 0x0036DD45 (Zero Hour's name)
	Bool rva0036E0B6() const;	// 0x0036E0B6 (the map's name): true when any member's AI has its flag at +0x3CA set
	Bool rva0036E0E3();			// 0x0036E0E3 (the map's name): true when every member is effectively dead (an empty group is true)
	void releaseWeaponLockForGroup(WeaponLockType lock);
	// Zero Hour's inline forwarder: ScriptActions 0x003C3B7D calls the private 0x0036FD64 straight from the script row (its false is the first pushed argument).
	void groupAttackObject(Object *victim, Int maxShotsToFire, CommandSourceType cmdSource) { groupAttackObjectPrivate(false, victim, maxShotsToFire, cmdSource); }
	void groupForceAttackObject(Object *victim, Int maxShotsToFire, CommandSourceType cmdSource);

protected:
	friend class AI;
private:
	void recompute();									// 0x0036D2C5 (private: the map spells ?recompute@AIGroup@@AAEXXZ; getSpeed 0x0036E333 calls it)
	void groupAttackObjectPrivate(Bool forced, Object *victim, Int maxShotsToFire, CommandSourceType cmdSource);	// 0x0036FD64 (map spelling ?groupAttackObjectPrivate@AIGroup@@AAEX_NPAVObject@@HW4CommandSourceType@@@Z)
protected:

	virtual ~AIGroup();									// 0x0036E564
	virtual void loadPostProcess() {}					// slot 1, shared empty body
	virtual const char *GetSnapshotName() const;		// slot 2, 0x0036E55E
	virtual void xfer(Xfer *xfer);						// slot 3, 0x0036E3C6

	_STL::list<Object *> m_memberList;					// +0x04
	Real m_speed;										// +0x08
	Bool m_dirty;										// +0x0C
	UnsignedInt m_id;									// +0x10
	Path *m_groundPath;									// +0x14
	Coord3D m_position;									// +0x18 (WorldBuilder name; the constructor 0x0036E4C7 zeroes it)
	Coord3D m_spacing;									// +0x24 (WorldBuilder name; the constructor sets (10, 0, 0) from 0x00BC2428)
	_STL::vector<Int> m_unk0x30;						// +0x30, element type folded with int
};
// SIZE 0x3C

#endif
