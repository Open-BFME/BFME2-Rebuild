// W3DModelDrawModuleData.h: the INI data of W3DModelDraw. SIZE 0x188.
// Target facts: ModuleFactory registers the module with operator new 0x188 and
// constructor RVA 0x000C8EEF; buildFieldParse RVA 0x000C9240 adds VA 0x00BCB3C8 at +0x0.
// Offsets are the table entries'; each type is what the entry's parse function stores
// (h05 fpgen: INI::parseReal -> Real, parseDurationUnsignedInt -> UnsignedInt, ...);
// names are the INI tokens. Byte gaps are bytes no table entry writes.
// Unresolved (byte storage up to the next member, named after the token):
//   m_extraPublicBone +0x30: INI::parseAsciiStringVectorAppend 0x0002E896
//   m_trackMarks +0x3C: INI::Rva000B6E5A_ParseLowercasedString 0x000B6E5A
//   m_attachToBoneInAnotherModule +0x40: INI::Rva000B6E5A_ParseLowercasedString 0x000B6E5A
//   m_embedPortal +0x11C: W3DModelDrawModuleData::parseEmbedPortal 0x000C39F5
//   m_alphaRefRange +0x13C: INI::parseICoord2D 0x0002F5BA
#pragma once
#ifndef BFME2_W3_D_MODEL_DRAW_MODULE_DATA_H
#define BFME2_W3_D_MODEL_DRAW_MODULE_DATA_H

#include <stddef.h>
#include "Common/GameType.h"
#include "GameLogic/Module/ModuleData.h"
#include "Common/AsciiString.h"
#include "Common/BitFlags.h"

class INI;
class MultiIniFieldParse;

class W3DModelDrawModuleData : public ModuleData
{
public:
	W3DModelDrawModuleData();
	static void buildFieldParse(MultiIniFieldParse &parse);
	static ModuleData *friend_newModuleData(INI *ini);
	static void parseEmbedPortal(INI *ini, void *instance, void *store, const void *userData);
	// slot 14 override: the W3DModelDraw, W3DQuadrupedDraw, W3DSupplyDraw, W3DTankDraw and
	// W3DTruckDraw module-data tables hold 0x000B6E28 here (findBestInfo 0x000B4B23, then the
	// entry's model name), not ModuleData's 0x000B69A1
	virtual AsciiString getBestModelNameForWB(const ModelConditionFlags &c) const;
	// slots 18 and 22 of the same five tables: 0x0047A6A9 (`mov eax,ecx`) and 0x002004A1 (+0x64)
	virtual const W3DModelDrawModuleData *getAsW3DModelDrawModuleData() const { return this; }
	virtual Int getMinimumRequiredGameLOD() const { return m_minLODRequired; }
	// slot 30: 0x0006520B returns AffectedByStealth (+0x15E) where ModuleData returns true
	virtual Bool slot30() const { return m_affectedByStealth; }

	UnsignedByte m_unk0x8[0xC];
	AsciiString m_wadingParticleSys;                       // +0x14 WadingParticleSys
	UnsignedByte m_unk0x18[0x18];
	UnsignedByte m_extraPublicBone[0xC];                   // +0x30 ExtraPublicBone; unresolved: INI::parseAsciiStringVectorAppend
	UnsignedByte m_trackMarks[0x4];                        // +0x3C TrackMarks; unresolved: INI::Rva000B6E5A_ParseLowercasedString
	UnsignedByte m_attachToBoneInAnotherModule[0x4];       // +0x40 AttachToBoneInAnotherModule; unresolved: INI::Rva000B6E5A_ParseLowercasedString
	AsciiString m_trackMarksLeftBone;                      // +0x44 TrackMarksLeftBone
	AsciiString m_trackMarksRightBone;                     // +0x48 TrackMarksRightBone
	UnsignedByte m_unk0x4C[0x4];
	UnsignedInt m_projectileBoneFeedbackEnabledSlots;      // +0x50 ProjectileBoneFeedbackEnabledSlots; bit string
	Real m_initialRecoilSpeed;                             // +0x54 InitialRecoilSpeed; velocity
	Real m_maxRecoilDistance;                              // +0x58 MaxRecoilDistance
	Real m_recoilDamping;                                  // +0x5C RecoilDamping
	Real m_recoilSettleSpeed;                              // +0x60 RecoilSettleSpeed; velocity
	Int m_minLODRequired;                                  // +0x64 MinLODRequired; index into a name list (enum)
	Bool m_okToChangeModelColor;                           // +0x68 OkToChangeModelColor
	Bool m_noRotate;                                       // +0x69 NoRotate
	Bool m_useFiringArcRotation;                           // +0x6A UseFiringArcRotation
	Bool m_animationsRequirePower;                         // +0x6B AnimationsRequirePower
	UnsignedByte m_unk0x6C[0x1C];
	Bool m_randomTextureFixedRandomIndex;                  // +0x88 RandomTextureFixedRandomIndex
	UnsignedByte m_unk0x89[0x2F];
	Bool m_useProducerTexture;                             // +0xB8 UseProducerTexture
	BitFlags<591> m_dependencySharedModelFlags;            // +0xBC DependencySharedModelFlags; BitFlags<591>
	Bool m_particlesAttachedToAnimatedBones;               // +0x108 ParticlesAttachedToAnimatedBones
	AsciiString m_wallBoundsMesh;                          // +0x10C WallBoundsMesh
	AsciiString m_raisedWallMesh;                          // +0x110 RaisedWallMesh
	AsciiString m_rampMesh1;                               // +0x114 RampMesh1
	AsciiString m_rampMesh2;                               // +0x118 RampMesh2
	UnsignedByte m_embedPortal[0xC];                       // +0x11C EmbedPortal; unresolved: W3DModelDrawModuleData::parseEmbedPortal
	Bool m_glowEnabled;                                    // +0x128 GlowEnabled
	Bool m_glowEmissive;                                   // +0x129 GlowEmissive
	Bool m_particleBonesCheckDrawable;                     // +0x12A ParticleBonesCheckDrawable
	Bool m_shadowForceDisable;                             // +0x12B ShadowForceDisable
	Real m_highDetailLODThreshold;                         // +0x12C HighDetailLODThreshold
	Real m_lowDetailLODThreshold;                          // +0x130 LowDetailLODThreshold
	Bool m_switchModelLODMode;                             // +0x134 SwitchModelLODMode
	Bool m_staticModelLODMode;                             // +0x135 StaticModelLODMode
	Bool m_showShadowWhileContained;                       // +0x136 ShowShadowWhileContained
	Bool m_useStandardModelNames;                          // +0x137 UseStandardModelNames
	Bool m_useDefaultAnimation;                            // +0x138 UseDefaultAnimation
	Bool m_alphaRefAnimated;                               // +0x139 AlphaRefAnimated
	UnsignedByte m_unk0x13A[0x2];
	UnsignedByte m_alphaRefRange[0x8];                     // +0x13C AlphaRefRange; unresolved: INI::parseICoord2D
	Real m_alphaCameraFadeOuterRadius;                     // +0x144 AlphaCameraFadeOuterRadius; positive, non-zero
	Real m_alphaCameraFadeInnerRadius;                     // +0x148 AlphaCameraFadeInnerRadius; positive, non-zero
	Real m_alphaCameraAtInnerRadius;                       // +0x14C AlphaCameraAtInnerRadius; percent
	UnsignedInt m_birthFadeTime;                           // +0x150 BirthFadeTime; duration, frames
	Bool m_birthFadeAdditive;                              // +0x154 BirthFadeAdditive
	Int m_staticSortLevelWhileFading;                      // +0x158 StaticSortLevelWhileFading
	Bool m_zWriteDisableOverride;                          // +0x15C ZWriteDisableOverride
	Bool m_multiPlayerOnly;                                // +0x15D MultiPlayerOnly
	Bool m_affectedByStealth;                              // +0x15E AffectedByStealth
	Bool m_highDetailOnly;                                 // +0x15F HighDetailOnly
	UnsignedByte m_unk0x160[0x28];
};
// SIZE 0x188
typedef char W3DModelDrawModuleData_size[(sizeof(W3DModelDrawModuleData) == 0x188) ? 1 : -1];
typedef char W3DModelDrawModuleData_off_m_wadingParticleSys[(offsetof(W3DModelDrawModuleData, m_wadingParticleSys) == 0x14) ? 1 : -1];
typedef char W3DModelDrawModuleData_off_m_extraPublicBone[(offsetof(W3DModelDrawModuleData, m_extraPublicBone) == 0x30) ? 1 : -1];
typedef char W3DModelDrawModuleData_off_m_trackMarks[(offsetof(W3DModelDrawModuleData, m_trackMarks) == 0x3C) ? 1 : -1];
typedef char W3DModelDrawModuleData_off_m_attachToBoneInAnotherModule[(offsetof(W3DModelDrawModuleData, m_attachToBoneInAnotherModule) == 0x40) ? 1 : -1];
typedef char W3DModelDrawModuleData_off_m_trackMarksLeftBone[(offsetof(W3DModelDrawModuleData, m_trackMarksLeftBone) == 0x44) ? 1 : -1];
typedef char W3DModelDrawModuleData_off_m_trackMarksRightBone[(offsetof(W3DModelDrawModuleData, m_trackMarksRightBone) == 0x48) ? 1 : -1];
typedef char W3DModelDrawModuleData_off_m_projectileBoneFeedbackEnabledSlots[(offsetof(W3DModelDrawModuleData, m_projectileBoneFeedbackEnabledSlots) == 0x50) ? 1 : -1];
typedef char W3DModelDrawModuleData_off_m_initialRecoilSpeed[(offsetof(W3DModelDrawModuleData, m_initialRecoilSpeed) == 0x54) ? 1 : -1];
typedef char W3DModelDrawModuleData_off_m_maxRecoilDistance[(offsetof(W3DModelDrawModuleData, m_maxRecoilDistance) == 0x58) ? 1 : -1];
typedef char W3DModelDrawModuleData_off_m_recoilDamping[(offsetof(W3DModelDrawModuleData, m_recoilDamping) == 0x5C) ? 1 : -1];
typedef char W3DModelDrawModuleData_off_m_recoilSettleSpeed[(offsetof(W3DModelDrawModuleData, m_recoilSettleSpeed) == 0x60) ? 1 : -1];
typedef char W3DModelDrawModuleData_off_m_minLODRequired[(offsetof(W3DModelDrawModuleData, m_minLODRequired) == 0x64) ? 1 : -1];
typedef char W3DModelDrawModuleData_off_m_okToChangeModelColor[(offsetof(W3DModelDrawModuleData, m_okToChangeModelColor) == 0x68) ? 1 : -1];
typedef char W3DModelDrawModuleData_off_m_noRotate[(offsetof(W3DModelDrawModuleData, m_noRotate) == 0x69) ? 1 : -1];
typedef char W3DModelDrawModuleData_off_m_useFiringArcRotation[(offsetof(W3DModelDrawModuleData, m_useFiringArcRotation) == 0x6A) ? 1 : -1];
typedef char W3DModelDrawModuleData_off_m_animationsRequirePower[(offsetof(W3DModelDrawModuleData, m_animationsRequirePower) == 0x6B) ? 1 : -1];
typedef char W3DModelDrawModuleData_off_m_randomTextureFixedRandomIndex[(offsetof(W3DModelDrawModuleData, m_randomTextureFixedRandomIndex) == 0x88) ? 1 : -1];
typedef char W3DModelDrawModuleData_off_m_useProducerTexture[(offsetof(W3DModelDrawModuleData, m_useProducerTexture) == 0xB8) ? 1 : -1];
typedef char W3DModelDrawModuleData_off_m_dependencySharedModelFlags[(offsetof(W3DModelDrawModuleData, m_dependencySharedModelFlags) == 0xBC) ? 1 : -1];
typedef char W3DModelDrawModuleData_off_m_particlesAttachedToAnimatedBones[(offsetof(W3DModelDrawModuleData, m_particlesAttachedToAnimatedBones) == 0x108) ? 1 : -1];
typedef char W3DModelDrawModuleData_off_m_wallBoundsMesh[(offsetof(W3DModelDrawModuleData, m_wallBoundsMesh) == 0x10C) ? 1 : -1];
typedef char W3DModelDrawModuleData_off_m_raisedWallMesh[(offsetof(W3DModelDrawModuleData, m_raisedWallMesh) == 0x110) ? 1 : -1];
typedef char W3DModelDrawModuleData_off_m_rampMesh1[(offsetof(W3DModelDrawModuleData, m_rampMesh1) == 0x114) ? 1 : -1];
typedef char W3DModelDrawModuleData_off_m_rampMesh2[(offsetof(W3DModelDrawModuleData, m_rampMesh2) == 0x118) ? 1 : -1];
typedef char W3DModelDrawModuleData_off_m_embedPortal[(offsetof(W3DModelDrawModuleData, m_embedPortal) == 0x11C) ? 1 : -1];
typedef char W3DModelDrawModuleData_off_m_glowEnabled[(offsetof(W3DModelDrawModuleData, m_glowEnabled) == 0x128) ? 1 : -1];
typedef char W3DModelDrawModuleData_off_m_glowEmissive[(offsetof(W3DModelDrawModuleData, m_glowEmissive) == 0x129) ? 1 : -1];
typedef char W3DModelDrawModuleData_off_m_particleBonesCheckDrawable[(offsetof(W3DModelDrawModuleData, m_particleBonesCheckDrawable) == 0x12A) ? 1 : -1];
typedef char W3DModelDrawModuleData_off_m_shadowForceDisable[(offsetof(W3DModelDrawModuleData, m_shadowForceDisable) == 0x12B) ? 1 : -1];
typedef char W3DModelDrawModuleData_off_m_highDetailLODThreshold[(offsetof(W3DModelDrawModuleData, m_highDetailLODThreshold) == 0x12C) ? 1 : -1];
typedef char W3DModelDrawModuleData_off_m_lowDetailLODThreshold[(offsetof(W3DModelDrawModuleData, m_lowDetailLODThreshold) == 0x130) ? 1 : -1];
typedef char W3DModelDrawModuleData_off_m_switchModelLODMode[(offsetof(W3DModelDrawModuleData, m_switchModelLODMode) == 0x134) ? 1 : -1];
typedef char W3DModelDrawModuleData_off_m_staticModelLODMode[(offsetof(W3DModelDrawModuleData, m_staticModelLODMode) == 0x135) ? 1 : -1];
typedef char W3DModelDrawModuleData_off_m_showShadowWhileContained[(offsetof(W3DModelDrawModuleData, m_showShadowWhileContained) == 0x136) ? 1 : -1];
typedef char W3DModelDrawModuleData_off_m_useStandardModelNames[(offsetof(W3DModelDrawModuleData, m_useStandardModelNames) == 0x137) ? 1 : -1];
typedef char W3DModelDrawModuleData_off_m_useDefaultAnimation[(offsetof(W3DModelDrawModuleData, m_useDefaultAnimation) == 0x138) ? 1 : -1];
typedef char W3DModelDrawModuleData_off_m_alphaRefAnimated[(offsetof(W3DModelDrawModuleData, m_alphaRefAnimated) == 0x139) ? 1 : -1];
typedef char W3DModelDrawModuleData_off_m_alphaRefRange[(offsetof(W3DModelDrawModuleData, m_alphaRefRange) == 0x13C) ? 1 : -1];
typedef char W3DModelDrawModuleData_off_m_alphaCameraFadeOuterRadius[(offsetof(W3DModelDrawModuleData, m_alphaCameraFadeOuterRadius) == 0x144) ? 1 : -1];
typedef char W3DModelDrawModuleData_off_m_alphaCameraFadeInnerRadius[(offsetof(W3DModelDrawModuleData, m_alphaCameraFadeInnerRadius) == 0x148) ? 1 : -1];
typedef char W3DModelDrawModuleData_off_m_alphaCameraAtInnerRadius[(offsetof(W3DModelDrawModuleData, m_alphaCameraAtInnerRadius) == 0x14C) ? 1 : -1];
typedef char W3DModelDrawModuleData_off_m_birthFadeTime[(offsetof(W3DModelDrawModuleData, m_birthFadeTime) == 0x150) ? 1 : -1];
typedef char W3DModelDrawModuleData_off_m_birthFadeAdditive[(offsetof(W3DModelDrawModuleData, m_birthFadeAdditive) == 0x154) ? 1 : -1];
typedef char W3DModelDrawModuleData_off_m_staticSortLevelWhileFading[(offsetof(W3DModelDrawModuleData, m_staticSortLevelWhileFading) == 0x158) ? 1 : -1];
typedef char W3DModelDrawModuleData_off_m_zWriteDisableOverride[(offsetof(W3DModelDrawModuleData, m_zWriteDisableOverride) == 0x15C) ? 1 : -1];
typedef char W3DModelDrawModuleData_off_m_multiPlayerOnly[(offsetof(W3DModelDrawModuleData, m_multiPlayerOnly) == 0x15D) ? 1 : -1];
typedef char W3DModelDrawModuleData_off_m_affectedByStealth[(offsetof(W3DModelDrawModuleData, m_affectedByStealth) == 0x15E) ? 1 : -1];
typedef char W3DModelDrawModuleData_off_m_highDetailOnly[(offsetof(W3DModelDrawModuleData, m_highDetailOnly) == 0x15F) ? 1 : -1];

#endif
