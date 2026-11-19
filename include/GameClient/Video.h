#pragma once
#include "Common/AsciiString.h"
// Donor Video.h (BFME1 10af19f44a89ab7ecc23195bb9a842ceafbc02c9).
// Retail insertion and 122-byte copy ctor prove three strings at +0/+4/+8,
// POD at +0xC/+0x10/+0x14/+0x18 and total stride 28 bytes.
class SubtitleManager;
struct Video {
    AsciiString m_filename;
    AsciiString m_internalName;
    AsciiString m_commentForWB;
    unsigned char m_hasSubtitles;
    float m_volume;
    unsigned char m_isDefault;
    SubtitleManager *m_subtitleManager;
    Video() : m_hasSubtitles(0),m_volume(1.0f),m_isDefault(0),m_subtitleManager(0) {}
    ~Video();
    Video &operator=(const Video &); // 0x00689380
};
typedef char Video_size[(sizeof(Video)==28)?1:-1];
