// Portions derived from Command & Conquer Generals and Command & Conquer Generals Zero Hour, (c) Electronic Arts Inc., GPL-3.0-or-later; see LICENSE and NOTICE.
// Diagnostic pilot import; object boundaries may be unproven.
// FUNCTION: BFME2 0x0048C50D size=81 name=?calcSleepTime@FlammableUpdate@@IAE?AW4UpdateSleepTime@@XZ evidence=N:ledger
// FUNCTION: BFME2 0x0048C55E size=28 name=?stopBurningSound@FlammableUpdate@@IAEXXZ evidence=N:ledger
// FUNCTION: BFME2 0x0048C57A size=8 name=?wouldIgnite@FlammableUpdate@@QAE_NXZ evidence=N:ledger
// FUNCTION: BFME2 0x0048C582 size=205 name=?xfer@FlammableUpdate@@MAEXPAVXfer@@@Z evidence=N:ledger
// FUNCTION: BFME2 0x0048C64F size=138 name=??0FlammableUpdate@@QAE@PAVThing@@PBVModuleData@@@Z evidence=N:ledger
// FUNCTION: BFME2 0x0048C6D9 size=6 name=?GetSnapshotName@FlammableUpdate@@MBEPBDXZ evidence=N:ledger
// FUNCTION: BFME2 0x0048C6DF size=69 name=?getModuleNameKey@FlammableUpdate@@UBE?AW4NameKeyType@@XZ evidence=N:ledger
// FUNCTION: BFME2 0x0048C724 size=77 name=??1FlammableUpdate@@UAE@XZ evidence=N:ledger
// FUNCTION: BFME2 0x0048C771 size=78 name=?doAflameDamage@FlammableUpdate@@QAEXXZ evidence=N:ledger
// FUNCTION: BFME2 0x0048C7BF size=96 name=?rva0048C7BF@FlammableUpdate@@QAEXXZ evidence=?:placeholder
// FUNCTION: BFME2 0x0048C81F size=45 name=?Rva0048C81FConstruct@@YAXPAUTreeKey00242F5E@@ABU1@@Z evidence=?:placeholder
// FUNCTION: BFME2 0x0048C84C size=38 name=?Rva0048C84CUninitializedCopy@@YAPAUTreeKey00242F5E@@PBU1@0PAU1@ABU__false_type@_STL@@@Z evidence=?:placeholder
// FUNCTION: BFME2 0x0048C8F9 size=467 name=?rva0048C8F9@FlammableUpdate@@QAEXXZ evidence=?:placeholder
// FUNCTION: BFME2 0x0048CACC size=76 name=?rva0048CACC@Rva0048CACC@@QAEXPAURva0048CACCArg@@@Z evidence=?:placeholder
// FUNCTION: BFME2 0x0048CB18 size=781 name=?tryToIgnite@FlammableUpdate@@QAEXXZ evidence=N:ledger
// FUNCTION: BFME2 0x0048CE3E size=453 name=?onDamage@FlammableUpdate@@UAEXPAVDamageInfo@@@Z evidence=N:ledger
// FUNCTION: BFME2 0x0048D003 size=63 name=??1CameraMarkerVec@@QAE@XZ evidence=N:ledger
// FUNCTION: BFME2 0x0048D042 size=51 name=?erase@CameraMarkerVec@@QAEPAVCameraMarker@@PAV2@0@Z evidence=N:ledger
// FUNCTION: BFME2 0x0048D0BE size=977 name=?update@FlammableUpdate@@UAE?AW4UpdateSleepTime@@XZ evidence=N:ledger
// FUNCTION: BFME2 0x0048D541 size=203 name=??0FlammableUpdateModuleData@@QAE@XZ evidence=N:ledger
// FUNCTION: BFME2 0x0048D65F size=203 name=?Rva0048D65FParse@@YAXPAVINI@@PAX1PBX@Z evidence=?:placeholder
// FUNCTION: BFME2 0x0048D72A size=17 name=?buildFieldParse@FlammableUpdateModuleData@@SAXAAVMultiIniFieldParse@@@Z evidence=N:ledger
// cl: /EHsc- /D_CRTIMP=
// stlport46
// The module data's destructor chain collapses to a bare vptr store
// (0x0048D075 ends in `mov [this],0x00BBB554` with no call).
#include <vector>
namespace FlammableCRT { extern "C" __declspec(dllimport) int __cdecl _strcmpi(const char *,const char *); }
#include <new>
#include <stl/_algobase.h>
#include "GameLogic/Module/FlammableUpdate.h"
#include "GameLogic/Module/FlammableUpdateModuleData.h"
#include "GameLogic/Object.h"
#include "GameLogic/GameLogic.h"
#include "GameLogic/Damage.h"
#include "Common/NameKeyGenerator.h"
#include "Common/Xfer.h"
#include "Common/INI.h"
#include "Common/INIException.h"
#include "Common/Rva0048C8C6.h"
#include "Common/FireFX.h"
#include "GameLogic/Module/FlammableUpdateVectorRecords.h"
namespace _STL { template<> Rva0048D48FRecord *allocator<Rva0048D48FRecord>::allocate(size_type,const void *); }
#include "GameLogic/Module/BodyModule.h"
#include "GameLogic/Module/AIUpdate.h"
#include "GameLogic/AICommandInterface.h"
#include "GameLogic/LuaScriptEngine.h"
#include "GameLogic/Rva002918E0Object.h"
#include "GameLogic/Module/FireSpreadUpdate.h"
#include "GameLogic/FXList.h"
#include "GameLogic/ContainModuleSlots.h"
#include "GameLogic/FlammableObjectView.h"
#include "GameLogic/TerrainLogicSlots.h"
#include "GameLogic/TerrainLogic.h"
#include "GameLogic/AI.h"
#include "GameLogic/AIPathfind.h"
#include "GameClient/Drawable.h"

// BFME 2 keeps the logic frame rate in a global (0x009BA4E4).
extern int BfmeLogicFramesPerSecond;

// The module data's INI table (retail 0x00C4C600, its row in another unit).
extern const FieldParse FlammableUpdateFields[];

// The range-assign wrapper erase shares with the other FireFXEntry vectors
// (retail 0x00288A5C).
FireFXEntry *Rva00288A5CAssign(FireFXEntry *first, FireFXEntry *last, FireFXEntry *dest, void *unused);

// Status transfers are rowed out of line (retail 0x00305E72).
void XferFlammabilityStatusType(Xfer *xfer, int *value);

// ---- STLport rows -----------------------------------------------------------
// Retail's GameEngine builds against STLport, so the container operations this
// unit uses are the vendored templates rather than stand-ins. `// stlport46`
// puts them ahead of the VC headers; each operation retail keeps out of line is
// then instantiated (or specialized) here so the copy carries its own name.

inline FireFXEntry::~FireFXEntry() {}

class FXList;

void Rva0048C81FConstruct(TreeKey00242F5E *dest, const TreeKey00242F5E &source);

namespace _STL
{
// The three records are the same eight-byte shape, so their lifetime hooks are
// the rowed FireFX ones; keeping the specializations in this unit stops a
// second copy of each from being emitted.
template <>
void _Construct<Rva0048C872Element, Rva0048C872Element>(Rva0048C872Element *dest, const Rva0048C872Element &value)
{
	Rva0048C81FConstruct((TreeKey00242F5E *)dest, *(const TreeKey00242F5E *)&value);
}

template <>
void _Construct<Rva0048D48FRecord, Rva0048D48FRecord>(Rva0048D48FRecord *dest, const Rva0048D48FRecord &value)
{
	Rva0048C81FConstruct((TreeKey00242F5E *)dest, *(const TreeKey00242F5E *)&value);
}

template <> void _Destroy<FireFXEntry *>(FireFXEntry *first, FireFXEntry *last)
{
	for (; first != last; ++first)
		first->~FireFXEntry();
}

template <>
void _Destroy<Rva0048D48FRecord *>(Rva0048D48FRecord *first, Rva0048D48FRecord *last)
{
	_Destroy((FireFXEntry *)first, (FireFXEntry *)last);
}
}

// BFME 2's bit numbers of the statuses and model conditions these bodies use;
// Zero Hour's enumerators of the same names have other values.
static const ObjectStatusTypes OBJECT_STATUS_STATUS_3 = (ObjectStatusTypes)3;
static const ObjectStatusTypes OBJECT_STATUS_STATUS_5 = (ObjectStatusTypes)5;
static const ObjectStatusTypes BFME_OBJECT_STATUS_AFLAME = (ObjectStatusTypes)10;
static const ObjectStatusTypes BFME_OBJECT_STATUS_BURNED = (ObjectStatusTypes)11;
static const ObjectStatusTypes OBJECT_STATUS_STATUS_82 = (ObjectStatusTypes)82;
enum
{
	BFME_MODELCONDITION_AFLAME = 2 * 32 + 14,
	BFME_MODELCONDITION_SMOLDERING = 2 * 32 + 16,
	BFME_MODELCONDITION_FLAG_190 = 5 * 32 + 30,
	BFME_MODELCONDITION_FLAG_191 = 5 * 32 + 31
};
static const DisabledType DISABLED_TYPE_4 = (DisabledType)4;

// GameLogic/Object/Update/FlammableUpdate.cpp: retail translation unit (pilot skeleton).
// Write each function below its FUNCTION annotation, in this (retail) order.
// Bodies matched in the old repo (read-only reference) are listed per function as old=<source>.
// Shared types belong in include/; no class or struct with a body in this file.
// cl: /D_STLP_USE_STATIC_LIB /D_STLP_USE_MALLOC /D_STLP_NO_EXCEPTIONS


UpdateSleepTime FlammableUpdate::calcSleepTime()
{
	if (m_fleeing)
		return UPDATE_SLEEP_NONE;
	UnsignedInt now = TheGameLogic->getFrame();
	if (m_status == FS_AFLAME && m_aflameEndFrame != 0 && m_aflameEndFrame > now)
	{
		UnsignedInt soonest = m_aflameEndFrame;
		if (m_burnedEndFrame != 0 && m_burnedEndFrame < soonest && m_burnedEndFrame > now) soonest = m_burnedEndFrame;
		if (m_damageEndFrame != 0 && m_damageEndFrame < soonest && m_damageEndFrame > now) soonest = m_damageEndFrame;
		// UPDATE_SLEEP requires a count-of-frames, not an absolute frame, so subtract 'now'
		return UPDATE_SLEEP(soonest - now);
	}
	return UPDATE_SLEEP_FOREVER;
}


void FlammableUpdate::stopBurningSound()
{
	if (m_audioHandle)
	{
		TheAudio->removeAudioEvent(m_audioHandle);
		m_audioHandle = 0;
	}
}


Bool FlammableUpdate::wouldIgnite()
{
	if (m_status == FS_NORMAL)
		return true;

	return false;
}


void FlammableUpdate::xfer(Xfer *xfer)
{
	UpdateModule::xfer(xfer);
	if (xfer->IsLightCRC())
		return;
	Xfer::Version version(1, 3);
	*xfer == version;
	XferFlammabilityStatusType(xfer, (int *)&m_status);
	*xfer == m_aflameEndFrame;
	*xfer == m_burnedEndFrame;
	*xfer == m_damageEndFrame;
	*xfer == m_flameDamageLimit;
	*xfer == m_lastFlameDamageDealt;
	*xfer == m_specialConditionEndFrame;
	TheAudio->xferAudioHandle(xfer, &m_audioHandle);
	*xfer == m_fleeing;
	if (version.m_minimum >= 2)
		XferObjectID(xfer, &m_lastIgniterID);
	if (version.m_minimum >= 3)
		*xfer == m_flag4C;
}


FlammableUpdate::FlammableUpdate(Thing *thing, const ModuleData *moduleData)
	: UpdateModule(thing, moduleData)
	, m_status(FS_NORMAL)
	, m_aflameEndFrame(0)
	, m_burnedEndFrame(0)
	, m_damageEndFrame(0)
	, m_audioHandle(0)
	, m_lastFlameDamageDealt(0)
	, m_fleeing(false)
	, m_specialConditionEndFrame(0)
	, m_lastIgniterID(INVALID_ID)
	, m_flag4C(false)
{
	m_flameDamageLimit = getFlammableUpdateModuleData()->m_flameDamageLimitData;
	setWakeFrame(getObject(), UPDATE_SLEEP_FOREVER);
}


const char *FlammableUpdate::GetSnapshotName() const
{
	return "FlammableUpdate";
}


NameKeyType FlammableUpdate::getModuleNameKey() const
{
	static NameKeyType TheFlammableUpdatePoolKey = TheNameKeyGenerator->nameToKey("FlammableUpdate");
	return TheFlammableUpdatePoolKey;
}


FlammableUpdate::~FlammableUpdate()
{
	stopBurningSound();
}


void FlammableUpdate::doAflameDamage()
{
	Object *obj = getObject();
	const FlammableUpdateModuleData *moduleData = getFlammableUpdateModuleData();
	Rva00263895Member damageRecord;
	DamageInfo *damageInfo = (DamageInfo *)&damageRecord;
	Real amount = (Real)moduleData->m_aflameDamageAmount;
	damageInfo->in.m_sourceID = m_lastIgniterID;
	damageInfo->in.m_amount = amount;
	damageInfo->in.m_damageType = DAMAGE_FLAME;
	damageInfo->in.m_deathType = DEATH_BURNED;
	damageInfo->in.m_damageSubType = (DamageSubType)2;
	obj->attemptDamage(damageInfo);
}


void FlammableUpdate::rva0048C7BF()
{
	const FlammableUpdateModuleData *moduleData = getFlammableUpdateModuleData();
	Object *obj = getObject();
	BfmeAudioEventPrefix136 evt(moduleData->m_burningSoundName.m_ref, obj->getID());
	m_audioHandle = TheAudio->addAudioEvent(&evt);
}


void Rva0048C81FConstruct(TreeKey00242F5E *dest, const TreeKey00242F5E &source)
{
	new (dest) TreeKey00242F5E(source);
}


TreeKey00242F5E *Rva0048C84CUninitializedCopy(const TreeKey00242F5E *first, const TreeKey00242F5E *last,
	TreeKey00242F5E *result, const _STL::__false_type &)
{
	TreeKey00242F5E *cur = result;
	for (; first != last; ++first, ++cur)
		Rva0048C81FConstruct(cur, *first);
	return cur;
}


// The fill operation over the eight-byte record is not instantiated yet: the
// vendored __uninitialized_fill_n does not reproduce retail's 37 bytes, so the
// element type's lifetime hook still needs its own spelling.


template FireFXEntry *_STL::__copy<FireFXEntry *, FireFXEntry *, int>(
	FireFXEntry *, FireFXEntry *, FireFXEntry *, const _STL::random_access_iterator_tag &, int *);


Rva0048C8C6::Rva0048C8C6() : m_00(0), m_04("")
{
}



// Ends a burn: the status becomes BURNED or NORMAL from the object's BURNED bit,
// statuses 3/5 and AFLAME are cleared, an optional SMOLDERING mark is set, the
// EntEnragedUpdate module is reset, the BURNED Lua event is dispatched, the
// flee flag is dropped through the AI module's slot 142, and the body's aflame
// state is refreshed.
void FlammableUpdate::rva0048C8F9()
{
	Object *me = getObject();
	m_status = me->testStatus(BFME_OBJECT_STATUS_BURNED) ? FS_BURNED : FS_NORMAL;
	const FlammableUpdateModuleData *data = getFlammableUpdateModuleData();
	m_aflameEndFrame = 1;
	setWakeFrame(me, UPDATE_SLEEP_NONE);
	me->setStatus(OBJECT_STATUS_STATUS_3, false);
	me->setStatus(OBJECT_STATUS_STATUS_5, false);
	if (data->m_setBurnedStatus)
	{
		me->setStatus(BFME_OBJECT_STATUS_BURNED, true);
		me->setModelConditionState((ModelConditionFlagType)BFME_MODELCONDITION_SMOLDERING);
	}
	me->rva0028EBDA(false);

	static const NameKeyType key_EntEnragedUpdate = TheNameKeyGenerator->nameToKey("EntEnragedUpdate");
	Rva002918E0Object *enraged = (Rva002918E0Object *)me->findUpdateModule(key_EntEnragedUpdate);
	if (enraged)
	{
		enraged->rva004B239A(0, 0);
		enraged->set(0);
	}

	if (data->m_swapModelWhenQuenched)
		me->clearModelConditionState((ModelConditionFlagType)BFME_MODELCONDITION_FLAG_190);
	if (data->m_swapTextureWhenQuenched)
		me->clearModelConditionState((ModelConditionFlagType)BFME_MODELCONDITION_FLAG_191);
	if (data->m_swapModelWhenQuenched || data->m_swapTextureWhenQuenched)
		me->getDrawable()->rva00274176(true);

	DelayedLuaEventList list;
	TheLuaScriptEngine->DispatchEvent((Int)BFME_OBJECT_STATUS_BURNED, me, &list);

	stopBurningSound();
	me->setStatus(BFME_OBJECT_STATUS_AFLAME, false);
	me->clearModelConditionState((ModelConditionFlagType)BFME_MODELCONDITION_AFLAME);
	if (data->m_panicLocomotorWhileAflame && m_flag4C)
	{
		AIUpdateInterface *ai = me->getAI();
		if (ai)
		{
			ai->m_flag3C9 = false;
			me->getAI()->chooseLocomotorSet(0);
			m_flag4C = false;
		}
	}
	me->getBodyModule()->updateBodyParticleSystems();
}


// Healing while aflame: once the accumulated limit is below the module data's
// limit it grows by the healed amount, and crossing the limit ends the burn.
void FlammableUpdate::onHealing(DamageInfo *damageInfo)
{
	Object *obj = getObject();
	if (obj->testStatus(BFME_OBJECT_STATUS_AFLAME) == 0)
		return;
	const FlammableUpdateModuleData *data = getFlammableUpdateModuleData();
	if (data->m_flameDamageLimitData > m_flameDamageLimit)
	{
		m_flameDamageLimit += damageInfo->in.m_amount;
		if (m_flameDamageLimit >= data->m_flameDamageLimitData)
		{
			rva0048C8F9();
			*(UnsignedInt *)&m_flameDamageLimit = *(const UnsignedInt *)&data->m_flameDamageLimitData;
		}
	}
}


// Lights the object: the AFLAME status and model condition, the burning sound,
// fire spreading to neighbours, the AFLAME Lua event, the three end frames, the
// swap model conditions with a drawable refresh, the module data's fire effects
// played at their bone transform or on the object, and the custom animation
// with its disable timer and status 82.
void FlammableUpdate::tryToIgnite()
{
	if (m_status == FS_NORMAL)
	{
		Object *me = getObject();
		me->setStatus(BFME_OBJECT_STATUS_AFLAME, true);
		me->getBodyModule()->updateBodyParticleSystems();
		me->setModelConditionState((ModelConditionFlagType)BFME_MODELCONDITION_AFLAME);
		rva0048C7BF();

		static const NameKeyType key_FireSpreadUpdate = TheNameKeyGenerator->nameToKey("FireSpreadUpdate");
		FireSpreadUpdate *fu = (FireSpreadUpdate *)getObject()->findUpdateModule(key_FireSpreadUpdate);
		if (fu != 0)
			fu->startFireSpreading();

		m_status = FS_AFLAME;

		DelayedLuaEventList list;
		TheLuaScriptEngine->DispatchEvent((Int)BFME_OBJECT_STATUS_AFLAME, me, &list);

		const FlammableUpdateModuleData *data = getFlammableUpdateModuleData();
		UnsignedInt now = TheGameLogic->getFrame();
		if (data->m_aflameDuration > 0)
			m_aflameEndFrame = now + data->m_aflameDuration;
		else
			m_aflameEndFrame = 0x3fffffff;
		m_burnedEndFrame = data->m_burnedDelay ? now + data->m_burnedDelay : 0;
		m_damageEndFrame = data->m_aflameDamageDelay ? now + data->m_aflameDamageDelay : 0;

		setWakeFrame(getObject(), calcSleepTime());

		if (data->m_swapModelWhenAflame)
			me->setModelConditionState((ModelConditionFlagType)BFME_MODELCONDITION_FLAG_190);
		if (data->m_swapTextureWhenAflame)
			me->setModelConditionState((ModelConditionFlagType)BFME_MODELCONDITION_FLAG_191);
		if (data->m_swapModelWhenAflame || data->m_swapTextureWhenAflame)
			me->getDrawable()->rva00274176(true);

		for (FireFXEntry *it = data->m_fireFXList.begin(); it != data->m_fireFXList.end(); )
		{
			FireFXEntry info = *it;
			if (!info.boneName.isEmpty() && getObject()->getDrawable())
			{
				Matrix3D mtx(true);
				getObject()->getDrawable()->getCurrentWorldspaceClientBonePositions(info.boneName.str(), mtx);
				Coord3D pos;
				pos.x = mtx.Row[0][3];
				pos.y = mtx.Row[1][3];
				pos.z = mtx.Row[2][3];
				FXList::doFXPos(info.fx, &pos, &mtx);
			}
			else
			{
				FXList::doFXObj(info.fx, getObject(), 0);
			}
			++it;
		}

		if (data->m_customAnimAndDuration.m_frames > 0)
		{
			me->setSpecialModelConditionState((ModelConditionFlagType)data->m_customAnimAndDuration.m_animType,
				data->m_customAnimAndDuration.m_frames);
			me->setDisabledUntil(DISABLED_TYPE_4,
				TheGameLogic->getFrame() + data->m_customAnimAndDuration.m_frames - 1);
			m_specialConditionEndFrame = TheGameLogic->getFrame() + data->m_customAnimAndDuration.m_frames;
			me->setStatus(OBJECT_STATUS_STATUS_82, true);
		}
		else
		{
			m_specialConditionEndFrame = TheGameLogic->getFrame();
		}
		setWakeFrame(getObject(), UPDATE_SLEEP_NONE);
	}
}


namespace _STL
{
// Defined above, ahead of the records that share its shape.
}


// Remembers the igniter, refreshes the flame damage limit after the expiration
// delay, and ignites once the limit is used up. A damage-type filter and an
// underwater test gate it; on ignition with burnContained set, the contained
// objects each take half the aflame amount, otherwise the contain module runs
// with mode 2.
void FlammableUpdate::onDamage(DamageInfo *damageInfo)
{
	if (damageInfo->out.m_actualDamageClipped > 0.0f)
		m_lastIgniterID = damageInfo->in.m_sourceID;

	Object *me = getObject();
	FlammableObjectView *view = (FlammableObjectView *)me;
	Bool underwater = !(((const ThingTemplateKind *)view->m_template)->m_kindOf & 0x80000000)
		&& ((TerrainLogicSlots *)TheTerrainLogic)->isUnderwater(view->m_pos.x, view->m_pos.y);

	const FlammableUpdateModuleData *data = getFlammableUpdateModuleData();
	if ((data->m_damageType == 0 || data->m_damageType == damageInfo->in.m_damageType) && !underwater)
	{
		Int now = TheGameLogic->getFrame();
		Int since = now - (Int)data->m_flameDamageExpirationDelay;
		if (since > (Int)m_lastFlameDamageDealt)
			m_flameDamageLimit = data->m_flameDamageLimitData;
		m_lastFlameDamageDealt = now;

		if (!me->testStatus(BFME_OBJECT_STATUS_AFLAME) && !me->testStatus(BFME_OBJECT_STATUS_BURNED))
		{
			m_flameDamageLimit -= damageInfo->out.m_actualDamageDealt;
			if (m_flameDamageLimit <= 0.0f)
			{
				m_flameDamageLimit = 0.0f;
				tryToIgnite();
				if (data->m_burnContained && view->m_contain)
				{
					Rva00263895Member damageRecord;
					DamageInfo &info = *(DamageInfo *)&damageRecord;
					info.in.m_amount = (Real)_STL::max(data->m_aflameDamageAmount / 2, 1);
					info.in.m_sourceID = me->getID();
					info.in.m_damageType = DAMAGE_FLAME;
					info.in.m_deathType = DEATH_BURNED;
					ContainmentList contained = view->m_contain->rva70().rva0036AE51();
					for (ContainmentList::iterator it = contained.begin(); it != contained.end(); )
					{
						Object *obj = *it;
						++it;
						if (obj)
							obj->attemptDamage(&info);
					}
				}
				else
				{
					ContainModuleSlots *contain = view->m_contain;
					if (contain)
						contain->rva34(2);
				}
			}
		}
	}
}


FireFXEntryVec::~FireFXEntryVec()
{
	_STL::_Destroy(_M_start, _M_finish);
}


FireFXEntry *FireFXEntryVec::erase(FireFXEntry *first, FireFXEntry *last)
{
	FireFXEntry *newFinish = _STL::__copy_ptrs(last, _M_finish, first, _STL::__false_type());
	_STL::_Destroy(newFinish, _M_finish);
	_M_finish = newFinish;
	return first;
}


FlammableUpdateModuleData::~FlammableUpdateModuleData()
{
}


// Expires the damage, burned and aflame timers and sleeps through
// calcSleepTime. When the special-condition timer runs out and the module data
// asks it, the object scans a grid around itself for the closest water deep
// enough and reachable by the pathfinder, then orders the move, deselects, sets
// statuses 3 and 5 and notifies an EntEnragedUpdate module; while underwater
// and not moving, the aflame end is clamped to three seconds.
UpdateSleepTime FlammableUpdate::update()
{
	Object *me = getObject();
	FlammableObjectView *view = (FlammableObjectView *)me;
	UnsignedInt now = TheGameLogic->getFrame();
	const FlammableUpdateModuleData *data = getFlammableUpdateModuleData();

	if (m_specialConditionEndFrame > 0)
	{
		if (now >= m_specialConditionEndFrame)
		{
			m_specialConditionEndFrame = 0;
			if (data->m_runToWater)
			{
				const Coord3D *pos = &view->m_pos;
				Coord3D best;
				best.x = 10000000.0f;
				best.y = 10000000.0f;
				best.z = 10000000.0f;
				AIUpdateInterface *ai = (AIUpdateInterface *)view->m_ai;
				if (ai)
				{
					Real radius = data->m_runToWaterSearchRadius;
					Real step = data->m_runToWaterSearchIncrement;
					for (Real x = (Real)(Int)(pos->x - radius); pos->x + radius >= x; x = (Real)(Int)(x + step))
					{
						// A second pointer for the row bounds: retail loads row->y
						// before adding the radius, which the shared pos pointer
						// does not give.
						const Coord3D *row = pos;
						for (Real y = (Real)(Int)(row->y - radius); row->y + radius >= y; y = (Real)(Int)(y + step))
						{
							if (((TerrainLogicSlots *)TheTerrainLogic)->Rva0027D815(x, y) > getFlammableUpdateModuleData()->m_runToWaterDepth)
							{
								Coord3D cand;
								cand.z = pos->z;
								cand.x = x;
								cand.y = y;
								Real candDX = pos->x - cand.x;
								Real candDY = pos->y - cand.y;
								Real bestDX = pos->x - best.x;
								Real bestDY = pos->y - best.y;
								if (candDX * candDX + candDY * candDY < bestDX * bestDX + bestDY * bestDY
									&& TheAI->pathfinder()->QuickDoesPathExist(me, pos, &cand, 0))
									best = cand;
							}
						}
					}

					static const NameKeyType key_EntEnragedUpdate = TheNameKeyGenerator->nameToKey("EntEnragedUpdate");
					Rva002918E0Object *enraged = (Rva002918E0Object *)me->findUpdateModule(key_EntEnragedUpdate);
					if (((TerrainLogicSlots *)TheTerrainLogic)->isUnderwater(best.x, best.y))
					{
						if (data->m_panicLocomotorWhileAflame)
						{
							AIUpdateInterface *aIUpdate = (AIUpdateInterface *)view->m_ai;
							if (aIUpdate)
							{
								aIUpdate->chooseLocomotorSet(4);
								((AIUpdateInterface *)view->m_ai)->m_flag3C9 = true;
								m_flag4C = true;
							}
						}
						ai->getAICommandInterface()->aiMoveToPosition(&best, (CommandSourceType)1);
						m_fleeing = true;
						TheGameLogic->deselectObject(me, (UnsignedInt)0xFFFFF, true);
						me->setStatus(OBJECT_STATUS_STATUS_3, true);
						me->setStatus(OBJECT_STATUS_STATUS_5, true);
						if (enraged)
							enraged->set(1);
					}
					else if (enraged)
					{
						enraged->rva004B239A(1, 0);
					}
				}
			}
		}
		return UPDATE_SLEEP_NONE;
	}

	if (!(((const ThingTemplateKind *)view->m_template)->m_kindOf & 0x80000000)
		&& ((TerrainLogicSlots *)TheTerrainLogic)->isUnderwater(view->m_pos.x, view->m_pos.y))
	{
		AIUpdateInterface *ai = (AIUpdateInterface *)view->m_ai;
		if (ai && !ai->isMoving())
		{
			m_aflameEndFrame = _STL::min((UnsignedInt)(BfmeLogicFramesPerSecond * 3 + TheGameLogic->getFrame()), m_aflameEndFrame);
			m_fleeing = false;
		}
	}

	if (m_damageEndFrame != 0 && now >= m_damageEndFrame)
	{
		m_damageEndFrame = now + data->m_aflameDamageDelay;
		doAflameDamage();
	}
	if (m_burnedEndFrame != 0 && now >= m_burnedEndFrame && data->m_setBurnedStatus)
	{
		me->setStatus(BFME_OBJECT_STATUS_BURNED, true);
		me->setModelConditionState((ModelConditionFlagType)BFME_MODELCONDITION_SMOLDERING);
	}
	if (m_aflameEndFrame != 0 && now >= m_aflameEndFrame)
		rva0048C8F9();
	return calcSleepTime();
}


template void _STL::vector<Rva0048D48FRecord>::_M_insert_overflow(
	Rva0048D48FRecord *, const Rva0048D48FRecord &, const _STL::__false_type &, unsigned int, bool);


FlammableUpdateModuleData::FlammableUpdateModuleData()
	: m_burnedDelay(0)
	, m_aflameDuration(0)
	, m_aflameDamageDelay(0)
	, m_aflameDamageAmount(0)
	, m_flameDamageLimitData(20.0f)
	, m_flameDamageExpirationDelay(BfmeLogicFramesPerSecond * 5)
	, m_damageType(6)
	, m_fireFXList(_STL::allocator<FireFXEntry>())
{
	m_runToWaterDepth = 0.0f;
	m_runToWaterSearchRadius = 200.0f;
	m_setBurnedStatus = true;
	m_swapModelWhenAflame = false;
	m_swapModelWhenQuenched = false;
	m_swapTextureWhenAflame = false;
	m_swapTextureWhenQuenched = false;
	m_burnContained = false;
	m_runToWater = false;
	m_runToWaterSearchIncrement = 60.0f;
	m_panicLocomotorWhileAflame = false;
	m_customAnimAndDuration.reset();
	m_fireFXList.clear();
	m_customAnimAndDuration.reset();
}



template void _STL::vector<Rva0048D628Record>::push_back(const Rva0048D628Record &);


// One fire-effect entry from the module data's INI block: an FX name and an
// optional bone, appended to the instance's vector at +0x28.
void Rva0048D65FParse(INI *ini, void *instance, void *, const void *)
{
	Rva0048C8C6 entry;
	const char *token = ini->getNextToken(ini->m_sepsColon);
	if (FlammableCRT::_strcmpi(token, "FX") != 0)
		throw INIException(3, "'fx' expected");

	INI::parseFXList(ini, instance, &entry.m_00, NULL);

	token = ini->getNextTokenOrNull(ini->m_sepsColon);
	if (token && FlammableCRT::_strcmpi(token, "BONE") == 0)
		entry.m_04.set(ini->getNextTokenOrNull(NULL));

	((Rva0048D65FOwner *)instance)->m_fx.push_back((const Rva0048D628Record &)entry);
}


void FlammableUpdateModuleData::buildFieldParse(MultiIniFieldParse &parse)
{
	parse.add(FlammableUpdateFields, 0);
}







