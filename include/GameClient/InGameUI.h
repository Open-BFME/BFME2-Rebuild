#pragma once
struct FieldParse;
class View;
enum RadiusCursorType;
enum WeaponSlotType;
#include <stddef.h>
#include "Common/SubsystemInterface.h"
#include "Common/Snapshot.h"
#include "Common/Coord2D.h"
#include "Common/Coord3D.h"
#include "Common/PixelRegion.h"
#include "Common/StlList.h"
#include "Common/FreelistPool.h"
#include "Common/GameType.h"
#include "GameClient/InGameNotificationBox.h"
#if defined(_STLPORT_VERSION)
#include <map>
#include <list>
#include <vector>
#endif
#include "GameClient/Mouse.h"
#include "Common/RadiusDecal.h"
#include "Common/UnicodeString.h"
#include "Common/GlobalLanguage.h"
class DisplayString; struct RGBColor;
class Drawable; class Object; class Player; class CommandButton;
class ThingTemplate; class SpecialPowerTemplate; class GameMessage;
class GameWindow; class VideoBuffer; class VideoStreamInterface;
class InGameNotificationBoxMovieClip; class WindowLayout;
class FloatingTextData; class WorldAnimationData; class MaterialPassClass;
class Overridable; class Anim2DTemplate; class SuperweaponInfo; class INI;
enum LegalBuildCode; enum KindOfType; enum CanAttackResult;
enum SpecialPowerType; enum WorldAnimationOptions;
// Target ctor2A61A9/dtor2A5B30: SubsystemInterface+0, Snapshot+C,
// InGameNotificationBox+10. Primary table7FD410 has120 slots; secondary
// tables7FD400 (4) and7C7A64 (5). Derived ctor8EF3F starts members at9F0.
// Sixty synthesized old views; fields below also audited against native ctor
// and INI table7FC338. Names: donor (ZH) where target accesses agree.
// Unknown spans and slot names remain explicit; no include-dependent layout.
class GameClientRandomVariable;
// A help box font setting: the font description and its colour. Size 0x10 (the getters 0x0029F8B8, 0x0029D844, 0x0029D8C6 and 0x0029D948 return one by value: a 16-byte frame slot).
struct HelpBoxFontDesc : public FontDesc { int color; };
class InGameUI : public SubsystemInterface, public Snapshot, public InGameNotificationBox {
public:
    enum SelectionRules { SELECTION_ANY = 0, SELECTION_ALL = 1 };
    // Values from getCanSelectedObjectsAttack 0x0029C779: 1 is the attack action, 14 the rally point action.
    enum ActionType { ACTIONTYPE_ATTACK_OBJECT = 1, ACTIONTYPE_SET_RALLY_POINT = 14 };
    class FormationPreviewPoolObject;
    // The subtitle shown by militarySubtitle (0x0029F530) and typed out by updateMilitarySubtitle (0x0029C404); size 0x48.
    // Names are Zero Hour's where it has the member; layout from the three functions and the old private view.
    enum { MAX_SUBTITLE_LINES = 4 };
    struct MilitarySubtitleData {
        UnicodeString subtitle;                                 // +0x00
        unsigned int index;                                     // +0x04
        DisplayString *displayStrings[MAX_SUBTITLE_LINES];      // +0x08
        int lineOffsets[MAX_SUBTITLE_LINES];                    // +0x18
        DisplayString *wordString;                              // +0x28 the word being typed
        unsigned int currentDisplayString;                      // +0x2C
        unsigned int lifetime;                                  // +0x30
        unsigned int nextCharTime;                              // +0x34
        unsigned int elapsed;                                   // +0x38
        unsigned int lastTime;                                  // +0x3C
        unsigned int color;                                     // +0x40
        bool finished;                                          // +0x44
    };
    class FormationPreviewDecal;
    InGameUI();
    virtual ~InGameUI();
    virtual void init();
    virtual void reset();
    virtual void update();
    virtual void draw() = 0; // SubsystemInterface slot 12; InGameUI's table holds __purecall there

    virtual void __cdecl multiplayerMessageColor(const RGBColor *, UnicodeString, ...); //29E99F: multiplayer gate then formatted message
    virtual void __cdecl multiplayerMessage(UnicodeString, ...); //reverse declaration: actual slot16
    virtual void __cdecl multiplayerMessage(AsciiString, ...); //actual slot15;29E656/29E846 same multiplayer gate
    virtual void __cdecl messageColor(const RGBColor *, UnicodeString, ...);
    virtual void __cdecl message(UnicodeString, ...);
    virtual void __cdecl message(AsciiString, ...);
    virtual void toggleMessages() { m_messagesOn = 1 - m_messagesOn; }
    virtual bool isMessagesOn() { return m_messagesOn; }
    virtual void militarySubtitle(const AsciiString &, int);
    virtual void militarySubtitle(UnicodeString, int);
    virtual void removeMilitarySubtitle();
    virtual void displayCantBuildMessage(LegalBuildCode);
    virtual void beginAreaSelectHint(const GameMessage *);
    virtual void endAreaSelectHint(const GameMessage *);
    virtual void createMoveHint(const GameMessage *);
    virtual void createAttackHint(const GameMessage *);
    virtual void createForceAttackHint(const GameMessage *);
    virtual void createMouseoverHint(const GameMessage *);
    virtual void createCommandHint(const GameMessage *);
    virtual void createGarrisonHint(const GameMessage *);
    virtual void addSuperweapon(int, const AsciiString &, ObjectID, const SpecialPowerTemplate *);
    virtual bool removeSuperweapon(int, const AsciiString &, ObjectID, const SpecialPowerTemplate *);
    virtual void objectChangedTeam(const Object *, int, int);
    virtual void setSuperweaponDisplayEnabledByScript(bool);
    virtual bool getSuperweaponDisplayEnabledByScript() const;
    virtual void hideObjectSuperweaponDisplayByScript(const Object *);
    virtual void showObjectSuperweaponDisplayByScript(const Object *);
    virtual void setScrolling(bool);
    virtual bool isScrolling();
    virtual void setSelecting(bool);
    virtual bool isSelecting();
    virtual void setScrollAmount(Coord2D);
    virtual Coord2D getScrollAmount();
    virtual void setGUICommand(const CommandButton *);
    virtual const CommandButton *getGUICommand() const;
    virtual void beginFormationPreview(const Coord3D *); //29AA62 seeds990/99C and active98C; inferred
    virtual void setFormationPreviewEnd(const Coord3D *); //29AAC8 copies99C; inferred
    virtual bool isFormationPreviewActive() { return m_formationPreviewActive; }
    virtual int slot52() { return 0; }
    virtual void updateFormationPreview();
    virtual bool finishFormationPreview(); //29DB61 posts formation command464 and voice; inferred
    virtual void placeBuildAvailable(const ThingTemplate *, Drawable *);
    virtual const ThingTemplate *getPendingPlaceType();
    virtual ObjectID getPendingPlaceSourceObjectID();
    virtual void setPlacementStart(const ICoord2D *);
    virtual void setPlacementEnd(const ICoord2D *);
    virtual bool isPlacementAnchored();
    virtual void getPlacementPoints(ICoord2D *, ICoord2D *);
    virtual float getPlacementAngle();
    virtual bool isClientQuiet() const { return m_clientQuiet; } //name: donor(ZH)
    virtual void setClientQuiet(bool value) { m_clientQuiet = value; }
    virtual void clearPlacementAnchor(); //29AB03 clears anchor554; inferred
    virtual void selectDrawable(Drawable *);
    virtual void deselectDrawable(Drawable *);
    virtual void deselectAllDrawables();
    virtual void selectAllUnitsAcrossMap(bool, Player *); //29BE05 enumerates eligible player objects; inferred
    virtual int getSelectCount() { return m_selectCount; }
    virtual int getMaxSelectCount() { return m_maxSelectCount; }
    virtual unsigned int getFrameSelectionChanged() { return m_frameSelectionChanged; }
    virtual const _STL::list<Drawable *> *getAllSelectedDrawables() const;
    virtual const _STL::list<Drawable *> *getAllSelectedLocalDrawables();
    virtual Drawable *getFirstSelectedDrawable();
    virtual DrawableID getSoloNexusSelectedDrawableID() { return (DrawableID)m_soloNexusSelectedDrawableID; } //name: donor(ZH)
    virtual bool isDrawableSelected(DrawableID) const; //29C0A9 walks selected list and compares Drawable::getID; donor(ZH)
    virtual bool isAnySelectedKindOf(KindOfType) const;
    virtual bool isAllSelectedKindOf(KindOfType) const;
    virtual void setRadiusCursor(RadiusCursorType,const SpecialPowerTemplate *,WeaponSlotType,bool); //29FD1B; caller29A889 passes CommandButton4C/44/80; donor(ZH)
    virtual void setRadiusCursorNone();
    virtual void setInputEnabled(bool, bool *);
    virtual void disregardDrawable(Drawable *);
    virtual void preDraw();
    virtual void postDraw();
    virtual void drawMilitarySubtitle();
    virtual void playMovie(const AsciiString &); //name: donor(ZH);29ACE3 forwards named movie to player
    virtual void stopMovie();
    virtual VideoBuffer *videoBuffer(); //29ACFB delegates stream slot15; donor(ZH)
    virtual void playCameoMovie(const AsciiString &);
    virtual void stopCameoMovie();
    virtual VideoBuffer *cameoVideoBuffer(); //43812F reads5C8; donor(ZH)
    virtual int getMousedOverDrawableID() const;
    virtual void setQuitMenuVisible(bool); //29AC03 sets810 and cancels input modes; donor(ZH)
    virtual bool isQuitMenuVisible() const { return m_isQuitMenuVisible; }
    virtual const FieldParse *getInGameUIFieldParse() const; //slot96,8EFCC:81-entry table7FC338; distinct from inherited subsystem slot6
    virtual int selectUnitsMatchingCurrentSelection();
    virtual int selectMatchingAcrossScreen();
    virtual int selectMatchingAcrossMap();
    virtual int selectMatchingAcrossRegion(IRegion2D *);
    virtual void unidentifiedSlot101(const int *, const int *, int *);
    virtual bool getDisplayedMaxWarning() { return m_displayedMaxWarning; }
    virtual void setDisplayedMaxWarning(bool value) { m_displayedMaxWarning = value; }
    virtual void addFloatingText(const UnicodeString &, const Coord3D *, int);
    virtual void addIdleWorker(Object *);
    virtual void removeIdleWorker(Object *, int);
    virtual void selectNextIdleWorker();
    virtual void recreateControlBar();
    virtual void disableTooltipsUntil(unsigned int); //29B04D monotonic deadline7EC; donor(ZH)
    virtual void clearTooltipsDisabled(); //29B060 clears same deadline; donor(ZH)
    virtual bool areTooltipsDisabled() const;
private:
    virtual int getIdleWorkerCount();
    virtual Object *findIdleWorker(Object *);
    virtual void showIdleWorkerLayout();
    virtual void hideIdleWorkerLayout();
    virtual void updateIdleWorker();
public:
    virtual void resetIdleWorker();
    virtual View *createView() = 0; //W3D8EFD8 constructs W3DView2508; donor(ZH)
    virtual void createPalantir() = 0; //W3D8F32C allocates88, ctor10461F extends WB Palantir base; inferred

protected:
    virtual void loadPostProcess();
    virtual const char *GetSnapshotName() const { return "InGameUI"; }

    virtual void xfer(Xfer *);
public:
    virtual bool isNotificationEnabled() const;
    virtual void setNotificationEnabled(bool);
    virtual void openNotification(const UnicodeString &, const InGameNotificationData &, int);
    virtual void DoClose();
    virtual int hasActiveNotification();

    bool getInputEnabled() { return m_inputEnabled && m_engineInputEnabled; }
    bool isInWaypointMode() const { return m_inWaypointMode; }
    bool isInForceAttackMode() const { return m_inForceAttackMode; }
    bool isInForceMoveToMode() const { return m_inForceMoveMode; }
    int getDrawableCaptionPointSize() { return m_drawableCaptionPointSize; }
    bool isDrawableCaptionBold() { return m_drawableCaptionBold; }
    void setEngineInputEnabled(bool);
    void setInputEnabled(bool);
    void resetCamera();
    void showNamedTimerDisplay(Bool show);	// 0x0029A61A (Zero Hour's name; ScriptActions 0x003BC107 passes 0 and 0x003BC115 passes 1; the map spells it ?set@Rva0029A61AByteSlot@@QAEXE@Z)
    bool areSelectedObjectsControllable() const;
    void expireHint(int, unsigned int);
    void expireAllHints(int);
protected:
    void setMouseCursor(Mouse::MouseCursor);
    static GameWindow *m_replayWindow;  // VA 0x00DFEDF4, next to TheInGameUI: createReplayControl 0x0029C269 stores the replay-control window there (name: Zero Hour's member)
    void evaluateSoloNexus(Drawable * = 0);
    void addMessageText(const UnicodeString &, const RGBColor * = 0);
    void createControlBar();
    void createReplayControl();
    void updateMilitarySubtitle();
    void updateFloatingText();
    void drawFloatingText();
    void updateAndDrawWorldAnimations();
    void handleRadiusCursor();
    void handleBuildPlacements();
public:
    void updateLocalPhantomStructureDisplay();
    void updateOrderDisplayMode();
    void addNamedTimer(const AsciiString &, const UnicodeString &, bool);
    void addWorldAnimation(Anim2DTemplate *, const Coord3D *, WorldAnimationOptions, float, float);
    void rva002A4A68();
    Coord2D rva0029D9F9();
    void clearFormationPreviewDrawables();
    void rva002A3E24();
    bool rva0029CC19(const Object *);
    bool rva0029FCD9(const char *);
    float rva0029B3C1(const Object *) const;
    bool canSelectedObjectsDoAction(ActionType, const Object *, SelectionRules, bool) const;
    bool canSelectedObjectsEffectivelyUseWeapon(const CommandButton *, const Object *, const Coord3D *, SelectionRules) const;
    bool canSelectedObjectsNonAttackInteractWithObject(const Object *, SelectionRules) const;
    bool canSelectedObjectsDoSpecialPower(const CommandButton *, const Object *, const Coord3D *, SelectionRules, unsigned int, Object *) const;
    bool canSelectedObjectsOverrideSpecialPowerDestination(const Coord3D *, SelectionRules, SpecialPowerType) const;
    CanAttackResult getCanSelectedObjectsAttack(ActionType, const Object *, SelectionRules, bool) const;
    HelpBoxFontDesc getHelpBoxNameFont();		// 0x0029F8B8: the INI font, replaced by GlobalLanguage's when that has a name
    HelpBoxFontDesc getHelpBoxCostFont();		// 0x0029D844
    HelpBoxFontDesc getHelpBoxShortcutFont();	// 0x0029D8C6
    HelpBoxFontDesc getHelpBoxDescriptionFont();	// 0x0029D948
    void freeMessageResources();                // 0x0029B34B
    void freeMessageResource(int index);        // 0x0029B380
    void clearFloatingText();                   // 0x0029D30A
    void removeLocalPhantomStructureID(int id); // 0x0029F93A
    void addLocalPhantomStructureID(int id);    // 0x002A1111: adds the id unless it is 0 or already listed
    void rva002A123F(Object *);
    void rva002A1261(Object *);
    void rva002A1283(Object *);
    void notifyHeroEarnedAward(class CreateAHeroHero *, int);
    void notifyOfHeroSpawnEvent(Object *, const char *, const int *, float);
    FormationPreviewPoolObject *rva002A3E5A(ThingTemplate *);
    void rva002A470F(ThingTemplate *, const ICoord2D *, RGBColor);
    void rva002A4BFC(Object *, ThingTemplate *, const ICoord2D *, RGBColor);
    static void parseNamedRadiusDecalTemplate(INI *, void *, void *, const void *);
protected:
    SuperweaponInfo *findSWInfo(int, const AsciiString &, ObjectID, const SpecialPowerTemplate *);
public:
	// 0x002A4A0C: spawns a cursor particle burst (Anim2D template name by value, burst count, particle
	// life, system life, drift X, drift Y); CursorParticleSystemFXNugget 0x001E0D2D/0x001E0D73 call it.
	void rva002A4A0C(AsciiString anim2DName, UnsignedInt burstCount, const GameClientRandomVariable *particleLife,
		const GameClientRandomVariable *systemLife, const GameClientRandomVariable *driftVelX, const GameClientRandomVariable *driftVelY);

    bool m_unknown014;
    bool m_inputEnabled;
    bool m_engineInputEnabled;
    _STL::list<WindowLayout *> m_windowLayouts;

    AsciiString m_currentlyPlayingMovie;
    _STL::list<Drawable *> m_selectedDrawables;
    _STL::list<Drawable *> m_selectedLocalDrawables;

    bool m_dragSelecting;
    char opaque029[0x2C - 0x29];
    IRegion2D m_dragSelectRegion;

    bool m_displayedMaxWarning;
    // Move-hint ring (evidence: createMoveHint 0x0029EAFA stores the position at +0x40+0x14*i,
    // the frame at +0x4C+0x14*i, a byte at +0x50+0x14*i, then bumps the index at +0x234 and wraps it at 25;
    // expireHint 0x0029ACA0 clears the frame and sets the byte).
    struct MoveHint {
        Coord3D pos;
        unsigned int frame;
        bool expired;
        char pad[3];
    };
    MoveHint m_moveHint[25];
    int m_moveHintIndex;
    const CommandButton *m_currentCommand;
    char opaque23C[0x53C - 0x23C];
    const ThingTemplate *m_pendingPlaceType;
    ObjectID m_pendingPlaceSourceObjectID;
    Drawable **m_placeIcon;       // +0x544 drawables that follow the cursor while placing a building (m_placeIcon[0] is the anchor)
#if defined(_STLPORT_VERSION)
    _STL::vector<DrawableID> m_placeIconDrawableIDs; // +0x548 ids of the build-placement drawables; loadPostProcess 0x002A2184 (Snapshot +0xC: [ecx+0x53C]) hides and destroys them
#else
    unsigned char m_placeIconDrawableIDsStorage[0xC];
#endif
    bool m_placeAnchorInProgress;
    char opaque555[0x558 - 0x555];
    ICoord2D m_placeAnchorStart;
    ICoord2D m_placeAnchorEnd;

    int m_selectCount;
    int m_maxSelectCount;
    unsigned int m_frameSelectionChanged;
    char opaque574[0x5C4 - 0x574];
    VideoStreamInterface *m_videoStream;
    VideoBuffer *m_cameoVideoBuffer;
    VideoStreamInterface *m_5CC;
    // The on-screen message ring (addMessageText 0x0029BA58 shifts entries 0..4 into 1..5 through the
    // out-of-line UIMessage copy assignment 0x0029A778, then fills entry 0).
    struct UIMessage {
        UnicodeString fullText;        // +0
        DisplayString *displayString;  // +4
        unsigned int timestamp;        // +8 logic frame the message was added
        int color;                     // +0xC
    };
    UIMessage m_uiMessages[6];         // +0x5D0
    // Per-player superweapon displays: 20 maps (0xC bytes each) from the power name to its entries
    // (findSWInfo 0x002A435D indexes them as 0x630 + 0xC * playerIndex and reads each entry's object id at +0x18;
    // hide/showObjectSuperweaponDisplayByScript 0x0029E20B/0x0029E25A walk all 20). Real STLport types under // stlport46.
#if defined(_STLPORT_VERSION)
    _STL::map<AsciiString, _STL::list<SuperweaponInfo *> > m_superweapons[20];
#else
    unsigned char m_superweaponStorage[20][0xC];
#endif
    char opaque720[0x728 - 0x720];
    float m_superweaponFlashDuration;
    AsciiString m_superweaponNormalFont;
    int m_superweaponNormalPointSize;
    bool m_superweaponNormalBold;
    AsciiString m_superweaponReadyFont;
    int m_superweaponReadyPointSize;
    bool m_superweaponReadyBold;
    char opaque741[0x748 - 0x741];
    int m_superweaponFlashColor;
    char opaque74C[0x76C - 0x74C];
    float m_namedTimerFlashDuration;
    char opaque770[0x774 - 0x770];
    int m_namedTimerFlashColor;
    char opaque778[0x77C - 0x778];
    AsciiString m_namedTimerNormalFont;
    int m_namedTimerNormalPointSize;
    bool m_namedTimerNormalBold;
    int m_namedTimerNormalColor;
    AsciiString m_namedTimerReadyFont;
    int m_namedTimerReadyPointSize;
    bool m_namedTimerReadyBold;
    int m_namedTimerReadyColor;
    AsciiString m_drawableCaptionFont;
    int m_drawableCaptionPointSize;
    bool m_drawableCaptionBold;
    int m_drawableCaptionColor;
    // The four help box fonts (INI HelpBox<Name|Cost|Shortcut|Description> Font/PointSize/Bold/Color, table 0x007FC338): each a font description followed by its colour.
    HelpBoxFontDesc m_helpBoxNameFont;          // +0x7AC
    HelpBoxFontDesc m_helpBoxCostFont;          // +0x7BC
    HelpBoxFontDesc m_helpBoxShortcutFont;      // +0x7CC
    HelpBoxFontDesc m_helpBoxDescriptionFont;   // +0x7DC
    unsigned int field7EC;
    MilitarySubtitleData *m_militarySubtitle;   // +0x7F0 the typed military subtitle being shown, or null (removeMilitarySubtitle 0x002A0C87)
    char opaque7F4[0x7F8 - 0x7F4];
    bool m_isScrolling;
    bool m_isSelecting;
    int m_mouseMode;          // 0 default, 2 GUI-command targeting (setGUICommand 0x0029A889)
    int m_mouseModeCursor;    // cursor restored after a GUI command (setMouseCursor 0x0029A5D6)
    unsigned int m_804;
    Coord2D m_scrollAmt;

    bool m_isQuitMenuVisible;
    bool m_messagesOn;
    int m_messageColor1;
    int m_messageColor2;
    ICoord2D m_messagePosition;
    ICoord2D m_824;

    AsciiString m_messageFont;
    int m_messagePointSize;
    bool m_messageBold;
    int m_messageDelayMS;
    char opaque83C[0x858 - 0x83C];
    AsciiString m_militaryCaptionTitleFont;
    int m_militaryCaptionTitlePointSize;
    bool m_militaryCaptionTitleBold;
    AsciiString m_militaryCaptionFont;
    int m_militaryCaptionPointSize;
    bool m_militaryCaptionBold;
    char opaque86D[0x888 - 0x86D];
    int m_radiusCursorType;   // setRadiusCursorNone 0x0029A64A stores 0
    RadiusDecal m_radiusDecal; // +0x88C, size 0x10; clear() at 0x00330DBA
    char opaque89C[4];
    _STL::list<FloatingTextData *> m_floatingTextList;

    float m_floatingTextTimeOut;
    float m_floatingTextMoveUpSpeed;
    float m_floatingTextMoveVanishRate;
    bool m_inWaypointMode;
    int m_8B4;
    bool m_inForceAttackMode;
    bool m_inForceMoveMode;
    bool m_8BA;
    bool m_8BB;
    bool m_8BC;
    bool m_8BD;
    bool m_8BE;
    bool m_8BF;
    bool m_8C0;
    bool m_8C1;
    bool m_8C2;
    bool m_drawRMBScrollAnchor;
    bool m_moveRMBScrollAnchor;
    unsigned char m_8C5;
    bool m_clientQuiet;
    char opaque8C7[0x8CC - 0x8C7];
    _STL::list<WorldAnimationData *> m_worldAnimationList;

    char opaque8D0[0x928 - 0x8D0];
    _STL::list<Object *, FreelistAllocator<Object *> > m_idleWorkers[20];	// pooled: addIdleWorker 0x002A3D98 calls the pooled push_back 0x001EC03C

    GameWindow *m_idleWorkerWin;
    int m_97C;
    unsigned int m_soloNexusSelectedDrawableID;
    unsigned int m_984;
    unsigned int m_unknown988;
    bool m_formationPreviewActive;
    char opaque98D[0x9B4 - 0x98D];
    unsigned char m_9B4;
    char opaque9B5[0x9B8 - 0x9B5];
    Coord3D m_9B8;

    _STL::list<int> m_localPhantomStructureIDs;  // +0x9C4 ObjectIDs of the local player's phantom (not yet built) structures; updateLocalPhantomStructureDisplay 0x002A1158 walks it, 0x0029F93A removes an id

    int m_9C8;
    InGameNotificationBoxMovieClip *m_notificationBox;
    // The INI table 0x007FC338 (entries 73..80) names these eight members: a message and a timeout for each hero event.
    AsciiString m_heroInitialSpawnNotificationMessage;    // +0x9D0 HeroInitialSpawnNotificationMessage
    float m_heroInitialSpawnNotificationTimeout;          // +0x9D4
    AsciiString m_heroRespawnNotificationMessage;         // +0x9D8
    float m_heroRespawnNotificationTimeout;               // +0x9DC
    AsciiString m_heroDeathNotificationMessage;           // +0x9E0
    float m_heroDeathNotificationTimeout;                 // +0x9E4
    AsciiString m_heroEarnedAwardNotificationMessage;     // +0x9E8
    float m_heroEarnedAwardNotificationTimeout;           // +0x9EC
};

extern InGameUI *TheInGameUI;

typedef char InGameUISizeCheck[sizeof(InGameUI)==0x9F0 ? 1 : -1];
typedef char InGameUICommandOffset[offsetof(InGameUI,m_currentCommand)==0x238 ? 1 : -1];
