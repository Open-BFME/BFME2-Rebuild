// cl: /O2 /G6 /MD /D_CRTIMP=
// Generated from the ledger; map snapshot c591f7146f88c8a8248ebb8ae22f0c978c22aed463db64e05ef57ff0dfae99cc
// object=G006E6C00 name-evidence=invented
// Extents and edges are hypotheses; byte verification alone does not establish original TU boundaries.
#include "Apt/AptValue.h"
#include "Apt/AptValueVector.h"
#include "Apt/Assert.h"
extern void (__cdecl *g_bfmeAptAssertAtE17734)(const char *, const char *, int);
extern int g_bfmeAptBreakOnAssertAtDDC01C;
void __debugbreak();
// FUNCTION: BFME2 0x006E6C00 size=137 name=?PushValue@AptValueVector@@QAEXPAVAptValue@@@Z evidence=X:map request (link (from standalone link 0cf3e833)) tier=B hypothesis:mm1-gap-unit
void AptValueVector::PushValue(AptValue *pValue)
{
    char releaseAtEnd = pValue->IsReleaseAtEnd();
    if (!releaseAtEnd) {
        g_bfmeAptAssertAtE17734("pValue->IsReleaseAtEnd()", ".\\AptValue/AptValueVector.inl", 0x35);
        if (g_bfmeAptBreakOnAssertAtDDC01C) __debugbreak();
    }
    if (mCurrentNum >= field00) {
        pValue->ClearReleaseAtEnd();
        return;
    }
    if (pValue == 0) {
        g_bfmeAptAssertAtE17734("pValue != NULL", ".\\AptValue/AptValueVector.inl", 0x43);
        if (g_bfmeAptBreakOnAssertAtDDC01C) __debugbreak();
    }
    mpValues[mCurrentNum] = pValue;
    mCurrentNum++;
    if (mCurrentNum > field0C)
        field0C = mCurrentNum;
}
// FUNCTION: BFME2 0x006E6C90 size=56 name=?PopValue@AptValueVector@@QAEPAVAptValue@@XZ evidence=N:ledger tier=B hypothesis:mm1-gap-unit
// FUNCTION: BFME2 0x006E6CD0 size=17 name=?IsFull@AptValueVector@@QBEHXZ evidence=X:map request (standalone ca6e45a3dd) tier=B hypothesis:mm1-gap-unit
int AptValueVector::IsFull() const
{
	return mCurrentNum >= field00;
}
