#pragma once
// SIZE >= 0x69 (the byte at +0x68 is the last member any retail body writes;
// 0x6C once padded). `struct` in retail: the map's mangled names use U for
// the interpreter and for LocalContextT. No base, no virtuals.
// Evidence for the layout (retail bodies, RVAs):
//   +0x00/+0x0C/+0x18/+0x24  four 12-byte pointer stacks. The constructor
//                            0x006FE980 zeroes +0..+0x2C; the destructor
//                            0x006FE9C0 frees the arrays at +0x08, +0x14,
//                            +0x20 and +0x2C; asserts in 0x006FDD40/0x006FE0B0
//                            name m_nElements, m_nCapacity, m_aElements.
//   +0x30                    pointer, the current script function: the
//                            constructor does NOT clear it.
//   +0x34                    debug call stack, 12 bytes (0x006FE160 is its
//                            destructor, cleared at 0x006FE9A7..0x006FE9AD).
//   +0x40/+0x44              constant pool {count, items}: DefineDictionary
//                            0x006FDC30 stores both.
//   +0x60..+0x68             thrown value pointer, stack frame base, a flag
//                            byte (0x006FEB27, 0x006FEB2A, 0x006FD300).
// The member names of the stacks are this header's, the interpreter's real
// member names are not in any assert string.
// The 185-entry opcode table is a private static: each entry is {int
// check, handler}; the handlers below are exactly the map's named rows.
// Handlers that retail declares with LocalContextT take (interp, ctx); the
// five bit-operator handlers at 0x007020B0 and 0x00704100..0x00704400 are
// public static and take only the interpreter.
// The class-key and access of every method below come from the mangled
// names of the map's rows (UAE/QAE/AAE/CAX/SAX); the order is the rows' address order.
#include "Apt/AptBasePtrStack.h"
#include "Apt/_AptValuePtrStack.h"
#include "Apt/_AptDebugStack.h"
#include "Apt/AptConstantPool.h"
#include "Apt/EAStringC.h"
#include "Apt/DogmaAllocator.h"

class AptValue;
class AptObject;
class AptCIH;
struct AptCharacterInst; // `struct` in retail: runStream's mangled name spells PAUAptCharacterInst
class AptScriptFunctionBase;
class EAStringC;
struct AptActionSetup;
struct AptInitParmsT;


// The Try action's operand (0x00706070 reads it after aligning to four): three block sizes, a flag byte (1 catch, 2 finally,
// 4 the caught value goes to a register), the register at +0xF and the caught value's name at +0x10; the try block's code follows.
struct AptAction_Try
{
    unsigned int trySize;     // 0x0
    unsigned int catchSize;   // 0x4
    unsigned int finallySize; // 0x8
    unsigned char flags;      // 0xC
    unsigned char reservedD[2]; // 0xD
    unsigned char caughtReg;  // 0xF
    const char *caughtName;   // 0x10
};

struct AptActionInterpreter
{
public:
    // One entry of the interpreter's debug call stack (PrepareForExecution 0x00700090 builds it from the AptActionSetup, 12 bytes from the
    // pool, constructor 0x006FBD40 (name by value), destructor 0x006FBDB0 clears the function and destroys the name; the map files them
    // under the placeholder class Rva006FBDB0). Member names are this header's.
    struct DebugCallStackInfo_t
    {
        DebugCallStackInfo_t(EAStringC name, AptValue *function, int flags);
        ~DebugCallStackInfo_t() { mpFunction = 0; }
        static void *operator new(unsigned int size) { return g_pDogmaPoolManager->allocBlock(size); }
        static void operator delete(void *block, unsigned int size) { g_pDogmaPoolManager->freeBlock(block, size); }
        EAStringC mName;       // 0x0
        AptValue *mpFunction;  // 0x4 (the AptActionSetup's function word)
        int mFlags;            // 0x8 (the AptActionSetup's flag word)
    };
    // 0x1C bytes: Return sets bEncounteredReturn at +0x14 (0x006FDA04).
    struct LocalContextT
    {
        const unsigned char *pInstruction; // 0x0
        AptCIH *pCurrentContext; // 0x4
        AptValue *pCurWith; // 0x8
        const unsigned char *pRemoveWithAt; // 0xC
        AptValue *pSuper; // 0x10
        bool bEncounteredReturn; // 0x14
        AptCharacterInst *pParentCharacter; // 0x18
    };
    struct FunctionTable
    {
        int mCheckAlignment; // 0x0
        void (__cdecl *mFunctionPointer)(AptActionInterpreter *const, LocalContextT *const); // 0x4
    };
public:
    int doFSCommand(const char *, const char *); // 0x006FD7D0
    void *rva006FD300(AptActionSetup *); // 0x006FD300: the light prepare (asserts no thrown value, pushes the static data)
    static bool isObjectOfType(AptValue *, AptValue *); // 0x006FDC60
    void stackPushIndirect(AptValue *const); // 0x006FE610
    // 0x006FEAF0 and 0x006FEB50 (the map files them under placeholder names): the interpreter's init (sizes its stacks from
    // the init parameter block) and its shutdown (frees them, asserts the stack frame base is back at zero).
    void rva006FEAF0(const AptInitParmsT &parms);
    void rva006FEB50();
    AptActionInterpreter(); // 0x006FE980
    ~AptActionInterpreter(); // 0x006FE9C0
    bool setVariable(AptValue *, AptValue *, const EAStringC *, AptValue *, int, int, int); // 0x006FED00
    AptValue *getVariable(AptValue *, AptValue *, const EAStringC *, int, int, int); // 0x006FFD80
    void *PrepareForExecution(AptActionSetup *); // 0x00700090
    unsigned const char * runStream(unsigned const char *, AptCIH *, int, AptCharacterInst *); // 0x007002C0
    static void _FunctionAptActionRandom(AptActionInterpreter *const); // 0x007020B0
    static void _FunctionAptActionBitAnd(AptActionInterpreter *const); // 0x00704100
    static void _FunctionAptActionBitOr(AptActionInterpreter *const); // 0x007041C0
    static void _FunctionAptActionBitXor(AptActionInterpreter *const); // 0x00704280
    static void _FunctionAptActionBitLShift(AptActionInterpreter *const); // 0x00704340
    static void _FunctionAptActionBitRShift(AptActionInterpreter *const); // 0x00704400
    void loadVariables(AptValue *, AptValue *, const EAStringC *); // 0x00706370
    void callFunction(AptValue *, AptValue *, int); // 0x00706660
    void CleanupAfterExecution(void *, AptActionSetup *); // 0x00706950
    void CleanupAfterExecution(char *name, void *saveBase); // 0x006FD340: the light cleanup (rva006FD340 in the map): logs an uncaught thrown value, releases it, pops the static data
    void enumerate(AptValue *context, AptValue *with); // 0x00700170 (the map files it as rva00700170 on AptBasePtrStack; `this` is the interpreter: it calls getVariable on it): the enumerate opcode, replaces the top value by its key names
    friend void aptConvertWith(AptValue *, AptValue *, AptValue *, AptValue **); // 0x007065E0 calls the private getObject
    friend AptValue *rva006ec670(AptValue *, int); // 0x006EC670 (AptCharacter.cpp, MovieClip.duplicateMovieClip) calls the private duplicate helper
private:
    // 0x007092F0 (the map files it as rva007092F0, private): duplicates a movie clip: (the clip's CIH, null, the clip, the new name, the depth, the init object).
    void rva007092F0(AptCIH *cih, AptValue *with, AptValue *clip, AptValue *name, int depth, AptValue *initObject);
    const char * urlDecode(const char *, EAStringC &, EAStringC &); // 0x006FD700
    static void _FunctionAptActionEnd(AptActionInterpreter *const, LocalContextT *const); // 0x006FD850
    static void _FunctionAptActionToggleQuality(AptActionInterpreter *const, LocalContextT *const); // 0x006FD880
    static void _FunctionAptActionStopDragMovie(AptActionInterpreter *const, LocalContextT *const); // 0x006FD8B0
    static void _FunctionAptActionStringLessThan(AptActionInterpreter *const, LocalContextT *const); // 0x006FD8E0
    static void _FunctionAptActionMBLength(AptActionInterpreter *const, LocalContextT *const); // 0x006FD910
    static void _FunctionAptActionCharToAscii(AptActionInterpreter *const, LocalContextT *const); // 0x006FD940
    static void _FunctionAptActionMBSubString(AptActionInterpreter *const, LocalContextT *const); // 0x006FD970
    static void _FunctionAptActionMBCharToAscii(AptActionInterpreter *const, LocalContextT *const); // 0x006FD9A0
    static void _FunctionAptActionMBAsciiToChar(AptActionInterpreter *const, LocalContextT *const); // 0x006FD9D0
    static void _FunctionAptActionReturn(AptActionInterpreter *const, LocalContextT *const); // 0x006FDA00
    static void _FunctionAptActionBitURShift(AptActionInterpreter *const, LocalContextT *const); // 0x006FDA10
    static void _FunctionAptActionGetUrl(AptActionInterpreter *const, LocalContextT *const); // 0x006FDA40
    static void _FunctionAptActionDefineDictionary(AptActionInterpreter *const, LocalContextT *const); // 0x006FDC30
    static void _FunctionAptActionNextFrame(AptActionInterpreter *const, LocalContextT *const); // 0x00700560
    static void _FunctionAptActionPrevFrame(AptActionInterpreter *const, LocalContextT *const); // 0x007005E0
    static void _FunctionAptActionPlay(AptActionInterpreter *const, LocalContextT *const); // 0x00700660
    static void _FunctionAptActionStop(AptActionInterpreter *const, LocalContextT *const); // 0x007006D0
    static void _FunctionAptActionAdd(AptActionInterpreter *const, LocalContextT *const); // 0x00700720
    static void _FunctionAptActionSubtract(AptActionInterpreter *const, LocalContextT *const); // 0x00700880
    static void _FunctionAptActionMultiply(AptActionInterpreter *const, LocalContextT *const); // 0x007009E0
    static void _FunctionAptActionDivide(AptActionInterpreter *const, LocalContextT *const); // 0x00700B40
    static void _FunctionAptActionEquals(AptActionInterpreter *const, LocalContextT *const); // 0x00700C80
    static void _FunctionAptActionLessThan(AptActionInterpreter *const, LocalContextT *const); // 0x00700DF0
    static void _FunctionAptActionAnd(AptActionInterpreter *const, LocalContextT *const); // 0x00700F50
    static void _FunctionAptActionOr(AptActionInterpreter *const, LocalContextT *const); // 0x007010D0
    static void _FunctionAptActionNot(AptActionInterpreter *const, LocalContextT *const); // 0x00701260
    static void _FunctionAptActionStringEquals(AptActionInterpreter *const, LocalContextT *const); // 0x00701300
    static void _FunctionAptActionStringLength(AptActionInterpreter *const, LocalContextT *const); // 0x007014C0
    static void _FunctionAptActionSubString(AptActionInterpreter *const, LocalContextT *const); // 0x007015B0
    static void _FunctionAptActionPop(AptActionInterpreter *const, LocalContextT *const); // 0x00701770
    static void _FunctionAptActionToInteger(AptActionInterpreter *const, LocalContextT *const); // 0x007017F0
    static void _FunctionAptActionGetVariable(AptActionInterpreter *const, LocalContextT *const); // 0x007018B0
    static void _FunctionAptActionSetVariable(AptActionInterpreter *const, LocalContextT *const); // 0x007019B0
    static void _FunctionAptActionStringAdd(AptActionInterpreter *const, LocalContextT *const); // 0x00701AF0
    static void _FunctionAptActionTrace(AptActionInterpreter *const, LocalContextT *const); // 0x00701C10
    static void _FunctionAptActionStartDragMovie(AptActionInterpreter *const, LocalContextT *const); // 0x00701D80
    static void _FunctionAptActionAsciiToChar(AptActionInterpreter *const, LocalContextT *const); // 0x00702160
    static void _FunctionAptActionGetTimer(AptActionInterpreter *const, LocalContextT *const); // 0x00702290
    static void _FunctionAptActionDelete(AptActionInterpreter *const, LocalContextT *const); // 0x007022B0
    static void _FunctionAptActionDelete2(AptActionInterpreter *const, LocalContextT *const); // 0x007023F0
    static void _FunctionAptActionDefineLocal(AptActionInterpreter *const, LocalContextT *const); // 0x007024F0
    static void _FunctionAptActionModulo(AptActionInterpreter *const, LocalContextT *const); // 0x00702600
    static void _FunctionAptActionDefineLocal2(AptActionInterpreter *const, LocalContextT *const); // 0x00702740
    static void _FunctionAptActionInitArray(AptActionInterpreter *const, LocalContextT *const); // 0x00702860
    static void _FunctionAptActionTypeOf(AptActionInterpreter *const, LocalContextT *const); // 0x007029A0
    static void _FunctionAptActionEnumerate(AptActionInterpreter *const, LocalContextT *const); // 0x00702B40 (the map files it as the free function rva00702b40: it forwards the context and with value to enumerate 0x00700170)
    static void _FunctionAptActionAdd2(AptActionInterpreter *const, LocalContextT *const); // 0x00702B60
    static void _FunctionAptActionLessThan2(AptActionInterpreter *const, LocalContextT *const); // 0x00702F20
    static void _FunctionAptActionEquals2(AptActionInterpreter *const, LocalContextT *const); // 0x007031E0
    static void _FunctionAptActionToNumber(AptActionInterpreter *const, LocalContextT *const); // 0x00703730
    static void _FunctionAptActionToString(AptActionInterpreter *const, LocalContextT *const); // 0x00703860
    static void _FunctionAptActionPushDuplicate(AptActionInterpreter *const, LocalContextT *const); // 0x007039B0
    static void _FunctionAptActionStackSwap(AptActionInterpreter *const, LocalContextT *const); // 0x007039F0
    static void _FunctionAptActionGetMember(AptActionInterpreter *const, LocalContextT *const); // 0x00703AE0
    static void _FunctionAptActionSetMember(AptActionInterpreter *const, LocalContextT *const); // 0x00703CE0
    static void _FunctionAptActionIncrement(AptActionInterpreter *const, LocalContextT *const); // 0x00703F40
    static void _FunctionAptActionDecrement(AptActionInterpreter *const, LocalContextT *const); // 0x00704020
    static void _FunctionAptActionStrictEquals(AptActionInterpreter *const, LocalContextT *const); // 0x007044C0
    static void _FunctionAptActionGreater(AptActionInterpreter *const, LocalContextT *const); // 0x00704710
    static void _FunctionAptActionGotoFrame(AptActionInterpreter *const, LocalContextT *const); // 0x00704900
    static void _FunctionAptActionStoreRegister(AptActionInterpreter *const, LocalContextT *const); // 0x007049B0
    static void _FunctionAptActionGotoLabel(AptActionInterpreter *const, LocalContextT *const); // 0x00704A10
    static void _FunctionAptActionPush(AptActionInterpreter *const, LocalContextT *const); // 0x00704B10
    static void _FunctionAptActionDefineFunction(AptActionInterpreter *const, LocalContextT *const); // 0x00704B50
    static void _FunctionAptActionDefineFunction2(AptActionInterpreter *const, LocalContextT *const); // 0x00704C60
    static void _FunctionAptActionBranchIfTrue(AptActionInterpreter *const, LocalContextT *const); // 0x00704D50
    static void _FunctionAptActionCallFrame(AptActionInterpreter *const, LocalContextT *const); // 0x00704E10
    static void _FunctionAptActionGotoFrame2(AptActionInterpreter *const, LocalContextT *const); // 0x00704FE0
    static void _FunctionAptActionBranchAlways(AptActionInterpreter *const, LocalContextT *const); // 0x00705200
    static void _FunctionAptActionPushThis(AptActionInterpreter *const, LocalContextT *const); // 0x00705240
    static void _FunctionAptActionPushGlobal(AptActionInterpreter *const, LocalContextT *const); // 0x00705270
    static void _FunctionAptActionPush0(AptActionInterpreter *const, LocalContextT *const); // 0x007052A0
    static void _FunctionAptActionPush1(AptActionInterpreter *const, LocalContextT *const); // 0x007052C0
    static void _FunctionAptActionPushTrue(AptActionInterpreter *const, LocalContextT *const); // 0x007052E0
    static void _FunctionAptActionPushFalse(AptActionInterpreter *const, LocalContextT *const); // 0x00705300
    static void _FunctionAptActionPushUndefined(AptActionInterpreter *const, LocalContextT *const); // 0x00705320
    static void _FunctionAptActionPushThisVariable(AptActionInterpreter *const, LocalContextT *const); // 0x00705330
    static void _FunctionAptActionPushGlobalVariable(AptActionInterpreter *const, LocalContextT *const); // 0x00705370
    static void _FunctionAptActionPushZeroSetVar(AptActionInterpreter *const, LocalContextT *const); // 0x00705380
    static void _FunctionAptActionPushString(AptActionInterpreter *const, LocalContextT *const); // 0x007053B0
    static void _FunctionAptActionPushStringDictByte(AptActionInterpreter *const, LocalContextT *const); // 0x00705430
    static void _FunctionAptActionPushStringDictWord(AptActionInterpreter *const, LocalContextT *const); // 0x00705450
    static void _FunctionAptActionPushStringGetVar(AptActionInterpreter *const, LocalContextT *const); // 0x00705480
    static void _FunctionAptActionPushStringGetMember(AptActionInterpreter *const, LocalContextT *const); // 0x00705520
    static void _FunctionAptActionPushStringSetVar(AptActionInterpreter *const, LocalContextT *const); // 0x007055B0
    static void _FunctionAptActionPushStringSetMember(AptActionInterpreter *const, LocalContextT *const); // 0x00705640
    static void _FunctionAptActionStringDictByteGetVar(AptActionInterpreter *const, LocalContextT *const); // 0x007056D0
    static void _FunctionAptActionStringDictByteGetMember(AptActionInterpreter *const, LocalContextT *const); // 0x00705720
    static void _FunctionAptActionPushFloat(AptActionInterpreter *const, LocalContextT *const); // 0x00705750
    static void _FunctionAptActionPushByte(AptActionInterpreter *const, LocalContextT *const); // 0x00705790
    static void _FunctionAptActionPushWord(AptActionInterpreter *const, LocalContextT *const); // 0x007057C0
    static void _FunctionAptActionPushDWord(AptActionInterpreter *const, LocalContextT *const); // 0x007057F0
    static void _FunctionAptActionBranchIfFalse(AptActionInterpreter *const, LocalContextT *const); // 0x00705830
    static void _FunctionAptActionExtends(AptActionInterpreter *const, LocalContextT *const); // 0x007058F0
    static void _FunctionAptActionInstanceOf(AptActionInterpreter *const, LocalContextT *const); // 0x00705B80
    static void _FunctionAptActionCastOp(AptActionInterpreter *const, LocalContextT *const); // 0x00705C70
    static void _FunctionAptActionImplementsOp(AptActionInterpreter *const, LocalContextT *const); // 0x00705D90
    static void _FunctionAptActionTry(AptActionInterpreter *const, LocalContextT *const); // 0x00706070
    static void _FunctionAptActionThrow(AptActionInterpreter *const, LocalContextT *const); // 0x00706240
    static AptValue *getObject(AptValue *, AptValue *, const EAStringC *); // 0x007064F0
    AptObject *_createObject(AptValue *, AptValue *, const EAStringC *, int, bool); // 0x00706A40
    static void _FunctionAptActionSetTarget2(AptActionInterpreter *const, LocalContextT *const); // 0x00707400
    static void _FunctionAptActionGetProperty(AptActionInterpreter *const, LocalContextT *const); // 0x00707570
    static void _FunctionAptActionSetProperty(AptActionInterpreter *const, LocalContextT *const); // 0x00707700
    static void _FunctionAptActionRemoveSprite(AptActionInterpreter *const, LocalContextT *const); // 0x00707840
    static void _FunctionAptActionCallFunction(AptActionInterpreter *const, LocalContextT *const); // 0x00707910
    static void _FunctionAptActionNewObject(AptActionInterpreter *const, LocalContextT *const); // 0x00707B80
    static void _FunctionAptActionInitObject(AptActionInterpreter *const, LocalContextT *const); // 0x00707CD0
    static void _FunctionAptActionTargetPath(AptActionInterpreter *const, LocalContextT *const); // 0x00707ED0
    static void _FunctionAptActionCallMethod(AptActionInterpreter *const, LocalContextT *const); // 0x00708070
    static void _FunctionAptActionNewMethod(AptActionInterpreter *const, LocalContextT *const); // 0x007087F0
    static void _FunctionAptActionSetTarget(AptActionInterpreter *const, LocalContextT *const); // 0x007089D0
    static void _FunctionAptActionWith(AptActionInterpreter *const, LocalContextT *const); // 0x00708B20
    static void _FunctionAptActionGetUrl2(AptActionInterpreter *const, LocalContextT *const); // 0x00708C40
    static void _FunctionAptActionCallFuncAndPop(AptActionInterpreter *const, LocalContextT *const); // 0x00708FB0
    static void _FunctionAptActionCallFuncSetVar(AptActionInterpreter *const, LocalContextT *const); // 0x00709020
    static void _FunctionAptActionCallMethodPop(AptActionInterpreter *const, LocalContextT *const); // 0x00709060
    static void _FunctionAptActionCallMethodSetVar(AptActionInterpreter *const, LocalContextT *const); // 0x007090D0
    static void _FunctionAptActionDictCallFuncPop(AptActionInterpreter *const, LocalContextT *const); // 0x00709110
    static void _FunctionAptActionDictCallFuncSetVar(AptActionInterpreter *const, LocalContextT *const); // 0x007091A0
    static void _FunctionAptActionDictCallMethodPop(AptActionInterpreter *const, LocalContextT *const); // 0x00709200
    static void _FunctionAptActionDictCallMethodSetVar(AptActionInterpreter *const, LocalContextT *const); // 0x00709290
    static void _FunctionAptActionCloneSprite(AptActionInterpreter *const, LocalContextT *const); // 0x007094E0


public:
    AptBasePtrStack stack; // 0x0
    AptValuePtrStack<AptValue> withStack; // 0xC
    AptValuePtrStack<AptValue> setTargetStack; // 0x18
    AptValuePtrStack<AptValue> thisStack; // 0x24
    AptScriptFunctionBase *currentFunction; // 0x30
    AptDebugStack<DebugCallStackInfo_t> debugCallStack; // 0x34
    AptConstantPool constantPool; // 0x40
    // 0x48..0x5F: the retail object is ONE instance (the constructor 0x006FE980 has one call, 0x007B67A5) whose last member, the flag byte at +0x68, is the map's
    // g_00E18348 and whose thrownValue/stackFrameBase (+0x60/+0x64) are the unnamed cells 0xE18340/0xE18344, so the cells the map lists between 0xE182E0 and 0xE1834C
    // (0xE18328, 0xE1832C..0xE18338, 0xE1833C) are inside it: no other object can sit in the middle of this one. Absolute-address bodies reach them as members of the global.
    unsigned int keyEvent;         // 0x48: the last key event (flags in the low bits, key code from bit 17); written by the input entry 0x006E6540, read by the Key natives; the map's Rva008A5250LastKey
    int initializerCounters[4];    // 0x4C: zeroed by 0x006DF470 (0xE1832C..0xE18338); the map's g_aptInitializerCounters
    int reserved5C;                // 0x5C: 0xE1833C, not touched by any recovered body
    AptValue *thrownValue; // 0x60
    int stackFrameBase; // 0x64
    unsigned char flag68; // 0x68
private:
    static FunctionTable sGlobalTable[185];
};

// The interpreter instance whose value stack carries the arguments of the native (C++) Apt methods
// (Math, Date, Key, ...): VA 0x00E182E0, constructed by AptDate.cpp's initializer 0x007B67A0.
extern AptActionInterpreter g_aptNativeInterpreter;
