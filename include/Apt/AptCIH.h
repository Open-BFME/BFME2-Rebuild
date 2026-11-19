#pragma once
// SIZE 0x60 (allocation 0x006F6E21 and the deleting destructor 0x006CBF55 both
// use 0x60). A character instance handle: the runtime object behind every
// movie clip, sprite, button and text field on screen. A garbage-collected
// AptValue (no hash of its own: GetNativeHashVirtual is overridden to
// reach the one the instance owns).
// Constructor 0x006CBDE0 calls AptValue's (type, flags) constructor, stores
// vtable 0x008E8C24, builds the EAStringC at +8, then zeroes +0x0C..+0x4C and
// the flag word at +0x5C. Destructor 0x006E2A70 destroys the string.
// Vtable 0x008E8C24 (15 slots): AddRef inherited; Release 0x006E05B0,
// GetNativeHashVirtual 0x006E0500, ContainsNativeHashVirtual 0x006E0510,
// getHasClass 0x006CBED0, setHasClass 0x006CBEB0, objectMemberLookup 0x006E0520,
// objectMemberSet 0x006E0560, PreDestroy 0x006E0440, DestroyGCPointers
// 0x006E0460, RegisterReferences 0x006E11B0 and the deleting destructor 0x006CBF40
// last; IsGarbageCollected (0x001826C0, returns true) is AptValueGC's.
// Member evidence: factoryEnsureProperties 0x006E06A0 allocates the 12-float block
// at +0x44; factorySetProperty 0x006E0930 writes it and derives the floats at
// +0x0C..+0x40 (cos/sin of a rotation, scales, translation, alpha); SetEventHandler
// 0x006E0B20 ors a mask into the hash's handler word (the hash of the instance);
// 0x006E0B60 follows the +0x48 parent chain; the display-list links at +0x50/+0x54 are
// proven by 0x006F7B97/0x006F7BA4 and 0x006F6D30; +0x58 holds a signed 17-bit depth (0x006F72F0);
// the flag word at +0x5C has bit 16 written by factorySetProperty.
// Names other than the first four are this header's (the donor's PDB names for
// these members were not used for want of retail evidence).
#include "Apt/AptValueGC.h"
#include "Apt/EAStringC.h"
#include "Apt/AptValueGC_PoolManager.h"
#include "Apt/Assert.h"

class AptRenderingContext;
struct AptSpriteInstBase;
struct AptCharacterAnimationInst;

// The per-type instance counts a movie clip info record keeps; AptCIH::rva006E2560 (0x006E2560, retail asserts `pMCInfo`) adds one for the
// kind of the instance it visits (the table at 0x006E2624 maps the kinds 0x0C..0x12 to these offsets). The names are this header's.
struct AptMCInfo
{
    int nAnimationInsts;  // 0x00: kind 0x12
    int nSpriteInsts;     // 0x04: kind 0x0D
    int nButtonInsts;     // 0x08: kind 0x0E
    int nStaticTextInsts; // 0x0C: kind 0x10
    int nTextInsts;       // 0x10: kind 0x0F
    int nMorphInsts;      // 0x14: kind 0x11
    int nShapeInsts;      // 0x18: kind 0x0C
};

struct AptCIHDepthBits
{
    int nDepth : 17;
    int nCreated : 14;       // the creation stamp 0x006F91F0 stores and 0x006F9410 compares (the old decomp's reading of the same bits)
    unsigned int mTop : 1;
};

class AptCIH : public AptValueGC
{
public:
    AptCIH(int type, void *instance, AptValue *parent);
    virtual void Release();
    virtual AptNativeHash *GetNativeHashVirtual();
    virtual bool ContainsNativeHashVirtual() const;
    virtual int getHasClass() const;
    virtual void setHasClass(int hasClass);
    virtual AptValue *objectMemberLookup(AptValue *const self, const EAStringC *const name) const;
    virtual bool objectMemberSet(AptValue *const self, const EAStringC *const name, AptValue *const value);
    virtual void PreDestroy();
    virtual void DestroyGCPointers();
    virtual void RegisterReferences() const;

    // 0x006CFF40 (the map files it as rva006CFF40@AptCIH): the sprite playback state, or null.
    AptSpriteInstBase *GetSpriteInstBase() const;
    bool isSpriteInstBase() const; // 0x006CFCD0
    // Inline in retail (expanded inside AptMovie::DoTemporaryFrameControls, whose assert text calls it
    // `pSprInst->getSpriteInstBase()->nFrame`): asserts isSpriteInstBase() (AptCIH.h line 0x7d), then returns the instance
    // pointer at +0x4C. The out-of-line row 0x006CFF40 (GetSpriteInstBase above) is its emitted copy.
    AptSpriteInstBase *getSpriteInstBase() const
    {
        if (!isSpriteInstBase()) {
            g_bfmeAptAssertAtE17734("isSpriteInstBase()", "c:\\projects\\bfme2patch103\\bfme2\\code\\libraries\\source\\apt\\AptCIH.h", 0x7d);
            if (g_bfmeAptBreakOnAssertAtDDC01C) APT_ASSERT_BREAK();
        }
        return static_cast<AptSpriteInstBase *>(mpInstance);
    }
    // Allocation is the AptValue pool (0x006CC58A: `mov ecx,[0x00E176F4]; push 0x60; call 0x006D29E0`).
    static void *operator new(unsigned int size) { return g_pAptValueGCPoolManager->allocBlock(size); }
    // The unwind cleanup of a `new AptCIH` (0x007A8080: push 0x60, push block, call 0x006F12F0) calls this out of line.
    static void operator delete(void *block, unsigned int size) { g_pAptValueGCPoolManager->freeBlock(block, size); }
    // The depth is a signed 17-bit field at the bottom of mDepth (0x006CC570: shl 15, sar 15).
    int getDepth() const { return (int)(mDepth << 15) >> 15; }
    // 0x006CD650 (the map files it under the placeholder class Rva006CD650): asserts this != NULL (AptCIH.h line 0xd3) and
    // `isAnimationInst()` (line 0x9b: type 0x12 and not undefined), then returns the instance pointer at +0x4C.
    AptCharacterAnimationInst *rva006CD650() const;
    // 0x006E1170 (the map files it as rva006E1170@AptCIH): the instance pointer (+0x4C) as a plain pointer; the movie it belongs to is
    // the character at its +0x0C (AptAnimationPoolData::removeTimerFunctions compares the two).
    void *rva006E1170() const;
    // Inline in retail (expanded inside AptAnimationPoolData::removeTimerFunctions, whose assert text is `isCharacterInst()`):
    // asserts isCharacterInst() (AptCIH.h line 0xa5), then returns the instance pointer at +0x4C. The name is this header's.
    void *getCharacterInst() const
    {
        if (!isCharacterInst()) {
            g_bfmeAptAssertAtE17734("isCharacterInst()", "c:\\projects\\bfme2patch103\\bfme2\\code\\libraries\\source\\apt\\AptCIH.h", 0xa5);
            if (g_bfmeAptBreakOnAssertAtDDC01C) APT_ASSERT_BREAK();
        }
        return mpInstance;
    }
    // 0x006E0CB0 (the map files it as rva006E0CB0@AptCIH): walks the parent chain (+0x48) to the CIH the script context belongs to.
    const AptCIH *rva006E0CB0() const;
    // Inline in retail (expanded inside AptInput.cpp's validateBIL and AptAnimation.cpp's removeFromBIL, whose assert text calls it
    // `pBI->isButtonInst(UNDEF_OK)`): asserts this != NULL (AptCIH.h line 0xb5), then type 0xE and, unless undefined is allowed, defined.
    bool isButtonInst(bool bUndefOK = false) const
    {
        if (!(this != 0)) {
            g_bfmeAptAssertAtE17734("this", "c:\\projects\\bfme2patch103\\bfme2\\code\\libraries\\source\\apt\\AptCIH.h", 0xb5);
            if (g_bfmeAptBreakOnAssertAtDDC01C) __debugbreak();
        }
        return getVtblIndex() == 0xE && (bUndefOK || !isUndefined());
    }
    // Inline in retail (expanded inside AptAnimationPoolData::_tickNewInsts): asserts this != NULL (AptCIH.h line 0xab), then type 0xD
    // and defined. The name is this header's.
    bool isSpriteInst() const
    {
        if (!(this != 0)) {
            g_bfmeAptAssertAtE17734("this", "c:\\projects\\bfme2patch103\\bfme2\\code\\libraries\\source\\apt\\AptCIH.h", 0xab);
            if (g_bfmeAptBreakOnAssertAtDDC01C) APT_ASSERT_BREAK();
        }
        return getVtblIndex() == 0xD && !isUndefined();
    }
    // Inline in retail (expanded inside AptDisplayList::rva006F7720Sub and AptActionInterpreter::callFunction): asserts this != NULL (AptCIH.h line 0xd8),
    // then type 0x13 (the level's root instance) and defined. The name is this header's.
    bool isLevelInstInline() const
    {
        if (!(this != 0)) {
            g_bfmeAptAssertAtE17734("this", "c:\\projects\\bfme2patch103\\bfme2\\code\\libraries\\source\\apt\\AptCIH.h", 0xd8);
            if (g_bfmeAptBreakOnAssertAtDDC01C) APT_ASSERT_BREAK();
        }
        return getVtblIndex() == 0x13 && !isUndefined();
    }
    // 0x006E1090 / 0x006E1F00 (the map files them as rva006E1090 / rva006E1F00 on AptCIH): the button state record at the instance (its word at
    // +0x18 is the state _tickNewInsts tests) and the state setter. 0x006E3C80 (the map files it under the placeholder class Rva006E3C80) is the
    // sprite instance's counterpart: it asserts isSpriteInst-style type 0xD (AptCIH.h line 0x78) and returns the instance pointer at +0x4C.
    void *rva006E1090() const;
    void rva006E1F00(int state);
    int rva006E3C80();
    void SetEventHandler(int mask);
    void RemoveEventHandler(int mask);
    void factoryEnsureProperties();
    void factorySetProperty(int index, float value, bool flag);
    float factoryGetProperty(int index) const;
    bool queueClipEvents(int a, int b, int c);
    void ClearCIH(int flags);
    void jumpToFrame(int frame);
    void rva006F97B0(); // 0x006F97B0 (the map files it under the placeholder class Rva006F97B0): takes a count on the CIH's zombie word at +0x5C (the counterpart of ReleaseZombieReference)
    // 0x006CBDA0 / 0x006CBDC0 / 0x006CBDD0 (the map files them as set / get / rva006CBDD0 on three placeholder classes): the two-bit state at flag bits 18..19
    // and the zombie counter's value.
    // 0x006F6960 (the map files it as rva006F6960 on a placeholder class): flag bit 16 of the word at +0x5C.
    unsigned int getFlag16() const;
    void setClearState(int state);
    int getClearState() const;
    unsigned int getZombieCounter() const;
    void ReleaseZombieReference(); // 0x006E0D40: drops one count of the 16-bit zombie counter at +0x5C (the map files it as rva006E0D40)
    // Inline in retail (expanded in the AptScriptFunctionBase constructors 0x00709870 and 0x00709A10, whose assert text calls the counter
    // `nZombieCounter`): asserts it is below 65536 (AptCIH.h line 0x53), then takes one count (`inc word [this+0x5C]`).
    void IncZombieCount()
    {
        if (!(nZombieCounter < 65536)) {
            g_bfmeAptAssertAtE17734("nZombieCounter < 65536", "c:\\projects\\bfme2patch103\\bfme2\\code\\libraries\\source\\apt\\AptCIH.h", 0x53);
            if (g_bfmeAptBreakOnAssertAtDDC01C) APT_ASSERT_BREAK();
        }
        ++nZombieCounter;
    }
    void rva006E2D60(); // 0x006E2D60 (AptDisplayList::rva006F7A30 runs it on button and sprite-inst-base nodes)
    void bfmeFinish1279(); // 0x006E2B40 (map name: bfmeFinish1279 on BfmeNode1279)

    int rva006E0B60() const;            // 0x006E0B60: the number of ancestors on the +0x48 parent chain
    int rva006E0B80(int nLvl) const;    // 0x006E0B80: the depth of the ancestor nLvl levels from the root (-1 if beyond the chain)
    bool rva006E0BF0() const;           // 0x006E0BF0: every alpha in the parent chain is at least one half
    bool rva006E0C50(const AptCIH *other) const; // 0x006E0C50: other is an ancestor
    void *rva006E0FB0() const;          // 0x006E0FB0: the static text instance (asserts type 0x10)
    void *rva006E1020() const;          // 0x006E1020: the morph instance (asserts type 0x11)
    void *rva006E1100() const;          // 0x006E1100: the shape instance (asserts type 0x0C)
    int rva006E1F90(int flag);          // 0x006E1F90: tests the flag word of the sprite instance, its hash and the hash of its prototype
    bool rva006E2460(const AptCIH *other) const; // 0x006E2460: orders two instances by their ancestor depth codes
    void rva006E1C40(AptRenderingContext *context, int rect); // 0x006E1C40: grows the rect (4 floats) by the instance's bounds
    // 0x006E0610 (the map files it as bfmeAccumulate1206 on the placeholder class BfmeNode1206): concatenates the 6-float matrices at +0x0C of this instance and
    // every ancestor (+0x48) onto the default matrix and returns the translation (elements 4 and 5); either pointer may be null.
    void getAccumulatedPosition(float *pX, float *pY);
    void rva006E1DD0(void *pRect);      // 0x006E1DD0: resets the rect to empty (+1e9, -1e9) and grows it by the bounds
    void rva006E1E30(float *pRect);     // 0x006E1E30 (the map files it on the placeholder class Rva006E1E30; AptAnimationPoolData::rva006F9FC0 calls it with the button): the instance's bounds as left, top, right, bottom
    bool rva006E24E0() const; // 0x006E24E0 (the map name): whether the instance still has something to run (sprite: its display list is active)
    void rva006E2560(int item); // 0x006E2560 (the map name): applies one child to the instance's movie clip info
    int rva006E24D0();                  // 0x006E24D0: rva006E1F90 with the clip-event mask 0x9FC38

    // 0x006E1260 / 0x006E15C0 (the map files them as call on the placeholder class Rva006E1260): the two per-instance draw steps AptDisplayList's 0x006F7330 runs
    // (the first for text instances, with the parent; the second draws the instance's contents into the context).
    void rva006E1260(void *pParent);
    void rva006E15C0(AptRenderingContext *context, void *pClip, int nRect);

    EAStringC mName;        // 0x08
    float mTransform[14];   // 0x0C..0x40
    float *mpProperties;    // 0x44, 12 floats allocated by factoryEnsureProperties
    AptCIH *mpParent;       // 0x48
    void *mpInstance;       // 0x4C
    AptCIH *mpPrevious;     // 0x50 (zeroed by the display list state, name not evidenced)
    AptCIH *mpNext;         // 0x54: AptDisplayListState::DisplayListItem 0x006F6D30 walks the list through it
    union {
        unsigned int mDepth;    // 0x58 (signed 17-bit depth in the low bits)
        AptCIHDepthBits depthBits; // the same word as its 17-bit depth field and the rest (AptCharacter.cpp 0x006EDF20 stores the field through a reference)
    };
    // 0x5C: the low 16 bits are the zombie reference count (ReleaseZombieReference 0x006E0D40: `dec word [this+0x5C]`, then a test of
    // the low word); the flag bits above it are what the other bodies test (bit 16: factorySetProperty; 0xC0000: the clip event groups).
    // Bit 17 is the has-class flag (getHasClass 0x006CBED0 / setHasClass 0x006CBEB0, vtable slots) and bits 18..19 a two-bit state that
    // AptCIH::Release (0x006E05B0), ClearCIH and bfmeFinish1279 test through the mask 0xC0000 (0: live, 1: already cleared); the accessors are 0x006CBDA0 / 0x006CBDC0
    // and the counter's is 0x006CBDD0. The names of everything but nZombieCounter (the retail assert text) are this header's.
    union {
        unsigned int mFlags;
        struct {
            unsigned int nZombieCounter : 16;
            unsigned int mFlag16 : 1;
            unsigned int mbHasClass : 1;
            unsigned int mClearState : 2;
        };
    };

protected:
    virtual ~AptCIH();
};

typedef char AptCIHSize[(sizeof(AptCIH) == 0x60) ? 1 : -1];
