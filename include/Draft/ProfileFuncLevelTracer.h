#pragma once

// The optional frame hooks. Retail calls them with two int arguments and
// cleans nothing after the call, so the pointer is __stdcall.
typedef void (__stdcall *ProfileFuncLevelHook)(int, int);

// ProfileFuncLevelTracer: the !HAS_PROFILE build of the Zero Hour profiler's
// frame entry points (retail 0x006C8360 Shutdown, 0x006C8380 FrameStart,
// 0x006C83A0 FrameEnd). Evidence: the three bodies read the .bss slots at
// 0x00E0C764 (the VSPerf module), 0x00E0C768 and 0x00E0C76C, release the
// module through the import at 0x00BBA1FC and call the two hooks.
class ProfileFuncLevelTracer
{
public:
	static void Shutdown();
	static int FrameStart();
	static void FrameEnd(int which, int mixIndex);

	static ProfileFuncLevelHook frameStartHook;	// .bss 0x00E0C768
	static ProfileFuncLevelHook frameEndHook;	// .bss 0x00E0C76C
};
