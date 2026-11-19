// ShroudClient.h: what the shroud manager tracks for anything it shrouds or
// ghosts: geometry, position, orientation and the manager's per-object data.
// SIZE 0x4 (vptr only). Class and method names are PROVISIONAL: no retail or
// WorldBuilder string names them. The Shroud prefix follows the WorldBuilder-
// named overrides on the sibling interface (Object::ShroudHideIfFogged).
// Target facts (retail game.dat; functions by RVA, tables by VA):
//  - Object (ctor 0x00298EA9) and GhostObject (ctor 0x00305A8F) are its only
//    derived classes, both through a virtual base. Object's vbtable 0x00BFC320
//    {-4, 0x46C} puts it at +0x4D4 with a vtordisp at +0x4D0; GhostObject's
//    0x00C07950 {-4, 0x7C} at +0x84. Its own table 0x00C078DC (five
//    __purecall) is ICF-folded with other five-slot abstract tables (InGameUI,
//    TurretAI, ...), so most stores of 0x00C078DC are not this class.
//  - Object's overrides, through the vtordisp thunks of table 0x00BFC27C:
//    slot 0 0x00299C99 returns &m_geometryInfo (+0xA8), slot 1 0x00299CCE
//    returns Thing's position (+0x38), slot 2 0x00299CDD loads Thing's angle
//    (+0x44), slot 3 0x00299CA8 stores its argument at +0x4C4, slot 4
//    0x00299CBA returns it. WorldBuilder's Object::ShroudHideIfFogged asserts
//    !(!m_smData) on that member, and ~Object (0x00299E2E) calls
//    TheShroudManager->UnRegisterObject only when it is set.
//  - ShroudManager::Data::UpdateCellsTouched (0x0073AD90) converts its owning
//    or ghost object to this base through the vbptr and calls slots 0 and 1.
// The data type keeps the map's (Zero Hour) spelling PartitionData for what
// WorldBuilder calls ShroudManager::Data (makeDirty 0x0073A0F0 sits in
// shroudmanager_data.cpp).
#pragma once
#ifndef BFME2_SHROUDCLIENT_H
#define BFME2_SHROUDCLIENT_H

#include "Common/GameType.h"

class GeometryInfo;
class PartitionData;
class Coord3D;

class ShroudClient
{
public:
	virtual const GeometryInfo *ShroudGetGeometryInfo() const = 0;	// slot 0
	virtual const Coord3D *ShroudGetPosition() const = 0;			// slot 1
	virtual Real ShroudGetOrientation() const = 0;					// slot 2
	virtual void ShroudSetData(PartitionData *data) = 0;			// slot 3
	virtual PartitionData *ShroudGetData() const = 0;				// slot 4
};
// SIZE 0x4

#endif
