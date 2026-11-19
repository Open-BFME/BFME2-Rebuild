// DX8DeviceLock.h: the scope guard around Direct3D device calls.
// Target facts (retail game.dat): BFME 2 serialises device access with a mutex plus a recursion count, where Zero Hour's
// DX8_THREAD_ASSERT compiles to nothing. Guard sites (0x0011DD80's neighbours, the Get_Render_Target_Resolution arm at
// 0x00121860, the index and vertex buffer constructors) call BFME_DX8_Thread_Lock 0x0011F520 with no object
// initialisation and BFME_DX8_Thread_Assert 0x00120F50 (the release) from the destructor, which is the whole object:
// no members, no virtuals (sizeof 1). The lock waits twenty seconds on the mutex handle at 0x00DEC598, then records the
// owning thread (0x00DEDA88) and bumps the recursion count (0x00DEDA8C) under the critical section at 0x00DEC540.
#pragma once
#ifndef BFME2_DX8DEVICELOCK_H
#define BFME2_DX8DEVICELOCK_H

void BFME_DX8_Thread_Lock(void);		// 0x0011F520
bool BFME_DX8_Thread_Assert(void);		// 0x00120F50 (the release half)

class DX8DeviceLock
{
public:
	DX8DeviceLock() { BFME_DX8_Thread_Lock(); }
	~DX8DeviceLock() { BFME_DX8_Thread_Assert(); }
};

#endif
