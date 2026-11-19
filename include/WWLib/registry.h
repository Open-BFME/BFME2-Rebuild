// Portions derived from Command & Conquer Generals and Command & Conquer Generals Zero Hour, (c) Electronic Arts Inc., GPL-3.0-or-later; see LICENSE and NOTICE.
// registry.h: WWLib's wrapper over one HKEY_LOCAL_MACHINE sub key
// (WWVegas/WWLib/registry.h).
// SIZE 0x08, no bases, no virtuals.
// Target facts (retail game.dat): the constructor 0x0060F190 stores the opened
// key handle at +0 (as an int) and sets the validity flag at +4 only when the
// open succeeded; the lock flag is a class static that turns every writer into a
// no-op and makes the constructor open read-only. Get_Bool 0x0060F280 and Set_Bool
// 0x0060F2F0 call Get_Int 0x0060F230 and Set_Int 0x0060F390. Get_String is
// overloaded for char buffers (0x0060F450), StringClass (0x0060FC80) and wide
// strings (0x0060FD00); Set_String for char (0x0060F4C0) and wide (0x0060F520).
// Donor facts: names follow Zero Hour's WWLib/registry.h (GPL), naming only.
#pragma once
#ifndef BFME2_REGISTRY_H
#define BFME2_REGISTRY_H

class StringClass;
class WideStringClass;
template <class T> class DynamicVectorClass;
class INIClass;

class RegistryClass
{
public:
	RegistryClass(const char *sub_key, bool create = true);			// 0x0060F190
	~RegistryClass();							// 0x0060F210

	bool Is_Valid() { return IsValid; }

	int Get_Int(const char *name, int def_value = 0);			// 0x0060F230
	void Set_Int(const char *name, int value);				// 0x0060F390
	bool Get_Bool(const char *name, bool def_value = false);		// 0x0060F280
	void Set_Bool(const char *name, bool value);				// 0x0060F2F0
	float Get_Float(const char *name, float def_value = 0);			// 0x0060F330
	void Set_Float(const char *name, float value);
	int Get_Bin_Size(const char *name);					// 0x0060F3C0
	void Get_Bin(const char *name, void *buffer, int buffer_size);		// 0x0060F3F0
	void Set_Bin(const char *name, const void *buffer, int buffer_size);	// 0x0060F420
	char *Get_String(const char *name, char *value, int value_size, const char *default_string = 0);	// 0x0060F450
	void Get_String(const char *name, StringClass &string, const char *default_string = 0);		// 0x0060FC80
	void Get_String(const unsigned short *name, WideStringClass &string, const unsigned short *default_string = 0);	// 0x0060FD00
	void Set_String(const char *name, const char *value);			// 0x0060F4C0
	void Set_String(const unsigned short *name, const unsigned short *value);	// 0x0060F520
	void Delete_Value(const char *name);					// 0x0060F500
	void Get_Value_List(DynamicVectorClass<StringClass> &list);		// 0x00610110
	void Deleta_All_Values();						// 0x00610300

	static bool Exists(const char *sub_key);				// 0x0060F150
	static void Save_Registry(const char *filename, char *path);		// 0x0060F920
	static void Delete_Registry_Tree(char *path);				// 0x0060FA30
	static void Load_Registry(const char *filename, char *old_path, char *new_path);	// 0x0060FDC0

private:
	static void Save_Registry_Values(void *key, char *path, INIClass *ini);	// 0x0060F560
	static void Save_Registry_Tree(char *path, INIClass *ini);		// 0x0060F750
	static void Delete_Registry_Values(void *key);				// 0x0060F9B0

	int Key;				// +0x00
	bool IsValid;				// +0x04
	static bool IsLocked;
};
// SIZE 0x08

#endif
