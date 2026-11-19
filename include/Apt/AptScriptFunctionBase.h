#pragma once
// SIZE 0x30: AptObject (0x20) plus four members. Constructors 0x00709870
// (type, parent function, CIH, flag) and 0x00709A10 (type, parent function,
// CIH) are protected; vtable 0x008EE8C8 has 25 slots. Slots 0..14 are
// AptObject's (the destructor is last, deleting copy 0x0070A560), slots 15..24
// are new here: Duplicate, GetName, GetNumArguments, GetByteCodeBase,
// GetByteCodeSize, GetConstantPool (all pure: __purecall 0x0003B810),
// SetupBeforeExecution 0x00709E50, SetArgument (pure), CleanupAfterExecution
// 0x00709E70 and CreatingNestedFunction 0x00709A00. The derived tables
// (AptScriptFunction1/2, ByteCodeBlock) fill 15..20 and 22 with their bodies.
// The retail assertions in this file name spRegBlockBase,
// spRegBlockCurrentFrameBase, snRegisterBlockSize, snRegBlockCurrentFrameCount,
// MAX_REGISTERS_IN_FUNCTION (0x100) and gpUndefinedValue (the data word at
// VA 0x00E18078); the five statics sit at VA 0x00E1834C..0x00E1835C.
// Member names for +0x20..+0x2C come from the donor's PDB; the offsets and
// uses are retail's: DestroyGCPointers 0x00709E00 releases +0x28, +0x20 and
// +0x24, CleanupAfterExecution stores the saved scope size at +0x2C.
#include "Apt/AptObject.h"
#include "Apt/AptConstantPool.h"
#include "Apt/AptCIH.h"
#include "Apt/AptFrameStack.h"

class AptCIH;
class AptFrameStack;
struct AptInitParmsT;

// 8 bytes: Cleanup 0x0070A230 reads +0 (frame scope) and +4 (register base).
struct _AptScriptFunctionState
{
    AptFrameStack *mpFrameStack;              // 0x0
    AptValue **mpRegBlockPreviousFrameBase;   // 0x4
};

class AptScriptFunctionBase : public AptObject
{
public:
    virtual AptScriptFunctionBase *Duplicate(AptCIH *cih) = 0;
    virtual const char *GetName() const = 0;
    virtual unsigned int GetNumArguments() = 0;
    virtual const unsigned char *GetByteCodeBase() = 0;
    virtual unsigned int GetByteCodeSize() = 0;
    virtual AptConstantPool GetConstantPool() = 0;
    virtual void SetupBeforeExecution(_AptScriptFunctionState *state, AptValue *self);
    virtual void SetArgument(AptValue *value, int index) = 0;
    virtual void CleanupAfterExecution(_AptScriptFunctionState *state);
    virtual void CreatingNestedFunction();
    // Overrides (no new slots): DestroyGCPointers (slot 11) = 0x00709E00 releases the three owned
    // references and then AptObject's; RegisterReferences (13) = 0x00709D80 marks them for the collector.
    virtual void DestroyGCPointers();
    virtual void RegisterReferences() const;

    bool rva006FBE60(const EAStringC *name, AptValue *value); // 0x006FBE60 (the map files it under a placeholder class): stores a variable in the scope the function is running in (the frame stack, or its creator's scope); false when there is none
    AptValue *rva006FBE80(const EAStringC *name); // 0x006FBE80 (the same placeholder class): the lookup counterpart of the store above, null when there is no such scope or variable
    void CreateFrameStack(); // 0x006FBED0 (the map files it under the placeholder Rva8D0D80Result)
    void SetLocalVariable(const EAStringC *name, AptValue *value); // 0x006FE360 (the map files it as setName on a placeholder class): creates the frame stack when there is none, then stores the variable in its hash
    static void SetRegisterValue(int nIndex, AptValue *pNewValue);
    static void InitializeStaticData(const AptInitParmsT &parms); // 0x00709610: sizes and fills the register block (parms.iRegArraySize)
    static void ShutdownStaticData();
    static void *PushStaticData();
    static void PopStaticData(void *saveBase);      // 0x007097B0
    static AptValue *GetRegisterValue(int nIndex);   // 0x00709EC0
    // The register block pointer moved on by a frame's register count: SetupBeforeExecution 0x0070A040 loads the count before the pointer (retail: `mov eax,[count]; mov ecx,[base];
    // lea edx,[ecx+eax*4]`), which a plain `base += count` does not give (PushStaticData 0x00709730 loads them the other way round) but a call to this inline does.
    static AptValue **advanceRegisterBlock(AptValue **base, int count) { return base + count; }

    static AptValue **spRegBlockBase;             // VA 0x00E1834C
    static AptValue **spRegBlockCurrentFrameBase; // VA 0x00E18354
    static int snRegBlockCurrentFrameCount;       // VA 0x00E18350 (the count comes first in retail: matched code reads it there)
    static int snRegisterBlockSize;               // VA 0x00E18358
    static AptFrameStack *spFrameStack;           // VA 0x00E1835C (the interpreter's DefineLocal reads it directly)
    // SetArgument 0x00709C10 / 0x0070A400 store into the scope's hash (the AptNativeHash at +8) in place.
    static void SetScopeVariable(const EAStringC *name, AptValue *value) { ((AptValueWithHash *)spFrameStack)->mNativeHash.Set(name, value); }

    friend class AptAnimationPoolData; // removeTimerFunctions 0x006E44F0 reads the owner CIH at +0x20
    friend struct AptActionInterpreter; // DefineLocal2 0x00702740 reads the creator scope at +0x28
    friend class AptCIH; // queueClipEvents 0x006E2010 compares the function's clip with the instance
protected:
    AptScriptFunctionBase(AptVirtualFunctionTable_Indices type, AptScriptFunctionBase *parent, AptCIH *cih, bool flag);
    AptScriptFunctionBase(AptVirtualFunctionTable_Indices type, AptScriptFunctionBase *parent, AptCIH *cih);
    virtual ~AptScriptFunctionBase();


    AptCIH *mpCIH;                       // 0x20
    AptCIH *mpParentAnim;                // 0x24
    AptFrameStack *mpCreatorScope;       // 0x28
    unsigned short mnFrameStackReserve;  // 0x2C
};

typedef char AptScriptFunctionBaseSize[(sizeof(AptScriptFunctionBase) == 0x30) ? 1 : -1];
