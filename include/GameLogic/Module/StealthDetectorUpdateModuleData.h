// StealthDetectorUpdateModuleData.h: the stealth detector's INI data.
// Target facts: the constructor reads the update rate at +0x08 and the
// initially-disabled flag at +0x10; buildFieldParse 0x004A2D82 registers the
// retail table at 0x00C52188 with offset 0. Member offsets follow that table's
// entries (map types/class_fields.csv, bfp:0x004A2D82).
#pragma once
#ifndef BFME2_STEALTHDETECTORUPDATEMODULEDATA_H
#define BFME2_STEALTHDETECTORUPDATEMODULEDATA_H

#include "Common/AsciiString.h"
#include "Common/OpaqueRefElement4.h"
#include "Common/BitFlags.h"
#include "GameLogic/Module/ModuleData.h"

class MultiIniFieldParse;
class ParticleSystemTemplate;

struct FieldParse;

class StealthDetectorUpdateModuleData : public ModuleData
{
public:
	StealthDetectorUpdateModuleData();
    const OpaqueRefElement4 &soundRef() const { return m_pingSound; }
    const OpaqueRefElement4 &loudRef() const { return m_loudPingSound; }
    const ParticleSystemTemplate *beacon() const { return m_IRBeaconParticleSysTmpl; }
    const ParticleSystemTemplate *ping() const { return m_IRParticleSysTmpl; }
    const ParticleSystemTemplate *bright() const { return m_IRBrightParticleSysTmpl; }
    const ParticleSystemTemplate *grid() const { return m_IRGridParticleSysTmpl; }
    const AsciiString &bone() const { return m_IRParticleSysBone; }
    const KindOfMaskType &require() const { return m_extraDetectKindof; }
    const KindOfMaskType &forbid() const { return m_extraDetectKindofNot; }

	static void buildFieldParse(MultiIniFieldParse &parse);

	// Offsets are the retail field table 0x00C52188 (types/class_fields.csv).
	Int m_updateRate;									// 0x08 DetectionRate
	Real m_detectionRange;								// 0x0C DetectionRange
	Bool m_initiallyDisabled;							// 0x10 InitiallyDisabled
	OpaqueRefElement4 m_pingSound;						// 0x14 PingSound
	OpaqueRefElement4 m_loudPingSound;					// 0x18 LoudPingSound
	const ParticleSystemTemplate *m_IRBeaconParticleSysTmpl;	// 0x1C IRBeaconParticleSysName
	const ParticleSystemTemplate *m_IRParticleSysTmpl;		// 0x20 IRParticleSysName
	const ParticleSystemTemplate *m_IRBrightParticleSysTmpl;	// 0x24 IRBrightParticleSysName
	const ParticleSystemTemplate *m_IRGridParticleSysTmpl;	// 0x28 IRGridParticleSysName
	AsciiString m_IRParticleSysBone;					// 0x2C IRParticleSysBone
	KindOfMaskType m_extraDetectKindof;						// 0x30 ExtraRequiredKindOf
	KindOfMaskType m_extraDetectKindofNot;					// 0x4C ExtraForbiddenKindOf
	Bool m_garrison;									// 0x68 CanDetectWhileGarrisoned
	Bool m_transport;									// 0x69 CanDetectWhileContained
	Bool m_cancelRing;									// 0x6A CancelOneRingEffect
	AsciiString m_upgradeName;							// 0x6C RequiredUpgrade
};

// Retail 0x00C52188; its fields are not yet recovered.
extern const FieldParse StealthDetectorUpdateModuleDataFieldParse[];

#endif
