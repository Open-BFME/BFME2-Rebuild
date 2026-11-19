#pragma once
// SIZE 0x64: AptObject (0x20), the local clock at +0x20, the UTC clock at +0x40
// and the timezone offset in hours at +0x60. Constructor 0x006FBF90 takes seven
// ints (-1 keeps the value read from the system clock), builds both clocks through
// the platform callback (VA 0x00E177B8) and derives the offset from the day/hour
// difference between them (+0x60). Type id 0x1D; vtable 0x008ED8BC (15 slots):
// AptObject's table with objectMemberLookup (slot 7) = 0x006F5110 and the deleting
// destructor 0x006FC0F0 last.
// The member names (local, utc, timezoneHours) are the donor's; offsets and
// meaning are the retail natives': getHours reads +0x28, getUTCHours +0x48,
// getTimezoneOffset scales +0x60, setX writes the local clock then calls
// setDates(local, utc, offset), setUTCX writes the UTC clock then calls
// setDates(utc, local, -offset).
#include "Apt/AptObject.h"
#include "Apt/AptSysClock.h"

class EAStringC;

class AptDate : public AptObject
{
public:
    AptDate(int year, int month, int date, int hour, int minute, int second, int millisecond);
    virtual AptValue *objectMemberLookup(AptValue *const self, const EAStringC *const name) const;
    int dateGetNumDaysInMonth(int month, int year);
    bool dateIsLeapYear(int year); // 0x006F4080
    void setDates(AptSysClock *source, AptSysClock *destination, int offsetHours);
    int getDayOfWeek(int year, int month, int date);
    void toString(EAStringC &out);

    static AptValue *sMethod_getDate(AptValue *self, int argc);
    static AptValue *sMethod_getDay(AptValue *self, int argc);
    static AptValue *sMethod_getFullYear(AptValue *self, int argc);
    static AptValue *sMethod_getHours(AptValue *self, int argc);
    static AptValue *sMethod_getMilliseconds(AptValue *self, int argc);
    static AptValue *sMethod_getMinutes(AptValue *self, int argc);
    static AptValue *sMethod_getMonth(AptValue *self, int argc);
    static AptValue *sMethod_getSeconds(AptValue *self, int argc);
    static AptValue *sMethod_getTime(AptValue *self, int argc);
    static AptValue *sMethod_getTimezoneOffset(AptValue *self, int argc);
    static AptValue *sMethod_getUTCDate(AptValue *self, int argc);
    static AptValue *sMethod_getUTCDay(AptValue *self, int argc);
    static AptValue *sMethod_getUTCFullYear(AptValue *self, int argc);
    static AptValue *sMethod_getUTCHours(AptValue *self, int argc);
    static AptValue *sMethod_getUTCMilliseconds(AptValue *self, int argc);
    static AptValue *sMethod_getUTCMinutes(AptValue *self, int argc);
    static AptValue *sMethod_getUTCMonth(AptValue *self, int argc);
    static AptValue *sMethod_getUTCSeconds(AptValue *self, int argc);
    static AptValue *sMethod_getYear(AptValue *self, int argc);
    static AptValue *sMethod_setDate(AptValue *self, int argc);
    static AptValue *sMethod_setFullYear(AptValue *self, int argc);
    static AptValue *sMethod_setHours(AptValue *self, int argc);
    static AptValue *sMethod_setMilliseconds(AptValue *self, int argc);
    static AptValue *sMethod_setMinutes(AptValue *self, int argc);
    static AptValue *sMethod_setMonth(AptValue *self, int argc);
    static AptValue *sMethod_setSeconds(AptValue *self, int argc);
    static AptValue *sMethod_setUTCDate(AptValue *self, int argc);
    static AptValue *sMethod_setUTCFullYear(AptValue *self, int argc);
    static AptValue *sMethod_setUTCHours(AptValue *self, int argc);
    static AptValue *sMethod_setUTCMilliseconds(AptValue *self, int argc);
    static AptValue *sMethod_setUTCMinutes(AptValue *self, int argc);
    static AptValue *sMethod_setUTCMonth(AptValue *self, int argc);
    static AptValue *sMethod_setUTCSeconds(AptValue *self, int argc);
    static AptValue *sMethod_setYear(AptValue *self, int argc);
    static AptValue *sMethod_toString(AptValue *self, int argc);
    static void CleanNativeFunctions();

protected:
    virtual ~AptDate();

    AptSysClock local;  // 0x20
    AptSysClock utc;    // 0x40
    int timezoneHours;  // 0x60
};

typedef char AptDateSize[(sizeof(AptDate) == 0x64) ? 1 : -1];
