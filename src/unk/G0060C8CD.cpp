// Generated from the ledger; map snapshot c591f7146f88c8a8248ebb8ae22f0c978c22aed463db64e05ef57ff0dfae99cc
// object=G0060C8CD name-evidence=invented
// Extents and edges are hypotheses; byte verification alone does not establish original TU boundaries.
#include "Common/XferSave.h"
#include "Common/File.h"
// FUNCTION: BFME2 0x0060C8CD size=45 name=?Close@XferSave@@QAEXXZ evidence=S:strong (caller-this) tier=B hypothesis:mm1-gap-unit
void XferSave::Close()
{
	if (m_file != 0)
	{
		if (m_stringTableMode)
		{
			UnsignedInt tag = 0x454E44;
			m_file->write(&tag, 4);
		}
		m_file = 0;
	}
}
