// cl: /O1 /EHs /EHc- /D_BFME_RETAIL_TREE_INSERT_LAYOUT /D_STLP_USE_MALLOC /D_STLP_USE_STATIC_LIB /D_CRTIMP=
// stlport46
#include "Draft/CaveSystem.h"
#include "Common/Xfer.h"
#include "Common/XferException.h"

// The singleton (retail VA 0x00E031F4).
CaveSystem *TheCaveSystem = NULL;
// Tier B; object=O004275B6 evidence=Z flags=align=A
// Extent is a hypothesis; only Object VERIFIED establishes recovery.
// Edges: left=unknown-rows right=unknown-rows
// The cave subsystem: a vector of tunnel trackers at +0x10, one per cave.
// FUNCTION: BFME2 0x0042755D size=89 name=?canSwitchIndexToIndex@CaveSystem@@QAE_NII@Z evidence=X:map request (link (from standalone link 0cf3e833)) tier=B hypothesis:mm1-held-object-same-path
// FUNCTION: BFME2 0x004275B6 size=30 name=?getTunnelTrackerForCaveIndex@CaveSystem@@QAEPAVTunnelTracker@@I@Z evidence=N:ledger

TunnelTracker *CaveSystem::getTunnelTrackerForCaveIndex(UnsignedInt index)
{
	TunnelTracker *tracker = NULL;
	if (index < m_tunnelTrackerVector.size())
		tracker = m_tunnelTrackerVector[index];
	return tracker;
}

// FUNCTION: BFME2 0x004275D4 size=47 name=??0CaveSystem@@QAE@XZ evidence=N:ledger

CaveSystem::CaveSystem(void)
{
}

// FUNCTION: BFME2 0x00427603 size=6 name=?GetSnapshotName@CaveSystem@@MBEPBDXZ evidence=N:ledger

const char *CaveSystem::GetSnapshotName(void) const
{
	return "CaveSystem";
}

// FUNCTION: BFME2 0x00427611 size=82 name=??1CaveSystem@@UAE@XZ evidence=N:ledger

CaveSystem::~CaveSystem(void)
{
}

// FUNCTION: BFME2 0x0042767F size=55 name=?reset@CaveSystem@@QAEXXZ evidence=N:ledger

void CaveSystem::reset(void)
{
	for (std::vector<TunnelTracker *>::iterator it = m_tunnelTrackerVector.begin(); it != m_tunnelTrackerVector.end(); ++it)
	{
		TunnelTracker *tracker = *it;
		if (tracker != NULL)
			::delete tracker;
	}

	m_tunnelTrackerVector.clear();
}

// FUNCTION: BFME2 0x004276B6 size=130 name=?registerNewCave@CaveSystem@@QAEXH@Z evidence=N:ledger
void CaveSystem::registerNewCave(Int theIndex)
{
	bool needToCreate = false;
	if (theIndex >= m_tunnelTrackerVector.size())
	{
		needToCreate = true;
		while (theIndex >= m_tunnelTrackerVector.size())
		{
			TunnelTracker *empty = 0;
			m_tunnelTrackerVector.push_back(empty);
		}
	}
	else
	{
		if (m_tunnelTrackerVector[theIndex] == 0)
			needToCreate = true;
	}
	if (needToCreate)
		m_tunnelTrackerVector[theIndex] = new TunnelTracker;
}

// FUNCTION: BFME2 0x00427738 size=216 name=?DoXfer@CaveSystem@@UAEXPAVXfer@@@Z evidence=N:ledger
