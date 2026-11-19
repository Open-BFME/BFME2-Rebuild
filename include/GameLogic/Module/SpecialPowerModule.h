// SpecialPowerModule.h: the special-power module interface, the concrete
// SpecialPowerModule base and its INI data.
// Target facts (retail game.dat):
// - SpecialPowerModule is BehaviorModule (ctor 0x00253330) plus
//   SpecialPowerModuleInterface at +0x10; size 0x34 (friend_newModuleInstance
//   0x0024DC54 `push 0x34`). Its ctor 0x00493C5A stores 0x00C4E868 at +0,
//   0x00BEFCF8 at +0xC and 0x00C5E080 at +0x10, zeroes +0x14..+0x30, pauses
//   when the data's StartsPaused is set and starts at TheGameLogic's frame when
//   AvailableAtStart is. Members follow xfer 0x00493009 (Version 3; after
//   BehaviorModule::xfer: v2 +0x14 uint, +0x18 uint, +0x1C int, +0x20 uint,
//   +0x24 real, +0x28 bool, +0x2C XferObjectID, v3 +0x30 bool).
// - Primary table 0x00C4E868 (14 slots) is BehaviorModule's 12 plus slots 12
//   (0x00493805) and 13 (ApplyPowerToObject 0x00493EA7); the interface table
//   0x00C5E080 has the 25 interface slots. The interface overrides take `this`
//   at +0x10 (getPowerName 0x004934B8 reads the module data at this-0xC).
// - SpecialPowerModuleData is 0x7C (friend_newModuleData 0x0024DC8C `push
//   0x7c`); members are retail field table entries 0x0084E628..0x0084E848.
#pragma once
#ifndef __BFME2_SPECIALPOWERMODULE_H_
#define __BFME2_SPECIALPOWERMODULE_H_

#include "Common/GameType.h"
#include "Common/AsciiString.h"
#include "Common/SpecialPowerTemplate.h"
#include "GameLogic/Module/BehaviorModule.h"
#include "GameLogic/Module/ModuleData.h"
#include "Common/Rva002390CB.h"
#include "Common/BitFlags11.h"
#include "GameLogic/ObjectFilter.h"

template <int N> class BitFlags;
class FXList;
class MultiIniFieldParse;
class Object;
class Rva002390CB;
class SpecialPowerModule;
struct BfmeWideResult;
class Thing;
class Waypoint;
class Xfer;
class Coord3D;

class SpecialPowerModuleInterface
{
public:
    virtual bool isModuleForPower(const SpecialPowerTemplate *power) const = 0;
    virtual bool isReady() const = 0;
    virtual float getPercentReady() const = 0;
    virtual bool specialPowerSlot03() const = 0;
    virtual unsigned int getReadyFrame() const = 0;
    virtual AsciiString getPowerName() const = 0;
    virtual const SpecialPowerTemplate *getSpecialPowerTemplate() const = 0;
    virtual void onSpecialPowerCreation() = 0;
    virtual void setReadyFrame(unsigned int frame) = 0;
    virtual void pauseCountdown(bool pause) = 0;
    virtual void doSpecialPower(unsigned int commandOptions) = 0;
    virtual void doSpecialPowerAtObject(Object *object, unsigned int commandOptions) = 0;
    virtual void doSpecialPowerAtLocation(const Coord3D *location, unsigned int commandOptions) = 0;
    virtual void doSpecialPowerUsingWaypoints(const Coord3D *location, const Waypoint *waypoint, unsigned int commandOptions) = 0;
    virtual void markSpecialPowerTriggered(const Coord3D *location) = 0;
    virtual void startPowerRecharge(float readyFraction) = 0;
    virtual Rva002390CB getInitiateSound() const = 0;
    virtual void stopSpecialPower() = 0;							// 17: SiegeDeploySpecialPower overrides it (0x004C5E41)
    virtual bool rva0049466D(const Coord3D *location) = 0;
    virtual void specialPowerSlot19() = 0;
    virtual unsigned int specialPowerSlot20() const = 0;
    virtual bool specialPowerSlot21() const = 0;
    virtual void specialPowerSlot22() = 0;
    virtual SpecialPowerModule *specialPowerSlot23() = 0;
    virtual void *specialPowerSlot24() = 0;
};


class SpecialPowerModuleData : public ModuleData
{
public:
	SpecialPowerModuleData();											// 0x004930A0
	static void buildFieldParse(MultiIniFieldParse &parse);				// 0x00493207

	const SpecialPowerTemplate *m_specialPowerTemplate;					// 0x08 SpecialPowerTemplate
	Bool m_updateModuleStartsAttack;									// 0x0C UpdateModuleStartsAttack
	Bool m_startsPaused;												// 0x0D StartsPaused
	Rva002390CB m_initiateSound;									// 0x10 InitiateSound, AudioEventRTS
	AsciiString m_attributeModifier;									// 0x18 AttributeModifier
	Real m_attributeModifierRange;										// 0x1C AttributeModifierRange
	Bool m_attributeModifierAffectsSelf;								// 0x20 AttributeModifierAffectsSelf
	ObjectFilter m_attributeModifierAffects;								// 0x24 AttributeModifierAffects, ObjectKinds filter
	const FXList *m_attributeModifierFX;								// 0x28 AttributeModifierFX
	Bool m_attributeModifierWeatherBased;								// 0x2C AttributeModifierWeatherBased
	UnsignedInt m_weatherDuration;										// 0x30 WeatherDuration
	BitFlags<11> m_antiCategory;											// 0x34 AntiCategory
	ObjectFilter m_requirementsFilterMPSkirmish;							// 0x38 RequirementsFilterMPSkirmish
	ObjectFilter m_requirementsFilterStrategic;							// 0x3C RequirementsFilterStrategic
	Bool m_targetEnemy;													// 0x40 TargetEnemy
	Bool m_targetAllSides;												// 0x41 TargetAllSides
	Bool m_reEnableAntiCategory;										// 0x42 ReEnableAntiCategory
	const FXList *m_initiateFX;											// 0x44 InitiateFX
	const FXList *m_triggerFX;											// 0x48 TriggerFX
	const FXList *m_antiFX;												// 0x4C AntiFX
	Int m_setModelCondition;											// 0x50 SetModelCondition
	Real m_setModelConditionTime;										// 0x54 SetModelConditionTime
	Int m_giveLevels;													// 0x58 GiveLevels
	Bool m_disableDuringAnimDuration;									// 0x5C DisableDuringAnimDuration
	Bool m_idleWhenStartingPower;										// 0x5D IdleWhenStartingPower
	Bool m_affectGood;													// 0x5E AffectGood
	Bool m_affectEvil;													// 0x5F AffectEvil
	Bool m_affectAllies;												// 0x60 AffectAllies
	Bool m_availableAtStart;											// 0x61 AvailableAtStart
	Int m_changeWeather;												// 0x64 ChangeWeather
	Bool m_adjustVictim;												// 0x68 AdjustVictim
	AsciiString m_onTriggerRechargeSpecialPower;						// 0x6C OnTriggerRechargeSpecialPower
	UnsignedInt m_burnDecayModifier;									// 0x70 BurnDecayModifier
	Bool m_useDistanceFromCommandCenter;								// 0x74 UseDistanceFromCommandCenter
	Real m_distanceFromCommandCenter;									// 0x78 DistanceFromCommandCenter
};
// SIZE 0x7C

class SpecialPowerModule : public BehaviorModule, public SpecialPowerModuleInterface
{
public:
	SpecialPowerModule(Thing *thing, const ModuleData *moduleData);		// 0x00493C5A

	// BehaviorModuleInterface slot 8 returns the special-power interface at +0x10.
	virtual SpecialPowerModuleInterface *getSpecialPower() { return this; }
	virtual void slot44();										// Dispatches primary slot 12.
	virtual ~SpecialPowerModule();										// 0x00493DEF

	virtual NameKeyType getModuleNameKey() const;						// 0x00493DAA
	virtual void rva00493805();											// primary slot 12
	virtual void ApplyPowerToObject(Object *object, Int arg, const BitFlags<11> *flags);	// primary slot 13, 0x00493EA7

	virtual bool isModuleForPower(const SpecialPowerTemplate *power) const;	// 0x0049323D
	virtual bool isReady() const;										// 0x004934DE
	virtual float getPercentReady() const;								// 0x0049355E
	virtual bool specialPowerSlot03() const { return m_pausedCount > 0; }		// fold 0x00251C08 (`cmp [ecx+0xC],0; setg`)
	virtual unsigned int getReadyFrame() const;							// 0x00493AB7
	virtual AsciiString getPowerName() const;							// 0x004934B8
	virtual void onSpecialPowerCreation();								// 0x004933C2
	virtual void setReadyFrame(unsigned int frame);
	virtual void pauseCountdown(bool pause);							// 0x00492FC2
	virtual void doSpecialPower(unsigned int commandOptions);			// 0x0049490F
	virtual void doSpecialPowerAtObject(Object *object, unsigned int commandOptions);	// 0x0049495B
	virtual void doSpecialPowerAtLocation(const Coord3D *location, unsigned int commandOptions);	// 0x004949D8
	virtual void doSpecialPowerUsingWaypoints(const Coord3D *location, const Waypoint *waypoint, unsigned int commandOptions);	// 0x00494A2A
	virtual void markSpecialPowerTriggered(const Coord3D *location);	// 0x00494591
	// Behavior slot 43 forwards the float to the special-power interface at +0x10.
	virtual void slot43(float readyFraction);
	virtual void startPowerRecharge(float readyFraction);				// 0x0049369D
	virtual Rva002390CB getInitiateSound() const;						// 0x00493218
	virtual void stopSpecialPower() { m_unk0x28 = false; }				// 0x00251C2C (`mov byte [ecx+0x18],0` on the +0x10 interface: retail table 0x0085E080 slot 17; slot 19 holds the same fold)
	virtual bool rva0049466D(const Coord3D *location);					// 0x0049466D
	virtual void specialPowerSlot19() { m_unk0x28 = false; }			// fold 0x00251C2C (`mov byte [ecx+0x18],0` on the +0x10 interface), the body of slot 17 (stopSpecialPower) too: retail table 0x0085E080 holds it at slots 17 and 19; slot 19's own name is not known, publish 3c lists this one at the fold
	virtual unsigned int specialPowerSlot20() const { return m_unk0x2C; }	// fold 0x0030F2C7 (`mov eax,[ecx+0x1C]`)
	virtual bool specialPowerSlot21() const;										// 0x004932D5
	virtual void specialPowerSlot22();												// 0x0049329E
	virtual SpecialPowerModule *specialPowerSlot23() { return this; }	// fold 0x004BF893 (`lea eax,[ecx-0x10]`: the module behind the +0x10 interface), slots 23 and 24 of table 0x0085E080
	virtual void *specialPowerSlot24() { return this; }					// fold 0x004BF893 (`lea eax,[ecx-0x10]`: the module behind the +0x10 interface), shared with ActiveBody::getIface; retail table 0x0085E080 slots 23 and 24

	// Non-virtual helpers of this unit, spelled by their addresses.
	bool rva004932DC();																// 0x004932DC, module data +0x74
	bool rva00493251();																// 0x00493251, the anti-category filter
	bool rva00493620();																// 0x00493620, the template's 0x20 gate
	bool rva00494599(const Coord3D *location);
	void rva0049402B(BfmeWideResult *iter);										// 0x0049402B: applies the power to every object a range query found										// 0x00494599, command-centre range

	const SpecialPowerModuleData *getSpecialPowerModuleData() const { return (const SpecialPowerModuleData *)getModuleData(); }

	// Rowed in this unit: the intent entry 0x004946F3 (540B, ret 0x14) and the
	// trigger 0x004941F3 (926B). The do-special-power entries call both.
	void initiateIntentToDoSpecialPower(const Object *obj, const Coord3D *loc, UnsignedInt commandOptions, const Waypoint *waypoint);
	void triggerSpecialPower(const Coord3D *location);
	void copyReadyStateFrom(SpecialPowerModule *other);	// 0x00493BD4: copies the other module's ready-state words (f11)

protected:
	virtual const SpecialPowerTemplate *getSpecialPowerTemplate() const;	// 0x00493236
	virtual void loadPostProcess();										// 0x00493B1E
	virtual const char *GetSnapshotName() const;						// 0x00493DA4
	virtual void xfer(Xfer *xfer);										// 0x00493009

	void createViewObject(const Coord3D *location);					// 0x00493845
	void aboutToDoSpecialPower(const Coord3D *location);				// 0x004939AB

	UnsignedInt m_unk0x14;												// 0x14, transferred from version 2
	UnsignedInt m_availableOnFrame;										// 0x18
	Int m_pausedCount;													// 0x1C
	UnsignedInt m_pausedOnFrame;										// 0x20
	Real m_pausedPercent;												// 0x24
	Bool m_unk0x28;														// 0x28
	ObjectID m_unk0x2C;													// 0x2C
	Bool m_unk0x30;														// 0x30, transferred from version 3
};
// SIZE 0x34

#endif
