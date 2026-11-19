#pragma once
// The queue of deferred actions an AptAnimationPoolData runs (AptAnimation.cpp's enqueueAction 0x006E4B80, prependAction
// 0x006E4C70, enqueueFront 0x006E3810, rva006E3740 0x006E3740, ClearActions 0x006E4A90, size 0x006E38D0). It is a ring buffer of
// 0x18-byte actions. Layout read from those bodies: the buffer start at +0, the first queued action at +4, the slot after the
// last queued action at +8 (enqueueAction writes at +8 and advances it, wrapping to +0 when it reaches start + capacity * 0x18;
// prependAction retreats +4 the same way), the capacity in actions at +0x10. The total size is NOT recovered (the donor's
// views put the queue at +0xA0 of the pool data), so the class is only usable through a pointer. All member names but the
// class's are this header's.
class AptCIH;
class AptValue;

struct AptQueuedAction
{
    int mType;            // 0x00: 1 for a queued action, 2 for one put in front (enqueueFront)
    int mArgument0;       // 0x04
    int mArgument1;       // 0x08
    // +0xC..+0x17 are read two ways: a type-1 action keeps the frame number, the function and the context; a type-2 action
    // (enqueueFront 0x006E3810) keeps three AptValue pointers, the first two AddRef'd while queued.
    union { int mFrame; AptValue *mpValue0; };            // 0x0C: the frame number read from the target's instance (+0x28)
    union { void *mpFunction; AptValue *mpValue1; };      // 0x10
    union { AptCIH *mpContext; AptValue *mpValue2; };     // 0x14: AddRef'd while queued (type 1)
};

typedef char AptQueuedActionSize[(sizeof(AptQueuedAction) == 0x18) ? 1 : -1];

class AptActionQueueC
{
public:
    // 0x006E38D0 (the map names it rva006E38D0): the number of queued actions, wrapped by the capacity when the tail is behind
    // the head; asserts the delta is positive (AptAnimation.cpp line 0x75f, `iDelta > 0`).
    int rva006E38D0();
    // 0x006E3230 (the map files it under the placeholder class Rva006E3230Action): makes room at the given slot when the buffer is full.
    void rva006E3230(AptQueuedAction *slot);
    // 0x006E4B80 appends at the tail, 0x006E4C70 puts in front of the head; both log "Dequeue is full" when the slot is taken.
    void enqueueAction(void *pAction, AptCIH *pCIH, int nArgument0, int nArgument1);
    void enqueueFront(AptValue *pContext, AptValue *pValue1, AptValue *pValue2, int nArgument0, int nArgument1);
    // 0x006E3740 (map name rva006E3740): the same type-2 action appended at the tail instead of put in front of the head.
    // 0x006E4A90: releases every queued action (type 1 releases the context, type 2 the first two values) and empties the buffer. Its inline
    // iterator asserts (_Apt.h lines 0x4e0/0x4e1) call the buffer `m_aActionPool` and its capacity `m_iActionPoolSize`.
    void ClearActions();
    // 0x006E4D50: removes one queued type-1 action whose context (+0x14) is the given value, releasing the value (the retail name is its __FUNCTION__).
    void RemoveActionFor(AptValue *pValue);
    // 0x006E3920 (the map files it as rva006E3920 on a placeholder return type): the action at an index counted from the head, wrapping at the capacity.
    AptQueuedAction *actionAt(int index);
    // 0x006E3DB0 / 0x006E3DD0: the head action (validated through 0x006E3230) and the action after a given one (wrapping at the end of the buffer).
    AptQueuedAction *getCurrent();
    AptQueuedAction *next(AptQueuedAction *pAction);
    // 0x006E39A0: marks the values the queued actions hold for the collector (type 1: the context; type 2: the first two values).
    void rva006E39A0();
    void rva006E3740(AptValue *pContext, AptValue *pValue1, AptValue *pValue2, int nArgument0, int nArgument1);
    void prependAction(void *pAction, AptCIH *pCIH, int nArgument0, int nArgument1);
    // 0x006E34A0 (the map files it as rva006E34A0 on the placeholder class Rva006E34A0): the queue's scalar deleting destructor shape: gives the buffer back to the Apt pool and,
    // when bit 0 of the flag is set, the 0x14-byte queue itself; returns this.
    void *Destroy(unsigned int flags);

    AptQueuedAction *mpBuffer;   // 0x00
    AptQueuedAction *mpHead;     // 0x04
    AptQueuedAction *mpTail;     // 0x08
    AptQueuedAction *mpExec;     // 0x0C: the action being run; RemoveActionFor 0x006E4D50 never removes it (retail's name is m_pExec, the donor's)
    int mnCapacity;              // 0x10
};
