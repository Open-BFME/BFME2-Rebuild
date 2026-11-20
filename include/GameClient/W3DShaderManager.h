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

#include "Common/Coord2D.h"
#include "WW3D2/TextureAsset.h"

struct IDirect3DTexture8;
struct IDirect3DSurface8;
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

class Coord3D;
enum CustomScenePassModes;

// Zero Hour's FilterModes (CommandXlat.h). BFME 2 renumbers the list: ScreenBWFilter::set 0x000FBC67 picks the
// tint for modes 1, 2 and 3, and ScreenMotionBlurFilter::postRender 0x000FD5B7 treats 13 as the end of a pan and 16
// and above as a pan (Zero Hour: 12 and 14). The modes in between have no evidence yet and are left out.
enum FilterModes
{
	FM_NULL_MODE = 0,
	FM_VIEW_BW_BLACK_AND_WHITE,
	FM_VIEW_BW_RED_AND_WHITE,
	FM_VIEW_BW_GREEN_AND_WHITE,
	FM_VIEW_MB_END_PAN_ALPHA = 13,
	FM_VIEW_MB_PAN_ALPHA = 16
};

// D3DX's four-float vector (d3dx8math.h) with its implicit copy: the screen filters build each value as a temporary
// and copy it whole (ScreenMotionBlurFilter::postRender 0x000FD5B7 moves the four words with movsd).
struct D3DXVECTOR4
{
	D3DXVECTOR4() {}
	D3DXVECTOR4(float fx, float fy, float fz, float fw) : x(fx), y(fy), z(fz), w(fw) {}
	float x, y, z, w;
};

// Zero Hour's pre-transformed, lit, textured quad vertex for the screen filters (FVF XYZRHW | DIFFUSE | TEX1).
struct _TRANS_LIT_TEX_VERTEX
{
	D3DXVECTOR4 p;
	unsigned int color;
	float u;
	float v;
};

// Zero Hour's screen filter interface. The motion blur table 0x007CF410 holds its seven slots in this order
// (init, shutdown, preRender, postRender, setup, set, reset); BFME 2's postRender also takes the display size.
class W3DFilterInterface
{
public:
	virtual int init(void) = 0;
	virtual int shutdown(void);
	virtual bool preRender(bool &skipRender, CustomScenePassModes &scenePassMode);
	virtual bool postRender(FilterModes mode, Coord2D &scrollDelta, bool &doExtraRender, Coord2D *displaySize);
	virtual bool setup(FilterModes mode);

protected:
	virtual int set(FilterModes mode) = 0;
	virtual void reset(void) = 0;
};

// Zero Hour's ScreenMotionBlurFilter, members in Zero Hour's order at the offsets postRender 0x000FD5B7 reads.
// The statics are 0x00DEC20C (the zoom target postRender passes to lookAt) and 0x00DEC1EC (its valid flag).
class ScreenMotionBlurFilter : public W3DFilterInterface
{
public:
	virtual int init(void);
	virtual bool preRender(bool &skipRender, CustomScenePassModes &scenePassMode);	// 0x000FD4CE
	virtual bool postRender(FilterModes mode, Coord2D &scrollDelta, bool &doExtraRender, Coord2D *displaySize);	// 0x000FD5B7
	virtual bool setup(FilterModes mode);	// 0x000FD4E1
	virtual int set(FilterModes mode);	// 0x000FDB21
	virtual void reset(void);

protected:
	enum
	{
		MAX_COUNT = 60,
		MAX_LIMIT = 30,
		COUNT_STEP = 5,
		DEFAULT_PAN_FACTOR = 30
	};

	int m_maxCount;			// +0x04
	int m_lastFrame;		// +0x08
	bool m_decrement;		// +0x0C
	bool m_skipRender;		// +0x0D
	bool m_additive;		// +0x0E
	bool m_doZoomTo;		// +0x0F
	Coord2D m_priorDelta;	// +0x10
	int m_panFactor;		// +0x18

	static Coord3D m_zoomToPos;
	static bool m_zoomToValid;
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
	static IDirect3DTexture8 *endRenderToTexture(void);
	static IDirect3DTexture8 *getRenderTexture(void);
	static bool isRenderingToTexture(void) { return m_renderingToTexture; }		// out-of-line copy 0x000A60EA
	static bool canRenderToTexture(void) { return m_oldRenderSurface && m_newRenderSurface; }	// out-of-line copy 0x000F630C

protected:
	static ShaderTypes m_currentShader;					// 0x00DE1F54
	static FilterTypes m_currentFilter;					// 0x00DE1F5C
	static bool m_renderingToTexture;					// 0x00DE1F60
	static IDirect3DSurface8 *m_oldRenderSurface;		// 0x00DE1F64
	static IDirect3DTexture8 *m_renderTexture;			// 0x00DE1F68
	static IDirect3DSurface8 *m_newRenderSurface;		// 0x00DE1F6C
	static IDirect3DSurface8 *m_oldDepthSurface;		// 0x00DE1F70
};

class Coord2D;
enum FilterModes;

// Zero Hour keeps this in W3DCustomScene.h. The cross-fade's preRender stores 1 for the alpha-mask pass.
enum CustomScenePassModes
{
	SCENE_PASS_DEFAULT,
	SCENE_PASS_ALPHA_MASK
};

// Screen filter interface, Zero Hour's shape. BFME 2's postRender takes a fourth argument, the display size:
// ScreenCrossFadeFilter::postRender returns with `ret 0x10` and divides its texture coordinates by it.
class W3DFilterInterface
{
public:
	virtual int init(void) = 0;
	virtual int shutdown(void) { return true; }
	virtual bool preRender(bool &skipRender, CustomScenePassModes &scenePassMode) { skipRender = false; return false; }
	virtual bool postRender(FilterModes mode, Coord2D &scrollDelta, bool &doExtraRender, Coord2D *displaySize) { return false; }
	virtual bool setup(FilterModes mode) { return false; }
protected:
	virtual int set(FilterModes mode) = 0;
	virtual void reset(void) = 0;
};

// The view cross-fade. Virtual order is the class's own table at 0x007CF204; its setup slot reaches a shared
// `mov al, 1; ret 4` body (0x005CB9FA), as Zero Hour's setup returns true.
class ScreenCrossFadeFilter : public W3DFilterInterface
{
public:
	virtual int init(void);
	virtual int shutdown(void);
	virtual bool preRender(bool &skipRender, CustomScenePassModes &scenePassMode);
	virtual bool postRender(FilterModes mode, Coord2D &scrollDelta, bool &doExtraRender, Coord2D *displaySize);
	virtual bool setup(FilterModes mode) { return true; }
protected:
	virtual int set(FilterModes mode);
	virtual void reset(void);
	bool updateFadeLevel(void);		// advances the fade; false once it has finished
	static int m_fadeFrames;			// 0x00DEBFFC
	static int m_fadeDirection;			// 0x00DEBFF8
	static int m_curFadeFrame;			// 0x00DEC000
	static float m_curFadeValue;		// 0x00DEBFF4
	static bool m_skipRender;			// 0x00DEC004
};

#endif
