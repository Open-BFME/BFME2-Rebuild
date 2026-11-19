// EmotionNugget.h: one emotion a game object is carrying. SIZE 0x34.
// Target facts: constructor 0x004DD206 (Emotion(Object *, EmotionTrackerUpdateEntry *)),
// destructor 0x004DCE8E (destroys the two trees at +0x20 and +0x14, in that order),
// scalar deleting destructor 0x004B0D0C, OnActivation 0x004DCC84, OnDeactivation
// 0x004DD2C0, CanBeActivatedFor 0x004DD018 and the per-frame step 0x004DCEC4
// (Ghidra named that one Emotion::rva004DCEC4; it reads the same +0x00 object,
// +0x04 entry, +0x08 source id, +0x2C and +0x30 frames, so it is this class).
// The two members at +0x14 and +0x20 are retail's own STLport maps (12 bytes:
// the tree's header proxy and node count), so this header needs <map> from the
// opt-in overlay: every unit that includes it carries a `// stlport46` line.
#pragma once
#ifndef BFME2_EMOTIONNUGGET_H
#define BFME2_EMOTIONNUGGET_H

#include <map>
#include "Common/GameType.h"

class Object;
class Xfer;
class EmotionTrackerUpdateEntry;

class EmotionNugget
{
public:
	EmotionNugget(Object *object, EmotionTrackerUpdateEntry *entry);
	~EmotionNugget();

	void OnActivation(Object *target, int duration);
	void OnDeactivation();
	void xfer(Xfer *xfer);					// 0x004DD489 (EmotionTrackerUpdate::xfer 0x004B1ECB calls it for every nugget)
	// The third argument is the candidate object: 0x004DD018 reads +0x74 as its
	// id and +0x04 -> +0x5D8 as its template id, and every caller
	// (EmotionTrackerUpdate 0x004B10E3 and friends) passes findObjectByID's result.
	bool CanBeActivatedFor(int a, int b, Object *target);
	Bool rva004DCEC4();

	EmotionTrackerUpdateEntry *getTemplate() const { return m_entry; }

	Object *m_object;							// 0x00
	EmotionTrackerUpdateEntry *m_entry;			// 0x04
	ObjectID m_id;								// 0x08
	unsigned short m_templateID;				// 0x0C
	UnsignedInt m_nextFrame;					// 0x10
	_STL::map<ObjectID, UnsignedInt> m_targets;	// 0x14 keyed by the source object's id, which
												// OnDeactivation 0x004DD2C0 passes by address
	_STL::map<unsigned short, int> m_templates;	// 0x20
	UnsignedInt m_untilFrame;					// 0x2C
	UnsignedInt m_activatedFrame;				// 0x30
};
// SIZE 0x34

#endif
