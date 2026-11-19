// RelatedObjectQuery.h: what Object::rva0029439D hands back, when it hands back
// anything. Slot 19 (offset 0x4C) names the object an emotion's FX is aimed at.
// Target facts: OnActivation 0x004DCC84, OnDeactivation 0x004DD2C0 and the
// per-frame step 0x004DCEC4 all call `mov edx,[eax]; mov ecx,eax; call
// dword ptr [edx+0x4C]` on the result, and use the returned Object * in place of
// the emotion's own object when it is non-null.
#pragma once
#ifndef BFME2_RELATEDOBJECTQUERY_H
#define BFME2_RELATEDOBJECTQUERY_H

class Object;

class RelatedObjectQuery
{
public:
	virtual void vslot00();
	virtual void vslot01();
	virtual void vslot02();
	virtual void vslot03();
	virtual void vslot04();
	virtual void vslot05();
	virtual void vslot06();
	virtual void vslot07();
	virtual void vslot08();
	virtual void vslot09();
	virtual void vslot10();
	virtual void vslot11();
	virtual void vslot12();
	virtual void vslot13();
	virtual void vslot14();
	virtual void vslot15();
	virtual void vslot16();
	virtual void vslot17();
	virtual void vslot18();
	virtual Object *v19();
};

#endif
