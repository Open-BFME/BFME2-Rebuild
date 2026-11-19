// BfmeNetAddress.h: a peer's network address (the map's BfmeNetAddress; Zero Hour passes a bare IP where BFME 2 passes this).
// Target facts (code RVAs): 8 bytes, an address dword at +0 and a port word at +4 (two bytes of padding follow). GameSlot's
// record at +0x38 and GameInfo's local address at +0x38 are this type; the comparison 0x00248CBF (the map holds it as
// BfmeNetAddress::Rva00248CBF) tests both fields; LANAPI::LookupPlayer 0x0044998E, GameSlot::isPlayer 0x003FF145 and
// ConnectionManager::ackCommand 0x004CF40E take it by const pointer.
#pragma once
#ifndef BFME2_BFMENETADDRESS_H
#define BFME2_BFMENETADDRESS_H

#include "Common/GameType.h"

struct BfmeNetAddress
{
	BfmeNetAddress() : m_ip(0), m_port(0) {}
	Bool Rva00248CDD(const BfmeNetAddress *other) const;	// 0x00248CDD (the addresses differ; the map has no better name)
	Bool Rva00248CBF(const BfmeNetAddress *other) const;	// 0x00248CBF (both fields equal; the map has no better name)

	UnsignedInt m_ip;										// +0
	UnsignedShort m_port;									// +4
};
// SIZE 8 (two bytes of padding)

#endif
