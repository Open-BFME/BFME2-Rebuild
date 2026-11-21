// cl: /O2 /Oy-
#include "debug/Debug.h"
#include "debug/DebugStackwalk.h"
// Tier A; object=O0003B9E0 evidence=W flags=align=A
// Extent is a hypothesis; only Object VERIFIED establishes recovery.
// Edges: left=unknown-rows right=unknown-rows
// The stack walker: dbghelp.dll is loaded and its entry points resolved once by
// InitDbghelp (0x0003B850, its own unit); everything here goes through those.
// The two leaves at 0x0003BC60/0x0003BC70 forward into the Debug manager's
// pointer getters (+0xA0, +0xA4).
// FUNCTION: BFME2 0x0003B850 size=329 name=?InitDbghelp@@YAXXZ evidence=N:ledger tier=B hypothesis:mm1-neighbour-callers+libfile
// FUNCTION: BFME2 0x0003B9A0 size=32 name=??4Signature@DebugStackwalk@@QAEAAV01@ABV01@@Z evidence=N:ledger tier=B hypothesis:mm1-neighbour-class
// FUNCTION: BFME2 0x0003B9C0 size=31 name=?GetAddress@Signature@DebugStackwalk@@QBEIH@Z evidence=N:ledger tier=B hypothesis:mm1-neighbour-class
unsigned DebugStackwalk::Signature::GetAddress(int n) const
{
	if (n < 0 || n >= MAX_ADDR)
		return 0;
	return m_addr[n];
}

// FUNCTION: BFME2 0x0003B9E0 size=635 name=?GetSymbol@Signature@DebugStackwalk@@SAXIPADI@Z evidence=N:ledger

// FUNCTION: BFME2 0x0003BC60 size=14 name=?rva0003BC60@@YAPAXXZ evidence=?:placeholder

extern Debug *theDebug;

void *rva0003BC60()
{
	return const_cast<char *>(theDebug->GetVersion());
}

// FUNCTION: BFME2 0x0003BC70 size=14 name=?rva0003BC70@@YAPAXXZ evidence=?:placeholder

void *rva0003BC70()
{
	return const_cast<char *>(theDebug->GetInternalVersion());
}

// FUNCTION: BFME2 0x0003BC80 size=736 name=?GetSymbol@Signature@DebugStackwalk@@SAXIPADIPAI0I10I11_N@Z evidence=N:ledger

// FUNCTION: BFME2 0x0003BF60 size=369 name=??6@YAAAVDebug@@AAV0@ABVSignature@DebugStackwalk@@@Z evidence=N:ledger

// FUNCTION: BFME2 0x0003C0E0 size=12 name=??0DebugStackwalk@@AAE@XZ evidence=N:ledger

void InitDbghelp(void);

DebugStackwalk::DebugStackwalk(void)
{
	InitDbghelp();
}

// FUNCTION: BFME2 0x0003C0F0 size=6 name=?GetDbghelpHandle@DebugStackwalk@@SAPAXXZ evidence=N:ledger

bool g_oldDbghelp = 0;	// .bss 0x00DE08A8, before g_dbghelp 0x00DE08AC in retail
void *g_dbghelp = 0;

void *DebugStackwalk::GetDbghelpHandle(void)
{
	return g_dbghelp;
}

// FUNCTION: BFME2 0x0003C100 size=6 name=?IsOldDbghelp@DebugStackwalk@@SA_NXZ evidence=N:ledger


bool DebugStackwalk::IsOldDbghelp(void)
{
	return g_oldDbghelp;
}

// FUNCTION: BFME2 0x0003C110 size=1054 name=?StackWalk@DebugStackwalk@@SAHAAVSignature@1@PAU_CONTEXT@@_N@Z evidence=N:ledger
