#pragma once
// SIZE 0x20 (0x006FBF0A and 0x006FBF28 allocate 0x20 bytes from the AptValueGC pool).
// A function-call scope: AptValueWithHash (0x1C) plus the parent scope at +0x1C. Type id 20
// (the predicate 0x006DC350 is the map's unnamed one at that id). Vtable 0x008ED880
// (15 slots): AptValueWithHash's table with DestroyGCPointers (11) = 0x006FBC10 (releases the
// parent, then AptValueWithHash::DestroyGCPointers), RegisterReferences (13) = 0x006FBC30
// (marks the parent for the collector, has it register its references, then calls
// AptValueWithHash::RegisterReferences) and the deleting destructor 0x006FBC60 last.
// Constructors: 0x006FBAA0 (parent; hash size 4) and 0x006FBB10 (parent, hash size): each stores
// the parent at +0x1C and AddRefs it. AptScriptFunctionBase::CleanupAfterExecution
// 0x00709E70 reaches it through the static spFrameStack. The member name is the donor's.
#include "Apt/AptValueWithHash.h"

class AptFrameStack : public AptValueWithHash
{
public:
    AptFrameStack(AptValue *parentScope);
    AptFrameStack(AptValue *parentScope, int hashSize);
    virtual void DestroyGCPointers();
    virtual void RegisterReferences() const;
protected:
    virtual ~AptFrameStack();
    AptValue *mpParentScope; // 0x1C
};

typedef char AptFrameStackSize[(sizeof(AptFrameStack) == 0x20) ? 1 : -1];
