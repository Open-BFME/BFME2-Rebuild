// Portions derived from Command & Conquer Generals and Command & Conquer Generals Zero Hour, (c) Electronic Arts Inc., GPL-3.0-or-later; see LICENSE and NOTICE.
// ParticleBufferClass.h: WW3D2's particle buffer render object (Zero Hour's part_buf.h): the buffer that holds the particles
// one ParticleEmitterClass emits. Target facts (retail game.dat): the copy constructor 0x001AD1B0 copies, after
// RenderObjClass's 0xC4 bytes, the new-particle queue +0xC4..+0xD0, RenderMode +0xD4, FrameMode +0xD8, Accel +0xDC (the
// three floats), HasAccel +0xE8, MaxAge +0xEC, FutureStartTime +0xF0, LastUpdateTime +0xF4 (WW3D::SyncTime), IsEmitterDead
// +0xF8, MaxSize +0xFC, MaxNum +0x100, the circular-buffer indices +0x104..+0x114, the bounding box +0x118..+0x12C and its
// dirty flag +0x130, seven keyframe groups (count, times, values, deltas; rotation has a fifth array) from +0x134, the
// default tail diffuse +0x198, seven random-table groups (count minus one, table) from +0x1A8, ColorRandom +0x1E0 and five
// more randomizers to InitialOrientationRandom +0x200, the point group +0x204, line renderer +0x208, line group +0x20C,
// two position buffers +0x210, then Diffuse, Color, Alpha, Size, Frame, UCoord, TailPosition, TailDiffuse, Orientation, APT
// and GroupID +0x218..+0x240, PingPongPosition +0x244, Velocity +0x248, TimeStamp +0x24C, Emitter +0x250,
// DecimationThreshold +0x254, LodCount +0x258 (min(MaxNum, 17)), Cost[17] +0x25C, Value[18] +0x2A0, LodBias +0x2E8,
// ProjectedArea +0x2EC (set to 0) and CurrentGroupID +0x2F0. SIZE 0x2F4. The virtual table is the one retail's
// constructor stores (slots as listed in the comments). Overridden RenderObjClass slots: 2 Clone, 3 Class_ID, 10
// Get_Num_Polys, 12 Render, 13 On_Frame_Update, 26/27 Notify_Added/Removed, 67/68 bounds, 72 Prepare_LOD, 74/75 LOD steps,
// 76..81, 82 Set_LOD_Bias, 83 Calculate_Cost_Value_Arrays, 92 Scale, 122 Is_Complete, 127 Update_Cached_Bounding_Volumes.
#pragma once
#ifndef BFME2_PARTICLEBUFFERCLASS_H
#define BFME2_PARTICLEBUFFERCLASS_H

#include "WW3D2/RenderObjClass.h"
#include "WW3D2/ShaderClass.h"
#include "WW3D2/ShareBufferClass.h"
#include "WW3D2/TextureAsset.h"
#include "WW3D2/ParticlePropertyStruct.h"
#include "WW3D2/w3d_file.h"
#include "WWMath/AABoxClass.h"
#include "WWMath/Vector2.h"
#include "WWMath/Vector3.h"
#include "WWMath/Vector4.h"

class ParticleEmitterClass;
class PointGroupClass;
class LineGroupClass;
class SegLineRendererClass;
class TextureClass;

struct NewParticleStruct
{
	Vector3 Position;			// Particle position in worldspace.
	Vector3 Velocity;			// Particle velocity in worldspace.
	unsigned int TimeStamp;		// Millisecond time at creation.
	unsigned char GroupID;		// Group ID of the particle
};

class ParticleBufferClass : public RenderObjClass
{
public:
	ParticleBufferClass(const ParticleBufferClass &src);
	virtual ~ParticleBufferClass();									// slot 1: 0x001AD190 (deleting), body 0x001A80D0

	virtual RenderObjClass *Clone() const;							// slot 2: 0x001AE470
	virtual int Class_ID() const;									// slot 3: 0x0013D320
	virtual int Get_Num_Polys() const;								// slot 10: 0x001A65A0
	virtual void Render(RenderInfoClass &rinfo);					// slot 12: 0x001AF590
	virtual void On_Frame_Update();									// slot 13: 0x001A6710
	virtual void Notify_Added(SceneClass *scene);					// slot 26: 0x001A6750
	virtual void Notify_Removed(SceneClass *scene);					// slot 27: 0x001A6770
	virtual void Get_Obj_Space_Bounding_Sphere(SphereClass &sphere) const;	// slot 67: 0x001AF400
	virtual void Get_Obj_Space_Bounding_Box(AABoxClass &box) const;			// slot 68: 0x001AF480
	virtual void Prepare_LOD(CameraClass &camera);					// slot 72: 0x001A87A0
	virtual void Increment_LOD();									// slot 74: 0x001A6790
	virtual void Decrement_LOD();									// slot 75: 0x001A67B0
	virtual float Get_Cost() const;									// slot 76: 0x001A67D0
	virtual float Get_Value() const;								// slot 77: 0x001A67F0
	virtual float Get_Post_Increment_Value() const;					// slot 78: 0x001A6810
	virtual void Set_LOD_Level(int lod);							// slot 79: 0x001A89A0
	virtual int Get_LOD_Level() const;								// slot 80: 0x001A6830
	virtual int Get_LOD_Count() const;								// slot 81: 0x001A6840
	virtual void Set_LOD_Bias(float bias);							// slot 82: 0x001A84F0
	virtual int Calculate_Cost_Value_Arrays(float screen_area, float *values, float *costs) const;	// slot 83: 0x001A6850
	virtual void Scale(float scale);								// slot 92: 0x001A65E0
	virtual bool Is_Complete();										// slot 122: 0x001A8510

	int Get_Particle_Count() const;
	void Emitter_Is_Dead();
	void Set_Emitter(ParticleEmitterClass *emitter);
	NewParticleStruct *Add_Uninitialized_New_Particle();

	// The texture of the renderer in use (retail returns the handle through a hidden pointer: 0x001A7A60).
	TextureAsset Get_Texture() const;
	ShaderClass Get_Shader() const;
	int Get_Render_Mode() const { return RenderMode; }
	int Get_Frame_Mode() const { return FrameMode; }
	float Get_Initial_Orientation_Random() const { return InitialOrientationRandom; }
	float Get_Particle_Size() const;
	Vector3 Get_Acceleration() const;
	float Get_Lifetime() const;
	float Get_Future_Start_Time() const;
	float Get_Fade_Time() const;
	void Set_Current_GroupID(unsigned char grp) { CurrentGroupID = grp; }	// emitter's Start 0x001A1B20 inlines it (row 0x001A0AE0 is retail's separate copy)
	unsigned int Get_Buffer_Size() const { return MaxNum; }

	// Line rendering properties: default values when line rendering is not enabled.
	int Get_Line_Texture_Mapping_Mode() const;
	int Is_Merge_Intersections() const;
	int Is_Freeze_Random() const;
	int Is_Sorting_Disabled() const;
	int Are_End_Caps_Enabled() const;
	int Get_Subdivision_Level() const;
	float Get_Noise_Amplitude() const;
	float Get_Merge_Abort_Factor() const;
	float Get_Texture_Tile_Factor() const;
	Vector2 Get_UV_Offset_Rate() const;

	// Global control of particle LOD.
	static void Set_LOD_Max_Screen_Size(int lod_level, float max_screen_size);
	static float Get_LOD_Max_Screen_Size(int lod_level);

protected:
	virtual void Update_Cached_Bounding_Volumes() const;			// slot 127: 0x001AF4D0 (protected in retail: ?M mangling)

	enum TailDiffuseTypeEnum {
		BLACK,
		WHITE,
		SAME_AS_HEAD,
		SAME_AS_HEAD_ALPHA_ZERO
	};

	void Render_Particles(RenderInfoClass &rinfo);
	// Builds the table of the particles to draw this frame (the active point table) and counts them.
	void Generate_APT(ShareBufferClass<unsigned int> **apt, unsigned int &active_point_count);
	void Render_Line(RenderInfoClass &rinfo);
	void Render_Line_Group(RenderInfoClass &rinfo);
	void Update_Kinematic_Particle_State();
	void Update_Visual_Particle_State();
	void Update_Bounding_Box();
	void Combine_Color_And_Alpha();
	void Get_New_Particles();
	void Kill_Old_Particles();
	void Update_Non_New_Particles(unsigned int elapsed);
	TailDiffuseTypeEnum Determine_Tail_Diffuse();

	NewParticleStruct *NewParticleQueue;				// +0xC4 the emitter's circular queue
	unsigned int NewParticleQueueStart;					// +0xC8
	unsigned int NewParticleQueueEnd;					// +0xCC
	int NewParticleQueueCount;							// +0xD0
	int RenderMode;										// +0xD4
	int FrameMode;										// +0xD8
	Vector3 Accel;										// +0xDC worldspace acceleration per ms^2
	bool HasAccel;										// +0xE8
	unsigned int MaxAge;								// +0xEC maximum age in milliseconds
	unsigned int FutureStartTime;						// +0xF0
	unsigned int LastUpdateTime;						// +0xF4
	bool IsEmitterDead;									// +0xF8
	float MaxSize;										// +0xFC used for bounding box calculations
	unsigned int MaxNum;								// +0x100
	unsigned int Start;									// +0x104
	unsigned int End;									// +0x108
	unsigned int NewEnd;								// +0x10C
	int NonNewNum;										// +0x110
	int NewNum;											// +0x114
	AABoxClass BoundingBox;								// +0x118 worldspace-aligned
	bool BoundingBoxDirty;								// +0x130

	unsigned int NumColorKeyFrames;						// +0x134
	unsigned int *ColorKeyFrameTimes;					// +0x138
	Vector3 *ColorKeyFrameValues;						// +0x13C
	Vector3 *ColorKeyFrameDeltas;						// +0x140
	unsigned int NumAlphaKeyFrames;						// +0x144
	unsigned int *AlphaKeyFrameTimes;					// +0x148
	float *AlphaKeyFrameValues;							// +0x14C
	float *AlphaKeyFrameDeltas;							// +0x150
	unsigned int NumSizeKeyFrames;						// +0x154
	unsigned int *SizeKeyFrameTimes;					// +0x158
	float *SizeKeyFrameValues;							// +0x15C (Get_Particle_Size 0x001A6560 reads [0])
	float *SizeKeyFrameDeltas;							// +0x160
	unsigned int NumRotationKeyFrames;					// +0x164
	unsigned int *RotationKeyFrameTimes;				// +0x168
	float *RotationKeyFrameValues;						// +0x16C
	float *HalfRotationKeyFrameDeltas;					// +0x170
	float *OrientationKeyFrameValues;					// +0x174
	unsigned int NumFrameKeyFrames;						// +0x178
	unsigned int *FrameKeyFrameTimes;					// +0x17C
	float *FrameKeyFrameValues;							// +0x180
	float *FrameKeyFrameDeltas;							// +0x184
	unsigned int NumBlurTimeKeyFrames;					// +0x188
	unsigned int *BlurTimeKeyFrameTimes;				// +0x18C
	float *BlurTimeKeyFrameValues;						// +0x190
	float *BlurTimeKeyFrameDeltas;						// +0x194
	Vector4 DefaultTailDiffuse;							// +0x198

	unsigned int NumRandomColorEntriesMinus1;			// +0x1A8
	Vector3 *RandomColorEntries;						// +0x1AC
	unsigned int NumRandomAlphaEntriesMinus1;			// +0x1B0
	float *RandomAlphaEntries;							// +0x1B4
	unsigned int NumRandomSizeEntriesMinus1;			// +0x1B8
	float *RandomSizeEntries;							// +0x1BC
	unsigned int NumRandomRotationEntriesMinus1;		// +0x1C0
	float *RandomRotationEntries;						// +0x1C4
	unsigned int NumRandomOrientationEntriesMinus1;		// +0x1C8
	float *RandomOrientationEntries;					// +0x1CC
	unsigned int NumRandomFrameEntriesMinus1;			// +0x1D0
	float *RandomFrameEntries;							// +0x1D4
	unsigned int NumRandomBlurTimeEntriesMinus1;		// +0x1D8
	float *RandomBlurTimeEntries;						// +0x1DC
	Vector3 ColorRandom;								// +0x1E0
	float OpacityRandom;								// +0x1EC
	float SizeRandom;									// +0x1F0
	float RotationRandom;								// +0x1F4
	float FrameRandom;									// +0x1F8
	float BlurTimeRandom;								// +0x1FC
	float InitialOrientationRandom;						// +0x200

	PointGroupClass *PointGroup;						// +0x204
	SegLineRendererClass *LineRenderer;					// +0x208
	LineGroupClass *LineGroup;							// +0x20C
	ShareBufferClass<Vector3> *Position[2];				// +0x210 only [0] unless ping-pong
	ShareBufferClass<Vector4> *Diffuse;					// +0x218
	ShareBufferClass<Vector3> *Color;					// +0x21C
	ShareBufferClass<float> *Alpha;						// +0x220
	ShareBufferClass<float> *Size;						// +0x224
	ShareBufferClass<unsigned char> *Frame;				// +0x228
	ShareBufferClass<float> *UCoord;					// +0x22C line groups only
	ShareBufferClass<Vector3> *TailPosition;			// +0x230 line groups only
	ShareBufferClass<Vector4> *TailDiffuse;				// +0x234 line groups only
	ShareBufferClass<unsigned char> *Orientation;		// +0x238
	ShareBufferClass<unsigned int> *APT;				// +0x23C
	ShareBufferClass<unsigned char> *GroupID;			// +0x240 lines only
	bool PingPongPosition;								// +0x244
	Vector3 *Velocity;									// +0x248
	unsigned int *TimeStamp;							// +0x24C
	ParticleEmitterClass *Emitter;						// +0x250
	unsigned int DecimationThreshold;					// +0x254
	static const unsigned int PermutationArray[16];
	unsigned int LodCount;								// +0x258
	float Cost[17];										// +0x25C
	float Value[18];									// +0x2A0
	float LodBias;										// +0x2E8
	float ProjectedArea;								// +0x2EC
	static unsigned int TotalActiveCount;
	static float LODMaxScreenSizes[17];
	unsigned char CurrentGroupID;						// +0x2F0
};

#endif
