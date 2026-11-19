// mapper.h: WW3D2's texture-coordinate mappers (mapper.cpp), the classes a VertexMaterialClass holds per stage.
// Class list, hierarchy and member order from Zero Hour's mapper.h; retail differs in three recorded ways:
// no W3DMPO base (BFME 2's Clone is a plain `new`), TextureMapperClass has the extra slot 8, and
// GridTextureMapperClass carries one more dword before Sign (the two GridWSEnvMapperClass constructors
// 0x00186DA0 and 0x00186E30 store LastFrame at +0x20, Offset at +0x24 and the axis at +0x34 where Zero Hour's
// layout would put them at +0x1C, +0x20 and +0x30). Mapper ids are Zero Hour's enum (retail slot 2 of the scale
// mapper 0x001820A0 returns 8, of the grid mapper 0x001820B0 returns 9).
// Portions derived from Command & Conquer Generals Zero Hour, (c) Electronic Arts Inc., GPL-3.0-or-later; see LICENSE and NOTICE.
#pragma once
#ifndef BFME2_MAPPER_H
#define BFME2_MAPPER_H

#include "WW3D2/TextureMapperClass.h"
#include "WWMath/Vector2.h"
#include "WWMath/Vector3.h"

class INIClass;
class RenderObjClass;

class ScaleTextureMapperClass : public TextureMapperClass
{
public:
	ScaleTextureMapperClass(const Vector2 &scale, unsigned int stage);
	ScaleTextureMapperClass(const INIClass &ini, const char *section, unsigned int stage);
	ScaleTextureMapperClass(const ScaleTextureMapperClass &src);

	virtual int Mapper_ID(void) const { return MAPPER_ID_SCALE; }
	virtual TextureMapperClass *Clone(void) const;
	virtual void Apply(int uv_array_index);
	virtual bool Convert_To_FXShader(FXShaderParameterList *parameters);	// 0x00187690
	virtual void Calculate_Texture_Matrix(Matrix4 &tex_matrix);

protected:
	Vector2 Scale;		// +0x0C
};

class LinearOffsetTextureMapperClass : public ScaleTextureMapperClass
{
public:
	LinearOffsetTextureMapperClass(const Vector2 &offset_per_sec, const Vector2 &start_offset, bool clamp_fix, const Vector2 &scale, unsigned int stage);
	LinearOffsetTextureMapperClass(const INIClass &ini, const char *section, unsigned int stage);
	LinearOffsetTextureMapperClass(const LinearOffsetTextureMapperClass &src);

	virtual int Mapper_ID(void) const { return MAPPER_ID_LINEAR_OFFSET; }
	virtual TextureMapperClass *Clone(void) const;
	virtual bool Is_Time_Variant(void) { return true; }
	virtual void Reset(void);
	virtual bool Convert_To_FXShader(FXShaderParameterList *parameters);	// 0x00187850
	virtual void Calculate_Texture_Matrix(Matrix4 &tex_matrix);

	void Set_Current_UV_Offset(const Vector2 &cur) { CurrentUVOffset = cur; }
	void Set_UV_Offset_Delta(const Vector2 &per_sec) { UVOffsetDeltaPerMS = per_sec; UVOffsetDeltaPerMS.X *= -0.001f; UVOffsetDeltaPerMS.Y *= -0.001f; }
	void Get_Current_UV_Offset(Vector2 &cur) { cur = CurrentUVOffset; }
	void Set_LastUsedSyncTime(unsigned int time) { LastUsedSyncTime = time; }
	unsigned int Get_LastUsedSyncTime() { return LastUsedSyncTime; }

protected:
	Vector2 CurrentUVOffset;		// +0x14
	Vector2 UVOffsetDeltaPerMS;		// +0x1C
	unsigned int LastUsedSyncTime;		// +0x24
	Vector2 StartingUVOffset;		// +0x28
	bool ClampFix;				// +0x30
};

class GridTextureMapperClass : public TextureMapperClass
{
public:
	GridTextureMapperClass(float fps, unsigned int gridwidth_log2, unsigned int last_frame, unsigned int offset, unsigned int stage);
	GridTextureMapperClass(const INIClass &ini, const char *section, unsigned int stage);
	GridTextureMapperClass(const GridTextureMapperClass &src);

	virtual int Mapper_ID(void) const { return MAPPER_ID_GRID; }
	virtual TextureMapperClass *Clone(void) const;
	virtual bool Is_Time_Variant(void) { return true; }
	virtual void Apply(int uv_array_index);
	virtual void Reset(void);
	virtual bool Convert_To_FXShader(FXShaderParameterList *parameters);	// 0x00187AD0
	virtual void Calculate_Texture_Matrix(Matrix4 &tex_matrix);

	void Set_Frame(unsigned int frame) { CurrentFrame = frame; }
	void Set_Frame_Per_Second(float fps);

protected:
	void initialize(float fps, unsigned int gridwidth_log2);
	void update_temporal_state(void);
	void calculate_uv_offset(float *u_offset, float *v_offset);

	float FPS;				// +0x0C: initialize 0x00182480 stores its fps argument here first (Zero Hour keeps no copy)
	int Sign;				// +0x10: +1 if the frame rate is positive, -1 otherwise
	unsigned int MSPerFrame;		// +0x14
	float OOGridWidth;			// +0x18
	unsigned int GridWidthLog2;		// +0x1C
	unsigned int LastFrame;			// +0x20
	unsigned int Offset;			// +0x24
	unsigned int Remainder;			// +0x28
	unsigned int CurrentFrame;		// +0x2C
	unsigned int LastUsedSyncTime;		// +0x30
};

// The grid environment mappers have the GridTextureMapperClass layout.
// Their tables share an environment matrix calculation and use distinct Apply slots.
class GridClassicEnvironmentMapperClass : public GridTextureMapperClass
{
public:
	GridClassicEnvironmentMapperClass(const INIClass &ini, const char *section, unsigned int stage);
	virtual int Mapper_ID(void) const { return MAPPER_ID_GRID_CLASSIC_ENVIRONMENT; }
	virtual TextureMapperClass *Clone(void) const;
	virtual void Apply(int uv_array_index);
	virtual bool Needs_Normals(void) { return true; }
	virtual void Calculate_Texture_Matrix(Matrix4 &tex_matrix);
};

class GridEnvironmentMapperClass : public GridTextureMapperClass
{
public:
	GridEnvironmentMapperClass(const INIClass &ini, const char *section, unsigned int stage);
	virtual int Mapper_ID(void) const { return MAPPER_ID_GRID_ENVIRONMENT; }
	virtual TextureMapperClass *Clone(void) const;
	virtual void Apply(int uv_array_index);
	virtual bool Needs_Normals(void) { return true; }
	virtual void Calculate_Texture_Matrix(Matrix4 &tex_matrix);
};

class RotateTextureMapperClass : public ScaleTextureMapperClass
{
public:
	RotateTextureMapperClass(float rad_per_sec, const Vector2 &center, const Vector2 &scale, unsigned int stage);
	RotateTextureMapperClass(const INIClass &ini, const char *section, unsigned int stage);
	RotateTextureMapperClass(const RotateTextureMapperClass &src);

	virtual int Mapper_ID(void) const { return MAPPER_ID_ROTATE; }
	virtual TextureMapperClass *Clone(void) const;
	virtual bool Is_Time_Variant(void) { return true; }
	virtual void Reset(void);
	virtual void Calculate_Texture_Matrix(Matrix4 &tex_matrix);

private:
	float CurrentAngle;
	float RadiansPerMilliSec;
	Vector2 Center;
	unsigned int LastUsedSyncTime;
};

class SineLinearOffsetTextureMapperClass : public ScaleTextureMapperClass
{
public:
	SineLinearOffsetTextureMapperClass(const Vector3 &uafp, const Vector3 &vafp, const Vector2 &scale, unsigned int stage);
	SineLinearOffsetTextureMapperClass(const INIClass &ini, const char *section, unsigned int stage);
	SineLinearOffsetTextureMapperClass(const SineLinearOffsetTextureMapperClass &src);

	virtual int Mapper_ID(void) const { return MAPPER_ID_SINE_LINEAR_OFFSET; }
	virtual TextureMapperClass *Clone(void) const;
	virtual bool Is_Time_Variant(void) { return true; }
	virtual void Reset(void);
	virtual void Calculate_Texture_Matrix(Matrix4 &tex_matrix);

private:
	Vector3 UAFP;			// U coordinate amplitude, frequency, phase
	Vector3 VAFP;			// V coordinate amplitude, frequency, phase
	float CurrentAngle;
	unsigned int LastUsedSyncTime;
};

class StepLinearOffsetTextureMapperClass : public ScaleTextureMapperClass
{
public:
	StepLinearOffsetTextureMapperClass(const Vector2 &step, float steps_per_sec, bool clamp_fix, const Vector2 &scale, unsigned int stage);
	StepLinearOffsetTextureMapperClass(const INIClass &ini, const char *section, unsigned int stage);
	StepLinearOffsetTextureMapperClass(const StepLinearOffsetTextureMapperClass &src);

	virtual int Mapper_ID(void) const { return MAPPER_ID_STEP_LINEAR_OFFSET; }
	virtual TextureMapperClass *Clone(void) const;
	virtual bool Is_Time_Variant(void) { return true; }
	virtual void Reset(void);
	virtual void Calculate_Texture_Matrix(Matrix4 &tex_matrix);

private:
	Vector2 Step;
	float StepsPerMilliSec;
	Vector2 CurrentStep;
	float Remainder;
	unsigned int LastUsedSyncTime;
	bool ClampFix;
};

class ZigZagLinearOffsetTextureMapperClass : public ScaleTextureMapperClass
{
public:
	ZigZagLinearOffsetTextureMapperClass(const Vector2 &speed, float period, const Vector2 &scale, unsigned int stage);
	ZigZagLinearOffsetTextureMapperClass(const INIClass &ini, const char *section, unsigned int stage);
	ZigZagLinearOffsetTextureMapperClass(const ZigZagLinearOffsetTextureMapperClass &src);

	virtual int Mapper_ID(void) const { return MAPPER_ID_ZIGZAG_LINEAR_OFFSET; }
	virtual TextureMapperClass *Clone(void) const;
	virtual bool Is_Time_Variant(void) { return true; }
	virtual void Reset(void);
	virtual void Calculate_Texture_Matrix(Matrix4 &tex_matrix);

private:
	Vector2 Speed;
	float Period;
	float Half_Period;
	float Remainder;
	unsigned int LastUsedSyncTime;
};

class EdgeMapperClass : public TextureMapperClass
{
public:
	EdgeMapperClass(unsigned int stage);
	EdgeMapperClass(const INIClass &ini, const char *section, unsigned int stage);
	EdgeMapperClass(const EdgeMapperClass &src);

	virtual int Mapper_ID(void) const { return MAPPER_ID_EDGE; }
	virtual TextureMapperClass *Clone(void) const;
	virtual void Apply(int uv_array_index);
	virtual void Reset(void);
	virtual bool Is_Time_Variant(void) { return true; }
	virtual bool Needs_Normals(void) { return true; }
	virtual void Calculate_Texture_Matrix(Matrix4 &tex_matrix);

protected:
	unsigned int LastUsedSyncTime;
	float VSpeed, VOffset;
	bool UseReflect;
};

class RandomTextureMapperClass : public ScaleTextureMapperClass
{
public:
	RandomTextureMapperClass(float fps, const Vector2 &scale, unsigned int stage);
	RandomTextureMapperClass(const INIClass &ini, const char *section, unsigned int stage);
	RandomTextureMapperClass(const RandomTextureMapperClass &src);

	virtual int Mapper_ID(void) const { return MAPPER_ID_RANDOM; }
	virtual TextureMapperClass *Clone(void) const;
	virtual bool Is_Time_Variant(void) { return true; }
	virtual void Reset(void);
	virtual void Calculate_Texture_Matrix(Matrix4 &tex_matrix);

protected:
	void randomize(void);

	float FPMS;			// frames per millisecond
	float Remainder;
	float CurrentAngle;
	Vector2 Center;
	Vector2 Speed;
	unsigned int LastUsedSyncTime;
};

// The non-grid world-space mappers carry the axis immediately after Stage.
// Clone 0x0013D2B0 copies both words and allocates 0x10 bytes.
class WSEnvMapperClass : public TextureMapperClass
{
public:
	enum AxisType { AXISTYPE_X = 0, AXISTYPE_Y = 1, AXISTYPE_Z = 2 };
	WSEnvMapperClass(AxisType axis, unsigned int stage) : TextureMapperClass(stage), Axis(axis) {}
	WSEnvMapperClass(const INIClass &ini, const char *section, unsigned int stage);	// 0x00183620

	virtual bool Needs_Normals(void) { return true; }
	virtual void Calculate_Texture_Matrix(Matrix4 &tex_matrix);	// 0x001854E0

protected:
	AxisType Axis;	// +0x0C
};

class WSClassicEnvironmentMapperClass : public WSEnvMapperClass
{
public:
	WSClassicEnvironmentMapperClass(AxisType axis, unsigned int stage) : WSEnvMapperClass(axis, stage) {}
	WSClassicEnvironmentMapperClass(const INIClass &ini, const char *section, unsigned int stage);

	virtual int Mapper_ID(void) const { return MAPPER_ID_WS_CLASSIC_ENVIRONMENT; }
	virtual TextureMapperClass *Clone(void) const;	// 0x0013C6D0
	virtual void Apply(int uv_array_index);			// slot 5: 0x00184AD0
};

class WSEnvironmentMapperClass : public WSEnvMapperClass
{
public:
	WSEnvironmentMapperClass(AxisType axis, unsigned int stage) : WSEnvMapperClass(axis, stage) {}
	WSEnvironmentMapperClass(const INIClass &ini, const char *section, unsigned int stage) :
		WSEnvMapperClass(ini, section, stage) {}

	virtual int Mapper_ID(void) const { return MAPPER_ID_WS_ENVIRONMENT; }
	virtual TextureMapperClass *Clone(void) const;	// 0x0013D2B0
	virtual void Apply(int uv_array_index);			// slot 5: 0x00185980
};

typedef char WSEnvMapperClassSize[(sizeof(WSEnvMapperClass) == 0x10) ? 1 : -1];
typedef char WSClassicEnvironmentMapperClassSize[(sizeof(WSClassicEnvironmentMapperClass) == 0x10) ? 1 : -1];
typedef char WSEnvironmentMapperClassSize[(sizeof(WSEnvironmentMapperClass) == 0x10) ? 1 : -1];

// The grid world-space environment mappers: GridWSEnvMapperClass stores its axis at +0x34 (constructors 0x00186DA0 and
// 0x00186E30), after Grid's extra dword and CurrentFrame/LastUsedSyncTime. Apply is 0x00184AD0 and 0x00185980.
class GridWSEnvMapperClass : public GridTextureMapperClass
{
public:
	enum AxisType { AXISTYPE_X = 0, AXISTYPE_Y = 1, AXISTYPE_Z = 2 };
	GridWSEnvMapperClass(float fps, unsigned int gridwidth_log2, unsigned int last_frame, unsigned int offset, AxisType axis, unsigned int stage);	// 0x00186DA0
	GridWSEnvMapperClass(const GridWSEnvMapperClass &src);											// 0x00186E00
	GridWSEnvMapperClass(const INIClass &ini, const char *section, unsigned int stage);				// 0x00186E30
	virtual void Calculate_Texture_Matrix(Matrix4 &tex_matrix);										// 0x00186F10
	virtual bool Needs_Normals(void) { return true; }

protected:
	AxisType Axis;		// +0x34
};

class GridWSClassicEnvironmentMapperClass : public GridWSEnvMapperClass
{
public:
	GridWSClassicEnvironmentMapperClass(float fps, unsigned int gridwidth_log2, unsigned int last_frame, unsigned int offset, AxisType axis, unsigned int stage);	// 0x001873A0
	GridWSClassicEnvironmentMapperClass(const INIClass &ini, const char *section, unsigned int stage);	// 0x00187410
	GridWSClassicEnvironmentMapperClass(const GridWSEnvMapperClass &src);									// 0x00187440

	virtual int Mapper_ID(void) const { return MAPPER_ID_GRID_WS_CLASSIC_ENVIRONMENT; }
	virtual TextureMapperClass *Clone(void) const;
	virtual void Apply(int uv_array_index);																// 0x00184AD0
};

class GridWSEnvironmentMapperClass : public GridWSEnvMapperClass
{
public:
	GridWSEnvironmentMapperClass(float fps, unsigned int gridwidth_log2, unsigned int last_frame, unsigned int offset, AxisType axis, unsigned int stage);	// 0x00187470
	GridWSEnvironmentMapperClass(const INIClass &ini, const char *section, unsigned int stage);			// 0x001874E0
	GridWSEnvironmentMapperClass(const GridWSEnvMapperClass &src);										// 0x00187510

	virtual int Mapper_ID(void) const { return MAPPER_ID_GRID_WS_ENVIRONMENT; }
	virtual TextureMapperClass *Clone(void) const;
	virtual void Apply(int uv_array_index);																// 0x00185980
};

// The screen-space mapper (constructors 0x0013DA40 family, Calculate_Texture_Matrix 0x00186380, Apply 0x00185E80).
class ScreenMapperClass : public LinearOffsetTextureMapperClass
{
public:
	ScreenMapperClass(const Vector2 &offset_per_sec, const Vector2 &start_offset, bool clamp_fix, const Vector2 &scale, unsigned int stage);
	ScreenMapperClass(const INIClass &ini, const char *section, unsigned int stage);
	ScreenMapperClass(const ScreenMapperClass &src) : LinearOffsetTextureMapperClass(src) {}

	virtual int Mapper_ID(void) const { return MAPPER_ID_SCREEN; }
	virtual TextureMapperClass *Clone(void) const;
	virtual void Apply(int uv_array_index);															// 0x00185E80
	virtual void Calculate_Texture_Matrix(Matrix4 &tex_matrix);										// 0x00186380
};

// BumpEnv: Apply 0x00186A90 reads LastUsedSyncTime, CurrentAngle, RadiansPerSecond and ScaleFactor at +0x34..+0x40.
class BumpEnvTextureMapperClass : public LinearOffsetTextureMapperClass
{
public:
	BumpEnvTextureMapperClass(float rad_per_sec, float scale_factor, const Vector2 &offset_per_sec, const Vector2 &start_offset, bool clamp_fix, const Vector2 &scale, unsigned int stage);	// 0x00186940
	BumpEnvTextureMapperClass(INIClass &ini, char *section, unsigned int stage);					// 0x001869B0
	BumpEnvTextureMapperClass(const BumpEnvTextureMapperClass &src);								// 0x00186A50

	virtual int Mapper_ID(void) const { return MAPPER_ID_BUMPENV; }
	virtual TextureMapperClass *Clone(void) const;													// 0x001875E0
	virtual void Apply(int uv_array_index);															// 0x00186A90

protected:
	unsigned int LastUsedSyncTime;		// +0x34
	float CurrentAngle;					// +0x38
	float RadiansPerSecond;				// +0x3C
	float ScaleFactor;					// +0x40
};

#endif
