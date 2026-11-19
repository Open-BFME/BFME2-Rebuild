#pragma once
#include "Common/GameType.h"
#include "Common/AsciiString.h"
#include "Common/Coord3D.h"
#include "Common/AudioEventRef.h"
#include <stddef.h>
class Xfer;
struct OpaqueRefElement4;
// Target constructors2D97D6/2DA461, destructor2D9A43 and table803908 identify
// the old BfmeAudioEventPrefix136/BfmeStringTailRecord144 views as AudioEventRTS.
// Size88, one deleting destructor slot2DA070. Seven synthesized AudioEventRTS
// views plus the complete constructor/copy views establish the member order.
// Names from ZH or WB are used only where their reads/stores agree.
class Coord3D;
class AudioEventRTS
{
public:
    AudioEventRTS(const OpaqueRefElement4 &,ObjectID);
    AudioEventRTS(const PooledAudioEventRTSRef &,ObjectID);   // the same 0x002DA461 constructor reached through the module's pooled-event handles (BattlePlanUpdate::setStatus 0x00497FF3)
    AudioEventRTS(const OpaqueRefElement4 &,int);   // 0x002D97D6 (map ??0BfmeAudioEventPrefix136@@QAE@ABUOpaqueRefElement4@@H@Z; StealthUpdate::changeVisualDisguise 0x00374BC8)
    void rva002D96D3(const OpaqueRefElement4 &);	// 0x002D96D3 (map name; the shared member initialiser the constructors run on the event name)
    AudioEventRTS(const OpaqueRefElement4 &,const Coord3D *,int);   // 0x002D982A (ScriptActions::doPlaySoundEffectAt 0x003BD679 passes the waypoint's location and 0)
    virtual ~AudioEventRTS();
    OpaqueRefElement4 &assignSound(const OpaqueRefElement4 &); // legacy consumed API2D9C2F
    void setObjectID(ObjectID);
    // 0x002D9508: positions the event (SoundFXNugget 0x001E00C3/0x001E012E).
    void setPosition(const Coord3D *pos);
    // 0x0033F15D (an ICF-folded one-store setter): the owning player index
    // (SoundFXNugget::doFXObj 0x001E012E passes the source's player index).
    void setPlayerIndex(Int playerIndex);
    ObjectID getObjectID();
    void setDrawableID(DrawableID);
    DrawableID getDrawableID();
    // Native owner categories3/4/5: strategic clickable/region/army identifiers.
    // Their original accessor spellings are unresolved; keep map spellings.
    void rva002D9583(int id);
    int rva002D959F() const;
    void rva002D95AC(int id);
    int rva002D95C8() const;
    void rva002D95D5(int id);
    int rva002D95F1() const;
    void clampDelay(float minimum, float maximum); // 0x002D9BDC: m_delay limited to the range
    const AsciiString *getEventName() const; // 0x002D9AC3: the event info's audio name, or the empty string
    bool rva002D9608();
    AsciiString getDecayFilename() const;
    AsciiString rva002DA867();
    bool hasMoreLoops() const;
    bool isPositionalAudio() const;
    unsigned int getSoundClass() const;
    void advanceNextPlayPortion();
    void generatePlayInfo();
    void generateFilename();
    AsciiString getFilename();
    AsciiString getAttackFilename() const;
    static const AsciiString *generateFilenamePrefix(int);
    void rva002D9ADC();
    void internalXfer(Xfer *,const void *);
    const AudioEventInfo *getAudioEventInfo() const { return m_eventInfo.get(); }
    int getNextPlayPortion() const { return m_portionToPlayNext; }
    int getViewType() const { return m_viewType; }
    void decrementNumberOfEventsNeedingToBeDoneBeforeReplaying() { --m_eventsBeforeReplay; }
    void setNumberOfTimesToPlayMusicOrMultisound(int count) { m_loopCount = count; }

    AsciiString m_filenameToLoad; // 04
    AudioEventInfoRef m_eventInfo; // 08
    int m_playingHandle; // 0C
    PooledAudioEventRTSRef m_waitingEvent; // 10
    int m_eventsBeforeReplay; // 14
    int m_unk18;
    AsciiString m_attackName, m_decayName; // 1C,20
    float m_unk24, m_unk28;
    float m_volume; // 2C
    int m_viewType; // 30
    unsigned int m_ownerID; // 34
    int m_ownerType; // 38: drawable1,object2,other owners3..5,invalid6
    Coord3D m_position; // 3C
    bool m_unk48, m_unk49, m_unk4A, m_unk4B;
    bool m_bypassLoops, m_filenameDirty, m_filenameGenerated, m_unk4F;
    bool m_startedPlaying, m_unk51, m_unk52, m_sequential;
    float m_pitchShift, m_pitchShift2, m_volumeShift, m_volumeShift2, m_delay; // 54..64
    int m_playingAudioIndex; // 68
    void *m_ownerCache; // 6C
    int m_unk70;
    int m_portionToPlayNext; // 74: attack0,sound1,decay2,done3
    int m_musicSystem; // 78
    int m_loopCount; // 7C
    int m_stopHolds; // 80
    AsciiString m_completionName; // 84
};
typedef char AudioEventRTSSize[sizeof(AudioEventRTS)==0x88?1:-1];
typedef char AudioEventRTSOwnerOffset[offsetof(AudioEventRTS,m_ownerID)==0x34?1:-1];
typedef char AudioEventRTSPortionOffset[offsetof(AudioEventRTS,m_portionToPlayNext)==0x74?1:-1];
// Compatibility name only; this is now one shared class, not a second layout.
typedef AudioEventRTS BfmeAudioEventPrefix136;