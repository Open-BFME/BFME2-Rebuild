// Generated from the ledger; map snapshot c591f7146f88c8a8248ebb8ae22f0c978c22aed463db64e05ef57ff0dfae99cc
// object=G0035C872 name-evidence=invented
// Extents and edges are hypotheses; byte verification alone does not establish original TU boundaries.
// FUNCTION: BFME2 0x0035C872 size=231 name=?GadgetTextEntryValidateCharacter@@YA_NGC@Z evidence=N:ledger tier=B hypothesis:mm1-gap-unit
#include <ctype.h>
#include "Common/GameType.h"

bool GadgetTextEntryValidateCharacter(WideChar character, char restrictions)
{
	if ((character >= 0x0E01 && character <= 0x0E3A) || (character >= 0x0E3F && character <= 0x0E5B))
		return false;
	if (restrictions < 0 && character == L' ')
		return true;
	if ((restrictions & 0x01) && character == L' ')
		return false;
	if ((restrictions & 0x02) && character == L'%')
		return false;
	if (restrictions & 0x04)
	{
		if (character < L'"' || character > L'~' || character == L'\\')
			return false;
	}
	if (restrictions & 0x08)
	{
		if (character == L'*' || character == L'?' || character == L':' || character == L'\\' || character == L'/'
			|| character == L'"' || character == L'<' || character == L'>' || character == L'|')
			return false;
	}
	if ((restrictions & 0x10) && !iswascii(character))
		return false;
	if ((restrictions & 0x20) && !iswdigit(character))
		return false;
	if ((restrictions & 0x40) && !iswalnum(character))
		return false;
	return true;
}
