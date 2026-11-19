// ScriptAction.h: one action of a map script, chained through +0x3C. Target facts: the vptr at +0x00
// (the virtual destructor is slot 0); the action type at +0x04, the parameter count at +0x08 and the
// parameter pointers at +0x0C (ScriptAction's destructor 0x003B43CC, which mirrors Condition's, reads
// them), the next action at +0x3C. The constructors store the table at +0x00 (see the old header's notes).
#pragma once
#ifndef BFME2_SCRIPTACTION_H
#define BFME2_SCRIPTACTION_H

#include "Common/GameType.h"
#include "GameLogic/Scripts.h"

class ScriptAction;
class Object;
class Team;
void WriteActionDataChunk(char *label, DataChunkOutput &chunkWriter, ScriptAction *pAction);

class ScriptAction
{
public:
	enum ScriptActionType
	{
		VICTORY = 3,					// ScriptEngine::cheatKeyVictory 0x0020CDA2
		DEFEAT = 4,					// ScriptEngine::cheatKeyDefeat 0x0020CE13
		NO_OP = 5,					// ScriptAction() 0x003B2804 (the numbering is Zero Hour's)
		CAMERA_FADE_ADD = 0x7C,			// ScriptEngine::setFade 0x00203777 (BFME 2 inserted three entries before Zero Hour's 0x79)
		CAMERA_FADE_SUBTRACT = 0x7D,
		CAMERA_FADE_SATURATE = 0x7E,
		CAMERA_FADE_MULTIPLY = 0x7F
	};

	ScriptAction();									// 0x003B2804 (the default type is NO_OP, 5)
	ScriptAction(ScriptActionType type);			// 0x003B5413
	void setActionType(ScriptActionType type);		// 0x003B4343
	void rva003B3D75(void *script, void *name);		// 0x003B3D75 (the map's name; ScriptEngine::executeActions hands every action the script and its name before it runs it)
	// The five predicates rva003B3D75 uses (map placeholders on Rva003B3AE2ActionPrefix, which is ScriptAction here; role names, each a switch over the action type):
	Bool targetsNamedUnit() const;		// 0x003B2858: the action acts on the unit its first (or second) parameter names
	Bool targetsNamedTeam() const;		// 0x003B2AAB: the action acts on the team its first (or second) parameter names
	Bool isSequentialScriptable() const;	// 0x003B2D03: the action can run inside a sequential script
	Object *getNamedUnit() const;		// 0x003B2F2E: getUnitNamed of parameter 0 (84: parameter 1); null for the other types
	Team *getTeam() const;		// 0x003B3AE2: getTeamNamed of parameter 0 (273, 462, 510: parameter 1); null for the other types
	ScriptAction *duplicate() const;				// 0x003B5449
	virtual ~ScriptAction();							// slot 0, 0x003B53F7 (deleting), 0x003B43CC (non-deleting)

	AsciiString getUiText();						// 0x003B54DC
	int getTemplateKindWord();						// 0x003B2832 (the first word of the action's template; Condition::rva003B275A is the condition side)
	AsciiString getTemplateName() const;			// 0x003B3AB9 (the UI name of the action's template; the map's placeholder text@...ActionPrefix)
	int getUiStrings(AsciiString *const strings);	// 0x003B2843 (the template's UI strings for this action type)
	static Bool ParseActionDataChunk(DataChunkInput &file, DataChunkInfo *info, void *userData);		// 0x003B65BF
	static Bool ParseActionFalseDataChunk(DataChunkInput &file, DataChunkInfo *info, void *userData);	// 0x003B65F9
	friend class ScriptEngine;						// ScriptEngine::setFade 0x00203777 switches on the type
	friend class Script;							// Script::deleteAction 0x003B25CD walks the chain
	friend void WriteActionDataChunk(char *label, DataChunkOutput &chunkWriter, ScriptAction *pAction);	// 0x003B3DFF
	Parameter *getParameter(Int ndx) { if (ndx >= 0 && ndx < m_numParms) return m_parms[ndx]; return 0; }	// 0x00203553
	Parameter *getParameter(Int ndx) const { if (ndx >= 0 && ndx < m_numParms) return m_parms[ndx]; return 0; }
	ScriptAction *getNext() const { return m_nextAction; }
	void setNextAction(ScriptAction *next) { m_nextAction = next; }

protected:
	static ScriptAction *ParseAction(DataChunkInput &file, DataChunkInfo *info, void *userData);	// 0x003B5EA5

	Int m_actionType;								// +0x04
	Int m_numParms;									// +0x08
	Parameter *m_parms[12];							// +0x0C
	ScriptAction *m_nextAction;						// +0x3C
	Bool m_hasWarnings;								// +0x40 (constructors clear it)
	unsigned char m_tail;							// +0x41 (constructors set it to 1)
	char m_pad42[2];								// +0x42
	Int m_bfmeTail;									// +0x44 (constructors clear it)
};
// SIZE 0x48

#endif
