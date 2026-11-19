#pragma once
#include "Common/AsciiString.h"
class MultiIniFieldParse;
// AudioSettings is the shared54-byte prefix, not the204-byte Miles record.
// Base ctor238AC7 and dtor238B90 install the one-slot table7ED654. The
// platform ctor41589 calls that base, then installs its own table7C2420 and
// adds fields54..203 (MilesAudioSettings.cpp in the WB counterpart).
// Parser table7ED3E0 establishes folder/volume/campness names below. The
// five preferred volumes30 are set by238A61 and consumed by Miles init.
struct AudioSettings {
    AudioSettings();
    virtual ~AudioSettings();
    static void buildFieldParse(MultiIniFieldParse &);
    AsciiString m_soundsFolder; //04
    AsciiString m_musicFolder; //08
    AsciiString m_streamingFolder; //0C
    AsciiString m_ambientStreamFolder; //10
    AsciiString m_soundsExtension; //14
    AsciiString m_musicScriptLibraryName; //18
    // 1C..2C: the five default volumes (sound, voice, music, ambient, movie), five
    // consecutive Real entries of parser table 7ED3E0; OptionPreferences::getVolume
    // 2E4B2D indexes them as [settings + index*4 + 0x1C] when no preference is stored.
    float m_defaultVolumes[5];
    float m_preferredVolumes[5]; //30..40, same channel order
    unsigned int m_voiceMoveToCampMaxCampnessAtStartPoint; //44
    unsigned int m_voiceMoveToCampMinCampnessAtEndPoint; //48
    unsigned int m_minDelayBetweenEnterStateVoice; //4C, duration in frames
    AsciiString m_audioRoot; //50
};
typedef char AudioSettingsSizeCheck[sizeof(AudioSettings)==0x54?1:-1];
