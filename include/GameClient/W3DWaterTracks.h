// Portions derived from Command & Conquer Generals and Command & Conquer Generals Zero Hour, (c) Electronic Arts Inc., GPL-3.0-or-later; see LICENSE and NOTICE.
// W3DWaterTracks.h: the animated wave and splash tracks drawn on the water (Zero Hour's W3DWaterTracks.h).
// Target facts (retail game.dat): WaterTracksObj is 0xB8 bytes with Zero Hour's member order (vptr +0, texture +4, bounding
// sphere +8, bounding box +0x18, wave type +0x30, vertex counts +0x34/+0x38, bound flag +0x3C, start position +0x40, wave
// direction +0x48, perpendicular +0x50, initial start +0x58 and end +0x60, time offset +0x68, fade +0x6C, total +0x70,
// elapsed +0x74, the wave shape reals +0x78..+0xAC, next +0xB0 and previous +0xB4 in the system's lists: bindTrack 0x000FDF4C,
// findTrack 0x000FE207). WaterTracksRenderSystem keeps the vertex buffer +0, index buffer +4, vertex material +8, shader +0xC
// (the constructor 0x000FE90D stores 0x0010441B), used list +0x10, free list +0x14, strip size +0x18/+0x1C (2 and 2), batch
// start +0x20 and the water level, and sets the global TheWaterTracksRenderSystem 0x00DEC218.
#pragma once
#ifndef BFME2_W3DWATERTRACKS_H
#define BFME2_W3DWATERTRACKS_H

#include "Common/AsciiString.h"
#include "WW3D2/ShaderClass.h"
#include "WWMath/AABoxClass.h"
#include "WWMath/SphereClass.h"
#include "WWMath/Vector2.h"

enum waveType;	// forward reference (the enumerators live with the water renderer)

class VertexBufferClass;
class DX8IndexBufferClass;

// Custom render object that draws a small breaking wave or splash animation. These objects are managed only by the
// WaterTracksRenderSystem.
class WaterTracksObj
{
	friend class WaterTracksRenderSystem;

public:
	virtual void Render(void) {}												// slot 0
	virtual void Get_Obj_Space_Bounding_Sphere(SphereClass &sphere) const;		// 0x000FE359
	virtual void Get_Obj_Space_Bounding_Box(AABoxClass &aabox) const;			// 0x000FE369

protected:
	void *m_stageZeroTexture;		// +0x04 primary texture
	SphereClass m_boundingSphere;	// +0x08
	AABoxClass m_boundingBox;		// +0x18
	waveType m_type;				// +0x30 used for render state sorting
	int m_x;						// +0x34 vertex count
	int m_y;						// +0x38 vertex count
	bool m_bound;					// +0x3C object is bound to owner and accepts new edges
	Vector2 m_startPos;				// +0x40 starting position of wave
	Vector2 m_waveDir;				// +0x48 direction of wave travel
	Vector2 m_perpDir;				// +0x50 direction perpendicular to wave travel
	Vector2 m_initStartPos;			// +0x58 original settings used to create wave
	Vector2 m_initEndPos;			// +0x60
	int m_initTimeOffset;			// +0x68 time offset when wave is added into the system
	int m_fadeMs;					// +0x6C time for wave to fade out after it stops moving
	int m_totalMs;					// +0x70 amount of time to complete full motion
	int m_elapsedMs;				// +0x74 amount of time since start of motion

	float m_waveInitialWidth;		// +0x78
	float m_waveInitialHeight;		// +0x7C
	float m_waveFinalWidth;			// +0x80
	float m_waveFinalWidthPeakFrac;	// +0x84
	float m_waveFinalHeight;		// +0x88

	float m_initialVelocity;		// +0x8C initial velocity in world units per ms.
	float m_waveDistance;			// +0x90 total distance traveled by wave front.
	float m_timeToReachBeach;		// +0x94
	float m_frontSlowDownAcc;		// +0x98
	float m_timeToStop;				// +0x9C
	float m_timeToRetreat;			// +0xA0
	float m_backSlowDownAcc;		// +0xA4
	float m_timeToCompress;			// +0xA8
	float m_flipU;					// +0xAC force uv coordinates to flip

	WaterTracksObj *m_nextSystem;	// +0xB0 next track in system
	WaterTracksObj *m_prevSystem;	// +0xB4 previous track in system
};
// SIZE 0xB8
typedef char WaterTracksObjSize[(sizeof(WaterTracksObj) == 0xB8) ? 1 : -1];

// System for drawing, updating, and re-using water mark render objects.
class WaterTracksRenderSystem
{
	friend class WaterTracksObj;

public:
	WaterTracksRenderSystem(void);					// 0x000FE90D
	~WaterTracksRenderSystem(void);					// 0x000FF4D6
	WaterTracksObj *bindTrack(waveType type);		// 0x000FDF4C track object to be controlled by owner
	void shutdown(void);							// 0x000FF448

protected:
	VertexBufferClass *m_vertexBuffer;				// +0x00 vertex buffer used to draw all tracks
	DX8IndexBufferClass *m_indexBuffer;				// +0x04 indices defining triangles in maximum length track
	void *m_vertexMaterialClass;					// +0x08 vertex lighting material
	ShaderClass m_shaderClass;						// +0x0C shader or rendering state
	WaterTracksObj *m_usedModules;					// +0x10 active objects being rendered in the scene
	WaterTracksObj *m_freeModules;					// +0x14 unused modules that are free to use again
	int m_stripSizeX;								// +0x18 resolution (vertex count) of wave strip
	int m_stripSizeY;								// +0x1C resolution (vertex count) of wave strip
	int m_batchStart;								// +0x20 start of unused vertices in vertex buffer
	float m_level;									// +0x24 water level
	AsciiString m_unk0x28;							// +0x28 string member (role not recovered; the destructor 0x000FF4D6 destroys it)
};

extern WaterTracksRenderSystem *TheWaterTracksRenderSystem;	// 0x00DEC218

#endif
