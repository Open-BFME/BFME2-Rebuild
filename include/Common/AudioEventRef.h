#pragma once
#include "Common/AtomicRefCounted.h"
class AudioEventInfo;
class AudioEventRTS;
class PooledAudioEventRTS;
// Counted info reference: twenty compiled views agree on one pointer.
// Constructor51914 retains pointee+4; copyA8C7C and destructor10F149 use
// the same counter. Original spelling is not independently recovered.
class AudioEventInfoRef
{
public:
    AudioEventInfoRef() : m_info(0) {}
    AudioEventInfoRef(const AudioEventInfo *);
    AudioEventInfoRef(const AudioEventInfoRef &);
    ~AudioEventInfoRef() { if (m_info) const_cast<AtomicRefCounted *>(reinterpret_cast<const AtomicRefCounted *>(m_info))->Release_Ref(); }	// inline in retail: ScriptActions::doPlaySoundEffect 0x003BD5E0 releases the null-tested pointee through Release_Ref 0x00050ED3 in place (map row 0x0010F149 is the out-of-line copy)
    AudioEventInfoRef &operator=(const AudioEventInfoRef &);
    const AudioEventInfo *get() const { return m_info; }
    const AudioEventInfo *operator->() const { return m_info; }
private:
    const AudioEventInfo *m_info;
};
// Event holder51931/51950/519AB is one pointer; its referent is AudioEventRTS
// plus the counted base at+88 (pool allocation90). Role-inferred class name.
class PooledAudioEventRTSRef
{
public:
    PooledAudioEventRTSRef() : m_event(0) {}
    PooledAudioEventRTSRef(PooledAudioEventRTS *);
    PooledAudioEventRTSRef(const PooledAudioEventRTSRef &);
    ~PooledAudioEventRTSRef();
    PooledAudioEventRTSRef &operator=(const PooledAudioEventRTSRef &);
    AudioEventRTS *get() const { return m_event; }
private:
    AudioEventRTS *m_event;
};
typedef char AudioEventInfoRefSize[sizeof(AudioEventInfoRef)==4?1:-1];
typedef char PooledAudioEventRTSRefSize[sizeof(PooledAudioEventRTSRef)==4?1:-1];