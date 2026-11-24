// cl: /O2 /EHs /EHc-
// stlport46
#include <ctype.h>
#include <malloc.h>
#include <stddef.h>
#include <stdio.h>
#include "WWLib/b64pipe.h"
#include "WWLib/ini.h"
#include "WWLib/pipe.h"
// Portions derived from Command & Conquer Generals and Command & Conquer Generals Zero Hour, (c) Electronic Arts Inc., GPL-3.0-or-later; see LICENSE and NOTICE.
// ini.cpp: WWLib's INI database handler (INIClass, INISection, INIEntry).
// Target facts (code RVAs):
//  - This unit owns 61 rows from 0x00615B80 to 0x006192F0, and it also emits the
//    GenericNode/GenericList and IndexClass members it instantiates (Unlink
//    0x00615DF0, Link 0x00615E20, Add_Tail 0x00615E70, GenericList ctor
//    0x00616330 and dtor 0x006163A0, List<INIEntry *>::Delete 0x00615CB0), which
//    is what fixes the layouts: GenericNode is a vptr plus NextNode/PrevNode,
//    GenericList is two GenericNodes (0x20 bytes), so INISection's EntryList
//    (a List<INIEntry *>) puts INIEntry::Value at +0x10 and INISection's
//    EntryIndex at +0x2C -- the two offsets the porter gap list named.
//  - All three INIClass constructors call Initialize 0x006168F0, which allocates
//    a List<INISection *> and an IndexClass<int, INISection *> and stores
//    nstrdup("<unknown>"); the single vtable slot is the deleting destructor
//    0x00619100, so INIClass is {vptr, SectionList, SectionIndex, Filename}.
//  - The body shapes are the WWLib ini.cpp the binary's own strings name
//    ("Duplicate CRC in INI file.", "INIClass::Put_String"), built /O2 with the
//    asserts stripped.
#include "WWLib/ini.h"
#include "WWLib/inisup.h"
#include "WWLib/listnode.h"
#include "WWLib/index.h"
#include "WWLib/crc.h"
#include "WWLib/trim.h"
#include "WWLib/nstrdup.h"
#include "WWLib/pipe.h"
#include "WWLib/ffactory.h"
#include "WWLib/straw.h"
#include "WWLib/pk.h"
#include "WWLib/b64straw.h"
int Read_Line(Straw &straw, char *buffer, int length, bool &eof);
#include "WWLib/b64pipe.h"
#include <malloc.h>
#include "WWLib/wwstring.h"
#include "WWLib/widestring.h"
// Retail copies and frees the entry, value and section strings with the game's own
// duplicateString and freeBlock (direct five-byte calls), not the CRT imports.
#include "Common/GameMemory.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
extern "C" __declspec(dllimport) void __stdcall OutputDebugStringA(const char *);
extern "C" __declspec(dllimport) int __stdcall MessageBoxA(void *, const char *, const char *, unsigned int);
#include <ctype.h>
#include <wchar.h>


INIEntry::~INIEntry()
{
	freeBlock(Entry);
	Entry = 0;
	freeBlock(Value);
	Value = 0;
}

bool INIClass::KeepBlankEntries = false;
const int INIClass::MAX_LINE_LENGTH = 4096;

// FUNCTION: BFME2 0x00615B20 size=46 name=?Is_Empty@GenericList@@QBE_NXZ evidence=N:ledger tier=B hypothesis:mm1-neighbour-class
// FUNCTION: BFME2 0x00615B50 size=15 name=?crc@Rva00615B50Text@@QBEKXZ evidence=?:placeholder tier=B hypothesis:mm1-contiguity-class
// FUNCTION: BFME2 0x00615B60 size=6 name=?Chunk_ID@LightPersistFactoryClass@@UBEKXZ evidence=N:ledger tier=B hypothesis:mm1-contiguity-class
// FUNCTION: BFME2 0x00615B80 size=49 name=?Is_Loaded@INIClass@@QBE_NXZ evidence=N:ledger
bool INIClass::Is_Loaded() const
{
	return !SectionList->Is_Empty();
}


// FUNCTION: BFME2 0x00615BC0 size=39 name=?Strip_Comments@INIClass@@SAXPAD@Z evidence=N:ledger
void INIClass::Strip_Comments(char * buffer)
{
	if (buffer != 0) {
		char * comment = strchr(buffer, ';');
		if (comment) {
			*comment = '\0';
			strtrim(buffer);
		}
	}
}


// FUNCTION: BFME2 0x00615BF0 size=16 name=?CRC@INIClass@@SAHPBD@Z evidence=N:ledger
int INIClass::CRC(const char *string)
{
	return CRC::String(string);
}


// FUNCTION: BFME2 0x00615C00 size=91 name=?DuplicateCRCError@INIClass@@QAEXPBD00@Z evidence=N:ledger
void INIClass::DuplicateCRCError(const char *message, const char *section, const char *entry)
{
	char text[512];
	_snprintf(text, sizeof(text), "%s - Duplicate Entry \"%s\" in section \"%s\" (%s)\n", message, entry, section, Filename);
	OutputDebugStringA(text);
	MessageBoxA(0, text, "Duplicate CRC in INI file.", 0x10);
}

// FUNCTION: BFME2 0x00615C60 size=10 name=?Keep_Blank_Entries@INIClass@@SAX_N@Z evidence=N:ledger
void INIClass::Keep_Blank_Entries(bool keep_blanks)
{
	KeepBlankEntries = keep_blanks;
}


// FUNCTION: BFME2 0x00615DF0 size=45 name=?Unlink@GenericNode@@QAEXXZ evidence=N:ledger

// FUNCTION: BFME2 0x00615E20 size=75 name=?Link@GenericNode@@QAEXPAV1@@Z evidence=N:ledger

// FUNCTION: BFME2 0x00615E70 size=78 name=?Add_Tail@GenericList@@QAEXPAVGenericNode@@@Z evidence=N:ledger

// FUNCTION: BFME2 0x00615EF0 size=47 name=?Get_Length@WideStringClass@@QBEHXZ evidence=N:ledger

// Retail's string writes take the length from the text itself in a small inline helper.
static int Put_Text(Pipe &pipe, const char *text)
{
	return pipe.Put(text, strlen(text));
}

// FUNCTION: BFME2 0x00615F20 size=379 name=?Save@INIClass@@QBEHAAVPipe@@@Z evidence=N:ledger
int INIClass::Save(Pipe &pipe) const
{
	const char *eol = "\r\n";
	int total = 0;
	INISection *section = SectionList->First();
	while (section != 0 && section->Is_Valid()) {
		total += pipe.Put("[", 1);
		total += Put_Text(pipe, section->Section);
		total += pipe.Put("]", 1);
		total += Put_Text(pipe, eol);

		INIEntry *entry = section->EntryList.First();
		while (entry != 0 && entry->Is_Valid()) {
			total += Put_Text(pipe, entry->Entry);
			total += pipe.Put("=", 1);
			total += Put_Text(pipe, entry->Value);
			total += Put_Text(pipe, eol);
			entry = entry->Next();
		}
		total += Put_Text(pipe, eol);
		section = section->Next();
	}
	return total + pipe.End();
}

// FUNCTION: BFME2 0x006160A0 size=7 name=?get@Rva006160A0PtrChaseField@@QBEHXZ evidence=?:placeholder

// FUNCTION: BFME2 0x00616330 size=103 name=??0GenericList@@QAE@XZ evidence=N:ledger

// FUNCTION: BFME2 0x006163A0 size=149 name=??1GenericList@@UAE@XZ evidence=N:ledger

// FUNCTION: BFME2 0x006165B0 size=145 name=?Save@INIClass@@QBEHAAVFileClass@@@Z evidence=N:ledger
int INIClass::Save(FileClass &file) const
{
	FilePipe pipe(&file);
	delete Filename;
	Filename = nstrdup(file.File_Name());
	return Save(pipe);
}

// FUNCTION: BFME2 0x00616650 size=82 name=?Save@INIClass@@QBEHPBD@Z evidence=N:ledger
int INIClass::Save(const char *filename) const
{
	RawFileClass *file = _TheWritingFileFactory->Get_File(filename);
	int result = 0;
	if (file != 0) {
		result = Save(*file);
		_TheWritingFileFactory->Return_File(file);
	}
	delete Filename;
	Filename = nstrdup(filename);
	return result;
}

// FUNCTION: BFME2 0x006167E0 size=65 name=??0INISection@@QAE@PAD@Z evidence=N:ledger
INISection::INISection(char * section) : Section(section)
{
}


// FUNCTION: BFME2 0x00616830 size=189 name=??1INISection@@UAE@XZ evidence=N:ledger
INISection::~INISection()
{
	freeBlock(Section);
	Section = 0;
	EntryList.Delete();
}


// FUNCTION: BFME2 0x006168F0 size=98 name=?Initialize@INIClass@@AAEXXZ evidence=N:ledger
void INIClass::Initialize()
{
	SectionList = new List<INISection *> ();
	SectionIndex = new IndexClass<int, INISection *> ();
	Filename = nstrdup("<unknown>");
}


// FUNCTION: BFME2 0x006169A0 size=25 name=??0INIClass@@QAE@XZ evidence=N:ledger
INIClass::INIClass() : Filename(0)
{
	Initialize();
}


// FUNCTION: BFME2 0x00616AC0 size=86 name=?Shutdown@INIClass@@AAEXXZ evidence=N:ledger
void INIClass::Shutdown()
{
	::delete SectionList;
	::delete SectionIndex;
	delete Filename;
}


// FUNCTION: BFME2 0x00616B20 size=102 name=?Find_Section@INIClass@@QBEPAUINISection@@PBD@Z evidence=N:ledger
INISection * INIClass::Find_Section(const char * section) const
{
	if (section != 0) {
		long crc = CRC(section);
		if (SectionIndex->Is_Present(crc)) {
			return (*SectionIndex)[crc];
		}
	}
	return 0;
}


// FUNCTION: BFME2 0x00616B90 size=25 name=?Entry_Count@INIClass@@QBEHPBD@Z evidence=N:ledger
int INIClass::Entry_Count(const char * section) const
{
	INISection * sec = Find_Section(section);
	if (sec != 0) {
		return sec->EntryIndex.Count();
	}
	return 0;
}


// FUNCTION: BFME2 0x00616BB0 size=69 name=?Get_Entry@INIClass@@QBEPBDPBDH@Z evidence=N:ledger
const char * INIClass::Get_Entry(const char * section, int index) const
{
	INISection * sec = Find_Section(section);

	if (sec != 0 && index < sec->EntryIndex.Count()) {
		INIEntry * ent = sec->EntryList.First();

		while (ent != 0 && ent->Is_Valid()) {
			if (index == 0) return(ent->Entry);
			index--;
			ent = ent->Next();
		}
	}
	return 0;
}


// FUNCTION: BFME2 0x00616C00 size=89 name=?Find_Entry@INISection@@QBEPAUINIEntry@@PBD@Z evidence=N:ledger
INIEntry * INISection::Find_Entry(const char * entry) const
{
	if (entry != 0) {
		int crc = CRC::String(entry);
		if (EntryIndex.Is_Present(crc)) {
			return EntryIndex[crc];
		}
	}
	return 0;
}


// FUNCTION: BFME2 0x00616C60 size=225 name=?Clear@INIClass@@QAE_NPBD0@Z evidence=N:ledger
bool INIClass::Clear(const char * section, const char * entry)
{
	if (section == 0) {
		SectionList->Delete();
		SectionIndex->Clear();

		delete Filename;
		Filename = nstrdup("<unknown>");
	} else {
		INISection * sec = Find_Section(section);
		if (sec != 0) {
			if (entry != 0) {
				INIEntry * ent = sec->Find_Entry(entry);
				if (ent != 0) {
					sec->EntryIndex.Remove_Index(ent->Index_ID());
					::delete ent;
				}
			} else {
				SectionIndex->Remove_Index(sec->Index_ID());
				::delete sec;
			}
		}
	}

	return true;
}


// FUNCTION: BFME2 0x00616D50 size=34 name=?Find_Entry@INIClass@@QBEPAUINIEntry@@PBD0@Z evidence=N:ledger
INIEntry * INIClass::Find_Entry(const char * section, const char * entry) const
{
	INISection * sec = Find_Section(section);
	if (sec != 0) {
		return sec->Find_Entry(entry);
	}
	return 0;
}


// FUNCTION: BFME2 0x00616D80 size=187 name=?Get_Int@INIClass@@QBEHPBD0H@Z evidence=N:ledger
int INIClass::Get_Int(const char * section, const char * entry, int fallback) const
{
	if (section == 0 || entry == 0) return(fallback);

	INIEntry * ent = Find_Entry(section, entry);
	if (ent && ent->Value != 0) {

		if (*ent->Value == '$') {
			sscanf(ent->Value, "$%x", &fallback);
		} else {
			if (tolower(ent->Value[strlen(ent->Value)-1]) == 'h') {
				sscanf(ent->Value, "%xh", &fallback);
			} else {
				fallback = atoi(ent->Value);
			}
		}
	}
	return fallback;
}


// FUNCTION: BFME2 0x00616E40 size=74 name=?Get_Hex@INIClass@@QBEHPBD0H@Z evidence=N:ledger
int INIClass::Get_Hex(const char * section, const char * entry, int fallback) const
{
	if (section == 0 || entry == 0) return(fallback);

	INIEntry * ent = Find_Entry(section, entry);
	if (ent && ent->Value != 0) {
		sscanf(ent->Value, "%x", &fallback);
	}
	return fallback;
}


// FUNCTION: BFME2 0x00616E90 size=136 name=?Get_Float@INIClass@@QBEMPBD0M@Z evidence=N:ledger
float INIClass::Get_Float(const char * section, const char * entry, float fallback) const
{
	if (section == 0 || entry == 0) return(fallback);

	INIEntry * ent = Find_Entry(section, entry);
	if (ent != 0 && ent->Value != 0) {
		float val = fallback;
		sscanf(ent->Value, "%f", &val);
		fallback = val;
		if (strchr(ent->Value, '%') != 0) {
			fallback /= 100.0f;
		}
	}
	return fallback;
}


// FUNCTION: BFME2 0x00616F20 size=131 name=?Get_Double@INIClass@@QBENPBD0N@Z evidence=N:ledger
double INIClass::Get_Double(const char * section, const char * entry, double fallback) const
{
	if (section == 0 || entry == 0) return(fallback);

	INISection * sec = Find_Section(section);
	if (sec == 0) return(fallback);

	INIEntry * ent = sec->Find_Entry(entry);
	if (ent != 0 && ent->Value != 0) {
		double val = fallback;
		sscanf(ent->Value, "%lf", &val);
		fallback = val;
		if (strchr(ent->Value, '%') != 0) {
			fallback *= 0.01;
		}
	}
	return fallback;
}


// FUNCTION: BFME2 0x00616FB0 size=554 name=?Put_String@INIClass@@QAE_NPBD00@Z evidence=N:ledger
bool INIClass::Put_String(const char * section, const char * entry, const char * string)
{
	if (section == 0 || entry == 0) return(false);

	INISection * sec = Find_Section(section);

	if (sec == 0) {
		sec = new INISection(duplicateString(section));
		if (sec == 0) return(false);
		SectionList->GenericList::Add_Tail(sec);
		SectionIndex->Add_Index(sec->Index_ID(), sec);
	}

	INIEntry * ent = sec->Find_Entry(entry);
	if (ent != 0) {
		if (strcmp(ent->Entry, entry)) {
			DuplicateCRCError("INIClass::Put_String", section, entry);
		}
		sec->EntryIndex.Remove_Index(ent->Index_ID());
		::delete ent;
	}

	if (string != 0 && strlen(string) > 0) {
		ent = new INIEntry(duplicateString(entry), duplicateString(string));
		if (ent == 0) {
			return false;
		}
		sec->EntryList.GenericList::Add_Tail(ent);
		sec->EntryIndex.Add_Index(ent->Index_ID(), ent);
	}
	return true;
}


// FUNCTION: BFME2 0x006171E0 size=114 name=?Get_String@INIClass@@QBEHPBD00PADH@Z evidence=N:ledger
int INIClass::Get_String(const char * section, const char * entry, const char * fallback, char * buffer, int size) const
{
	if (buffer == 0 || size < 2 || section == 0 || entry == 0) return(0);

	INIEntry * ent = Find_Entry(section, entry);
	if (ent != 0 && ent->Value != 0) {
		fallback = ent->Value;
	}

	if (fallback == 0) {
		buffer[0] = '\0';
		return 0;
	} else {
		strncpy(buffer, fallback, size);
		buffer[size-1] = '\0';
		strtrim(buffer);
		return strlen(buffer);
	}
}


// FUNCTION: BFME2 0x00617260 size=92 name=?Get_String@INIClass@@QBEABVStringClass@@AAV2@PBD11@Z evidence=N:ledger
const StringClass &INIClass::Get_String(StringClass &new_string, const char *section, const char *entry, const char *defvalue) const
{
	if (section == NULL || entry == NULL) {
		new_string="";
		return new_string;
	}

	INIEntry * entryptr = Find_Entry(section, entry);
	if (entryptr != NULL) {
		defvalue = entryptr->Value;
	}

	if (defvalue == NULL) {
		new_string="";
		return new_string;
	}
	new_string=defvalue;
	return new_string;
}

// FUNCTION: BFME2 0x006172C0 size=71 name=?Get_Alloc_String@INIClass@@QBEPADPBD00@Z evidence=N:ledger

char *INIClass::Get_Alloc_String(const char * section, const char * entry, const char * fallback) const
{
	if (section == 0 || entry == 0) return(0);

	INIEntry * ent = Find_Entry(section, entry);
	if (ent != 0) {
		fallback = ent->Value;
	}

	if (fallback == 0) return 0;
	return duplicateString(fallback);
}



// FUNCTION: BFME2 0x00617310 size=132 name=?Get_List_Index@INIClass@@QAEHPBD0HQAPAD@Z evidence=N:ledger

int INIClass::Get_List_Index(const char * section, const char * entry, const int fallback, char *list[])
{
	if (section == 0 || entry == 0) return(0);

	INIEntry * ent = Find_Entry(section, entry);
	if (ent == 0 || ent->Value == 0) {
		return fallback;
	}

	for (int lp = 0; list[lp]; lp++) {
		if (_strcmpi(ent->Value, list[lp]) == 0) {
			return lp;
		}
	}
	return fallback;
}



// FUNCTION: BFME2 0x006173A0 size=210 name=?Get_Int_Bitfield@INIClass@@QAEHPBD0HQAPAD@Z evidence=N:ledger
int INIClass::Get_Int_Bitfield(const char *section, const char *entry, int defvalue, char *list[])
{
	INISection *sec = Find_Section(section);
	if (sec != 0) {
		INIEntry *found = sec->Find_Entry(entry);
		if (found != 0 && found->Value != 0) {
			// Each delimited token names a flag by its position in the caller's list; the flags are OR-ed together.
			int flags = 0;
			char *scratch = duplicateString(found->Value);
			char *token = strtok(scratch, "|+");
			while (token != 0) {
				int position = 0;
				if (list[position] != 0) {
					do {
						if (_strcmpi(token, list[position]) == 0) {
							flags |= 1 << position;
							break;
						}
						position++;
					} while (list[position] != 0);
				}
				token = strtok(0, "|+");
			}
			freeBlock(scratch);
			return flags;
		}
	}
	return defvalue;
}

// FUNCTION: BFME2 0x00617480 size=238 name=?Get_Alloc_Int_Array@INIClass@@QAEPAHPBD0H@Z evidence=N:ledger
int *INIClass::Get_Alloc_Int_Array(const char *section, const char *entry, int listend)
{
	INISection *sec = Find_Section(section);
	if (sec != 0) {
		INIEntry *found = sec->Find_Entry(entry);
		if (found != 0 && found->Value != 0) {
			// Count the space-separated fields on a scratch copy, then convert them on a second copy.
			int count = 0;
			char *scratch = duplicateString(found->Value);
			for (char *token = strtok(scratch, " "); token != 0; token = strtok(0, " "))
				count++;
			freeBlock(scratch);

			int *values = (int *)::operator new(count * 4 + 4);
			int filled = 0;
			scratch = duplicateString(found->Value);
			for (char *token = strtok(scratch, " "); token != 0; token = strtok(0, " "))
				values[filled++] = atoi(token);
			freeBlock(scratch);
			values[filled] = listend;
			return values;
		}
	}

	int *only = (int *)::operator new(4);
	only[0] = listend;
	return only;
}

// FUNCTION: BFME2 0x00617570 size=28 name=?Put_Bool@INIClass@@QAE_NPBD0_N@Z evidence=N:ledger
bool INIClass::Put_Bool(const char * section, const char * entry, bool value)
{
	if (value) {
		return Put_String(section, entry, "yes");
	} else {
		return Put_String(section, entry, "no");
	}
}


// FUNCTION: BFME2 0x00617590 size=104 name=?Get_Bool@INIClass@@QBE_NPBD0_N@Z evidence=N:ledger
bool INIClass::Get_Bool(const char * section, const char * entry, bool fallback) const
{
	if (section != 0 && entry != 0) {
		INISection * sec = Find_Section(section);
		if (sec != 0) {
			INIEntry * ent = sec->Find_Entry(entry);
			if (ent != 0 && ent->Value != 0) {
				switch (toupper(*ent->Value)) {
					case '1':
					case 'T':
					case 'Y':
						return true;

					case '0':
					case 'F':
					case 'N':
						return false;
				}
			}
		}
	}
	return fallback;
}


// FUNCTION: BFME2 0x00617A70 size=79 name=?Is_Present@INIClass@@QBE_NPBD0@Z evidence=N:ledger
bool INIClass::Is_Present(const char * section, const char * entry) const
{
	if (entry == 0) return(Find_Section(section) != 0);
	return Find_Entry(section, entry) != 0;
}


// FUNCTION: BFME2 0x00617AC0 size=79 name=??1INIClass@@UAE@XZ evidence=N:ledger
INIClass::~INIClass()
{
	Clear();
	Shutdown();
}


// FUNCTION: BFME2 0x00617B10 size=1827 name=?Load@INIClass@@QAEHAAVStraw@@@Z evidence=N:ledger

// FUNCTION: BFME2 0x00618240 size=367 name=?Enumerate_Entries@INIClass@@QAEIPBD0II@Z evidence=N:ledger

unsigned INIClass::Enumerate_Entries(const char *section, const char *entry_prefix, unsigned startnumber, unsigned endnumber)
{
	unsigned number = startnumber;
	bool found = false;
	char name[256];
	do {
		sprintf(name, "%s%d", entry_prefix, number);
		found = Is_Present(section, name);
		if (found) number++;
	} while (found && number < endnumber);
	return number - startnumber;
}



// FUNCTION: BFME2 0x006183B0 size=332 name=?Put_UUBlock@INIClass@@QAE_NPBDPBXH@Z evidence=N:ledger
bool INIClass::Put_UUBlock(const char *section, const void *block, int len)
{
	if (section == 0 || block == 0 || len < 1) return false;

	Clear(section);
	BufferStraw source(block, len);
	Base64Straw encoder(Base64Straw::ENCODE);
	encoder.Get_From(source);

	int index = 1;
	char line[71];
	int count = encoder.Get(line, 70);
	line[count] = 0;
	while (count != 0) {
		char key[32];
		sprintf(key, "%d", index);
		Put_String(section, key, line);
		index++;
		count = encoder.Get(line, 70);
		line[count] = 0;
	}
	return true;
}

// FUNCTION: BFME2 0x00618500 size=451 name=?Get_UUBlock@INIClass@@QBEHPBDPAXH@Z evidence=N:ledger
int INIClass::Get_UUBlock(const char *section, void *block, int len) const
{
	if (section == 0) return 0;

	Base64Pipe decoder(Base64Pipe::DECODE);
	BufferPipe sink(block, len);
	decoder.Put_To(&sink);

	int total = 0;
	int count = Entry_Count(section);
	for (int index = 0; index < count; index++) {
		char line[128];
		int length = Get_String(section, Get_Entry(section, index), "=", line, sizeof(line));
		total += decoder.Put(line, length);
	}
	return decoder.End() + total;
}

// FUNCTION: BFME2 0x006186D0 size=361 name=?Get_Wide_String@INIClass@@QBEABVWideStringClass@@AAV2@PBD1PBG@Z evidence=N:ledger
const WideStringClass &INIClass::Get_Wide_String(WideStringClass &new_string, const char *section, const char *entry, const wchar_t *defvalue) const
{
	wchar_t out[1024];
	char buffer[1024];

	Base64Pipe b64pipe(Base64Pipe::DECODE);
	BufferPipe bpipe(out, sizeof(out));
	b64pipe.Put_To(&bpipe);

	int length = Get_String(section, entry, "", buffer, sizeof(buffer));
	if (length == 0) {
		new_string = defvalue;
	} else {
		int outcount = b64pipe.Put(buffer, length);
		outcount += b64pipe.End();
		new_string = out;
	}
	return(new_string);
}

// FUNCTION: BFME2 0x00618840 size=298 name=?Put_Wide_String@INIClass@@QAE_NPBD0PBG@Z evidence=N:ledger
bool INIClass::Put_Wide_String(const char *section, const char *entry, const wchar_t *string)
{
	if (section == 0 || entry == 0 || string == 0) return false;

	WideStringClass wide(string, true);
	int chars = wide.Get_Length();
	if (chars == 0) {
		Put_String(section, entry, "");
	} else {
		char *text = (char *)alloca(chars * 8 + 0x20);
		BufferStraw source(string, chars * 2 + 2);
		Base64Straw encoder(Base64Straw::ENCODE);
		encoder.Get_From(source);

		int total = 0;
		int got;
		do {
			got = encoder.Get(text + total, 16);
			total += got;
		} while (got != 0);
		text[total] = 0;
		Put_String(section, entry, text);
	}
	return true;
}

// FUNCTION: BFME2 0x00618970 size=220 name=?Put_UUBlock@INIClass@@QAE_NPBD0PBXH@Z evidence=N:ledger
bool INIClass::Put_UUBlock(const char *section, const char *entry, const void *block, int len)
{
	if (section == 0 || block == 0 || len < 1) return false;

	BufferStraw source(block, len);
	Base64Straw encoder(Base64Straw::ENCODE);
	encoder.Get_From(source);

	char *text = (char *)alloca(len * 3);
	int count = encoder.Get(text, len * 3 - 1);
	text[count] = 0;
	Put_String(section, entry, text);
	return true;
}

// FUNCTION: BFME2 0x00618A50 size=225 name=?Get_UUBlock@INIClass@@QBEHPBD0PAXH@Z evidence=N:ledger
int INIClass::Get_UUBlock(char const * section, char const *entry, void * block, int len) const
{
	if (section == NULL) return(0);
	if (entry == NULL) return(0);

	Base64Pipe b64pipe(Base64Pipe::DECODE);
	BufferPipe bpipe(block, len);

	b64pipe.Put_To(&bpipe);

	int total = 0;
	char *buffer = (char*) _alloca(len * 3);

	int length = Get_String(section, entry, "=", buffer, len*3);
	int outcount = b64pipe.Put(buffer, length);
	total += outcount;
	total += b64pipe.End();
	return(total);
}

// FUNCTION: BFME2 0x00618B40 size=273 name=?Put_TextBlock@INIClass@@QAE_NPBD0@Z evidence=N:ledger

bool INIClass::Put_TextBlock(const char * section, const char * text)
{
	if (section == 0) return(false);

	Clear(section);

	int index = 1;
	while (text != 0 && *text != 0) {

		char buffer[128];

		strncpy(buffer, text, 75);
		buffer[75] = '\0';

		char b[32];
		sprintf(b, "%d", index);

		int count = strlen(buffer);
		if (count > 0) {
			if (count >= 75) {
				while (count) {
					char c = buffer[count];

					if (isspace(c)) break;
					count--;
				}

				if (count == 0) {
					break;
				} else {
					buffer[count] = '\0';
				}
			}

			strtrim(buffer);
			Put_String(section, b, buffer);
			index++;
			text = ((char  *)text) + count;
		} else {
			break;
		}
	}
	return true;
}



// FUNCTION: BFME2 0x00618C60 size=185 name=?Get_TextBlock@INIClass@@QBEHPBDPADH@Z evidence=N:ledger

int INIClass::Get_TextBlock(const char * section, char * buffer, int len) const
{
	if (len <= 0) return(0);

	buffer[0] = 0;
	if (len <= 1) return(0);

	int entries = Entry_Count(section);
	int total = 0;
	for (int index = 0; index < entries; index++) {

		if (index > 0) {
			*buffer++ = ' ';
			len--;
			total++;
		}

		Get_String(section, Get_Entry(section, index), "", buffer, len);

		int partial = strlen(buffer);
		len -= partial;
		total += partial;
		buffer += partial;
		if (len <= 1) break;
	}
	return total;
}



// FUNCTION: BFME2 0x00618D20 size=107 name=?Put_Int@INIClass@@QAE_NPBD0HH@Z evidence=N:ledger
bool INIClass::Put_Int(const char * section, const char * entry, int number, int format)
{
	char buffer[64];

	switch (format) {
		default:
		case 0:
			sprintf(buffer, "%d", number);
			break;

		case 1:
			sprintf(buffer, "%Xh", number);
			break;

		case 2:
			sprintf(buffer, "$%X", number);
			break;
	}
	return Put_String(section, entry, buffer);
}


// FUNCTION: BFME2 0x00618EF0 size=59 name=?Put_Hex@INIClass@@QAE_NPBD0H@Z evidence=N:ledger
bool INIClass::Put_Hex(const char * section, const char * entry, int number)
{
	char buffer[64];

	sprintf(buffer, "%X", number);
	return Put_String(section, entry, buffer);
}


// FUNCTION: BFME2 0x00618F30 size=64 name=?Put_Float@INIClass@@QAE_NPBD0M@Z evidence=N:ledger
bool INIClass::Put_Float(const char * section, const char * entry, float number)
{
	char buffer[64];

	sprintf(buffer, "%f", number);
	return Put_String(section, entry, buffer);
}


// FUNCTION: BFME2 0x00618F70 size=64 name=?Put_Double@INIClass@@QAE_NPBD0N@Z evidence=N:ledger
bool INIClass::Put_Double(const char * section, const char * entry, double number)
{
	char buffer[64];

	sprintf(buffer, "%lf", number);
	return Put_String(section, entry, buffer);
}


// FUNCTION: BFME2 0x00618FB0 size=90 name=?Put_PKey@INIClass@@QAE_NABVPKey@@@Z evidence=N:ledger
bool INIClass::Put_PKey(const PKey &key)
{
	char buffer[512];

	int length = key.Encode_Modulus(buffer);
	Put_UUBlock("PublicKey", buffer, length);
	length = key.Encode_Exponent(buffer);
	Put_UUBlock("PrivateKey", buffer, length);
	return true;
}

// FUNCTION: BFME2 0x00619010 size=236 name=?Get_PKey@INIClass@@QBE?AVPKey@@_N@Z evidence=N:ledger
PKey INIClass::Get_PKey(bool fast) const
{
	char buffer[512];
	PKey key;

	if (fast) {
		Int<PKey::MAX_UNIT_PRECISION> fast_exponent(0x10001);
		fast_exponent.DEREncode((unsigned char *)buffer);
		key.Decode_Exponent(buffer);
	} else {
		Get_UUBlock("PrivateKey", buffer, sizeof(buffer));
		key.Decode_Exponent(buffer);
	}
	Get_UUBlock("PublicKey", buffer, sizeof(buffer));
	key.Decode_Modulus(buffer);
	return key;
}

// FUNCTION: BFME2 0x00619120 size=145 name=?Load@INIClass@@QAEHAAVFileClass@@@Z evidence=N:ledger
int INIClass::Load(FileClass &file)
{
	FileStraw straw(file);
	delete Filename;
	Filename = nstrdup(file.File_Name());
	return Load(straw);
}

// FUNCTION: BFME2 0x006191C0 size=131 name=?Load@INIClass@@QAEHPBD@Z evidence=N:ledger
int INIClass::Load(const char *filename)
{
	file_auto_ptr file(_TheFileFactory, filename);
	int result = Load(*file.File);
	delete Filename;
	Filename = nstrdup(filename);
	return result;
}

// FUNCTION: BFME2 0x00619250 size=39 name=??0INIClass@@QAE@AAVFileClass@@@Z evidence=N:ledger
INIClass::INIClass(FileClass &file) : Filename(0)
{
	Initialize();
	Load(file);
}

// FUNCTION: BFME2 0x00619280 size=71 name=??0INIClass@@QAE@PBD@Z evidence=N:ledger
INIClass::INIClass(const char *filename) : Filename(0)
{
	Initialize();
	FileClass *file = _TheFileFactory->Get_File(filename);
	if (file != 0) {
		Load(*file);
		_TheFileFactory->Return_File(file);
	}
}

