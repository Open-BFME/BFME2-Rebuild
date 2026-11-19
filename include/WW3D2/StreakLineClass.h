// Portions derived from Command & Conquer Generals and Command & Conquer Generals Zero Hour, (c) Electronic Arts Inc., GPL-3.0-or-later; see LICENSE and NOTICE.
// StreakLineClass.h: WW3D2's streak-line render object (Zero Hour's streak.h: "segmentedlineclass++" with a per-point
// width and colour). Target facts (retail game.dat): constructor 0x00742470 stores the RenderObjClass base, MaxSubdivisionLevels
// +0xC4 = 0, NormalizedScreenArea +0xCC = 0, the three SimpleDynVecClass members PointLocations +0xD0, PointColors +0xE0 and
// PointWidths +0xF0 (vftable VA 0x00BC6F50 and two others, zeroed), LineRenderer +0x100 (constructor 0x00191140) and
// StreakRenderer +0x150 (constructor 0x00742F90), then Personalities +0xC8 = 0. SIZE 0x19C. Overridden RenderObjClass slots as in
// SegmentedLineClass: 1 dtor, 2 Clone, 3 Class_ID (0x1C), 10 Get_Num_Polys, 12 Render, 60 Cast_Ray, 67/68 bounds, 72 Prepare_LOD,
// 74/75 LOD steps, 76..79, 80 Get_LOD_Level (folded with the +0x120 getter 0x0019B6E0), 81 Get_LOD_Count (folded 0x0015E600).
#pragma once
#ifndef BFME2_STREAKLINECLASS_H
#define BFME2_STREAKLINECLASS_H

#include "WW3D2/RenderObjClass.h"
#include "WW3D2/SegLineRendererClass.h"
#include "WW3D2/StreakRendererClass.h"
#include "WWLib/simplevec.h"
#include "WWMath/Vector2.h"
#include "WWMath/Vector3.h"
#include "WWMath/Vector4.h"

class StreakLineClass : public RenderObjClass
{
public:
	enum { MAX_SEGLINE_SUBDIV_LEVELS = 7 };

	StreakLineClass();											// 0x00742470
	StreakLineClass(const StreakLineClass &src);				// 0x007426E0
	StreakLineClass &operator=(const StreakLineClass &that);	// 0x00742830
	virtual ~StreakLineClass();									// 0x00742590

	virtual RenderObjClass *Clone() const;						// slot 2: 0x00742B70
	virtual int Class_ID() const { return 0x1C; }				// slot 3: 0x00742550 (CLASSID_SEGLINE)
	virtual int Get_Num_Polys() const;							// slot 10: 0x00741CF0
	virtual void Render(RenderInfoClass &rinfo);				// slot 12: 0x00742BD0
	virtual bool Cast_Ray(RayCollisionTestClass &raytest);		// slot 60: 0x00742C50
	virtual void Get_Obj_Space_Bounding_Sphere(SphereClass &sphere) const;	// slot 67 (folded with HLodClass's, 0x0019C520)
	virtual void Get_Obj_Space_Bounding_Box(AABoxClass &box) const;			// slot 68: 0x00741D10
	virtual void Prepare_LOD(CameraClass &camera);				// slot 72: 0x00741A80
	virtual void Increment_LOD();								// slot 74: 0x00741AC0
	virtual void Decrement_LOD();								// slot 75: 0x00741AF0
	virtual float Get_Cost() const;								// slot 76 (folded with SegmentedLineClass's, 0x0015E500)
	virtual float Get_Value() const;							// slot 77: 0x00741B10
	virtual float Get_Post_Increment_Value() const;				// slot 78: 0x00741B70
	virtual void Set_LOD_Level(int lod);						// slot 79: 0x00741BC0
	virtual int Get_LOD_Level() const { return (int)LineRenderer.Get_Current_Subdivision_Level(); }	// slot 80 (0x0019B6E0)
	virtual int Get_LOD_Count() const { return (int)MaxSubdivisionLevels; }	// slot 81 (0x0015E600)

	void Set_LocsWidthsColors(unsigned int num_points, Vector3 *locs, float *widths = 0, Vector4 *colors = 0, unsigned int *personalities = 0);	// 0x00742A90

	int Get_Num_Points();										// 0x00741C20
	void Set_Point_Location(unsigned int point_idx, const Vector3 &location);	// 0x00741C30
	void Get_Point_Location(unsigned int point_idx, Vector3 &loc);				// 0x00741C70
	void Add_Point(const Vector3 &location);					// 0x00742AE0
	void Delete_Point(unsigned int point_idx);					// 0x00742B00

	Vector2 Get_UV_Offset_Rate();								// 0x00741810
	int Is_Merge_Intersections();								// 0x00741840
	int Is_Freeze_Random();
	int Is_Sorting_Disabled();									// 0x00741860
	int Are_End_Caps_Enabled();									// 0x00741870

	void Set_Texture(TextureClass *texture);					// 0x00741880
	void Set_Shader(ShaderClass shader);						// 0x007418B0
	void Set_Width(float width);								// 0x007418D0
	void Set_Color(const Vector3 &color);						// 0x00741910
	void Set_Opacity(float opacity);							// 0x00741950
	void Set_Noise_Amplitude(float amplitude);					// 0x00741970
	SegLineRendererClass::TextureMapMode Get_Texture_Mapping_Mode();
	float Get_Texture_Tile_Factor();
	void Set_Texture_Mapping_Mode(SegLineRendererClass::TextureMapMode mode);	// 0x007419A0
	void Set_Texture_Tile_Factor(float factor);					// 0x007419D0
	void Set_UV_Offset_Rate(const Vector2 &rate);				// 0x00741CC0
	void Set_Merge_Intersections(int onoff);					// 0x00741A00
	void Set_Freeze_Random(int onoff);							// 0x00741A20
	void Set_Disable_Sorting(int onoff);						// 0x00741A40
	void Set_End_Caps(int onoff);								// 0x00741A60

protected:
	void Set_Locs(unsigned int num_points, Vector3 *locs);		// 0x00742910
	void Set_Widths(unsigned int num_points, float *widths);	// 0x00742990
	void Set_Colors(unsigned int num_points, Vector4 *colors);	// 0x00742A30
	void Render_Seg_Line(RenderInfoClass &rinfo);				// 0x00742390
	void Render_Streak_Line(RenderInfoClass &rinfo);			// 0x007423E0

private:
	unsigned int MaxSubdivisionLevels;							// +0xC4
	unsigned int *Personalities;								// +0xC8
	float NormalizedScreenArea;									// +0xCC
	SimpleDynVecClass<Vector3> PointLocations;					// +0xD0
	SimpleDynVecClass<Vector4> PointColors;						// +0xE0
	SimpleDynVecClass<float> PointWidths;						// +0xF0
	SegLineRendererClass LineRenderer;							// +0x100
	StreakRendererClass StreakRenderer;							// +0x150
};
// SIZE 0x19C
typedef char StreakLineClassSize[(sizeof(StreakLineClass) == 0x19C) ? 1 : -1];

#endif
