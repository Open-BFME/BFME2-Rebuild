// Generated from the ledger; map snapshot c591f7146f88c8a8248ebb8ae22f0c978c22aed463db64e05ef57ff0dfae99cc
// object=G004D6F86 name-evidence=invented
// Extents and edges are hypotheses; byte verification alone does not establish original TU boundaries.
#include "GameLogic/Damage.h"
#include "Common/Xfer.h"
// FUNCTION: BFME2 0x004D6F86 size=62 name=?xfer@DamageInfo@@UAEXPAVXfer@@@Z evidence=X:sole slot of DamageInfo's table 0x007F9200 (stored by its constructors); public virtual in Damage.h tier=B hypothesis:mm1-gap-unit
void DamageInfo::xfer(Xfer *xfer)
{
	if (xfer->IsLightCRC())
		return;
	Xfer::Version version(1, 1);
	*xfer == version;
	DamageInfoInput *input = &in;
	input->xfer(xfer);
	DamageInfoOutput *output = &out;
	output->xfer(xfer);
}
// FUNCTION: BFME2 0x004D6FC4 size=273 name=?xfer@DamageInfoInput@@UAEXPAVXfer@@@Z evidence=X:sole slot of DamageInfoInput's table 0x007F91FC (stored by its constructors); public virtual in Damage.h tier=B hypothesis:mm1-gap-unit
// FUNCTION: BFME2 0x004D70D5 size=82 name=?xfer@DamageInfoOutput@@UAEXPAVXfer@@@Z evidence=X:sole slot of DamageInfoOutput's table 0x007F91AC (stored by its constructors); public virtual in Damage.h tier=B hypothesis:mm1-gap-unit
void DamageInfoOutput::xfer(Xfer *xfer)
{
	if (xfer->IsLightCRC())
		return;
	Xfer::Version version(1, 1);
	*xfer == version == m_actualDamageDealt == m_actualDamageClipped == m_noEffect;
}
