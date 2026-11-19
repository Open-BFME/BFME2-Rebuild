// VideoStream.h: one playing movie. VideoPlayerInterface::open (table 0x008E4918 slot
// 17) returns it; the VP6 player's stream class (constructor 0x000907C1, table
// 0x007C7F20, 21 slots) is its one implementation in the retail image.
//
// Evidence (retail game.dat): WindowVideo::~WindowVideo 0x0053F10E calls slot +0x1C on
// the stream and then clears the pointer; the table's slot 7 (0x0068A130) runs the
// callback at +0x10 and then deletes the stream through its own slot 0, which is the
// close of the donor's interface (a stream closes by deleting itself). WindowVideo's
// getVideoBuffer 0x0053F0BF and init 0x0053F165 call slot +0x3C, whose body is a 4-byte
// getter (0x005C4ACD, folded with GameWindow::winGetUserData) returning the buffer.
// Slot 0 is the deleting destructor (0x00091153). Slots 1..5, 9..13 and 16..20 are
// not named here: nothing in the client UI calls them, and their donor names cannot be
// assigned from the table alone (it does not follow the donor's declaration order), so
// they are declared as numbered placeholders to keep the slot positions right.
#pragma once
#ifndef BFME2_VIDEOSTREAM_H
#define BFME2_VIDEOSTREAM_H

class VideoBuffer;

class VideoStreamInterface
{
public:
	virtual ~VideoStreamInterface() {}									// slot 0
	virtual void streamSlot1() = 0;
	virtual void streamSlot2() = 0;
	virtual void streamSlot3() = 0;
	virtual void streamSlot4() = 0;
	virtual void streamSlot5() = 0;
	virtual int update(int flags) = 0;										// slot 6, +0x18: flags 4 = loop; WindowVideoManager::update 0x0053F53C ("name: inferred")
	virtual void close() = 0;											// slot 7, +0x1C
	virtual int frameIndex() = 0;										// slot 8, +0x20: 4-byte getter 0x005C4AFD (the frame counter at stream+0x48); update compares it with 0 after a pass ("name: inferred")
	virtual void streamSlot9() = 0;
	virtual void streamSlot10() = 0;
	virtual void streamSlot11() = 0;
	virtual void streamSlot12() = 0;
	virtual void streamSlot13() = 0;
	virtual bool setVideoBuffer(VideoBuffer *buffer) = 0;					// slot 14, +0x38: 0x00090F88 sizes the buffer for the stream, stores it at +0x2C and returns true (WindowVideoManager::playMovie 0x0053F63D closes the stream when it returns false) ("name: inferred")
	virtual VideoBuffer *videoBuffer() = 0;								// slot 15, +0x3C
	virtual void streamSlot16() = 0;
	virtual void streamSlot17() = 0;
	virtual void streamSlot18() = 0;
	virtual void streamSlot19() = 0;
	virtual void streamSlot20() = 0;
};

#endif
