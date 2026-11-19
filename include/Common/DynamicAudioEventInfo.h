#pragma once
#include "Common/AudioEventInfo.h"
#include "Common/BitFlags11.h"
class Xfer;
// SIZE D0: alloc274DD4; constructors433390/433762 call AudioEventInfo,
// construct BitFlags<11> atC4, clear original-name stringC8 and store argCC.
// Table83CB1C has destructor4334BB and audio-name getter4333FE. The latter
// calls4331B3, selecting C8 when override bit0 is set, otherwise base name8.
// Thus old Rva004333C0/Rva004EC166/Rva000B56F0Object are this class.
// name: donor(ZH), independently named xferNoName4331C6 and WB BF0260.
class DynamicAudioEventInfo : public AudioEventInfo {
public:
    DynamicAudioEventInfo(int context);
    DynamicAudioEventInfo(const AudioEventInfo &,int context);
    virtual ~DynamicAudioEventInfo();
    virtual const AsciiString &getAudioName() const;
    void overrideAudioName(const AsciiString &);
    void overrideLoopFlag(bool);
    void overrideVolume(float);
    void overrideMinVolume(float);
    void overrideMinRange(float);
    void overrideMaxRange(float);
    void overridePriority(AudioPriority);
    void xferNoName(Xfer *);
    BitFlags<11> m_overriddenFields;
    AsciiString m_originalName;
    int m_context;
};
typedef char DynamicAudioEventInfoSize[sizeof(DynamicAudioEventInfo)==0xD0?1:-1];
