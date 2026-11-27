// Generated from the ledger; map snapshot c591f7146f88c8a8248ebb8ae22f0c978c22aed463db64e05ef57ff0dfae99cc
// object=G00038250 name-evidence=invented
// Extents and edges are hypotheses; byte verification alone does not establish original TU boundaries.
#include "Common/AsciiString.h"
#include "Common/UnicodeString.h"

// FUNCTION: BFME2 0x00038250 size=91 name=??0AsciiString@@QAE@ABVUnicodeString@@@Z evidence=N:ledger tier=B hypothesis:mm1-gap-unit
AsciiString::AsciiString(const UnicodeString &that)
{
	translate(that.str());
}

// FUNCTION: BFME2 0x000382B0 size=47 name=??4AsciiString@@QAEAAV0@ABVUnicodeString@@@Z evidence=N:ledger tier=B hypothesis:mm1-gap-unit
AsciiString &AsciiString::operator=(const UnicodeString &that)
{
	translate(that.str());
	return *this;
}
