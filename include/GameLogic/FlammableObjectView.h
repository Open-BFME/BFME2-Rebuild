// FlammableObjectView.h: the Object fields the flammable bodies read at their
// retail offsets. Thing keeps the template at +0x04 and the position at +0x38;
// the module pointers follow at +0x250.
#pragma once
#ifndef BFME2_FLAMMABLEOBJECTVIEW_H
#define BFME2_FLAMMABLEOBJECTVIEW_H

#include "Common/GameType.h"
#include "Common/Coord3D.h"

class ThingTemplate;
class ContainModuleSlots;

// The kind mask at +0x11C; bit 31 marks a kind that never counts as underwater.
struct ThingTemplateKind
{
	unsigned char m_pad[0x11C];					// 0x000
	UnsignedInt m_kindOf;						// 0x11C
};

struct FlammableObjectView
{
	void *m_vtable;								// 0x000
	const ThingTemplate *m_template;			// 0x004
	unsigned char m_unknown08[0x38 - 0x08];		// 0x008
	Coord3D m_pos;								// 0x038
	unsigned char m_unknown44[0x250 - 0x44];	// 0x044
	ContainModuleSlots *m_contain;				// 0x250
	void *m_body;								// 0x254
	void *m_ai;									// 0x258
};
// SIZE 0x25C

#endif
