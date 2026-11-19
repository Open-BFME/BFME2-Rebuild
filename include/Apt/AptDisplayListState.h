#pragma once
// SIZE 0x4: the root of one display list. Constructor 0x006F7B20 (the map names it
// BfmeQuery1279) allocates the root AptCIH (0x60 bytes from the AptValueGC pool,
// built with type 0x2E, instance 0xBAADF00D, no parent), stores it here, makes it
// undefined, gives it one GC root, AddRefs it and sets its depth to the all-ones
// 17-bit value with null links. The retail assert text in AptDisplayList::~AptDisplayList
// names the member (`pHead->isUndefined()`). Allocation is the Apt block pool
// (operator new/delete take the pool path, size 4: 0x006F7FF0 and 0x006F8546).
// DisplayListItem 0x006F6D30 walks the list from here through AptCIH::mpNext.
#include "Apt/DogmaAllocator.h"
class AptCIH;
class EAStringC;

class AptDisplayListState
{
public:
    AptDisplayListState();
    ~AptDisplayListState(); // 0x006F7BC0 (the map names it BfmeQuery1279): asserts the root is undefined, clears its instance pointer and force-deletes it
    void DisplayListItem(void *item);
    // 0x006F7AF0 / 0x006F7AC0 (the map files them under the placeholder class Rva006F7AC0List): Apt.cpp's ApplyChild and
    // AnyActive entry points forward to them with the root list's state.
    void rva006F7AF0(int item);
    int getItemCount() const; // 0x006F6CE0: the number of nodes on the list from the root
    AptCIH *getItemAt(int index); // 0x006F6D00 (the map files it as rva006F6D00 on the placeholder class Rva006F6D00List): the node the given number of steps from the root, or null
    bool rva006F7AC0();
    // 0x006F6BB0 (map name bfmeQuery1279 on the placeholder class BfmeQuery1279): looks up the item at a depth and
    // returns two item pointers through the out parameters (the first is left alone when nothing is found).
    // The second parameter is the name to look for first (a defined item with that name wins; null: look by depth only).
    void bfmeQuery1279(int depth, const EAStringC *name, void **previous, void **found);
    // 0x006F7D50 (the map files it as rva006F7D50 on BfmeQuery1279): queries the item at a depth and unlinks it (bfmeUnlinkNestedBE), returning it.
    AptCIH *unlinkAtDepth(int depth);
    // 0x006F8060 (the map files it as rva006F8060 on a placeholder class): the destructor's body (asserts the root is undefined, clears its instance pointer and
    // force-deletes it) plus the pool free of the 4-byte state when bit 0 of the flag is set; returns this (the old decomp's name for the same shape on the pseudo list is Destroy).
    AptDisplayListState *Destroy(int flags);
    // 0x006F6ED0 (the map files it as rva006F6ED0 on the placeholder class BfmeQuery1279): builds a CIH of the given type for the instance (the pool block is 0x60 bytes, the
    // constructor runs with the instance and no parent), looks up what is at the depth, asserts that is undefined, stamps the 17-bit depth into the new item and
    // links it after the item the query found; returns the linked item.
    AptCIH *createAtDepth(int depth, int type, void *instance);
    // 0x006F6E00 (the map files it as rva006F6E00 on BfmeQuery1279): createAtDepth's linking half for a caller that has already queried the depth: builds a CIH of the type for the
    // instance, asserts the item found at the depth is undefined or absent (AptDisplayList.cpp line 0x1ec), stamps the depth and links it after `previous`.
    AptCIH *rva006F6E00(int depth, int type, void *instance, AptCIH *previous, AptValue *itemAtDepth);
    void rva006F6FB0(int depth, AptCIH *cih); // 0x006F6FB0: inserts cih at the given depth (map name kept)
    // 0x006F7030: the same link step for a caller that has already queried the previous node and the item at the depth.
    void insertAtFoundDepth(int depth, AptCIH *newItem, AptCIH *previous, AptCIH *itemAtDepth);
    // 0x006F6D60 (the map's ledger name): splices newItem into the list after previous and returns it.
    AptCIH *insert(AptCIH *previous, AptCIH *newItem);
    static void *operator new(unsigned int size) { return g_pDogmaPoolManager->allocBlock(size); }
    static void operator delete(void *block, unsigned int size) { g_pDogmaPoolManager->freeBlock(block, size); }

    AptCIH *pHead; // 0x0
};

typedef char AptDisplayListStateSize[(sizeof(AptDisplayListState) == 4) ? 1 : -1];
