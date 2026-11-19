// SegmentedLineClass.h: WW3D2's polyline render object (segline.h).
// Target facts: allocation 0x12C (Clone 0x0015F200). Constructor 0x0015EF20:
// RenderObjClass base, MaxSubdivisionLevels +0xC4 = 0, NormalizedScreenArea
// +0xC8 = 0, PointLocations (SimpleDynVecClass<Vector3>, vftable VA 0x00BC6F50)
// +0xCC, LineRenderer +0xDC (its constructor 0x00191140). Overridden
// RenderObjClass slots: 1, 2 Clone, 3 Class_ID (shared body), 10 Get_Num_Polys,
// 12 Render, 60 Cast_Ray, 67/68 bounds, 72 Prepare_LOD, 74/75 LOD steps,
// 76 Get_Cost, 77/78 LOD values, 79 Set_LOD_Level, 80 Get_LOD_Level,
// 81 Get_LOD_Count. The accessors below forward to LineRenderer and are out of
// line in segline.cpp.
#pragma once
#ifndef BFME2_SEGMENTEDLINECLASS_H
#define BFME2_SEGMENTEDLINECLASS_H

#include "WW3D2/RenderObjClass.h"
#include "WW3D2/SegLineRendererClass.h"
#include "WWLib/simplevec.h"
#include "WWMath/Vector2.h"
#include "WWMath/Vector3.h"

class SegmentedLineClass : public RenderObjClass
{
public:
	enum { MAX_SEGLINE_SUBDIV_LEVELS = 7 };

	SegmentedLineClass();										// 0x0015EF20
	SegmentedLineClass(const SegmentedLineClass &src);			// 0x0015EFD0
	SegmentedLineClass &operator=(const SegmentedLineClass &that);	// 0x0015F090
	virtual ~SegmentedLineClass();								// 0x0015E690

	virtual RenderObjClass *Clone() const;						// slot 2: 0x0015F200
	virtual int Class_ID() const;								// slot 3
	virtual int Get_Num_Polys() const;							// slot 10: 0x0015E830
	virtual void Render(RenderInfoClass &rinfo);				// slot 12: 0x0015F260
	virtual bool Cast_Ray(RayCollisionTestClass &raytest);		// slot 60: 0x0015F2C0
	virtual void Get_Obj_Space_Bounding_Sphere(SphereClass &sphere) const;	// slot 67
	virtual void Get_Obj_Space_Bounding_Box(AABoxClass &box) const;			// slot 68: 0x0015E850
	virtual void Prepare_LOD(CameraClass &camera);				// slot 72: 0x0015E470
	virtual void Increment_LOD();								// slot 74: 0x0015E4B0
	virtual void Decrement_LOD();								// slot 75: 0x0015E4E0
	virtual float Get_Cost() const;								// slot 76: 0x0015E500
	virtual float Get_Value() const;							// slot 77: 0x0015E510
	virtual float Get_Post_Increment_Value() const;				// slot 78: 0x0015E570
	virtual void Set_LOD_Level(int lod);						// slot 79: 0x0015E5C0
	virtual int Get_LOD_Level() const;							// slot 80: 0x0015E5F0
	virtual int Get_LOD_Count() const;							// slot 81

	void Set_Points(unsigned int num_points, Vector3 *locs);	// 0x0015F100
	int Get_Num_Points();										// 0x0015E760
	void Set_Point_Location(unsigned int point_idx, const Vector3 &location);	// 0x0015E770
	void Get_Point_Location(unsigned int point_idx, Vector3 &loc);				// 0x0015E7B0
	void Add_Point(const Vector3 &location);					// 0x0015F170
	void Delete_Point(unsigned int point_idx);					// 0x0015F190

	TextureClass *Get_Texture();
	ShaderClass Get_Shader();									// 0x0015E150
	float Get_Width();											// 0x0015E270
	void Get_Color(Vector3 &color);								// 0x0015E160
	float Get_Opacity();										// 0x0015E190
	float Get_Noise_Amplitude();
	float Get_Merge_Abort_Factor();
	unsigned int Get_Subdivision_Levels();
	SegLineRendererClass::TextureMapMode Get_Texture_Mapping_Mode();	// 0x0015E1C0
	float Get_Texture_Tile_Factor();							// 0x0015E1D0
	Vector2 Get_UV_Offset_Rate();								// 0x0015E1E0
	int Is_Merge_Intersections();								// 0x0015E210
	int Is_Freeze_Random();										// 0x0015E220
	int Is_Sorting_Disabled();									// 0x0015E230
	int Are_End_Caps_Enabled();									// 0x0015E240

	void Set_Texture(TextureClass *texture);					// 0x0015E250
	void Set_Shader(ShaderClass shader);						// 0x0015E260
	void Set_Width(float width);								// 0x0015E290
	void Set_Width(float width, float end_width);				// 0x0015E2B0 (BFME 2 adds the end width the renderer keeps at +0x1C; both clamp at 0)
	void Set_Color(const Vector3 &color);						// 0x0015E2F0
	void Set_Opacity(float opacity);							// 0x0015E320
	void Set_Noise_Amplitude(float amplitude);					// 0x0015E340
	void Set_Merge_Abort_Factor(float factor);					// 0x0015E370
	void Set_Subdivision_Levels(unsigned int levels);			// 0x0015E390
	void Set_Texture_Mapping_Mode(SegLineRendererClass::TextureMapMode mode);	// 0x0015E3B0
	void Set_Texture_Tile_Factor(float factor);					// 0x0015E3D0
	void Set_UV_Offset_Rate(const Vector2 &rate);				// 0x0015E800
	void Set_Merge_Intersections(int onoff);					// 0x0015E3F0
	void Set_Freeze_Random(int onoff);							// 0x0015E410
	void Set_Disable_Sorting(int onoff);						// 0x0015E430
	void Set_End_Caps(int onoff);								// 0x0015E450

protected:
	void Render_Seg_Line(RenderInfoClass &rinfo);				// 0x0015EED0

	unsigned int MaxSubdivisionLevels;							// +0xC4
	float NormalizedScreenArea;									// +0xC8
	SimpleDynVecClass<Vector3> PointLocations;					// +0xCC
	SegLineRendererClass LineRenderer;							// +0xDC
};
// SIZE 0x12C
typedef char SegmentedLineClassSize[(sizeof(SegmentedLineClass) == 0x12C) ? 1 : -1];

#endif
