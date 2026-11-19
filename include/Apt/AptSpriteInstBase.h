#pragma once
// The per-sprite playback state an AptCIH points at (AptCIH::GetSpriteInstBase 0x006CFF40 returns it).
// ONLY the flag word at +0x1C is recovered: the Play/Stop actions (0x00700660, 0x007006D0) set and
// clear bit 25 (0x02000000) there. The size is not established, so nothing may embed it.
#include "Apt/AptDisplayList.h"

struct AptCharacter;
struct AptNativeHash;

// The flag word's bit-field view (see the union below).
struct AptSpriteFlagBits
{
    int reservedBits : 24; // signed in retail (0x006F6980/0x006F69A0 sign-extend it before the or/and)
    unsigned int mbLatched : 1;   // bit 24: a pending clip-event latch that AptCIH::rva006E2D60 (0x006E2D60) clears after queueing state 1
    unsigned int mbIsPlaying : 1;
    unsigned int reservedHigh : 6;
};

// One onClipEvent record of a sprite instance: 12 bytes, the event mask, the key code (key events only) and the action stream.
struct AptClipActionRecord
{
    unsigned int nEventMask;       // 0x0
    unsigned int nKeyCode;         // 0x4
    const unsigned char *pActions; // 0x8
};

struct AptClipActionSet
{
    int nRecords;                  // 0x0
    AptClipActionRecord *aRecords; // 0x4
};

struct AptSpriteInstBase
{
    // 0x006F6980 / 0x006F69A0 (the map files them under the placeholder class Rva006F6980Bits24): set and clear bits of the low 24 flag bits.
    void setFlagBits(int mask);
    void clearFlagBits(int mask);
    // 0x006E3490 (the map files it as rva006E3490 on the placeholder class Rva006E3490): empties the instance's display list without destroying the instances.
    void clearDisplayList();

    char reserved00[0xC];  // 0x00..0x0B
    AptCharacter *pCharacter; // 0x0C: the sprite's character (jumpToFrame 0x006E2C10 reads its frame count and runs its frame controls)
    AptNativeHash *pHash;  // 0x10: the instance's member hash (jumpToFrame hands it to mergeState)
    int reserved14;        // 0x14
    int currentFrame;      // 0x18: NextFrame/PrevFrame 0x00700560/0x007005E0 pass currentFrame +/- 1 to AptCIH::jumpToFrame
    union {
        unsigned int flags;    // 0x1C
        // GotoFrame2 0x00704FE0 stores `(operand != 0)` into bit 25 through the bit-field mask idiom (xor/and/xor); Play and Stop
        // (0x00700660, 0x007006D0) set and clear the same bit with the whole word.
        AptSpriteFlagBits bits;
    };
    AptClipActionSet *pClipActions; // 0x20: the onClipEvent records the instance was placed with (queueClipEvents 0x006E2010 walks them)
    AptDisplayList displayList; // 0x24: RemoveSprite 0x00707840 calls its removeClonedObject (0x006F72F0)
    int field28;           // 0x28: the frame number AptActionQueueC::enqueueAction (0x006E4B80) stamps into a queued action (name not evidenced)
    int mnMode;            // 0x2C: AptCIH::rva006E2D60 restarts the sprite at frame 0 instead of stepping when it is 1 (name not evidenced)
};
