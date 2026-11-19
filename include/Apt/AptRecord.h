#pragma once
// The two record layouts Apt's input-recording hooks (Apt.cpp 0x006CC740 and 0x006CC820) hand the host's recording
// callback: a frame number followed by a kind. Their sizes are the byte counts the callers pass (8 and 5).
// The member and type names are this header's (nothing recovered names them).
struct AptRecordHeader
{
    int frame;            // 0x0
    unsigned short kind;  // 0x4: 7 for a saved custom event
    unsigned short size;  // 0x6: the byte count of the buffer that follows
};

struct AptRecordMarker
{
    int frame;  // 0x0
    int kind;   // 0x4: 3 for the frame marker; only the low byte is sent (5 bytes in all)
};

// The recording and playback state these hooks use, defined once in Apt.cpp (one cell each; other units read them).
extern int g_nAptPauseState;                                            // VA 0x00E176D4: nonzero while input is paused for playback
extern int g_nAptRecordFlag;                                            // VA 0x00E17708: nonzero records input events
extern void (__cdecl *g_pfnAptRecord)(const void *, unsigned int);      // VA 0x00E17740: the host's recording callback
