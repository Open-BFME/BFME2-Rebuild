// AudioManager: BFME2 abstract audio service.
// Target ctor 0x0005107D installs primary table RVA 0x007C5148 and Snapshot
// table 0x007C5138 at +0xC. Miles ctor 0x0005CF5F proves first own field +0x10.
// Primary slots 0..13 belong to SubsystemInterface; 14..104 are pure.
// Secondary table is Snapshot's destructor/loadPostProcess/name/xfer order.
// Sources: old canonical GameAudio.h, MilesAudioManager.cpp and its small-slot
// units; h01 synthesis of all 59 AudioManager views. Slot names below are
// separated from target layout facts. Unknown slots have no recovered ABI;
// do not call them until their declaration is reconciled with target evidence.
// 89 of91 own slots named;2 remain unresolved. Role-inferred spellings are
// explicitly tagged and are not claims of original source spelling.
// Legacy BfmeAudioEventPrefix136 is now an alias of canonical AudioEventRTS.
#pragma once
#ifndef BFME2_GAMEAUDIO_H
#define BFME2_GAMEAUDIO_H
#include "Common/GameType.h"
#include "Common/Vector.h"
#include "Common/StlHashMap.h"
#include "Common/STLTypedefs.h"
#include "Common/RTSEqualTo.h"
#include "Common/AsciiString.h"
#include "Common/AudioEventRTS.h"
#include "Common/Snapshot.h"
#include "Common/SubsystemInterface.h"
class Xfer;
class AudioEventRTS;
class AudioEventInfo;
class AudioEventInfoRef; // counted four-byte info handle; ctor51914/dtor10F149
class PooledAudioEventRTSRef; // role name; four-byte holder pointer, count at pointee+88
struct OpaqueRefElement4;
class Coord3D;
struct AudioSettings;
class MiscAudio;
class PolygonTrigger;
typedef Int AudioHandle;
// ZH's map type; target getter55D96F returns the native table at Miles+BC.
typedef _STL::hash_map<AsciiString,AudioEventInfo *,rts::hash<AsciiString>,rts::equal_to<AsciiString> > AudioEventInfoHash;



class AudioManager : public SubsystemInterface, public Snapshot
{
public:
    AudioManager();
    virtual ~AudioManager();
    virtual void resetForViews(unsigned int viewMask) = 0; // +0x038: name: 61C87 clears requests, music stacks, per-view volumes and overrides; inferred
    virtual void stopAudio(int affect) = 0; // +0x03C: name: donor (ZH); Miles 0x00054284 stops selected audio
    virtual void pauseAudio(unsigned int affect, unsigned int views, int flags) = 0; // +0x040: name: donor (ZH); 0x000583BA sets pause masks, pauses samples/streams
    virtual void resumeAudio(unsigned int affect, unsigned int views, int keepMasks) = 0; // +0x044: name: donor (ZH); 0x00058591 clears the same masks and resumes
    virtual void muteGlobalAudio() = 0; // +0x048: name: 5CAE2 -> 524D6 clears global enable;51ED6 writes zero channel gains; inferred
    virtual void unmuteGlobalAudio() = 0; // +0x04C: name: 5CB39 -> 524E2 sets global enable and recomputes channel gains; inferred
    virtual void setViewFocus(int viewType) = 0; // +0x050: target name: Miles 0x0005DAFC
    virtual int getViewFocus() const = 0; // +0x054: name: inferred; 0x0005189D reads focused view +0x678
    virtual void invalidateListenerPosition() = 0; // +0x058: name: inferred; 5116B sets6AB, update62690 then recalculates microphone and clears it
    virtual void unknownAudioSlot23() = 0; // +0x05C: UNRESOLVED name/signature; position only
    virtual AudioHandle addPooledAudioEvent(const PooledAudioEventRTSRef &event) = 0; // +0x060: role-inferred;5DACF dereferences holder into addOrResumeAudioEvent;56A7AA/5C92EC pass51931/53D26 pooled refs and store EAX via5F69C4
    virtual AudioHandle addAudioEvent(const AudioEventRTS *event) = 0; // +0x064: name: donor (ZH); 0x0005DABB forwards to addOrResumeAudioEvent; callers use EAX
    virtual void removePlayingAudio(const AsciiString &name, int views) = 0; // +0x068: target name: 0x000609A1
    virtual void removeAudioEvent(AudioHandle handle) = 0; // +0x06C: name: donor (ZH); 0x00057787 removes the handle
    virtual bool killAudioEventImmediately(unsigned int handle) = 0; // +0x070: target name: 0x000615DA
    virtual void resumeAudioEvent(AudioEventRTS *event, int handle) = 0; // +0x074: target name: 0x0005DAE6 forwards with resume flag
    virtual void retainAudioEvent(unsigned int handle) = 0; // +0x078: name: 57408 ->562CF increments event stop-hold+80;5FA3C decrements before stopping; inferred
    virtual bool isValidAudioEvent(const AudioEventRTS *event) const = 0; // +0x07C: name: donor (ZH); 0x000527E2 tests the info pointer at event+8
    virtual bool hasMusicTrackCompleted(const AsciiString &name,int loops,int viewType,int musicSystem) = 0; // +0x080: name: donor (ZH);55E6C searches playing/stack streams, A8B59 waits and tests AIL_stream_loop_count via10F11C; wrapper discards predicate result
    virtual int pushMusicEvent(AudioEventRTS *event,int transition) = 0; // +0x084: name: 5A7BE calls named pushMusicEventInternal with append enabled; inferred
    virtual void popMusicEvent(int viewType,int musicSystem,int transition,int resume) = 0; // +0x088: name: 55A58 queues opcode4;57232 processPopMusicRequest removes current and resumes stack; inferred
    virtual void stopMusicEvent(int viewType,int musicSystem,int transition) = 0; // +0x08C: name: 55B40 queues opcode2;57297 removes current or pops inactive stack without resume; inferred
    virtual void pushMusicSystem(int viewType,int musicSystem,int transition) = 0; // +0x090: name: 568CE queues opcode5;5AC61 calls moveUpMusicSystems; inferred
    virtual void popMusicSystem(int viewType,int musicSystem,int transition,int resume) = 0; // +0x094: name: 569A8 queues opcode6;5876E moves down to populated earlier system; inferred
    virtual void clearMusicSystem(int viewType,int musicSystem,int transition) = 0; // +0x098: name: 567F4 queues opcode7;5774F removes current and clears selected stack; inferred
    virtual int getActiveMusicSystem(int viewType) = 0; // +0x09C: name: inferred; 518A4 reads active system array+B3C indexed by view
    virtual AudioHandle allocateNewHandle() = 0; // +0x0A0: name: donor (ZH); 52917 increments handle counter+D0 under mutex
    virtual bool isMusicPlaying() = 0; // +0x0A4: name: donor (ZH); 0x0005538C searches current-system playing and queued streams
    virtual AsciiString getMusicTrackName() = 0; // +0x0A8: name: donor (ZH); 0x00054714 returns current/queued music info name
    virtual void nextMusicTrack() = 0; // +0x0AC: name: donor (ZH); 0x0005BD20 calls track advance 0x0005BBE2 with previous=0
    virtual void prevMusicTrack() = 0; // +0x0B0: name: donor (ZH); 0x0005BD28 calls same with previous=1
    virtual void enableAudioEvent(const AsciiString &name,int viewType) = 0; // +0x0B4: name: inferred; 5E322 erases name from per-view disabled set+A14
    virtual void disableAudioEvent(const AsciiString &name,int viewType) = 0; // +0x0B8: name: inferred; 59DF5 stops name via slot26 then inserts into same disabled set
    virtual void enableAllAudioEvents(int viewType) = 0; // +0x0BC: name: inferred; 59E5C clears that per-view disabled set
    virtual void setAudioEventVolumeOverride(const AsciiString &name,float volume,int viewType) = 0; // +0x0C0: name: donor (ZH); 5CA03 stores name/volume in GlobalVolumeData and updates matching active events
    virtual void removeAudioEventVolumeOverride(const AsciiString &name,int viewType) = 0; // +0x0C4: name: inferred; 5E377 erases name/volume entry and applies sentinel-1 to matching active events
    virtual void removeAllAudioEventVolumeOverrides(int viewType) = 0; // +0x0C8: name: inferred; 5CA75 clears per-view volume overrides and refreshes active events
    virtual void getAudioEventInfosForHandle(unsigned int handle,_STL::vector<AudioEventInfoRef> *output) = 0; // +0x0CC: name inferred;5A95B clears output then5A92A resolves handle and appends event.info+8; retains original handle during alias walk
    virtual bool isCurrentlyPlaying(AudioHandle handle) = 0; // +0x0D0: name: donor (ZH); 0x000572DC tests tracked handle
    virtual void setAudioEventPosition(unsigned int handle,const Coord3D *position) = 0; // +0x0D4: name: inferred; 57378 alias walk reaches562A2 -> 2D9508 twelve-byte position setter
    virtual bool getAudioEventPosition(unsigned int handle,Coord3D *position) = 0; // +0x0D8: name: 57492 alias walk ->562FA copies twelve-byte world point and returns validity; inferred
    virtual void setAudioEventVolume(unsigned int handle,float volume,int mode) = 0; // +0x0DC: name: inferred; 57530 alias walk reaches5634C event/playing volume and fade setter
    virtual bool isOn(int affect) const = 0; // +0x0E0: name: donor (ZH); 0x0005164F reads +0x69E..+0x6A2 affect bits
    virtual void setOn(bool on, int affect) = 0; // +0x0E4: name: donor (ZH); 0x0005168C writes same flags
    virtual void setGlobalSystemVolume(int volumeType,float volume) = 0; // +0x0E8: name: 5CB90 ->524EE updates one of six global slider gains and refreshes; inferred
    virtual void setVolumeForView(float volume,int affect,int viewType) = 0; // +0x0EC: name: inferred; 53CE1 calls GlobalVolumeData::setVolumes for one view
    virtual void setVolume(float volume, int views) = 0; // +0x0F0: name: donor (ZH); 0x0005C8BB updates volume for selected view mask
    virtual void setVolumeEnvelope(float start,float end,float phaseA,float phaseB,float phaseC,int viewType) = 0; // +0x0F4: name: 537F4 ->5230D clamps start/end, stores three phase values, activates scale+9C; inferred; phase meanings unresolved
    virtual void excludeAudioEventFromVolumeEnvelope(const AsciiString &name,int viewType) = 0; // +0x0F8: name: 59A25 inserts name in volume-data set+A0;5910F selects gain without envelope+9C for members; inferred
    virtual void includeAudioEventInVolumeEnvelope(const AsciiString &name,int viewType) = 0; // +0x0FC: name: 5E168 erases same envelope-exclusion name and refreshes; inferred
    virtual void clearVolumeEnvelopeExclusions(int viewType) = 0; // +0x100: name: 59A7C clears same exclusion set and refreshes; inferred
    virtual void beginDeferredAudioStarts() = 0; // +0x104: inferred;515AB sets6AC,535CC defers starting held audio while set
    virtual void endDeferredAudioStarts(int skipFade) = 0; // +0x108: inferred;515B3 clears6AC/sets6B0;535CC resumes held audio, suppresses fade when6B0 nonzero
    virtual void toggleForcedHeadphones() = 0; // +0x10C: name: inferred; 533CB toggles6A6 and calls AIL_set_3D_speaker_type(1) or restoresBF0
    virtual bool isHeadphonesForced() const = 0; // +0x110: name: inferred; 515A4 reads the same6A6 flag
    virtual void setMovieMixaheadLatency() = 0; // +0x114: name: inferred; 517F1 uses AudioSettings.MixaheadLatencyDuringMovies+88 then AIL_serve
    virtual void restoreMixaheadLatency() = 0; // +0x118: name: inferred; 5181B uses AudioSettings.MixaheadLatency+84
    virtual void serviceAudio() = 0; // +0x11C: name: inferred; 51840 tail-calls AIL_serve
    virtual const Coord3D &getListenerPosition() const = 0; // +0x120: name: donor (ZH); folded 0x0036CBE7 returns address of listener at +0x18
    virtual AudioEventInfoRef newOwnedAudioEventInfo(const AsciiString &name,int context) = 0; // +0x124: target name5AC80; four-byte counted return, ctor51914/copyA8C7C/dtor10F149
    virtual void addUnownedAudioEventInfo(AudioEventInfo *info) = 0; // +0x128: target name: 0x0005ADA3
    virtual AudioEventInfoRef findAudioEventInfo(const AsciiString &name) const = 0; // +0x12C: target name59EAE; tableBC lookup returns counted info handle
    virtual void friend_onAudioEventInfoDestruction(AudioEventInfo *info) = 0; // +0x130: target name: 0x0005AF92
    virtual const AudioSettings *getAudioSettings() const = 0; // +0x134: name: donor (ZH); folded 0x001DB09D returns settings pointer +0x10
    virtual const MiscAudio *getMiscAudio() const = 0; // +0x138: name: donor (ZH); folded 0x0057E556 returns misc pointer +0x14
    virtual MiscAudio *friend_getMiscAudio() = 0; // +0x13C: name: donor (ZH); INIMiscAudio 0x001ED6EE calls +0x13C; mutable getter fold
    virtual float getAudioLengthMS(const AudioEventRTS *event) = 0; // +0x140: name: donor (ZH); 0x000549A3
    virtual int getMixaheadLatency() = 0; // +0x144: name: inferred; 51846 multiplies Miles preferences22 and2A, inverse of latency setters
    virtual void getAudioCacheCounters(int *first,int *second,int *third) = 0; // +0x148: name: inferred; 5185E copies AudioFileCache counters38/3C/40; individual meanings unresolved
    virtual bool isMusicAlreadyLoaded() const = 0; // +0x14C: target name: 0x0005B017
    virtual bool isMusicPlayingFromCD() const = 0; // +0x150: name: donor (ZH);51896 reads6A3, constructor false; legacy CD flag
    virtual void findAllAudioEventsOfType(int type,_STL::vector<AudioEventInfoRef> *output) = 0; // +0x154: name: donor (ZH);5AE2B compares info+B0 then appends retained handles
    virtual void findAllAudioEventsContainingType(int type,_STL::vector<AudioEventInfoRef> *output) = 0; // +0x158: name: inferred;5AEDD uses recursive1D98BD including composite child types
    virtual const AudioEventInfoHash &getAllAudioEvents() const = 0; // +0x15C: name: donor (ZH);55D96F returns table+BC populated by addUnownedAudioEventInfo5ADA3
    virtual void xferAudioHandle(Xfer *xfer, AudioHandle *handle) = 0; // +0x160: caller ABI: FlammableUpdate; Miles 0x000596E9
    virtual void setReverbRoomType(const AsciiString &name) = 0; // +0x164: name: 5522C name lookup, storesBE4 and calls internalSetReverbRoomType; inferred
    virtual AsciiString getReverbRoomName() const = 0; // +0x168: name: 54243 AIL_3D_room_type -> environment-name lookup -> AsciiString; inferred
    virtual void clearReverbRoomType() = 0; // +0x16C: name: 530DF clearsBE4 and applies room type0; inferred
    virtual const _STL::vector<const char *> &getReverbRoomNames() = 0; // +0x170: name: 58314 builds all25 selectable environment names and returns static vector; inferred
    virtual void setAudioVolumeForArea(PolygonTrigger *area,float volume) = 0; // +0x174: name: 5824D stores clamped level;55C5D selects minimum containing-area gain; inferred
    virtual void removeAudioVolumeForArea(PolygonTrigger *area) = 0; // +0x178: name: 551C2 erases matching area and invalidates area gains; inferred
    virtual void clearAudioVolumeAreas() = 0; // +0x17C: name: 5710F clears area vector+B54 and invalidates gains; inferred
    virtual void onPolygonTriggerDeleted(PolygonTrigger *) = 0; // +0x180: role-inferred;23D661 clears object trigger refs via28B31A then calls this;4B0849 does so after unlink and before deletion;512BC forwards to slot94
    virtual bool setUseEAX(bool enable) = 0; // +0x184: name: old caller evidence; 0x000619D1
    virtual void onAudioLODChanged() = 0; // +0x188: target name: 0x000607BB
    virtual bool hasActiveSubtitles() = 0; // +0x18C: name: inferred; 531F6 tests SubTitleWindow existence and nonempty text/control span via45837
    virtual void destroySubtitleWindow() = 0; // +0x190: name: inferred; 514BB deletes and clears SubTitleWindow+BE8
    virtual void lockAudioDevice() = 0; // +0x194: name: inferred; 53281 enters device reference+B90 critical section+40 unless flag58 set
    virtual void unlockAudioDevice() = 0; // +0x198: name: inferred; 5329C leaves same conditional critical section
    virtual void friend_xferSubAudioEventRTS(Xfer *,PooledAudioEventRTSRef *) = 0; // +0x19C: target5987D, WB79AEB0 names pooled-event serialization; original handle spelling unresolved
    virtual void unknownAudioSlot104() = 0; // +0x1A0: UNRESOLVED name/signature; position only
};
// SIZE 0x10; no members beyond the two bases.
typedef char AudioManagerSizeCheck[sizeof(AudioManager)==0x10 ? 1 : -1];
extern AudioManager *TheAudio;
#endif
