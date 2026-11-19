#pragma once
// The Apt string pool: an array of EAStringC at VA 0x00E18388 indexed by a
// string constant (the retail assert text in the AptNativeHash constructor
// reads StringPool::GetString(SC___proto__) and StringPool::GetString(SC_prototype)).
// GetString 0x0070B4F0 returns &table[index] and asserts the entry was set.
// Teardown 0x0070D9F0 destroys the table. The enumeration of the SC_ constants
// is not recovered here (the initializer 0x0070B670 builds the table).
class EAStringC;

class StringPool
{
public:
    static EAStringC *GetString(int index);
    static void Teardown();
};
