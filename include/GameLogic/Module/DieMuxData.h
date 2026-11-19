// Target facts: DieMuxData has no base or virtual functions: RVA 0x004CE534 initializes the death-type word at +0, resets two 16-byte masks at +4/+0x14 through RVA 0x0024CA24, and stores floats at +0x24/+0x28/+0x2C from VA 0x00BBB9AC (-1) and 0x00BBB8D8 (1); it never stores a vptr. RVA 0x00256ACC calls it on +8 and starts the next member at +0x38, fixing sizeof at 0x30.
// Target facts: RVA 0x004CE52E returns the FieldParse table at VA 0x00C5FFA0, whose DeathTypes, ExemptStatus, RequiredStatus, DamageAmountRequired, MinKillerAngle and MaxKillerAngle offsets are 0/4/0x14/0x24/0x28/0x2C. RVA 0x004CE588 tests the death word, passes masks to RVA 0x0026157E and reads all three floats; these are nonvirtual methods. RVA 0x00256B94 destroys the enclosing vectors and restores Snapshot's VA 0x00BBB554 without destroying DieMuxData; constructor tables VA 0x00BF3E30/0x00BF3B40 belong to the enclosing module data at +0, not its DieMuxData at +8.
// Names: Zero Hour DieModule.h supplies m_deathTypes, m_exemptStatus and m_requiredStatus; retail's table confirms their roles. RVA 0x004CE534 is also called as the default constructor at RVA 0x00256AEC; the constructor and rowed init, getFieldParse and isDieApplicable are declared out of line. The storage is opaque; retail proves the offsets above without requiring STLport in consumers.
#pragma once
#ifndef __BFME2_DIEMUXDATA_H_
#define __BFME2_DIEMUXDATA_H_

#include "Common/BitFlags.h"
#include "Common/GameType.h"

class Object;
class DamageInfo;
struct FieldParse;

class DieMuxData
{
public:
    DieMuxData();
    DieMuxData *init();
    static const FieldParse *getFieldParse();
    bool isDieApplicable(const Object *obj, const DamageInfo *damageInfo) const;

public:
    UnsignedInt m_deathTypes;				// 0x00 DeathTypes, bit (death type - 1)
    ObjectStatusMaskType m_exemptStatus;	// 0x04 ExemptStatus
    ObjectStatusMaskType m_requiredStatus;	// 0x14 RequiredStatus
    Real m_damageThreshold;				// 0x24 DamageAmountRequired; negative means none
    Real m_angleMin;					// 0x28 MinKillerAngle
    Real m_angleMax;					// 0x2C MaxKillerAngle
};

#endif
