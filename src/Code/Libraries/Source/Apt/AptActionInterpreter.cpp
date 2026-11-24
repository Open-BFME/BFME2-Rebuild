// cl: /O2 /G6 /MD /D_CRTIMP=
// stlport46
#include <math.h>
#include <string.h>
#include <stdio.h>
#include "Apt/AptActionInterpreter.h"
#include "Apt/AptBasePtrStack.h"
#include "Apt/AptString.h"
#include "Apt/EAStringC.h"
// cl-arch: x87
void Rva006CC110Log(int level, const char *format, ...); // 0x006CC110 (the map names it Rva006CC110Log)
#include "Apt/AptSpriteInstBase.h"
#include "Apt/AptCharacterInst.h"
#include "Apt/AptActionSetup.h"
#include "Apt/AptLinker.h"
#include "Apt/AptPrototype.h"
#include "Apt/AptAnimationPoolData.h"
#include "Apt/AptDisplayListState.h"
#include "Apt/AptScriptFunction1.h"
#include "Apt/AptScriptFunction2.h"
#include "Apt/EAStringC.h"
#include "Apt/AptScriptFunctionBase.h"
#include "Apt/AptNativeFunction.h"
#include <math.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include "Apt/AptActionInterpreter.h"
#include "Apt/AptInteger.h"
#include "Apt/_AptBasePtrStack.h"
#include "Apt/AptInitParmsT.h"
#include "Apt/AptLookup.h"
#include "Apt/AptArray.h"
extern int g_nAptRecordFrame;	// 0x00E176F0 (Apt.cpp)
#include "Apt/AptInteger.h"
#include "Apt/AptBoolean.h"
#include "Apt/AptFloat.h"
#include "Apt/AptString.h"
#include "Apt/AptStringObject.h"
#include "Apt/AptObject.h"
#include "Apt/AptNativeHash.h"
#include "Apt/StringPool.h"
#include "Apt/AptCIH.h"
#include <math.h>
#include <string.h>
#include "Apt/AptActionInterpreter.h"
#include "Apt/AptValueVector.h"
#include "Apt/_AptBasePtrStack.h"
#include "Apt/AptInitParmsT.h"
#include "Apt/AptLookup.h"
extern AptValueVector *g_releaseVectorAtE17710;
extern AptValue *gpUndefinedValue;
extern AptValue *gpGlobalGlobalObject;
extern AptValue *g_00E180B0; // VA 0x00E180B0 (the map's name): stored by _constructBuiltInObjects 0x006DE8E0; setVariable 0x006FED00 compares the target with it
AptValue *gpUndefinedValue = 0;
AptValue *gpGlobalGlobalObject = 0;
#include "Apt/AptActionInterpreter.h"
#include "Apt/Assert.h"
#include "Apt/DogmaAllocator.h"
// The assert expansion in this file's own bodies (the file name is retail's build path).
#define APT_AI_ASSERT(text, line) \
    g_bfmeAptAssertAtE17734(text, "C:\\projects\\bfme2patch103\\bfme2\\Code\\Libraries\\Source\\Apt\\AptActionInterpreter.cpp", line); \
    if (g_bfmeAptBreakOnAssertAtDDC01C) APT_ASSERT_BREAK()
extern void *(__cdecl *g_pfnMemAlloc)(unsigned int size);        // VA 0x00E17728 (assert text gAptFuncs.pfnMemAlloc)
extern void (__cdecl *g_pfnMemFreeSize)(void *block, unsigned int size); // VA 0x00E17730 (gAptFuncs.pfnMemFreeSize)
extern void (__cdecl *g_pfnAptLog)(const char *format, ...); // VA 0x00E1773C (the host's log callback)
#include "Apt/AptAnimationPoolData.h"
#include "Apt/AptDisplayListState.h"
void aptConvertWith(AptValue *context, AptValue *with, AptValue *target, AptValue **result); // 0x007065E0 (the map files it as rva007065e0)
static inline AptValue *convertWith(AptValue *context, AptValue *with, AptValue *target)
{
    AptValue *result;
    aptConvertWith(context, with, target, &result);
    return result;
}
const char *g_pFSCommandPrefix = "FSCommand:"; // VA 0x00DDC920: the text an FSCommand getURL target starts with
void (__cdecl *g_pfnFSCommand)(const char *command, const char *args) = 0; // VA 0x00E17758 (the host callback)
extern AptValue *(__cdecl *g_pfnLoadVariables)(const char *url); // VA 0x00E1775C (gAptFuncs.pfnLoadVariables)
extern AptValue *(__cdecl *g_pfnLoadVariablesNULL)(); // VA 0x00E17760 (gAptFuncs.pfnLoadVariablesNULL)
extern void (__cdecl *g_pfnSetExtern)(const char *name, const char *value); // VA 0x00E17764: SetMember on an extern value
extern AptValue *(__cdecl *g_pfnGetExtern)(const char *name); // VA 0x00E17768: GetMember on an extern value
extern void (__cdecl *g_pfnUndefinedVariable)(const char *name); // VA 0x00E177C4: the host hook getVariable calls when a name resolves to nothing
unsigned char Rva006FD5B0(char a, char b); // 0x006FD5B0: one hex pair of a URL escape
unsigned char rva006FEC00(int context, int with, EAStringC *path, int *resolved, EAStringC *rest); // (the map files it as rva006FEC00): resolves a path string against a context and a with value, to the value and the unresolved rest
void rva006ffce0(AptValue *value, EAStringC &path); // The dotted target path of a clip
EAStringC rva006D5160(const char *prefix, const EAStringC &text); // (the map name): the C string followed by the string
void rva006ffc30(AptValue *value, EAStringC &path); // The slash form of the same path
void rva006ff980(AptValue *value, EAStringC &path, int dotted); // Appends a clip's ancestors and name to the path
bool rva006fc370(AptValue *value); // (placeholder in the map): a predicate on the value taken before the numeric conversions
extern EAStringC g_aptScratchString; // VA 0x00E177D4: the name the PushString* opcodes build before the lookup
int gaPropertyStringIndices[22] = { 23, 26, 25, 28, 2, 17, 1, 21, 22, 8, 14, 16, 6, 11, 4, 20, 9, 5, 15, 12, 24, 27 }; // VA 0x00DDC928: the StringPool index of each numbered MovieClip property name
int Rva006CD220Get(); // 0x006CD220: the Flash version the interpreter emulates (7)
unsigned int bfmeNext1221(); // 0x0070F830 (the map files it as bfmeNext1221): the pseudo-random generator behind `random`
// AptCIH.h's inline accessor (line 0x7d asserts isSpriteInstBase()), expanded in place by the frame actions.
static inline AptSpriteInstBase *spriteInstBaseOf(AptCIH *cih)
{
    if (!cih->isSpriteInstBase()) {
        g_bfmeAptAssertAtE17734("isSpriteInstBase()", "c:\\projects\\bfme2patch103\\bfme2\\code\\libraries\\source\\apt\\AptCIH.h", 0x7d);
        if (g_bfmeAptBreakOnAssertAtDDC01C) APT_ASSERT_BREAK();
    }
    return (AptSpriteInstBase *)cih->mpInstance;
}
// Generated by gen_repo.py; map snapshot 3a961c89bb620f8aa35d246db3d63678884d99ec6b44f1f8a27ad1a3b46b8d37
// Tier A; object=O006FC240 evidence=F flags=align=A;ehform=inline
// Extent is a hypothesis; only Object VERIFIED establishes recovery.
// Edges: left=unknown-rows right=unknown-rows
// FUNCTION: BFME2 0x006FBA20 size=36 name=?set@Rva006FBA20Holder@@QAEXPAVRva006FBA20Ref@@@Z evidence=?:placeholder

// FUNCTION: BFME2 0x006FBA50 size=32 name=?body@Rva008C44F0Body@@QAEXH@Z evidence=?:placeholder

// FUNCTION: BFME2 0x006FBA70 size=38 name=?set@Rva006FBA70Holder@@QAEXPAVRva006FBA20Ref@@@Z evidence=?:placeholder

// FUNCTION: BFME2 0x006FBB70 size=22 name=?rva006FBB70@Rva006FBB70@@QAE_NABVEAStringC@@@Z evidence=?:placeholder

// FUNCTION: BFME2 0x006FBBA0 size=64 name=?rva006FBBA0@AptActionInterpreter@@QAE_NPBVEAStringC@@PAVAptValue@@@Z evidence=P:provisional (wb-file)

// FUNCTION: BFME2 0x006FBBE0 size=43 name=?find@Rva006FBBE0@@QAEPAVBfmeN1034@@H@Z evidence=?:placeholder

// FUNCTION: BFME2 0x006FBC10 size=25 name=?rva006FBC10@AptActionInterpreter@@UAEXXZ evidence=P:provisional (wb-file)

// FUNCTION: BFME2 0x006FBC30 size=45 name=?rva006FBC30@AptActionInterpreter@@UAEXXZ evidence=P:provisional (wb-file)

// FUNCTION: BFME2 0x006FBCF0 size=9 name=?bit@Rva006FBCF0ByteBit@@QBEIXZ evidence=?:placeholder

// FUNCTION: BFME2 0x006FBD00 size=11 name=?bit@Rva006FBD00ByteBit@@QBEIXZ evidence=?:placeholder

// FUNCTION: BFME2 0x006FBD10 size=12 name=?bit@Rva006FBD10ByteBit@@QBEIXZ evidence=?:placeholder

// FUNCTION: BFME2 0x006FBD20 size=7 name=?addressBits@Rva006FBD20Address@@QBEIXZ evidence=?:placeholder

// FUNCTION: BFME2 0x006FBD30 size=10 name=?addressBits@Rva006FBD30Address@@QBEIXZ evidence=?:placeholder

// FUNCTION: BFME2 0x006FBDC0 size=11 name=?isHeld@Rva006FBDF0@@QBE_NXZ evidence=?:placeholder

// FUNCTION: BFME2 0x006FBDD0 size=22 name=?Hold@Rva006FBDF0@@QAEXPAVRva006FBDF0Iface@@@Z evidence=?:placeholder

// FUNCTION: BFME2 0x006FBDF0 size=20 name=?Release@Rva006FBDF0@@QAEXXZ evidence=?:placeholder

// FUNCTION: BFME2 0x006FBE10 size=65 name=?hasName@Rva006FBE10Scope@@QAE_NPBVEAStringC@@@Z evidence=?:placeholder

// FUNCTION: BFME2 0x006FBE60 size=29 name=?rva006FBE60@AptScriptFunctionBase@@QAE_NPBVEAStringC@@PAVAptValue@@@Z evidence=X:map request (link (from standalone link 0cf3e833))

// FUNCTION: BFME2 0x006FBE80 size=66 name=?rva006FBE80@AptScriptFunctionBase@@QAEPAVAptValue@@PBVEAStringC@@@Z evidence=X:map request (link (from standalone link 0cf3e833))

// FUNCTION: BFME2 0x006FBED0 size=150 name=?CreateFrameStack@AptScriptFunctionBase@@QAEXXZ evidence=X:map request (link (from standalone link 0cf3e833))

// FUNCTION: BFME2 0x006FBF70 size=22 name=?bfmeKeepFU@BfmeThingFU@@QAEXPAVBfmeItemFU@@@Z evidence=N:ledger

// FUNCTION: BFME2 0x006FC370 size=762 name=?rva006fc370@@YA_NPAVAptValue@@@Z evidence=?:placeholder
bool rva006fc370(AptValue *value)
{
    if (value->isInteger())
        return false;
    if (value->isFloat())
        return false;

    if (value->isString()) {
        EAStringC string;
        value->toString(string);

        if (string.GetLength() == 0)
            return true;

        if (string.GetAt(0) == '0' && (int)string.GetLength() > 2 && string.GetAt(1) == 'x') {
            char *end;
            strtol(string.c_str(), &end, 16);
            if (*end == '\0')
                return false;
        }

        bool decimalPointSeen = false;
        char last = (char)string.GetAt((int)string.GetLength() - 1);
        if (last != '-' && last != '+' && last != 'e' && last != '.' && !isdigit(last))
            return true;

        if (string.GetAt(0) != '.' && string.GetAt(0) != '-' && string.GetAt(0) != '+' && !isdigit(string.GetAt(0)))
            return true;

        for (int index = 1; index < (int)string.GetLength(); ++index) {
            if (string.GetAt(index) == '.') {
                if (!decimalPointSeen) {
                    decimalPointSeen = true;
                    continue;
                }
            }

            if (string.GetAt(index) == 'e' && index != 1) {
                if (index == 2) {
                    if (string.GetAt(0) == '+')
                        goto invalid_string;
                    if (string.GetAt(0) == '-')
                        goto invalid_string;
                }

                int next = index + 1;
                if (next < (int)string.GetLength()) {
                    char exponent = (char)string.GetAt(next);
                    if (exponent == '-' || exponent == '+') {
                        index = next;
                    } else {
                        if (!isdigit(exponent))
                            goto invalid_string;
                        index = next;
                    }
                }
                continue;
            }

            if (!isdigit(string.GetAt(index))) {
                goto invalid_string;
            }
        }

        return false;

    invalid_string:
        return true;
    } else if (value->isUndefined() || value->isNone()) {
        int version = Rva006CD220Get();
        if (version == 7)
            return true;
        return false;
    }

    return true;
}

// FUNCTION: BFME2 0x006FC670 size=2400 name=_BfmeAux006FC670 evidence=N:ledger

// FUNCTION: BFME2 0x006FD040 size=26 name=_Rva006FD040 evidence=?:placeholder
extern "C" void BfmeAux006FC670(const unsigned char *stream, int base, void *constants, int *index); // 0x006FC670
extern "C" void Rva006FD040(void *first, void *last, void *index)
{
    BfmeAux006FC670((const unsigned char *)first, (int)last, 0, (int *)index);
}

// FUNCTION: BFME2 0x006FD060 size=149 name=?Rva006FD060Forward@@YAXPAX000@Z evidence=?:placeholder
void Rva006FD060Forward(void *stream, void *base, void *constants, void *index)
{
    if (!stream) {
        APT_AI_ASSERT("aActionStream", 0x379);
    }
    if (!base) {
        APT_AI_ASSERT("pBase", 0x37a);
    }
    if (!constants) {
        APT_AI_ASSERT("aConstantFile", 0x37b);
    }
    BfmeAux006FC670((const unsigned char *)stream, (int)base, constants, (int *)index);
}

// FUNCTION: BFME2 0x006FD100 size=454 name=?rva006FD100@@YAEHHPAVEAStringC@@PAHPAD@Z evidence=?:placeholder
unsigned char rva006FD100(int current, int with, EAStringC *path, int *resolved, char *rest)
{
    const char *cursor;
    AptValue *context = (AptValue *)current;
    AptValue *next;
    bool isRooted = false;
    const char *text = path->c_str();
    rest[0] = 0;
    if (text[0] == '/') {
        context = g_pAptAnimationPoolData->mDisplayList.state->pHead->mpNext;
        cursor = text + 1;
        *resolved = (int)context;
        isRooted = true;
    } else {
        *resolved = current;
        cursor = text;
    }
    char segment[256];
    char *dest = segment;
    for (;;) {
        switch (*cursor) {
        case '.':
            if (cursor[1] == '.') {
                *dest++ = *cursor++;
                *dest++ = *cursor++;
                continue;
            }
            *dest = 0;
            {
                EAStringC part(segment);
                context = context->findChild(&part, (AptValue *)with);
            }
            with = 0;
            if (!context) {
                *resolved = 0;
                return isRooted;
            }
            ++cursor;
            dest = segment;
            break;
        case ':':
            *dest = 0;
            {
                EAStringC part(segment);
                next = context->findChild(&part, (AptValue *)with);
            }
            with = 0;
            if (next) {
                *resolved = (int)next;
                strcpy(rest, cursor + 1);
                return isRooted;
            }
            ++cursor;
            dest = segment;
            break;
        case 0:
            *dest = 0;
            *resolved = with ? with : (int)context;
            strcpy(rest, segment);
            return isRooted;
        default:
            *dest++ = *cursor++;
            break;
        }
    }
}

// FUNCTION: BFME2 0x006FD2D0 size=47 name=?callback006FD2D0@@YAPAVAptValue@@PAV1@H@Z evidence=N:ledger
AptValue *callback006FD2D0(AptValue *, int nParams)
{
    if (!(nParams <= 3)) {
        APT_AI_ASSERT("nParams <= 3", 0x66e);
    }
    return gpUndefinedValue;
}

// FUNCTION: BFME2 0x006FD300 size=49 name=?rva006FD300@AptActionInterpreter@@QAEPAXPAUAptActionSetup@@@Z evidence=?:placeholder
void *AptActionInterpreter::rva006FD300(AptActionSetup *)
{
    if (thrownValue) {
        APT_AI_ASSERT("!hasThrownValue()", 0x857);
    }
    return AptScriptFunctionBase::PushStaticData();
}

// FUNCTION: BFME2 0x006FD340 size=171 name=?rva006FD340@Rva006FD340Object@@QAEXPADPAX@Z evidence=?:placeholder
void AptActionInterpreter::CleanupAfterExecution(char *name, void *saveBase)
{
    AptValue *pending = thrownValue;
    if (pending) {
        EAStringC message;
        pending->toString(message);
        Rva006CC110Log(3, "<WARNING> Actionscript un-caught exception encountered during \"%s\"\n", name);
        Rva006CC110Log(3, "<WARNING> Actionscript error message: \"%s\"\n", message.c_str());
        thrownValue->Release();
        thrownValue = 0;
    }
    AptScriptFunctionBase::PopStaticData(saveBase);
}

// FUNCTION: BFME2 0x006FD3F0 size=203 name=?_concatAsStrings@@YAPAVAptString@@PAVAptValue@@0@Z evidence=N:ledger
static AptString *_concatAsStrings(AptValue *first, AptValue *second)
{
    AptString *result = AptString::Create();
    if (first->isString())
        result->mString = ((AptString *)first->checkedString())->mString;
    else
        first->toString(result->mString);
    if (second->isString()) {
        result->mString.Append(((AptString *)second->checkedString())->mString);
    } else {
        EAStringC secondText;
        second->toString(secondText);
        result->mString.Append(secondText);
    }
    return result;
}

// FUNCTION: BFME2 0x006FD4C0 size=233 name=?rva006FD4C0@@YAXPAVEAStringC@@@Z evidence=?:placeholder
void rva006FD4C0(EAStringC *pString)
{
    char cHex[6] = "";
    char cText[2] = "";
    EAStringC result;
    result.Truncate(pString->GetLength() * 3);
    const char *p = pString->c_str();
    cText[1] = 0;
    unsigned char c = *p++;
    while (c != 0) {
        if (c < 0x80 && isalnum(c)) {
            cText[0] = c;
            result.Append(cText);
        } else {
            sprintf(cHex, "%%%X", c);
            result.Append(cHex);
        }
        c = *p++;
    }
    *pString = result;
}

// FUNCTION: BFME2 0x006FD5B0 size=117 name=?Rva006FD5B0@@YAEDD@Z evidence=?:placeholder
unsigned char Rva006FD5B0(char a, char b)
{
    if (!isxdigit(a))
        return b;
    if (!isxdigit(b)) {
        APT_AI_ASSERT("isxdigit(b)", 0xbed);
    }
    char digits[3] = { a, b, 0 };
    return (unsigned char)strtoul(digits, 0, 16);
}

// FUNCTION: BFME2 0x006FD630 size=199 name=?rva006FD630@@YAXPAVEAStringC@@@Z evidence=?:placeholder
void rva006FD630(EAStringC *pString)
{
    char cText[2] = "*";
    EAStringC result;
    result.Truncate(pString->GetLength());
    const char *p = pString->c_str();
    char c = *p++;
    while (c != 0) {
        if (c == '+') {
            cText[0] = ' ';
        } else {
            if (c == '%') {
                if ((unsigned char)p[0] != 0) {
                    c = (char)Rva006FD5B0(p[0], p[1]);
                    p += 2;
                }
            }
            cText[0] = c;
        }
        result.Append(cText);
        c = *p++;
    }
    *pString = result;
}

// FUNCTION: BFME2 0x006FD700 size=133 name=?urlDecode@AptActionInterpreter@@AAEPBDPBDAAVEAStringC@@1@Z evidence=N:ledger
const char *AptActionInterpreter::urlDecode(const char *url, EAStringC &key, EAStringC &value)
{
    const char *cur = url;
    const char *equals = 0;
    key.rva006D3470();
    value.rva006D3470();
    for (; cur && *cur && *cur != '&'; ++cur)
        if (*cur == '=')
            equals = cur;
    if (equals) {
        key.Append(url, equals - url);
        rva006FD630(&key);
        ++equals;
        value.Append(equals, cur - equals);
        rva006FD630(&value);
        if (*cur == '&')
            ++cur;
    } else {
        cur = 0;
    }
    return cur;
}

// FUNCTION: BFME2 0x006FD790 size=50 name=?bfmeHasPrefix8C5680@@YGEPBD@Z evidence=N:ledger
unsigned char __stdcall bfmeHasPrefix8C5680(const char *text)
{
    return strncmp(text, g_pFSCommandPrefix, strlen(g_pFSCommandPrefix)) == 0;
}

// FUNCTION: BFME2 0x006FD7D0 size=121 name=?doFSCommand@AptActionInterpreter@@QAEHPBD0@Z evidence=N:ledger
int AptActionInterpreter::doFSCommand(const char *command, const char *args)
{
    if (!(strncmp(command, g_pFSCommandPrefix, strlen(g_pFSCommandPrefix)) == 0)) {
        APT_AI_ASSERT("isFSCommand(szCommand)", 0xc4b);
    }
    g_pfnFSCommand(command + strlen(g_pFSCommandPrefix), args);
    return 1;
}

// FUNCTION: BFME2 0x006FD850 size=35 name=?_FunctionAptActionEnd@AptActionInterpreter@@CAXQAU1@QAULocalContextT@1@@Z evidence=N:ledger
void AptActionInterpreter::_FunctionAptActionEnd(AptActionInterpreter *const, LocalContextT *const)
{
    if (!false) {
        g_bfmeAptAssertAtE17734("false", "C:\\projects\\bfme2patch103\\bfme2\\Code\\Libraries\\Source\\Apt\\AptActionInterpreter.cpp", 0xc6b);
        if (g_bfmeAptBreakOnAssertAtDDC01C) __debugbreak();
    }
}

// FUNCTION: BFME2 0x006FD880 size=35 name=?_FunctionAptActionToggleQuality@AptActionInterpreter@@CAXQAU1@QAULocalContextT@1@@Z evidence=N:ledger
void AptActionInterpreter::_FunctionAptActionToggleQuality(AptActionInterpreter *const, LocalContextT *const)
{
    if (!false) {
        g_bfmeAptAssertAtE17734("false && \" [APT] \\\"Toggle Quality\\\" is NOT supported. Please do not use this in your actionscript\\n\"", "C:\\projects\\bfme2patch103\\bfme2\\Code\\Libraries\\Source\\Apt\\AptActionInterpreter.cpp", 0xcf0);
        if (g_bfmeAptBreakOnAssertAtDDC01C) __debugbreak();
    }
}

// FUNCTION: BFME2 0x006FD8B0 size=43 name=?_FunctionAptActionStopDragMovie@AptActionInterpreter@@CAXQAU1@QAULocalContextT@1@@Z evidence=N:ledger
void AptActionInterpreter::_FunctionAptActionStopDragMovie(AptActionInterpreter *const interp, LocalContextT *const context)
{
    if (g_pAptAnimationPoolData->field44)
        g_pAptAnimationPoolData->field44->Release();
    g_pAptAnimationPoolData->field44 = gpUndefinedValue;
}

// FUNCTION: BFME2 0x006FD8E0 size=35 name=?_FunctionAptActionStringLessThan@AptActionInterpreter@@CAXQAU1@QAULocalContextT@1@@Z evidence=N:ledger
void AptActionInterpreter::_FunctionAptActionStringLessThan(AptActionInterpreter *const, LocalContextT *const)
{
    if (!false) {
        g_bfmeAptAssertAtE17734("false", "C:\\projects\\bfme2patch103\\bfme2\\Code\\Libraries\\Source\\Apt\\AptActionInterpreter.cpp", 0x1204);
        if (g_bfmeAptBreakOnAssertAtDDC01C) __debugbreak();
    }
}

// FUNCTION: BFME2 0x006FD910 size=35 name=?_FunctionAptActionMBLength@AptActionInterpreter@@CAXQAU1@QAULocalContextT@1@@Z evidence=N:ledger
void AptActionInterpreter::_FunctionAptActionMBLength(AptActionInterpreter *const, LocalContextT *const)
{
    if (!false) {
        g_bfmeAptAssertAtE17734("false", "C:\\projects\\bfme2patch103\\bfme2\\Code\\Libraries\\Source\\Apt\\AptActionInterpreter.cpp", 0x123d);
        if (g_bfmeAptBreakOnAssertAtDDC01C) __debugbreak();
    }
}

// FUNCTION: BFME2 0x006FD940 size=35 name=?_FunctionAptActionCharToAscii@AptActionInterpreter@@CAXQAU1@QAULocalContextT@1@@Z evidence=N:ledger
void AptActionInterpreter::_FunctionAptActionCharToAscii(AptActionInterpreter *const, LocalContextT *const)
{
    if (!false) {
        g_bfmeAptAssertAtE17734("false", "C:\\projects\\bfme2patch103\\bfme2\\Code\\Libraries\\Source\\Apt\\AptActionInterpreter.cpp", 0x125a);
        if (g_bfmeAptBreakOnAssertAtDDC01C) __debugbreak();
    }
}

// FUNCTION: BFME2 0x006FD970 size=35 name=?_FunctionAptActionMBSubString@AptActionInterpreter@@CAXQAU1@QAULocalContextT@1@@Z evidence=N:ledger
void AptActionInterpreter::_FunctionAptActionMBSubString(AptActionInterpreter *const, LocalContextT *const)
{
    if (!false) {
        g_bfmeAptAssertAtE17734("false", "C:\\projects\\bfme2patch103\\bfme2\\Code\\Libraries\\Source\\Apt\\AptActionInterpreter.cpp", 0x12b0);
        if (g_bfmeAptBreakOnAssertAtDDC01C) __debugbreak();
    }
}

// FUNCTION: BFME2 0x006FD9A0 size=35 name=?_FunctionAptActionMBCharToAscii@AptActionInterpreter@@CAXQAU1@QAULocalContextT@1@@Z evidence=N:ledger
void AptActionInterpreter::_FunctionAptActionMBCharToAscii(AptActionInterpreter *const, LocalContextT *const)
{
    if (!false) {
        g_bfmeAptAssertAtE17734("false", "C:\\projects\\bfme2patch103\\bfme2\\Code\\Libraries\\Source\\Apt\\AptActionInterpreter.cpp", 0x12ce);
        if (g_bfmeAptBreakOnAssertAtDDC01C) __debugbreak();
    }
}

// FUNCTION: BFME2 0x006FD9D0 size=35 name=?_FunctionAptActionMBAsciiToChar@AptActionInterpreter@@CAXQAU1@QAULocalContextT@1@@Z evidence=N:ledger
void AptActionInterpreter::_FunctionAptActionMBAsciiToChar(AptActionInterpreter *const, LocalContextT *const)
{
    if (!false) {
        g_bfmeAptAssertAtE17734("false", "C:\\projects\\bfme2patch103\\bfme2\\Code\\Libraries\\Source\\Apt\\AptActionInterpreter.cpp", 0x12eb);
        if (g_bfmeAptBreakOnAssertAtDDC01C) __debugbreak();
    }
}

// FUNCTION: BFME2 0x006FDA00 size=9 name=?_FunctionAptActionReturn@AptActionInterpreter@@CAXQAU1@QAULocalContextT@1@@Z evidence=N:ledger
void AptActionInterpreter::_FunctionAptActionReturn(AptActionInterpreter *const, LocalContextT *const context)
{
    context->bEncounteredReturn = true;
}

// FUNCTION: BFME2 0x006FDA10 size=35 name=?_FunctionAptActionBitURShift@AptActionInterpreter@@CAXQAU1@QAULocalContextT@1@@Z evidence=N:ledger
void AptActionInterpreter::_FunctionAptActionBitURShift(AptActionInterpreter *const, LocalContextT *const)
{
    if (!false) {
        g_bfmeAptAssertAtE17734("false", "C:\\projects\\bfme2patch103\\bfme2\\Code\\Libraries\\Source\\Apt\\AptActionInterpreter.cpp", 0x1b92);
        if (g_bfmeAptBreakOnAssertAtDDC01C) __debugbreak();
    }
}

// FUNCTION: BFME2 0x006FDA40 size=490 name=?_FunctionAptActionGetUrl@AptActionInterpreter@@CAXQAU1@QAULocalContextT@1@@Z evidence=N:ledger
void AptActionInterpreter::_FunctionAptActionGetUrl(AptActionInterpreter *const interp, LocalContextT *const context)
{
    const char *const *operand = (const char *const *)(((unsigned int)context->pInstruction + 3) & ~3u);
    context->pInstruction = (const unsigned char *)(operand + 2);
    const char *url = operand[0];
    if (strncmp(url, g_pFSCommandPrefix, strlen(g_pFSCommandPrefix)) == 0) {
        interp->doFSCommand(operand[0], operand[1]);
    } else {
        char buffer[260];
        strcpy(buffer, operand[0]);
        int length = strlen(buffer);
        if ((buffer[length - 1] == 'f' || buffer[length - 1] == 'F') && (buffer[length - 2] == 'w' || buffer[length - 2] == 'W')
            && (buffer[length - 3] == 's' || buffer[length - 3] == 'S') && buffer[length - 4] == '.') {
            buffer[length - 4] = 0;
            g_pAptLinker->Load(EAStringC(buffer), EAStringC(operand[1]));
        } else if (length == 0) {
            g_pAptLinker->Load(EAStringC(""), EAStringC(operand[1]));
        } else {
            Rva006CC110Log(4, "not loading non-swf file: '%s'\n", buffer);
        }
    }
}

// FUNCTION: BFME2 0x006FDC30 size=33 name=?_FunctionAptActionDefineDictionary@AptActionInterpreter@@CAXQAU1@QAULocalContextT@1@@Z evidence=N:ledger
void AptActionInterpreter::_FunctionAptActionDefineDictionary(AptActionInterpreter *const interp, LocalContextT *const context)
{
    const AptConstantPool *pool = (const AptConstantPool *)(((unsigned int)context->pInstruction + 3) & ~3u);
    context->pInstruction = (const unsigned char *)(pool + 1);
    interp->constantPool = *pool;
}

// FUNCTION: BFME2 0x006FDC60 size=218 name=?isObjectOfType@AptActionInterpreter@@SA_NPAVAptValue@@0@Z evidence=N:ledger
bool AptActionInterpreter::isObjectOfType(AptValue *pObject, AptValue *pInterface)
{
    bool bIsOfType = false;
    if (pObject->ContainsNativeHashVirtual() && pInterface->ContainsNativeHashVirtual()) {
        AptValue *pFuncPrototype = pInterface->GetNativeHashVirtual()->mpPrototype;
        if (!pFuncPrototype->isPrototype()) {
            APT_AI_ASSERT("pFuncPrototype->isPrototype()", 0x2443);
        }
        if (pObject->isCIH()) {
            AptNativeHash *pObjectHash = pObject->GetNativeHashVirtual();
            AptValue *pProto = pObjectHash->mp__proto__;
            while (pProto) {
                if (pProto == pFuncPrototype)
                    bIsOfType = true;
                AptNativeHash *pProtoHash = pProto->GetNativeHashVirtual();
                pProto = pProtoHash->mp__proto__;
            }
        } else {
            if (((AptObject *)pObject)->DoesImplementObject(pFuncPrototype))
                bIsOfType = true;
        }
    } else if (!pObject->isScriptFunction() && !pObject->isObject() && pObject->getVtblIndex() == pInterface->getVtblIndex())
        bIsOfType = true;
    return bIsOfType;
}

// FUNCTION: BFME2 0x006FDD40 size=110 name=??1AptBasePtrStack@@QAE@XZ evidence=N:ledger
AptBasePtrStack::~AptBasePtrStack()
{
    if (!(m_nElements == 0)) {
        g_bfmeAptAssertAtE17734("m_nElements == 0", "c:\\projects\\bfme2patch103\\bfme2\\code\\libraries\\source\\apt\\_AptBasePtrStack.h", 0x4e);
        if (g_bfmeAptBreakOnAssertAtDDC01C) APT_ASSERT_BREAK();
    }
    if (m_aElements) {
        if (!g_pfnMemFreeSize) {
        g_bfmeAptAssertAtE17734("gAptFuncs.pfnMemFreeSize", "c:\\projects\\bfme2patch103\\bfme2\\code\\libraries\\source\\apt\\_AptBasePtrStack.h", 0x54);
        if (g_bfmeAptBreakOnAssertAtDDC01C) APT_ASSERT_BREAK();
        }
        g_pDogmaPoolManager->freeBlock(m_aElements, m_nCapacity * 4);
    }
}

// FUNCTION: BFME2 0x006FDDB0 size=151 name=?rva006FDDB0@AptBasePtrStack@@QAEXH@Z evidence=?:placeholder
void AptBasePtrStack::rva006FDDB0(int nCapacity)
{
    if (!(m_nCapacity == 0)) {
        g_bfmeAptAssertAtE17734("m_nCapacity == 0", "c:\\projects\\bfme2patch103\\bfme2\\code\\libraries\\source\\apt\\_AptBasePtrStack.h", 0x5c);
        if (g_bfmeAptBreakOnAssertAtDDC01C) APT_ASSERT_BREAK();
    }
    m_nCapacity = nCapacity;
    if (!g_pfnMemAlloc) {
        g_bfmeAptAssertAtE17734("gAptFuncs.pfnMemAlloc", "c:\\projects\\bfme2patch103\\bfme2\\code\\libraries\\source\\apt\\_AptBasePtrStack.h", 0x5f);
        if (g_bfmeAptBreakOnAssertAtDDC01C) APT_ASSERT_BREAK();
    }
    m_aElements = (AptValue **)g_pDogmaPoolManager->allocBlock(nCapacity * 4);
    if (!(m_aElements != 0)) {
        g_bfmeAptAssertAtE17734("m_aElements != NULL", "c:\\projects\\bfme2patch103\\bfme2\\code\\libraries\\source\\apt\\_AptBasePtrStack.h", 0x63);
        if (g_bfmeAptBreakOnAssertAtDDC01C) APT_ASSERT_BREAK();
    }
}

// FUNCTION: BFME2 0x006FDE50 size=130 name=?rva006FDE50@AptBasePtrStack@@QAEXXZ evidence=?:placeholder
void AptBasePtrStack::rva006FDE50()
{
    if (!(m_nElements == 0)) {
        g_bfmeAptAssertAtE17734("m_nElements == 0", "c:\\projects\\bfme2patch103\\bfme2\\code\\libraries\\source\\apt\\_AptBasePtrStack.h", 0x68);
        if (g_bfmeAptBreakOnAssertAtDDC01C) APT_ASSERT_BREAK();
    }
    if (m_aElements) {
        if (!g_pfnMemFreeSize) {
        g_bfmeAptAssertAtE17734("gAptFuncs.pfnMemFreeSize", "c:\\projects\\bfme2patch103\\bfme2\\code\\libraries\\source\\apt\\_AptBasePtrStack.h", 0x6b);
        if (g_bfmeAptBreakOnAssertAtDDC01C) APT_ASSERT_BREAK();
        }
        g_pDogmaPoolManager->freeBlock(m_aElements, m_nCapacity * 4);
    }
    m_nCapacity = 0;
    m_nElements = 0;
    m_aElements = 0;
}

// FUNCTION: BFME2 0x006FDEE0 size=66 name=?rva006FDEE0@AptBasePtrStack@@QAEXPAVBfmeAptValue006DCD20@@@Z evidence=?:placeholder
void AptBasePtrStack::PushRaw(AptValue *pValue)
{
    if (!(m_nElements < m_nCapacity)) {
        g_bfmeAptAssertAtE17734("m_nElements < m_nCapacity", "c:\\projects\\bfme2patch103\\bfme2\\code\\libraries\\source\\apt\\_AptBasePtrStack.h", 0x80);
        if (g_bfmeAptBreakOnAssertAtDDC01C) APT_ASSERT_BREAK();
    }
    m_aElements[m_nElements] = pValue;
    m_nElements++;
    pValue->AddRef();
}

// FUNCTION: BFME2 0x006FDF30 size=62 name=?rva006FDF30@AptBasePtrStack@@QAEXPAVBfmeAptValue006DCD20@@@Z evidence=?:placeholder
void AptBasePtrStack::PushRawNoAddRef(AptValue *pValue)
{
    if (!(m_nElements < m_nCapacity)) {
        g_bfmeAptAssertAtE17734("m_nElements < m_nCapacity", "c:\\projects\\bfme2patch103\\bfme2\\code\\libraries\\source\\apt\\_AptBasePtrStack.h", 0x90);
        if (g_bfmeAptBreakOnAssertAtDDC01C) APT_ASSERT_BREAK();
    }
    m_aElements[m_nElements] = pValue;
    m_nElements++;
}

// FUNCTION: BFME2 0x006FDF70 size=45 name=?PopNoDec@AptBasePtrStack@@QAEXXZ evidence=N:ledger

// FUNCTION: BFME2 0x006FDFA0 size=162 name=?PopAndPush@AptBasePtrStack@@QAEXHPAVBfmeAptValue006DCD20@@@Z evidence=N:ledger
void AptBasePtrStack::PopAndPush(int nItems, AptValue *pValue)
{
    if (!(nItems >= 0)) {
        g_bfmeAptAssertAtE17734("nItems >= 0", "c:\\projects\\bfme2patch103\\bfme2\\code\\libraries\\source\\apt\\_AptBasePtrStack.h", 0xc9);
        if (g_bfmeAptBreakOnAssertAtDDC01C) APT_ASSERT_BREAK();
    }
    if (!(m_nElements >= nItems)) {
        g_bfmeAptAssertAtE17734("false && \"[APT] Error, Popping more elements than the stack contains. Please contact the Apt Team for Support.\"", "c:\\projects\\bfme2patch103\\bfme2\\code\\libraries\\source\\apt\\_AptBasePtrStack.h", 0xcd);
        if (g_bfmeAptBreakOnAssertAtDDC01C) APT_ASSERT_BREAK();
        return;
    }
    pValue->AddRef();
    for (int i = 1; i <= nItems; i++)
        m_aElements[m_nElements - i]->Release();
    m_aElements[m_nElements - nItems] = pValue;
    m_nElements = m_nElements + (1 - nItems);
}

// FUNCTION: BFME2 0x006FE050 size=93 name=?rva006FE050@AptBasePtrStack@@QAEXH@Z evidence=?:placeholder
void AptBasePtrStack::PopN(int nItems)
{
    if (nItems > 0) {
        if (!(m_nElements >= nItems)) {
            g_bfmeAptAssertAtE17734("false && \"[APT] Error, Popping more elements than the stack contains. Please contact the Apt Team for Support.\"", "c:\\projects\\bfme2patch103\\bfme2\\code\\libraries\\source\\apt\\_AptBasePtrStack.h", 0xf0);
            if (g_bfmeAptBreakOnAssertAtDDC01C) APT_ASSERT_BREAK();
            return;
        }
        for (int i = 1; i <= nItems; i++)
            m_aElements[m_nElements - i]->Release();
        m_nElements -= nItems;
    }
}

// FUNCTION: BFME2 0x006FE0B0 size=105 name=?rva006FE0B0@AptBasePtrStack@@QAEXH@Z evidence=?:placeholder
void AptBasePtrStack::rva006FE0B0(int nCapacity)
{
    if (!(m_nCapacity == 0)) {
        g_bfmeAptAssertAtE17734("m_nSize == 0", "c:\\projects\\bfme2patch103\\bfme2\\code\\libraries\\source\\apt\\_AptValuePtrStack.h", 0x59);
        if (g_bfmeAptBreakOnAssertAtDDC01C) APT_ASSERT_BREAK();
    }
    m_nCapacity = nCapacity;
    m_aElements = (AptValue **)g_pDogmaPoolManager->allocBlock(nCapacity * 4);
    if (!(m_aElements != 0)) {
        g_bfmeAptAssertAtE17734("m_aElements != NULL", "c:\\projects\\bfme2patch103\\bfme2\\code\\libraries\\source\\apt\\_AptValuePtrStack.h", 0x5e);
        if (g_bfmeAptBreakOnAssertAtDDC01C) APT_ASSERT_BREAK();
    }
}

// FUNCTION: BFME2 0x006FE120 size=51 name=?rva006FE120@AptBasePtrStack@@QAEXXZ evidence=?:placeholder
void AptBasePtrStack::rva006FE120()
{
    if (m_aElements)
        g_pDogmaPoolManager->freeBlock(m_aElements, m_nCapacity * 4);
    m_nCapacity = 0;
    m_nElements = 0;
    m_aElements = 0;
}

// FUNCTION: BFME2 0x006FE360 size=28 name=?setName@Rva006FBE10Scope@@QAEXPBVEAStringC@@PAVAptValue@@@Z evidence=?:placeholder
void AptScriptFunctionBase::SetLocalVariable(const EAStringC *name, AptValue *value)
{
    if (!spFrameStack)
        CreateFrameStack();
    spFrameStack->GetNativeHash()->Set(name, value);
}

// FUNCTION: BFME2 0x006FE400 size=42 name=?rva006FE400@Rva006FE400@@QAEHPAXPAURva008A4570Owner@@@Z evidence=?:placeholder
AptValue *AptStringObject::objectMemberLookup(AptValue *const self, const EAStringC *const name) const
{
    AptValue *found = mpString->objectMemberLookup(self, name);
    if (!found)
        found = AptObject::objectMemberLookup(self, name);
    return found;
}

// FUNCTION: BFME2 0x006FE580 size=66 name=?At@AptBasePtrStack@@QAEPAVBfmeAptValue006DCD20@@H@Z evidence=N:ledger

// FUNCTION: BFME2 0x006FE5D0 size=55 name=?rva006FE5D0@AptBasePtrStack@@QAEPAVBfmeAptValue006DCD20@@XZ evidence=?:placeholder
AptValue *AptBasePtrStack::PopReturn()
{
    if (!(GetSize() >= 1)) {
        g_bfmeAptAssertAtE17734("GetSize() >= 1", "c:\\projects\\bfme2patch103\\bfme2\\code\\libraries\\source\\apt\\_AptBasePtrStack.h", 0x110);
        if (g_bfmeAptBreakOnAssertAtDDC01C) APT_ASSERT_BREAK();
    }
    return m_aElements[--m_nElements];
}

// FUNCTION: BFME2 0x006FE610 size=199 name=?stackPushIndirect@AptActionInterpreter@@QAEXQAVAptValue@@@Z evidence=N:ledger
void AptActionInterpreter::stackPushIndirect(AptValue *const pValue)
{
    if (!pValue) {
        g_bfmeAptAssertAtE17734("pValue", "c:\\projects\\bfme2patch103\\bfme2\\code\\libraries\\source\\apt\\AptActionInterpreter.inl", 0x5e);
        if (g_bfmeAptBreakOnAssertAtDDC01C) APT_ASSERT_BREAK();
    }
    AptValue *pPushValue;
    if (pValue->isLookup()) {
        pPushValue = constantPool.apItems[((AptLookup *)pValue->checkedLookup())->nLookup];
    } else if (pValue->isRegister()) {
        int iRegNum = ((AptRegister *)pValue->checkedRegister())->nVal;
        if (iRegNum < 0) {
            g_bfmeAptAssertAtE17734("iRegNum >= 0", "c:\\projects\\bfme2patch103\\bfme2\\code\\libraries\\source\\apt\\AptActionInterpreter.inl", 0x68);
            if (g_bfmeAptBreakOnAssertAtDDC01C) APT_ASSERT_BREAK();
        }
        pPushValue = AptScriptFunctionBase::GetRegisterValue(iRegNum);
    } else {
        pPushValue = pValue;
    }
    if (!(stack.m_nElements < stack.m_nCapacity)) {
        g_bfmeAptAssertAtE17734("m_nElements < m_nCapacity", "c:\\projects\\bfme2patch103\\bfme2\\code\\libraries\\source\\apt\\_AptBasePtrStack.h", 0x80);
        if (g_bfmeAptBreakOnAssertAtDDC01C) APT_ASSERT_BREAK();
    }
    stack.m_aElements[stack.m_nElements] = pPushValue;
    stack.m_nElements++;
    pPushValue->AddRef();
}

// FUNCTION: BFME2 0x006FE6E0 size=198 name=?Push@AptBasePtrStack@@QAEXPAVBfmeAptValue006DCD20@@@Z evidence=N:ledger
void AptBasePtrStack::Push(AptValue *pValue)
{
    if (!(pValue)) {
        g_bfmeAptAssertAtE17734("pValue", "c:\\projects\\bfme2patch103\\bfme2\\code\\libraries\\source\\apt\\AptActionInterpreter.inl", 0x85);
        if (g_bfmeAptBreakOnAssertAtDDC01C) APT_ASSERT_BREAK();
    }
    if (pValue->isLookup()) {
        g_bfmeAptAssertAtE17734("pValue->isLookup() == false", "c:\\projects\\bfme2patch103\\bfme2\\code\\libraries\\source\\apt\\AptActionInterpreter.inl", 0x86);
        if (g_bfmeAptBreakOnAssertAtDDC01C) APT_ASSERT_BREAK();
    }
    if (pValue->isRegister()) {
        g_bfmeAptAssertAtE17734("pValue->isRegister() == false", "c:\\projects\\bfme2patch103\\bfme2\\code\\libraries\\source\\apt\\AptActionInterpreter.inl", 0x87);
        if (g_bfmeAptBreakOnAssertAtDDC01C) APT_ASSERT_BREAK();
    }
    if (!(m_nElements < m_nCapacity)) {
        g_bfmeAptAssertAtE17734("m_nElements < m_nCapacity", "c:\\projects\\bfme2patch103\\bfme2\\code\\libraries\\source\\apt\\_AptBasePtrStack.h", 0x80);
        if (g_bfmeAptBreakOnAssertAtDDC01C) APT_ASSERT_BREAK();
    }
    m_aElements[m_nElements] = pValue;
    m_nElements++;
    pValue->AddRef();
}

// FUNCTION: BFME2 0x006FE7B0 size=195 name=?rva006FE7B0@AptBasePtrStack@@QAEXPAVBfmeAptValue006DCD20@@@Z evidence=?:placeholder
void AptBasePtrStack::PushNoAddRef(AptValue *pValue)
{
    if (!(pValue)) {
        g_bfmeAptAssertAtE17734("pValue", "c:\\projects\\bfme2patch103\\bfme2\\code\\libraries\\source\\apt\\AptActionInterpreter.inl", 0x9c);
        if (g_bfmeAptBreakOnAssertAtDDC01C) APT_ASSERT_BREAK();
    }
    if (pValue->isLookup()) {
        g_bfmeAptAssertAtE17734("pValue->isLookup() == false", "c:\\projects\\bfme2patch103\\bfme2\\code\\libraries\\source\\apt\\AptActionInterpreter.inl", 0x9d);
        if (g_bfmeAptBreakOnAssertAtDDC01C) APT_ASSERT_BREAK();
    }
    if (pValue->isRegister()) {
        g_bfmeAptAssertAtE17734("pValue->isRegister() == false", "c:\\projects\\bfme2patch103\\bfme2\\code\\libraries\\source\\apt\\AptActionInterpreter.inl", 0x9e);
        if (g_bfmeAptBreakOnAssertAtDDC01C) APT_ASSERT_BREAK();
    }
    if (!(m_nElements < m_nCapacity)) {
        g_bfmeAptAssertAtE17734("m_nElements < m_nCapacity", "c:\\projects\\bfme2patch103\\bfme2\\code\\libraries\\source\\apt\\_AptBasePtrStack.h", 0x90);
        if (g_bfmeAptBreakOnAssertAtDDC01C) APT_ASSERT_BREAK();
    }
    m_aElements[m_nElements] = pValue;
    m_nElements++;
}

// FUNCTION: BFME2 0x006FE880 size=154 name=?rva006FE880@AptBasePtrStack@@QAEXHPAVBfmeAptValue006DCD20@@@Z evidence=?:placeholder
void AptBasePtrStack::PopAndPushValue(int nItems, AptValue *pValue)
{
    if (!(pValue)) {
        g_bfmeAptAssertAtE17734("pValue", "c:\\projects\\bfme2patch103\\bfme2\\code\\libraries\\source\\apt\\AptActionInterpreter.inl", 0xc8);
        if (g_bfmeAptBreakOnAssertAtDDC01C) APT_ASSERT_BREAK();
    }
    if (pValue->isLookup()) {
        g_bfmeAptAssertAtE17734("pValue->isLookup() == false", "c:\\projects\\bfme2patch103\\bfme2\\code\\libraries\\source\\apt\\AptActionInterpreter.inl", 0xc9);
        if (g_bfmeAptBreakOnAssertAtDDC01C) APT_ASSERT_BREAK();
    }
    if (pValue->isRegister()) {
        g_bfmeAptAssertAtE17734("pValue->isRegister() == false", "c:\\projects\\bfme2patch103\\bfme2\\code\\libraries\\source\\apt\\AptActionInterpreter.inl", 0xca);
        if (g_bfmeAptBreakOnAssertAtDDC01C) APT_ASSERT_BREAK();
    }
    PopAndPush(nItems, pValue);
}

// FUNCTION: BFME2 0x006FE920 size=61 name=?rva006FE920@AptBasePtrStack@@QAEXXZ evidence=?:placeholder

// FUNCTION: BFME2 0x006FE980 size=49 name=??0AptActionInterpreter@@QAE@XZ evidence=N:ledger
AptActionInterpreter::AptActionInterpreter()
{
}

// FUNCTION: BFME2 0x006FE9C0 size=298 name=??1AptActionInterpreter@@QAE@XZ evidence=N:ledger
AptActionInterpreter::~AptActionInterpreter()
{
}

// FUNCTION: BFME2 0x006FEAF0 size=87 name=?rva006FEAF0@AptActionInterpreter@@QAEXABUAptInitParmsT@@@Z evidence=?:placeholder
void AptActionInterpreter::rva006FEAF0(const AptInitParmsT &parms)
{
    stack.rva006FDDB0(parms.nValueStackSize);
    ((AptBasePtrStack *)&withStack)->rva006FE0B0(parms.nScopeStackSize);
    ((AptBasePtrStack *)&setTargetStack)->rva006FE0B0(parms.nScopeStackSize);
    ((AptBasePtrStack *)&thisStack)->rva006FE0B0(parms.nScopeStackSize);
    flag68 = 0;
    stackFrameBase = 0;
    debugCallStack.Rva006FE1D0(parms.nScopeStackSize);
    AptScriptFunctionBase::InitializeStaticData(parms);
}

// FUNCTION: BFME2 0x006FEB50 size=171 name=?rva006FEB50@AptActionInterpreter@@QAEXXZ evidence=?:placeholder
void AptActionInterpreter::rva006FEB50()
{
    stack.rva006FDE50();
    ((AptBasePtrStack *)&withStack)->rva006FE120();
    ((AptBasePtrStack *)&setTargetStack)->rva006FE120();
    ((AptBasePtrStack *)&thisStack)->rva006FE120();
    AptScriptFunctionBase::ShutdownStaticData();
    debugCallStack.Shutdown();
    if (stackFrameBase != 0) {
        APT_AI_ASSERT("mnStackFrameBase == 0", 0x10c);
    }
}

// FUNCTION: BFME2 0x006FEC00 size=251 name=?rva006FEC00@@YAEHHPAVEAStringC@@PAH0@Z evidence=?:placeholder
unsigned char rva006FEC00(int current, int strict, EAStringC *path, int *resolved, EAStringC *rest)
{
    const char *scan = path->c_str();
    char ch = *scan++;
    bool allDigits = true;
    while (ch) {
        if (ch < '0') {
            allDigits = false;
            break;
        }
        if (ch == ':') {
            allDigits = false;
            break;
        }
        ch = *scan++;
    }
    if (allDigits && !strict) {
        *rest = *path;
        *resolved = current;
        return 0;
    }
    char buffer[0x100];
    unsigned char rooted = rva006FD100(current, strict, path, resolved, buffer);
    EAStringC tmp(buffer);
    *rest = tmp;
    return rooted;
}

// FUNCTION: BFME2 0x006FED00 size=739 name=?setVariable@AptActionInterpreter@@QAE_NPAVAptValue@@0PBVEAStringC@@0HHH@Z evidence=N:ledger
bool AptActionInterpreter::setVariable(AptValue *context, AptValue *with, const EAStringC *sourceName, AptValue *value, int localFrameMode, int searchCurrentFrame, int directContext)
{
    EAStringC name;
    AptValue *resolvedContext;
    if (directContext == 0) {
        AptValue *found;
        rva006FEC00((int)context, (int)with, (EAStringC *)sourceName, (int *)&found, &name);
        resolvedContext = found;
    } else {
        resolvedContext = context;
        name = *sourceName;
    }
    if (!resolvedContext)
        return false;
    if (resolvedContext->isCIH(false) && (char)resolvedContext->checkedCIH(false)->rva006E0260())
        return false;
    if (!resolvedContext->isUndefined() && resolvedContext->objectMemberSet(resolvedContext, &name, value))
        return true;
    if (localFrameMode) {
        if (searchCurrentFrame && currentFunction && currentFunction->rva006FBE60(&name, value))
            return true;
        AptNativeHash *hash = resolvedContext->GetNativeHashVirtual();
        if (hash) {
            hash->Set(&name, value);
            hash->UpdateEventHandlers(resolvedContext, &name, !value || value->isUndefined());
            if (resolvedContext == g_00E180B0 && value->isScriptFunction()) {
                AptNativeHash *functionHash = value->GetNativeHashVirtual();
                AptNativeHash *prototypeHash = functionHash->mpPrototype->GetNativeHashVirtual();
                if (prototypeHash->mp__proto__)
                    prototypeHash->mp__proto__->Release();
                prototypeHash->mp__proto__ = 0;
            }
        } else if (!resolvedContext->isCIH(false) && currentFunction && !directContext) {
            AptNativeHash *parentHash = currentFunction->mpParentAnim->GetNativeHashVirtual();
            if (parentHash)
                parentHash->Set(&name, value);
        } else if (resolvedContext->isCIH(false)) {
            if ((char)((AptCIH *)resolvedContext->checkedCIH(false))->rva006DCC60(false)) {
                APT_AI_ASSERT("false && \"SHOULD NOT GET HERE ANYMORE\"", 0x4cc);
                AptValue *owner = ((AptCIH *)resolvedContext->checkedCIH(false))->mpParent;
                void *ownerHash = owner->rva006E04A0();
                if (ownerHash)
                    ((AptNativeHash *)ownerHash)->Set(&name, value);
            }
        }
    } else if (currentFunction) {
        if (!AptScriptFunctionBase::spFrameStack)
            currentFunction->CreateFrameStack();
        AptScriptFunctionBase::SetScopeVariable(&name, value);
    } else {
        AptNativeHash *hash = resolvedContext->GetNativeHashVirtual();
        if (hash) {
            hash->Set(&name, value);
            hash->UpdateEventHandlers(resolvedContext, &name, !value || value->isUndefined());
        }
    }
    return true;
}

// FUNCTION: BFME2 0x006FEFF0 size=1034 name=?callback006FEFF0@@YAPAVAptValue@@PAV1@H@Z evidence=N:ledger
AptValue *callback006FEFF0(AptValue *, int nParams)
{
    int firstParam = 2;
    if (!(nParams >= 2)) {
        APT_AI_ASSERT("nParams >= 2", 0x4ef);
    }
    AptValue *callback = g_aptNativeInterpreter.stack.At(0);
    AptValue *delay = g_aptNativeInterpreter.stack.At(1);
    if (callback->isUndefined())
        return AptInteger::Create(0);
    int i;
    for (i = 0; i < g_pAptAnimationPoolData->mnMaxIntervalTimers; ++i) {
        if (!g_pAptAnimationPoolData->maIntervalTimers[i].active) {
            ++g_pAptAnimationPoolData->mnIntervalTimers;
            if (!(g_pAptAnimationPoolData->mnIntervalTimers <= g_pAptAnimationPoolData->mnMaxIntervalTimers)) {
                APT_AI_ASSERT("gpPool->nIntervalTimers <= gpPool->GetMaxIntervalTimers()", 0x4fd);
            }
            g_pAptAnimationPoolData->maIntervalTimers[i].active = 1;
            g_pAptAnimationPoolData->maIntervalTimers[i].target = gpUndefinedValue;
            if (!callback->isScriptFunction() && !callback->isNativeFunction()) {
                if (!(callback->isObject() || (char)((AptCIH *)callback->checkedCIH(false))->rva006E01A0(false) || (char)((AptCIH *)callback->checkedCIH(false))->rva006CBEE0(false))) {
                    APT_AI_ASSERT("pCBValue->isObject() || pCBValue->c_cih()->isSpriteInst() || pCBValue->c_cih()->isAnimationInst()", 0x509);
                }
                AptValue *scope = callback;
                delay = g_aptNativeInterpreter.stack.At(2);
                callback = callback->findChild(&((AptString *)g_aptNativeInterpreter.stack.At(1))->mString, 0);
                if (!callback) {
                    APT_AI_ASSERT("pCBValue", 0x512);
                }
                firstParam = 3;
                if (!(callback->isScriptFunction() && !callback->isNativeFunction())) {
                    APT_AI_ASSERT("pCBValue->isScriptFunction() && !pCBValue->isNativeFunction()", 0x51a);
                }
                g_pAptAnimationPoolData->maIntervalTimers[i].target = scope;
            }
            g_pAptAnimationPoolData->maIntervalTimers[i].func = callback;
            g_pAptAnimationPoolData->maIntervalTimers[i].func->AddRef();
            g_pAptAnimationPoolData->maIntervalTimers[i].interval = delay->toFloat();
            g_pAptAnimationPoolData->maIntervalTimers[i].remaining = g_pAptAnimationPoolData->maIntervalTimers[i].interval;
            if (nParams > firstParam) {
                for (int remaining = nParams - firstParam; remaining > 0; --remaining) {
                    AptValue *argument = g_aptNativeInterpreter.stack.At(firstParam);
                    g_pAptAnimationPoolData->maIntervalTimers[i].params.push(argument);
                    ++firstParam;
                }
            }
            break;
        }
    }
    if (!(i != g_pAptAnimationPoolData->mnMaxIntervalTimers)) {
        APT_AI_ASSERT("i != gpPool->GetMaxIntervalTimers()", 0x535);
    }
    return AptInteger::Create(i);
}

// FUNCTION: BFME2 0x006FF400 size=256 name=?callback006FF400@@YAPAVAptValue@@PAV1@H@Z evidence=N:ledger
AptValue *callback006FF400(AptValue *, int)
{
    AptValue *value = g_aptNativeInterpreter.stack.At(0);
    if (!value->isUndefined()) {
        int index = value->toInteger();
        if (!(index >= 0 && index < g_pAptAnimationPoolData->mnMaxIntervalTimers)) {
            APT_AI_ASSERT("nIndex >= 0 && nIndex < gpPool->GetMaxIntervalTimers()", 0x544);
        }
        if (g_pAptAnimationPoolData->maIntervalTimers[index].active) {
            g_pAptAnimationPoolData->maIntervalTimers[index].func->Release();
            g_pAptAnimationPoolData->maIntervalTimers[index].active = 0;
            g_pAptAnimationPoolData->maIntervalTimers[index].cleanParams();
            if (!(g_pAptAnimationPoolData->mnIntervalTimers > 0)) {
                APT_AI_ASSERT("gpPool->nIntervalTimers > 0", 0x54b);
            }
            --g_pAptAnimationPoolData->mnIntervalTimers;
        }
    }
    return gpUndefinedValue;
}

// FUNCTION: BFME2 0x006FF500 size=198 name=?Rva006FF500@@YAPAVAptValue@@PAV1@H@Z evidence=?:placeholder
// MovieClip.hitTest(x, y [, shapeFlag]): whether the point lies inside the clip's bounds (undefined for a lone argument).
AptValue *aptHitTest(AptValue *self, int nParams)
{
    AptValue *undefined = gpUndefinedValue;
    if (nParams == 1)
        return undefined;
    if (nParams > 1) {
        float pointX = g_aptNativeInterpreter.stack.At(0)->toFloat();
        float pointY = g_aptNativeInterpreter.stack.At(1)->toFloat();
        if (nParams > 2)
            g_aptNativeInterpreter.stack.At(2)->toInteger();
        float rect[4];
        static_cast<AptCIH *>(self)->rva006E1DD0(rect);
        if (pointX >= rect[0] && pointX <= rect[2] && pointY >= rect[1] && pointY <= rect[3])
            return AptInteger::Create(1);
    }
    return undefined;
}

// FUNCTION: BFME2 0x006FF5D0 size=134 name=?rva006ff5d0@@YAPAVAptValue@@PAUAptActionInterpreter@@H@Z evidence=?:placeholder
AptValue *rva006ff5d0(AptActionInterpreter *pInterpreter, int nParams)
{
    if (!(nParams <= 1)) {
        APT_AI_ASSERT("nParams <= 1", 0x57e);
    }
    if (nParams == 0)
        return AptBoolean::Create(true);
    return AptBoolean::Create(rva006fc370(g_aptNativeInterpreter.stack.At(0)));
}

// FUNCTION: BFME2 0x006FF660 size=231 name=?callback006FF660@@YAPAVAptValue@@PAV1@H@Z evidence=N:ledger
void rva006FD630(EAStringC *text);
AptValue *callback006FF660(AptValue *, int nParams)
{
    if (!(nParams <= 1)) {
        APT_AI_ASSERT("nParams <= 1", 0x58f);
    }
    AptString *result = AptString::Create();
    AptValue *value = g_aptNativeInterpreter.stack.At(0);
    if (value->isString()) {
        EAStringC temp;
        value->toString(temp);
        rva006FD630(&temp);
        result->mString = temp;
    }
    return result;
}

// FUNCTION: BFME2 0x006FF750 size=241 name=?callback006FF750@@YAPAVAptValue@@PAV1@H@Z evidence=N:ledger
void rva006FD4C0(EAStringC *text);
AptValue *callback006FF750(AptValue *, int nParams)
{
    if (!(nParams <= 1)) {
        APT_AI_ASSERT("nParams <= 1", 0x5a7);
    }
    AptString *result = AptString::Create();
    if (!nParams)
        return result;
    AptValue *value = g_aptNativeInterpreter.stack.At(0);
    if (value->isString()) {
        EAStringC temp;
        value->toString(temp);
        rva006FD4C0(&temp);
        result->mString.Append(temp);
    }
    return result;
}

// FUNCTION: BFME2 0x006FF850 size=302 name=?rva006ff850@@YAPAVAptValue@@PAUAptActionInterpreter@@H@Z evidence=?:placeholder
AptValue *rva006ff850(AptActionInterpreter *, int nParams)
{
    if (!(nParams <= 1)) {
        APT_AI_ASSERT("nParams <= 1", 0x5c1);
    }
    if (nParams == 0)
        return gpUndefinedValue;
    AptValue *value = g_aptNativeInterpreter.stack.At(0);
    if (value->isCIH(false) || value->isObject())
        return AptBoolean::Create(true);
    if (value != gpUndefinedValue) {
        if (value->isFloat() || value->isInteger()) {
            if (value->isBoolean())
                return AptBoolean::Create(((AptBoolean *)value->checkedBoolean())->GetBool());
            if (value->isFloat() || value->isInteger()) {
                if (value->toFloat() != 0.0f)
                    return AptBoolean::Create(true);
                return AptBoolean::Create(false);
            }
            return AptBoolean::Create(false);
        } else {
            if (!rva006fc370(value)) {
                if (value->toFloat() != 0.0f)
                    return AptBoolean::Create(true);
                return AptBoolean::Create(false);
            }
            return AptBoolean::Create(false);
        }
    }
    return AptBoolean::Create(false);
}

// FUNCTION: BFME2 0x006FF980 size=682 name=?rva006ff980@@YAXPAVAptValue@@AAVEAStringC@@H@Z evidence=?:placeholder
void rva006ff980(AptValue *value, EAStringC &path, int dotted)
{
    AptCIH *context = (AptCIH *)value;
    if (!(context->isSpriteInstBase() || context->isButtonInst() || context->isLevelInst() || context->rva006E02B0())) {
        APT_AI_ASSERT("pContext->isSpriteInstBase() || pContext->isButtonInst() || pContext->isLevelInst() || pContext->isTextInst()", 0x677);
    }
    const char *separator = !dotted ? "/" : ".";
    AptCIH *parent = context->mpParent;
    if (!parent) {
        if (!(context->isSpriteInstBase() || context->isButtonInst() || context->isLevelInst() || context->rva006E02B0())) {
            APT_AI_ASSERT("pContext->isSpriteInstBase() || pContext->isButtonInst() || pContext->isLevelInst() || pContext->isTextInst()", 0x67c);
        }
        if ((dotted == 0 && (context->mDepth & 0x1ffff) != 0) || dotted == 1) {
            char buffer[16];
            sprintf(buffer, "_level%d", context->getDepth());
            EAStringC level(buffer);
            path = level;
        }
        return;
    }
    rva006ff980(parent, path, dotted);
    if (!context->mName.IsEmpty()) {
        path.Append(rva006D5160(separator, context->mName));
    } else {
        int id = context->getDepth();
        EAStringC name;
        path.Append(separator);
        name.rva006D62C0("instance%ld", id);
        path.Append(name);
        context->mName = name;
        AptCharacterInst *instance = (AptCharacterInst *)parent->getCharacterInst();
        AptNativeHash *hash = instance->pHash;
        hash->Set(&name, context);
    }
}

// FUNCTION: BFME2 0x006FFC30 size=174 name=?rva006ffc30@@YAXPAVAptValue@@AAVEAStringC@@@Z evidence=?:placeholder
void rva006ffc30(AptValue *value, EAStringC &path)
{
    {
        EAStringC empty("");
        path = empty;
    }
    rva006ff980(value, path, 0);
    if (path.GetLength() == 0) {
        EAStringC slash("/");
        path = slash;
    }
}

// FUNCTION: BFME2 0x006FFCE0 size=155 name=?rva006ffce0@@YAXPAVAptValue@@AAVEAStringC@@@Z evidence=?:placeholder
void rva006ffce0(AptValue *value, EAStringC &path)
{
    {
        EAStringC empty("");
        path = empty;
    }
    rva006ff980(value, path, 1);
    if (!(path.GetLength() != 0)) {
        APT_AI_ASSERT("sBuf.Size() != 0", 0x6c1);
    }
}

// FUNCTION: BFME2 0x006FFD80 size=781 name=?getVariable@AptActionInterpreter@@QAEPAVAptValue@@PAV2@0PBVEAStringC@@HHH@Z evidence=N:ledger
AptValue *AptActionInterpreter::getVariable(AptValue *context, AptValue *with, const EAStringC *sourceName, int bWith, int bLocals, int bNoContext)
{
    EAStringC name;
    if (!(!context || !context->isCIH(false) || ((AptCIH *)context->checkedCIH(false))->isButtonInst() || ((AptCIH *)context->checkedCIH(false))->isSpriteInstBase()
          || ((AptCIH *)context->checkedCIH(false))->isLevelInst() || ((AptCIH *)context->checkedCIH(false))->rva006E02B0())) {
        APT_AI_ASSERT("!pCurrentContext || !pCurrentContext->isCIH() || pCurrentContext->c_cih()->isButtonInst() || pCurrentContext->c_cih()->isSpriteInstBase() || pCurrentContext->c_cih()->isLevelInst() || pCurrentContext->c_cih()->isTextInst()", 0x718);
    }
    if (*sourceName->c_str() == '$') {
        AptString *string = AptString::Create();
        string->mString = *sourceName;
        return string;
    }
    unsigned char found = 0;
    AptValue *resolved;
    if (!bNoContext) {
        AptValue *target;
        found = rva006FEC00((int)context, (int)with, (EAStringC *)sourceName, (int *)&target, &name);
        resolved = target;
    } else {
        resolved = context;
        name = *sourceName;
    }
    if (name.IsEmpty()) {
        if (resolved)
            return resolved;
        return gpUndefinedValue;
    }
    AptValue *result;
    if (found == 1 && resolved) {
        result = resolved->findChild(&name, with);
        if (result)
            return result;
    }
    if (bLocals && currentFunction) {
        result = currentFunction->rva006FBE80(&name);
        if (result)
            return result;
    }
    if (!resolved || resolved->isUndefined()) {
        if (with)
            return getVariable(context, 0, sourceName, bWith, 1, 0);
        return gpUndefinedValue;
    }
    result = resolved->objectMemberLookup(resolved, &name);
    if (result)
        return result;
    result = resolved->findChild(&name, with);
    if (result)
        return result;
    if (with)
        return getVariable(context, 0, sourceName, bWith, 1, 0);
    if (!resolved->isCIH(false) && currentFunction && !bNoContext) {
        AptNativeHash *hash = currentFunction->mpParentAnim->GetNativeHashVirtual();
        if (hash) {
            result = hash->Lookup(&name);
            if (result)
                return result;
        }
    }
    if (g_pfnUndefinedVariable)
        g_pfnUndefinedVariable(sourceName->c_str());
    return gpUndefinedValue;
}

// FUNCTION: BFME2 0x00700090 size=222 name=?PrepareForExecution@AptActionInterpreter@@QAEPAXPAUAptActionSetup@@@Z evidence=N:ledger
void *AptActionInterpreter::PrepareForExecution(AptActionSetup *setup)
{
    if (thrownValue) {
        APT_AI_ASSERT("!hasThrownValue()", 0x875);
    }
    debugCallStack.Push(new DebugCallStackInfo_t(setup->pDescription, setup->pFunction, setup->nFlags));
    return AptScriptFunctionBase::PushStaticData();
}

// FUNCTION: BFME2 0x00700170 size=327 name=?rva00700170@AptBasePtrStack@@QAEXPAX0@Z evidence=?:placeholder
void AptActionInterpreter::enumerate(AptValue *context, AptValue *with)
{
    AptValue *topValue = stack.At(0);
    if (topValue->isString()) {
        AptString *name = (AptString *)topValue->checkedString();
        topValue = getVariable(context, with, &name->mString, 1, 1, 0);
    }
    stack.PopNoDec();
    stack.PushNoAddRef(gpUndefinedValue);
    AptNativeHash *hash = topValue->GetNativeHashVirtual();
    while (hash) {
        AptHashItem *item = hash->GetFirstItem();
        while (item) {
            if (!item->mKey.Equals(StringPool::GetString(0)) && !item->mKey.Equals(StringPool::GetString(0x78))) {
                AptString *key = AptString::Create();
                key->mString = item->mKey;
                stack.Push(key);
            }
            item = hash->GetNextItem(item);
        }
        AptValue *next = hash->mp__proto__;
        hash = next ? next->GetNativeHashVirtual() : 0;
    }
    topValue->Release();
}

// FUNCTION: BFME2 0x007002C0 size=668 name=?runStream@AptActionInterpreter@@QAEPBEPBEPAVAptCIH@@HPAUAptCharacterInst@@@Z evidence=N:ledger
const unsigned char *AptActionInterpreter::runStream(const unsigned char *stream, AptCIH *current, int maximum, AptCharacterInst *parent)
{
    if (maximum == -1)
        thisStack.Push((AptValue *)current);
    LocalContextT context;
    context.pCurrentContext = current;
    context.pCurWith = 0;
    context.pInstruction = stream;
    context.pRemoveWithAt = 0;
    context.pSuper = getVariable((AptValue *)current, 0, StringPool::GetString(0xa0), 1, 1, 0);
    context.bEncounteredReturn = false;
    context.pParentCharacter = parent;
    int previousBase = stackFrameBase;
    stackFrameBase = stack.GetSize();
    int stackSizeBefore = stackFrameBase;
    while (!thrownValue) {
        if (!(!context.pRemoveWithAt || context.pInstruction <= context.pRemoveWithAt)) {
            APT_AI_ASSERT("!context.pRemoveWithAt || context.pInstruction <= context.pRemoveWithAt", 0xb43);
        }
        if (context.pRemoveWithAt && context.pInstruction == context.pRemoveWithAt) {
            context.pCurWith->Release();
            context.pCurWith = 0;
            context.pRemoveWithAt = 0;
        }
        int action = *context.pInstruction++;
        if (context.bEncounteredReturn)
            break;
        if (maximum >= 0 && context.pInstruction - stream > maximum) {
            stack.Push(gpUndefinedValue);
            break;
        } else if (!action) {
            if (maximum >= 0)
                stack.Push(gpUndefinedValue);
            break;
        }
        if (!(current == 0 || ((AptValue *)current)->getRefCount() > 0)) {
            APT_AI_ASSERT("pCurrentContext == NULL || pCurrentContext->getRefCount() > 0", 0xb64);
        }
        if (!(sGlobalTable[action].mCheckAlignment == action)) {
            APT_AI_ASSERT("sGlobalTable[eAction].mCheckAlignment == eAction", 0xb67);
        }
        sGlobalTable[action].mFunctionPointer(this, &context);
        if (!(stack.GetSize() >= stackSizeBefore)) {
            APT_AI_ASSERT("nStackSizeCurrent >= nStackSizePre", 0xb6f);
        }
    }
    int sizeAfter = stack.GetSize();
    if (maximum >= 0 && sizeAfter > stackFrameBase)
        stack.rva006E3AA0(sizeAfter - stackFrameBase - 1);
    else if (sizeAfter > stackFrameBase)
        stack.rva006E3AA0(sizeAfter - stackFrameBase);
    stackFrameBase = previousBase;
    if (maximum == -1)
        thisStack.Pop();
    int size = stack.GetSize();
    bool release = false;
    if (!size)
        release = true;
    else if (size == 1 && stack.At(0) == gpUndefinedValue)
        release = true;
    if (release && g_releaseVectorAtE17710->GetNumValues())
        g_releaseVectorAtE17710->ReleaseValues();
    return context.pInstruction;
}

// FUNCTION: BFME2 0x00700560 size=128 name=?_FunctionAptActionNextFrame@AptActionInterpreter@@CAXQAU1@QAULocalContextT@1@@Z evidence=N:ledger
void AptActionInterpreter::_FunctionAptActionNextFrame(AptActionInterpreter *const interp, LocalContextT *const context)
{
    AptCIH *cih = (AptCIH *)context->pCurrentContext->checkedCIH(false);
    cih->jumpToFrame(spriteInstBaseOf(cih)->currentFrame + 1);
    spriteInstBaseOf(cih)->flags &= ~0x2000000u;
}

// FUNCTION: BFME2 0x007005E0 size=128 name=?_FunctionAptActionPrevFrame@AptActionInterpreter@@CAXQAU1@QAULocalContextT@1@@Z evidence=N:ledger
void AptActionInterpreter::_FunctionAptActionPrevFrame(AptActionInterpreter *const interp, LocalContextT *const context)
{
    AptCIH *cih = (AptCIH *)context->pCurrentContext->checkedCIH(false);
    cih->jumpToFrame(spriteInstBaseOf(cih)->currentFrame - 1);
    spriteInstBaseOf(cih)->flags &= ~0x2000000u;
}

// FUNCTION: BFME2 0x00700660 size=101 name=?_FunctionAptActionPlay@AptActionInterpreter@@CAXQAU1@QAULocalContextT@1@@Z evidence=N:ledger
void AptActionInterpreter::_FunctionAptActionPlay(AptActionInterpreter *const interp, LocalContextT *const context)
{
    AptValue *current = context->pCurrentContext;
    if (!current->isUndefined() && !current->isLevelInst()) {
        AptValue *node;
        if (context->pCurWith && context->pCurWith->isCIH(false))
            node = context->pCurWith;
        else if (context->pCurrentContext->isCIH(false))
            node = current;
        else
            return;
        node->c_cih(false)->GetSpriteInstBase()->flags |= 0x2000000u;
    }
}

// FUNCTION: BFME2 0x007006D0 size=69 name=?_FunctionAptActionStop@AptActionInterpreter@@CAXQAU1@QAULocalContextT@1@@Z evidence=N:ledger
void AptActionInterpreter::_FunctionAptActionStop(AptActionInterpreter *const interp, LocalContextT *const context)
{
    AptValue *current = context->pCurrentContext;
    if (!current->isUndefined() && !current->isLevelInst() && ((AptCIH *)current)->mpInstance)
        current->c_cih(false)->GetSpriteInstBase()->flags &= ~0x2000000u;
}

// FUNCTION: BFME2 0x00700720 size=341 name=?_FunctionAptActionAdd@AptActionInterpreter@@CAXQAU1@QAULocalContextT@1@@Z evidence=N:ledger
void AptActionInterpreter::_FunctionAptActionAdd(AptActionInterpreter *const interp, LocalContextT *const context)
{
    AptValue *top = interp->stack.At(0);
    AptValue *below = interp->stack.At(1);
    AptValue *result = 0;
    if (Rva006CD220Get() == 7 && (top->isUndefined() || below->isUndefined()))
        result = gpUndefinedValue;
    if (!result) {
        if (top->isInteger() && below->isInteger()) {
            int x = ((AptInteger *)top->checkedInteger())->GetValue();
            int y = ((AptInteger *)below->checkedInteger())->GetValue();
            result = AptInteger::Create(x + y);
        } else {
            float x = top->toFloat();
            float y = below->toFloat();
            result = AptFloat::Create(x + y);
        }
    }
    interp->stack.Pop(2);
    interp->stack.Push(result);
}

// FUNCTION: BFME2 0x00700880 size=341 name=?_FunctionAptActionSubtract@AptActionInterpreter@@CAXQAU1@QAULocalContextT@1@@Z evidence=N:ledger
void AptActionInterpreter::_FunctionAptActionSubtract(AptActionInterpreter *const interp, LocalContextT *const context)
{
    AptValue *top = interp->stack.At(0);
    AptValue *below = interp->stack.At(1);
    AptValue *result = 0;
    if (Rva006CD220Get() == 7 && (top->isUndefined() || below->isUndefined()))
        result = gpUndefinedValue;
    if (!result) {
        if (top->isInteger() && below->isInteger()) {
            int x = ((AptInteger *)top->checkedInteger())->GetValue();
            int y = ((AptInteger *)below->checkedInteger())->GetValue();
            result = AptInteger::Create(y - x);
        } else {
            float x = top->toFloat();
            float y = below->toFloat();
            result = AptFloat::Create(y - x);
        }
    }
    interp->stack.Pop(2);
    interp->stack.Push(result);
}

// FUNCTION: BFME2 0x007009E0 size=341 name=?_FunctionAptActionMultiply@AptActionInterpreter@@CAXQAU1@QAULocalContextT@1@@Z evidence=N:ledger
void AptActionInterpreter::_FunctionAptActionMultiply(AptActionInterpreter *const interp, LocalContextT *const context)
{
    AptValue *top = interp->stack.At(0);
    AptValue *below = interp->stack.At(1);
    AptValue *result = 0;
    if (Rva006CD220Get() == 7 && (top->isUndefined() || below->isUndefined()))
        result = gpUndefinedValue;
    if (!result) {
        if (top->isInteger() && below->isInteger()) {
            int x = ((AptInteger *)top->checkedInteger())->GetValue();
            int y = ((AptInteger *)below->checkedInteger())->GetValue();
            result = AptInteger::Create(x * y);
        } else {
            float x = top->toFloat();
            float y = below->toFloat();
            result = AptFloat::Create(x * y);
        }
    }
    interp->stack.Pop(2);
    interp->stack.Push(result);
}

// FUNCTION: BFME2 0x00700B40 size=309 name=?_FunctionAptActionDivide@AptActionInterpreter@@CAXQAU1@QAULocalContextT@1@@Z evidence=N:ledger
void AptActionInterpreter::_FunctionAptActionDivide(AptActionInterpreter *const interp, LocalContextT *const context)
{
    AptValue *top = interp->stack.At(0);
    AptValue *below = interp->stack.At(1);
    AptValue *result = 0;
    if (Rva006CD220Get() == 7 && (top->isUndefined() || below->isUndefined()))
        result = gpUndefinedValue;
    if (!result) {
        float divisor = top->toFloat();
        float dividend = below->toFloat();
        if (divisor == 0.0f)
            result = gpUndefinedValue;
        else
            result = AptFloat::Create(dividend / divisor);
    }
    interp->stack.Pop(2);
    interp->stack.Push(result);
}

// FUNCTION: BFME2 0x00700C80 size=357 name=?_FunctionAptActionEquals@AptActionInterpreter@@CAXQAU1@QAULocalContextT@1@@Z evidence=N:ledger
void AptActionInterpreter::_FunctionAptActionEquals(AptActionInterpreter *const interp, LocalContextT *const context)
{
    AptValue *top = interp->stack.At(0);
    AptValue *below = interp->stack.At(1);
    AptValue *result = 0;
    if (Rva006CD220Get() == 7 && (top->isUndefined() || below->isUndefined()))
        result = gpUndefinedValue;
    if (!result) {
        if (top->isInteger() && below->isInteger()) {
            int x = ((AptInteger *)top->checkedInteger())->GetValue();
            int y = ((AptInteger *)below->checkedInteger())->GetValue();
            result = AptBoolean::Create(x == y);
        } else {
            float x = top->toFloat();
            float y = below->toFloat();
            result = AptBoolean::Create(fabs(x - y) < 0.001);
        }
    }
    interp->stack.Pop(2);
    interp->stack.Push(result);
}

// FUNCTION: BFME2 0x00700DF0 size=346 name=?_FunctionAptActionLessThan@AptActionInterpreter@@CAXQAU1@QAULocalContextT@1@@Z evidence=N:ledger
void AptActionInterpreter::_FunctionAptActionLessThan(AptActionInterpreter *const interp, LocalContextT *const context)
{
    AptValue *top = interp->stack.At(0);
    AptValue *below = interp->stack.At(1);
    AptValue *result = 0;
    if (Rva006CD220Get() == 7 && (top->isUndefined() || below->isUndefined()))
        result = gpUndefinedValue;
    if (!result) {
        if (top->isInteger() && below->isInteger()) {
            int x = ((AptInteger *)top->checkedInteger())->GetValue();
            int y = ((AptInteger *)below->checkedInteger())->GetValue();
            result = AptBoolean::Create(y < x);
        } else {
            float x = top->toFloat();
            float y = below->toFloat();
            result = AptBoolean::Create(y < x);
        }
    }
    interp->stack.Pop(2);
    interp->stack.Push(result);
}

// FUNCTION: BFME2 0x00700F50 size=381 name=?_FunctionAptActionAnd@AptActionInterpreter@@CAXQAU1@QAULocalContextT@1@@Z evidence=N:ledger
void AptActionInterpreter::_FunctionAptActionAnd(AptActionInterpreter *const interp, LocalContextT *const context)
{
    AptValue *top = interp->stack.At(0);
    AptValue *below = interp->stack.At(1);
    AptValue *result = 0;
    if (Rva006CD220Get() == 7 && (top->isUndefined() || below->isUndefined()))
        result = gpUndefinedValue;
    if (!result) {
        if (top->isInteger() && below->isInteger()) {
            int x = ((AptInteger *)top->checkedInteger())->GetValue();
            int y = ((AptInteger *)below->checkedInteger())->GetValue();
            result = AptBoolean::Create(x && y);
        } else {
            float x = top->toFloat();
            float y = below->toFloat();
            result = AptBoolean::Create(x != 0.0f && y != 0.0f);
        }
    }
    interp->stack.Pop(2);
    interp->stack.Push(result);
}

// FUNCTION: BFME2 0x007010D0 size=389 name=?_FunctionAptActionOr@AptActionInterpreter@@CAXQAU1@QAULocalContextT@1@@Z evidence=N:ledger
void AptActionInterpreter::_FunctionAptActionOr(AptActionInterpreter *const interp, LocalContextT *const context)
{
    AptValue *top = interp->stack.At(0);
    AptValue *below = interp->stack.At(1);
    AptValue *result = 0;
    if (Rva006CD220Get() == 7 && (top->isUndefined() || below->isUndefined()))
        result = gpUndefinedValue;
    if (!result) {
        if (top->isInteger() && below->isInteger()) {
            int x = ((AptInteger *)top->checkedInteger())->GetValue();
            int y = ((AptInteger *)below->checkedInteger())->GetValue();
            result = AptBoolean::Create(x || y);
        } else {
            float x = top->toFloat();
            float y = below->toFloat();
            result = AptBoolean::Create(x != 0.0f || y != 0.0f);
        }
    }
    interp->stack.Pop(2);
    interp->stack.Push(result);
}

// FUNCTION: BFME2 0x00701260 size=154 name=?_FunctionAptActionNot@AptActionInterpreter@@CAXQAU1@QAULocalContextT@1@@Z evidence=N:ledger
void AptActionInterpreter::_FunctionAptActionNot(AptActionInterpreter *const interp, LocalContextT *const context)
{
    AptValue *result = AptBoolean::Create(!interp->stack.At(0)->toBool());
    interp->stack.Pop();
    interp->stack.Push(result);
}
// FUNCTION: BFME2 0x00701300 size=434 name=?_FunctionAptActionStringEquals@AptActionInterpreter@@CAXQAU1@QAULocalContextT@1@@Z evidence=N:ledger
void AptActionInterpreter::_FunctionAptActionStringEquals(AptActionInterpreter *const interp, LocalContextT *const context)
{
    AptValue *a = interp->stack.At(0);
    AptValue *b = interp->stack.At(1);
    AptValue *result = 0;
    int nResult = 0;
    if (Rva006CD220Get() == 7) {
        if (a->isUndefined())
            ++nResult;
        if (b->isUndefined())
            ++nResult;
        switch (nResult) {
        case 1:
            result = gpUndefinedValue;
            break;
        case 2:
            result = AptBoolean::Create(true);
            break;
        }
    }
    if (!result) {
        EAStringC textA, textB;
        a->toString(textA);
        b->toString(textB);
        if (textA.IsEqualTo(&textB))
            nResult = 1;
        result = AptBoolean::Create(nResult != 0);
    }
    interp->stack.Pop(2);
    interp->stack.Push(result);
}

// FUNCTION: BFME2 0x007014C0 size=225 name=?_FunctionAptActionStringLength@AptActionInterpreter@@CAXQAU1@QAULocalContextT@1@@Z evidence=N:ledger
void AptActionInterpreter::_FunctionAptActionStringLength(AptActionInterpreter *const interp, LocalContextT *const context)
{
    AptValue *top = interp->stack.At(0);
    EAStringC text;
    top->toString(text);
    AptValue *result = AptInteger::Create(text.GetLength());
    interp->stack.Pop(1);
    interp->stack.Push(result);
}

// FUNCTION: BFME2 0x007015B0 size=436 name=?_FunctionAptActionSubString@AptActionInterpreter@@CAXQAU1@QAULocalContextT@1@@Z evidence=N:ledger
void AptActionInterpreter::_FunctionAptActionSubString(AptActionInterpreter *const interp, LocalContextT *const context)
{
    AptValue *count = interp->stack.At(0);
    AptValue *index = interp->stack.At(1);
    AptValue *value = interp->stack.At(2);
    int nCount = count->toInteger();
    int nIndex = index->toInteger();
    --nIndex;
    if (nIndex < 0)
        nIndex = 0;
    EAStringC text;
    value->toString(text);
    AptString *sub = AptString::Create();
    if (nCount == 0)
        sub->mString.rva006D3470();
    else if (nCount < 0)
        sub->mString = text.rva006D5ED0(nIndex);
    else
        sub->mString = text.rva006d5f30(nIndex, nCount);
    interp->stack.Pop(3);
    interp->stack.Push(sub);
}

// FUNCTION: BFME2 0x00701770 size=127 name=?_FunctionAptActionPop@AptActionInterpreter@@CAXQAU1@QAULocalContextT@1@@Z evidence=N:ledger
void AptActionInterpreter::_FunctionAptActionPop(AptActionInterpreter *const interp, LocalContextT *const context)
{
    int elements = interp->stack.m_nElements;
    if (!(elements > interp->stackFrameBase))
        Rva006CC110Log(3, "--Warning-- Actionscript Popping when no items are in stack!\n");
    if (elements > interp->stackFrameBase)
        interp->stack.Pop();
    if (elements == 1 && g_releaseVectorAtE17710->GetNumValues())
        g_releaseVectorAtE17710->ReleaseValues();
}

// FUNCTION: BFME2 0x007017F0 size=181 name=?_FunctionAptActionToInteger@AptActionInterpreter@@CAXQAU1@QAULocalContextT@1@@Z evidence=N:ledger
void AptActionInterpreter::_FunctionAptActionToInteger(AptActionInterpreter *const interp, LocalContextT *const context)
{
    AptValue *top = interp->stack.At(0);
    AptValue *result;
    if (!(Rva006CD220Get() == 7 && top->isUndefined() && (result = gpUndefinedValue)))
        result = AptInteger::Create(top->toInteger());
    interp->stack.Pop();
    interp->stack.Push(result);
}

// FUNCTION: BFME2 0x007018B0 size=251 name=?_FunctionAptActionGetVariable@AptActionInterpreter@@CAXQAU1@QAULocalContextT@1@@Z evidence=N:ledger
void AptActionInterpreter::_FunctionAptActionGetVariable(AptActionInterpreter *const interp, LocalContextT *const context)
{
    AptValue *name = interp->stack.At(0);
    if (!name->isUndefined()) {
        EAStringC text;
        name->toString(text);
        AptValue *value = interp->getVariable(context->pCurrentContext, context->pCurWith, &text, 1, 1, 0);
        interp->stack.Pop();
        interp->stack.Push(value);
    }
}

// FUNCTION: BFME2 0x007019B0 size=317 name=?_FunctionAptActionSetVariable@AptActionInterpreter@@CAXQAU1@QAULocalContextT@1@@Z evidence=N:ledger
void AptActionInterpreter::_FunctionAptActionSetVariable(AptActionInterpreter *const interp, LocalContextT *const context)
{
    EAStringC text;
    AptValue *value = interp->stack.At(0);
    AptValue *name = interp->stack.At(1);
    name->toString(text);
    interp->setVariable(context->pCurrentContext, context->pCurWith, &text, value, 1, 1, 0);
    interp->stack.Pop(2);
    if (g_releaseVectorAtE17710->GetNumValues() != 0 && interp->stack.GetSize() == 0)
        g_releaseVectorAtE17710->ReleaseValues();
}

// FUNCTION: BFME2 0x00701AF0 size=281 name=?_FunctionAptActionStringAdd@AptActionInterpreter@@CAXQAU1@QAULocalContextT@1@@Z evidence=N:ledger
void AptActionInterpreter::_FunctionAptActionStringAdd(AptActionInterpreter *const interp, LocalContextT *const context)
{
    AptValue *top = interp->stack.At(0);
    AptValue *below = interp->stack.At(1);
    if (Rva006CD220Get() == 7) {
        AptString *undefinedText = AptString::Create();
        undefinedText->SetString(StringPool::GetString(0xa9)->c_str());
        if (top->isUndefined())
            top = undefinedText;
        if (below->isUndefined())
            below = undefinedText;
    }
    AptString *result = _concatAsStrings(below, top);
    interp->stack.Pop(2);
    interp->stack.Push(result);
}

// FUNCTION: BFME2 0x00701C10 size=354 name=?_FunctionAptActionTrace@AptActionInterpreter@@CAXQAU1@QAULocalContextT@1@@Z evidence=N:ledger
void AptActionInterpreter::_FunctionAptActionTrace(AptActionInterpreter *const interp, LocalContextT *const context)
{
    AptValue *value = interp->stack.At(0);
    EAStringC text;
    AptString *undefinedText = 0;
    if (Rva006CD220Get() == 7 && value->isUndefined()) {
        undefinedText = AptString::Create();
        undefinedText->SetString(StringPool::GetString(0xa9)->c_str());
        undefinedText->toString(text);
    }
    if (!undefinedText)
        value->toString(text);
    EAStringC line("AptTrace: ");
    line.Append(text);
    line.Append("\n");
    g_pfnAptLog("%s", line.c_str());
    interp->stack.Pop();
}

// FUNCTION: BFME2 0x00701D80 size=810 name=?_FunctionAptActionStartDragMovie@AptActionInterpreter@@CAXQAU1@QAULocalContextT@1@@Z evidence=N:ledger
void AptActionInterpreter::_FunctionAptActionStartDragMovie(AptActionInterpreter *const interp, LocalContextT *const context)
{
    AptValue *target = interp->stack.At(0);
    if (target->isString()) {
        AptValue *resolved = 0;
        EAStringC name;
        AptString *label = (AptString *)target->checkedString();
        rva006FEC00((int)context->pCurrentContext, (int)context->pCurWith, &label->mString, (int *)&resolved, &name);
        target = interp->getVariable(resolved, context->pCurWith, &name, 1, 1, 0);
    }
    if (!target->isCIH(false)) {
        APT_AI_ASSERT("pTarget->isCIH()", 0x11ba);
    }
    int popCount = 3;
    target->AddRef();
    g_pAptAnimationPoolData->field44 = target;
    g_pAptAnimationPoolData->field58 = 0;
    g_pAptAnimationPoolData->field5C = 0;
    g_pAptAnimationPoolData->field48 = -9999.0f;
    g_pAptAnimationPoolData->field4C = -9999.0f;
    g_pAptAnimationPoolData->field50 = -9999.0f;
    g_pAptAnimationPoolData->field54 = -9999.0f;
    if (!interp->stack.At(1)->isInteger()) {
        g_pAptAnimationPoolData->field58 = (float)g_pAptAnimationPoolData->field74 - ((AptCIH *)target->checkedCIH(false))->mTransform[4];
        g_pAptAnimationPoolData->field5C = (float)g_pAptAnimationPoolData->field78 - ((AptCIH *)target->checkedCIH(false))->mTransform[5];
    }
    if (interp->stack.At(2)->isInteger()) {
        popCount += 4;
        g_pAptAnimationPoolData->field54 = interp->stack.At(3)->toFloat();
        g_pAptAnimationPoolData->field50 = interp->stack.At(4)->toFloat();
        g_pAptAnimationPoolData->field4C = interp->stack.At(5)->toFloat();
        g_pAptAnimationPoolData->field48 = interp->stack.At(6)->toFloat();
    }
    interp->stack.rva006E3AA0(popCount);
}

// FUNCTION: BFME2 0x007020B0 size=176 name=?_FunctionAptActionRandom@AptActionInterpreter@@SAXQAU1@@Z evidence=N:ledger
void AptActionInterpreter::_FunctionAptActionRandom(AptActionInterpreter *const interp)
{
    AptValue *top = interp->stack.At(0);
    unsigned int value;
    if (top->isUndefined()) {
        value = 0;
    } else {
        unsigned int range = top->toInteger();
        value = bfmeNext1221() % range;
    }
    AptValue *result = AptInteger::Create(value);
    interp->stack.Pop();
    interp->stack.Push(result);
}

// FUNCTION: BFME2 0x00702160 size=296 name=?_FunctionAptActionAsciiToChar@AptActionInterpreter@@CAXQAU1@QAULocalContextT@1@@Z evidence=N:ledger
void AptActionInterpreter::_FunctionAptActionAsciiToChar(AptActionInterpreter *const interp, LocalContextT *const context)
{
    AptValue *value = interp->stack.At(0);
    if (!value->isUndefined()) {
        AptString *text = AptString::Create();
        text->mString = EAStringC(value->toInteger(), 1);
        interp->stack.Pop();
        interp->stack.Push(text);
    } else {
        interp->stack.Pop();
        interp->stack.Push(gpUndefinedValue);
    }
}

// FUNCTION: BFME2 0x00702290 size=25 name=?_FunctionAptActionGetTimer@AptActionInterpreter@@CAXQAU1@QAULocalContextT@1@@Z evidence=N:ledger
void AptActionInterpreter::_FunctionAptActionGetTimer(AptActionInterpreter *const interpreter, LocalContextT *const localContext)
{
    interpreter->stack.Push(AptInteger::Create(g_nAptRecordFrame));
}

// FUNCTION: BFME2 0x007022B0 size=316 name=?_FunctionAptActionDelete@AptActionInterpreter@@CAXQAU1@QAULocalContextT@1@@Z evidence=N:ledger
void AptActionInterpreter::_FunctionAptActionDelete(AptActionInterpreter *const interp, LocalContextT *const context)
{
    AptValue *name = interp->stack.At(0);
    AptValue *object = interp->stack.At(1);
    if (object->ContainsNativeHashVirtual()) {
        EAStringC text;
        name->toString(text);
        interp->setVariable(object, context->pCurWith, &text, 0, 1, 1, 0);
    }
    interp->stack.Pop(2);
    interp->stack.Push(AptInteger::Create(1));
}

// FUNCTION: BFME2 0x007023F0 size=246 name=?_FunctionAptActionDelete2@AptActionInterpreter@@CAXQAU1@QAULocalContextT@1@@Z evidence=N:ledger
void AptActionInterpreter::_FunctionAptActionDelete2(AptActionInterpreter *const interp, LocalContextT *const context)
{
    AptValue *name = interp->stack.At(0);
    EAStringC text;
    name->toString(text);
    interp->setVariable(context->pCurrentContext, context->pCurWith, &text, 0, 1, 1, 0);
    interp->stack.Pop();
    interp->stack.Push(AptInteger::Create(1));
}

// FUNCTION: BFME2 0x007024F0 size=267 name=?_FunctionAptActionDefineLocal@AptActionInterpreter@@CAXQAU1@QAULocalContextT@1@@Z evidence=N:ledger
void AptActionInterpreter::_FunctionAptActionDefineLocal(AptActionInterpreter *const interp, LocalContextT *const context)
{
    AptValue *value = interp->stack.At(0);
    AptValue *name = interp->stack.At(1);
    if (interp->currentFunction) {
        AptString *nameString = (AptString *)name->checkedString();
        if (!AptScriptFunctionBase::spFrameStack)
            interp->currentFunction->CreateFrameStack();
        AptScriptFunctionBase::SetScopeVariable(&nameString->mString, value);
    } else {
        AptString *nameString = (AptString *)name->checkedString();
        interp->setVariable(context->pCurrentContext, context->pCurWith, &nameString->mString, value, 0, 1, 0);
    }
    interp->stack.Pop(2);
}

// FUNCTION: BFME2 0x00702600 size=315 name=?_FunctionAptActionModulo@AptActionInterpreter@@CAXQAU1@QAULocalContextT@1@@Z evidence=N:ledger
void AptActionInterpreter::_FunctionAptActionModulo(AptActionInterpreter *const interp, LocalContextT *const context)
{
    AptValue *top = interp->stack.At(0);
    AptValue *below = interp->stack.At(1);
    AptValue *result = 0;
    if (Rva006CD220Get() == 7 && (top->isUndefined() || below->isUndefined()))
        result = gpUndefinedValue;
    if (!result) {
        float divisor = top->toFloat();
        if (divisor == 0.0f) {
            result = gpUndefinedValue;
        } else {
            float dividend = below->toFloat();
            result = AptFloat::Create((float)fmod(dividend, divisor));
        }
    }
    interp->stack.Pop(2);
    interp->stack.Push(result);
}

// FUNCTION: BFME2 0x00702740 size=278 name=?_FunctionAptActionDefineLocal2@AptActionInterpreter@@CAXQAU1@QAULocalContextT@1@@Z evidence=N:ledger
void AptActionInterpreter::_FunctionAptActionDefineLocal2(AptActionInterpreter *const interp, LocalContextT *const context)
{
    EAStringC *name = &((AptString *)interp->stack.At(0)->checkedString())->mString;
    AptScriptFunctionBase *function = interp->currentFunction;
    if (function) {
        bool exists;
        if (!AptScriptFunctionBase::spFrameStack) {
            if (function->mpCreatorScope)
                exists = function->mpCreatorScope->GetNativeHash()->Lookup(name) != 0;
            else
                exists = false;
        } else {
            exists = AptScriptFunctionBase::spFrameStack->GetNativeHash()->Lookup(name) != 0;
        }
        if (!exists) {
            AptValue *undefined = gpUndefinedValue;
            if (!AptScriptFunctionBase::spFrameStack)
                function->CreateFrameStack();
            AptScriptFunctionBase::SetScopeVariable(name, undefined);
        }
    } else if (interp->getVariable(context->pCurrentContext, context->pCurWith, name, 0, 1, 0)->isUndefined()) {
        interp->setVariable(context->pCurrentContext, context->pCurWith, name, gpUndefinedValue, 0, 1, 0);
    }
    interp->stack.Pop();
}

// FUNCTION: BFME2 0x00702860 size=310 name=?_FunctionAptActionInitArray@AptActionInterpreter@@CAXQAU1@QAULocalContextT@1@@Z evidence=N:ledger
void AptActionInterpreter::_FunctionAptActionInitArray(AptActionInterpreter *const interp, LocalContextT *const context)
{
    int n = interp->stack.At(0)->toInteger();
    interp->stack.Pop();
    AptArray *array = new AptArray;
    array->AddRef();
    for (int i = 0; i < n; ++i)
        array->rva006D95E0(i, interp->stack.At(i));
    interp->stack.PopN(n);
    interp->stack.PushNoAddRef(array);
}

// FUNCTION: BFME2 0x007029A0 size=413 name=?_FunctionAptActionTypeOf@AptActionInterpreter@@CAXQAU1@QAULocalContextT@1@@Z evidence=N:ledger
void AptActionInterpreter::_FunctionAptActionTypeOf(AptActionInterpreter *const interp, LocalContextT *const context)
{
    AptValue *value = interp->stack.At(0);
    AptString *text = AptString::Create();
    if (value->getIsDefined()) {
        if (value->isInteger() || value->isFloat())
            text->SetString(StringPool::GetString(0x63)->c_str());
        else if (value->isBoolean())
            text->SetString(StringPool::GetString(0x27)->c_str());
        else if (value->isString())
            text->SetString(StringPool::GetString(0x9d)->c_str());
        else if (value->isObject() || value->isArray())
            text->SetString(StringPool::GetString(0x64)->c_str());
        else if (value->isCIH()) {
            if ((char)value->c_cih(false)->rva006E01A0(false) || (char)value->c_cih(false)->rva006CBEE0(false))
                text->SetString(StringPool::GetString(0x5e)->c_str());
            else if (value->c_cih(false)->isLevelInst())
                text->SetString(StringPool::GetString(0xa9)->c_str());
            else
                text->SetString(StringPool::GetString(0x64)->c_str());
        } else if (value->isNone())
            text->SetString(StringPool::GetString(0x62)->c_str());
        else if (value->isUndefined())
            text->SetString(StringPool::GetString(0xa9)->c_str());
        else if (value->isScriptFunction() || value->isNativeFunction())
            text->SetString(StringPool::GetString(0x37)->c_str());
    } else
        text->SetString(StringPool::GetString(0xa9)->c_str());
    interp->stack.Pop();
    interp->stack.Push(text);
}

// FUNCTION: BFME2 0x00702B40 size=22 name=?rva00702b40@@YAXPAVRva00700170@@PBURva00702B40Payload@@@Z evidence=?:placeholder
void AptActionInterpreter::_FunctionAptActionEnumerate(AptActionInterpreter *const interp, LocalContextT *const context)
{
    interp->enumerate((AptValue *)context->pCurrentContext, context->pCurWith);
}

// FUNCTION: BFME2 0x00702B60 size=956 name=?_FunctionAptActionAdd2@AptActionInterpreter@@CAXQAU1@QAULocalContextT@1@@Z evidence=N:ledger
void AptActionInterpreter::_FunctionAptActionAdd2(AptActionInterpreter *const interp, LocalContextT *const context)
{
    AptValue *a = interp->stack.At(0);
    AptValue *b = interp->stack.At(1);
    int version = Rva006CD220Get();
    if (a->isString() || b->isString()) {
        if (version == 7) {
            if (a->isUndefined()) {
                a = AptString::Create();
                a->checkedString()->SetString(StringPool::GetString(0xa9)->c_str());
            }
            if (b->isUndefined()) {
                b = AptString::Create();
                b->checkedString()->SetString(StringPool::GetString(0xa9)->c_str());
            }
        }
        AptString *text = _concatAsStrings(b, a);
        interp->stack.Pop(2);
        interp->stack.Push(text);
    } else if ((a->isInteger() || b->isInteger()) && !a->isFloat() && !b->isFloat()) {
        if (version == 7 && (a->isUndefined() || b->isUndefined())) {
            interp->stack.Pop(2);
            interp->stack.Push(gpUndefinedValue);
            return;
        }
        int x = a->toInteger();
        int y = b->toInteger();
        interp->stack.Pop(2);
        interp->stack.Push(AptInteger::Create(x + y));
    } else {
        if (version == 7 && (a->isUndefined() || b->isUndefined())) {
            interp->stack.Pop(2);
            interp->stack.Push(gpUndefinedValue);
            return;
        }
        float x = a->toFloat();
        float y = b->toFloat();
        interp->stack.Pop(2);
        interp->stack.Push(AptFloat::Create(x + y));
    }
}

// FUNCTION: BFME2 0x00702F20 size=700 name=?_FunctionAptActionLessThan2@AptActionInterpreter@@CAXQAU1@QAULocalContextT@1@@Z evidence=N:ledger
void AptActionInterpreter::_FunctionAptActionLessThan2(AptActionInterpreter *const interp, LocalContextT *const context)
{
    AptValue *a = interp->stack.At(0);
    AptValue *b = interp->stack.At(1);
    if (Rva006CD220Get() == 7) {
        if (a->isUndefined() || b->isUndefined()) {
            interp->stack.Pop(2);
            interp->stack.Push(gpUndefinedValue);
            return;
        }
    }
    int result = 0;
    if (a->isString() && b->isString()) {
        AptString *stringA = (AptString *)a->checkedString();
        AptString *stringB = (AptString *)b->checkedString();
        const char *textA = stringA->mString.c_str();
        const char *textB = stringB->mString.c_str();
        result = strcmp(textB, textA) < 0;
    } else if (rva006fc370(a) || rva006fc370(b)) {
        interp->stack.Pop(2);
        interp->stack.Push(gpUndefinedValue);
        return;
    } else if (a->isFloat() || b->isFloat()) {
        result = b->toFloat() < a->toFloat();
    } else {
        result = b->toInteger() < a->toInteger();
    }
    AptValue *value = AptBoolean::Create(result != 0);
    interp->stack.Pop(2);
    interp->stack.Push(value);
}

// FUNCTION: BFME2 0x007031E0 size=1345 name=?_FunctionAptActionEquals2@AptActionInterpreter@@CAXQAU1@QAULocalContextT@1@@Z evidence=N:ledger
void AptActionInterpreter::_FunctionAptActionEquals2(AptActionInterpreter *const interp, LocalContextT *const context)
{
    AptValue *a = interp->stack.At(0);
    AptValue *b = interp->stack.At(1);
    int result = 0;
    if (a->getVtblIndex() == AptVFT_CIHLast)
        a = gpUndefinedValue;
    if (b->getVtblIndex() == AptVFT_CIHLast)
        b = gpUndefinedValue;
    if (Rva006CD220Get() == 7) {
        if (a->isUndefined())
            result++;
        if (b->isUndefined())
            result++;
        if (result > 0) {
            interp->stack.Pop(2);
            interp->stack.Push(AptBoolean::Create(result == 2));
            return;
        }
    }
    if (((a->isInteger() || a->isFloat() || a->isBoolean() || a->isString()) &&
         (b->isInteger() || b->isFloat() || b->isBoolean() || b->isString())) ||
        (a->getVtblIndex() == b->getVtblIndex())) {
        if (a->isUndefined()) {
            result = 1;
        } else if (a->isInteger() && b->isInteger()) {
            result = (a->toInteger() == b->toInteger());
        } else if (a->isFloat() && b->isFloat()) {
            result = a->toFloat() == b->toFloat();
        } else if (a->isString() && b->isString()) {
            AptString *stringA = (AptString *)a->checkedString();
            AptString *stringB = (AptString *)b->checkedString();
            if (stringB->mString.IsEqualTo(&stringA->mString))
                result = 1;
        } else if (((a->isInteger() || a->isFloat()) && !rva006fc370(b)) || ((b->isInteger() || b->isFloat()) && !rva006fc370(a))) {
            bool aIsFloat = false;
            bool bIsFloat = false;
            if (a->isString() || a->isFloat()) {
                if (a->isFloat()) {
                    aIsFloat = true;
                } else {
                    AptString *stringA = (AptString *)a->checkedString();
                    if (stringA->mString.Find('.') != -1)
                        aIsFloat = true;
                }
            }
            if (b->isString() || b->isFloat()) {
                if (b->isFloat()) {
                    bIsFloat = true;
                } else {
                    AptString *stringB = (AptString *)b->checkedString();
                    if (stringB->mString.Find('.') != -1)
                        bIsFloat = true;
                }
            }
            if (a->isInteger()) {
                int x = a->toInteger();
                if (bIsFloat) {
                    float y = b->toFloat();
                    result = (float)fabs(x - y) < 0.001f;
                } else {
                    int y = b->toInteger();
                    result = (x == y);
                }
            } else if (b->isInteger()) {
                int y = b->toInteger();
                if (aIsFloat) {
                    float x = a->toFloat();
                    result = (float)fabs(x - y) < 0.001f;
                } else {
                    int x = a->toInteger();
                    result = (x == y);
                }
            } else {
                float x = a->toFloat();
                float y = b->toFloat();
                result = (float)fabs(x - y) < 0.001f;
            }
        } else {
            if (a->isString() && !b->isBoolean()) {
                EAStringC textA, textB;
                a->toString(textA);
                b->toString(textB);
                result = textA.IsEqualTo(&textB) ? 1 : 0;
            } else if (a->isBoolean() && !b->isString()) {
                result = a->toInteger() == b->toInteger() ? 1 : 0;
            } else {
                result = (a == b);
            }
        }
    } else {
        if (a->isUndefined() && b->isUndefined())
            result = 1;
    }
    interp->stack.Pop(2);
    interp->stack.Push(AptBoolean::Create(result != 0));
}

// FUNCTION: BFME2 0x00703730 size=301 name=?_FunctionAptActionToNumber@AptActionInterpreter@@CAXQAU1@QAULocalContextT@1@@Z evidence=N:ledger
void AptActionInterpreter::_FunctionAptActionToNumber(AptActionInterpreter *const interp, LocalContextT *const context)
{
    AptValue *value = interp->stack.At(0);
    if (!value->isFloat() && !value->isInteger()) {
        AptValue *result = gpUndefinedValue;
        if (!rva006fc370(value)) {
            if (Rva006CD220Get() == 7 && value->isUndefined()) {
                interp->stack.Pop();
                interp->stack.Push(result);
                return;
            }
            EAStringC text;
            value->toString(text);
            int place = text.rva006d6070(".", 0);
            if (place != -1 && place != text.GetLength())
                result = AptFloat::Create(value->toFloat());
            else
                result = AptInteger::Create(value->toInteger());
        }
        interp->stack.Pop();
        interp->stack.Push(result);
    }
}

// FUNCTION: BFME2 0x00703860 size=327 name=?_FunctionAptActionToString@AptActionInterpreter@@CAXQAU1@QAULocalContextT@1@@Z evidence=N:ledger
void AptActionInterpreter::_FunctionAptActionToString(AptActionInterpreter *const interp, LocalContextT *const context)
{
    AptValue *top = interp->stack.At(0);
    if (top->isString())
        return;
    if (Rva006CD220Get() == 7 && top->isUndefined()) {
        AptString *undefinedText = AptString::Create();
        undefinedText->SetString(StringPool::GetString(0xa9)->c_str());
        interp->stack.Pop();
        interp->stack.Push(undefinedText);
        return;
    }
    EAStringC text;
    top->toString(text);
    interp->stack.Pop();
    AptString *result = AptString::Create();
    result->mString = text;
    interp->stack.Push(result);
}

// FUNCTION: BFME2 0x007039B0 size=63 name=?_FunctionAptActionPushDuplicate@AptActionInterpreter@@CAXQAU1@QAULocalContextT@1@@Z evidence=N:ledger
void AptActionInterpreter::_FunctionAptActionPushDuplicate(AptActionInterpreter *const interp, LocalContextT *const context)
{
    interp->stack.Push(interp->stack.At(0));
}

// FUNCTION: BFME2 0x007039F0 size=229 name=?_FunctionAptActionStackSwap@AptActionInterpreter@@CAXQAU1@QAULocalContextT@1@@Z evidence=N:ledger
void AptActionInterpreter::_FunctionAptActionStackSwap(AptActionInterpreter *const interp, LocalContextT *const context)
{
    AptValue *first = interp->stack.At(0);
    AptValue *second = interp->stack.At(1);
    interp->stack.PopNoDec();
    interp->stack.PopNoDec();
    interp->stack.PushNoAddRef(first);
    interp->stack.PushNoAddRef(second);
}
// FUNCTION: BFME2 0x00703AE0 size=503 name=?_FunctionAptActionGetMember@AptActionInterpreter@@CAXQAU1@QAULocalContextT@1@@Z evidence=N:ledger
void AptActionInterpreter::_FunctionAptActionGetMember(AptActionInterpreter *const interp, LocalContextT *const context)
{
    AptValue *name = interp->stack.At(0);
    AptValue *object = interp->stack.At(1);
    if (object->isUndefined() || name->isUndefined()) {
        interp->stack.Pop(2);
        interp->stack.Push(gpUndefinedValue);
    } else if (object->isArray() && (name->isInteger() || name->isFloat())) {
        AptArray *array = (AptArray *)object->checkedArray();
        AptValue *value = array->get(name->toInteger());
        interp->stack.PopAndPushValue(2, value);
    } else if (object->isNone()) {
        AptString *key = (AptString *)name->checkedString();
        AptValue *value = g_pfnGetExtern(key->mString.c_str());
        interp->stack.PopAndPushValue(2, value);
    } else {
        EAStringC text;
        name->toString(text);
        AptValue *value = interp->getVariable(object, 0, &text, 1, 0, 1);
        interp->stack.PopAndPushValue(2, value);
    }
}

// FUNCTION: BFME2 0x00703CE0 size=602 name=?_FunctionAptActionSetMember@AptActionInterpreter@@CAXQAU1@QAULocalContextT@1@@Z evidence=N:ledger
void AptActionInterpreter::_FunctionAptActionSetMember(AptActionInterpreter *const interp, LocalContextT *const context)
{
    AptValue *value = interp->stack.At(0);
    AptValue *name = interp->stack.At(1);
    AptValue *object = interp->stack.At(2);
    if (object->isArray() && (name->isInteger() || name->isFloat())) {
        AptArray *array = (AptArray *)object->checkedArray();
        array->rva006D95E0(name->toInteger(), value);
    } else if (object->ContainsNativeHashVirtual() || object->isCIH(false)) {
        EAStringC text;
        name->toString(text);
        interp->setVariable(object, context->pCurWith, &text, value, 1, 0, 1);
        if (text.IsEqualTo(StringPool::GetString(0)) && (object->isObject() || object->isCIH(false)))
            object->setHasClass(1);
    } else if (object->isNone()) {
        EAStringC text;
        value->toString(text);
        AptString *key = (AptString *)name->checkedString();
        g_pfnSetExtern(key->mString.c_str(), text.c_str());
    }
    interp->stack.Pop(3);
    if (g_releaseVectorAtE17710->GetNumValues() && interp->stack.GetSize() == 0)
        g_releaseVectorAtE17710->ReleaseValues();
}

// FUNCTION: BFME2 0x00703F40 size=216 name=?_FunctionAptActionIncrement@AptActionInterpreter@@CAXQAU1@QAULocalContextT@1@@Z evidence=N:ledger
void AptActionInterpreter::_FunctionAptActionIncrement(AptActionInterpreter *const interp, LocalContextT *const context)
{
    AptValue *top = interp->stack.At(0);
    AptValue *result;
    if (!(Rva006CD220Get() == 7 && top->isUndefined() && (result = gpUndefinedValue))) {
        if (top->isInteger())
            result = AptInteger::Create(top->toInteger() + 1);
        else
            result = AptFloat::Create(top->toFloat() + 1.0f);
    }
    interp->stack.Pop();
    interp->stack.Push(result);
}

// FUNCTION: BFME2 0x00704020 size=216 name=?_FunctionAptActionDecrement@AptActionInterpreter@@CAXQAU1@QAULocalContextT@1@@Z evidence=N:ledger
void AptActionInterpreter::_FunctionAptActionDecrement(AptActionInterpreter *const interp, LocalContextT *const context)
{
    AptValue *top = interp->stack.At(0);
    AptValue *result;
    if (!(Rva006CD220Get() == 7 && top->isUndefined() && (result = gpUndefinedValue))) {
        if (top->isInteger())
            result = AptInteger::Create(top->toInteger() - 1);
        else
            result = AptFloat::Create(top->toFloat() - 1.0f);
    }
    interp->stack.Pop();
    interp->stack.Push(result);
}

// FUNCTION: BFME2 0x00704100 size=185 name=?_FunctionAptActionBitAnd@AptActionInterpreter@@SAXQAU1@@Z evidence=N:ledger
void AptActionInterpreter::_FunctionAptActionBitAnd(AptActionInterpreter *const interp)
{
    AptValue *top = interp->stack.At(0);
    AptValue *below = interp->stack.At(1);
    AptValue *result = 0;
    if (Rva006CD220Get() == 7 && (top->isUndefined() || below->isUndefined()))
        result = gpUndefinedValue;
    if (!result) {
        int a = top->toInteger();
        int b = below->toInteger();
        result = AptInteger::Create(a & b);
    }
    interp->stack.PopAndPushValue(2, result);
}

// FUNCTION: BFME2 0x007041C0 size=185 name=?_FunctionAptActionBitOr@AptActionInterpreter@@SAXQAU1@@Z evidence=N:ledger
void AptActionInterpreter::_FunctionAptActionBitOr(AptActionInterpreter *const interp)
{
    AptValue *top = interp->stack.At(0);
    AptValue *below = interp->stack.At(1);
    AptValue *result = 0;
    if (Rva006CD220Get() == 7 && (top->isUndefined() || below->isUndefined()))
        result = gpUndefinedValue;
    if (!result) {
        int a = top->toInteger();
        int b = below->toInteger();
        result = AptInteger::Create(a | b);
    }
    interp->stack.PopAndPushValue(2, result);
}

// FUNCTION: BFME2 0x00704280 size=185 name=?_FunctionAptActionBitXor@AptActionInterpreter@@SAXQAU1@@Z evidence=N:ledger
void AptActionInterpreter::_FunctionAptActionBitXor(AptActionInterpreter *const interp)
{
    AptValue *top = interp->stack.At(0);
    AptValue *below = interp->stack.At(1);
    AptValue *result = 0;
    if (Rva006CD220Get() == 7 && (top->isUndefined() || below->isUndefined()))
        result = gpUndefinedValue;
    if (!result) {
        int a = top->toInteger();
        int b = below->toInteger();
        result = AptInteger::Create(a ^ b);
    }
    interp->stack.PopAndPushValue(2, result);
}

// FUNCTION: BFME2 0x00704340 size=187 name=?_FunctionAptActionBitLShift@AptActionInterpreter@@SAXQAU1@@Z evidence=N:ledger
void AptActionInterpreter::_FunctionAptActionBitLShift(AptActionInterpreter *const interp)
{
    AptValue *top = interp->stack.At(0);
    AptValue *below = interp->stack.At(1);
    AptValue *result = 0;
    if (Rva006CD220Get() == 7 && (below->isUndefined() || top->isUndefined()))
        result = gpUndefinedValue;
    if (!result) {
        int a = top->toInteger();
        int b = below->toInteger();
        result = AptInteger::Create(b << a);
    }
    interp->stack.PopAndPushValue(2, result);
}

// FUNCTION: BFME2 0x00704400 size=187 name=?_FunctionAptActionBitRShift@AptActionInterpreter@@SAXQAU1@@Z evidence=N:ledger
void AptActionInterpreter::_FunctionAptActionBitRShift(AptActionInterpreter *const interp)
{
    AptValue *top = interp->stack.At(0);
    AptValue *below = interp->stack.At(1);
    AptValue *result = 0;
    if (Rva006CD220Get() == 7 && (below->isUndefined() || top->isUndefined()))
        result = gpUndefinedValue;
    if (!result) {
        int a = top->toInteger();
        int b = below->toInteger();
        result = AptInteger::Create(b >> a);
    }
    interp->stack.PopAndPushValue(2, result);
}

// FUNCTION: BFME2 0x007044C0 size=528 name=?_FunctionAptActionStrictEquals@AptActionInterpreter@@CAXQAU1@QAULocalContextT@1@@Z evidence=N:ledger
void AptActionInterpreter::_FunctionAptActionStrictEquals(AptActionInterpreter *const interp, LocalContextT *const context)
{
    AptValue *a = interp->stack.At(0);
    AptValue *b = interp->stack.At(1);
    int result = 0;
    if (a->getVtblIndex() == AptVFT_CIHLast)
        a = gpUndefinedValue;
    if (b->getVtblIndex() == AptVFT_CIHLast)
        b = gpUndefinedValue;
    if (Rva006CD220Get() == 7) {
        if (a->isUndefined())
            ++result;
        if (b->isUndefined())
            ++result;
        if (result > 0) {
            interp->stack.PopAndPushValue(2, AptBoolean::Create(result == 2));
            return;
        }
    }
    if (((a->isInteger() || a->isFloat()) && (b->isInteger() || b->isFloat())) || a->getVtblIndex() == b->getVtblIndex()) {
        switch (a->getVtblIndex()) {
        case AptVFT_String:
        case AptVFT_StringObject:
            {
                AptString *stringA = static_cast<AptString *>(a->checkedString());
                AptString *stringB = static_cast<AptString *>(b->checkedString());
                if (stringB->mString.Equals(&stringA->mString))
                    result = 1;
            }
            break;
        case AptVFT_Float: {
            float fa = a->toFloat();
            if (b->isInteger()) {
                int nb = b->toInteger();
                result = (float)fabs(fa - nb) < 0.001f;
            } else {
                float fb = b->toFloat();
                result = (float)fabs(fa - fb) < 0.001f;
            }
            break;
        }
        case AptVFT_Boolean:
        case AptVFT_Integer: {
            int na = a->toInteger();
            if (b->isInteger()) {
                int nb = b->toInteger();
                result = na == nb;
            } else {
                float fb = b->toFloat();
                result = (float)fabs(na - fb) < 0.001f;
            }
            break;
        }
        default:
            result = a == b;
            break;
        }
    }
    interp->stack.PopAndPushValue(2, AptBoolean::Create(result != 0));
}

// FUNCTION: BFME2 0x00704710 size=493 name=?_FunctionAptActionGreater@AptActionInterpreter@@CAXQAU1@QAULocalContextT@1@@Z evidence=N:ledger
void AptActionInterpreter::_FunctionAptActionGreater(AptActionInterpreter *const interp, LocalContextT *const context)
{
    AptValue *top = interp->stack.At(0);
    AptValue *below = interp->stack.At(1);
    if (Rva006CD220Get() == 7 && (top->isUndefined() || below->isUndefined())) {
        interp->stack.Pop(2);
        interp->stack.Push(gpUndefinedValue);
        return;
    }
    int greater;
    if (top->isString() && below->isString()) {
        AptString *belowString = (AptString *)below->checkedString();
        AptString *topString = (AptString *)top->checkedString();
        const char *belowText = belowString->mString.c_str();
        const char *topText = topString->mString.c_str();
        greater = strcmp(topText, belowText) < 0;
    } else if (!top->isFloat() && !below->isFloat()) {
        int belowValue = below->toInteger();
        int topValue = top->toInteger();
        greater = belowValue > topValue;
    } else {
        float belowValue = below->toFloat();
        float topValue = top->toFloat();
        greater = topValue < belowValue;
    }
    interp->stack.PopAndPushValue(2, AptBoolean::Create(greater != 0));
}

// FUNCTION: BFME2 0x00704900 size=172 name=?_FunctionAptActionGotoFrame@AptActionInterpreter@@CAXQAU1@QAULocalContextT@1@@Z evidence=N:ledger
void AptActionInterpreter::_FunctionAptActionGotoFrame(AptActionInterpreter *const interp, LocalContextT *const context)
{
    const int *frame = (const int *)(((unsigned int)context->pInstruction + 3) & ~3u);
    context->pInstruction = (const unsigned char *)(frame + 1);
    AptCIH *cih = 0;
    if (context->pCurWith && context->pCurWith->isCIH(false))
        cih = (AptCIH *)context->pCurWith->checkedCIH(false);
    else if (context->pCurrentContext->isCIH(false))
        cih = (AptCIH *)context->pCurrentContext->checkedCIH(false);
    if (cih) {
        cih->jumpToFrame(*frame);
        spriteInstBaseOf(cih)->flags &= ~0x2000000u;
    }
    if (g_releaseVectorAtE17710->GetNumValues() && interp->stack.GetSize() == 0)
        g_releaseVectorAtE17710->ReleaseValues();
}

// FUNCTION: BFME2 0x007049B0 size=86 name=?_FunctionAptActionStoreRegister@AptActionInterpreter@@CAXQAU1@QAULocalContextT@1@@Z evidence=N:ledger
void AptActionInterpreter::_FunctionAptActionStoreRegister(AptActionInterpreter *const interp, LocalContextT *const context)
{
    const int *operand = (const int *)(((unsigned int)context->pInstruction + 3) & ~3u);
    context->pInstruction = (const unsigned char *)(operand + 1);
    AptScriptFunctionBase::SetRegisterValue(operand[0], interp->stack.At(0));
}

// FUNCTION: BFME2 0x00704A10 size=252 name=?_FunctionAptActionGotoLabel@AptActionInterpreter@@CAXQAU1@QAULocalContextT@1@@Z evidence=N:ledger
void AptActionInterpreter::_FunctionAptActionGotoLabel(AptActionInterpreter *const interp, LocalContextT *const context)
{
    const char *const *data = (const char *const *)(((unsigned int)context->pInstruction + 3) & ~3u);
    context->pInstruction = (const unsigned char *)(data + 1);
    EAStringC label(*data);
    AptCIH *target;
    if (context->pCurWith && context->pCurWith->isCIH(false))
        target = (AptCIH *)context->pCurWith->checkedCIH(false);
    else
        target = context->pCurrentContext;
    int frame = ((AptCharacterInst *)target->getSpriteInstBase())->pCharacter->animation.movie.labelToFrame(&label);
    if (frame >= 0) {
        target->jumpToFrame(frame);
        target->getSpriteInstBase()->flags &= ~0x2000000u;
    }
}

// FUNCTION: BFME2 0x00704B10 size=57 name=?_FunctionAptActionPush@AptActionInterpreter@@CAXQAU1@QAULocalContextT@1@@Z evidence=N:ledger
void AptActionInterpreter::_FunctionAptActionPush(AptActionInterpreter *const interpreter, LocalContextT *const localContext)
{
    localContext->pInstruction=(const unsigned char *)(((unsigned int)localContext->pInstruction+3)&~3U);
    const AptConstantPool *data=(const AptConstantPool *)localContext->pInstruction;
    localContext->pInstruction+=sizeof(AptConstantPool);
    for (int i=0;i<data->nItems;++i) interpreter->stackPushIndirect(data->apItems[i]);
}

// FUNCTION: BFME2 0x00704B50 size=261 name=?_FunctionAptActionDefineFunction@AptActionInterpreter@@CAXQAU1@QAULocalContextT@1@@Z evidence=N:ledger
void AptActionInterpreter::_FunctionAptActionDefineFunction(AptActionInterpreter *const interp, LocalContextT *const context)
{
    const AptAction_DefineFunction *data = (const AptAction_DefineFunction *)(((unsigned int)context->pInstruction + 3) & ~3u);
    context->pInstruction = (const unsigned char *)(data + 1);
    context->pInstruction += data->nCodeSize;
    data->constantPool = interp->constantPool;
    EAStringC name(data->szName);
    AptCIH *cih = context->pCurrentContext;
    AptScriptFunction1 *function = new AptScriptFunction1(interp->currentFunction, data, cih);
    if (data->szName[0] == 0) {
        interp->stack.Push(function);
    } else {
        name = data->szName;
        interp->setVariable(context->pCurrentContext, context->pCurWith, &name, function, 1, 1, 0);
    }
}

// FUNCTION: BFME2 0x00704C60 size=239 name=?_FunctionAptActionDefineFunction2@AptActionInterpreter@@CAXQAU1@QAULocalContextT@1@@Z evidence=N:ledger
void AptActionInterpreter::_FunctionAptActionDefineFunction2(AptActionInterpreter *const interp, LocalContextT *const context)
{
    const AptAction_DefineFunction2 *data = (const AptAction_DefineFunction2 *)(((unsigned int)context->pInstruction + 3) & ~3u);
    context->pInstruction = (const unsigned char *)(data + 1);
    context->pInstruction += data->nCodeSize;
    data->constantPool = interp->constantPool;
    AptCIH *cih = context->pCurrentContext;
    AptScriptFunction2 *function = new AptScriptFunction2(interp->currentFunction, data, cih);
    if (data->szName[0] == 0) {
        interp->stack.Push(function);
    } else {
        EAStringC name(data->szName);
        interp->setVariable(context->pCurrentContext, context->pCurWith, &name, function, 1, 1, 0);
    }
}

// FUNCTION: BFME2 0x00704D50 size=179 name=?_FunctionAptActionBranchIfTrue@AptActionInterpreter@@CAXQAU1@QAULocalContextT@1@@Z evidence=N:ledger
void AptActionInterpreter::_FunctionAptActionBranchIfTrue(AptActionInterpreter *const interp, LocalContextT *const context)
{
    const int *operand = (const int *)(((unsigned int)context->pInstruction + 3) & ~3u);
    context->pInstruction = (const unsigned char *)(operand + 1);
    if (interp->stack.At(0)->toBool() == true)
        context->pInstruction += operand[0];
    interp->stack.Pop();
    if (g_releaseVectorAtE17710->GetNumValues() && interp->stack.m_nElements == 0)
        g_releaseVectorAtE17710->ReleaseValues();
}

// FUNCTION: BFME2 0x00704E10 size=455 name=?_FunctionAptActionCallFrame@AptActionInterpreter@@CAXQAU1@QAULocalContextT@1@@Z evidence=N:ledger
void AptActionInterpreter::_FunctionAptActionCallFrame(AptActionInterpreter *const interp, LocalContextT *const context)
{
    AptValue *value = interp->stack.At(0);
    int frame = -1;
    if (value->isString()) {
        EAStringC name;
        AptValue *labelContext;
        AptString *label = (AptString *)value->checkedString();
        rva006FEC00((int)context->pCurrentContext, (int)context->pCurWith, &label->mString, (int *)&labelContext, &name);
        frame = ((AptCharacterInst *)((AptCIH *)labelContext->checkedCIH(false))->getCharacterInst())->pCharacter->animation.movie.labelToFrame(&name);
    } else if (value->isInteger()) {
        frame = value->toInteger();
    } else {
        APT_AI_ASSERT("NOT_REACHED", 0x1efd);
    }
    interp->stack.Pop();
    if (frame != -1)
        ((AptCharacterInst *)context->pCurrentContext->getCharacterInst())->pCharacter->animation.movie.runFrameControls(context->pCurrentContext, frame);
}

// FUNCTION: BFME2 0x00704FE0 size=537 name=?_FunctionAptActionGotoFrame2@AptActionInterpreter@@CAXQAU1@QAULocalContextT@1@@Z evidence=N:ledger
void AptActionInterpreter::_FunctionAptActionGotoFrame2(AptActionInterpreter *const interp, LocalContextT *const context)
{
    const int *data = (const int *)(((unsigned int)context->pInstruction + 3) & ~3u);
    context->pInstruction = (const unsigned char *)(data + 1);
    AptValue *value = interp->stack.At(0);
    AptCIH *cih = 0;
    if (context->pCurWith && context->pCurWith->isCIH(false))
        cih = (AptCIH *)context->pCurWith->checkedCIH(false);
    else if (context->pCurrentContext->isCIH(false))
        cih = (AptCIH *)context->pCurrentContext->checkedCIH(false);
    int frame = -1;
    if (value->isString()) {
        EAStringC name;
        AptValue *volatile labelContext;
        AptString *label = (AptString *)value->checkedString();
        rva006FEC00((int)context->pCurrentContext, (int)context->pCurWith, &label->mString, (int *)&labelContext, &name);
        AptValue *resolved = labelContext;
        if (resolved->isCIH(false) && ((AptCIH *)resolved->checkedCIH(false))->isSpriteInstBase())
            frame = ((AptCharacterInst *)((AptCIH *)resolved->checkedCIH(false))->GetSpriteInstBase())->pCharacter->animation.movie.labelToFrame(&name);
    } else if (value->isInteger()) {
        frame = value->toInteger();
    }
    if (frame != -1) {
        cih->jumpToFrame(frame);
        spriteInstBaseOf(cih)->bits.mbIsPlaying = (*data != 0);
    }
    interp->stack.Pop();
}

// FUNCTION: BFME2 0x00705200 size=59 name=?_FunctionAptActionBranchAlways@AptActionInterpreter@@CAXQAU1@QAULocalContextT@1@@Z evidence=N:ledger
void AptActionInterpreter::_FunctionAptActionBranchAlways(AptActionInterpreter *const interpreter, LocalContextT *const localContext)
{
    localContext->pInstruction=(const unsigned char *)(((unsigned int)localContext->pInstruction+3)&~3U);
    const int *data=(const int *)localContext->pInstruction;
    localContext->pInstruction+=4;
    localContext->pInstruction+=*data;
    if (g_releaseVectorAtE17710->GetNumValues()!=0 && interpreter->stack.m_nElements==0)
        g_releaseVectorAtE17710->ReleaseValues();
}

// FUNCTION: BFME2 0x00705240 size=42 name=?_FunctionAptActionPushThis@AptActionInterpreter@@CAXQAU1@QAULocalContextT@1@@Z evidence=N:ledger
void AptActionInterpreter::_FunctionAptActionPushThis(AptActionInterpreter *const interp, LocalContextT *const context)
{
    AptString *string = AptString::Create();
    string->mString = *StringPool::GetString(0xa4);
    interp->stack.Push(string);
}

// FUNCTION: BFME2 0x00705270 size=39 name=?_FunctionAptActionPushGlobal@AptActionInterpreter@@CAXQAU1@QAULocalContextT@1@@Z evidence=N:ledger
void AptActionInterpreter::_FunctionAptActionPushGlobal(AptActionInterpreter *const interp, LocalContextT *const context)
{
    AptString *string = AptString::Create();
    string->mString = *StringPool::GetString(7);
    interp->stack.Push(string);
}

// FUNCTION: BFME2 0x007052A0 size=21 name=?_FunctionAptActionPush0@AptActionInterpreter@@CAXQAU1@QAULocalContextT@1@@Z evidence=N:ledger
void AptActionInterpreter::_FunctionAptActionPush0(AptActionInterpreter *const interp, LocalContextT *const context)
{
    interp->stack.Push(AptInteger::Create(0));
}

// FUNCTION: BFME2 0x007052C0 size=21 name=?_FunctionAptActionPush1@AptActionInterpreter@@CAXQAU1@QAULocalContextT@1@@Z evidence=N:ledger
void AptActionInterpreter::_FunctionAptActionPush1(AptActionInterpreter *const interp, LocalContextT *const context)
{
    interp->stack.Push(AptInteger::Create(1));
}

// FUNCTION: BFME2 0x007052E0 size=21 name=?_FunctionAptActionPushTrue@AptActionInterpreter@@CAXQAU1@QAULocalContextT@1@@Z evidence=N:ledger
void AptActionInterpreter::_FunctionAptActionPushTrue(AptActionInterpreter *const interp, LocalContextT *const context)
{
    interp->stack.Push(AptBoolean::Create(true));
}

// FUNCTION: BFME2 0x00705300 size=21 name=?_FunctionAptActionPushFalse@AptActionInterpreter@@CAXQAU1@QAULocalContextT@1@@Z evidence=N:ledger
void AptActionInterpreter::_FunctionAptActionPushFalse(AptActionInterpreter *const interp, LocalContextT *const context)
{
    interp->stack.Push(AptBoolean::Create(false));
}

// FUNCTION: BFME2 0x00705320 size=16 name=?_FunctionAptActionPushUndefined@AptActionInterpreter@@CAXQAU1@QAULocalContextT@1@@Z evidence=N:ledger
void AptActionInterpreter::_FunctionAptActionPushUndefined(AptActionInterpreter *const interpreter, LocalContextT *const localContext) { interpreter->stack.Push(gpUndefinedValue); }

// FUNCTION: BFME2 0x00705330 size=54 name=?_FunctionAptActionPushThisVariable@AptActionInterpreter@@CAXQAU1@QAULocalContextT@1@@Z evidence=N:ledger
void AptActionInterpreter::_FunctionAptActionPushThisVariable(AptActionInterpreter *const interp, LocalContextT *const context)
{
    interp->stack.Push(interp->getVariable(context->pCurrentContext, context->pCurWith, StringPool::GetString(0xa4), 1, 1, 0));
}

// FUNCTION: BFME2 0x00705370 size=16 name=?_FunctionAptActionPushGlobalVariable@AptActionInterpreter@@CAXQAU1@QAULocalContextT@1@@Z evidence=N:ledger
void AptActionInterpreter::_FunctionAptActionPushGlobalVariable(AptActionInterpreter *const interpreter, LocalContextT *const localContext)
{
    interpreter->stack.Push(gpGlobalGlobalObject);
}

// FUNCTION: BFME2 0x00705380 size=39 name=?_FunctionAptActionPushZeroSetVar@AptActionInterpreter@@CAXQAU1@QAULocalContextT@1@@Z evidence=N:ledger
void AptActionInterpreter::_FunctionAptActionPushZeroSetVar(AptActionInterpreter *const interp, LocalContextT *const context)
{
    interp->stack.Push(AptInteger::Create(0));
    _FunctionAptActionSetVariable(interp, context);
}

// FUNCTION: BFME2 0x007053B0 size=124 name=?_FunctionAptActionPushString@AptActionInterpreter@@CAXQAU1@QAULocalContextT@1@@Z evidence=N:ledger
void AptActionInterpreter::_FunctionAptActionPushString(AptActionInterpreter *const interp, LocalContextT *const context)
{
    const char *const *operand = (const char *const *)(((unsigned int)context->pInstruction + 3) & ~3u);
    context->pInstruction = (const unsigned char *)(operand + 1);
    AptString *text = AptString::Create();
    {
        EAStringC name(*operand);
        text->mString = name;
    }
    interp->stack.Push(text);
}

// FUNCTION: BFME2 0x00705430 size=31 name=?_FunctionAptActionPushStringDictByte@AptActionInterpreter@@CAXQAU1@QAULocalContextT@1@@Z evidence=N:ledger
void AptActionInterpreter::_FunctionAptActionPushStringDictByte(AptActionInterpreter *const interpreter, LocalContextT *const localContext)
{
    unsigned char index=*localContext->pInstruction++;
    interpreter->stack.Push(interpreter->constantPool.apItems[index]);
}

// FUNCTION: BFME2 0x00705450 size=44 name=?_FunctionAptActionPushStringDictWord@AptActionInterpreter@@CAXQAU1@QAULocalContextT@1@@Z evidence=N:ledger
void AptActionInterpreter::_FunctionAptActionPushStringDictWord(AptActionInterpreter *const interp, LocalContextT *const context)
{
    const unsigned char *code = context->pInstruction;
    unsigned short index;
    unsigned char *bytes = (unsigned char *)&index;
    bytes[0] = *code++;
    bytes[1] = *code++;
    context->pInstruction = code;
    interp->stack.Push(interp->constantPool.apItems[index]);
}

// FUNCTION: BFME2 0x00705480 size=145 name=?_FunctionAptActionPushStringGetVar@AptActionInterpreter@@CAXQAU1@QAULocalContextT@1@@Z evidence=N:ledger
void AptActionInterpreter::_FunctionAptActionPushStringGetVar(AptActionInterpreter *const interp, LocalContextT *const context)
{
    const char *const *operand = (const char *const *)(((unsigned int)context->pInstruction + 3) & ~3u);
    context->pInstruction = (const unsigned char *)(operand + 1);
    {
        EAStringC name(*operand);
        g_aptScratchString = name;
    }
    AptValue *value = interp->getVariable(context->pCurrentContext, context->pCurWith, &g_aptScratchString, 1, 1, 0);
    interp->stack.Push(value);
}

// FUNCTION: BFME2 0x00705520 size=138 name=?_FunctionAptActionPushStringGetMember@AptActionInterpreter@@CAXQAU1@QAULocalContextT@1@@Z evidence=N:ledger
void AptActionInterpreter::_FunctionAptActionPushStringGetMember(AptActionInterpreter *const interp, LocalContextT *const context)
{
    const char *const *operand = (const char *const *)(((unsigned int)context->pInstruction + 3) & ~3u);
    context->pInstruction = (const unsigned char *)(operand + 1);
    AptString *text = AptString::Create();
    {
        EAStringC name(*operand);
        text->mString = name;
    }
    interp->stack.Push(text);
    _FunctionAptActionGetMember(interp, context);
}

// FUNCTION: BFME2 0x007055B0 size=138 name=?_FunctionAptActionPushStringSetVar@AptActionInterpreter@@CAXQAU1@QAULocalContextT@1@@Z evidence=N:ledger
void AptActionInterpreter::_FunctionAptActionPushStringSetVar(AptActionInterpreter *const interp, LocalContextT *const context)
{
    const char *const *operand = (const char *const *)(((unsigned int)context->pInstruction + 3) & ~3u);
    context->pInstruction = (const unsigned char *)(operand + 1);
    AptString *text = AptString::Create();
    {
        EAStringC name(*operand);
        text->mString = name;
    }
    interp->stack.Push(text);
    _FunctionAptActionSetVariable(interp, context);
}

// FUNCTION: BFME2 0x00705640 size=138 name=?_FunctionAptActionPushStringSetMember@AptActionInterpreter@@CAXQAU1@QAULocalContextT@1@@Z evidence=N:ledger
void AptActionInterpreter::_FunctionAptActionPushStringSetMember(AptActionInterpreter *const interp, LocalContextT *const context)
{
    const char *const *operand = (const char *const *)(((unsigned int)context->pInstruction + 3) & ~3u);
    context->pInstruction = (const unsigned char *)(operand + 1);
    AptString *text = AptString::Create();
    {
        EAStringC name(*operand);
        text->mString = name;
    }
    interp->stack.Push(text);
    _FunctionAptActionSetMember(interp, context);
}

// FUNCTION: BFME2 0x007056D0 size=67 name=?_FunctionAptActionStringDictByteGetVar@AptActionInterpreter@@CAXQAU1@QAULocalContextT@1@@Z evidence=N:ledger
void AptActionInterpreter::_FunctionAptActionStringDictByteGetVar(AptActionInterpreter *const interp, LocalContextT *const context)
{
    unsigned char index = *context->pInstruction++;
    AptValue *entry = interp->constantPool.apItems[index];
    AptString *name = (AptString *)entry->checkedString();
    AptValue *result = interp->getVariable(context->pCurrentContext, context->pCurWith, &name->mString, 1, 1, 0);
    interp->stack.Push(result);
}
// FUNCTION: BFME2 0x00705720 size=47 name=?_FunctionAptActionStringDictByteGetMember@AptActionInterpreter@@CAXQAU1@QAULocalContextT@1@@Z evidence=N:ledger
void AptActionInterpreter::_FunctionAptActionStringDictByteGetMember(AptActionInterpreter *const interpreter, LocalContextT *const localContext)
{
    unsigned char index=*localContext->pInstruction++;
    interpreter->stack.Push(interpreter->constantPool.apItems[index]);
    _FunctionAptActionGetMember(interpreter,localContext);
}

// FUNCTION: BFME2 0x00705750 size=60 name=?_FunctionAptActionPushFloat@AptActionInterpreter@@CAXQAU1@QAULocalContextT@1@@Z evidence=N:ledger
void AptActionInterpreter::_FunctionAptActionPushFloat(AptActionInterpreter *const interp, LocalContextT *const context)
{
    const unsigned char *code = context->pInstruction;
    float value;
    unsigned char *bytes = (unsigned char *)&value;
    bytes[0] = *code++;
    bytes[1] = *code++;
    bytes[2] = *code++;
    bytes[3] = *code++;
    context->pInstruction = code;
    interp->stack.Push(AptFloat::Create(value));
}

// FUNCTION: BFME2 0x00705790 size=34 name=?_FunctionAptActionPushByte@AptActionInterpreter@@CAXQAU1@QAULocalContextT@1@@Z evidence=N:ledger
void AptActionInterpreter::_FunctionAptActionPushByte(AptActionInterpreter *const interp, LocalContextT *const context)
{
    signed char value = (signed char)*context->pInstruction++;
    interp->stack.Push(AptInteger::Create(value));
}

// FUNCTION: BFME2 0x007057C0 size=47 name=?_FunctionAptActionPushWord@AptActionInterpreter@@CAXQAU1@QAULocalContextT@1@@Z evidence=N:ledger
void AptActionInterpreter::_FunctionAptActionPushWord(AptActionInterpreter *const interp, LocalContextT *const context)
{
    const unsigned char *code = context->pInstruction;
    short value;
    unsigned char *bytes = (unsigned char *)&value;
    bytes[0] = *code++;
    bytes[1] = *code++;
    context->pInstruction = code;
    interp->stack.Push(AptInteger::Create(value));
}

// FUNCTION: BFME2 0x007057F0 size=60 name=?_FunctionAptActionPushDWord@AptActionInterpreter@@CAXQAU1@QAULocalContextT@1@@Z evidence=N:ledger
void AptActionInterpreter::_FunctionAptActionPushDWord(AptActionInterpreter *const interp, LocalContextT *const context)
{
    const unsigned char *code = context->pInstruction;
    int value;
    unsigned char *bytes = (unsigned char *)&value;
    bytes[0] = *code++;
    bytes[1] = *code++;
    bytes[2] = *code++;
    bytes[3] = *code++;
    context->pInstruction = code;
    interp->stack.Push(AptInteger::Create(value));
}

// FUNCTION: BFME2 0x00705830 size=179 name=?_FunctionAptActionBranchIfFalse@AptActionInterpreter@@CAXQAU1@QAULocalContextT@1@@Z evidence=N:ledger
void AptActionInterpreter::_FunctionAptActionBranchIfFalse(AptActionInterpreter *const interp, LocalContextT *const context)
{
    const int *operand = (const int *)(((unsigned int)context->pInstruction + 3) & ~3u);
    context->pInstruction = (const unsigned char *)(operand + 1);
    if (!interp->stack.At(0)->toBool())
        context->pInstruction += operand[0];
    interp->stack.Pop();
    if (g_releaseVectorAtE17710->GetNumValues() && interp->stack.m_nElements == 0)
        g_releaseVectorAtE17710->ReleaseValues();
}

// FUNCTION: BFME2 0x007058F0 size=656 name=?_FunctionAptActionExtends@AptActionInterpreter@@CAXQAU1@QAULocalContextT@1@@Z evidence=N:ledger
void AptActionInterpreter::_FunctionAptActionExtends(AptActionInterpreter *const interp, LocalContextT *const context)
{
    if (!(interp->stack.GetSize() >= 2)) {
        APT_AI_ASSERT("pInterpreter->stack.GetSize() >= 2", 0x23fe);
    }
    AptValue *superClass = interp->stack.At(0);
    AptValue *subClass = interp->stack.At(1);
    if (superClass->ContainsNativeHashVirtual() && subClass->isScriptFunction()) {
        AptNativeHash *superHash = superClass->GetNativeHashVirtual();
        AptNativeHash *subHash = subClass->GetNativeHashVirtual();
        AptValue *superPrototype = superHash->mpPrototype;
        AptValue *subPrototype = subHash->mpPrototype;
        if (!superPrototype) {
            superPrototype = new AptPrototype();
            if (superPrototype)
                superPrototype->AddRef();
            if (superHash->mpPrototype)
                superHash->mpPrototype->Release();
            superHash->mpPrototype = superPrototype;
        }
        if (!subPrototype) {
            subPrototype = new AptPrototype();
            if (subPrototype)
                subPrototype->AddRef();
            if (subHash->mpPrototype)
                subHash->mpPrototype->Release();
            subHash->mpPrototype = subPrototype;
        }
        if (!subPrototype->isPrototype()) {
            APT_AI_ASSERT("pSubPrototype->isPrototype() && \"Object Extending has invalid prototype object!\"", 0x2415);
        }
        ((AptPrototype *)subPrototype->checkedPrototype())->SetSuperConstructor(superClass);
        superClass->setHasClass(1);
        subClass->setHasClass(1);
        AptNativeHash *prototypeHash = subPrototype->GetNativeHashVirtual();
        if (superPrototype)
            superPrototype->AddRef();
        if (prototypeHash->mp__proto__)
            prototypeHash->mp__proto__->Release();
        prototypeHash->mp__proto__ = superPrototype;
    } else {
        Rva006CC110Log(3, "--AptWarning-- Actionscript is attempting to use invalid objects in Extends Opcode.");
    }
    interp->stack.Pop(2);
}

// FUNCTION: BFME2 0x00705B80 size=238 name=?_FunctionAptActionInstanceOf@AptActionInterpreter@@CAXQAU1@QAULocalContextT@1@@Z evidence=N:ledger
void AptActionInterpreter::_FunctionAptActionInstanceOf(AptActionInterpreter *const interp, LocalContextT *const context)
{
    if (!(interp->stack.GetSize() >= 2)) {
        g_bfmeAptAssertAtE17734("pInterpreter->stack.GetSize() >= 2", "C:\\projects\\bfme2patch103\\bfme2\\Code\\Libraries\\Source\\Apt\\AptActionInterpreter.cpp", 0x248d);
        if (g_bfmeAptBreakOnAssertAtDDC01C) APT_ASSERT_BREAK();
    }
    if (interp->stack.GetSize() < 2) {
        g_bfmeAptAssertAtE17734("false && \"[APT] Actionscript InstanceOf Op did not find enough parameters. Check Script code.\"", "C:\\projects\\bfme2patch103\\bfme2\\Code\\Libraries\\Source\\Apt\\AptActionInterpreter.cpp", 0x2491);
        if (g_bfmeAptBreakOnAssertAtDDC01C) APT_ASSERT_BREAK();
        interp->stack.PopAndPushValue(interp->stack.GetSize(), gpUndefinedValue);
        return;
    }
    AptValue *object = interp->stack.At(0);
    AptValue *type = interp->stack.At(1);
    bool result = isObjectOfType(object, type);
    interp->stack.PopAndPushValue(2, AptBoolean::Create(result));
}

// FUNCTION: BFME2 0x00705C70 size=287 name=?_FunctionAptActionCastOp@AptActionInterpreter@@CAXQAU1@QAULocalContextT@1@@Z evidence=N:ledger
void AptActionInterpreter::_FunctionAptActionCastOp(AptActionInterpreter *const interp, LocalContextT *const context)
{
    if (interp->stack.GetSize() < 2) {
        APT_AI_ASSERT("false && \"[APT] Actionscript Cast Op did not find enough parameters. Check Script code.\"", 0x24c9);
        interp->stack.PopAndPushValue(interp->stack.GetSize(), gpUndefinedValue);
        return;
    }
    AptValue *object = interp->stack.At(0);
    AptValue *interfaceValue = interp->stack.At(1);
    if (isObjectOfType(object, interfaceValue)) {
        interp->stack.PopAndPushValue(2, object);
    } else {
        interp->stack.Pop(2);
        interp->stack.PushNoAddRef(gpUndefinedValue);
    }
}

// FUNCTION: BFME2 0x00705D90 size=721 name=?_FunctionAptActionImplementsOp@AptActionInterpreter@@CAXQAU1@QAULocalContextT@1@@Z evidence=N:ledger
void AptActionInterpreter::_FunctionAptActionImplementsOp(AptActionInterpreter *const interp, LocalContextT *const context)
{
    EAStringC key;
    if (!(interp->stack.GetSize() > 2)) {
        APT_AI_ASSERT("pInterpreter->stack.GetSize() > 2", 0x2505);
    }
    AptValue *object = interp->stack.At(0);
    int nInterfaces = interp->stack.At(1)->toInteger();
    if (!(object->isScriptFunction() || object->isNativeFunction())) {
        if (!(object->isScriptFunction() || object->isNativeFunction())) {
            APT_AI_ASSERT("pObject->isScriptFunction() || pObject->isNativeFunction()", 0x250f);
        }
        Rva006CC110Log(4, "Implements Opcode attempting to mess with invalid object.");
        interp->stack.rva006E3AA0(nInterfaces + 2);
        return;
    }
    if (!(nInterfaces > 0)) {
        APT_AI_ASSERT("nInterfaces > 0", 0x2515);
    }
    AptArray *interfaces = new AptArray();
    for (int i = 0; i < nInterfaces; ++i) {
        AptValue *entry = interp->stack.At(i + 2);
        if (!entry->ContainsNativeHashVirtual()) {
            APT_AI_ASSERT("pTemp->ContainsNativeHashVirtual()", 0x251d);
        }
        AptNativeHash *entryHash = entry->GetNativeHashVirtual();
        AptValue *prototype = entryHash->mpPrototype;
        if (!prototype) {
            prototype = new AptPrototype();
            if (prototype)
                prototype->AddRef();
            if (entryHash->mpPrototype)
                entryHash->mpPrototype->Release();
            entryHash->mpPrototype = prototype;
        }
        interfaces->rva006D95E0(i, prototype);
    }
    if (object->GetNativeHashVirtual()) {
        EAStringC name("__INTERFACES__");
        object->GetNativeHashVirtual()->Set(&name, interfaces);
    }
    interp->stack.rva006E3AA0(nInterfaces + 2);
}

// FUNCTION: BFME2 0x00706070 size=453 name=?_FunctionAptActionTry@AptActionInterpreter@@CAXQAU1@QAULocalContextT@1@@Z evidence=N:ledger
void AptActionInterpreter::_FunctionAptActionTry(AptActionInterpreter *const interp, LocalContextT *const context)
{
    unsigned int nPreStackSize = interp->stack.GetSize();
    const AptAction_Try *data = (const AptAction_Try *)(((unsigned int)context->pInstruction + 3) & ~3u);
    context->pInstruction = (const unsigned char *)(data + 1);
    context->pInstruction += data->trySize;
    context->pInstruction += data->catchSize;
    context->pInstruction += data->finallySize;
    interp->runStream((const unsigned char *)(data + 1), context->pCurrentContext, data->trySize, context->pParentCharacter);
    AptValue *thrown = interp->thrownValue;
    if (thrown && (data->flags & 1)) {
        if (data->flags & 4) {
            AptScriptFunctionBase::SetRegisterValue(data->caughtReg, thrown);
        } else {
            EAStringC name(data->caughtName);
            if (interp->currentFunction) {
                if (!AptScriptFunctionBase::spFrameStack)
                    interp->currentFunction->CreateFrameStack();
                AptScriptFunctionBase::SetScopeVariable(&name, thrown);
            } else {
                interp->setVariable(context->pCurrentContext, 0, &name, thrown, 1, 1, 0);
            }
        }
        interp->thrownValue->Release();
        interp->thrownValue = 0;
        interp->runStream((const unsigned char *)(data + 1) + data->trySize, context->pCurrentContext, data->catchSize, context->pParentCharacter);
    }
    if (data->flags & 2) {
        AptValue *pending = interp->thrownValue;
        if (pending) {
            pending->AddRef();
            interp->thrownValue->Release();
            interp->thrownValue = 0;
        }
        interp->runStream((const unsigned char *)(data + 1) + data->trySize + data->catchSize, context->pCurrentContext, data->finallySize, context->pParentCharacter);
        if (pending && !interp->thrownValue) {
            pending->AddRef();
            interp->thrownValue = pending;
            pending->Release();
        }
    }
    unsigned int nPostStackSize = interp->stack.GetSize();
    if (nPostStackSize > nPreStackSize)
        interp->stack.rva006E3AA0(nPostStackSize - nPreStackSize);
    if ((unsigned int)interp->stack.GetSize() != nPreStackSize) {
        APT_AI_ASSERT("(uint32_t)pInterpreter->stack.GetSize() == nPreStackSize", 0x2599);
    }
}

// FUNCTION: BFME2 0x00706240 size=163 name=?_FunctionAptActionThrow@AptActionInterpreter@@CAXQAU1@QAULocalContextT@1@@Z evidence=N:ledger
void AptActionInterpreter::_FunctionAptActionThrow(AptActionInterpreter *const interp, LocalContextT *const context)
{
    if (!(interp->stack.GetSize() >= 1)) {
        g_bfmeAptAssertAtE17734("pInterpreter->stack.GetSize() >= 1", "C:\\projects\\bfme2patch103\\bfme2\\Code\\Libraries\\Source\\Apt\\AptActionInterpreter.cpp", 0x25bc);
        if (g_bfmeAptBreakOnAssertAtDDC01C) APT_ASSERT_BREAK();
    }
    AptValue *thrown = interp->stack.At(0);
    thrown->AddRef();
    interp->thrownValue = thrown;
    interp->stack.Pop();
}

// FUNCTION: BFME2 0x007062F0 size=53 name=?rva007062F0@AptBasePtrStack@@QAEPAVBfmeAptValue006DCD20@@XZ evidence=?:placeholder
AptValue *AptBasePtrStack::rva007062F0()
{
    return At(0);
}

// FUNCTION: BFME2 0x00706330 size=57 name=?rva00706330@Rva00706330Stack@@QAEXXZ evidence=?:placeholder

// FUNCTION: BFME2 0x00706370 size=369 name=?loadVariables@AptActionInterpreter@@QAEXPAVAptValue@@0PBVEAStringC@@@Z evidence=N:ledger
void AptActionInterpreter::loadVariables(AptValue *context, AptValue *with, const EAStringC *url)
{
    AptValue *value;
    if (!url) {
        if (!g_pfnLoadVariablesNULL) {
            APT_AI_ASSERT("gAptFuncs.pfnLoadVariablesNULL", 0x3a3);
        }
        value = g_pfnLoadVariablesNULL();
    } else {
        value = g_pfnLoadVariables(url->c_str());
    }
    EAStringC *text = 0;
    if (value->isString())
        text = &((AptString *)value->checkedString())->mString;
    else
        value->toString(*text);
    const char *cursor = text->c_str();
    EAStringC key;
    EAStringC val;
    cursor = urlDecode(cursor, key, val);
    while (cursor) {
        if (key.IsEmpty()) {
            Rva006CC110Log(4, "loadVariable for '%s' returned empty variable name\n", url->c_str());
        } else {
            AptString *variable = AptString::Create();
            variable->mString = val;
            setVariable(context, with, &key, variable, 1, 1, 0);
        }
        cursor = urlDecode(cursor, key, val);
    }
}

// FUNCTION: BFME2 0x007064F0 size=229 name=?getObject@AptActionInterpreter@@CAPAVAptValue@@PAV2@0PBVEAStringC@@@Z evidence=N:ledger
AptValue *AptActionInterpreter::getObject(AptValue *current, AptValue *with, const EAStringC *path)
{
    EAStringC name;
    if (path->GetLength() == 0)
        return current;
    AptValue *context;
    rva006FEC00((int)current, (int)with, (EAStringC *)path, (int *)&context, &name);
    if (context) {
        AptValue *value = context->findChild(&name, with);
        if (value && value->ContainsNativeHashVirtual())
            return value;
    }
    return 0;
}

// FUNCTION: BFME2 0x007065E0 size=121 name=?rva007065e0@@YAXHHPAVBfmeAptValue006DCD20@@PAPAV1@@Z evidence=?:placeholder
void aptConvertWith(AptValue *context, AptValue *with, AptValue *target, AptValue **result)
{
    if (target->isCIH(false) || target->ContainsNativeHashVirtual()) {
        *result = target;
        return;
    }
    if (target->isString()) {
        *result = AptActionInterpreter::getObject(context, with, &((AptString *)target->checkedString())->mString);
        return;
    }
    APT_AI_ASSERT("NOT_REACHED", 0x7a6);
}

// FUNCTION: BFME2 0x00706660 size=742 name=?callFunction@AptActionInterpreter@@QAEXPAVAptValue@@0H@Z evidence=N:ledger
void AptActionInterpreter::callFunction(AptValue *context, AptValue *function, int nParams)
{
    int nStackElementsPre = stack.GetSize() - nParams;
    if (function && function->isNativeFunction()) {
        AptConstantPool savedPool = constantPool;
        stack.PopAndPush(nParams, ((AptNativeFunction *)function->checkedNativeFunction())->mpFunction(context, nParams));
        constantPool = savedPool;
    } else if (function && function->isScriptFunction()) {
        AptConstantPool savedPool = constantPool;
        AptScriptFunctionBase *pPrevFunction = currentFunction;
        currentFunction = (AptScriptFunctionBase *)function->checkedScriptFunction();
        constantPool = currentFunction->GetConstantPool();
        if (currentFunction->mpCIH->isUndefined()
            || (currentFunction->mpCIH->isLevelInst() && (currentFunction->mpCIH->mFlags & 0xC0000) == 0)
            || (currentFunction->mpCIH->isLevelInst() && (currentFunction->mpCIH->mFlags & 0xC0000) == 0x80000)) {
            stack.rva006E3AA0(nParams);
            stack.PushRawNoAddRef(gpUndefinedValue);
            currentFunction->mpCIH = (AptCIH *)gpUndefinedValue;
        } else {
            thisStack.Push(context);
            currentFunction->AddRef();
            _AptScriptFunctionState state;
            currentFunction->SetupBeforeExecution(&state, context);
            const int nFormalArgs = currentFunction->GetNumArguments();
            const int nArgs = nFormalArgs < nParams ? nFormalArgs : nParams;
            int i;
            for (i = 0; i < nArgs; ++i) {
                AptValue *arg = stack.At(i);
                currentFunction->SetArgument(arg, i);
            }
            for (; i < nFormalArgs; ++i)
                currentFunction->SetArgument(gpUndefinedValue, i);
            stack.rva006E3AA0(nParams);
            const AptCIH *root = currentFunction->mpParentAnim->rva006E0CB0();
            AptCharacterInst *character = root ? root->rva006CD650() : 0;
            runStream(currentFunction->GetByteCodeBase(), currentFunction->mpCIH,
                      currentFunction->GetByteCodeSize(), character);
            currentFunction->CleanupAfterExecution(&state);
            currentFunction->Release();
            thisStack.Pop();
        }
        currentFunction = pPrevFunction;
        constantPool = savedPool;
    } else {
        stack.rva006E3AA0(nParams);
        stack.PushRawNoAddRef(gpUndefinedValue);
    }
    int nStackElementsPost = stack.GetSize();
    if (thrownValue) {
        if (nStackElementsPost > nStackElementsPre)
            stack.rva006E3AA0(nStackElementsPost - nStackElementsPre);
    } else if (!(nStackElementsPost == nStackElementsPre || nStackElementsPost == nStackElementsPre + 1)) {
        APT_AI_ASSERT("nStackElementsPost == nStackElementsPre || nStackElementsPost == (nStackElementsPre+1)", 0x837);
    }
}

// FUNCTION: BFME2 0x00706950 size=234 name=?CleanupAfterExecution@AptActionInterpreter@@QAEXPAXPAUAptActionSetup@@@Z evidence=N:ledger
void AptActionInterpreter::CleanupAfterExecution(void *saveBase, AptActionSetup *setup)
{
    AptValue *pending = thrownValue;
    if (pending) {
        EAStringC message;
        pending->toString(message);
        Rva006CC110Log(3, "<WARNING> Actionscript un-caught exception encountered during \"%s\"\n", setup->pDescription);
        Rva006CC110Log(3, "<WARNING> Actionscript error message: \"%s\"\n", message.c_str());
        thrownValue->Release();
        thrownValue = 0;
    }
    AptScriptFunctionBase::PopStaticData(saveBase);
    debugCallStack.Pop();
}

// FUNCTION: BFME2 0x00706A40 size=2496 name=?_createObject@AptActionInterpreter@@AAEPAVAptObject@@PAVAptValue@@0PBVEAStringC@@H_N@Z evidence=N:ledger

// FUNCTION: BFME2 0x00707400 size=356 name=?_FunctionAptActionSetTarget2@AptActionInterpreter@@CAXQAU1@QAULocalContextT@1@@Z evidence=N:ledger
void AptActionInterpreter::_FunctionAptActionSetTarget2(AptActionInterpreter *const interp, LocalContextT *const context)
{
    AptValue *object = interp->stack.At(0);
    EAStringC name;
    object->toString(name);
    if (name.GetLength() == 0) {
        if (context->pCurWith)
            context->pCurWith->Release();
        context->pCurWith = 0;
    } else {
        if (context->pCurWith) {
            APT_AI_ASSERT("!pLocalContext->pCurWith", 0x1087);
        }
        AptValue *target = getObject(context->pCurrentContext, context->pCurWith, &name);
        if (!target) {
            APT_AI_ASSERT("pTarget", 0x1089);
        }
        context->pRemoveWithAt = 0;
        context->pCurWith = target;
        context->pCurWith->AddRef();
    }
    interp->stack.Pop();
}

// FUNCTION: BFME2 0x00707570 size=386 name=?_FunctionAptActionGetProperty@AptActionInterpreter@@CAXQAU1@QAULocalContextT@1@@Z evidence=N:ledger
void AptActionInterpreter::_FunctionAptActionGetProperty(AptActionInterpreter *const interp, LocalContextT *const context)
{
    AptValue *index = interp->stack.At(0);
    AptValue *target = interp->stack.At(1);
    AptValue *object = convertWith(context->pCurrentContext, context->pCurWith, target);
    if (object) {
        unsigned int n = index->toInteger();
        AptValue *value = interp->getVariable(object, context->pCurWith, StringPool::GetString(gaPropertyStringIndices[n]), 1, 1, 0);
        interp->stack.Pop(2);
        interp->stack.Push(value);
    } else {
        interp->stack.Pop(2);
        interp->stack.Push(gpUndefinedValue);
    }
}

// FUNCTION: BFME2 0x00707700 size=307 name=?_FunctionAptActionSetProperty@AptActionInterpreter@@CAXQAU1@QAULocalContextT@1@@Z evidence=N:ledger
void AptActionInterpreter::_FunctionAptActionSetProperty(AptActionInterpreter *const interp, LocalContextT *const context)
{
    AptValue *value = interp->stack.At(0);
    AptValue *index = interp->stack.At(1);
    AptValue *target = interp->stack.At(2);
    AptValue *object = convertWith(context->pCurrentContext, context->pCurWith, target);
    unsigned int n = index->toInteger();
    if (object)
        interp->setVariable(object, context->pCurWith, StringPool::GetString(gaPropertyStringIndices[n]), value, 1, 1, 0);
    interp->stack.Pop(3);
}

// FUNCTION: BFME2 0x00707840 size=199 name=?_FunctionAptActionRemoveSprite@AptActionInterpreter@@CAXQAU1@QAULocalContextT@1@@Z evidence=N:ledger
void AptActionInterpreter::_FunctionAptActionRemoveSprite(AptActionInterpreter *const interp, LocalContextT *const context)
{
    AptValue *target = interp->stack.At(0);
    if (!target->isUndefined()) {
        target = convertWith(context->pCurrentContext, context->pCurWith, target);
        if (target && target->isCIH(false)) {
            AptCIH *cih = (AptCIH *)target->checkedCIH(false);
            AptSpriteInstBase *sprite = cih->mpParent->GetSpriteInstBase();
            sprite->displayList.removeClonedObject(cih);
        }
    }
    interp->stack.Pop();
}

// FUNCTION: BFME2 0x00707910 size=615 name=?_FunctionAptActionCallFunction@AptActionInterpreter@@CAXQAU1@QAULocalContextT@1@@Z evidence=N:ledger
void AptActionInterpreter::_FunctionAptActionCallFunction(AptActionInterpreter *const interp, LocalContextT *const context)
{
    AptValue *name = interp->stack.At(0);
    AptValue *params = interp->stack.At(1);
    EAStringC variable;
    int nParams = params->toInteger();
    AptValue *function = 0;
    AptValue *scope = 0;
    if (name->isArray())
        name = ((AptArray *)name->checkedArray())->get(0);
    if (name->isString()) {
        AptString *label = (AptString *)name->checkedString();
        rva006FEC00((int)context->pCurrentContext, (int)context->pCurWith, &label->mString, (int *)&scope, &variable);
        function = interp->getVariable(scope, context->pCurWith, &variable, 1, 1, 0);
    } else {
        function = name;
    }
    function->AddRef();
    interp->stack.Pop(2);
    interp->debugCallStack.Push(new DebugCallStackInfo_t(variable.c_str(), scope ? scope : (AptValue *)context->pCurrentContext, 0x4000000));
    interp->callFunction(scope ? scope : (AptValue *)context->pCurrentContext, function, nParams);
    interp->debugCallStack.Pop();
    function->Release();
}

// FUNCTION: BFME2 0x00707B80 size=331 name=?_FunctionAptActionNewObject@AptActionInterpreter@@CAXQAU1@QAULocalContextT@1@@Z evidence=N:ledger
void AptActionInterpreter::_FunctionAptActionNewObject(AptActionInterpreter *const interp, LocalContextT *const context)
{
    AptValue *name = interp->stack.At(0);
    AptValue *params = interp->stack.At(1);
    EAStringC objectName;
    name->toString(objectName);
    int nParams = params->toInteger();
    interp->stack.Pop(2);
    AptValue *object = interp->_createObject(context->pCurrentContext, context->pCurWith, &objectName, nParams, true);
    if (object) {
        interp->stack.Push(object);
        object->Release();
    } else {
        interp->stack.Push(gpUndefinedValue);
    }
}

// FUNCTION: BFME2 0x00707CD0 size=500 name=?_FunctionAptActionInitObject@AptActionInterpreter@@CAXQAU1@QAULocalContextT@1@@Z evidence=N:ledger
void AptActionInterpreter::_FunctionAptActionInitObject(AptActionInterpreter *const interp, LocalContextT *const context)
{
    int count = interp->stack.At(0)->toInteger();
    interp->stack.Pop();
    AptObject *object = interp->_createObject((AptValue *)context->pCurrentContext, context->pCurWith, StringPool::GetString(100), 0, true);
    if (object) {
        for (int i = 0, slot = 0; i < count; ++i, slot += 2) {
            AptValue *value = interp->stack.At(slot);
            AptValue *name = interp->stack.At(slot + 1);
            EAStringC text;
            name->toString(text);
            object->GetNativeHash()->Set(&text, value);
        }
        interp->stack.rva006E3AA0(2 * count);
        interp->stack.Push(object);
        object->Release();
    } else {
        interp->stack.PopN(2 * count);
        interp->stack.Push(gpUndefinedValue);
    }
}

// FUNCTION: BFME2 0x00707ED0 size=401 name=?_FunctionAptActionTargetPath@AptActionInterpreter@@CAXQAU1@QAULocalContextT@1@@Z evidence=N:ledger
void AptActionInterpreter::_FunctionAptActionTargetPath(AptActionInterpreter *const interp, LocalContextT *const context)
{
    EAStringC buffer;
    AptValue *target;
    aptConvertWith((AptValue *)context->pCurrentContext, context->pCurWith, interp->stack.At(0), &target);
    if (target) {
        if (target->isCIH(false))
            rva006ffce0((AptValue *)target->checkedCIH(false), buffer);
        else
            buffer = "";
        AptString *path = AptString::Create();
        path->mString = buffer;
        interp->stack.Pop();
        interp->stack.Push(path);
    } else {
        interp->stack.Pop();
        interp->stack.Push(gpUndefinedValue);
    }
}

// FUNCTION: BFME2 0x00708070 size=1905 name=?_FunctionAptActionCallMethod@AptActionInterpreter@@CAXQAU1@QAULocalContextT@1@@Z evidence=N:ledger

// FUNCTION: BFME2 0x007087F0 size=474 name=?_FunctionAptActionNewMethod@AptActionInterpreter@@CAXQAU1@QAULocalContextT@1@@Z evidence=N:ledger
void AptActionInterpreter::_FunctionAptActionNewMethod(AptActionInterpreter *const interp, LocalContextT *const context)
{
    AptValue *name = interp->stack.At(0);
    AptValue *scope = interp->stack.At(1);
    AptValue *params = interp->stack.At(2);
    EAStringC objectName;
    name->toString(objectName);
    int nParams = params->toInteger();
    interp->stack.Pop();
    interp->stack.PopNoDec();
    interp->stack.Pop();
    AptObject *object = interp->_createObject(scope, context->pCurWith, &objectName, nParams, true);
    scope->Release();
    if (object) {
        interp->stack.Push(object);
        object->Release();
    } else {
        interp->stack.Push(gpUndefinedValue);
    }
}

// FUNCTION: BFME2 0x007089D0 size=321 name=?_FunctionAptActionSetTarget@AptActionInterpreter@@CAXQAU1@QAULocalContextT@1@@Z evidence=N:ledger
void AptActionInterpreter::_FunctionAptActionSetTarget(AptActionInterpreter *const interp, LocalContextT *const context)
{
    context->pInstruction = (const unsigned char *)(((unsigned int)context->pInstruction + 3) & ~3u);
    const char *const *data = (const char *const *)context->pInstruction;
    context->pInstruction += 4;
    if ((*data)[0] == 0) {
        if (context->pCurWith)
            context->pCurWith->Release();
        context->pCurWith = 0;
    } else {
        if (context->pCurWith) {
            APT_AI_ASSERT("!pLocalContext->pCurWith", 0x1d3a);
        }
        EAStringC name(*data);
        AptValue *target;
        if ((*data)[0] == '/' || (*data)[0] == '.') {
            const char *path = *data;
            AptCIH *cih = context->pCurrentContext;
            while (path[0] == '.' && path[1] == '.' && cih->mpParent) {
                cih = cih->mpParent;
                path += 2;
            }
            target = cih;
        } else {
            name.TrimRight("/");
            target = getObject(context->pCurrentContext, 0, &name);
        }
        if (!target) {
            APT_AI_ASSERT("pTarget", 0x1d51);
        }
        context->pRemoveWithAt = 0;
        context->pCurWith = target;
        context->pCurWith->AddRef();
    }
}

// FUNCTION: BFME2 0x00708B20 size=278 name=?_FunctionAptActionWith@AptActionInterpreter@@CAXQAU1@QAULocalContextT@1@@Z evidence=N:ledger
void AptActionInterpreter::_FunctionAptActionWith(AptActionInterpreter *const interp, LocalContextT *const context)
{
    const unsigned char *const *data = (const unsigned char *const *)(((unsigned int)context->pInstruction + 3) & ~3u);
    context->pInstruction = (const unsigned char *)(data + 1);
    AptValue *value = interp->stack.At(0);
    if (value->isUndefined()) {
        context->pCurWith = 0;
        context->pInstruction = *data;
    } else {
        value = convertWith(context->pCurrentContext, 0, value);
        if (!value) {
            g_bfmeAptAssertAtE17734("pWith", "C:\\projects\\bfme2patch103\\bfme2\\Code\\Libraries\\Source\\Apt\\AptActionInterpreter.cpp", 0x1da8);
            if (g_bfmeAptBreakOnAssertAtDDC01C) APT_ASSERT_BREAK();
        }
        if (context->pCurWith) {
            g_bfmeAptAssertAtE17734("!pLocalContext->pCurWith", "C:\\projects\\bfme2patch103\\bfme2\\Code\\Libraries\\Source\\Apt\\AptActionInterpreter.cpp", 0x1da9);
            if (g_bfmeAptBreakOnAssertAtDDC01C) APT_ASSERT_BREAK();
        }
        context->pRemoveWithAt = *data;
        context->pCurWith = value;
        value->AddRef();
    }
    interp->stack.Pop();
}

// FUNCTION: BFME2 0x00708C40 size=877 name=?_FunctionAptActionGetUrl2@AptActionInterpreter@@CAXQAU1@QAULocalContextT@1@@Z evidence=N:ledger

// FUNCTION: BFME2 0x00708FB0 size=109 name=?_FunctionAptActionCallFuncAndPop@AptActionInterpreter@@CAXQAU1@QAULocalContextT@1@@Z evidence=N:ledger
void AptActionInterpreter::_FunctionAptActionCallFuncAndPop(AptActionInterpreter *const interp, LocalContextT *const context)
{
    _FunctionAptActionCallFunction(interp, context);
    interp->stack.Pop();
    if (g_releaseVectorAtE17710->GetNumValues() && interp->stack.GetSize() == 0)
        g_releaseVectorAtE17710->ReleaseValues();
}

// FUNCTION: BFME2 0x00709020 size=63 name=?_FunctionAptActionCallFuncSetVar@AptActionInterpreter@@CAXQAU1@QAULocalContextT@1@@Z evidence=N:ledger
void AptActionInterpreter::_FunctionAptActionCallFuncSetVar(AptActionInterpreter *const interp, LocalContextT *const context)
{
    _FunctionAptActionCallFunction(interp, context);
    _FunctionAptActionSetVariable(interp, context);
    if (g_releaseVectorAtE17710->GetNumValues() && interp->stack.GetSize() == 0)
        g_releaseVectorAtE17710->ReleaseValues();
}

// FUNCTION: BFME2 0x00709060 size=109 name=?_FunctionAptActionCallMethodPop@AptActionInterpreter@@CAXQAU1@QAULocalContextT@1@@Z evidence=N:ledger
void AptActionInterpreter::_FunctionAptActionCallMethodPop(AptActionInterpreter *const interp, LocalContextT *const context)
{
    _FunctionAptActionCallMethod(interp, context);
    interp->stack.Pop();
    if (g_releaseVectorAtE17710->GetNumValues() && interp->stack.GetSize() == 0)
        g_releaseVectorAtE17710->ReleaseValues();
}

// FUNCTION: BFME2 0x007090D0 size=63 name=?_FunctionAptActionCallMethodSetVar@AptActionInterpreter@@CAXQAU1@QAULocalContextT@1@@Z evidence=N:ledger
void AptActionInterpreter::_FunctionAptActionCallMethodSetVar(AptActionInterpreter *const interp, LocalContextT *const context)
{
    _FunctionAptActionCallMethod(interp, context);
    _FunctionAptActionSetVariable(interp, context);
    if (g_releaseVectorAtE17710->GetNumValues() && interp->stack.GetSize() == 0)
        g_releaseVectorAtE17710->ReleaseValues();
}

// FUNCTION: BFME2 0x00709110 size=138 name=?_FunctionAptActionDictCallFuncPop@AptActionInterpreter@@CAXQAU1@QAULocalContextT@1@@Z evidence=N:ledger
void AptActionInterpreter::_FunctionAptActionDictCallFuncPop(AptActionInterpreter *const interp, LocalContextT *const context)
{
    unsigned char index = *context->pInstruction++;
    interp->stack.Push(interp->constantPool.apItems[index]);
    _FunctionAptActionCallFunction(interp, context);
    interp->stack.Pop();
    if (g_releaseVectorAtE17710->GetNumValues() && interp->stack.GetSize() == 0)
        g_releaseVectorAtE17710->ReleaseValues();
}

// FUNCTION: BFME2 0x007091A0 size=87 name=?_FunctionAptActionDictCallFuncSetVar@AptActionInterpreter@@CAXQAU1@QAULocalContextT@1@@Z evidence=N:ledger
void AptActionInterpreter::_FunctionAptActionDictCallFuncSetVar(AptActionInterpreter *const interp, LocalContextT *const context)
{
    unsigned char index = *context->pInstruction++;
    interp->stack.Push(interp->constantPool.apItems[index]);
    _FunctionAptActionCallFunction(interp, context);
    _FunctionAptActionSetVariable(interp, context);
    if (g_releaseVectorAtE17710->GetNumValues() && interp->stack.GetSize() == 0)
        g_releaseVectorAtE17710->ReleaseValues();
}

// FUNCTION: BFME2 0x00709200 size=138 name=?_FunctionAptActionDictCallMethodPop@AptActionInterpreter@@CAXQAU1@QAULocalContextT@1@@Z evidence=N:ledger
void AptActionInterpreter::_FunctionAptActionDictCallMethodPop(AptActionInterpreter *const interp, LocalContextT *const context)
{
    unsigned char index = *context->pInstruction++;
    interp->stack.Push(interp->constantPool.apItems[index]);
    _FunctionAptActionCallMethod(interp, context);
    interp->stack.Pop();
    if (g_releaseVectorAtE17710->GetNumValues() && interp->stack.GetSize() == 0)
        g_releaseVectorAtE17710->ReleaseValues();
}

// FUNCTION: BFME2 0x00709290 size=87 name=?_FunctionAptActionDictCallMethodSetVar@AptActionInterpreter@@CAXQAU1@QAULocalContextT@1@@Z evidence=N:ledger
void AptActionInterpreter::_FunctionAptActionDictCallMethodSetVar(AptActionInterpreter *const interp, LocalContextT *const context)
{
    unsigned char index = *context->pInstruction++;
    interp->stack.Push(interp->constantPool.apItems[index]);
    _FunctionAptActionCallMethod(interp, context);
    _FunctionAptActionSetVariable(interp, context);
    if (g_releaseVectorAtE17710->GetNumValues() && interp->stack.GetSize() == 0)
        g_releaseVectorAtE17710->ReleaseValues();
}

// FUNCTION: BFME2 0x007092F0 size=491 name=?rva007092F0@AptActionInterpreter@@AAEXPAVAptCIH@@PAVAptValue@@11H1@Z evidence=?:placeholder

// FUNCTION: BFME2 0x007094E0 size=267 name=?_FunctionAptActionCloneSprite@AptActionInterpreter@@CAXQAU1@QAULocalContextT@1@@Z evidence=N:ledger

