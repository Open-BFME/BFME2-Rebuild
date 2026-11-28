#pragma once
#include <stddef.h>
#include "Common/GameType.h"
class AsciiString;
class OptionPreferences;
// LOD settings, presets and hardware benchmark profiles.
// Retail constructor 0x00201EAC locates presets at +0x228;
// newLODPreset 0x0020202B locates five counts at +0x17AC.
enum StaticGameLODLevel { STATIC_GAME_LOD_UNKNOWN=-1, STATIC_GAME_LOD_VERY_LOW, STATIC_GAME_LOD_LOW, STATIC_GAME_LOD_MEDIUM, STATIC_GAME_LOD_HIGH, STATIC_GAME_LOD_ULTRA_HIGH, STATIC_GAME_LOD_CUSTOM, STATIC_GAME_LOD_COUNT };
enum DynamicGameLODLevel { DYNAMIC_GAME_LOD_UNKNOWN=-1, DYNAMIC_GAME_LOD_VERY_LOW, DYNAMIC_GAME_LOD_LOW, DYNAMIC_GAME_LOD_MEDIUM, DYNAMIC_GAME_LOD_HIGH, DYNAMIC_GAME_LOD_VERY_HIGH, DYNAMIC_GAME_LOD_COUNT };
struct LODPresetInfo {
    // Constructor 0x00201D48: 32-byte record. Enum identities await shared definitions.
    int m_cpuType;
    int m_mhz;
    float m_cpuPerfIndex;
    int m_videoType;
    int m_memory;
    int m_unk0x14;
    int m_xResolution;
    int m_yResolution;
};
class GameLODManager {
public:
    void rva00202244(int level, OptionPreferences *out);	// Fills the preferences with the settings of a static detail level (AptOptions 0x0051889D)
    LODPresetInfo *newLODPreset(StaticGameLODLevel);
    const char *getDynamicGameLODLevelName(DynamicGameLODLevel);
    const char *getAudioLODLevelName(int);
    int findAudioLODLevel() const;
    struct BenchProfile { int m_cpuType; int m_mhz; float m_intBenchIndex; float m_floatBenchIndex; float m_memBenchIndex; };
    BenchProfile *newBenchProfile();		// (map class RetailBenchProfileAllocator): the next of sixteen records at +0x1628 (count at +0x17C0), or 0
    int rva00202613(float fps);			// The highest dynamic level whose MinimumFPS is below fps, else 0
    void rva00202633(int level);		// Copies dynamic record `level` into the current settings
    unsigned char get() const;			// 0x002026C7 the byte at +0x17AA (the map spells the class Rva002026C7ByteField)
    bool rva00202739(int level);		// Changes the static level (applies it through rva002022E9)
    bool rva0020276C(int level);		// Changes the dynamic level
    void rva00202790(int level);		// Changes the audio level (0 or 1)
    void rva002022E9(int level);		// Applies a static level (not written here)
    int rva00202B6C(AsciiString name);	// The index of a static level name in the six-entry table at VA 0x00DB95F4, else -1
    int getAudioLODIndex(const AsciiString &name); // OptionPreferences::getAudioLOD 0x002E4767 passes the AudioLOD value
    int getStaticGameLODIndex(AsciiString name); // By value (the caller's copy is released at its end)
    unsigned char m_unk0x0[0x1C8];
    // The five dynamic-level records at +0x1C8, 0x10 bytes each: the table at VA 0x00BE3478 parses MinimumFPS (+0x0),
    // ParticleSkipMask (+0x4), DebrisSkipMask (+0x8) and SlowDeathScale (+0xC) into them.
    struct DynamicInfo { int m_minFPS; int m_particleSkipMask; int m_debrisSkipMask; float m_slowDeathScale; };
    DynamicInfo m_dynamicInfo[5];
    // The two audio-level records at +0x218, 8 bytes each: the table at VA 0x00BE34B8 parses MaximumAmbientStreams (+0x0),
    // AllowDolby (+0x4) and AllowReverb (+0x5) into them (parseAudioLODDefinition 0x00202F8A: the record is +0x218 + index * 8).
    struct AudioInfo { int m_maximumAmbientStreams; Bool m_allowDolby; Bool m_allowReverb; };
    AudioInfo m_audioInfo[2];

    LODPresetInfo m_lodPresets[5][32];
    BenchProfile m_benchProfiles[16];	// +0x1628, the record at count - 1 once newBenchProfile 0x0020200D has incremented the count.
    int m_staticLevel;				// +0x1768 set by rva00202739, which applies the level through rva002022E9 first
    int m_dynamicLevel;				// +0x176C set by rva0020276C, which loads the dynamic record into +0x178C
    int m_audioLevel;				// +0x1770 set by rva00202790 (0 or 1), which tells the audio system
    unsigned char m_unk0x1774[0x1788-0x1774];
    int m_unk0x1788;				// +0x1788 GetLODIndex reads it as a one-based level
    int m_currentMinFPS;			// +0x178C zeroed by rva00202633
    int m_currentParticleSkipMask;	// +0x1790 the level record ParticleSkipMask
    int m_unk0x1794;				// +0x1794 zeroed by rva00202633
    int m_currentDebrisSkipMask;	// +0x1798 the record DebrisSkipMask
    float m_currentSlowDeathScale;	// +0x179C the record SlowDeathScale
    unsigned char m_unk0x17A0[0x17AA-0x17A0];
    unsigned char m_unk0x17AA;		// +0x17AA read by get()
    unsigned char m_unk0x17AB;

    int m_numLevelPresets[5];
    Int m_numBenchProfiles;			// +0x17C0, 0x0020200D compares and increments it.
    Int m_idealDetailLevel;			// +0x17C4, AptOptions warns above this level at 0x00518FEA.
    unsigned char m_unk0x17c8[0x10];
    int m_cpuFreq; // 0x002026A0 compares +0x17D8 with +0x17F0.
    unsigned char m_unk0x17dc[0x14];
    int m_reallyLowMHz;
};
typedef char GameLOD_presets_offset[(offsetof(GameLODManager,m_lodPresets)==0x228)?1:-1];
typedef char GameLOD_counts_offset[(offsetof(GameLODManager,m_numLevelPresets)==0x17ac)?1:-1];
typedef char GameLOD_cpu_offset[(offsetof(GameLODManager,m_cpuFreq)==0x17d8)?1:-1];
typedef char GameLOD_threshold_offset[(offsetof(GameLODManager,m_reallyLowMHz)==0x17f0)?1:-1];

extern GameLODManager *TheGameLODManager;	// VA 0x00DFE144 (the map has no name; OptionsMenu.cpp declares it the same way)

