// cl: /O2
#include "Draft/CullableClass.h"
#include "Draft/CullSystemClass.h"
#include "Draft/CullLinkClass.h"
// Tier A; object=O007244C0 evidence=W flags=align=A
// Extent is a hypothesis; only Object VERIFIED establishes recovery.
// Edges: left=unknown-rows right=unknown-rows
// WWMath's cullable base: every object carries a box (+0xC), a link into its
// culling system (+8) and the collection chain (+0x24); the system keeps only
// the head of that chain (+4).
// FUNCTION: BFME2 0x007244C0 size=65 name=??0CullableClass@@QAE@XZ evidence=N:ledger

CullableClass::CullableClass(void) :
	CullLink(0),
	NextCollected(0)
{
	CullBox.Init(Vector3(0.0f, 0.0f, 0.0f), Vector3(1.0f, 1.0f, 1.0f));
}

// FUNCTION: BFME2 0x00724520 size=17 name=?Set_Culling_System@CullableClass@@QAEXPAVCullSystemClass@@@Z evidence=N:ledger

void CullableClass::Set_Culling_System(CullSystemClass *system)
{
	if (CullLink != 0)
		CullLink->Set_Culling_System(system);
}

// FUNCTION: BFME2 0x00724540 size=14 name=?Get_Culling_System@CullableClass@@QBEPAVCullSystemClass@@XZ evidence=N:ledger

CullSystemClass *CullableClass::Get_Culling_System(void) const
{
	if (CullLink != 0)
		return CullLink->Get_Culling_System();
	return 0;
}

// FUNCTION: BFME2 0x00724550 size=16 name=??0CullSystemClass@@QAE@XZ evidence=N:ledger

CullSystemClass::CullSystemClass(void) :
	CollectionHead(0)
{
}

// FUNCTION: BFME2 0x00724560 size=7 name=??1CullSystemClass@@UAE@XZ evidence=N:ledger

CullSystemClass::~CullSystemClass(void)
{
}

// FUNCTION: BFME2 0x00724570 size=19 name=?Get_Next_Collected_Object_Internal@CullSystemClass@@IAEPAVCullableClass@@PAV2@@Z evidence=N:ledger

CullableClass *CullSystemClass::Get_Next_Collected_Object_Internal(CullableClass *current)
{
	if (current != 0)
		return current->NextCollected;
	return 0;
}

// FUNCTION: BFME2 0x00724590 size=16 name=?Add_To_Collection@CullSystemClass@@IAEXPAVCullableClass@@@Z evidence=N:ledger

void CullSystemClass::Add_To_Collection(CullableClass *added)
{
	added->NextCollected = CollectionHead;
	CollectionHead = added;
}

// FUNCTION: BFME2 0x007245A0 size=71 name=?Set_Cull_Box@CullableClass@@QAEXABVAABoxClass@@_N@Z evidence=N:ledger

void CullableClass::Set_Cull_Box(const AABoxClass &box, bool just_loaded)
{
	CullBox = box;
	if (!just_loaded)
	{
		CullSystemClass *sys = Get_Culling_System();
		if (sys != 0)
			sys->Update_Culling(this);
	}
}
