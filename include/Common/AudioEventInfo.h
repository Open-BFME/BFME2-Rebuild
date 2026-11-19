#pragma once
#include "Common/AtomicRefCounted.h"
#include "Common/AsciiString.h"
#include "Common/AudioEventRef.h"
#include "Common/Vector.h"
#include <stddef.h>
class INI;
enum AudioPriority { AP_LOWEST, AP_LOW, AP_NORMAL, AP_HIGH, AP_CRITICAL };
// Name+weight records: native vectors50/60/70 stride8; parser1DABB6 constructs
// AsciiString and integer weight. Subsounds80 replaces filename with an owned
// info handle (copy1D9975). Names describe roles, not recovered source spelling.
struct AudioSoundWeight { AsciiString name; int weight; };
struct AudioSubsoundWeight
{
    AudioSubsoundWeight() {}
    AudioSubsoundWeight(const AudioSubsoundWeight &);
    AudioSubsoundWeight(const AudioEventInfoRef &, int);
    AudioEventInfoRef info;
    int weight;
};
struct AudioEventChannelVolume { int channel; float volume; };
// Target table7DA270: deleting dtor1DA757, name getter5C4AD5. Constructor
// and newOwnedAudioEventInfo5AC80 prove sizeC4. Seventeen synthesized views
// plus parser table7D9D68 establish the offsets; the table corrects several
// old views' reversed min/max-range names and mislabelled float priorities.
class AudioEventInfo : public AtomicRefCounted
{
public:
    AudioEventInfo();
    virtual ~AudioEventInfo();
    virtual const AsciiString &getAudioName() const { return m_audioName; }
    // Native xfer4331C6 calls these base setters directly (folded bodies).
    void setControl(int); //1D96FF
    void clearControl(int); //1D9709
    void setVolume(float); //fold: stores10
    void setMinVolume(float); //fold: stores1C
    void setMinRange(float); //1D972B
    void setMaxRange(float); //210C91
    void setPriority(AudioPriority); //41FDEE, enum rather than float
    int getVolumeSlider();
    bool isTightlyCoupledSound() const;
    // Native recursive predicates; original method spellings remain unresolved.
    bool rva001D98BD(int type);
    bool rva001D990C(int type);
    bool rva001D9A00();
    static void parseMultisoundDefinition(INI *);
    static void parseSoundsList(INI *,void *,void *,const void *);
    static void parseSubsoundsList(INI *,void *,void *,const void *);
    static void parseVolumeSliderMultiplier(INI *,void *,void *,const void *);
    const _STL::vector<AudioSoundWeight> &getSoundsVector() const;
    const AsciiString &getFilename() const; // 0x001DBA69: lea eax,[ecx+0xC] (the map still spells it Rva001DBA69LeaField::get)
    const _STL::vector<AudioSoundWeight> &getAttackSoundsVector() const;
    const _STL::vector<AudioSoundWeight> &getDecaySoundsVector() const;
    const _STL::vector<AudioSubsoundWeight> &getSubsoundVector() const;

    AsciiString m_audioName, m_filename; //08/0C
    float m_volume, m_volumeShift, m_perFileVolumeShift, m_minVolume; //10..1C
    float m_pitchShiftMin, m_pitchShiftMax, m_perFilePitchShiftMin, m_perFilePitchShiftMax; //20..2C
    float m_playPercent; //30
    int m_delay, m_delayMax, m_limit, m_lastPlayedIndex; //34..40
    int m_priority; //44
    unsigned int m_type, m_flags; //48/4C, donor SoundType/AudioControl
    _STL::vector<AudioSoundWeight> m_sounds; //50
    unsigned int m_soundsTotalWeight; //5C
    _STL::vector<AudioSoundWeight> m_attackSounds; //60
    unsigned int m_attackTotalWeight; //6C
    _STL::vector<AudioSoundWeight> m_decaySounds; //70
    unsigned int m_decayTotalWeight; //7C
    _STL::vector<AudioSubsoundWeight> m_subsounds; //80
    unsigned int m_subsoundsTotalWeight; //8C
    float m_lowPassCutoff, m_minRange, m_maxRange; //90..98
    float m_zoomedInOffscreenVolumePercent, m_zoomedInOffscreenMinVolumePercent, m_zoomedInOffscreenOcclusionPercent; //9C..A4
    float m_reverbEffectLevel, m_dryLevel; //A8/AC
    int m_soundType, m_volumeSlider; //B0/B4
    _STL::vector<AudioEventChannelVolume> m_channelVolumes; //B8
};
typedef char AudioEventInfoSize[sizeof(AudioEventInfo)==0xC4?1:-1];
typedef char AudioEventInfoSoundsOffset[offsetof(AudioEventInfo,m_sounds)==0x50?1:-1];
typedef char AudioEventInfoTypeOffset[offsetof(AudioEventInfo,m_soundType)==0xB0?1:-1];