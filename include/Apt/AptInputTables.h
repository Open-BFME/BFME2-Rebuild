#pragma once
// The tables and globals AptInput.cpp reads (nothing in the recovered rows writes them; the names are this header's):
//  * the mouse event table at VA 0x00DDC8EC: six {event mask, string pool index} pairs (AptAnimationPoolData::AddListenerToQueue
//    0x006FAB40 looks the handler up under the string for every event bit that is set);
//  * the action code table at VA 0x00DDC2E8: stride 8, the action code in the first word (read in step with the two tables above);
//  * the key event table at VA 0x00DDC898: seven {event mask, string pool index, key code} entries (rva006FA100 0x006FA100 queues the
//    key handler for every state bit the button's hash listens to);
//  * the default action argument at VA 0x00E17704 and the host sound callback at VA 0x00E17784. That callback is one global,
//    g_pfnAptSound, defined in AptMovie.cpp: the movie's StartSound control and the button state sounds here call it with a
//    sound's +0x08 cell and 0; 0x006F3670 calls it with the +0x24 and +0x28 words of the object its argument resolves to.
struct AptInputEventName
{
    unsigned int mask;
    int stringId;
};

struct AptButtonKeyEvent
{
    unsigned int mask;
    int stringId;
    int keyCode;
};

extern const AptInputEventName g_aptMouseEventNames[6];
extern const AptInputEventName g_aptMouseEventActions[];
extern const AptButtonKeyEvent g_aptButtonKeyEvents[7];
extern int g_nAptInputActionArgument;
extern void (__cdecl *g_pfnAptSound)(void *sound, int flags);
