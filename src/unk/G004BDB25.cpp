// cl: /O1 /EHs /EHc-
// stlport46
// Generated from the ledger; map snapshot c591f7146f88c8a8248ebb8ae22f0c978c22aed463db64e05ef57ff0dfae99cc
// object=G004BDB25 name-evidence=invented
// Extents and edges are hypotheses; byte verification alone does not establish original TU boundaries.
#include "GameLogic/Object.h"
#include "GameClient/Drawable.h"
#include "GameLogic/Module/ActiveBody.h"
// FUNCTION: BFME2 0x004BDB25 size=18 name=?invoke@Rva0020DFC0Object@@QAEXXZ evidence=?:placeholder tier=B hypothesis:mm1-gap-unit
void ActiveBody::reapplyDamageState()
{
	setDamageState(getDamageState());
}

// FUNCTION: BFME2 0x004BDB37 size=78 name=?rva004BDB37@ActiveBody@@UAEXXZ evidence=X:map request (link: 5 referrers) tier=B hypothesis:mm1-gap-unit
void ActiveBody::rva004BDB37()
{
	if (m_object->getDrawable() == NULL)
		return;
	for (Int i = 0; i < 3; ++i)
	{
		setCorrectDamageState(true);
		m_object->rva0028FCD9(m_curDamageState, m_bodySideDestroyed);
		m_object->getDrawable()->rva00274176(false);
	}
	updateBodyParticleSystems();
}
