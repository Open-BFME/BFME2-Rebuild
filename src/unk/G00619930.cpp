// Generated from the ledger; map snapshot c591f7146f88c8a8248ebb8ae22f0c978c22aed463db64e05ef57ff0dfae99cc
// object=G00619930 name-evidence=invented
// Extents and edges are hypotheses; byte verification alone does not establish original TU boundaries.
#include <stddef.h>
namespace Msvcr71 {
	extern "C" __declspec(dllimport) __declspec(nothrow) size_t __cdecl strspn(const char *, const char *);
	extern "C" __declspec(dllimport) __declspec(nothrow) size_t __cdecl strcspn(const char *, const char *);
}
// FUNCTION: BFME2 0x00619930 size=117 name=?strtok_r@@YAPADPADPBDPAPAD@Z evidence=N:ledger tier=B hypothesis:mm1-gap-unit
char *strtok_r(char *str, const char *delim, char **save)
{
	if (str)
		*save = str;
	if (!**save)
		return 0;

	size_t length = Msvcr71::strcspn(*save, delim);
	if (length == 0) {
		*save += Msvcr71::strspn(*save, delim);
		if (!**save)
			return 0;
		length = Msvcr71::strcspn(*save, delim);
	}

	char *token = *save;
	char *end = token + length;
	if (!*end) {
		*save = end;
		return token;
	}
	*end = 0;
	*save += length + 1;
	return token;
}
