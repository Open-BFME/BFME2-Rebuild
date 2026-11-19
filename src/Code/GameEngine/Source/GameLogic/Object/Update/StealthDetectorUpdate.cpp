// Diagnostic pilot import; object boundaries may be unproven.
// FUNCTION: BFME2 0x004A2BD1 size=25 name=??1StealthDetectorUpdate@@UAE@XZ evidence=N:ledger
// FUNCTION: BFME2 0x004A2BEA size=6 name=?name@StealthDetectorUpdate@@QBEPBDXZ evidence=N:ledger
// FUNCTION: BFME2 0x004A2BF0 size=64 name=?allow@Rva004A2BF0@@QAE_NPAVObject@@@Z evidence=?:placeholder
// FUNCTION: BFME2 0x004A2C30 size=68 name=?xfer@StealthDetectorUpdate@@MAEXPAVXfer@@@Z evidence=N:ledger
// FUNCTION: BFME2 0x004A2C74 size=69 name=?getModuleNameKey@StealthDetectorUpdate@@UBE?AW4NameKeyType@@XZ evidence=N:ledger
// FUNCTION: BFME2 0x004A2CB9 size=116 name=??0StealthDetectorUpdate@@QAE@PAVThing@@PBVModuleData@@@Z evidence=N:ledger
// FUNCTION: BFME2 0x004A2D49 size=30 name=?rva004A2D49@Rva004A2D49@@QAEX_N@Z evidence=?:placeholder
// FUNCTION: BFME2 0x004A2D82 size=17 name=?buildFieldParse@StealthDetectorUpdateModuleData@@SAXAAVMultiIniFieldParse@@@Z evidence=N:ledger
// FUNCTION: BFME2 0x004A2D93 size=1579 name=?update@StealthDetectorUpdate@@UAE?AW4UpdateSleepTime@@XZ evidence=N:ledger
// cl: /D_STLP_USE_MALLOC /D_STLP_USE_STATIC_LIB /D_STLP_NO_EXCEPTIONS
// stlport46
#include <list>
// GameLogic/Object/Update/StealthDetectorUpdate.cpp: retail translation unit.
// Bodies are written against the shared headers; see StealthDetectorUpdate.h,
// StealthDetectorUpdateModuleData.h, Rva004A2BF0.h and Rva004A2D49.h.

#include <string.h>

#include "Common/GameType.h"
#include "Common/UpgradeCenter.h"
#include "Common/Player.h"
#include "Common/PlayerList.h"
#include "Common/GameAudio.h"
#include "GameClient/ParticleSys.h"
#include "GameClient/Drawable.h"
#include "GameLogic/FXList.h"
#include "GameLogic/GameLogic.h"
#include "GameLogic/InvisibilityManager.h"
#include "GameLogic/PartitionManager.h"
#include "GameLogic/PartitionFilterRelationship.h"
#include "GameLogic/PartitionFilterAcceptByKindOf.h"
#include "GameLogic/PartitionFilterSameMapStatus.h"
#include "GameLogic/Rva00261513Filter.h"
#include "GameLogic/Module/Rva00373EC6.h"
#include "GameLogic/Module/SpecialDisguiseUpdate.h"
#include "Common/INI.h"
#include "Common/INI/FieldParse.h"
#include "Common/BitFlags.h"
#include <stddef.h>
#include "Common/NameKeyGenerator.h"
#include "Common/Xfer.h"
#include "GameLogic/Module/StealthDetectorUpdate.h"
#include "GameLogic/Module/StealthDetectorUpdateModuleData.h"
#include "GameLogic/Object.h"
#include "GameLogic/Rva004A2BF0.h"
#include "GameLogic/Rva004A2D49.h"

StealthDetectorUpdate::~StealthDetectorUpdate()
{
}

DisabledMaskType StealthDetectorUpdate::getDisabledTypesToProcess() const
{
	return DisabledMaskType(DisabledMaskType::kInit, 3);
}


Bool Rva004A2BF0::allow(Object *obj)
{
	if (obj->rva002943B2(m_player))
		return true;
	Rva004A2BF0Interface *iface = (Rva004A2BF0Interface *)obj->getContain();
	if (iface && iface->s4() && iface->s73())
		return true;
	return false;
}

// Version 3 dropped the enabled flag; older saves still carry it, so it is read
// into a local and discarded.
void StealthDetectorUpdate::xfer(Xfer *xfer)
{
	Xfer::Version version(1, 3);
	*xfer == version;
	UpdateModule::xfer(xfer);
	if (version.m_minimum < 3)
	{
		Bool enabled = false;
		*xfer == enabled;
	}
}

NameKeyType StealthDetectorUpdate::getModuleNameKey() const
{
	static NameKeyType TheStealthDetectorUpdatePoolKey =
		TheNameKeyGenerator->nameToKey("StealthDetectorUpdate");
	return TheStealthDetectorUpdatePoolKey;
}

// Parks the module with a random wake over the data's update rate, or forever
// when the data starts disabled. The roll is the rowed 0x00233FF4, whose file
// argument is this class-name literal and whose line is 79.
Int __cdecl GetGameLogicRandomValue(Int low, Int high, const char *file, Int line);

StealthDetectorUpdate::StealthDetectorUpdate(Thing *thing, const ModuleData *moduleData)
	: UpdateModule(thing, moduleData)
{
	const StealthDetectorUpdateModuleData *data = (const StealthDetectorUpdateModuleData *)getModuleData();
	UnsignedInt wake;
	if (data->m_initiallyDisabled == 0)
		wake = GetGameLogicRandomValue(1, data->m_updateRate, "StealthDetectorUpdate", 79);
	else
		wake = UPDATE_SLEEP_FOREVER;
	setWakeFrame(getObject(), (UpdateSleepTime)wake);
}


void Rva004A2D49::rva004A2D49(Bool enabled)
{
	UpdateSleepTime wakeTime = enabled ? UPDATE_SLEEP_NONE : UPDATE_SLEEP_FOREVER;
	setWakeFrame(m_object, wakeTime);
}

// The INI field table buildFieldParse registers (retail .rdata 0x00852188: sixteen entries and the terminator; tokens,
// parsers and offsets read from game.dat).
const FieldParse StealthDetectorUpdateModuleDataFieldParse[] =
{
	{ "DetectionRate", INI::parseDurationUnsignedInt, 0, offsetof(StealthDetectorUpdateModuleData, m_updateRate) },
	{ "DetectionRange", INI::parseReal, 0, offsetof(StealthDetectorUpdateModuleData, m_detectionRange) },
	{ "InitiallyDisabled", INI::parseBool, 0, offsetof(StealthDetectorUpdateModuleData, m_initiallyDisabled) },
	{ "PingSound", INI::parseAudioEventRTS, 0, offsetof(StealthDetectorUpdateModuleData, m_pingSound) },
	{ "LoudPingSound", INI::parseAudioEventRTS, 0, offsetof(StealthDetectorUpdateModuleData, m_loudPingSound) },
	{ "IRBeaconParticleSysName", INI::parseParticleSystemTemplate, 0, offsetof(StealthDetectorUpdateModuleData, m_IRBeaconParticleSysTmpl) },
	{ "IRParticleSysName", INI::parseParticleSystemTemplate, 0, offsetof(StealthDetectorUpdateModuleData, m_IRParticleSysTmpl) },
	{ "IRBrightParticleSysName", INI::parseParticleSystemTemplate, 0, offsetof(StealthDetectorUpdateModuleData, m_IRBrightParticleSysTmpl) },
	{ "IRGridParticleSysName", INI::parseParticleSystemTemplate, 0, offsetof(StealthDetectorUpdateModuleData, m_IRGridParticleSysTmpl) },
	{ "IRParticleSysBone", INI::parseAsciiString, 0, offsetof(StealthDetectorUpdateModuleData, m_IRParticleSysBone) },
	{ "ExtraRequiredKindOf", BitFlags<218>::parseFromINI, 0, offsetof(StealthDetectorUpdateModuleData, m_extraDetectKindof) },
	{ "ExtraForbiddenKindOf", BitFlags<218>::parseFromINI, 0, offsetof(StealthDetectorUpdateModuleData, m_extraDetectKindofNot) },
	{ "CanDetectWhileGarrisoned", INI::parseBool, 0, offsetof(StealthDetectorUpdateModuleData, m_garrison) },
	{ "CanDetectWhileContained", INI::parseBool, 0, offsetof(StealthDetectorUpdateModuleData, m_transport) },
	{ "CancelOneRingEffect", INI::parseBool, 0, offsetof(StealthDetectorUpdateModuleData, m_cancelRing) },
	{ "RequiredUpgrade", INI::parseAsciiString, 0, offsetof(StealthDetectorUpdateModuleData, m_upgradeName) },
	{ 0, 0, 0, 0 }
};

void StealthDetectorUpdateModuleData::buildFieldParse(MultiIniFieldParse &parse)
{
	parse.add(StealthDetectorUpdateModuleDataFieldParse, 0);
}



UpdateSleepTime StealthDetectorUpdate::update(){
 const StealthDetectorUpdateModuleData *data=(const StealthDetectorUpdateModuleData *)m_moduleData;Object *self=getObject();
 if(self->testStatus((ObjectStatusTypes)0x58))return UPDATE_SLEEP_NONE;
 if(self->isEffectivelyDead())return UPDATE_SLEEP_FOREVER;
 if(self->testStatus((ObjectStatusTypes)2))return UPDATE_SLEEP_NONE;
 if(self->testStatus((ObjectStatusTypes)0x13))return UPDATE_SLEEP_FOREVER;
 Object *containedBy=self->m_containedBy;
 if(containedBy){ContainModuleInterface *contain=containedBy->m_contain;if(contain){if(contain->isGarrisonable()){if(!data->m_garrison)return (UpdateSleepTime)data->m_updateRate;}else if(!data->m_transport)return (UpdateSleepTime)data->m_updateRate;}}
 if(data->m_upgradeName.compare(AsciiString::TheEmptyString)!=0){const UpgradeTemplate *upgrade=TheUpgradeCenter->findUpgrade(data->m_upgradeName);if(!upgrade||!self->hasUpgrade(upgrade))return UPDATE_SLEEP_NONE;}
 float range=self->getVisionRange();if(data->m_detectionRange>0.0f)range=data->m_detectionRange;
 bool foundSomeone=false;
 ObjectIterator iter=ThePartitionManager->iterateObjectsInRange(&self->m_cachedPos,range,0,
  PartitionFilterRelationship(self,3,false).link(PartitionFilterAcceptByKindOf(data->require(),data->forbid()).link(Rva00261513Filter(self,true,range).link(&PartitionFilterSameMapStatus(getObject())))),0);
 for(Object *them=iter.next();them;them=iter.next()){
  if(them->isEffectivelyDead())continue;
  bool forceUnstealth;
  if(them->rva0028F4EF()==2&&!them->testModelConditionState((ModelConditionFlagType)0x12c))forceUnstealth=false;else forceUnstealth=true;
  if(them->testModelConditionState((ModelConditionFlagType)0x12c)){static NameKeyType key_SpecialDisguiseUpdate=TheNameKeyGenerator->nameToKey("SpecialDisguiseUpdate");SpecialDisguiseUpdate *disguise=(SpecialDisguiseUpdate*)them->findUpdateModule(key_SpecialDisguiseUpdate);if(disguise)disguise->rva004B05F5(false);}
  if(forceUnstealth&&!(unsigned char)them->rva002933CD()&&!them->testStatus((ObjectStatusTypes)0x11))forceUnstealth=false;else forceUnstealth=true;
  TheGameLogic->getManager178()->detected(them,(Object*)self,data->m_updateRate+1,2);
  Rva00373EC6 *stealth=(Rva00373EC6*)them->rva0028F4BC();
  if(stealth&&them->testStatus((ObjectStatusTypes)0x12)){
   foundSomeone=true;stealth->markAsDetected(data->m_updateRate+1,2,self,true);
   if(data->m_cancelRing&&((Rva00373EC6*)stealth)->m_31)((Rva00373EC6*)stealth)->rva00374815();
   if(data->grid()){ParticleSystemHandle sys=TheParticleSystemManager->createParticleSystem(data->grid(),true);
    if(sys){Coord3D gridPosition;gridPosition.x=them->m_cachedPos.x;gridPosition.y=them->m_cachedPos.y;gridPosition.z=them->m_cachedPos.z;gridPosition.z=self->m_cachedPos.z+17.0f;gridPosition.x-=((int)gridPosition.x)%12;gridPosition.y-=((int)gridPosition.y)%12;sys.m_system->setPosition(&gridPosition);}}
  }else{ContainModuleInterface *contain=them->m_contain;if(contain&&contain->isGarrisonable()&&contain->getStealthUnitsContained()){
   ContainmentListView view=contain->getList();
   for(_STL::list<Object *>::const_iterator item=view.objects().begin();item!=view.objects().end();++item){Object *rider=*item;Rva00373EC6 *riderStealth=(Rva00373EC6*)rider->rva0028F4BC();if(riderStealth){foundSomeone=true;if(self->getControllingPlayer()!=rider->getControllingPlayer()&&self->getRelationship(rider)!=(Relationship)2){riderStealth->markAsDetected(data->m_updateRate+2,2,self,true);TheGameLogic->getManager178()->detected(rider,(Object*)self,data->m_updateRate+2,2);}}}
  }}
  if(!forceUnstealth){Drawable *draw=((Thing*)them)->getDrawable();if(draw)draw->m_secondMaterialPassOpacity=1.0f;}
 }
 if(data->grid()&&self->getShroudStatusForPlayer(ThePlayerList->m_local->m_playerIndex)<=2){
  Drawable *draw=((Thing*)self)->getDrawable();Coord3D bone;bone.x=-1.66f;bone.y=5.5f;bone.z=15.0f;if(draw)draw->getPristineBonePositions(data->bone().str(),0,&bone,0,1,0);
  const ParticleSystemTemplate *ping=foundSomeone?data->bright():data->ping();
  if(ping){ParticleSystemHandle sys=TheParticleSystemManager->createParticleSystem(ping,true);if(sys){if(draw)sys.m_system->attachToDrawable(draw);else sys.m_system->attachToObject(self);sys.operator->()->setPosition(&bone);}}
  if(data->beacon()){ParticleSystemHandle sys=TheParticleSystemManager->createParticleSystem(data->beacon(),true);if(sys){if(draw)sys.m_system->attachToDrawable(draw);else sys.m_system->attachToObject(self);sys.operator->()->setPosition(&bone);}}
  // Native temporary cleanup conditionally releases the referent; use the
  // shared empty reference holder rather than a string-destructor alias.
  BfmeAudioEventPrefix136 sound(OpaqueRefElement4(),INVALID_ID);if(foundSomeone)sound.assignSound(data->loudRef());else sound.assignSound(data->soundRef());sound.setObjectID(self->m_id);TheAudio->addAudioEvent(&sound);
 }
 // A compiled-out string local reproduces the retail EH state map and
 // saved secondary receiver home. This source shape is a codegen inference.
 if (0) { AsciiString unused; }
 return (UpdateSleepTime)data->m_updateRate;
}

void StealthDetectorUpdate::loadPostProcess()
{
	UpdateModule::loadPostProcess();
}

// 0x004A2BEA
const char *StealthDetectorUpdate::GetSnapshotName() const { return "StealthDetectorUpdate"; }
