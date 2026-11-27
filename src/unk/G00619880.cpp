// Generated from the ledger; map snapshot c591f7146f88c8a8248ebb8ae22f0c978c22aed463db64e05ef57ff0dfae99cc
// object=G00619880 name-evidence=invented
// Extents and edges are hypotheses; byte verification alone does not establish original TU boundaries.
#include <string.h>
#include "WWLib/nstrdup.h"
#include "WWLib/wwstring.h"
// FUNCTION: BFME2 0x00619880 size=68 name=?nstrdup@@YAPADPBD@Z evidence=N:ledger tier=B hypothesis:mm1-gap-unit
char *nstrdup(const char *str)
{
	if (str == 0)
		return 0;
	char *copy = new('str') char[strlen(str) + 1];
	strcpy(copy, str);
	return copy;
}
