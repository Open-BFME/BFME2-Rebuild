// Anim2DTemplate.h: one named 2D animation definition: its frame images, the
// play mode and the frame timing. SIZE 0x1C.
// Layout from the constructor, destructor, allocateImages and getFrame rows and
// from Anim2D's reset, which reads the play-mode word at +0x14.
#pragma once
#ifndef BFME2_ANIM2D_TEMPLATE_H
#define BFME2_ANIM2D_TEMPLATE_H

#include "Common/AsciiString.h"
#include "Common/GameType.h"

class Image;
class INI;

class Anim2DTemplate
{
	friend class Anim2D;
	friend class Anim2DCollection;
public:
	Anim2DTemplate(AsciiString name);
	virtual ~Anim2DTemplate();

	void allocateImages(UnsignedShort count);
	const Image *getFrame(UnsignedShort index) const;
	void storeImage(const Image *image);				// 0x002D7355
	UnsignedShort getNumFrames() const { return m_numFrames; }

protected:
	// INI field parsers (protected static: the map's KAXPAVINI@@PAX1PBX@Z).
	static void parseNumImages(INI *ini, void *instance, void *store, const void *userData);		// 0x002D72F1
	static void parseImage(INI *ini, void *instance, void *store, const void *userData);			// 0x002D73EC
	static void parseImageSequence(INI *ini, void *instance, void *store, const void *userData);	// 0x002D7412

public:

private:
	Anim2DTemplate *m_next;			// +0x04 next template in the collection's list
	AsciiString m_name;				// +0x08
	Image **m_images;				// +0x0C array of m_numFrames image pointers
	UnsignedShort m_numFrames;		// +0x10
	UnsignedShort m_frameDuration;	// +0x12 logic frames each frame stays up
	Int m_playMode;					// +0x14 play mode 1..6; the default is 3
	Bool m_randomStart;				// +0x18 start each instance on a random frame
};

typedef char Anim2DTemplateSize[sizeof(Anim2DTemplate) == 0x1C ? 1 : -1];

#endif // BFME2_ANIM2D_TEMPLATE_H
