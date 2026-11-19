// SegLineRendererClass.h: WW3D2's segmented-line renderer (seglinerenderer.h).
// Target facts: embedded by value in SegmentedLineClass at +0xDC (constructor
// 0x00191140 runs there); 0x50 bytes (SegmentedLineClass is 0x12C). Offsets
// from SegmentedLineClass's forwarding accessors (0x0015E150-0x0015E610):
// Texture +0x0 (Set_Texture 0x001911E0 runs on it), Shader +0x4, Width +0x8,
// Color +0xC, Opacity +0x18, a BFME-only float +0x1C (set together with the
// width by 0x0015E2B0, clamped at 0), SubdivisionLevel +0x20 (the LOD
// functions clamp it to SegmentedLineClass's maximum), NoiseAmplitude +0x24
// (stored as fabs), MergeAbortFactor +0x28, TextureTileFactor +0x2C,
// UVOffsetDeltaPerMS +0x3C (Set_UV_Offset_Rate 0x0015E610 stores rate*0.001,
// the getter multiplies back by 1000), Bits +0x44 (1 merge intersections,
// 2 freeze random, 4 disable sorting, 8 end caps, texture map mode in the top
// byte). +0x30..+0x3B and +0x48..+0x4F are not read by the accessors.
#pragma once
#ifndef BFME2_SEGLINERENDERERCLASS_H
#define BFME2_SEGLINERENDERERCLASS_H

#include "WW3D2/ShaderClass.h"
#include "WW3D2/TextureBaseClass.h"
#include "WW3D2/VertexFormats.h"
#include "WW3D2/w3d_emitter.h"
#include "WWMath/Vector2.h"
#include "WWMath/Vector3.h"

class RenderInfoClass;
class Matrix3D;
class SphereClass;
class Vector4;

class StreakLineClass;

class SegLineRendererClass
{
	friend class StreakLineClass;

public:
	enum TextureMapMode
	{
		UNIFORM_WIDTH_TEXTURE_MAP = 0,
		UNIFORM_LENGTH_TEXTURE_MAP,
		TILED_TEXTURE_MAP
	};

	enum
	{
		MERGE_INTERSECTIONS = 0x00000001,
		FREEZE_RANDOM = 0x00000002,
		DISABLE_SORTING = 0x00000004,
		END_CAPS = 0x00000008,
		TEXTURE_MAP_MODE_MASK = 0xFF000000,
		TEXTURE_MAP_MODE_OFFSET = 24
	};

	SegLineRendererClass();										// 0x00191140
	void Reset_Line();											// 0x00191260
	void Scale(float scale);									// 0x00191280
	void Init(const W3dEmitterLinePropertiesStruct &props);		// 0x00191390
	void Set_Current_UV_Offset(const Vector2 &offset);			// 0x00191460
	SegLineRendererClass(const SegLineRendererClass &src);		// 0x00191B50
	~SegLineRendererClass();									// 0x001911C0
	SegLineRendererClass &operator=(const SegLineRendererClass &that);	// 0x001912F0

	void Set_Texture(TextureClass *texture);					// 0x001911E0
	void Render(RenderInfoClass &rinfo, const Matrix3D &transform, unsigned int point_count, Vector3 *points, const SphereClass &obj_sphere, Vector4 *rgbas = 0);	// 0x00191C10
	void Set_Texture_Tile_Factor(float factor);					// 0x00191230
	void Set_UV_Offset_Rate(const Vector2 &rate);				// 0x0015E610

	ShaderClass Get_Shader() const { return Shader; }
	void Set_Shader(ShaderClass shader) { Shader = shader; }
	float Get_Width() const { return Width; }
	void Set_Width(float width) { Width = width; }
	const Vector3 &Get_Color() const { return Color; }
	void Set_Color(const Vector3 &color) { Color = color; }
	float Get_Opacity() const { return Opacity; }
	void Set_Opacity(float opacity) { Opacity = opacity; }
	unsigned int Get_Current_Subdivision_Level() const { return SubdivisionLevel; }
	void Set_Current_Subdivision_Level(unsigned int level) { SubdivisionLevel = level; }
	float Get_Noise_Amplitude() const { return NoiseAmplitude; }
	void Set_Noise_Amplitude(float amplitude) { NoiseAmplitude = amplitude; }
	float Get_Merge_Abort_Factor() const { return MergeAbortFactor; }
	void Set_Merge_Abort_Factor(float factor) { MergeAbortFactor = factor; }
	float Get_Texture_Tile_Factor() const { return TextureTileFactor; }
	Vector2 Get_UV_Offset_Rate() const { return Vector2(UVOffsetDeltaPerMS.X * 1000.0f, UVOffsetDeltaPerMS.Y * 1000.0f); }

	TextureMapMode Get_Texture_Mapping_Mode() const { return (TextureMapMode)((Bits & TEXTURE_MAP_MODE_MASK) >> TEXTURE_MAP_MODE_OFFSET); }
	void Set_Texture_Mapping_Mode(TextureMapMode mode)
	{
		Bits &= ~TEXTURE_MAP_MODE_MASK;
		Bits |= (mode << TEXTURE_MAP_MODE_OFFSET);
	}
	int Is_Merge_Intersections() const { return Bits & MERGE_INTERSECTIONS; }
	void Set_Merge_Intersections(int onoff) { if (onoff) Bits |= MERGE_INTERSECTIONS; else Bits &= ~MERGE_INTERSECTIONS; }
	int Is_Freeze_Random() const { return Bits & FREEZE_RANDOM; }
	void Set_Freeze_Random(int onoff) { if (onoff) Bits |= FREEZE_RANDOM; else Bits &= ~FREEZE_RANDOM; }
	int Is_Sorting_Disabled() const { return Bits & DISABLE_SORTING; }
	void Set_Disable_Sorting(int onoff) { if (onoff) Bits |= DISABLE_SORTING; else Bits &= ~DISABLE_SORTING; }
	int Are_End_Caps_Enabled() const { return Bits & END_CAPS; }
	void Set_End_Caps(int onoff) { if (onoff) Bits |= END_CAPS; else Bits &= ~END_CAPS; }

	friend class SegmentedLineClass;
	friend class StreakLineClass;

private:
	VertexFormatXYZDUV1 *getVertexBuffer(unsigned int number);	// 0x001912B0

	TextureClass Texture;					// +0x00 the handle (one factory pointer); released by the destructor
	ShaderClass Shader;						// +0x04
	float Width;							// +0x08
	Vector3 Color;							// +0x0C
	float Opacity;							// +0x18
	float EndWidth;							// +0x1C BFME-only, set with the width; name: structural
	unsigned int SubdivisionLevel;			// +0x20
	float NoiseAmplitude;					// +0x24
	float MergeAbortFactor;					// +0x28
	float TextureTileFactor;				// +0x2C
	unsigned int LastUsedSyncTime;			// +0x30 name: donor (Westwood)
	Vector2 CurrentUVOffset;				// +0x34 name: donor (Westwood)
	Vector2 UVOffsetDeltaPerMS;				// +0x3C
	unsigned int Bits;						// +0x44
	unsigned int m_vertexBufferSize;		// +0x48 cleared by the constructor 0x00191140
	VertexFormatXYZDUV1 *m_vertexBuffer;	// +0x4C freed by the destructor 0x001911C0
};
// SIZE 0x50
typedef char SegLineRendererClassSize[(sizeof(SegLineRendererClass) == 0x50) ? 1 : -1];

#endif
