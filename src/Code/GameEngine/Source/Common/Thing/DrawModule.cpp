// Tier A; object=O000B19BD evidence=W flags=align=U
// DrawModule's snapshot name is the literal class name; retail's body is the
// six-byte `mov eax,<literal>; ret` the vtable reference at 0x007C9698 carries
// in slot 2.
// FUNCTION: BFME2 0x000B19A1 size=28 name=??0DrawModule@@QAE@PAVThing@@PBVModuleData@@@Z evidence=N:ledger tier=B hypothesis:mm1-neighbour-class
// FUNCTION: BFME2 0x000B19BD size=6 name=?GetSnapshotName@DrawModule@@MBEPBDXZ evidence=N:ledger

#include "GameClient/DrawModule.h"

const char *DrawModule::GetSnapshotName() const
{
	return "DrawModule";
}
