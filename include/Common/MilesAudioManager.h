// MilesAudioManager: the Miles Sound System implementation of AudioManager (Zero Hour's
// GameEngineDevice/Include/MilesAudioDevice/MilesAudioManager.h names the class; BFME 2's
// design differs, so the Zero Hour header is a naming hint only).
// Target facts (retail game.dat):
// - Win32GameEngine::createAudioManager 0x0005E959 allocates 0xC08 bytes and runs the
//   constructor 0x0005CF5F. SIZE 0xC08.
// - The constructor calls AudioManager's constructor 0x0005107D, then stores the primary
//   table VA 0x00BC55B0 at +0 and the Snapshot table VA 0x00BC559C at +0xC; the destructor
//   0x00060FE2 stores the same two tables. GetSnapshotName 0x000518B2 returns
//   "MilesAudioManager".
// - Primary table RVA 0x007C55B0 has 111 slots. Slots 0..13 are SubsystemInterface's
//   (overridden: 0 the deleting destructor 0x00061AA1, 1 init 0x00061ABD, 4 0x00061BD2,
//   5 0x00051155, 9 reset 0x00051173, 10 update 0x00062690; the rest are the base's own
//   bodies). Slots 14..104 are AudioManager's pure slots, all overridden here (no
//   __purecall left in the table), so their names and signatures are AudioManager's.
//   Slots 105..110 are new to this class; their original names are not recovered.
// - Snapshot table RVA 0x007C559C: 0 the deleting-destructor thunk 0x0005D41D,
//   1 loadPostProcess 0x000587C6, 2 GetSnapshotName 0x000518B2, 3 xfer 0x0005E3E5.
// Fields: only those the out-of-line accessors below read or write are declared; the
// other regions keep their constructor evidence in comments as typed padding. The
// constructor zeroes three per-view slots at +0x6B4, +0x6C0 and +0xB3C (a counted loop of
// 3) and builds three per-view records of 0x1C4 bytes at +0x12C, which update 0x00062690
// indexes by the focused view at +0x678.
#pragma once
#ifndef BFME2_MILESAUDIOMANAGER_H
#define BFME2_MILESAUDIOMANAGER_H

#include "Common/GameAudio.h"

class ScreenToTerrainSet;

class MilesAudioManager : public AudioManager
{
public:
	MilesAudioManager();														// 0x0005CF5F
	virtual ~MilesAudioManager();												// slot 0, 0x00061AA1; destructor 0x00060FE2

	// SubsystemInterface
	virtual void init();														// slot 1, 0x00061ABD
	virtual Bool attemptReload(Bool *needsRestart);								// slot 4, 0x00061BD2 (ignores its argument, ret 4)
	virtual Bool attemptRefresh();												// slot 5, 0x00051155
	virtual void reset();														// slot 9, 0x00051173
	virtual void update();														// slot 10, 0x00062690

	// AudioManager
	virtual void resetForViews(unsigned int viewMask);							// slot 14, 0x00061C87
	virtual void stopAudio(int affect);											// slot 15, 0x00054284
	virtual void pauseAudio(unsigned int affect, unsigned int views, int flags);	// slot 16, 0x000583BA
	virtual void resumeAudio(unsigned int affect, unsigned int views, int keepMasks);	// slot 17, 0x00058591
	virtual void muteGlobalAudio();												// slot 18, 0x0005CAE2
	virtual void unmuteGlobalAudio();											// slot 19, 0x0005CB39
	virtual void setViewFocus(int viewType);									// slot 20, 0x0005DAFC
	virtual int getViewFocus() const;											// slot 21, 0x0005189D
	virtual void invalidateListenerPosition();									// slot 22, 0x0005116B
	virtual void unknownAudioSlot23();											// slot 23, 0x0005DACF (the same body as slot 24; signature unresolved in the base)
	virtual AudioHandle addPooledAudioEvent(const PooledAudioEventRTSRef &event);	// slot 24, 0x0005DACF
	virtual AudioHandle addAudioEvent(const AudioEventRTS *event);				// slot 25, 0x0005DABB
	virtual void removePlayingAudio(const AsciiString &name, int views);			// slot 26, 0x000609A1
	virtual void removeAudioEvent(AudioHandle handle);							// slot 27, 0x00057787
	virtual bool killAudioEventImmediately(unsigned int handle);				// slot 28, 0x000615DA
	virtual void resumeAudioEvent(AudioEventRTS *event, int handle);			// slot 29, 0x0005DAE6
	virtual void retainAudioEvent(unsigned int handle);							// slot 30, 0x00057408
	virtual bool isValidAudioEvent(const AudioEventRTS *event) const;			// slot 31, 0x000527E2
	virtual bool hasMusicTrackCompleted(const AsciiString &name, int loops, int viewType, int musicSystem);	// slot 32, 0x00055E6C
	virtual int pushMusicEvent(AudioEventRTS *event, int transition);			// slot 33, 0x0005A7BE
	virtual void popMusicEvent(int viewType, int musicSystem, int transition, int resume);	// slot 34, 0x00055A58
	virtual void stopMusicEvent(int viewType, int musicSystem, int transition);	// slot 35, 0x00055B40
	virtual void pushMusicSystem(int viewType, int musicSystem, int transition);	// slot 36, 0x000568CE
	virtual void popMusicSystem(int viewType, int musicSystem, int transition, int resume);	// slot 37, 0x000569A8
	virtual void clearMusicSystem(int viewType, int musicSystem, int transition);	// slot 38, 0x000567F4
	virtual int getActiveMusicSystem(int viewType);								// slot 39, 0x000518A4
	virtual AudioHandle allocateNewHandle();									// slot 40, 0x00052917
	virtual bool isMusicPlaying();												// slot 41, 0x0005538C
	virtual AsciiString getMusicTrackName();									// slot 42, 0x00054714
	virtual void nextMusicTrack();												// slot 43, 0x0005BD20
	virtual void prevMusicTrack();												// slot 44, 0x0005BD28
	virtual void enableAudioEvent(const AsciiString &name, int viewType);		// slot 45, 0x0005E322
	virtual void disableAudioEvent(const AsciiString &name, int viewType);		// slot 46, 0x00059DF5
	virtual void enableAllAudioEvents(int viewType);							// slot 47, 0x00059E5C
	virtual void setAudioEventVolumeOverride(const AsciiString &name, float volume, int viewType);	// slot 48, 0x0005CA03
	virtual void removeAudioEventVolumeOverride(const AsciiString &name, int viewType);	// slot 49, 0x0005E377
	virtual void removeAllAudioEventVolumeOverrides(int viewType);				// slot 50, 0x0005CA75
	virtual void getAudioEventInfosForHandle(unsigned int handle, _STL::vector<AudioEventInfoRef> *output);	// slot 51, 0x0005A95B
	virtual bool isCurrentlyPlaying(AudioHandle handle);						// slot 52, 0x000572DC
	virtual void setAudioEventPosition(unsigned int handle, const Coord3D *position);	// slot 53, 0x00057378
	virtual bool getAudioEventPosition(unsigned int handle, Coord3D *position);	// slot 54, 0x00057492
	virtual void setAudioEventVolume(unsigned int handle, float volume, int mode);	// slot 55, 0x00057530
	virtual bool isOn(int affect) const;										// slot 56, 0x0005164F
	virtual void setOn(bool on, int affect);									// slot 57, 0x0005168C
	virtual void setGlobalSystemVolume(int volumeType, float volume);			// slot 58, 0x0005CB90
	virtual void setVolumeForView(float volume, int affect, int viewType);		// slot 59, 0x00053CE1
	virtual void setVolume(float volume, int views);							// slot 60, 0x0005C8BB
	virtual void setVolumeEnvelope(float start, float end, float phaseA, float phaseB, float phaseC, int viewType);	// slot 61, 0x000537F4
	virtual void excludeAudioEventFromVolumeEnvelope(const AsciiString &name, int viewType);	// slot 62, 0x00059A25
	virtual void includeAudioEventInVolumeEnvelope(const AsciiString &name, int viewType);	// slot 63, 0x0005E168
	virtual void clearVolumeEnvelopeExclusions(int viewType);					// slot 64, 0x00059A7C
	virtual void beginDeferredAudioStarts();									// slot 65, 0x000515AB
	virtual void endDeferredAudioStarts(int skipFade);							// slot 66, 0x000515B3
	virtual void toggleForcedHeadphones();										// slot 67, 0x000533CB
	virtual bool isHeadphonesForced() const;									// slot 68, 0x000515A4
	virtual void setMovieMixaheadLatency();										// slot 69, 0x000517F1
	virtual void restoreMixaheadLatency();										// slot 70, 0x0005181B
	virtual void serviceAudio();												// slot 71, 0x00051840 (a jump to AIL_serve)
	virtual const Coord3D &getListenerPosition() const;							// slot 72, folded 0x0036CBE7 (returns this + 0x18)
	virtual AudioEventInfoRef newOwnedAudioEventInfo(const AsciiString &name, int context);	// slot 73, 0x0005AC80
	virtual void addUnownedAudioEventInfo(AudioEventInfo *info);				// slot 74, 0x0005ADA3
	virtual AudioEventInfoRef findAudioEventInfo(const AsciiString &name) const;	// slot 75, 0x00059EAE
	virtual void friend_onAudioEventInfoDestruction(AudioEventInfo *info);		// slot 76, 0x0005AF92
	virtual const AudioSettings *getAudioSettings() const;						// slot 77, folded 0x001DB09D (returns +0x10)
	virtual const MiscAudio *getMiscAudio() const;								// slot 78, folded 0x0057E556 (returns +0x14)
	virtual MiscAudio *friend_getMiscAudio();									// slot 79, folded 0x0057E556
	virtual float getAudioLengthMS(const AudioEventRTS *event);					// slot 80, 0x000549A3
	virtual int getMixaheadLatency();											// slot 81, 0x00051846
	virtual void getAudioCacheCounters(int *first, int *second, int *third);	// slot 82, 0x0005185E
	virtual bool isMusicAlreadyLoaded() const;									// slot 83, 0x0005B017
	virtual bool isMusicPlayingFromCD() const;									// slot 84, 0x00051896
	virtual void findAllAudioEventsOfType(int type, _STL::vector<AudioEventInfoRef> *output);	// slot 85, 0x0005AE2B
	virtual void findAllAudioEventsContainingType(int type, _STL::vector<AudioEventInfoRef> *output);	// slot 86, 0x0005AEDD
	virtual const AudioEventInfoHash &getAllAudioEvents() const;				// slot 87, folded 0x0055D96F (returns +0xBC)
	virtual void xferAudioHandle(Xfer *xfer, AudioHandle *handle);				// slot 88, 0x000596E9
	virtual void setReverbRoomType(const AsciiString &name);					// slot 89, 0x0005522C
	virtual AsciiString getReverbRoomName() const;								// slot 90, 0x00054243
	virtual void clearReverbRoomType();											// slot 91, 0x000530DF
	virtual const _STL::vector<const char *> &getReverbRoomNames();				// slot 92, 0x00058314
	virtual void setAudioVolumeForArea(PolygonTrigger *area, float volume);		// slot 93, 0x0005824D
	virtual void removeAudioVolumeForArea(PolygonTrigger *area);				// slot 94, 0x000551C2
	virtual void clearAudioVolumeAreas();										// slot 95, 0x0005710F
	virtual void onPolygonTriggerDeleted(PolygonTrigger *);						// slot 96, 0x000512BC
	virtual bool setUseEAX(bool enable);										// slot 97, 0x000619D1
	virtual void onAudioLODChanged();											// slot 98, 0x000607BB
	virtual bool hasActiveSubtitles();											// slot 99, 0x000531F6
	virtual void destroySubtitleWindow();										// slot 100, 0x000514BB
	virtual void lockAudioDevice();												// slot 101, 0x00053281
	virtual void unlockAudioDevice();											// slot 102, 0x0005329C
	virtual void friend_xferSubAudioEventRTS(Xfer *, PooledAudioEventRTSRef *);	// slot 103, 0x0005987D
	virtual void unknownAudioSlot104();											// slot 104, folded 0x000B3FD0 (an empty body; signature unresolved in the base)

	// Slots new to MilesAudioManager. Original names are not recovered; the
	// spellings follow docs/naming.md (table offset) except getScreenToTerrainPositions,
	// the ledger's name for its target.
	virtual void vfunc_0x1A4(Coord3D *position);								// slot 105, 0x000511C3: fills *position from a source the focused view (+0x678) selects; zero for other views
	virtual void getScreenToTerrainPositions(ScreenToTerrainSet *positions);	// slot 106, 0x0005294A
	virtual Bool vfunc_0x1AC(const Coord3D *position);							// slot 107, 0x000516EF
	virtual Bool vfunc_0x1B0(AudioEventRTS *event);								// slot 108, 0x000527FB
	virtual Real vfunc_0x1B4();													// slot 109, 0x0005121F; update 0x00062690 stores its result at +0x90
	virtual void vfunc_0x1B8();													// slot 110, 0x000541DB: reads audio INI data through three loaders (0x0005407E, 0x000540A7, 0x00054120)

	// Snapshot (secondary table at +0xC); access follows the ledger's decorated names.
	virtual void xfer(Xfer *xfer);												// slot 3, 0x0005E3E5
protected:
	virtual void loadPostProcess();												// slot 1, 0x000587C6
	virtual const char *GetSnapshotName() const;								// slot 2, 0x000518B2

public:
	// Constructor evidence for the padding: +0x10 settings pointer (new 0x204, platform
	// AudioSettings ctor 0x00041589), +0x14 misc pointer (new 0xF4, ctor 0x001ED70C),
	// +0x18..+0x3B nine floats (the listener position at +0x18; 1.0f at +0x28),
	// +0x3C a ScreenToTerrainSet (ctor 0x00052751), +0x8C..+0x97 three scalars, hash tables
	// at +0x9C, +0xBC, +0x104 and +0x118 (shared ctor 0x0005B505), a handle counter at +0xD0
	// (5), and the three per-view records at +0x12C.
	UnsignedByte m_unk0x10[0x678 - 0x10];
	Int m_viewFocus;															// +0x678 the focused view (getViewFocus; setViewFocus compares and stores it); 2 after construction
	UnsignedByte m_unk0x67C[0x69E - 0x67C];										// +0x67C..+0x69B zeroed, +0x69C a word set to 2
	// The isOn/setOn flags, true after construction. isOn 0x0005164F and setOn 0x0005168C map
	// affect bit 1 to +0x6A1, 2 to +0x69F, 4 to +0x6A0, 8 (isOn: any other value) to +0x69E
	// and 0x10 to +0x6A2. Zero Hour's AudioAffect gives music 1, sound 2, sound3D 4 and
	// speech 8, and stores its flags in the same order (speech, sound, sound3D, music).
	Bool m_speechOn;															// +0x69E
	Bool m_soundOn;																// +0x69F
	Bool m_sound3DOn;															// +0x6A0
	Bool m_musicOn;																// +0x6A1
	Bool m_unk0x6A2;															// +0x6A2 affect bit 0x10 (no Zero Hour counterpart)
	Bool m_musicPlayingFromCD;													// +0x6A3 isMusicPlayingFromCD; false after construction (Zero Hour: m_musicPlayingFromCD)
	UnsignedByte m_unk0x6A4[0x6A6 - 0x6A4];										// +0x6A5 set by setViewFocus, cleared by update
	Bool m_headphonesForced;													// +0x6A6 isHeadphonesForced; toggleForcedHeadphones 0x000533CB flips it
	UnsignedByte m_unk0x6A7[0x6AB - 0x6A7];										// +0x6A8..+0x6AA cleared by update
	Bool m_listenerPositionInvalid;												// +0x6AB invalidateListenerPosition sets it; update recalculates the microphone (0x00052B53) while it is set; true after construction
	Bool m_deferAudioStarts;													// +0x6AC beginDeferredAudioStarts sets it, endDeferredAudioStarts clears it
	Int m_skipDeferredFade;														// +0x6B0 the argument of endDeferredAudioStarts
	UnsignedByte m_unk0x6B4[0xB3C - 0x6B4];										// +0x6B4 and +0x6C0 three Ints each, +0x6CC 0x40 records of 0xC bytes, +0x9D4 the lock setViewFocus and setOn hold, +0x9E8 a hash table
	Int m_activeMusicSystem[3];													// +0xB3C one per view (getActiveMusicSystem), zeroed by the constructor
	UnsignedByte m_unk0xB48[0xC08 - 0xB48];										// +0xB48 three counted info handles, +0xB54 the area-volume vector, +0xB8C the file cache pointer, +0xB90 the device reference, +0xBE4 the reverb room, +0xBE8 the subtitle window
};
typedef char MilesAudioManagerSizeCheck[sizeof(MilesAudioManager) == 0xC08 ? 1 : -1];
typedef char MilesAudioManagerViewFocusCheck[offsetof(MilesAudioManager, m_viewFocus) == 0x678 ? 1 : -1];
typedef char MilesAudioManagerFlagsCheck[offsetof(MilesAudioManager, m_skipDeferredFade) == 0x6B0 ? 1 : -1];
typedef char MilesAudioManagerMusicSystemCheck[offsetof(MilesAudioManager, m_activeMusicSystem) == 0xB3C ? 1 : -1];

#endif
