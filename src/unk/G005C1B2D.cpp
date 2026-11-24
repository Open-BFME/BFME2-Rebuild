// cl: /O1
// Generated from the ledger; map snapshot c591f7146f88c8a8248ebb8ae22f0c978c22aed463db64e05ef57ff0dfae99cc
// object=G005C1B2D name-evidence=invented
// Extents and edges are hypotheses; byte verification alone does not establish original TU boundaries.
#include "Common/AsciiString.h"
#include "Common/UnicodeString.h"
#include "GameClient/GameText.h"
// FUNCTION: BFME2 0x005C1B2D size=177 name=?Rva005C1B2D@@YA?AVUnicodeString@@ABVAsciiString@@@Z evidence=?:placeholder tier=B hypothesis:mm1-gap-unit
UnicodeString Rva005C1B2D(const AsciiString &side)
{
	UnicodeString text(L"-");
	if (!side.isEmpty())
	{
		AsciiString label("SIDE:");
		label.concat(side);
		text = TheGameText->fetch(label, 0);
	}
	return text;
}
