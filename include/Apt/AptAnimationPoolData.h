#pragma once
// The animation runtime's central state: the interval timers, the action queue, the button
// interaction list (BIL) and the focus/navigation logic (rows in AptAnimation.cpp and AptInput.cpp).
// ONLY the members below are recovered, so the class is usable through a pointer and its size is
// NOT established (the donor's old views put the timers at +0x34 and the action queue at +0xA0).
// BIL: appendButtonToBIL 0x006E35E0 stores the button in the first word of a 0x1C-byte item
// (AddRef'd), the 6-float matrix at +4 and bumps the count at +0x10; clearBIL 0x006E35A0 releases
// every button; removeFromBIL 0x006E3640 asserts the removed value is a CIH (type 0xE) and slides
// the rest down with memmove. The item type name is this header's.
#include "Apt/AptRenderingContext.h"
#include "Apt/AptDisplayList.h"
#include "Apt/AptSet.h"
#include "Apt/AptIntervalTimer.h"

class AptCIH;
class AptValue;
struct AptCharacter;
class AptActionQueueC;
struct AptButtonDescriptor;

// The button state record AptCIH::rva006E1090 returns; only its state word (+0x18) is read (0 means not started).
struct AptButtonInstState
{
    char reserved00[0xC];
    AptCharacter *pCharacter; // 0x0C: the button character (AptCharacterButton.h)
    char reserved10[0x8];
    int nState; // 0x18
    AptDisplayList mDisplayList; // 0x1C: the instances of the records that match the state (AptCIH::rva006E1F00 0x006E1F00 refills it)
};

struct AptBILItem
{
    AptCIH *pButton;    // 0x00
    AptMatrix matrix;   // 0x04
};

typedef char AptBILItemSize[(sizeof(AptBILItem) == 0x1C) ? 1 : -1];

// One pointer event as the host hands it in (16 bytes, passed by value to 0x006CC9F0 and on to 0x006E4480): the position, a flag byte and the event kind (0x1F5 / 0x1F6).
struct AptPointerEvent
{
    int x;
    int y;
    unsigned char flag;
    int kind;
};

class AptAnimationPoolData
{
public:
    // 0x006E3FF0: tears down what the pool owns before it is destroyed: drains the set at +8, releases every live interval
    // timer (asserting none are left), the Key and Mouse listener sets and the BIL, then clears the display list.
    void PreDestroy();
    // 0x006E4150 / 0x006E4390 / 0x006E44F0 (their bodies are in AptAnimation.cpp).
    void tickIntervalTimers(int elapsed);
    void _tickNewInsts();
    void removeTimerFunctions(AptCIH *target);
    void clearBIL();
    void appendButtonToBIL(AptCIH *button, AptMatrix *matrix);
    void removeFromBIL(AptCIH *button);
    // 0x006FA020: walks the list from the back through the global pool pointer (not `this`) and asserts every entry is a
    // defined button instance. The retail assert text names the list `aButtonInstanceList`, the entry field `pCIH` and the
    // pool `gpPool` (members mpBIL / pButton above are this header's older spellings).
    bool validateBIL();
    // 0x006FB910 (map name rva006FB910): when field44 is defined runs 0x006F9EF0, then feeds every id in field40 to
    // 0x006FB860 (the first with its flag set) and clears the count.
    void rva006FB910();
    void Rva006F9EF0();
    void rva006FB860(int id, char first);
    void rva006FA7E0(bool first, bool enabled, int value); // 0x006FA7E0: queues the press / release / roll-over / roll-out clip events for the buttons the pointer touches
    bool rva006F9FC0(AptCIH *cih, int value); // 0x006F9FC0 (the map's parameter is a placeholder type): whether the point lies inside the button
    // 0x006FB120 / 0x006FB5B0 / 0x006FACA0 / 0x006FAF80: the input dispatch 0x006FB860 runs on a packed input word (the map names the
    // first two rva006FB120 / rva006FB5B0 and the last two HandleFocusButton / HandleAutoNav).
    void rva006FB120(int x, int y, int packed, int field, char first);
    void rva006FB5B0(int x, int y, int packed, int field);
    void AddListenerToQueue(AptValue *value, int mask, int arg); // 0x006FAB40: queues the listener's handler for a mouse or key event mask
    bool HandleFocusButton(int x, int y, void **candidate);
    void HandleAutoNav(AptCIH *candidate, int x, int y);
    // 0x006E3580 / 0x006E3530 (the map files them under the placeholder class Rva006E34D0): the mouse-position and the
    // three-integer input entry points Apt.cpp forwards to once Apt is initialized and not paused.
    // 0x006E34D0 (the map files it as rva006E34D0 on the placeholder class Rva006E34D0): appends a packed input word to the id array
    // (field40, count field3C, capacity mnMaxInputEvents) unless it is full or the word repeats the last one; when the diagnostics
    // channel is open (VA 0x00E17708) it also sends the word as an 8-byte record through the host callback VA 0x00E17740.
    void queueInputEvent(int packed);
    void rva006E3580(int x, int y);
    void rva006E4480(AptPointerEvent event); // 0x006E4480: records a 0x1F5/0x1F6 pointer event, then queues it
    void rva006E3530(int a, int b, int c);
    int getFieldA4() const; // 0x006F6970 (AptDisplayList.cpp)
    void rva006E3700(int focus); // 0x006E3700 (placeholder class Rva006E3700DwordSlot in the map): sets the focus target (an AptCIH pointer, or 0)
    void setValidFocusButton(); // 0x006FA340 (AptInput.cpp); Apt.cpp 0x006CC5F0 forwards to it
    void rva006E6540(); // 0x006E6540 (the map name): runs after a button state change has queued its actions
    bool bfmeMeshHitTest1290(AptButtonDescriptor *desc, const AptMatrix *transform, int x, int y); // 0x006F9DE0 (the map name): whether the point lies inside the button's mesh shape
    void ProcessButtonState(); // 0x006FAA20: finds the button under the pointer (field74/field78) and moves focus and the press state to it
    AptCIH *rva006FA420(int x, int y); // 0x006FA420 (the map name): the topmost button instance whose hit area contains the point, or null
    void rva006FA100(AptCIH *cih, int mode); // 0x006FA100 (the map name): broadcasts a button state change (the event mask) to the button's actions and listeners
    bool rva006E3710(AptCIH *cih); // 0x006E3710 (the map files it under the placeholder class Rva006E3710; retail calls it on the global pool): true when the button is not selectable
    void rva006E47C0(); // 0x006E47C0 (placeholder class Rva006E34D0 in the map): called by Apt.cpp 0x006CD330
    // 0x006E0F00 / 0x006E0F10 / 0x006E0F20 / 0x006E0F30 (the map files them under the placeholder class Rva006E0F00Owner): forward to the
    // action queue held at +0xA0 (mpActionQueue): append an action, put one in front, append / put in front a three-value action.
    void enqueueAction(void *pAction, AptCIH *pCIH, int nArgument0, int nArgument1);
    void prependAction(void *pAction, AptCIH *pCIH, int nArgument0, int nArgument1);
    void enqueueValues(AptValue *pContext, AptValue *pValue1, AptValue *pValue2, int nArgument0, int nArgument1);
    void enqueueValuesFront(AptValue *pContext, AptValue *pValue1, AptValue *pValue2, int nArgument0, int nArgument1);

    AptCIH **maNewInsts;  // 0x00: the instances created since the last tick (_tickNewInsts 0x006E4390 starts the new buttons and sprites, then releases and forgets them)
    int mnNewInsts;       // 0x04
    AptSet mSet08;        // 0x08: PreDestroy 0x006E3FF0 removes every element from it first (the name is not evidenced)
    int mnBIL;             // 0x10: number of BIL entries
    AptBILItem *mpBIL;     // 0x14
    AptSet mKeyListeners;   // 0x18: Key.addListener/removeListener 0x006EA560/0x006EA5C0 (name is this header's)
    AptSet mMouseListeners; // 0x20: Mouse.addListener/removeListener 0x006EA630/0x006EA690
    AptSet mInputSet;       // 0x28: the pointer-driven elements (clips and buttons) the input dispatch 0x006FB120 walks (name is this header's)
    // 0x30: the root display list (Apt.cpp's clear/lookup entry points pass pool+0x30 to the AptDisplayList and
    // AptDisplayListState members; _AptGetAnimationAtLevel 0x006CC530 reads its state's head).
    AptDisplayList mDisplayList;
    AptIntervalTimer *maIntervalTimers; // 0x34: the 0x20-byte setInterval entries (PreDestroy 0x006E3FF0 walks them)
    int mnIntervalTimers;               // 0x38: the live entries (retail asserts it is zero once PreDestroy has cleared them: "nIntervalTimers == 0")
    int field3C;           // 0x3C: reset to 0 by 0x006CD0B0 (a count; AptInput's 0x006FB910 loops over it with the list at +0x40)
    int *field40;          // 0x40: array of field3C ids that 0x006FB910 hands to 0x006FB860 and then forgets
    AptValue *field44;     // 0x44: tested with isUndefined by 0x006FB910 (its value feeds 0x006F9EF0)
    // 0x48..0x5F: six floats Rva006F9EF0 (0x006F9EF0) reads: four limits (-9999.0f = unset) and two offsets (the donor-less names
    // below are this header's; 0x74/0x78 hold the two integer extents the offsets are subtracted from).
    float field48;         // 0x48: lower limit of the first coordinate
    float field4C;         // 0x4C: lower limit of the second coordinate
    float field50;         // 0x50: upper limit of the first coordinate
    float field54;         // 0x54: upper limit of the second coordinate
    float field58;         // 0x58
    float field5C;         // 0x5C
    AptValue *field60;     // 0x60: the button the pointer went down on (or undefined; rva006FA7E0 0x006FA7E0 moves it)
    AptValue *field64;     // 0x64: the button the pointer is over, as far as the clip events know (or undefined)
    AptValue *field68;     // 0x68: read by copyDisplayName 0x006CD2C0 (tested for isCIH, then its name)
    AptValue *field6C;     // 0x6C: preferred by copyDisplayName 0x006CD2C0 (its own string)
    unsigned char field70; // 0x70: set while the pointer button is held (ProcessButtonState 0x006FAA20 branches on it)
    char reserved71[0x3];  // 0x71..0x73
    int field74;           // 0x74: integer extent (0x006F9EF0 subtracts field58 from it)
    int field78;           // 0x78: integer extent (0x006F9EF0 subtracts field5C from it)
    // 0x7C..0x9B: two recorded pointer events (0x006E4480 keeps the last one of kind 0x1F5 at +0x7C and of kind 0x1F6 at +0x8C)
    int mnEvent1F5X;       // 0x7C
    int mnEvent1F5Y;       // 0x80
    unsigned char mbEvent1F5Flag; // 0x84
    char reserved85[0x7];  // 0x85..0x8B
    int mnEvent1F6X;       // 0x8C
    int mnEvent1F6Y;       // 0x90
    unsigned char mbEvent1F6Flag; // 0x94
    char reserved95[0x7];  // 0x95..0x9B
    AptCIH *field9C;       // 0x9C: the focus target (rva006E3700 0x006E3700 stores it, rva006E3710 0x006E3710 compares it against a CIH's parent chain)
    AptActionQueueC *mpActionQueue; // 0xA0: the deferred actions the pool runs (AptActionQueueC.h)
    int fieldA4;           // 0xA4: read by 0x006F6970 (the map files that getter under the placeholder class Rva006F6970DwordField)
    int mnMaxIntervalTimers; // 0xA8: the size of the timer array (PreDestroy reads it through the global pool; the name is this header's)
    int mnMaxInputEvents;    // 0xAC: the capacity of the input id array at +0x40 (0x006E34D0 refuses to append past it; the name is this header's)
};
// VA 0x00E176D0: the one runtime state object (null until Apt is initialized).
extern AptAnimationPoolData *g_pAptAnimationPoolData;
