// AptCallbackHost.h: the base every Apt screen and Apt sub-panel carries. It is the class the
// map spells Rva005248D0 (its destructor 0x005248D0; constructor 0x002D2C34).
//
// Target facts (retail game.dat, RVAs; table addresses are VAs):
//  - SIZE 0x58. One virtual, the destructor: its table VA 0x00C02A80 has one slot (the map's
//    vtable record says "slots 1"); a derived class installs its own table over it.
//  - The constructor 0x002D2C34 builds six members in order: +0x04 and +0x10 (ctor 0x001F81BF each,
//    12 bytes: vector<AsciiString>), +0x1C (ctor 0x00524415, 24 bytes: two such vectors at +0x1C
//    and +0x28), +0x34, +0x40 and +0x4C (12 bytes each, ctors 0x001F81BF). The destructor 0x005248D0
//    resets the vptr and destroys them in reverse.
//  - The Apt registration calls every screen constructor makes (`AptLanLobby::OnCancelBttn` and so on)
//    reach these through `this + 0x21C` = +0x04 (AptCommandMapAdder 0x0052458E), `this + 0x228` = +0x10
//    (AptExternHandlerAdder 0x005245F3) and `this + 0x24C` = +0x34 (AptCustomRenderAdder 0x005246C0);
//    the members' types are the *Adder classes of AptCallbackAdders.cpp and are not recovered here,
//    so the 0x54 bytes stay one array.
// A screen's secondary vtable is this class's (one slot, a deleting-destructor adjustor thunk).
#pragma once
#ifndef BFME2_APTCALLBACKHOST_H
#define BFME2_APTCALLBACKHOST_H

class AptCallbackHost
{
public:
	AptCallbackHost();					// 0x002D2C34 (the map names the class Rva005248D0)
	virtual ~AptCallbackHost();			// 0x005248D0 body, slot 0

	unsigned char m_hooks[0x54];		// +0x04..+0x57: the six registries (see above)
};
typedef char AptCallbackHostSizeCheck[sizeof(AptCallbackHost)==0x58 ? 1 : -1];

#endif
