// cl: /O2 /G7 /Oi
// Portions derived from Command & Conquer Generals Zero Hour, (c) Electronic Arts Inc., GPL-3.0-or-later; see LICENSE and NOTICE.
#include "WWLib/trim.h"
#include <string.h>
#include <wchar.h>
// Generated from the ledger; map snapshot c591f7146f88c8a8248ebb8ae22f0c978c22aed463db64e05ef57ff0dfae99cc
// object=G0061B600 name-evidence=invented
// Extents and edges are hypotheses; byte verification alone does not establish original TU boundaries.
// FUNCTION: BFME2 0x0061B600 size=105 name=?strtrim@@YAPADPAD@Z evidence=N:ledger tier=B hypothesis:mm1-gap-unit
char *strtrim(char *buffer)
{
	if (buffer != NULL)
	{
		char *source = buffer;
		while (*source != 0 && (unsigned char)*source <= 32)
			source++;
		if (source != buffer)
			strcpy(buffer, source);
		for (int index = (int)strlen(buffer) - 1; index >= 0; index--)
		{
			if (*source != 0 && (unsigned char)buffer[index] <= 32)
				buffer[index] = 0;
			else
				break;
		}
	}
	return buffer;
}
// FUNCTION: BFME2 0x0061B670 size=102 name=?wcstrim@@YAPAGPAG@Z evidence=N:ledger tier=B hypothesis:mm1-gap-unit
wchar_t *wcstrim(wchar_t *buffer)
{
	if (buffer != NULL)
	{
		wchar_t *source = buffer;
		while (*source != 0 && (unsigned short)*source <= 32)
			source++;
		if (source != buffer)
			wcscpy(buffer, source);
		for (int index = (int)wcslen(buffer) - 1; index >= 0; index--)
		{
			if (*source != 0 && (unsigned short)buffer[index] <= 32)
				buffer[index] = 0;
			else
				break;
		}
	}
	return buffer;
}
// FUNCTION: BFME2 0x0061B6E0 size=8 name=?XMP_Digits_To_Bits@@YAHH@Z evidence=N:ledger tier=B hypothesis:mm1-gap-unit
int XMP_Digits_To_Bits(int digits)
{
	return digits << 5;
}
// FUNCTION: BFME2 0x0061B6F0 size=17 name=?XMP_Bits_To_Digits@@YAHH@Z evidence=N:ledger tier=B hypothesis:mm1-gap-unit
int XMP_Bits_To_Digits(int bits)
{
	return (bits + 31) / 32;
}
