// Portions derived from Command & Conquer Generals and Command & Conquer Generals Zero Hour, (c) Electronic Arts Inc., GPL-3.0-or-later; see LICENSE and NOTICE.
// ProfileId.h: one high level profile value (Zero Hour's internal_highlevel.h). Target facts (retail game.dat): the
// constructor 0x006C5C60 stores the name +4, description +8, unit +0xC (each a copy made through ProfileAllocMemory, the
// last two NULL when absent), precision +0x10, exp10 +0x14, current value +0x18, total value +0x20, the recorded frame
// values +0x48, the first frame +0x4C and the value mode +0x50 (Increment 0x006C5DA0 / Maximum 0x006C5E00 touch +0x18, +0x20
// and the four frame slots +0x28..+0x47 selected by frameRecordMask 0x00A0C618); the list head is `first` 0x00A0C620, the
// current frame `curFrame` 0x00A0C61C, the string buffer 0x00A0C218 (1024 bytes) and its fill index 0x00A0C210. SIZE 0x58.
#pragma once
#ifndef BFME2_PROFILEID_H
#define BFME2_PROFILEID_H

class ProfileId
{
	ProfileId(const ProfileId &);
	ProfileId &operator=(const ProfileId &);

public:
	ProfileId(const char *name, const char *descr, const char *unit, int precision, int exp10);	// 0x006C5C60

	static ProfileId *GetFirst() { return first; }
	ProfileId *GetNext() const { return m_next; }
	const char *GetName() const { return m_name; }
	const char *GetUnit() const { return m_unit ? m_unit : ""; }
	const char *GetDescr() const { return m_descr ? m_descr : ""; }

	void Increment(double add);																// 0x006C5DA0
	void Maximum(double max);																// 0x006C5E00

	double GetCurrentValue()
	{
		double help = m_curVal;
		m_curVal = 0.;
		return help;
	}
	double GetTotalValue() const
	{
		return m_totalVal;
	}
	bool GetFrameValue(unsigned int frame, double &value) const
	{
		if (frame < (unsigned int)m_firstFrame ||
			frame >= (unsigned int)curFrame)
			return false;
		value = m_recFrameVal[frame - m_firstFrame];
		return true;
	}

	const char *AsString(double v) const;													// 0x006C60A0

	static void Shutdown();																	// 0x006C6360
	static int FrameStart();																// 0x006C61B0
	static void FrameEnd(int which, int mixIndex);											// 0x006C6220
	static void ClearTotals()
	{
		for (ProfileId *cur = first; cur; cur = cur->m_next)
			cur->m_totalVal = 0.;
	}

private:
	~ProfileId() {}

	enum
	{
		MAX_FRAME_RECORDS = 4,
		STRING_BUFFER_SIZE = 1024
	};
	enum ValueMode
	{
		Unknown,
		ModeIncrement,
		ModeMaximum
	};

	static ProfileId *first;						// 0x00A0C620
	ProfileId *m_next;								// +0x00
	char *m_name;									// +0x04
	char *m_descr;									// +0x08
	char *m_unit;									// +0x0C
	int m_precision;								// +0x10
	int m_exp10;									// +0x14
	double m_curVal;								// +0x18
	double m_totalVal;								// +0x20
	double m_frameVal[MAX_FRAME_RECORDS];			// +0x28
	double *m_recFrameVal;							// +0x48
	int m_firstFrame;								// +0x4C
	ValueMode m_valueMode;							// +0x50

	static int curFrame;							// 0x00A0C61C
	static unsigned int frameRecordMask;			// 0x00A0C618
	static char stringBuf[STRING_BUFFER_SIZE];		// 0x00A0C218
	static unsigned int stringBufUnused;			// 0x00A0C210

	friend class ProfileHighLevel;
};

typedef char ProfileIdSize[(sizeof(ProfileId) == 0x58) ? 1 : -1];

#endif
