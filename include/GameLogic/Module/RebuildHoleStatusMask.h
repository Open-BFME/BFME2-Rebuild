// RebuildHoleStatusMask.h: the map's placeholder classes for the status mask Object::rva0028CDEB takes.
// Target facts: Rva00346BC0 is retail's BitFlags<101> (ObjectStatusMaskType; class_identities.csv, strong): four words.
// Rva00391F4E is the three-integer constructor 0x00391F4E that RebuildHoleBehavior::newWorkerRespawnProcess 0x0048337A
// calls to build the mask for the status setter. Both names are the map's; they become BitFlags<101> when it is renamed.
#pragma once
#ifndef BFME2_REBUILDHOLESTATUSMASK_H
#define BFME2_REBUILDHOLESTATUSMASK_H

class Rva00346BC0
{
public:
	Rva00346BC0() {}
	Rva00346BC0(int unused, int first, int second, int third, int fourth);	// 0x00346BC0: DozerAIUpdate.cpp 0x00489256 builds the mask 0x3C, 3, 0x4F, 0x63 with it, then 0x0028CDEB
	// 0x0044EAF3 (map placeholder ?rva0044EAF3@Rva0044EAF3@@QAEPAV1@HIII@Z, out of line, returns this): a mask holding three statuses;
	// SpecialAbilityUpdate::onExit 0x004502CE builds (0, 0x18, 0x46, 0x17) with it for the status clearer 0x0028CDEB (f11).
	Rva00346BC0(int unused, int first, int second, int third);
	// 0x0023DA79 (map placeholder ?rva0023DA79@Rva0023DA79@@QAEPAV1@HH@Z, out of line, returns this): a mask holding one status;
	// Object::setStatus 0x0023DB0E builds (0, bit) with it for the status setter 0x0028CDEB (f11).
	Rva00346BC0(int unused, int bit);
	unsigned m_bits[4];
};

class Rva00391F4E : public Rva00346BC0
{
public:
	Rva00391F4E(int first, int second, int third);	// 0x00391F4E
};

#endif
