// Portions derived from Command & Conquer Generals and Command & Conquer Generals Zero Hour, (c) Electronic Arts Inc., GPL-3.0-or-later; see LICENSE and NOTICE.
// ParticleEmitterClass.h: WW3D2's particle emitter render object (Zero Hour's part_emt.h): it emits particles regularly into
// a ParticleBufferClass, one buffer per emitter. Target facts (retail game.dat): the copy constructor 0x001A1530 copies,
// after RenderObjClass's 0xC4 bytes, EmitRate +0xC4, BurstSize +0xC8, OneTimeBurstSize +0xCC, OneTimeBurst +0xD0, a clone
// of PosRand +0xD4 (randomizer slot 5), BaseVel +0xD8, a clone of VelRand +0xE4, OutwardVel +0xE8, VelInheritFactor +0xEC,
// EmitRemain +0xF0, PrevQ +0xF4, PrevOrig +0x104, sets Active +0x110, FirstTime +0x111 and BufferSceneNeeded +0x112 to
// true, copies ParticlesLeft +0x114 and MaxParticles +0x118, clears IsComplete +0x11C, duplicates NameString +0x120 and
// UserString +0x124, copies RemoveOnComplete +0x128, clears IsInScene +0x129 and GroupID +0x12A, clones the buffer into
// Buffer +0x12C and copies IsInvisible +0x130. SIZE 0x134. New virtual slots: 129 Emit (+0x204), 130 and 131 the
// user type/string accessors (shared stub 0x0065CE90), 132 Update_On_Visibilty (+0x210).
#pragma once
#ifndef BFME2_PARTICLEEMITTERCLASS_H
#define BFME2_PARTICLEEMITTERCLASS_H

#include "WW3D2/RenderObjClass.h"
#include "WW3D2/ShaderClass.h"
#include "WW3D2/ParticlePropertyStruct.h"
#include "WW3D2/w3d_emitter.h"
#include "WWMath/Quaternion.h"
#include "WWMath/Vector2.h"
#include "WWMath/Vector3.h"

class ParticleBufferClass;
class ParticleEmitterDefClass;
class Vector3Randomizer;
class ChunkSaveClass;
struct NewParticleStruct;

class ParticleEmitterClass : public RenderObjClass
{
public:
	ParticleEmitterClass(const ParticleEmitterClass &src);				// 0x001A1530
	ParticleEmitterClass &operator=(const ParticleEmitterClass &that);
	virtual ~ParticleEmitterClass();									// slot 1: 0x001A1720

	virtual RenderObjClass *Clone() const;								// slot 2: 0x001A1800
	static ParticleEmitterClass *Create_From_Definition(const ParticleEmitterDefClass &definition);	// 0x001A22F0
	ParticleEmitterDefClass *Build_Definition() const;					// 0x001A2B00
	bool Save(ChunkSaveClass &chunk_save) const;						// 0x001A30D0

	virtual int Class_ID() const { return CLASSID_PARTICLEEMITTER; }	// slot 3: 0x001A13F0
	virtual const char *Get_Name() const { return NameString; }		// slot 6: 0x0019B6E0
	virtual void Set_Name(const char *pname);							// slot 7: 0x001A2220
	virtual void Notify_Added(SceneClass *scene);						// slot 26: 0x001A1860
	virtual void Notify_Removed(SceneClass *scene);						// slot 27: 0x001A18A0
	virtual void Render(RenderInfoClass &rinfo) {}						// slot 12
	virtual void Scale(float scale);									// slot 92: 0x001A18D0
	virtual void On_Frame_Update();										// slot 13: 0x001A1970
	virtual void Restart();												// slot 14: 0x001A2660
	virtual void Get_Obj_Space_Bounding_Sphere(SphereClass &sphere) const;	// slot 67: 0x001A1400
	virtual void Set_Hidden(int onoff);									// slot 101: 0x001A1450
	// slot 99 (0x001A1480) overrides RenderObjClass's Set_Visible(int, int), which RenderObjClass.h spells SetVisible(SceneClass *, int): left for the owner
	virtual void Set_Animation_Hidden(int onoff);						// slot 103: 0x001A14B0
	virtual void Set_Force_Visible(int onoff);							// slot 105: 0x001A14E0
	virtual bool Is_Complete() { return IsComplete; }					// slot 122: 0x001A1520

	void Reset();														// 0x001A1A80
	void Start();														// 0x001A1B20
	void Stop();														// 0x001A1BD0
	bool Is_Stopped();													// 0x001A1BE0
	// Accessors that forward to the buffer or read the emitter's own members (out of line in retail: O001A0C30).
	int Get_Render_Mode() const;										// 0x001A0C40
	int Get_Frame_Mode() const;											// 0x001A0C50
	Vector3 Get_Acceleration() const;									// 0x001A0C60
	float Get_Lifetime() const;											// 0x001A0CB0
	float Get_Future_Start_Time() const;								// 0x001A0CE0
	float Get_Fade_Time() const;										// 0x001A0D40
	float Get_Emission_Rate() const;									// 0x001A0D80
	int Get_Max_Particles() const;										// 0x001A0DA0
	Vector3 Get_Start_Velocity() const;									// 0x001A0DB0
	float Get_Outwards_Vel() const;										// 0x001A0E00
	float Get_Velocity_Inherit() const;									// 0x001A0E10
	ShaderClass Get_Shader() const;										// 0x001A0E20
	float Get_Initial_Orientation_Random() const;						// 0x001A0EA0
	int Get_Line_Texture_Mapping_Mode() const;							// 0x001A0EB0
	int Is_Merge_Intersections() const;									// 0x001A0EC0
	int Is_Freeze_Random() const;										// 0x001A0ED0
	int Is_Sorting_Disabled() const;									// 0x001A0EE0
	int Are_End_Caps_Enabled() const;									// 0x001A0EF0
	int Get_Subdivision_Level() const;									// 0x001A0F00
	float Get_Noise_Amplitude() const;									// 0x001A0F10
	float Get_Merge_Abort_Factor() const;								// 0x001A0F20
	float Get_Texture_Tile_Factor() const;								// 0x001A0F30
	Vector2 Get_UV_Offset_Rate() const;									// 0x001A0F40
	bool Is_Remove_On_Complete_Enabled() { return RemoveOnComplete; }
	void Enable_Remove_On_Complete(bool onoff) { RemoveOnComplete = onoff; }

	void Set_Position_Randomizer(Vector3Randomizer *rand);				// 0x001A1BF0
	void Set_Velocity_Randomizer(Vector3Randomizer *rand);				// 0x001A1C40
	Vector3Randomizer *Get_Creation_Volume() const;						// 0x001A1C90
	Vector3Randomizer *Get_Velocity_Random() const;						// 0x001A1CB0
	void Set_Base_Velocity(const Vector3 &base_vel);					// 0x001A1CE0
	void Set_Outwards_Velocity(float out_vel);							// 0x001A1D30
	void Set_Velocity_Inheritance_Factor(float inh_factor);				// 0x001A1D50

	virtual void Emit();												// slot 129: 0x001A2970
	virtual int Get_User_Type() const { return EMITTER_TYPEID_DEFAULT; }	// slot 130 (shared stub 0x0065CE90)
	virtual const char *Get_User_String() const { return 0; }			// slot 131 (shared stub 0x0065CE90)

protected:
	virtual void Update_On_Visibilty();									// slot 132: 0x001A2260

private:
	virtual void Update_Cached_Bounding_Volumes() const;				// slot 127: 0x001A1D70
	void Create_New_Particles(const Quaternion &curr_quat, const Vector3 &curr_orig);	// 0x001A2670
	void Initialize_Particle(NewParticleStruct *newpart, unsigned int age, const Quaternion &quat, const Vector3 &orig);	// 0x001A1DE0

	unsigned int EmitRate;												// +0xC4
	unsigned int BurstSize;												// +0xC8
	unsigned int OneTimeBurstSize;										// +0xCC
	bool OneTimeBurst;													// +0xD0
	Vector3Randomizer *PosRand;											// +0xD4
	Vector3 BaseVel;													// +0xD8
	Vector3Randomizer *VelRand;											// +0xE4
	float OutwardVel;													// +0xE8
	float VelInheritFactor;												// +0xEC
	unsigned int EmitRemain;											// +0xF0
	Quaternion PrevQ;													// +0xF4
	Vector3 PrevOrig;													// +0x104
	bool Active;														// +0x110
	bool FirstTime;														// +0x111
	bool BufferSceneNeeded;												// +0x112
	int ParticlesLeft;													// +0x114
	int MaxParticles;													// +0x118
	bool IsComplete;													// +0x11C
	char *NameString;													// +0x120
	char *UserString;													// +0x124
	bool RemoveOnComplete;												// +0x128
	bool IsInScene;														// +0x129
	unsigned char GroupID;												// +0x12A
	ParticleBufferClass *Buffer;										// +0x12C
	bool IsInvisible;													// +0x130
};
// SIZE 0x134
typedef char ParticleEmitterClassSize[(sizeof(ParticleEmitterClass) == 0x134) ? 1 : -1];

#endif
