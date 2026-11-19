// NetFileAnnounceCommandMsg.h: "a file is coming" (Zero Hour's NetFileAnnounceCommandMsg by role; the map names the class
// through ConnectionManager::processFileAnnounce 0x004D2C8E, the rest are placeholders).
// Target facts (code RVAs): SIZE 0x24 (NetPacket::rva005923C5 reader allocates 0x24). The constructor 0x004D62A9 runs
// NetCommandMsg's, stores its own table 0x00C60494 and the command type 0x15, zeroes the AsciiString +0x1C (the portable
// file name), the file id word +0x20 and the player mask byte +0x22. The destructor 0x004D62F7 (deleting 0x004D6AD5) releases
// the name. The name getter 0x002D9BA6 and the plain name setter 0x00590F13 are folded with the other file messages'; the
// file id accessors are 0x004D5973 / 0x004D5978 and the player mask accessors 0x001DCD01 / 0x004D5984.
#pragma once
#ifndef BFME2_NETFILEANNOUNCECOMMANDMSG_H
#define BFME2_NETFILEANNOUNCECOMMANDMSG_H

#include "GameNetwork/NetCommandMsg.h"
#include "Common/AsciiString.h"

class NetFileAnnounceCommandMsg : public NetCommandMsg
{
public:
	NetFileAnnounceCommandMsg();								// 0x004D62A9
	virtual ~NetFileAnnounceCommandMsg();						// slot 0, 0x004D62F7 (deleting 0x004D6AD5)
	AsciiString getPortableFilename() const;					// 0x002D9BA6 (folded)
	void setPortableFilename(AsciiString name);					// 0x00590F13 (folded)
	UnsignedShort getFileID();									// 0x004D5973
	void setFileID(UnsignedShort fileID);						// 0x004D5978
	UnsignedByte getPlayerMask();								// 0x001DCD01
	void setPlayerMask(UnsignedByte mask);						// 0x004D5984

	AsciiString m_portableFilename;								// +0x1C
	UnsignedShort m_fileID;										// +0x20
	UnsignedByte m_playerMask;									// +0x22
	unsigned char m_pad23;										// +0x23
};
// SIZE 0x24
typedef char NetFileAnnounceCommandMsgSize[(sizeof(NetFileAnnounceCommandMsg) == 0x24) ? 1 : -1];

#endif
