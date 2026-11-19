// PresentParameters9.h: the present parameters of the Direct3D 9 device BFME 2 creates.
// Target facts (retail game.dat): the global at 0x00DED5B8 (ledger name _PresentParameters) is read at +0x28 by
// Has_Stencil 0x0011CDC0 (AutoDepthStencilFormat: compared with 0x4B D24S8 and 0x4F D24X4S4), written at +0x34 by
// Set_Swap_Interval 0x00122A60 (0x80000000, 2, 4 and 1 are the D3DPRESENT_INTERVAL values), and Create_Additional_Swap_Chain
// 0x0011E580 builds a local copy of 0x38 bytes (a 14 dword struct, three more members than d3d8.h's 0x34).
// The layout is d3d9types.h's _D3DPRESENT_PARAMETERS_ (the Direct3D 8 structure with MultiSampleQuality after
// MultiSampleType and the refresh rate and presentation interval renamed); the tag follows the ledger's mangled name
// (U_D3DPRESENT_PARAMETERS) so it does not collide with d3d8types.h's _D3DPRESENT_PARAMETERS_.
#pragma once
#ifndef BFME2_PRESENTPARAMETERS9_H
#define BFME2_PRESENTPARAMETERS9_H

#include <d3d8.h>

struct _D3DPRESENT_PARAMETERS
{
	UINT BackBufferWidth;						// +0x00
	UINT BackBufferHeight;						// +0x04
	D3DFORMAT BackBufferFormat;					// +0x08
	UINT BackBufferCount;						// +0x0C
	D3DMULTISAMPLE_TYPE MultiSampleType;		// +0x10
	DWORD MultiSampleQuality;					// +0x14
	D3DSWAPEFFECT SwapEffect;					// +0x18
	HWND hDeviceWindow;							// +0x1C
	BOOL Windowed;								// +0x20
	BOOL EnableAutoDepthStencil;				// +0x24
	D3DFORMAT AutoDepthStencilFormat;			// +0x28
	DWORD Flags;								// +0x2C
	UINT FullScreen_RefreshRateInHz;			// +0x30
	UINT PresentationInterval;					// +0x34
};

#endif
