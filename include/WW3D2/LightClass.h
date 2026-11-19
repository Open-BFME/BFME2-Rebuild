// LightClass.h: WW3D's light render object (point, directional or spot).
//
// Target facts (light.cpp, O00130AC0):
// - vftable 0x007D23C0, 130 slots: RenderObjClass's 129 with 1 (deleting
//   destructor 0x00130FB0), 2 Clone 0x00130EA0, 3 Class_ID 0x0006DD63 (returns
//   14), 12 Render (the empty `ret 4` 0x0047A69C), 26/27 Notify_Added/Removed
//   0x00130F00/0x00130F20, 67/68 the object-space bounds 0x00130F40/0x00130F70
//   replaced, then one of its own, 129 (0x0050B5C6, returns true).
// - The constructor 0x00130AC0(LightType) runs RenderObjClass's 0x0013BF00,
//   stores the type at +0xC4, clears +0xC8 and the byte +0xCC, sets +0xD0 and
//   the three colours +0xD4..+0xF4 to 1, +0xF8/+0xFC to 0, +0x100 to 50,
//   +0x104 to 100, +0x108 to pi/4, +0x10C to 0.707, +0x110 to 1 and the
//   direction +0x114..+0x11C to (0, 0, 1); a DIRECTIONAL (1) light is made
//   force-visible (RenderObjClass::Set_Force_Visible 0x0013B720).
// - Notify_Added registers the light with the scene as SceneClass::LIGHT after
//   RenderObjClass's own notification; Notify_Removed unregisters it first.
// - The object-space bounds are a sphere / cube of radius +0x104 (the far
//   attenuation end) around the origin. Clone allocates 0x120 bytes. SIZE 0x120.
// The name of slot 129 is a role name.
#pragma once
#ifndef BFME2_LIGHTCLASS_H
#define BFME2_LIGHTCLASS_H

#include "WW3D2/RenderObjClass.h"
#include "WWMath/Vector3.h"

class LightClass : public RenderObjClass
{
public:
	enum LightType
	{
		POINT = 0,
		DIRECTIONAL,
		SPOT
	};

	LightClass(LightType type = POINT);								// 0x00130AC0
	LightClass(const LightClass &src);								// 0x00130C00
	LightClass &operator=(const LightClass &src);							// 0x00130D40
	virtual ~LightClass(void);									// 1: 0x00130E80

	virtual RenderObjClass *Clone(void) const;							// 2: 0x00130EA0
	virtual int Class_ID(void) const { return 14; }							// 3: 0x0006DD63
	virtual void Render(RenderInfoClass &rinfo) {}							// 12: 0x0047A69C
	virtual void Notify_Added(SceneClass *scene);							// 26: 0x00130F00
	virtual void Notify_Removed(SceneClass *scene);							// 27: 0x00130F20
	virtual void Get_Obj_Space_Bounding_Sphere(SphereClass &sphere) const;				// 67: 0x00130F40
	virtual void Get_Obj_Space_Bounding_Box(AABoxClass &box) const;					// 68: 0x00130F70
	virtual bool Is_Light_Source(void) { return true; }						// 129: 0x0050B5C6 (role name)

	LightType Get_Type(void) const { return Type; }
	void Set_Ambient(const Vector3 &color);							// 0x00042EBD
	void Set_Diffuse(const Vector3 &color);							// 0x00042EDA
	void Set_Specular(const Vector3 &color);						// 0x00042EF7
	void Set_Far_Attenuation_Range(double start, double end);				// 0x00042F14
	float Get_Intensity(void) const { return Intensity; }
	void Get_Far_Attenuation_Range(float &start, float &end) const { start = FarAttenStart; end = FarAttenEnd; }
	void Get_Far_Attenuation_Range(double &start, double &end) const { start = FarAttenStart; end = FarAttenEnd; }
	// Zero Hour's inline accessors (DX8Wrapper::Set_Light 0x0011DD80 reads them).
	void Get_Ambient(Vector3 *set_ambient) const { *set_ambient = Ambient; }
	void Get_Diffuse(Vector3 *set_diffuse) const { *set_diffuse = Diffuse; }
	void Get_Specular(Vector3 *set_specular) const { *set_specular = Specular; }
	void Get_Spot_Direction(Vector3 &dir) const { dir = SpotDirection; }
	float Get_Attenuation_Range(void) const { return FarAttenEnd; }
	float Get_Spot_Exponent(void) const { return SpotExponent; }
	float Get_Spot_Angle(void) const { return SpotAngle; }

protected:
	LightType Type;			// +0x0C4
	unsigned int Flags;		// +0x0C8
	bool CastShadows;		// +0x0CC
	float Intensity;		// +0x0D0
	Vector3 Ambient;		// +0x0D4
	Vector3 Diffuse;		// +0x0E0
	Vector3 Specular;		// +0x0EC
	float NearAttenStart;		// +0x0F8
	float NearAttenEnd;		// +0x0FC
	float FarAttenStart;		// +0x100
	float FarAttenEnd;		// +0x104
	float SpotAngle;		// +0x108
	float SpotAngleCos;		// +0x10C
	float SpotExponent;		// +0x110
	Vector3 SpotDirection;		// +0x114
};

typedef char LightClassSize[(sizeof(LightClass) == 0x120) ? 1 : -1];

#endif
