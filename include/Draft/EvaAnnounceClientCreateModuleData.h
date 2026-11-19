// DRAFT (h05 fpgen): generated from retail's field-parse table (VA 0x00C5ECB8).
// Offsets and size are compile-checked below; no function matches against it yet,
// so it stays in Draft until one does. Unresolved members: m_announcementEventEnemy, m_announcementEventAlly, m_announcementEventOwner.
// EvaAnnounceClientCreateModuleData.h: the INI data of EvaAnnounceClientCreate. SIZE 0x1C.
// Target facts: ModuleFactory registers the module with operator new 0x1C and
// constructor RVA 0x004C999D; buildFieldParse RVA 0x004C99C3 adds VA 0x00C5ECB8 at +0x0.
// Offsets are the table entries'; each type is what the entry's parse function stores
// (h05 fpgen: INI::parseReal -> Real, parseDurationUnsignedInt -> UnsignedInt, ...);
// names are the INI tokens. Byte gaps are bytes no table entry writes.
// Unresolved (byte storage up to the next member, named after the token):
//   m_announcementEventEnemy +0x8: Rva001DEE74Parse 0x001DEE74
//   m_announcementEventAlly +0xC: Rva001DEE74Parse 0x001DEE74
//   m_announcementEventOwner +0x10: Rva001DEE74Parse 0x001DEE74
#pragma once
#ifndef BFME2_EVA_ANNOUNCE_CLIENT_CREATE_MODULE_DATA_H
#define BFME2_EVA_ANNOUNCE_CLIENT_CREATE_MODULE_DATA_H

#include <stddef.h>
#include "Common/GameType.h"
#include "GameLogic/Module/ModuleData.h"

class INI;
class MultiIniFieldParse;

class EvaAnnounceClientCreateModuleData : public ModuleData
{
public:
	EvaAnnounceClientCreateModuleData();
	static void buildFieldParse(MultiIniFieldParse &parse);

	UnsignedByte m_announcementEventEnemy[0x4];            // +0x8 AnnouncementEventEnemy; unresolved: Rva001DEE74Parse
	UnsignedByte m_announcementEventAlly[0x4];             // +0xC AnnouncementEventAlly; unresolved: Rva001DEE74Parse
	UnsignedByte m_announcementEventOwner[0x4];            // +0x10 AnnouncementEventOwner; unresolved: Rva001DEE74Parse
	UnsignedInt m_delayBeforeAnnouncementMS;               // +0x14 DelayBeforeAnnouncementMS; duration, frames
	Bool m_onlyIfVisible;                                  // +0x18 OnlyIfVisible
	Bool m_countAsFirstSightedAnnoucement;                 // +0x19 CountAsFirstSightedAnnoucement
	Bool m_useObjectsPosition;                             // +0x1A UseObjectsPosition
	Bool m_createFakeRadarEvent;                           // +0x1B CreateFakeRadarEvent
};
// SIZE 0x1C
typedef char EvaAnnounceClientCreateModuleData_size[(sizeof(EvaAnnounceClientCreateModuleData) == 0x1C) ? 1 : -1];
typedef char EvaAnnounceClientCreateModuleData_off_m_announcementEventEnemy[(offsetof(EvaAnnounceClientCreateModuleData, m_announcementEventEnemy) == 0x8) ? 1 : -1];
typedef char EvaAnnounceClientCreateModuleData_off_m_announcementEventAlly[(offsetof(EvaAnnounceClientCreateModuleData, m_announcementEventAlly) == 0xC) ? 1 : -1];
typedef char EvaAnnounceClientCreateModuleData_off_m_announcementEventOwner[(offsetof(EvaAnnounceClientCreateModuleData, m_announcementEventOwner) == 0x10) ? 1 : -1];
typedef char EvaAnnounceClientCreateModuleData_off_m_delayBeforeAnnouncementMS[(offsetof(EvaAnnounceClientCreateModuleData, m_delayBeforeAnnouncementMS) == 0x14) ? 1 : -1];
typedef char EvaAnnounceClientCreateModuleData_off_m_onlyIfVisible[(offsetof(EvaAnnounceClientCreateModuleData, m_onlyIfVisible) == 0x18) ? 1 : -1];
typedef char EvaAnnounceClientCreateModuleData_off_m_countAsFirstSightedAnnoucement[(offsetof(EvaAnnounceClientCreateModuleData, m_countAsFirstSightedAnnoucement) == 0x19) ? 1 : -1];
typedef char EvaAnnounceClientCreateModuleData_off_m_useObjectsPosition[(offsetof(EvaAnnounceClientCreateModuleData, m_useObjectsPosition) == 0x1A) ? 1 : -1];
typedef char EvaAnnounceClientCreateModuleData_off_m_createFakeRadarEvent[(offsetof(EvaAnnounceClientCreateModuleData, m_createFakeRadarEvent) == 0x1B) ? 1 : -1];

#endif
