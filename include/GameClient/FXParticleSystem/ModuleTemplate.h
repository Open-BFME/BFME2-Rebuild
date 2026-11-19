// FXParticleSystem/ModuleTemplate.h: FXParticleSystem::ModuleTemplate, the root of the
// module templates (the parsed INI description of one module of a particle system).
//
// Retail facts (game.dat 1.06; agent fx1):
// - The class is exported whole (class-level __declspec(dllexport)): its table's slot 0
//   is the vector deleting destructor `??_EModuleTemplate@FXParticleSystem@@UAEPAXI@Z`
//   0x000011EC, which MSVC emits instead of the scalar one only for exported classes,
//   and the implicit default/copy constructors and operator= are exported
//   (0x000011B4, 0x000011C4, 0x001FD28E).
// - Vptr only: the constructor 0x000011B4 stores 0x007BB52C and returns; the table has
//   one slot, the destructor. The destructor 0x000011BD is public virtual (`UAE`).
// - `#include <new>`: the vector deleting destructor calls operator delete[]
//   (0x0002FD80); without the declaration MSVC 7.1 calls the scalar one.
#pragma once
#ifndef BFME2_FXPARTICLESYSTEM_MODULETEMPLATE_H
#define BFME2_FXPARTICLESYSTEM_MODULETEMPLATE_H

#include <new>

namespace FXParticleSystem {

class __declspec(dllexport) ModuleTemplate {
public:
	virtual ~ModuleTemplate() {}
};

} // namespace FXParticleSystem

#endif // BFME2_FXPARTICLESYSTEM_MODULETEMPLATE_H
