#pragma once
// The small iterator, counter, record and handle rows of the playback/handle list family (Apt.cpp 0x006CC230..0x006CE600 and
// AptLoad.cpp 0x006CFD30..0x006D0F20). The types and names are this header's: the map files these rows under placeholder classes.
#include "Apt/DogmaAllocator.h"
#include "Apt/AptValueNameEntry.h"

// Small helpers of the playback list family (8-byte entries). The types and names are this file's: the map files these rows under placeholder
// classes (Rva006CDD50Iterator, Rva006CE660Vec, Rva00892500Object, Rva006CD530, ...).
// The 8-byte (name, index) entry of the playback buffers: retail's own AptValueNameEntry (the copy helpers assign the name through
// EAStringC::operator= 0x006D3030 and copy the int).
typedef AptValueNameEntry AptEntry;
// Retail's checked-iterator asserts are compiled out of the release build; the condition is not evaluated.
inline void aptIteratorCheck(bool) {}
struct AptEntryIterator
{
    AptEntryIterator() {}
    AptEntryIterator(AptEntry *pCur, AptEntry *pFirst, AptEntry *pLast) : mpCur(pCur), mpFirst(pFirst), mpLast(pLast) {}
    AptEntry *mpCur;
    AptEntry *mpFirst;
    AptEntry *mpLast;
    AptEntryIterator &advance();                          // 0x006CD4B0
    AptEntryIterator &retreat();                          // 0x006CD5E0
    int distance(const AptEntryIterator &other) const;    // 0x006CD590
    AptEntryIterator next(int);                           // 0x006CD5F0 (postfix ++)
    AptEntryIterator plus(int n) const;                   // 0x006CDD30
    // The checked-iterator operators the callers inline (the original asserted "Trying to dereference an invalid iterator" and
    // "Iterators are not in same range"; the release build compiles the checks out).
    AptEntry &operator*() { aptIteratorCheck(mpCur >= mpFirst && mpCur < mpLast); return *mpCur; }
    bool operator!=(const AptEntryIterator &other) { aptIteratorCheck(mpFirst == other.mpFirst && mpLast == other.mpLast); return mpCur != other.mpCur; }
    AptEntryIterator &operator++() { ++mpCur; return *this; }
    AptEntryIterator operator++(int) { AptEntryIterator previous = *this; ++mpCur; return previous; }
};
// The first/last pair the iterators of one table are made from.
struct AptEntryRange
{
    AptEntryRange(AptEntry *pFirst, AptEntry *pLast) : mpFirst(pFirst), mpLast(pLast) {}
    AptEntry *mpFirst;
    AptEntry *mpLast;
    AptEntryIterator begin() { return AptEntryIterator(mpFirst, mpFirst, mpLast); }
    AptEntryIterator end() { return AptEntryIterator(mpLast, mpFirst, mpLast); }
};
// A table of 8-byte state entries (count, capacity, data) the playback code asks whether every entry is in one of two states.
struct AptStateTable
{
    int mnCount;
    int mnCapacity;
    AptEntry *mpData;
    AptEntryIterator begin() { return AptEntryRange(mpData, mpData + mnCount).begin(); }
    AptEntryIterator end() { return AptEntryRange(mpData, mpData + mnCount).end(); }
    bool allSettled(); // 0x006D0410: every entry is in state 3 or 2
};
// The playback state buffer: the table with two entries stored inline behind it (+0x0C).
struct AptPlaybackBuffer : AptStateTable
{
    AptEntry mInline[2];
    bool readyFor(int firstState, int secondState); // 0x006CE760
    bool ready();                                   // 0x006CE7A0: every entry is in state 4 or 2
};
struct AptEntryVector
{
    int mnCount;
    int mnCapacity;
    AptEntry *mpData;
    // 0x006CDBA0 (the map files it as Gen_uw_00893e70): empty, the data pointing at the inline pair. A class-body inline: clear() 0x006D0390 expands it in
    // place and retail keeps one out-of-line copy for the callers that do not.
    AptEntryVector() : mnCount(0), mnCapacity(0), mpData(mInline) {}
    ~AptEntryVector(); // 0x006CE570: gives the data back unless it still is the inline pair
    AptEntry mInline[2]; // 0x0C: the two entries the vector starts with (the data pointer points here until it grows; 0x006CF956 allocates the pair)
    AptEntryIterator begin(); // 0x006CE5E0
    AptEntryIterator end();   // 0x006CE600
    void reserve(int nWanted);                                                                       // 0x006CE660: grows the data to hold nWanted entries plus a terminator
    void insert(AptEntry *const &first, AptEntry *const &last, const AptEntryIterator &position);    // 0x006CE890
    void append(AptEntry *pEntry);                                                                   // 0x006CEA90
    void swap(AptEntryVector *pOther); // 0x006D0040: exchanges the counts, capacities and data; the inline pairs trade places when either side still points at its own
    void clear();                      // 0x006D0390: swaps with a fresh empty vector
    void updatePlaybackState(const EAStringC *pKey, int nExpected, int nReplacement, int nInitial);  // 0x006D0930
    void settlePlaybackName(const EAStringC *pKey);                                                  // 0x006D0A10: updatePlaybackState(key, 1, 3, 2)
};
struct AptInlineBuffer
{
    int reserved0;
    int reserved4;
    void *mpData;      // 0x08
    char mInline[4];   // 0x0C
    bool usesInlineBuffer(); // 0x006CD4E0
};
bool AptNotEqualInt(const int *pValue, int value); // 0x006CD560
// The entry-copy helpers of the playback buffer: forward copy of a pointer range (0x006CD5A0), backward copy of an iterator range ending
// at the destination's last element (0x006CDD50), forward copy of a pointer range into an iterator (0x006CDD90) and of an iterator
// range into a pointer (0x006CDE00). Each assigns the name through EAStringC::operator= and copies the index.
AptEntry *aptCopyEntries(AptEntry *first, AptEntry *last, AptEntry *result);
AptEntry *aptCopyEntriesBackward(AptEntryIterator first, AptEntryIterator last, AptEntry *result);
AptEntryIterator aptCopyEntriesToIterator(AptEntry *first, AptEntry *last, AptEntryIterator result);
AptEntry *aptCopyEntryIterators(AptEntryIterator first, AptEntryIterator last, AptEntry *result);
struct AptRecord8
{
    int mnA; // 0x00
    int mnB; // 0x04
    int mnC; // 0x08
    int mnD; // 0x0C
    int mnE; // 0x10
    int mnF; // 0x14
    int mnG; // 0x18
    int mnH; // 0x1C
    void reset(); // 0x006CC230
    // 0x006CC160 / 0x006CC1E0 (the map files them under the placeholder class Rva006CC160): the field-wise sum, by value and in place.
    AptRecord8 operator+(const AptRecord8 &other);
    AptRecord8 &operator+=(const AptRecord8 &other);
};
void AptClearRecord(AptRecord8 *pRecord); // 0x006CD3A0
int AptDecRef(int *pCount);               // 0x006CFDF0 (AptLoad.cpp)
void AptDestroyShared(void *pShared);     // 0x006D1210 (not recovered yet)
struct AptSharedHandle
{
    int *mpCount;
    void detach(); // 0x006CD530
};
struct AptHandleNode
{
    AptSharedHandle mHandle; // 0x00
    AptHandleNode *mpNext;   // 0x04
};
struct AptHandleStack
{
    AptHandleNode *mpHead;
    void popPlain();  // 0x006CDBD0
    void popDetach(); // 0x006CE860
    void detachAll(); // 0x006CEAD0: pops (and detaches) every node
};


// Small helpers of the handle/key list family. The types and names are this file's: the map files these rows under placeholder
// classes (Rva006CFD30, Rva006CFDB0ByteField, Rva006CFEA0Iterator, Rva006D0350Vector, ...).
struct AptStepCounter
{
    int mnValue;
    int increment(); // 0x006CFD30
    int decrement(); // 0x006CFD40
};
struct AptByteField
{
    char reserved0[0xC];
    unsigned char mnByte; // 0x0C
    unsigned char get() const;       // 0x006CFDB0
    void set(unsigned char value);   // 0x006CFDC0
};
struct AptPairBox
{
    int first;
    int second;
};
struct AptRefCountAccessor
{
    static unsigned int increment(unsigned int *pCount); // 0x006CFDE0
};
struct AptWordIterator
{
    int *mpCur;
    AptWordIterator &advance();                     // 0x006CFEA0
    AptWordIterator &retreat();                     // 0x006CFF00
    int distance(const AptWordIterator &other) const; // 0x006CFEF0
};
struct AptWordRange
{
    AptWordRange() {}
    AptWordRange(int *pCur, int *pFirst, int *pLast) : mpCur(pCur), mpFirst(pFirst), mpLast(pLast) {}
    int *mpCur;
    int *mpFirst;
    int *mpLast;
    AptWordRange next(int);      // 0x006CFF10 (postfix ++)
    AptWordRange plus(int n) const; // 0x006D0170
};
struct AptWordVector
{
    int mnCount;
    int mnCapacity;
    int *mpData;
    AptWordRange begin(); // 0x006D0350
    AptWordRange end();   // 0x006D0370
};
void AptStoreSecond(AptPairBox *pBox, int value); // 0x006CFDD0
int AptDecRef(int *pCount);                       // 0x006CFDF0
bool AptEqualsInt(const int *pValue, int value);  // 0x006CFED0
// The shared 0x1C-byte block the playback handles count references on (its destructor is out of line, 0x006D0280).
struct AptSharedBlock
{
    // 0x006CFD50: a fresh record counted 0 with the name, kind 1 and the object and buffer words null (the data word is left alone); retail keeps one out-of-line copy and
    // the registry rows expand it. The layout is AptFile's (the loaded-file record).
    AptSharedBlock(const EAStringC *pName) : mnRefs(0), mName(*pName), mnKind(1), mpObject(0), mpBuffer(0) {}
    int mnRefs;
    int reserved04;
    EAStringC mName;     // 0x08
    int mnKind;          // 0x0C: 0x006CE180 notes whether it is 4 (the loaded-file state AptFile keeps at the same offset)
    int reserved10;
    void *mpObject;      // 0x14
    void *mpBuffer;      // 0x18
    ~AptSharedBlock(); // 0x006D0280
    int increaseCount() { return ++mnRefs; }
    int decreaseCount() { int &nRefs = mnRefs; return --nRefs; }
    static void *operator new(unsigned int size) { return g_pDogmaPoolManager->allocBlock(size); }
    static void operator delete(void *block, unsigned int size) { g_pDogmaPoolManager->freeBlock(block, size); }
};
// A counted by-value handle on the shared block (the map files the rows that pass it under the placeholder class Rva006D07E0Key): taking one counts the block, the last
// one to go destroys it and gives the 0x1C bytes back to the pool.
struct AptFileKey
{
    AptFileKey(AptSharedBlock *pBlock = 0) : mpBlock(pBlock)
    {
        if (pBlock)
            pBlock->increaseCount();
    }
    AptFileKey(const AptFileKey &other)
    {
        mpBlock = other.mpBlock;
        if (mpBlock)
            mpBlock->increaseCount();
    }
    ~AptFileKey()
    {
        AptSharedBlock *pCounted = mpBlock;
        if (pCounted && pCounted->decreaseCount() == 0) {
            AptSharedBlock *pReleased = mpBlock;
            if (pReleased) {
                pReleased->~AptSharedBlock();
                g_pDogmaPoolManager->freeBlock(pReleased, 0x1C);
            }
        }
    }
    AptSharedBlock *mpBlock;
};
// The keyed chain of AptLoad.cpp 0x006D07E0 / 0x006D0860 (map placeholder class Rva006D07E0List): nodes of 8 bytes holding an entry (whose second word is the block the entry is keyed
// by) and the next node.
struct AptKeyEntry
{
    int reserved0;
    AptSharedBlock *mpKey; // 0x4
};
struct AptKeyNode
{
    AptKeyEntry *mpEntry;
    AptKeyNode *mpNext;
};
struct AptKeyIterator
{
    AptKeyIterator(AptKeyNode *pNode) : mpNode(pNode) {}
    bool operator!=(const AptKeyIterator &other) const { return mpNode != other.mpNode; }
    AptKeyIterator &operator++() { mpNode = mpNode->mpNext; return *this; }
    AptKeyEntry *operator->() const { return mpNode->mpEntry; }
    AptKeyNode *mpNode;
};
struct AptKeyList
{
    AptKeyNode *mpHead;
    AptKeyIterator find(AptFileKey key); // 0x006D07E0: the node whose entry is keyed by the block, or null
    int contains(AptFileKey key);        // 0x006D0860: whether any entry's file imports from the key's movie
};
// The registry of named blocks (map placeholder class Rva006D0A30List): nodes holding the block itself and the next node.
struct AptNamedNode
{
    AptNamedNode(AptSharedBlock *pBlock) : mpBlock(pBlock), mpNext(0) {}
    static void *operator new(unsigned int size) { return g_pDogmaPoolManager->allocBlock(size); }
    AptSharedBlock *mpBlock;
    AptNamedNode *mpNext;
};
class EAStringC;
struct AptNamedList
{
    AptNamedNode *mpHead;
    AptFileKey find(const EAStringC *pName);        // 0x006D0A30: the block whose name compares equal, counted
    AptFileKey findSpecial(const EAStringC *pName); // 0x006D0A90: the same, only when the block's kind is 4 or 5
    AptFileKey findOrAdd(const EAStringC &name);    // 0x006D0B40: the counted block for a name, a new one registered on the head when there is none
    void unload(const EAStringC &name);             // 0x006D0D70 (the map files it as rva006D0D70 on the placeholder class Rva006D1090): unloads the named file and everything that imports from it
    bool allImportsLoaded(AptFileKey key);          // 0x006D0C60: whether every movie the block's character imports from is registered with a kind of 4 or 5
};
extern AptNamedList *g_pAptFileRegistry; // VA 0x00E176CC (the map names it g_rva00893030Manager): the registry of loaded files
AptFileKey aptFindOrAddFile(const char *pName); // 0x006CE110
void aptRegisterLoadedFile(AptFileKey key);       // 0x006D1520: consumes a counted file handle: publishes it to the linker (0x006D1450) and notes it in the playback buffer when the playback flag is set
void __stdcall aptRegisterLoadedFileCallback(AptFileKey key); // 0x006D1600: the host-callable form (callee-clean) of the same
struct AptSharedRef
{
    int reserved0;
    AptSharedBlock *mpBlock; // 0x4
    void release(); // 0x006D0F20
};
// The 0x10-byte handle AptDestroyShared (0x006D1210) tears down: a shared reference at +0 and 8 bytes of payload; from the DOGMA pool.
struct AptSharedRef16
{
    AptSharedRef mRef;
    char reserved8[8];
    static void operator delete(void *block, unsigned int size) { g_pDogmaPoolManager->freeBlock(block, size); }
};
void bfmeDropVGO(void *pBlock);                   // 0x006D0440
bool aptReleaseSharedBlock(AptSharedBlock *pBlock); // 0x006CE180: drops one reference (0x006CFDF0), frees the block (0x006D0440) at zero; returns whether its kind was 4
// The element of the handle vectors (retail's placeholder class BfmeRefVGO): a counted pointer to the shared block. Assigning drops the old block's count
// (destroying and freeing it at zero) and takes the new one's; assigning an element to itself does nothing.
struct AptBlockRef
{
    AptSharedBlock *mpBlock;
    AptBlockRef &assign(const AptBlockRef &other); // 0x006CDB10: the out-of-line assignment the vector rows call (it counts through 0x006CFDF0 / 0x006CFDE0 and drops the block at zero; the copy helpers inline their own)
    AptBlockRef &operator=(const AptBlockRef &other)
    {
        if (&other != this) {
            AptSharedBlock *old = mpBlock;
            if (old) {
                if (--old->mnRefs == 0) {
                    AptSharedBlock *dead = mpBlock;
                    if (dead)
                        delete dead;
                }
            }
            mpBlock = other.mpBlock;
            if (mpBlock)
                ++mpBlock->mnRefs;
        }
        return *this;
    }
};
// The by-value iterator of the handle vectors (the map's placeholder Rva006D1130Iterator): the current element and the range's two ends.
struct AptBlockIterator
{
    AptBlockIterator() {}
    AptBlockIterator(AptBlockRef *pCur, AptBlockRef *pFirst, AptBlockRef *pLast) : mpCur(pCur), mpFirst(pFirst), mpLast(pLast) {}
    AptBlockRef *mpCur;
    AptBlockRef *mpFirst;
    AptBlockRef *mpLast;
    bool operator!=(const AptBlockIterator &other) { aptIteratorCheck(mpFirst == other.mpFirst && mpLast == other.mpLast); return mpCur != other.mpCur; }
    AptBlockIterator &operator++() { ++mpCur; return *this; }
    AptBlockIterator operator++(int) { AptBlockIterator previous = *this; ++mpCur; return previous; }
};
// The copy helpers of the handle vectors: a pointer range forward (0x006D0460), an iterator range backward ending at the destination's last element
// (0x006D04D0), a pointer range into an iterator (0x006D0540) and an iterator range into a pointer (0x006D05E0).
// The vector of counted block handles the linker keeps its pending files in (the map files its rows under the placeholder class Rva006D1130): count, capacity and data pointer, then
// the one slot it starts with. begin/end are the checked iterators the callers expand; append is an inline over insert.
struct AptBlockVector
{
    int mnCount;            // 0x0
    int mnCapacity;         // 0x4
    AptBlockRef *mpData;    // 0x8
    AptBlockRef mInline[1]; // 0xC
    AptBlockIterator begin() { return AptBlockIterator(mpData, mpData, mpData + mnCount); }
    AptBlockIterator end() { return AptBlockIterator(mpData + mnCount, mpData, mpData + mnCount); }
    void grow(int nCapacity);                                                                         // 0x006D1130
    void insert(AptBlockRef *const &first, AptBlockRef *const &last, const AptBlockIterator &position); // 0x006D1230
    void append(AptBlockRef *pValue)                                                                  // 0x006D13E0 (retail keeps an out-of-line copy too)
    {
        AptBlockRef *last = pValue + 1;
        AptBlockRef *first = pValue;
        insert(first, last, end());
    }
};
AptBlockRef *aptCopyBlockRefs(AptBlockRef *first, AptBlockRef *last, AptBlockRef *result);
AptBlockRef *aptCopyBlockRefsBackward(AptBlockIterator first, AptBlockIterator last, AptBlockRef *result);
AptBlockIterator aptCopyBlockRefsToIterator(AptBlockRef *first, AptBlockRef *last, AptBlockIterator result);
AptBlockRef *aptCopyBlockIterators(AptBlockIterator first, AptBlockIterator last, AptBlockRef *result);

// The singly linked chain list of the playback handle owners and the linker (AptLoad.cpp 0x006CFE30..0x006D01D0): a head
// pointer at +0 and 8-byte nodes (value at +0, next at +4) from the DOGMA pool. The map files its rows under the placeholder
// classes Rva006CFE30, Rva008951B0Owner and Rva006D01D0Tracker: they are one list (the node, the free size 8 and the
// unlink-and-free tails are identical).
struct AptChainNode
{
    void *mpValue;
    AptChainNode *mpNext;
};
// The by-value cursor the list's members take and return (a node pointer with a copy constructor).
struct AptChainCursor
{
    AptChainCursor(AptChainNode *pNode) : mpNode(pNode) {}
    AptChainCursor(const AptChainCursor &other) : mpNode(other.mpNode) {}
    AptChainNode *mpNode;
};
struct AptChainList
{
    AptChainNode *mpHead;
    AptChainCursor popFront(int);                  // 0x006CFE30: unlinks the head and returns it
    void pushFront(const AptChainCursor &value);   // 0x006CFF70: a new node holding the value on the head
    void eraseAfter(const AptChainCursor &at);     // 0x006CFFB0
    void erase(const AptChainCursor &at);          // 0x006CFFE0
    void remove(void *pValue);                     // 0x006D01D0
};

// The range/iterator pair the playback list hands out (class names from the map; the three words are the cursor, the first and
// the one-past-last element, as the checked iterator of the original container keeps them).
class AptValueNameEntry;
class PlaybackIterator
{
public:
    PlaybackIterator(AptValueNameEntry *pCur, AptValueNameEntry *pFirst, AptValueNameEntry *pLast) : mpCur(pCur), mpFirst(pFirst), mpLast(pLast) {}
    AptValueNameEntry *mpCur;
    AptValueNameEntry *mpFirst;
    AptValueNameEntry *mpLast;
};
class PlaybackRange
{
public:
    PlaybackRange(AptValueNameEntry *pFirst, AptValueNameEntry *pLast) : mpFirst(pFirst), mpLast(pLast) {}
    PlaybackIterator begin(); // 0x006D0240
    PlaybackIterator end();   // 0x006D0260
    AptValueNameEntry *mpFirst;
    AptValueNameEntry *mpLast;
};

