// Portions derived from Command & Conquer Generals and Command & Conquer Generals Zero Hour, (c) Electronic Arts Inc., GPL-3.0-or-later; see LICENSE and NOTICE.
// ParticleEmitterDefClass.h: WW3D2's particle emitter definition (Zero Hour's part_ldr.h): the data one W3D_CHUNK_EMITTER
// describes, with its loader and saver. Target facts (retail game.dat): constructor 0x001B1350 stores the vftable
// (0x007D74E8), the name +4, user string +8, user type +0xC and version +0x10 as zero, the default shader word 0x0010441B
// +0x14, then clears W3dEmitterInfoStruct +0x18 (0x14C bytes), W3dEmitterInfoStructV2 +0x164 (0x7C), the extra info
// +0x1E0, the line properties +0x208 (0x40), the colour keyframes +0x248 (nine dwords), the opacity +0x26C, size +0x280,
// rotation +0x294 keyframes (five dwords each), the initial orientation random +0x2A8, the frame +0x2AC and blur time
// +0x2C0 keyframes and the two randomizer pointers +0x2D4 and +0x2D8. SIZE 0x2DC. The loader and saver functions return
// bool in BFME 2 (the mangled names end in _N). Slots 15 (Set_Burst_Size 0x001B15A0) and 42 (Set_Subdivision_Level
// 0x001B16C0) are the placeholder DwordSlot rows; slot 36, Set_User_Type, is shared with a COMDAT body (0x0065D790).
#pragma once
#ifndef BFME2_PARTICLEEMITTERDEFCLASS_H
#define BFME2_PARTICLEEMITTERDEFCLASS_H

#include "Common/StlAlgobase.h"
#include "WWLib/always.h"
#include "WW3D2/ShaderClass.h"
#include "WW3D2/ParticlePropertyStruct.h"
#include "WW3D2/w3d_emitter.h"
#include "WW3D2/W3dUtilityClass.h"
#include "WWMath/Vector2.h"
#include "WWMath/Vector3.h"

#define BYTE_TO_FLOAT(byte)		(((float)byte) / 255.0F)

// Zero Hour's part_ldr.h text: WWLib's by-value max (Save_Props 0x001B1970).
#define FLOAT_TO_BYTE(fval)				((unsigned char)(max(fval,0.0F) * 255.0F))

#define RGBA_TO_VECTOR3(rgba)																				Vector3 (BYTE_TO_FLOAT(rgba.R), BYTE_TO_FLOAT (rgba.G), BYTE_TO_FLOAT (rgba.B))

#define VECTOR3_TO_RGBA(vector3, rgba)			rgba.R = FLOAT_TO_BYTE (vector3.X);			rgba.G = FLOAT_TO_BYTE (vector3.Y);			rgba.B = FLOAT_TO_BYTE (vector3.Z);

class ChunkLoadClass;
class ChunkSaveClass;
class Vector3Randomizer;

class ParticleEmitterDefClass
{
public:
	ParticleEmitterDefClass();											// 0x001B1350
	ParticleEmitterDefClass(const ParticleEmitterDefClass &src);		// 0x001B1FD0
	virtual ~ParticleEmitterDefClass();									// 0x001B1770
	const ParticleEmitterDefClass &operator=(const ParticleEmitterDefClass &src);	// 0x001B17F0

	virtual bool Load_W3D(ChunkLoadClass &chunk_load);					// slot 1: 0x001AF910
	virtual bool Save_W3D(ChunkSaveClass &chunk_save);					// slot 2: 0x001B0A70

	const char *Get_Name() const { return m_pName; }
	virtual void Set_Name(const char *pname);							// slot 3: 0x001AF880

	unsigned int Get_Version() const { return m_Version; }

	int Get_Render_Mode() const { return m_InfoV2.RenderMode; }
	int Get_Frame_Mode() const { return m_InfoV2.FrameMode; }
	const char *Get_Texture_Filename() const { return m_Info.TextureFilename; }
	float Get_Lifetime() const { return m_Info.Lifetime; }
	float Get_Emission_Rate() const { return m_Info.EmissionRate; }
	float Get_Max_Emissions() const { return m_Info.MaxEmissions; }
	float Get_Fade_Time() const { return m_Info.FadeTime; }
	float Get_Gravity() const { return m_Info.Gravity; }
	float Get_Elasticity() const { return m_Info.Elasticity; }
	Vector3 Get_Velocity() const { return Vector3(m_Info.Velocity.X, m_Info.Velocity.Y, m_Info.Velocity.Z); }
	Vector3 Get_Acceleration() const { return Vector3(m_Info.Acceleration.X, m_Info.Acceleration.Y, m_Info.Acceleration.Z); }
	unsigned int Get_Burst_Size() const { return m_InfoV2.BurstSize; }
	float Get_Outward_Vel() const { return m_InfoV2.OutwardVel; }
	float Get_Vel_Inherit() const { return m_InfoV2.VelInherit; }
	float Get_Future_Start_Time() const { return m_ExtraInfo.FutureStartTime; }

	virtual void Set_Render_Mode(int mode);								// slot 4: 0x001B1460
	virtual void Set_Frame_Mode(int mode);								// slot 5: 0x001B1470
	virtual void Set_Texture_Filename(const char *pname);				// slot 6: 0x001AF8B0
	virtual void Set_Lifetime(float value);								// slot 7: 0x001B1480
	virtual void Set_Emission_Rate(float value);						// slot 8: 0x001B14A0
	virtual void Set_Max_Emissions(float value);						// slot 9: 0x001B14C0
	virtual void Set_Fade_Time(float value);							// slot 10: 0x001B14E0
	virtual void Set_Gravity(float value);								// slot 11: 0x001B1500
	virtual void Set_Elasticity(float value);							// slot 12: 0x001B1520
	virtual void Set_Velocity(const Vector3 &value);					// slot 13: 0x001B1540
	virtual void Set_Acceleration(const Vector3 &value);				// slot 14: 0x001B1570
	virtual void Set_Burst_Count(unsigned int count);					// slot 15: 0x001B15A0 (the map's ?Set_Burst_Size@ParticleEmitterDefClass at 0x00262FEB is an invented name)
	virtual void Set_Outward_Vel(float value);							// slot 16: 0x001B15B0
	virtual void Set_Vel_Inherit(float value);							// slot 17: 0x001B15D0
	virtual void Set_Future_Start_Time(float value);					// slot 18: 0x001B15F0

	void Get_Shader(ShaderClass &shader) const { W3dUtilityClass::Convert_Shader(m_InfoV2.Shader, &shader); }
	virtual void Set_Shader(const ShaderClass &shader);					// slot 19: 0x001B1610

	virtual void Set_Creation_Volume(Vector3Randomizer *randomizer);	// slot 20: 0x001AF800
	virtual void Set_Velocity_Random(Vector3Randomizer *randomizer);	// slot 21: 0x001AF7B0

	virtual void Get_Color_Keyframes(ParticlePropertyStruct<Vector3> &keyframes) const;		// slot 22: 0x001B1EF0
	virtual void Get_Opacity_Keyframes(ParticlePropertyStruct<float> &keyframes) const;		// slot 23: 0x001B1F10
	virtual void Get_Size_Keyframes(ParticlePropertyStruct<float> &keyframes) const;		// slot 24: 0x001B1F30
	virtual void Get_Rotation_Keyframes(ParticlePropertyStruct<float> &rotationframes) const;	// slot 25: 0x001B1F50
	virtual void Get_Frame_Keyframes(ParticlePropertyStruct<float> &frameframes) const;		// slot 26: 0x001B1F70
	virtual void Get_Blur_Time_Keyframes(ParticlePropertyStruct<float> &blurtimeframes) const;	// slot 27: 0x001B1F90
	virtual float Get_Initial_Orientation_Random() const;				// slot 28: 0x001B1630

	virtual void Set_Color_Keyframes(ParticlePropertyStruct<Vector3> &keyframes);			// slot 29: 0x001B1CA0
	virtual void Set_Opacity_Keyframes(ParticlePropertyStruct<float> &keyframes);			// slot 30: 0x001B1D00
	virtual void Set_Size_Keyframes(ParticlePropertyStruct<float> &keyframes);				// slot 31: 0x001B1D60
	virtual void Set_Rotation_Keyframes(ParticlePropertyStruct<float> &keyframes, float orient_rnd);	// slot 32: 0x001B1DC0
	virtual void Set_Frame_Keyframes(ParticlePropertyStruct<float> &keyframes);				// slot 33: 0x001B1E30
	virtual void Set_Blur_Time_Keyframes(ParticlePropertyStruct<float> &keyframes);			// slot 34: 0x001B1E90

	const char *Get_User_String() const { return m_pUserString; }
	int Get_User_Type() const { return m_iUserType; }
	virtual void Set_User_String(const char *pstring);					// slot 35: 0x001AF850
	virtual void Set_User_Type(int type) { m_iUserType = type; }		// slot 36: 0x0065D790

	const W3dEmitterLinePropertiesStruct *Get_Line_Properties() const { return &m_LineProperties; }
	int Get_Line_Texture_Mapping_Mode() const { return (m_LineProperties.Flags & W3D_ELINE_TEXTURE_MAP_MODE_MASK) >> W3D_ELINE_TEXTURE_MAP_MODE_OFFSET; }
	int Is_Merge_Intersections() const { return m_LineProperties.Flags & W3D_ELINE_MERGE_INTERSECTIONS; }
	int Is_Freeze_Random() const { return m_LineProperties.Flags & W3D_ELINE_FREEZE_RANDOM; }
	int Is_Sorting_Disabled() const { return m_LineProperties.Flags & W3D_ELINE_DISABLE_SORTING; }
	int Are_End_Caps_Enabled() const { return m_LineProperties.Flags & W3D_ELINE_END_CAPS; }
	int Get_Subdivision_Level() const { return m_LineProperties.SubdivisionLevel; }
	float Get_Noise_Amplitude() const { return m_LineProperties.NoiseAmplitude; }
	float Get_Merge_Abort_Factor() const { return m_LineProperties.MergeAbortFactor; }
	float Get_Texture_Tile_Factor() const { return m_LineProperties.TextureTileFactor; }
	Vector2 Get_UV_Offset_Rate() const { return Vector2(m_LineProperties.UPerSec, m_LineProperties.VPerSec); }

	virtual void Set_Line_Texture_Mapping_Mode(int mode);				// slot 37: 0x001B1750
	virtual void Set_Merge_Intersections(int onoff);					// slot 38: 0x001B1640
	virtual void Set_Freeze_Random(int onoff);							// slot 39: 0x001B1660
	virtual void Set_Disable_Sorting(int onoff);						// slot 40: 0x001B1680
	virtual void Set_End_Caps(int onoff);								// slot 41: 0x001B16A0
	virtual void Set_Subdivision_Levels(int lvl);						// slot 42: 0x001B16C0
	virtual void Set_Noise_Amplitude(float k);							// slot 43: 0x001B16D0
	virtual void Set_Merge_Abort_Factor(float k);						// slot 44: 0x001B16F0
	virtual void Set_Texture_Tile_Factor(float k);						// slot 45: 0x001B1710
	virtual void Set_UV_Offset_Rate(const Vector2 &rate);				// slot 46: 0x001B1730

protected:
	virtual bool Read_Header(ChunkLoadClass &chunk_load);				// slot 47: 0x001AFDE0
	virtual bool Read_User_Data(ChunkLoadClass &chunk_load);			// slot 48: 0x001AFE60
	virtual bool Read_Info(ChunkLoadClass &chunk_load);					// slot 49: 0x001AFF00
	virtual bool Read_InfoV2(ChunkLoadClass &chunk_load);				// slot 50: 0x001B0160
	virtual bool Read_Props(ChunkLoadClass &chunk_load);				// slot 51: 0x001B0230
	virtual bool Read_Line_Properties(ChunkLoadClass &chunk_load);		// slot 52: 0x001B06E0
	virtual bool Read_Rotation_Keyframes(ChunkLoadClass &chunk_load);	// slot 53: 0x001B0720
	virtual bool Read_Frame_Keyframes(ChunkLoadClass &chunk_load);		// slot 54: 0x001B0840
	virtual bool Read_Blur_Time_Keyframes(ChunkLoadClass &chunk_load);	// slot 55: 0x001B0930
	virtual bool Read_Extra_Info(ChunkLoadClass &chunk_load);			// slot 56: 0x001B0A20

	virtual bool Read_Color_Keyframe(ChunkLoadClass &chunk_load, float *key_time, Vector3 *value);	// slot 57: 0x001B05E0
	virtual bool Read_Opacity_Keyframe(ChunkLoadClass &chunk_load, float *key_time, float *value);	// slot 58: 0x001B0680
	virtual bool Read_Size_Keyframe(ChunkLoadClass &chunk_load, float *key_time, float *value);		// slot 59: 0x001B0680 (shared body)

	virtual bool Save_Header(ChunkSaveClass &chunk_save);				// slot 60: 0x001B0B40
	virtual bool Save_User_Data(ChunkSaveClass &chunk_save);			// slot 61: 0x001B0BC0
	virtual bool Save_Info(ChunkSaveClass &chunk_save);					// slot 62: 0x001B0C50
	virtual bool Save_InfoV2(ChunkSaveClass &chunk_save);				// slot 63: 0x001B0CA0
	virtual bool Save_Props(ChunkSaveClass &chunk_save);				// slot 64: 0x001B1970
	virtual bool Save_Line_Properties(ChunkSaveClass &chunk_save);		// slot 65: 0x001B0E50
	virtual bool Save_Frame_Keyframes(ChunkSaveClass &chunk_save);		// slot 66: 0x001B0F90
	virtual bool Save_Rotation_Keyframes(ChunkSaveClass &chunk_save);	// slot 67: 0x001B0EA0
	virtual bool Save_Blur_Time_Keyframes(ChunkSaveClass &chunk_save);	// slot 68: 0x001B1070
	virtual bool Save_Extra_Info(ChunkSaveClass &chunk_save);			// slot 69: 0x001B1150
	virtual bool Save_Color_Keyframes(ChunkSaveClass &chunk_save);		// slot 70: 0x001B1AE0
	virtual bool Save_Opacity_Keyframes(ChunkSaveClass &chunk_save);	// slot 71: 0x001B0CF0
	virtual bool Save_Size_Keyframes(ChunkSaveClass &chunk_save);		// slot 72: 0x001B0DA0
	virtual Vector3Randomizer *Create_Randomizer(W3dVolumeRandomizerStruct &info);	// slot 73: 0x001AFF60
	virtual void Initialize_Randomizer_Struct(const Vector3Randomizer &randomizer, W3dVolumeRandomizerStruct &info);	// slot 74: 0x001B00C0

	virtual void Initialize_To_Ver2();									// slot 75: 0x001AFA20
	virtual void Convert_To_Ver2();										// slot 76: 0x001AFAE0

	virtual void Normalize_Filename();									// slot 77: 0x001AF8D0
	virtual void Free_Props();											// slot 78: 0x001AF650

private:
	char *m_pName;														// +0x04
	char *m_pUserString;												// +0x08
	int m_iUserType;													// +0x0C
	unsigned int m_Version;												// +0x10
	ShaderClass m_Shader;												// +0x14
	W3dEmitterInfoStruct m_Info;										// +0x18
	W3dEmitterInfoStructV2 m_InfoV2;									// +0x164
	W3dEmitterExtraInfoStruct m_ExtraInfo;								// +0x1E0
	W3dEmitterLinePropertiesStruct m_LineProperties;					// +0x208
	ParticlePropertyStruct<Vector3> m_ColorKeyframes;					// +0x248
	ParticlePropertyStruct<float> m_OpacityKeyframes;					// +0x26C
	ParticlePropertyStruct<float> m_SizeKeyframes;						// +0x280
	ParticlePropertyStruct<float> m_RotationKeyframes;					// +0x294
	float m_InitialOrientationRandom;									// +0x2A8
	ParticlePropertyStruct<float> m_FrameKeyframes;						// +0x2AC
	ParticlePropertyStruct<float> m_BlurTimeKeyframes;					// +0x2C0
	Vector3Randomizer *m_pCreationVolume;								// +0x2D4
	Vector3Randomizer *m_pVelocityRandomizer;							// +0x2D8
};
// SIZE 0x2DC
typedef char ParticleEmitterDefClassSize[(sizeof(ParticleEmitterDefClass) == 0x2DC) ? 1 : -1];

#endif
