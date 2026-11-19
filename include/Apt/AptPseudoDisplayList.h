#pragma once
// A display list built without any instances: AptMovie::DoTemporaryFrameControls (0x0070F040) replays a frame's
// controls into it to learn what a frame would show. SIZE 0x8. The retail assert text names the node type AptPseudoCIH_t and its members
// pControl / pControlInfo ("pItem->pControlInfo != NULL", "pItem->pControl->eType == AptControlType_PlaceObject2").
// Allocation of a node is the DOGMA pool (0x0070F100: `mov ecx,[0x00E176E8]; push 0x14; call 0x006DB160`) and the
// unwind frees it out of line through 0x006D8680.
#include "Apt/AptControl.h"
#include "Apt/DogmaAllocator.h"

class AptCIH;
struct AptCharacter;

// 0x1C bytes: the placement a node carries when a later PlaceObject2 only modifies it. Each pointer aims at the
// control's own data (the matrix, the colour words, the actions); the flags word accumulates the modify masks.
struct AptControlInfo
{
    AptCharacter *pCharacter; // 0x00: the character a placement creates (null when it only modifies the object at its depth; 0x006F9350 tests it)
    float *pMatrix;         // 0x04
    unsigned int *pCxform;  // 0x08
    void *pActions;         // 0x0C
    float ratio;            // 0x10
    int flags;              // 0x14
    short nCreated;         // 0x18: the creation stamp the new instance takes (0x006F91F0 stores it in the 14 bits above the depth of the instance's word at +0x58; 0x006F9410 compares it)
    short nClip;            // 0x1A: the clip depth (0x006F91F0 passes it on)
};

// SIZE 0x14. A node of the pseudo display list, kept ordered by depth.
struct AptPseudoCIH_t
{
    // 0x006F6A50 (the map files it under the placeholder class Rva006F6A50): sets the control, builds its
    // AptControlInfo for a placement and links nothing.
    AptPseudoCIH_t(AptControl *pControl, int nFrame, int nDepth, AptCharacter *pCharacter);
    static void *operator new(unsigned int size) { return g_pDogmaPoolManager->allocBlock(size); }
    static void operator delete(void *block, unsigned int size); // 0x006D8680 (out of line)

    AptControl *pControl;         // 0x00
    AptControlInfo *pControlInfo; // 0x04
    // 0x006E0E70 (the map files it as rva006E0E70 on the placeholder class Rva006E0DE0): drops the node's links and gives its
    // 0x1C-byte control info back to the pool (the step 0x006F7C10 expands inline for every node).
    void Reset();
    // 0x006E0EB0 (the map files it as rva006E0EB0 on the placeholder class Rva006E0EB0): Reset's steps plus the pool free of the 0x14-byte node when bit 0 of the flag
    // is set; returns this (the node's scalar deleting destructor shape).
    void *Destroy(unsigned int flags);
    AptPseudoCIH_t *pNext;        // 0x08
    AptPseudoCIH_t *pPrev;        // 0x0C
    int nDepth;                   // 0x10
};

typedef char AptPseudoCIHSize[(sizeof(AptPseudoCIH_t) == 0x14) ? 1 : -1];

// 0x006F7C70 (the map names it Rva006F7C70Free, a __stdcall free function over a placeholder item type): unlinks and frees a node.


class AptPseudoDisplayList
{
public:
    // 0x006E00E0 (the ledger name): allocates the sentinel node (four zero arguments) and stores the parent instance.
    AptPseudoDisplayList(AptCIH *pParent);
    // The list is an 8-byte block of the Apt pool (jumpToFrame 0x006E2C10 allocates it with pool size 8 and destroys it with Destroy(1)); the unwind
    // of its constructor frees it out of line through 0x006D8680.
    static void *operator new(unsigned int size) { return g_pDogmaPoolManager->allocBlock(size); }
    static void operator delete(void *block, unsigned int size); // 0x006D8680 (out of line)
    // 0x006F6AC0 (map name rva006F6AC0 on a placeholder class): the node at a depth and the node before the place it
    // would go; either out parameter may come back null.
    void FindInst(int nDepth, AptPseudoCIH_t **ppPrev, AptPseudoCIH_t **ppItem);
    // 0x006F8380: inserts a node, first removing any node already at its depth.
    void Insert(AptPseudoCIH_t *pItem);
    // 0x006F6B80: links pItem after pPrev (the old name for it is Insert(prev, item)).
    void Insert(AptPseudoCIH_t *pPrev, AptPseudoCIH_t *pItem);
    // 0x006F7C70 (the map names it Rva006F7C70Free: a stdcall free function over a placeholder item type, but retail calls it with
    // this in ecx and the node on the stack): unlinks a node and frees it.
    void Remove(AptPseudoCIH_t *pItem);
    // 0x006F7C10 (the map files it as rva006F7C10 on the placeholder class Rva006F7C10): gives every node after the sentinel back to the pool
    // (the node's 0x1C-byte control info first, then the 0x14-byte node).
    void Clear();
    // 0x006E2640 (the map files it as rva006E2640 on Rva006F7C10): Clear, then the sentinel node itself. Also the unwind of a list whose owner failed.
    ~AptPseudoDisplayList();
    // 0x006E2A00 (the map files it as rva006E2A00 on Rva006E2A00; the old name for it is Destroy): the destructor's body plus the pool free of the 8-byte list
    // when bit 0 of the flag is set (AptCIH::jumpToFrame 0x006E2C10 destroys its temporary list with Destroy(1)); returns this.
    AptPseudoDisplayList *Destroy(int flags);
    // 0x006F83F0: removes pOld when given, then links pItem after pPrev (the map name is kept).
    void rva006F83F0(AptPseudoCIH_t *pItem, AptPseudoCIH_t *pPrev, AptPseudoCIH_t *pOld);
    // 0x006F8430: removes the node at a depth (the map name is kept).
    void rva006F8430(int nDepth);

    AptPseudoCIH_t *pHead; // 0x00: the sentinel node the list hangs from (the constructor 0x006E00E0 allocates it with four zero arguments)
    AptCIH *pParent;  // 0x04: the sprite instance the frame belongs to
};
// The constructor is a COMDAT in retail (0x006E00E0, 102 bytes: called from AptCIH::jumpToFrame and the pseudo-frame builders): inline, so every includer that creates a list emits it.
inline AptPseudoDisplayList::AptPseudoDisplayList(AptCIH *pParentInstance)
{
    pHead = new AptPseudoCIH_t(0, 0, 0, 0);
    pParent = pParentInstance;
}

