// DRAFT (h05 modgen): generated from constructor RVA 0x004B21B9 and xfer RVA 0x004B1ECB; no function
// matches against it yet, so it stays in Draft until one does.
// EmotionTrackerUpdate.h: the EmotionTrackerUpdate module. SIZE 0xC8 (ModuleFactory operator new).
// Target facts (h05 modgen): the constructor 0x004B21B9 calls UpdateModule first; it stores the
// tables 0x00C1C780 at +0x20, 0x00C5667C at +0x0, 0x00C565C0 at +0xC, 0x00C565B0 at +0x10, 0x00C565AC at +0x20. Members are what xfer (0x004B1ECB) transfers after UpdateModule::xfer,
// typed by the Xfer slot each goes through, in that order; m_unk names are offsets
// (no map or donor name established). Bytes xfer does not touch are gaps.
// Overrides of the base tables besides the trio (slot -> target, map name):
//   +0x20 slot 0 -> 0x0003B810 __purecall
//   +0x20 slot 2 -> 0x000B3FD0 ??1Region2D@@QAE@XZ
//   +0x20 slot 5 -> 0x0055CFF0 ?rva0055CFF0@Rva0055CFF0@@QAEXMMM@Z
//   +0x20 slot 6 -> 0x0047A699 ?IsCRC@Xfer@@UBE_NXZ
//   +0x20 slot 7 -> 0x003ABEFD ?rva003ABEFD@Rva003ABEFD@@QAEMXZ
//   +0x20 slot 8 -> 0x0055D0D5 ?rva0055D0D5@Rva0055D0D5@@QAE?AUCoord3D0055D0D5@@IIII@Z
//   +0x20 slot 9 -> 0x003ABFD9 ??_ERva003ABC58@@WBM@AEPAXI@Z
//   +0x20 slot 13 -> 0x0055E636 ?Rva0055E636Compute@@YGXPAVCoord3D@@0MM@Z
//   +0x20 slot 17 -> 0x0055D3F5 ?rva0055D3F5@Rva0055D3F5@@UAEXPAVXfer@@@Z
//   +0x20 slot 18 -> 0x0055D422 ?debugDraw@Rva0055D3F5@@QAEXUCoord3D@@@Z
//   +0x20 slot 21 -> 0x0055D48C ?getPosition@SphereEmissionVolumeModule@FXParticleSystem@@UAE?AUCoord3D@@MMMM@Z
//   +0x20 slot 25 -> 0x003AC0D2 ?rva0055D3F5@Rva0055D3F5@@WBM@AEXPAVXfer@@@Z
//   +0x20 slot 26 -> 0x0055D686 ?computeCylinderEmissionPosition@@YGXPAVCoord3D@@PAVCoord2D@@MM@Z
//   +0x20 slot 28 -> 0x0055D616 ?rva0055D616@Rva003AF7FE@@UAEXXZ
//   +0x20 slot 31 -> 0x0055D6D7 ?rva0055D6D7@Rva003AF7FE@@QAEXUCoord3D@@@Z
//   +0x20 slot 34 -> 0x0055D741 ?getPosition@CylinderEmissionVolumeModule@FXParticleSystem@@UAE?AUCoord3D@2@MMMM@Z
//   +0x20 slot 43 -> 0x000D1407 ?Set_Animation@RenderObjClass@@UAEXPAVHAnimClass@@MH@Z
//   +0x20 slot 45 -> 0x00098555 ?Get_ObjectScale@RenderObjClass@@QBE?BMXZ
//   +0x20 slot 46 -> 0x0055E697 ?rva0055E697@Rva0055E697@@QAEXHHHHH@Z
//   +0x20 slot 60 -> 0x0055E81F ?getVelocity@OrthoEmissionVelocityModule@FXParticleSystem@@UAE?AUCoord3D@2@HH@Z
//   +0x20 slot 61 -> 0x003AC1EF ??_ERva003ABA36@@WBI@AEPAXI@Z
//   +0x20 slot 69 -> 0x00564AB8 ?rva00564AB8@Rva00564AB8@@QAEXPAUCoord3D@@PBX1@Z
//   +0x20 slot 74 -> 0x003AC243 ??_ERva003AD19BModule@@WBI@AEPAXI@Z
//   +0x20 slot 82 -> 0x0055EB84 ?getVelocity@CylindricalEmissionVelocityModule@FXParticleSystem@@UAE?AUCoord3D@2@HH@Z
//   +0x20 slot 91 -> 0x0055ECF9 ?getVelocity@OutwardEmissionVelocityModule@FXParticleSystem@@UAE?AUCoord3D@2@PBU32@PAVEmissionVolumeModuleInterface@2@@Z
//   +0x20 slot 92 -> 0x003A5A48 ??_ERva003ADFB2@@WM@AEPAXI@Z
//   +0x20 slot 94 -> 0x003AC930 ?Rva003AC930Get@@YAHXZ
//   +0x20 slot 96 -> 0x003ACF2D ?getClass@?$ConcreteModuleTemplate@V?$DefaultModuleTag@$00@FXParticleSystem@@@FXParticleSystem@@UBEABV?$ConcreteModuleClass@V?$DefaultModuleTag@$00@FXParticleSystem@@@2@XZ
//   +0x20 slot 98 -> 0x0055B489 ?updateAlpha@Rva0055B39C@@QAEXXZ
//   +0x20 slot 99 -> 0x003ADC32 ?clone@Rva003ADC4F@@QBEPAV1@XZ
//   +0x20 slot 101 -> 0x0055B417 ?rva0055B417@Rva0055B417@@QBE_NH@Z
//   +0x20 slot 102 -> 0x0008BB58 ?get@Rva0008BB58FloatField@@QBEMXZ
//   +0x0 slot 1 -> 0x004B0FB9 ?loadPostProcess@EmotionTrackerUpdate@@MAEXXZ
//   +0x0 slot 2 -> 0x004B138D ?name@EmotionTrackerUpdate@@QBEPBDXZ
//   +0x0 slot 12 -> 0x000D43D0 ?ControlBarInput@@YA?AW4WindowMsgHandledType@@PAVGameWindow@@III@Z
//   +0xC slot 6 -> 0x004543A9 ?getUpdateExitInterface@QueueProductionExitUpdate@@UAEPAVExitInterface@@XZ
//   +0x10 slot 0 -> 0x004B1633 ?update@EmotionTrackerUpdate@@UAE?AW4UpdateSleepTime@@XZ
//   +0x20 slot 0 -> 0x004B15E6 ?rva004B15E6@EmotionTrackerUpdate@@UAEXXZ
//   +0x20 slot 2 -> 0x00253376 ?getDisabledTypesToProcess@UpdateModule@@UBE?AV?$BitFlags@$0L@@@XZ
//   +0x20 slot 3 -> 0x0044DF8D ?rva0044DF8D@UpdateModule@@UAEXW4UpdateSleepTime@@@Z
// Not established: transfers xfer makes that this view cannot place: Bool ?, UnsignedInt ?, slot8 ?, ?xfer@Rva004DD489@@UAEXPAVXfer@@@Z ?, slot4 +0xA0, XferRawBytes +0xB0.
// Interfaces after the base (own table -> class): +0x20 DieModuleInterface.
#pragma once
#ifndef BFME2_EMOTION_TRACKER_UPDATE_H
#define BFME2_EMOTION_TRACKER_UPDATE_H

#include <stddef.h>
#include <vector>
#include <set>
#include "Common/GameType.h"
#include "GameLogic/Module/UpdateModule.h"
#include "GameLogic/Module/DieModuleInterface.h"
#include "GameLogic/PartitionFilter.h"

class Thing;
class ModuleData;
class Xfer;
class EmotionNugget;
class Player;

// UpdateModule ends at 0x20; this class's members follow it.
typedef char EmotionTrackerUpdate_base_size[(sizeof(UpdateModule) == 0x20) ? 1 : -1];

// The +0x20 interface: one slot, a void method without arguments (retail table 0x00C565AC, slot 0 is 0x004B15E6; a DieModuleInterface
// slot would take the DamageInfo and return with `ret 4`, this returns with `ret`). The base table is the shared one-slot purecall table 0x00C1C780.
class EmotionTrackerUpdateInterface
{
public:
	virtual void rva004B15E6() = 0;											// 0: destroys the active nugget's state and every nugget
};

class EmotionTrackerUpdate : public UpdateModule, public EmotionTrackerUpdateInterface
{
public:
	// The partition filter the taunt/point scan builds (vtable: shared PartitionFilter destructor 0x00395A19, allow 0x004B0FEB,
	// getPlayerMask 0x004B0D28). Members are what those two read: the owning tracker at +0x08, the scanning object at +0x0C,
	// its player at +0x10, two words handed to EmotionNugget::CanBeActivatedFor at +0x14/+0x18, the last accepted candidate at
	// +0x1C and a flag at +0x20 (f10, 2026-11-17; the Draft spelled the class as a namespace).
	class TauntPointFilter : public PartitionFilter
	{
	public:
		virtual bool Allow(Object *candidate);		// 0x004B0FEB
		virtual int getPlayerMask();				// 0x004B0D28
		EmotionTrackerUpdate *m_tracker;			// +0x08
		Object *m_source;							// +0x0C
		Player *m_player;							// +0x10
		Int m_unk0x14;								// +0x14
		Int m_unk0x18;								// +0x18
		Object *m_lastAccepted;						// +0x1C
		Bool m_unk0x20;								// +0x20
	};
	EmotionTrackerUpdate(Thing *thing, const ModuleData *moduleData);
	virtual NameKeyType getModuleNameKey() const;
	virtual const char *GetSnapshotName() const;
	const char *name() const;				// 0x004B138D (QBE: public, non-virtual)
	virtual ~EmotionTrackerUpdate();
	virtual UpdateSleepTime update();					// 0x004B1633 (the +0x10 UpdateModuleInterface slot 0; the row is unwritten)
	virtual void *slot06() { return static_cast<EmotionTrackerUpdateInterface *>(this); }	// BehaviorModuleInterface slot 6 (retail 0x00C565C0): returns the +0x20 interface (shared stub 0x004543A9)

protected:
	virtual void xfer(Xfer *xfer);
	virtual void loadPostProcess();					// 0x004B0FB9: slot 1 of the own table
	virtual SleepyUpdatePhase getUpdatePhase() const { return (SleepyUpdatePhase)0; }	// slot 12 is the shared `xor eax,eax; ret` 0x000D43D0, not UpdateModule's 0x0022C4CB

public:
	virtual void rva004B15E6();							// 0x004B15E6 (the +0x20 interface slot)
	void rva004B0D4C(Int index, void *p);			// 0x004B0D4C: PulseEmotion without a delay (frame slot cleared)
	void PulseEmotion(Int index, void *p, Int delay);	// 0x004B0D70
	void clearFlag(Int index);					// 0x004B0DA0
	EmotionNugget *selectNugget();			// 0x004B13D8: the nugget to run now, or none (retries once with the forced emotion cleared)
	void deactivateIfStateSet();			// 0x004B0E9C
	Bool rva004B0EBC();							// 0x004B0EBC: true when any nugget's entry has the byte at +0x18C set
	void ForceEmotion(Int kind, Real seconds, Int source);	// 0x004B0DAC (the ledger spells the third argument int; the body reads an Object* in it)
	Bool m_flags[12];                              // +0x24; PulseEmotion 0x004B0D70 sets flags[index] (12 slots)
	UnsignedInt m_expiry[12];                      // +0x30; PulseEmotion stores the frame plus the delay
	ObjectID m_targetID[12];                       // +0x60; PulseEmotion stores the object id (0 without an object)
	_STL::vector<EmotionNugget *> m_nuggets;       // +0x90; begin, end, end of storage (loadPostProcess 0x004B0FB9 indexes it)
	EmotionNugget *m_active;                       // +0x9C; ForceEmotion 0x004B0DAC clears it, loadPostProcess sets it
	Int m_unk0xA0;                                 // +0xA0; xfer Int
	_STL::set<ObjectID> m_unk0xA4;                 // +0xA4; xfer (versions below 2) writes its size from +0xA8 and each id; destructor 0x004B1322 destroys it
	Int m_forcedKind;                              // +0xB0; ForceEmotion stores its index arg; xfer raw 4 bytes
	Int m_forcedFrames;                            // +0xB4; ForceEmotion stores the ceiling of value*scale*1000; xfer Int
	ObjectID m_sourceID;                           // +0xB8; ForceEmotion stores the arg object's id (0 without one); xfer ObjectID
	UnsignedInt m_unk0xBC;                         // +0xBC; xfer UnsignedInt
	Bool m_unk0xC0;                                // +0xC0; xfer Bool
	Int m_activeIndex;                             // +0xC4; xfer Int; the saved index of m_active in m_nuggets (-1 without one)
};
// SIZE 0xC8
typedef char EmotionTrackerUpdate_size[(sizeof(EmotionTrackerUpdate) == 0xC8) ? 1 : -1];
typedef char EmotionTrackerUpdate_off_m_unk0xA0[(offsetof(EmotionTrackerUpdate, m_unk0xA0) == 0xA0) ? 1 : -1];
typedef char EmotionTrackerUpdate_off_m_unk0xA4[(offsetof(EmotionTrackerUpdate, m_unk0xA4) == 0xA4) ? 1 : -1];
typedef char EmotionTrackerUpdate_off_m_nuggets[(offsetof(EmotionTrackerUpdate, m_nuggets) == 0x90) ? 1 : -1];
typedef char EmotionTrackerUpdate_off_m_active[(offsetof(EmotionTrackerUpdate, m_active) == 0x9C) ? 1 : -1];
typedef char EmotionTrackerUpdate_off_m_forcedKind[(offsetof(EmotionTrackerUpdate, m_forcedKind) == 0xB0) ? 1 : -1];
typedef char EmotionTrackerUpdate_off_m_forcedFrames[(offsetof(EmotionTrackerUpdate, m_forcedFrames) == 0xB4) ? 1 : -1];
typedef char EmotionTrackerUpdate_off_m_sourceID[(offsetof(EmotionTrackerUpdate, m_sourceID) == 0xB8) ? 1 : -1];
typedef char EmotionTrackerUpdate_off_m_unk0xBC[(offsetof(EmotionTrackerUpdate, m_unk0xBC) == 0xBC) ? 1 : -1];
typedef char EmotionTrackerUpdate_off_m_unk0xC0[(offsetof(EmotionTrackerUpdate, m_unk0xC0) == 0xC0) ? 1 : -1];
typedef char EmotionTrackerUpdate_off_m_activeIndex[(offsetof(EmotionTrackerUpdate, m_activeIndex) == 0xC4) ? 1 : -1];

#endif
