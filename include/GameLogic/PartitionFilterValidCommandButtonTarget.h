#pragma once
#include "GameLogic/PartitionFilter.h"
#include "Common/GameType.h"
class CommandButton;
// Zero Hour's PartitionFilterValidCommandButtonTarget: accepts the objects a command button can be used on from a
// source object. Target facts: vtable 0x0081FE34, allow 0x002614BC asks the button's 0x0035B2C3 (source, candidate,
// no position, command source) and compares the answer with the match byte at +0x10; the constructor is what the
// NAMED_/TEAM_USE_COMMANDBUTTON_ON_NEAREST_* rows of ScriptActions store inline (object, button, match = 1,
// command source = CMD_FROM_SCRIPT), which is Zero Hour's constructor of that name (and its member names).
class PartitionFilterValidCommandButtonTarget : public PartitionFilter
{
public:
 PartitionFilterValidCommandButtonTarget(const Object *source, const CommandButton *commandButton, bool match, UnsignedInt commandSource)
  : m_source(source), m_commandButton(commandButton), m_match(match), m_commandSource(commandSource) {}
 virtual bool allow(Object *);	// 0x002614BC
 const Object *m_source;	// 0x08
 const CommandButton *m_commandButton;	// 0x0C
 bool m_match;	// 0x10
 UnsignedInt m_commandSource;	// 0x14
};
// SIZE 0x18
