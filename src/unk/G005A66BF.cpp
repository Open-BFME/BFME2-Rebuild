#include "GameNetwork/NAT.h"
#include "GameNetwork/PortNegotiationSchema.h"
// Generated from the ledger; map snapshot c591f7146f88c8a8248ebb8ae22f0c978c22aed463db64e05ef57ff0dfae99cc
// object=G005A66BF name-evidence=invented
// Extents and edges are hypotheses; byte verification alone does not establish original TU boundaries.
// FUNCTION: BFME2 0x005A66BF size=45 name=?hasSamples@PortNegotiationPing@@QAEMXZ evidence=X:map request (link (from standalone link 0cf3e833)) tier=B hypothesis:mm1-gap-unit
Real PortNegotiationPing::hasSamples()
{
	return m_unk0x08 > 0.0f && m_unk0x04 > 0.0f;
}
// FUNCTION: BFME2 0x005A6709 size=20 name=?rva005A6709@NAT@@QBEHXZ evidence=X:proven (caller-this) tier=B hypothesis:mm1-gap-unit
Bool NAT::isReady() const
{
	return m_hostSlot != 8 && m_hostSlot == m_localSlot;
}
