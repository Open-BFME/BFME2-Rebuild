// Animatable3DObjClass.h: a composite render object driven by a hierarchy tree (skeleton) and its
// animation state; the base of HLodClass.
//
// Target facts (retail game.dat):
// - The constructor 0x001A4900 runs CompositeRenderObjClass's default constructor, installs vftable
//   0x007D6D10 (134 slots: RenderObjClass's 129 and five more) and the secondary 0x007D6D08 at +8, then
//   clears IsTreeValid +0xF4, HTree +0xF8 and +0xFC, sets CurMotionMode +0x100 to 1 (base pose), stamps
//   the single-animation record with WW3D's sync time (0x009EC3CC), 1.0 for the direction and the rate,
//   and clears the blend record. A null tree name leaves HTree null; an empty one builds a default
//   HTreeClass (0x00166270 then Init_Default 0x001666D0); otherwise the named tree from Get_HTree
//   0x0017FC7C is copied (0x001662A0), with the default tree when it is not found.
// - Release 0x001A4AA0 drops the references the current mode holds (mode 2: +0x104; mode 3: +0x104 and
//   +0x108; the combo of mode 4 is not counted) and then +0xFC. The destructor 0x001A54F0 calls it,
//   deletes HTree and runs ~CompositeRenderObjClass.
// - The animation state at +0x104 is one of three records sharing storage, chosen by CurMotionMode:
//   Set_Animation(HAnimClass *, float, int) 0x001A4BF0 fills the single one (motion, raw frame, the frame
//   clamped to the motion's frame count, mode, sync time, direction -1 for modes 5 and up or for mode 4
//   when the target lies behind, rate 1.0); the five-argument overload 0x001A4D10 the blend one (first,
//   second, first_frame, second_frame, blend); the combo overload 0x001A4D90 stores the combo pointer.
// - +0xFC is a render object whose hierarchy this one follows: slot 59 0x001A5460 keeps it (add-ref'd)
//   only when its slot 58 (Get_HTree) is non-null.
// SIZE 0x120.
#pragma once
#ifndef BFME2_ANIMATABLE3DOBJCLASS_H
#define BFME2_ANIMATABLE3DOBJCLASS_H

#include "WW3D2/CompositeRenderObjClass.h"

class HTreeClass;
class HAnimClass;
class HAnimComboClass;

class Animatable3DObjClass : public CompositeRenderObjClass
{
public:
	// CurMotionMode values.
	enum { MOTION_NONE = 0, MOTION_BASE_POSE, MOTION_SINGLE, MOTION_BLEND, MOTION_COMBO };

	Animatable3DObjClass(const char *htree_name);					// 0x001A4900
	Animatable3DObjClass(const Animatable3DObjClass &src);				// 0x001A5C50
	virtual ~Animatable3DObjClass(void);						// 1: 0x001A54F0
	Animatable3DObjClass &operator=(const Animatable3DObjClass &that);		// 0x001A5570

	virtual void Render(RenderInfoClass &rinfo);					// 12: 0x001A5680
	virtual void Set_Transform(const Matrix3D &m);					// 21: 0x001A4B30
	virtual void Set_Position(const Vector3 &v);					// 22: 0x001A4B50
	virtual void Set_Animation(void);						// 46: 0x001A4BD0
	virtual void Set_Animation(HAnimClass *motion, float frame, int anim_mode = 0);	// 45: 0x001A4BF0
	virtual void Set_Animation(HAnimClass *first, float first_frame, HAnimClass *second, float second_frame, float blend);	// 44: 0x001A4D10
	virtual void Set_Animation(HAnimComboClass *combo);			// 43: 0x001A4D90
	virtual HAnimClass *Peek_Animation(void);					// 47: 0x001A4DC0
	virtual int Get_Num_Bones(void);						// 48: 0x001A4B70
	virtual const char *Get_Bone_Name(int index);				// 49: 0x001A4B90
	virtual int Get_Bone_Index(const char *name);				// 50: 0x001A4BB0
	virtual Matrix3D Get_Bone_Transform(const char *name);			// 52: 0x001A4DE0
	virtual Matrix3D Get_Bone_Transform(int boneindex);				// 51: 0x001A56E0
	virtual bool _bfme_ro_v50(int bone);						// 53: 0x001A4E70 (the bone's visibility flag; RenderObjClass's role name)
	virtual void Capture_Bone(int bindex);						// 54: 0x001A4EC0
	virtual void Release_Bone(int bindex);						// 55: 0x001A4EE0
	virtual bool Is_Bone_Captured(int bindex) const;				// 56: 0x001A4F00
	virtual void Control_Bone(int bindex, const Matrix3D &objtm, bool world_space_translation = false);	// 57: 0x001A4F20
	virtual const HTreeClass *Get_HTree(void) const { return HTree; }		// 58: 0x0019F010
	virtual void _bfme_ro_v56(RenderObjClass *source);				// 59: 0x001A5460 (keeps a render object whose hierarchy this one follows; RenderObjClass's role name)

	// 129..133: the slots this class adds (same-name overloads are laid out in reverse).
	virtual void Set_Animation_Frame_Rate_Multiplier(float multiplier);		// 129: 0x001A5440
	virtual HAnimClass *Peek_Animation_And_Info(float &frame, int &numFrames, int &mode, float &mult);	// 130: 0x001A53E0
	virtual bool Is_Animation_Complete(void) const;					// 131: 0x001A5360
	virtual bool Simple_Evaluate_Bone(int boneindex, Matrix3D *tm) const;		// 133: 0x001A5A60
	virtual bool Simple_Evaluate_Bone(int boneindex, float frame, Matrix3D *tm) const;	// 132: 0x001A4F50

protected:
	virtual void Update_Sub_Object_Transforms(void);				// 42: 0x001A5900

	bool Is_Hierarchy_Valid(void) const;					// 0x001A47E0
	void Set_Hierarchy_Valid(bool onoff) const { IsTreeValid = onoff; }
	void Base_Update(const Matrix3D &root);						// 0x001A47F0
	void Anim_Update(const Matrix3D &root, HAnimClass *motion, float frame);	// 0x001A4820
	void Blend_Update(const Matrix3D &root, HAnimClass *first, float first_frame, HAnimClass *second, float second_frame, float blend);	// 0x001A4890
	void Combo_Update(const Matrix3D &root, HAnimComboClass *anim);			// 0x001A48D0
	void Release(void);								// 0x001A4AA0
	float Compute_Current_Frame(float *newDirection) const;				// 0x001A5020
	void Single_Anim_Progress(void);						// 0x001A5320

	mutable bool IsTreeValid;			// +0xF4
	HTreeClass *HTree;				// +0xF8
	RenderObjClass *HTreeSourceObj;			// +0xFC: kept by slot 59 when it has a hierarchy
	int CurMotionMode;				// +0x100

	union
	{
		struct
		{
			HAnimClass *Motion;		// +0x104
			float Frame;			// +0x108
			float PrevFrame;		// +0x10C
			int AnimMode;			// +0x110
			mutable unsigned int LastSyncTime;	// +0x114
			float AnimDirection;		// +0x118
			float FrameRateMultiplier;	// +0x11C
		} ModeAnim;

		struct
		{
			HAnimClass *Motion0;		// +0x104
			HAnimClass *Motion1;		// +0x108
			float Frame0;			// +0x10C
			float Frame1;			// +0x110
			float Percentage;		// +0x114
		} ModeInterp;

		struct
		{
			HAnimComboClass *AnimCombo;	// +0x104
		} ModeCombo;
	};
};

typedef char Animatable3DObjClassSize[(sizeof(Animatable3DObjClass) == 0x120) ? 1 : -1];

#endif
