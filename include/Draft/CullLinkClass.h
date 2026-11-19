// CullLinkClass: the per-object link a CullableClass holds into its culling
// system. CullableClass::Set_Culling_System 0x00724520 writes the system
// pointer straight to [link+4] and Get_Culling_System 0x00724540 reads it back,
// so the link is a vptr plus CullingSystem at +0x04.
#pragma once
#ifndef BFME2_CULLLINKCLASS_H
#define BFME2_CULLLINKCLASS_H

class CullSystemClass;

class CullLinkClass
{
public:
	// Inlined into GridCullSystemClass::Add_Object_Internal 0x0071CB90 (`mov [eax+4], esi`).
	CullLinkClass(CullSystemClass *system) : CullingSystem(system) {}
	virtual ~CullLinkClass(void);		// +0x00 the table the link carries

	void Set_Culling_System(CullSystemClass *sys) { CullingSystem = sys; }
	CullSystemClass *Get_Culling_System(void) { return CullingSystem; }

	CullSystemClass *CullingSystem;		// +0x04
};

#endif
