// CameraMarker.h: one named camera position in the tactical view's marker list (View +0x80). SIZE 0x30.
// Target facts: View::xfer 0x0025F020 allocates 0x30 bytes per marker, clears +0x00 and +0x04, transfers the
// record through the node function 0x0025ECFC and then links it in front of the list head; the node function
// transfers, in this order, the AsciiString at +0x04, a Coord3DBase at +0x08, six floats from +0x14 to
// +0x28 and two bools at +0x2C and +0x2D. 0x0025F30D pops and destroys the nodes (the string release is
// the fold 0x0029D7C2), 0x0025F385 walks them comparing the string at +0x04 with a name.
// Not the CameraMarker class of FlammableUpdateModuleData.h (that is an 8-byte fx/bone pair the ledger
// gave the same name); only View.cpp includes this one.
#pragma once
#ifndef BFME2_CAMERAMARKER_H
#define BFME2_CAMERAMARKER_H

#include "Common/GameType.h"
#include "Common/AsciiString.h"
#include "Common/Coord3DBase.h"
#include "Common/Coord3D.h"

class Xfer;

struct CameraMarker
{
	CameraMarker() : m_next(0) {}
	void xfer(Xfer *xfer);						// 0x0025ECFC (the map still names it Rva0025ECFC::rva0025ECFC)

	CameraMarker *m_next;						// +0x00
	AsciiString m_name;							// +0x04
	Coord3D m_position;							// +0x08 (a Coord3D: ScriptActions::doMoveCameraTo 0x003BAAE4 copies it into its Coord3D with movsd; View::xfer still hands the Coord3DBase base to Xfer)
	Real m_unk0x14;								// +0x14 six floats, xfer'd in order
	Real m_unk0x18;								// +0x18
	Real m_unk0x1C;								// +0x1C
	Real m_unk0x20;								// +0x20
	Real m_unk0x24;								// +0x24
	Real m_unk0x28;								// +0x28
	Bool m_unk0x2C;								// +0x2C
	Bool m_unk0x2D;								// +0x2D
};
// SIZE 0x30 (retail allocates 0x30 bytes; the two bools end at 0x2E)

#endif
