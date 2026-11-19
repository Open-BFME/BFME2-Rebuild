// CollisionManager.h: the collision library's manager class (WorldBuilder's CollisionManager, with its
// per-object CollisionManager::Data; the ledger names ?UpdateCoord@Data@CollisionManager@@ at 0x00758590).
// Only what game code calls is declared; the layout stays the library's (Object holds a Data * at +0x4CC).
#pragma once
#ifndef BFME2_COLLISIONMANAGER_H
#define BFME2_COLLISIONMANAGER_H

class CollisionManager
{
public:
	class Data
	{
	public:
		// 0x007584C0 (map placeholder ?init@Rva009A2350): links the data into its manager's dirty list once
		// (+0x10 / +0x14 against the manager's +8), the shape of the shroud data's makeDirty 0x0073A0F0.
		// Object::setGeometryInfoZ 0x0028ABFC and Object::setTeam 0x00298AE4 call it.
		void makeDirty();
	};
};

#endif
