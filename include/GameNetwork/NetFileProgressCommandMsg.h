// NetFileProgressCommandMsg.h: how much of a file transfer has arrived (Zero Hour's NetFileProgressCommandMsg by role; the
// map names only its constructor, as a placeholder).
// Target facts (code RVAs): SIZE 0x24. The constructor 0x004D598E runs NetCommandMsg's, stores the shared table 0x00C60244
// and the command type 0x16, and zeroes the word +0x1C and the dword +0x20. NetPacket::rva0058E2D7 (the file progress
// reader) reads a word through the word setter 0x004D59AC and a dword through the dword setter 0x00317B9B.
#pragma once
#ifndef BFME2_NETFILEPROGRESSCOMMANDMSG_H
#define BFME2_NETFILEPROGRESSCOMMANDMSG_H

#include "GameNetwork/NetCommandMsg.h"

class NetFileProgressCommandMsg : public NetCommandMsg
{
public:
	NetFileProgressCommandMsg();								// 0x004D598E
	void setFileID(UnsignedShort fileID);						// 0x004D59AC
	UnsignedShort getFileID();									// 0x004D5767
	Int getProgress();											// 0x0030D377 (folded dword getter at +0x20)
	void setProgress(Int progress);								// 0x00317B9B (folded dword setter at +0x20)

	UnsignedShort m_fileID;										// +0x1C
	unsigned char m_pad1E[2];									// +0x1E
	Int m_progress;												// +0x20
};
// SIZE 0x24
typedef char NetFileProgressCommandMsgSize[(sizeof(NetFileProgressCommandMsg) == 0x24) ? 1 : -1];

#endif
