// FlammableUpdateModuleData.h: the INI data of the flammable module.
// Target facts (old repo reference/shims/bfme2_flammableupdate/GameLogic/Module/
// FlammableUpdate.h): the own INI table is 0x00C4C600 and the factory
// (0x0024D068) news 0x58 bytes and calls 0x0048D541. The burning-sound holder at
// +0x18 is released by the destructor through Release_Ref 0x00050ED3, and the
// FireFXList vector at +0x28 is torn down by 0x0048D003/0x0048D042.
#pragma once
#ifndef BFME2_FLAMMABLEUPDATEMODULEDATA_H
#define BFME2_FLAMMABLEUPDATEMODULEDATA_H

#include "GameLogic/Module/ModuleData.h"
#include "Common/Vector.h"
#include "Common/OpaqueRefElement4.h"
#include "GameLogic/Module/CustomAnimAndDuration.h"

class MultiIniFieldParse;
class FXList;

// The +0x18 holder: the parse proc (0x00739900) allocates it and the destructor
// Releases it, so it is a reference-counted audio event name rather than an
// AsciiString.
struct BurningSoundHolder
{
	OpaqueRefElement4 m_ref;					// 0x0
};

// One entry of the module data's FireFXList (the INI field of that name, table
// 0x00C4C600 row 7, parse proc 0x0048D65F): `FX = <fxlist>` followed by an
// optional `BONE = <name>`, i.e. the fire effect and the bone it plays at. The
// role name comes from that INI token; retail's class name is unknown. Its
// destructor is what 0x0048CE25's range loop calls. (Not the camera position
// record of GameClient/CameraMarker.h: it carried that name until this change.)
class FireFXEntry
{
public:
	~FireFXEntry();
	FireFXEntry &operator=(const FireFXEntry &);

	const FXList *fx;							// 0x0
	AsciiString boneName;						// 0x4
};
// SIZE 0x8

// STLport's vector cleanup is out of line in retail: 0x0048D003 destroys the
// range and 0x0048D042 is the erase it shares with clear(). The base supplies
// the representation and storage cleanup.
struct FireFXEntryVec : public _STL::_Vector_base<FireFXEntry, _STL::allocator<FireFXEntry> >
{
	FireFXEntryVec(const _STL::allocator<FireFXEntry> &alloc)
		: _STL::_Vector_base<FireFXEntry, _STL::allocator<FireFXEntry> >(alloc) {}
	FireFXEntryVec() : _STL::_Vector_base<FireFXEntry, _STL::allocator<FireFXEntry> >(_STL::allocator<FireFXEntry>()) {}
	~FireFXEntryVec();
	FireFXEntry *erase(FireFXEntry *first, FireFXEntry *last);
	void clear() { erase(_M_start, _M_finish); }
	FireFXEntry *begin() const { return _M_start; }
	FireFXEntry *end() const { return _M_finish; }
};


class FlammableUpdateModuleData : public ModuleData
{
public:
	FlammableUpdateModuleData();
	virtual ~FlammableUpdateModuleData();

	static void buildFieldParse(MultiIniFieldParse &p);

	UnsignedInt m_burnedDelay;					// 0x08
	UnsignedInt m_aflameDuration;				// 0x0C
	UnsignedInt m_aflameDamageDelay;			// 0x10
	Int m_aflameDamageAmount;					// 0x14
	BurningSoundHolder m_burningSoundName;		// 0x18
	Real m_flameDamageLimitData;				// 0x1C
	UnsignedInt m_flameDamageExpirationDelay;	// 0x20
	Int m_damageType;							// 0x24
	FireFXEntryVec m_fireFXList;				// 0x28
	Bool m_setBurnedStatus;						// 0x34
	Bool m_swapModelWhenAflame;					// 0x35
	Bool m_swapModelWhenQuenched;				// 0x36
	Bool m_swapTextureWhenAflame;				// 0x37
	Bool m_swapTextureWhenQuenched;				// 0x38
	Bool m_burnContained;						// 0x39
	Bool m_runToWater;							// 0x3A
	Real m_runToWaterDepth;						// 0x3C
	Real m_runToWaterSearchRadius;				// 0x40
	Real m_runToWaterSearchIncrement;			// 0x44
	Bool m_panicLocomotorWhileAflame;			// 0x48
	CustomAnimAndDuration m_customAnimAndDuration;	// 0x4C
};
// SIZE 0x58

#endif
