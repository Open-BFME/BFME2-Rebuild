// Scripts.h: map-script data. Only Parameter so far.
// Parameter target facts: ctor 0x003B333A (Parameter(ParameterType, Int)) stores
// the type +0x00, clears the initialized byte +0x04, stores the int +0x08, and
// zeroes the real +0x0C, the string +0x10, the coordinate +0x14..+0x1C and the
// two words at +0x20/+0x24 (getStatus 0x00579696 returns them as BitFlags<45>).
// That is Zero Hour's member order. No vtable. getCoord3D 0x003B27BB copies
// +0x14 only when the type is 0x10, Zero Hour's COORD3D.
// Size not measured yet (the allocator is a pool), so there is no size check.
#pragma once
#ifndef BFME2_SCRIPTS_H
#define BFME2_SCRIPTS_H

#include "Common/GameType.h"
#include "Common/AsciiString.h"
#include "Common/Coord3D.h"
#include "Common/BitFlags.h"

class DataChunkOutput;
class DataChunkInput;
struct DataChunkInfo;

enum { MAX_PARMS = 12 };		// Zero Hour's Scripts.h value; ScriptAction 0x003B43CC and Template 0x003B276B loop over twelve

class Parameter
{
public:
	enum ParameterType
	{
		TEAM = 3,				// ScriptEngine::checkConditionsForTeamNames 0x002062A8 (cmp [parameter],3; Zero Hour's numbering)
		SIDE = 11,				// ParseConditionDataChunk 0x003B6D92 (0xB): the side parameter it adds to old SKIRMISH_SPECIAL_POWER_READY conditions
		OBJECT_TYPE = 15,			// ParseConditionDataChunk 0x003B6D92 (0xF)
		COORD3D = 16,				// getCoord3D 0x003B27BB
		KIND_OF_PARAM = 27,			// WriteParameter 0x003B3A38 (0x1B) turns the kind-of bit index into its name
		SURFACES_ALLOWED = 37,			// ParseConditionDataChunk 0x003B6D92 (0x25): the parameter added to old chunks (Zero Hour's numbering)
		OBJECT_STATUS = 41,			// ReadParameter 0x003B5CA1 (0x29) turns the status name into a bit index
		OBJECT_TYPE_LIST = 61			// 0x3D: what ParseConditionDataChunk turns an OBJECT_TYPE parameter into when the template's slot has this type (Zero Hour's name for the special case; BFME 2's number)
	};

	enum				// the comparison values a COMPARISON parameter stores in its Int (Zero Hour's numbering; ScriptEngine::evaluateCounter 0x00208F61's six cases)
	{
		LESS_THAN = 0,
		LESS_EQUAL,
		EQUAL,
		GREATER_EQUAL,
		GREATER,
		NOT_EQUAL
	};

	Parameter(ParameterType type, Int val = 0) throw();	// 0x003B333A (COMDAT; no exception state around it at its callers 0x003B4343/0x003B4A94)

	ParameterType getParameterType() const { return m_paramType; }
	Int getInt() const { return m_int; }
	Real getReal() const { return m_real; }
	const AsciiString &getString() const { return m_string; }
	void getCoord3D(Coord3D *coord) const;		// 0x003B27BB
	void convertToObjectTypeList();			// 0x003B27F8 (OBJECT_TYPE becomes OBJECT_TYPE_LIST; map: checkAndSet@Rva0034FF50)
	void friend_setString(AsciiString string);	// 0x00333B02 (Zero Hour's setter: m_string = string)
	AsciiString getUiText() const;				// 0x003B4B1D
	void WriteParameter(DataChunkOutput &chunkWriter);	// 0x003B3A38
	static Parameter *ReadParameter(DataChunkInput &file);	// 0x003B5CA1

protected:
	void setCoord3D(const Coord3D *coord);		// 0x003B27E2 (protected, 0x003B27E2 copies only when the type is COORD3D)

public:

	ParameterType m_paramType;		// +0x00
	Bool m_initialized;			// +0x04
	Int m_int;				// +0x08
	Real m_real;				// +0x0C
	AsciiString m_string;			// +0x10
	Coord3D m_coord;			// +0x14
	BitFlags<45> m_objectStatus;		// +0x20 (getStatus 0x00579696; the implicit operator= 0x003B270F copies it as two words)
};

class ObjectTypes;
// Fills outObjectTypes from a script parameter: a single type name, or the named
// list ScriptEngine::getObjectTypes finds. cdecl, 0x00566E6C.
void Script_objectTypesFromParam(Parameter *pTypeParm, ObjectTypes *outObjectTypes);

#endif
