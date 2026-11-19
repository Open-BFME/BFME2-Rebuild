// CullSystemClass.h: the culling system's collection list base.
//
// Target facts: the constructor 0x00724550 stores the table then zeroes
// CollectionHead at +4, Add_To_Collection 0x00724590 pushes onto it and
// Get_Next_Collected_Object_Internal 0x00724570 follows CullableClass's
// NextCollected link at +0x24.
#pragma once
#ifndef BFME2_CULLSYSTEMCLASS_H
#define BFME2_CULLSYSTEMCLASS_H

class AABoxClass;
class CullableClass;
class FrustumClass;
class OBBoxClass;
class Vector3;

// Vtable 0x00CEFD60: slot 0 is the deleting destructor, slots 1..5 are pure
// (__purecall). GridCullSystemClass's table 0x00CEFCB0 fills them with its
// overrides in this order: Collect_Objects(Frustum), (OBBox), (AABox),
// (Vector3), then Update_Culling.
class CullSystemClass
{
public:
	CullSystemClass(void);
	virtual ~CullSystemClass(void);
	// Declared in Zero Hour's order (point, AABox, OBBox, frustum): MSVC 7.1 lays same-name overloads out in reverse, which
	// gives retail's slots 1..4 (frustum 0x0071BFB0, OBBox 0x0071BBC0, AABox 0x0071B820, point 0x0071B4A0).
	virtual void Collect_Objects(const Vector3 &point) = 0;
	virtual void Collect_Objects(const AABoxClass &box) = 0;
	virtual void Collect_Objects(const OBBoxClass &box) = 0;
	virtual void Collect_Objects(const FrustumClass &frustum) = 0;
	virtual void Update_Culling(CullableClass *obj) = 0;

protected:
	CullableClass *Get_First_Collected_Object_Internal(void);		// 0x007244B0 (GridCullSystemClass::Load 0x0071CA00 calls it)
	CullableClass *Get_Next_Collected_Object_Internal(CullableClass *obj);
	void Add_To_Collection(CullableClass *obj);
public:
	// Out of line: GridCullSystemClass::Collect_And_Unlink_All 0x0071C380 calls it;
	// its body (`CollectionHead = 0`) is folded with other one-store functions at
	// 0x0065D1C0.
	void Reset_Collection(void);

	CullableClass *CollectionHead;		// +0x04
};

#endif
