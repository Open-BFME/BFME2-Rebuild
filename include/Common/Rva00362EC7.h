// Rva00362EC7.h: BFME2 module base of LaserUpdate. SIZE 0xC.
// Target facts: ctor 0x00362EC7 stores vtable 0x00C170A4 at +0, the ModuleData
// at +4 and a Drawable at +8. Its empty destructor 0x00362EE3 stores 0x00C170A4
// and tail-calls 0x0049B47C; its scalar deleting destructor is 0x003630E6.
// LaserUpdate (ctor 0x00362EEE) derives from it and its destructor 0x00362F64
// restores the same 0x00C170A4. The class name is unproven, so the row's
// address-derived spelling is kept.
// Its table 0x00C170A4 carries Module's slots 4-8 (slot 4 __purecall, 5-8 the
// shared empty `ret` 0x000B3FD0), so it is a Module; it derives from
// DrawableModule (the Drawable at +8), and ~LaserUpdate inlines this destructor
// and ends in a call to the out-of-line ~DrawableModule at 0x0049B47C.
#pragma once
#ifndef BFME2_RVA00362EC7_H
#define BFME2_RVA00362EC7_H

#include "GameLogic/Module/Module.h"

class Drawable;
class ModuleData;
class Thing;
class Xfer;

class Rva00362EC7 : public DrawableModule
{
public:
	Rva00362EC7(Thing *thing, const ModuleData *moduleData);
	virtual ~Rva00362EC7();


	// Slots 11-14 of its 15-slot table (DrawableModule's 0-10 come first): 11 the
	// shared `mov al,1; ret` 0x0050B5C6, 12 __purecall (LaserUpdate's table
	// 0x008170E0 puts clientUpdate 0x00363233 there), 13 and 14 the shared
	// `xor eax,eax; ret` 0x000D43D0. Names of 11, 13 and 14 not recovered.
	virtual Bool vslot11() { return true; }				// slot 11
	virtual void clientUpdate() = 0;					// slot 12
	virtual void *vslot13() { return 0; }				// slot 13
	virtual void *vslot14() { return 0; }				// slot 14

};
// SIZE 0xC

#endif
