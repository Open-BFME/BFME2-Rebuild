// NetFileCommandMsg.h: one file's contents sent to the other players (Zero Hour's NetFileCommandMsg by role; the map names
// the class through ConnectionManager::processFile 0x004D2B16 and setFileData 0x004D594C, the rest are placeholders).
// Target facts (code RVAs): SIZE 0x28 (NetPacket::rva005922FB reader allocates 0x28). The constructor 0x004D61BB runs
// NetCommandMsg's, stores its own table 0x00C60484 and the command type 0x13, and zeroes the AsciiString +0x1C (the
// portable file name), the data pointer +0x20 and the data length +0x24. The destructor 0x004D6208 (deleting 0x004D6AB9,
// protected) frees the data with delete[] and releases the name. setFileData 0x004D594C allocates and copies.
// The name getter 0x002D9BA6 copies +0x1C out (a folded AsciiString getter shared with the other file messages), the
// plain name setter 0x00590F13 is folded with FileAnnounce's, 0x004D6250 sets the name from a real path (through the
// file system) and 0x004D632D returns the real path; the data getter 0x0030D377 and length getter 0x00091A56 are the
// shared dword accessors at +0x20 / +0x24.
#pragma once
#ifndef BFME2_NETFILECOMMANDMSG_H
#define BFME2_NETFILECOMMANDMSG_H

#include "GameNetwork/NetCommandMsg.h"
#include "Common/AsciiString.h"

class NetFileCommandMsg : public NetCommandMsg
{
public:
	NetFileCommandMsg();										// 0x004D61BB
protected:
	virtual ~NetFileCommandMsg();								// slot 0, 0x004D6208 (deleting 0x004D6AB9)
public:
	AsciiString getPortableFilename() const;					// 0x002D9BA6 (folded)
	void setPortableFilename(AsciiString name);					// 0x00590F13 (folded)
	void setRealFilename(AsciiString name);						// 0x004D6250
	AsciiString getRealFilename() const;						// 0x004D632D
	void setFileData(UnsignedByte *data, UnsignedInt dataLength);	// 0x004D594C
	UnsignedByte *getFileData();								// 0x0030D377 (folded dword getter at +0x20)
	UnsignedInt getFileLength();								// 0x00091A56 (folded dword getter at +0x24)

	AsciiString m_portableFilename;								// +0x1C
	UnsignedByte *m_data;										// +0x20
	UnsignedInt m_dataLength;									// +0x24
};
// SIZE 0x28
typedef char NetFileCommandMsgSize[(sizeof(NetFileCommandMsg) == 0x28) ? 1 : -1];

#endif
