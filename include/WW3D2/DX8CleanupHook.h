// Portions derived from Command & Conquer Generals and Command & Conquer Generals Zero Hour, (c) Electronic Arts Inc., GPL-3.0-or-later; see LICENSE and NOTICE.
// DX8CleanupHook.h: Direct3D device loss and reset callback interface.
#pragma once
#ifndef BFME2_DX8CLEANUPHOOK_H
#define BFME2_DX8CLEANUPHOOK_H

class DX8_CleanupHook
{
public:
	virtual void ReleaseResources(void) = 0;
	virtual void ReAcquireResources(void) = 0;
};

#endif
