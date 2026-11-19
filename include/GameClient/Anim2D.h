// Anim2D.h: one playing 2D animation instance. SIZE 0x34.
// Layout from the constructor, destructor, setters and the instance-list links
// that Anim2DCollection writes (+0x20 owner, +0x24 next, +0x28 previous).
// Frame indices and times are the words and dwords those rows read and write.
#pragma once
#ifndef BFME2_ANIM2D_H
#define BFME2_ANIM2D_H

#include "Common/GameType.h"
#include "Common/ICoord2D.h"
#include "Common/Snapshot.h"

class Anim2DTemplate;
class Anim2DCollection;
class Xfer;

class Anim2D : public Snapshot
{
	friend class Anim2DCollection;
public:
	Anim2D(Anim2DTemplate *tmpl, Anim2DCollection *collection);

	void setCurrentFrame(UnsignedShort frame);
	void randomizeCurrentFrame();
	void reset();
	void setStatus(UnsignedByte bits);
	void clearStatus(UnsignedByte bits);
	UnsignedInt getCurrentFrameWidth() const;
	UnsignedInt getCurrentFrameHeight() const;
	void draw(Int x, Int y);				// 0x002D6EF1 draws at the image's natural size
	void draw(Int x, Int y, Int width, Int height);	// 0x002D709A

protected:
	virtual ~Anim2D();
	virtual void loadPostProcess() {}
	virtual const char *GetSnapshotName() const;
	virtual void xfer(Xfer *xfer);

	void tryNextFrame();

private:
	UnsignedShort m_frame;			// +0x04 frame index shown now
	char m_pad06[2];				// +0x06 not written by any measured row
	UnsignedInt m_lastFrameChange;	// +0x08 logic frame of the last frame change
	Anim2DTemplate *m_template;		// +0x0C
	UnsignedByte m_status;			// +0x10 status bits set and cleared by setStatus/clearStatus
	char m_pad11;					// +0x11
	UnsignedShort m_playStartFrame;	// +0x12 first frame of the play range
	UnsignedShort m_playEndFrame;	// +0x14 last frame of the play range (count - 1)
	UnsignedInt m_frameDuration;	// +0x18 logic frames each frame stays up
	Real m_opacity;					// +0x1C scaled into the draw alpha byte
	Anim2DCollection *m_collection;	// +0x20 owner list, null when unregistered
	Anim2D *m_nextInstance;			// +0x24
	Anim2D *m_prevInstance;			// +0x28
	ICoord2D m_frameSize;			// +0x2C x = image width, y = image height of the current frame
};

typedef char Anim2DSize[sizeof(Anim2D) == 0x34 ? 1 : -1];

#endif // BFME2_ANIM2D_H
