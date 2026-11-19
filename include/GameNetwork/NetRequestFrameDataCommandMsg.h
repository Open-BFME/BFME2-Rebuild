// NetRequestFrameDataCommandMsg.h: a request to resend a range of frame data (the map names only the reader NetPacket::
// readRequestFrameDataMessage 0x0058E481; the constructor is a placeholder).
// Target facts (code RVAs): SIZE 0x24 (the reader allocates 0x24). The constructor 0x004D5A30 runs NetCommandMsg's, zeroes
// the dwords +0x1C and +0x20, and stores its own table 0x00C60294 and the command type 9. The reader fills +0x1C through the
// shared dword setter 0x00318B09 and +0x20 through 0x00317B9B.
#pragma once
#ifndef BFME2_NETREQUESTFRAMEDATACOMMANDMSG_H
#define BFME2_NETREQUESTFRAMEDATACOMMANDMSG_H

#include "GameNetwork/NetCommandMsg.h"

class NetRequestFrameDataCommandMsg : public NetCommandMsg
{
public:
	NetRequestFrameDataCommandMsg();							// 0x004D5A30
	virtual AsciiString getContentsAsAsciiString();				// slot 3, 0x004D6468 (this class has its own table 0x00C60294)
	void setFirstFrame(UnsignedInt frame);						// 0x00318B09 (folded dword setter at +0x1C)
	void setLastFrame(UnsignedInt frame);						// 0x00317B9B (folded dword setter at +0x20)
	UnsignedInt getFirstFrame();								// 0x0030F2C7 (folded dword getter at +0x1C)
	UnsignedInt getLastFrame();									// 0x0030D377 (folded dword getter at +0x20)

	UnsignedInt m_firstFrame;									// +0x1C
	UnsignedInt m_lastFrame;									// +0x20
};
// SIZE 0x24
typedef char NetRequestFrameDataCommandMsgSize[(sizeof(NetRequestFrameDataCommandMsg) == 0x24) ? 1 : -1];

#endif
