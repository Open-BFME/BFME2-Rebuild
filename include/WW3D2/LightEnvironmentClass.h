// LightEnvironmentClass.h: WW3D's per-object lighting summary (up to four
// input lights reduced to output directional lights plus an ambient term).
//
// Target facts:
// - The constructor 0x0013F410 clears the byte +0, the count +4 and the
//   object centre +8..+0x10, then four 0x54-byte InputLightStruct at +0x14
//   (+0x24 and +0x25 bytes cleared, every float zeroed), the output ambient
//   +0x164..+0x16C, four 0x18-byte OutputLightStruct at +0x170 and a BFME
//   tail +0x1D0..+0x227 (floats, bytes +0x1F4/+0x1F5). SIZE 0x228 (MeshClass
//   embeds one at +0xCC; its next member is at +0x2F4).
// - InputLightStruct (getters 0x0011C320..0x0011C400 index it at stride 0x54
//   from +0x14): Direction +0, Ambient +0xC, Diffuse +0x18 (Contribution
//   0x0013F2E0 is its squared length), +0x24 a flag, IsPointLight +0x25,
//   Center +0x28, inner/outer radius +0x34/+0x38, point ambient +0x3C,
//   point diffuse +0x48 (the getters 0x0011C3E0 and 0x0011C3C0 return them). Init 0x00140730 clears IsPointLight and dispatches on
//   the light's type: directional to Init_From_Directional_Light 0x0013F250,
//   point and spot to Init_From_Point_Or_Spot_Light 0x0013FB60.
// - Set_Output_Ambient 0x0006E136 copies a Vector3 to +0x164.
// - The LOD cutoff is a pair of static ints (0x009F3408 and its square at
//   0x009F340C): Set_Lighting_LOD_Cutoff 0x0013F7B0 truncates its float,
//   Get_Lighting_LOD_Cutoff 0x0013F7D0 returns it as float.
// Names of the +0 byte, the +0x24 flag and the tail are not recovered.
#pragma once
#ifndef BFME2_LIGHTENVIRONMENTCLASS_H
#define BFME2_LIGHTENVIRONMENTCLASS_H

#include "WWMath/Vector3.h"

class LightClass;
class Matrix3D;

class LightEnvironmentClass
{
public:
	enum { MAX_LIGHTS = 4 };

	struct InputLightStruct
	{
		void Init(const LightClass &light, const Vector3 &object_center);					// 0x00140730
		void Init_From_Directional_Light(const LightClass &light, const Vector3 &object_center);		// 0x0013F250
		void Init_From_Point_Or_Spot_Light(const LightClass &light, const Vector3 &object_center);	// 0x0013FB60
		float Contribution(void);										// 0x0013F2E0

		Vector3 Direction;		// +0x00
		Vector3 Ambient;		// +0x0C
		Vector3 Diffuse;		// +0x18
		bool Flag24;			// +0x24 cleared by Init_From_Directional_Light; role not seen yet
		bool IsPointLight;		// +0x25
		Vector3 Center;			// +0x28
		float InnerRadius;		// +0x34
		float OuterRadius;		// +0x38
		Vector3 PointAmbient;		// +0x3C (getPointAmbient 0x0011C3E0 returns it)
		Vector3 PointDiffuse;		// +0x48 (getPointDiffuse 0x0011C3C0 returns it)
	};

	struct OutputLightStruct
	{
		void Init(const InputLightStruct &input, const Matrix3D &camera_tm);	// 0x0013F300

		Vector3 Direction;		// +0x00
		Vector3 Diffuse;		// +0x0C
	};

	LightEnvironmentClass(void);								// 0x0013F410
	// Out of line and empty: ~MeshClass 0x0014A5B0 calls the folded body 0x0069E440.
	~LightEnvironmentClass(void);

	void Set_Output_Ambient(Vector3 &color);						// 0x0006E136
	void Add_Light(const LightClass &light);						// 0x00140770
	void Pre_Render_Update(const Matrix3D &camera_tm);					// 0x00140A20
	void Add_Fill_Light(void);								// 0x0013F7E0
	void Calculate_Fill_Light(void);							// 0x00140010

	static void Set_Lighting_LOD_Cutoff(float inten);					// 0x0013F7B0
	static float Get_Lighting_LOD_Cutoff(void);						// 0x0013F7D0
	// Per-light getters (0x0011C320..0x0011C400, stride 0x54 from +0x14).
	const Vector3 &Get_Light_Direction(int index) const;					// 0x0011C320
	const Vector3 &Get_Light_Diffuse(int index) const;					// 0x0011C340
	bool isPointLight(int index) const;							// 0x0011C360
	float getPointIrad(int index) const;							// 0x0011C380
	float getPointOrad(int index) const;							// 0x0011C3A0
	const Vector3 &getPointDiffuse(int index) const;					// 0x0011C3C0
	const Vector3 &getPointAmbient(int index) const;					// 0x0011C3E0
	const Vector3 &getPointCenter(int index) const;						// 0x0011C400
	bool Get_Byte00(void) const { return Byte00; }					// MeshClass::Set_Lighting_Environment 0x0014BB20 tests it

protected:
	bool Byte00;					// +0x000 role not seen yet
	int LightCount;					// +0x004
	Vector3 ObjectCenter;				// +0x008
	InputLightStruct InputLights[MAX_LIGHTS];	// +0x014
	Vector3 OutputAmbient;				// +0x164
	OutputLightStruct OutputLights[MAX_LIGHTS];	// +0x170
	float Tail1D0[9];				// +0x1D0 BFME addition, zeroed by the constructor
	bool Byte1F4;					// +0x1F4
	bool Byte1F5;					// +0x1F5
	float Tail1F8[12];				// +0x1F8 BFME addition, zeroed by the constructor

	static int _LightingLODCutoff;			// 0x009F3408
	static int _LightingLODCutoff2;			// 0x009F340C
};

typedef char InputLightStructSize[(sizeof(LightEnvironmentClass::InputLightStruct) == 0x54) ? 1 : -1];
typedef char OutputLightStructSize[(sizeof(LightEnvironmentClass::OutputLightStruct) == 0x18) ? 1 : -1];
typedef char LightEnvironmentClassSize[(sizeof(LightEnvironmentClass) == 0x228) ? 1 : -1];

#endif
