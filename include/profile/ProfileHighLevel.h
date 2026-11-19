// Portions derived from Command & Conquer Generals and Command & Conquer Generals Zero Hour, (c) Electronic Arts Inc., GPL-3.0-or-later; see LICENSE and NOTICE.
// ProfileHighLevel.h: the high level profiler's public interface (Zero Hour's profile_highlevel.h). Target facts (retail
// game.dat): an Id is one pointer to a ProfileId (0x006C5F70..0x006C64E0 test it for NULL and forward).
#pragma once
#ifndef BFME2_PROFILEHIGHLEVEL_H
#define BFME2_PROFILEHIGHLEVEL_H

class ProfileId;

class ProfileHighLevel
{
	friend class Profile;
	ProfileHighLevel(const ProfileHighLevel &);
	ProfileHighLevel &operator=(const ProfileHighLevel &);

public:
	class Id
	{
		friend ProfileHighLevel;

	public:
		Id() : m_idPtr(0) {}

		void Increment(double add = 1.0);					// 0x006C5F70
		void SetMax(double max);							// 0x006C5F90
		const char *GetName() const;						// 0x006C5FB0
		const char *GetDescr() const;						// 0x006C5FC0
		const char *GetUnit() const;						// 0x006C5FE0
		const char *GetCurrentValue() const;				// 0x006C6470
		const char *GetValue(unsigned int frame) const;		// 0x006C64A0
		const char *GetTotalValue() const;					// 0x006C64E0

	private:
		ProfileId *m_idPtr;
	};

	static Id AddProfile(const char *name, const char *descr, const char *unit, int precision, int exp10 = 0);	// 0x006C6500
	static bool EnumProfile(unsigned int index, Id &id);							// 0x006C63A0
	static bool FindProfile(const char *name, Id &id);								// 0x006C63E0

private:
	ProfileHighLevel() {}
};

#endif
