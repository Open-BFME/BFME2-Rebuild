// The weather subsystem owns the five parsed weather records beginning at +0x30.
// Constructor 0x00318588 installs SubsystemInterface at +0 and Snapshot at +0x0C.
// Its element constructor 0x00318329 clears the reference at +0 and the flag at +4.
// The destructor at 0x0010F149 releases the AudioEventInfoRef's counted pointee.
// SIZE 0x5C (the factory allocates it): the constructor sets +0x10 to 0, +0x14 to 1, +0x18 to 0, builds the
// filter at +0x1C and the name at +0x20 (the empty text), clears +0x28 and +0x2C, builds the five records and
// the flag word at +0x58; +0x24 is not touched there. reset 0x003186A5 and rva00318719 clear them again.
#pragma once
#ifndef BFME2_GLOBALWEATHERSYSTEM_H
#define BFME2_GLOBALWEATHERSYSTEM_H

#include "Common/AudioEventRef.h"
#include "Common/AsciiString.h"
#include "Common/BitFlags.h"
#include "Common/INI/FieldParse.h"
#include "Common/Snapshot.h"
#include "Common/SubsystemInterface.h"
#include "GameLogic/ObjectFilter.h"

class Object;

struct WeatherData
{
	AudioEventInfoRef m_weatherSound;								// 0x00
	Bool m_hasLightning;								// 0x04

	static const FieldParse m_fieldParseTable[];
};

typedef char WeatherDataSize[(sizeof(WeatherData) == 8) ? 1 : -1];

class GlobalWeatherSystem : public SubsystemInterface, public Snapshot
{
public:
	GlobalWeatherSystem();
	virtual ~GlobalWeatherSystem();

	virtual void init() {}
	virtual void reset();
	virtual void update();

	static void parseWeatherData(INI *ini);

	void rva003184B8();								// 0x003184B8
	void rva003186EE(Int, Int, Int);					// 0x003186EE
	void rva00318719();								// 0x00318719: clears every object's weather effect and the state below
	void rva00318333(Object *, Bool);					// 0x00318333

protected:
	virtual void loadPostProcess();
	virtual const char *GetSnapshotName() const;
	virtual void xfer(Xfer *xfer);

private:
	Int m_unk0x10;									// +0x10
	Int m_unk0x14;									// +0x14: the playing weather sound's handle (reset passes it to removeAudioEvent, then stores 1)
	Int m_unk0x18;									// +0x18
	ObjectFilter m_unk0x1C;							// +0x1C
	AsciiString m_unk0x20;							// +0x20
	Int m_unk0x24;									// +0x24
	Int m_unk0x28;									// +0x28: frames until rva003186EE(0, 0, 0), counted down by update
	Int m_unk0x2C;									// +0x2C: frames until rva00318719, counted down by update
	WeatherData m_weather[5];						// 0x30
	BitFlags<15> m_unk0x58;							// +0x58: a 15-bit mask (its xfer helper 0x00318871 is BitFlags<15>'s)
};
typedef char GlobalWeatherSystemSize[(sizeof(GlobalWeatherSystem) == 0x5C) ? 1 : -1];

extern GlobalWeatherSystem *TheGlobalWeatherSystem;

#endif /* BFME2_GLOBALWEATHERSYSTEM_H */
