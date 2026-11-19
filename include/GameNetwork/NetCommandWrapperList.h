// NetCommandWrapperList.h: the wrapped commands being reassembled (Zero Hour's NetCommandWrapperList;
// names are theirs by role, offsets are retail's).
// Target facts (code RVAs, VA 0x00C70A24 table): SIZE 8. The constructor 0x0058C0AE clears the list head
// +4 and stores the table of one slot, the scalar deleting destructor 0x0058C268 (destructor 0x0058C0BB,
// protected). reset 0x0058C0F0 and the destructor free every node through its virtual destructor (flag 0) and
// the global operator delete.
#pragma once
#ifndef BFME2_NETCOMMANDWRAPPERLIST_H
#define BFME2_NETCOMMANDWRAPPERLIST_H

#include "Common/GameType.h"

class NetCommandList;
class NetCommandRef;
class NetCommandWrapperListNode;

class NetCommandWrapperList
{
public:
	NetCommandWrapperList();								// 0x0058C0AE

	friend class ConnectionManager;						// ~ConnectionManager 0x004D2344 deletes the list through the virtual destructor
protected:
	virtual ~NetCommandWrapperList();						// slot 0, 0x0058C0BB (deleting 0x0058C268)

public:
	void init();											// 0x0055059A (folded dword clearer: m_list = 0)
	void reset();											// 0x0058C0F0
	void processWrapper(NetCommandRef *ref);				// 0x0058C11F
	NetCommandList *getReadyCommands();						// 0x0058C2AD
	Int getPercentComplete(UnsignedShort wrappedCommandID);	// 0x0058C284

protected:
	void removeFromList(NetCommandWrapperListNode *node);	// 0x0058C1A5

	NetCommandWrapperListNode *m_list;						// +0x04
};
// SIZE 0x08

#endif
