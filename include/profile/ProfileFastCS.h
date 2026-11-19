// Portions derived from Command & Conquer Generals and Command & Conquer Generals Zero Hour, (c) Electronic Arts Inc., GPL-3.0-or-later; see LICENSE and NOTICE.
// ProfileFastCS.h: the profile library's spin-lock critical section (Zero Hour's internal.h). Target facts (retail game.dat):
// the flag word is the first member; ThreadSafeSetFlag 0x006C5EF0 is an out-of-line body (a `lock bts` loop that waits on
// testEvent, 0x00A0C774, while the bit is set), the lock's constructor and destructor are expanded in place by their users
// (ProfileId::AsString 0x006C60A0 calls ThreadSafeSetFlag on the static `cs` 0x00A0C628 and clears the flag inline).
#pragma once
#ifndef BFME2_PROFILEFASTCS_H
#define BFME2_PROFILEFASTCS_H

class ProfileFastCS
{
	ProfileFastCS(const ProfileFastCS &);
	ProfileFastCS &operator=(const ProfileFastCS &);

	volatile unsigned int m_Flag;
	static void *testEvent;						// 0x00A0C774

	void ThreadSafeSetFlag();					// 0x006C5EF0
	void ThreadSafeClearFlag()
	{
		m_Flag = 0;
	}

public:
	ProfileFastCS() : m_Flag(0)
	{
	}

	class Lock
	{
		Lock(const Lock &);
		Lock &operator=(const Lock &);

		ProfileFastCS &CriticalSection;

	public:
		Lock(ProfileFastCS &cs) : CriticalSection(cs)
		{
			CriticalSection.ThreadSafeSetFlag();
		}
		~Lock()
		{
			CriticalSection.ThreadSafeClearFlag();
		}
	};
	friend class Lock;
};

void *ProfileAllocMemory(unsigned int numBytes);				// 0x006C5460
void *ProfileReAllocMemory(void *oldPtr, unsigned int newSize);
void ProfileFreeMemory(void *ptr);

#endif
