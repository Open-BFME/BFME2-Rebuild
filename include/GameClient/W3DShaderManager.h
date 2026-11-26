// Portions derived from Command & Conquer Generals and Command & Conquer Generals Zero Hour, (c) Electronic Arts Inc., GPL-3.0-or-later; see LICENSE and NOTICE.
// W3DShaderManager.h: the manager for custom (non-W3D) shaders and the render-to-texture support the screen filters use
// (W3DDevice/GameClient/W3DShaderManager.cpp, retail 0x00075480..).
// Class shape and member names are Zero Hour's w3dshadermanager.h. Target facts (retail game.dat), where BFME 2 differs:
// - The class is all statics. The constructor 0x00075480 clears them: m_currentShader is set to -1 (0x00DE1F54), then
//   m_currentFilter (+0x5C), m_oldRenderSurface (+0x64), m_renderTexture (+0x68), m_newRenderSurface (+0x6C),
//   m_oldDepthSurface (+0x70), the byte m_renderingToTexture (+0x60), and the four render resource words at 0x00DE1F74..0x00DE1F80.
//   getRenderTexture 0x00075E36 returns m_renderTexture. The constructor clears two pass counts at 0x00DE1F1C, two shader
//   pointers at 0x00DE1F24, then ten independent filter pointers at 0x00DE1F2C.
// - The shader texture slots (setTexture 0x00072B25) hold texture handles (TextureAsset), eight of them from 0x00DE1F8C.
#pragma once
#ifndef BFME2_W3DSHADERMANAGER_H
#define BFME2_W3DSHADERMANAGER_H

#include "WW3D2/TextureAsset.h"

struct IDirect3DTexture9;
struct IDirect3DSurface9;
// Zero Hour's filter list (view.h): the four entries match the four-dword filter table the constructor clears.
enum FilterTypes
{
	FT_NULL_FILTER = 0,
	FT_VIEW_BW_FILTER,
	FT_VIEW_MOTION_BLUR_FILTER,
	FT_VIEW_CROSSFADE,
	FT_MAX
};

// Custom shader dispatch table. ShroudTextureShader installs these four slots in 0x000F5B56.
class W3DShaderInterface
{
public:
	virtual int set(int pass);
	virtual void reset(void);
	virtual int init(void) = 0;
	virtual int shutdown(void);
};

class W3DShaderManager
{
public:
	// 0x000754BC..0x000754CB clears two pass counts and two shader pointers.
	// ShroudTextureShader::init 0x000F5B43 registers at slot 1.
	enum ShaderTypes
	{
		ST_INVALID,
		ST_SHROUD_TEXTURE,
		ST_MAX
	};

	W3DShaderManager(void);
	static void shutdown(void);
	static int getShaderPasses(ShaderTypes shader);
	static void resetShader(ShaderTypes shader);		// 0x00075695
	static void startRenderToTexture(void);
	static IDirect3DTexture9 *endRenderToTexture(void);
	static IDirect3DTexture9 *getRenderTexture(void);
	static bool isRenderingToTexture(void) { return m_renderingToTexture; }		// out-of-line copy 0x000A60EA
	static bool canRenderToTexture(void) { return m_oldRenderSurface && m_newRenderSurface; }	// out-of-line copy 0x000F630C

protected:
	static ShaderTypes m_currentShader;					// 0x00DE1F54
	static FilterTypes m_currentFilter;					// 0x00DE1F5C
	static bool m_renderingToTexture;					// 0x00DE1F60
	static IDirect3DSurface9 *m_oldRenderSurface;		// 0x00DE1F64
	static IDirect3DTexture9 *m_renderTexture;			// 0x00DE1F68
	static IDirect3DSurface9 *m_newRenderSurface;		// 0x00DE1F6C
	static IDirect3DSurface9 *m_oldDepthSurface;		// 0x00DE1F70
};

#endif
