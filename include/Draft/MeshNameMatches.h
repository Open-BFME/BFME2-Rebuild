// DRAFT (h05 fpgen): MeshNameMatches from retail's field-parse table (VA 0x00C3B958+0x0).
// Offsets are compile-checked below; no function matches against it yet, so it stays
// in Draft until one does. Types are what each entry's parse function stores; names are
// the INI tokens.
// SIZE not established: members reach 0x4; bases and vptr, if any, are the leading gap.
// Unresolved members: none.
#pragma once
#ifndef BFME2_MESH_NAME_MATCHES_H
#define BFME2_MESH_NAME_MATCHES_H

#include <stddef.h>
#include "Common/GameType.h"


class MeshNameMatches
{
public:

	Int m_instances;                                       // +0x0 Instances
};
typedef char MeshNameMatches_off_m_instances[(offsetof(MeshNameMatches, m_instances) == 0x0) ? 1 : -1];

#endif
