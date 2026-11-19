// Portions derived from Command & Conquer Generals and Command & Conquer Generals Zero Hour, (c) Electronic Arts Inc., GPL-3.0-or-later; see LICENSE and NOTICE.
// GameLOD.h: the game LOD manager and its records (Zero Hour's Common/GameLOD.h; the retail class is the same idea with
// six static levels, an audio LOD table and a few extra fields). Retail evidence, all game.dat 1.06:
//  - GameLODManager::GameLODManager 0x00201EAC stores no vtable pointer (the class has no virtual functions) and
//    builds its record arrays with vector-constructor loops: 6 x 0x4C at +0x000, 5 x 0x10 at +0x1C8, 2 x 8 at +0x218,
//    160 x 0x20 at +0x228 and 16 x 0x14 at +0x1628.
//  - The singleton is allocated with operator new(0x17F8) (0x0022E551): the last member ends at +0x17F4 and the class
//    is 8-aligned because of m_numRAM, so the size rounds up to 0x17F8.
//  - The INI field tables for the static records (VA 0x00BE3308, 23 rows) and the dynamic and audio records
//    (VA 0x00BE3478 and 0x00BE34B8) give each record's field offsets.
#pragma once
#include <stddef.h>
#include "Common/AsciiString.h"

class OptionPreferences;

#define MAX_LOD_PRESETS_PER_LEVEL	32	// 0x00202039 (newLODPreset): a level takes at most 32 presets
#define MAX_BENCH_PROFILES			16	// 0x00202015 (newBenchProfile): at most 16 profiles

// The level names are VeryLow, Low, Medium, High, UltraHigh, Custom (VA 0x00DB95F4, StaticGameLODNames). A level's
// index is its position in that list; the preset table has one row per level except Custom (0x00202B6C accepts 0..5,
// 0x00202739 tests 5 for Custom).
enum StaticGameLODLevel
{
	STATIC_GAME_LOD_UNKNOWN = -1,
	STATIC_GAME_LOD_VERY_LOW,
	STATIC_GAME_LOD_LOW,
	STATIC_GAME_LOD_MEDIUM,
	STATIC_GAME_LOD_HIGH,
	STATIC_GAME_LOD_ULTRA_HIGH,
	STATIC_GAME_LOD_CUSTOM,		// the level the player's own settings fill (index 5, 0x00202058)
	STATIC_GAME_LOD_COUNT
};

// VeryLow, Low, Medium, High, VeryHigh (VA 0x00DB969C, DynamicGameLODNames).
enum DynamicGameLODLevel
{
	DYNAMIC_GAME_LOD_UNKNOWN = -1,
	DYNAMIC_GAME_LOD_VERY_LOW,
	DYNAMIC_GAME_LOD_LOW,
	DYNAMIC_GAME_LOD_MEDIUM,
	DYNAMIC_GAME_LOD_HIGH,
	DYNAMIC_GAME_LOD_VERY_HIGH,
	DYNAMIC_GAME_LOD_COUNT
};

// Zero Hour's static LOD record, with BFME 2's fields. SIZE 0x4C. The offsets are the field table at VA 0x00BE3308
// (token, parser, offset); applyStaticLODLevel 0x002022E9 reads them with `level * 0x4C`. The defaults are what the
// element constructor 0x00201DC4 stores. The two priority fields are ParticlePriorityType (their table VA 0x00C1B6A4
// is the priority name list and 0x0030603A transfers the type as an enum); no definition of that enum is in the tree
// yet, so they stay int.
struct StaticGameLODInfo
{
	int m_modelLOD;							// +0x00 ModelLOD (default 2)
	int m_effectsLOD;						// +0x04 EffectsLOD (3)
	int m_maxParticleCount;					// +0x08 MaxParticleCount (2500)
	bool m_useShadowVolumes;				// +0x0C UseShadowVolumes (true)
	bool m_useShadowDecals;					// +0x0D UseShadowDecals (true)
	bool m_useShadowMapping;				// +0x0E UseShadowMapping (false)
	int m_shadowLOD;						// +0x10 ShadowLOD (3)
	bool m_useTerrainNormalMap;				// +0x14 UseTerrainNormalMap (true)
	bool m_useDistanceDependantTerrainTextures;	// +0x15 UseDistanceDependantTerrainTextures (true)
	int m_waterLOD;							// +0x18 WaterLOD (2)
	bool m_showSoftWaterEdge;				// +0x1C ShowSoftWaterEdge (true)
	int m_maxTankTrackEdges;				// +0x20 MaxTankTrackEdges (100)
	int m_maxTankTrackOpaqueEdges;			// +0x24 MaxTankTrackOpaqueEdges (25)
	int m_maxTankTrackFadeDelay;			// +0x28 MaxTankTrackFadeDelay (300000)
	int m_textureReductionFactor;			// +0x2C TextureReductionFactor (0)
	bool m_showProps;						// +0x30 ShowProps (true)
	int m_animationDetail;					// +0x34 AnimationDetail (3)
	int m_shaderLOD;						// +0x38 ShaderLOD (0)
	bool m_shaderMaterialReplacement;		// +0x3C ShaderMaterialReplacement (false)
	bool m_useHeatEffects;					// +0x3D UseHeatEffects (false)
	int m_decalLOD;							// +0x40 DecalLOD (2)
	int m_minParticlePriority;				// +0x44 MinParticlePriority (1)
	int m_minParticleSkipPriority;			// +0x48 MinParticleSkipPriority (1)
};

// SIZE 0x10. Field table VA 0x00BE3478; element constructor 0x00201E2B (the three words zero, SlowDeathScale 1.0).
struct DynamicGameLODInfo
{
	int m_minFPS;				// +0x0 MinimumFPS
	int m_particleSkipMask;		// +0x4 ParticleSkipMask
	int m_debrisSkipMask;		// +0x8 DebrisSkipMask
	float m_slowDeathScale;		// +0xC SlowDeathScale
};

// SIZE 0x8. Field table VA 0x00BE34B8; element constructor 0x00201E45 (2 streams, Dolby and reverb on).
struct AudioLODInfo
{
	int m_maximumAmbientStreams;	// +0x0 MaximumAmbientStreams
	bool m_allowDolby;				// +0x4 AllowDolby
	bool m_allowReverb;				// +0x5 AllowReverb
};

// SIZE 0x20. One hardware preset (INI LODPreset). Constructor 0x00201D48: cpu 0, MHz 1, perf index 1.0, video 0,
// memory 1, +0x14 1, resolution 800 x 600. parseLODPreset 0x00202E23 fills the first seven fields in the order
// cpu type, MHz, video chipset, +0x14, memory, width, height; findStaticLODLevel 0x00203288 compares the CPU type,
// MHz, chipset (+0xC) and memory (+0x10) and takes the resolution from +0x18/+0x1C. m_cpuType indexes CPUNames
// (XX, P3, P4, K7: VA 0x00DB96B0) and m_videoType indexes VideoNames (VA 0x00DB96D0).
struct LODPresetInfo
{
	int m_cpuType;			// +0x00
	int m_mhz;				// +0x04
	float m_cpuPerfIndex;	// +0x08 (the cpu preset for unidentified processors in Zero Hour)
	int m_videoType;		// +0x0C
	int m_memory;			// +0x10 megabytes of memory the preset wants
	int m_unk0x14;			// +0x14 parsed between the chipset and the memory; role unconfirmed
	int m_xResolution;		// +0x18
	int m_yResolution;		// +0x1C
};

class GameLODManager
{
public:
	// SIZE 0x14. Constructor 0x00201D79: cpu 0, MHz 1, the three indices 1.0. INI::parseBenchProfile 0x002026D4 fills it.
	// The map spells it GameLODManager::BenchProfile (newBenchProfile's return type).
	struct BenchProfile
	{
		int m_cpuType;				// +0x00
		int m_mhz;					// +0x04
		float m_intBenchIndex;		// +0x08
		float m_floatBenchIndex;	// +0x0C
		float m_memBenchIndex;		// +0x10
	};

	// The map's spellings for the placeholder rows are kept; the notes name the Zero Hour method each one matches.
	void rva00202058(OptionPreferences *preferences);	// 0x00202058 writes the Custom record from the player's option preferences (Zero Hour refreshCustomStaticLODLevel)
	void rva00202244(int level, OptionPreferences *out);	// 0x00202244: fills the preferences with the settings of a static detail level (AptOptions 0x0051889D)
	void rva002022E9(int level);			// 0x002022E9 applies a static level to the globals and to the m_current*LOD copies (Zero Hour applyStaticLODLevel)
	void init(int);							// 0x002027EA (the map's row; Zero Hour init loads the preset INI files)
	StaticGameLODLevel findStaticLODLevel();	// 0x00203288 picks the ideal level for this machine; caches it in m_idealDetailLevel
	LODPresetInfo *newLODPreset(StaticGameLODLevel);	// 0x0020202B the next of 32 presets for the level, or 0
	BenchProfile *newBenchProfile();		// 0x0020200D the next of sixteen records at +0x1628, or 0
	const char *getDynamicGameLODLevelName(DynamicGameLODLevel);	// 0x00202605
	const char *getAudioLODLevelName(int);	// 0x00202678
	int findAudioLODLevel() const;			// 0x002026A0 1 when m_cpuFreq reaches m_audioLowMHz
	int rva00202613(float fps);				// 0x00202613 the highest dynamic level whose MinimumFPS is below fps, else 0 (Zero Hour findDynamicLODLevel)
	void rva00202633(int level);			// 0x00202633 loads dynamic record `level` into the current settings (Zero Hour applyDynamicLODLevel)
	unsigned char get() const;				// 0x002026C7 the byte at +0x17AA, m_memPassed (Zero Hour didMemPass)
	bool rva00202739(int level);			// 0x00202739 changes the static level (Zero Hour setStaticLODLevel)
	bool rva0020276C(int level);			// 0x0020276C changes the dynamic level (Zero Hour setDynamicLODLevel)
	void rva00202790(int level);			// 0x00202790 changes the audio level (0 or 1)
	int rva00202B6C(AsciiString name);		// 0x00202B6C the index of a static level name in the six-entry table at VA 0x00DB95F4, else -1
	int getStaticGameLODIndex(AsciiString name);	// 0x00202C30, by value (the caller's copy is released at its end); the body scans the dynamic names
	int getAudioLODIndex(const AsciiString &name);	// 0x00202C76; OptionPreferences::getAudioLOD 0x002E4767 passes the AudioLOD value

	StaticGameLODInfo m_staticGameLODInfo[STATIC_GAME_LOD_COUNT];			// +0x000 the field table at VA 0x00BE3308 parses into one record per level
	DynamicGameLODInfo m_dynamicGameLODInfo[DYNAMIC_GAME_LOD_COUNT];		// +0x1C8 parseDynamicGameLODDefinition's record is +0x1C8 + index * 0x10 (0x00202F05)
	AudioLODInfo m_audioLODInfo[2];											// +0x218 parseAudioLODDefinition 0x00202F8A: +0x218 + index * 8
	LODPresetInfo m_lodPresets[STATIC_GAME_LOD_CUSTOM][MAX_LOD_PRESETS_PER_LEVEL];	// +0x228 newLODPreset 0x0020202B
	BenchProfile m_benchProfiles[MAX_BENCH_PROFILES];						// +0x1628 newBenchProfile 0x0020200D

	// The current levels. The constructor 0x00201EAC starts the static and audio level at -1 and the dynamic level at 3.
	int m_currentStaticLOD;			// +0x1768 set by rva00202739, which applies the level through rva002022E9 first
	int m_currentDynamicLOD;		// +0x176C set by rva0020276C, which loads the dynamic record through rva00202633
	int m_currentAudioLOD;			// +0x1770 set by rva00202790 (0 or 1), which tells the audio system
	// rva002022E9 copies these from the current static record (the field and its source offset): ModelLOD +0x00,
	// EffectsLOD +0x04, ShadowLOD +0x10, WaterLOD +0x18, AnimationDetail +0x34; the terrain value is derived from
	// UseTerrainNormalMap (2) / UseDistanceDependantTerrainTextures (1) / neither (0).
	int m_currentModelLOD;			// +0x1774 (0x002022E9, default 2)
	int m_currentEffectsLOD;		// +0x1778 (default 3)
	int m_currentShadowLOD;			// +0x177C (default 3)
	int m_currentTerrainLOD;		// +0x1780 (default 2)
	int m_currentWaterLOD;			// +0x1784 (default 2)
	int m_currentAnimationLOD;		// +0x1788 (default 3)
	// The dynamic settings rva00202633 loads (the order Zero Hour's applyDynamicLODLevel assigns them).
	int m_numParticleGenerations;		// +0x178C zeroed by rva00202633
	int m_dynamicParticleSkipMask;		// +0x1790 the level record's ParticleSkipMask
	int m_numDebrisGenerations;			// +0x1794 zeroed by rva00202633
	int m_dynamicDebrisSkipMask;		// +0x1798 the level record's DebrisSkipMask
	float m_slowDeathScale;				// +0x179C the level record's SlowDeathScale (constructor default 1.0)
	int m_minParticlePriority;			// +0x17A0 rva002022E9 copies it from static record +0x44 (ParticlePriorityType)
	int m_minParticleSkipPriority;		// +0x17A4 rva002022E9 copies it from static record +0x48
	bool m_videoPassed;					// +0x17A8 cleared by the constructor
	bool m_cpuPassed;					// +0x17A9
	unsigned char m_memPassed;			// +0x17AA read by get(); a Bool in Zero Hour, kept a byte because the map's accessor returns the byte
	int m_numLevelPresets[STATIC_GAME_LOD_CUSTOM];	// +0x17AC newLODPreset 0x0020202B
	int m_numBenchProfiles;				// +0x17C0 newBenchProfile 0x0020200D
	int m_idealDetailLevel;				// +0x17C4 -1 until findStaticLODLevel runs; AptOptions::rva00518FEA 0x00518FEA warns above it
	int m_videoChipType;				// +0x17C8 filled through 0x004768F5 (its first parameter is a ChipsetType *); findStaticLODLevel compares it with the preset's chipset
	int m_cpuType;						// +0x17CC compared with the preset's cpu type
	unsigned __int64 m_numRAM;			// +0x17D0 bytes: findStaticLODLevel shifts the pair at +0x17D0/+0x17D4 right by 20 (shrd) to get megabytes
	int m_cpuFreq;						// +0x17D8 MHz
	float m_intBenchIndex;				// +0x17DC the bench indices; findStaticLODLevel and init 0x002027EA read and write them (Zero Hour's four bench members, in its order)
	float m_floatBenchIndex;			// +0x17E0
	float m_memBenchIndex;				// +0x17E4
	float m_compositeBenchIndex;		// +0x17E8
	int m_reallyLowMHz;					// +0x17EC default 400; the setter 0x00201D9D and INI::parseReallyLowMHz 0x00201E56 store it
	int m_audioLowMHz;					// +0x17F0 default 1500; INI::parseAudioLowMHz 0x00201E81 stores it; findAudioLODLevel 0x002026A0 compares m_cpuFreq with it
};

typedef char GameLOD_staticInfo_size[(sizeof(StaticGameLODInfo) == 0x4C) ? 1 : -1];
typedef char GameLOD_staticInfo_water[(offsetof(StaticGameLODInfo, m_waterLOD) == 0x18) ? 1 : -1];
typedef char GameLOD_staticInfo_decal[(offsetof(StaticGameLODInfo, m_decalLOD) == 0x40) ? 1 : -1];
typedef char GameLOD_dynamicInfo_size[(sizeof(DynamicGameLODInfo) == 0x10) ? 1 : -1];
typedef char GameLOD_audioInfo_size[(sizeof(AudioLODInfo) == 0x8) ? 1 : -1];
typedef char GameLOD_preset_size[(sizeof(LODPresetInfo) == 0x20) ? 1 : -1];
typedef char GameLOD_bench_size[(sizeof(GameLODManager::BenchProfile) == 0x14) ? 1 : -1];
typedef char GameLOD_dynamic_offset[(offsetof(GameLODManager, m_dynamicGameLODInfo) == 0x1C8) ? 1 : -1];
typedef char GameLOD_audio_offset[(offsetof(GameLODManager, m_audioLODInfo) == 0x218) ? 1 : -1];
typedef char GameLOD_presets_offset[(offsetof(GameLODManager, m_lodPresets) == 0x228) ? 1 : -1];
typedef char GameLOD_bench_offset[(offsetof(GameLODManager, m_benchProfiles) == 0x1628) ? 1 : -1];
typedef char GameLOD_static_offset[(offsetof(GameLODManager, m_currentStaticLOD) == 0x1768) ? 1 : -1];
typedef char GameLOD_dynamicSettings_offset[(offsetof(GameLODManager, m_numParticleGenerations) == 0x178C) ? 1 : -1];
typedef char GameLOD_counts_offset[(offsetof(GameLODManager, m_numLevelPresets) == 0x17AC) ? 1 : -1];
typedef char GameLOD_ideal_offset[(offsetof(GameLODManager, m_idealDetailLevel) == 0x17C4) ? 1 : -1];
typedef char GameLOD_ram_offset[(offsetof(GameLODManager, m_numRAM) == 0x17D0) ? 1 : -1];
typedef char GameLOD_cpu_offset[(offsetof(GameLODManager, m_cpuFreq) == 0x17D8) ? 1 : -1];
typedef char GameLOD_threshold_offset[(offsetof(GameLODManager, m_audioLowMHz) == 0x17F0) ? 1 : -1];
typedef char GameLOD_size[(sizeof(GameLODManager) == 0x17F8) ? 1 : -1];

extern GameLODManager *TheGameLODManager;	// VA 0x00DFE144 (the map has no name; OptionsMenu.cpp declares it the same way)
