// Portions derived from Command & Conquer Generals and Command & Conquer Generals Zero Hour, (c) Electronic Arts Inc., GPL-3.0-or-later; see LICENSE and NOTICE.
// hanimmgr.h: WW3D2's animation manager (Zero Hour's hanimmgr.h): the loaded animations by name and the names whose load failed.
// Target facts (retail game.dat): HAnimManagerClass::Peek_Anim 0x000F0B81 looks the name up in the HashTableClass at +0 and
// Is_Missing 0x000F0BC2 in the one at +4; Add_Anim 0x000F0BAB takes a reference on the animation and adds it to the first.
#pragma once
#ifndef BFME2_HANIMMGR_H
#define BFME2_HANIMMGR_H

#include "WWLib/HashTableClass.h"
#include "WW3D2/hanim.h"

class HAnimManagerClass
{
public:
	HAnimClass *Peek_Anim(const char *name);	// 0x000F0B81
	HAnimClass *Get_Anim(const char *name);		// 0x000F0B98
	bool Add_Anim(HAnimClass *new_anim);		// 0x000F0BAB
	bool Is_Missing(const char *name);		// 0x000F0BC2

private:
	HashTableClass *AnimPtrTable;			// +0x00
	HashTableClass *MissingAnimTable;		// +0x04
};

#endif
