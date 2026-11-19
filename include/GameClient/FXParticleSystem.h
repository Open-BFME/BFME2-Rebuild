// FXParticleSystem.h: BFME 2's own particle system (namespace FXParticleSystem).
//
// Scope: a namespace, not a class. Retail's export table spells every member
// `...@FXParticleSystem@@` with the namespace as the outermost scope and no class
// key, e.g. `?getFirst@?$CategoryModuleClass@$00@FXParticleSystem@@SAPBV12@XZ`
// = public: static CategoryModuleClass<1> const *FXParticleSystem::CategoryModuleClass<1>::getFirst(void).
//
// This header is being recovered family by family (agent fx1). Evidence is
// retail game.dat 1.06 only: the export table (exact decorated names), vtables,
// constructor stores and the bytes. The BFME 1 rebuild's
// fx_particle_system_category.h was read as a lead for the shape; where it
// differs from BFME 2's exports (member names, destructor virtuality, base
// access), the exports win.
#pragma once
#ifndef BFME2_FXPARTICLESYSTEM_H
#define BFME2_FXPARTICLESYSTEM_H

class INI;

namespace FXParticleSystem {

template <int Category> class CategoryModuleClass;
template <int Category> class CategoryModuleTemplate;

// CategoryModuleClassBase<Category, HasDefault>
// Retail: the HasDefault = 1 form (categories 0..7) keeps the category's default
// module class. Its constructor is protected (`IAE`) and stores the module into
// s_default when asked (0x001F4363 for <1,1>: `cmp [esp+8],0; je; mov s_default,
// [esp+4]; ret 8`); getDefault is a static returning *s_default (0x001F435D).
// s_default is `private: static CategoryModuleClass<N> const *` (`0PBV...@B`).
template <int Category, int HasDefault = 1>
class CategoryModuleClassBase {
public:
	static const CategoryModuleClass<Category> &getDefault() { return *s_default; }

protected:
	CategoryModuleClassBase(const CategoryModuleClass<Category> &module, bool isDefault)
	{
		if (isDefault) {
			s_default = &module;
		}
	}

private:
	static const CategoryModuleClass<Category> *s_default;
};

template <int Category, int HasDefault>
const CategoryModuleClass<Category> *CategoryModuleClassBase<Category, HasDefault>::s_default;

// The HasDefault = 0 form (category 8 only): no default, and its constructor is
// public (`??0?$CategoryModuleClassBase@$07$0A@@FXParticleSystem@@QAE@...` =
// `mov eax,ecx; ret 8` at 0x001F446C).
template <int Category>
class CategoryModuleClassBase<Category, 0> {
public:
	CategoryModuleClassBase(const CategoryModuleClass<Category> &, bool) {}
};

// CategoryModuleClass<Category>: one registered module class of a category, kept
// in a singly linked list per category.
// Layout from the constructor 0x003A86DE (<1>): vptr +0, m_key +4, m_name +8,
// m_next +0xC; the constructor runs the base constructor inline (the s_default
// store), then links itself at the head of s_first.
// Vtable 0x0086FFFC (shared by all nine categories): two slots, both __purecall.
// The concrete classes fill them with createTemplate(INI *) const (slot 0) and
// createTemplate() const (slot 1) (ConcreteModuleClass<Box...> vtable 0x0081BEE8);
// MSVC 7.1 lays out same-name overloads in reverse declaration order.
// The destructor is protected and not virtual (`??1...@IAE@XZ`, 0x0057C03B: it
// only resets the vptr).
// Category 8 alone derives from the HasDefault = 0 form (retail exports
// CategoryModuleClassBase<8,0> and CategoryModuleClassBase<0..7,1>, nothing else).
template <int Category>
class CategoryModuleClass : public CategoryModuleClassBase<Category, Category != 8> {
public:
	const char *getKey() const { return m_key; }
	const char *getName() const { return m_name; }
	const CategoryModuleClass *getNext() const { return m_next; }

	static const CategoryModuleClass *getFirst() { return s_first; }

	virtual CategoryModuleTemplate<Category> *createTemplate() const = 0;
	virtual CategoryModuleTemplate<Category> *createTemplate(INI *ini) const = 0;

protected:
	CategoryModuleClass(bool isDefault, const char *key, const char *name)
		: CategoryModuleClassBase<Category, Category != 8>(*this, isDefault), m_key(key), m_name(name), m_next(s_first)
	{
		s_first = this;
	}
	~CategoryModuleClass() {}

private:
	const char *m_key;
	const char *m_name;
	const CategoryModuleClass *m_next;

	static const CategoryModuleClass *s_first;
};

template <int Category>
const CategoryModuleClass<Category> *CategoryModuleClass<Category>::s_first;

} // namespace FXParticleSystem

#endif // BFME2_FXPARTICLESYSTEM_H
