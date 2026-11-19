#pragma once
// The record AptActionInterpreter::PrepareForExecution (0x00700090) and CleanupAfterExecution (0x00706950) take.
// Every caller in the AptAnimation and AptMovie units builds it the same way on the stack: the CIH the
// script runs against, a zero word, a text label, and the flag word 0x100000. The two retail labels seen are
// "AptImported_Init_Actions" (0x006E46A0, 0x006E6A30) and "AptDoFrameControls" (0x0070F370). Field names are
// this header's: the labels show the third member is a description string; the second is the function a
// queued call or a timer runs (the old decomp's AptRun-Functions and AptTimerFunc setups fill it; the script
// runs of AptAnimation and AptMovie leave it zero).
class AptCIH;
class AptValue;

struct AptActionSetup
{
    AptValue *pContext;      // 0x0 (the CIH a script runs against, or the target a timer's function is called on)
    AptValue *pFunction;     // 0x4 (zero for a script run)
    const char *pDescription; // 0x8
    int nFlags;              // 0xC (0x100000 at every call site)
};
