// CreateAHeroManager.h: the Create-a-Hero manager, partial. Only the roll chance
// that retail's hero roll reads (rva00406DE3RollChance, 0x00406DE3) is recovered
// here. SIZE and the rest of the members are not established yet; the Draft
// stub include/Draft/CreateAHeroManager.h is superseded by this header once the
// manager's remaining members are recovered.
#pragma once
#ifndef BFME2_CREATEAHEROMANAGER_H
#define BFME2_CREATEAHEROMANAGER_H

#include <stddef.h>
#include <map>
#include <vector>
#include "Common/GameType.h"
#include "Common/AsciiString.h"
#include "GameLogic/CreateAHeroDataList.h"

// Records the manager's bling and binder arrays hold (0x10 and 0x14 bytes). The map names the bling record CreateAHeroBlingEntry.
struct CreateAHeroBlingEntry
{
	AsciiString m_nameTag;			// +0x00 the bling's name tag (GetBlingNameTag)
	AsciiString m_descTag;			// +0x04 the bling's description tag (GetBlingDescTag)
	AsciiString m_upgradeName;			// +0x08 the bling's upgrade name (GetBlingUpgradeName)
	Int m_value;						// +0x0C (FindBlingByUpgradeName 0x0021D4CE returns it)
};
typedef char CreateAHeroBlingEntrySize[sizeof(CreateAHeroBlingEntry) == 0x10 ? 1 : -1];

class CreateAHeroBinderRecord
{
public:
	unsigned char m_unk0x00[0x14];		// 20-byte records, contents not yet recovered
};
typedef char CreateAHeroBinderRecordSize[sizeof(CreateAHeroBinderRecord) == 0x14 ? 1 : -1];

// One attribute record a sub-class keeps: the attribute getters read the minimum at +4, the maximum at +8
// and the default at +0xC (0x0021BDAB, 0x0021BDD3, 0x0021BDFB). The lookup that finds it is 0x0021BD22.
class CreateAHeroAttribute
{
public:
	unsigned char m_unk0x00[0x04];	// +0x00, the attribute key, not yet recovered
	AsciiString m_min;				// +0x04
	AsciiString m_max;				// +0x08
	AsciiString m_default;			// +0x0C
};

// A four-byte zero mask (retail's guarded fallback for a bad faction-mask index): its constructor
// clears it through msvcr's memset import, which is called rather than expanded.
#include <string.h>
class CreateAHeroZeroMask
{
public:
	CreateAHeroZeroMask() throw() { memset(m_bits, 0, sizeof(m_bits)); }
	unsigned char m_bits[4];
};

class CreateAHeroManager
{
public:
	class CreateAHeroSubClass;
	class CreateAHeroClass;
	// A sub-class's bling groups: bling key -> the bling ids of the group (retail's STLport map at sub-class +0x24).
	typedef std::map<Int, std::vector<UnsignedInt> > BlingGroupMap;
	// Colour words for a hero's class and subclass (retail 0x0021A016, 0x0021A041,
	// 0x0021A06C: red, green, blue). Declared for the hero's SetDefaultColors;
	// not defined in this unit.
	Int rva0021A016(Int heroClass, Int heroSubclass);
	Int rva0021A041(Int heroClass, Int heroSubclass);
	Int rva0021A06C(Int heroClass, Int heroSubclass);
	// Bling count of a hero's bling group (retail 0x00219239; a null hero counts as zero).
	Int GetBlingCount(Int blingKey, const class CreateAHeroHero *hero) const;
	void *GetBling(UnsignedInt index);				// 0x00219D85: the bling record at index, or null
	class CreateAHeroData *rva002197A6(Int objectID);		// 0x002197A6: the create-a-hero record of the object with this ID, or null (RespawnUpdate::getDeadImage 0x004AF2DE; a CreateAHeroHero)
	const AsciiString &GetBlingNameTag(Int blingKey, const class CreateAHeroHero *hero, UnsignedInt index);	// 0x00219DAA
	const AsciiString &GetBlingDescTag(Int blingKey, const class CreateAHeroHero *hero, UnsignedInt index);	// 0x00219DEA
	const AsciiString &GetBlingUpgradeName(Int blingKey, const class CreateAHeroHero *hero, UnsignedInt index);	// 0x00219E2F
	Int GetStatCount(UnsignedInt classIndex, UnsignedInt stat);	// 0x00219E74
	Int GetSubClassSpendableAttributePoints(UnsignedInt classIndex, UnsignedInt subClass);	// 0x0021A097
	Int GetAwardCount(UnsignedInt classIndex, UnsignedInt subClass);	// 0x00219ED5
	Int GetStatNameKey(UnsignedInt classIndex, UnsignedInt subClass, UnsignedInt stat);	// 0x00219E9F
	Int GetAwardNameKey(UnsignedInt classIndex, UnsignedInt subClass, UnsignedInt award);	// 0x00219F00
	Int GetPreferedFaction(UnsignedInt classIndex, UnsignedInt index);	// 0x00219FE3
	const class CreateAHeroCamera *GetViewInfo(UnsignedInt heroClass, UnsignedInt subClass);	// 0x00219F36 (ledger name; the map spells the return PBVRva002195E6, respell asked of m08; decl_queue k02 17:51, ACCEPTED k11)
	CreateAHeroClass *rva0021B31C(const AsciiString &upgradeName);	// 0x0021B31C: the class with this upgrade name, or null
	Int GetRequiredButtonCount();	// 0x00219309: the buttons the command set holds (at most 32)
	const class CommandButton *GetRequiredButton(UnsignedInt index);	// 0x0021933E
	const AsciiString &GetClassUpgradeName(UnsignedInt classIndex);	// 0x0021A134
	Int GetSubClassAttributeMinValue(UnsignedInt attribute, UnsignedInt classIndex, UnsignedInt subClassIndex);	// 0x0021BF42
	Int GetSubClassAttributeMaxValue(UnsignedInt attribute, UnsignedInt classIndex, UnsignedInt subClassIndex);	// 0x0021BF74
	Int GetSubClassAttributeDefaultValue(UnsignedInt attribute, UnsignedInt classIndex, UnsignedInt subClassIndex);	// 0x0021BFA6
	Int GetSubClassDefaultBlingId(Int blingKey, UnsignedInt classIndex, UnsignedInt subClassIndex) const;	// 0x0021BF11
	Int rva0021A016(UnsignedInt classIndex, UnsignedInt subClassIndex);	// 0x0021A016: rva00219CDF of the class
	Int rva0021A041(UnsignedInt classIndex, UnsignedInt subClassIndex);	// 0x0021A041: rva00219CF6 of the class
	Int rva0021A06C(UnsignedInt classIndex, UnsignedInt subClassIndex);	// 0x0021A06C: rva00219D0D of the class
	Int rva0021A18B(UnsignedInt classIndex, UnsignedInt subClassIndex);	// 0x0021A18B: rva00219D3B of the class
	const CreateAHeroSubClass *rva0021A1B6(UnsignedInt classIndex, UnsignedInt subClassIndex);	// 0x0021A1B6: the sub-class at index
	Int rva002196DA();	// 0x002196DA: the class count
	void DestroyHero(class CreateAHeroData **hero);	// 0x0021929D: deletes *hero and clears it
	Int rva00219D52(UnsignedInt classIndex);	// 0x00219D52: the class's sub-class count, or 0
	Int rva0021BEE0(Int blingKey, UnsignedInt classIndex, UnsignedInt subClassIndex) const;	// 0x0021BEE0: the sub-class's bling count of blingKey
	void *GetFactionMaskType(UnsignedInt classIndex, UnsignedInt subClassIndex);	// 0x00219F8E: the sub-class's faction mask, or a zero mask
	Int rva002196EC();	// 0x002196EC: the binder count
	const AsciiString &GetClassNameTag(UnsignedInt classIndex);	// 0x0021AF7E
	const AsciiString &GetClassDescTag(UnsignedInt classIndex);	// 0x0021AFEA
	const AsciiString &GetClassPowersTag(UnsignedInt classIndex);	// 0x0021B05A
	const AsciiString &GetClassImageName(UnsignedInt classIndex);	// 0x0021B0CA
	const AsciiString &GetSubClassNameTag(UnsignedInt classIndex, UnsignedInt subClass);	// 0x0021B13A
	const AsciiString &GetSubClassImageName(UnsignedInt classIndex, UnsignedInt subClass);	// 0x0021B1B4
	const AsciiString &GetButtonImageName(UnsignedInt classIndex, UnsignedInt subClass);	// 0x0021B22E
	const AsciiString &GetButtonImageName(const class CreateAHeroHero *hero);	// 0x0021B670: the button of the hero's class and sub-class, or the empty string
	const AsciiString &GetSubClassDescTag(UnsignedInt classIndex, UnsignedInt subClass);	// 0x0021B2A2
	const AsciiString &GetSubClassUpgradeName(UnsignedInt classIndex, UnsignedInt subClass);	// 0x0021A15D
	void *GetBlingBinder(UnsignedInt index);		// 0x0021A106: the binder record at index, or null
	Bool FindBlingByUpgradeName(const AsciiString &upgradeName, Int *outIndex, Int *outValue);	// 0x0021D4CE: the bling with this upgrade name; its index and value
	class CreateAHeroData *GetDefaultHero(Int faction);	// 0x0021A6C8: the first default hero whose class prefers this faction, or null
	void LoadMyHeroList();							// 0x0021F47E
	void UnbindHeroFromObjectAndUpdate(class Object *object);	// 0x00219757: grants the player the unbind upgrade for a Create-a-Hero object
	Rva0040A3F9 *rva0021F797();						// 0x0021F797: loads the heroes, then returns the list
	void rva0021A428(const CreateAHeroData &hero);	// 0x0021A428 (AptSkirmish::InitCreateAHeroOnStartGame 0x005218AF calls it for the host slot's hero)

	// One sub-class record of a create-a-hero class (0xD8 bytes, retail's element of the class's
	// sub-class array). Members are the ones the class getters read (0x00219B9E..0x00219D38).
	class CreateAHeroSubClass
	{
	public:
		AsciiString m_nameTag;				// +0x00 (GetSubClassNameTag of the class)
		AsciiString m_descTag;				// +0x04 (GetSubClassDescTag)
		AsciiString m_imageName;			// +0x08 (GetSubClassImageName)
		AsciiString m_buttonImageName;		// +0x0C (GetButtonImageName)
		Int m_10;							// +0x10 (rva00219CDF)
		Int m_14;							// +0x14 (rva00219CF6)
		Int m_18;							// +0x18 (rva00219D0D)
		Int m_spendableAttributePoints;		// +0x1C (GetSubClassSpendableAttributePoints)
		AsciiString m_subClassUpgradeName;	// +0x20 (GetSubClassUpgradeName)
		// Bling groups (retail's map at +0x24, 0xC bytes): its node count is at +0x28 (rva00219BCA reads it),
		// and a node's value holds the key at +0x10 and the group's vector at +0x14.
		BlingGroupMap m_blingGroups;		// +0x24 (rva0021BC0D, rva0021BC2C, GetBling)
		// STLport vectors: the getters read size() as (finish - start) and reload start for operator[].
		std::vector<Int> m_awards;			// +0x30 (GetAwardCount)
		std::vector<Int> m_stats;			// +0x3C (GetStatCount)
		std::map<Int, Int> m_defaultBlingIds;	// +0x48: bling key -> default bling id (GetDefaultBlingId); node count +0x4C
		std::map<Int, CreateAHeroAttribute> m_attributes;	// +0x54: attribute key -> record (rva0021BD22); node count +0x58 (rva00219D3B)
		unsigned char m_unk0x60[0x04];		// +0x60..+0x63
		Int m_preferedFaction;				// +0x64 (GetPreferedFaction)
		UnsignedInt m_factionMask;			// +0x68 the factions the sub-class allows, one bit each (allowsFaction 0x002196A9, GetFactionMaskType 0x00219C5D returns its address)
		unsigned char m_unk0x6C[0xD8 - 0x6C];	// +0x6C..+0xD7

		Int GetAwardNameKey(UnsignedInt index) const;	// 0x00219B62
		Int allowsFaction(UnsignedInt faction);	// 0x002196A9: whether the faction's bit is set in m_factionMask
		// The bling group of blingKey: the find result is stored in *out (retail's out slot), true when found.
		UnsignedByte rva0021BC0D(Int blingKey, void **out);	// 0x0021BC0D
		void *GetBling(Int blingKey, UnsignedInt index);	// 0x0021BCA7: the manager's bling record, or null
		UnsignedInt GetBlingIndex(Int blingKey, UnsignedInt index);	// 0x0021BCEA: the bling id at index, or 0
		const AsciiString *rva0021C970(Int blingKey, UnsignedInt index);	// 0x0021C970: the bling's desc tag, or TheEmptyString
		void *rva0021C9A6(Int blingKey, UnsignedInt index);	// 0x0021C9A6: the bling record, or TheEmptyString's address
		const AsciiString *rva0021C9D7(Int blingKey, UnsignedInt index);	// 0x0021C9D7: the bling's upgrade name, or TheEmptyString
		Int GetAttributeMinValue(UnsignedInt attribute) const;	// 0x0021BDAB
		Int GetAttributeMaxValue(UnsignedInt attribute) const;	// 0x0021BDD3
		Int GetAttributeDefaultValue(UnsignedInt attribute) const;	// 0x0021BDFB
		Int *rva0021BD22(Int attribute) const;	// 0x0021BD22: the attribute record (its three strings follow the key), or null
		Int GetBlingIndex(Int attribute, const AsciiString &value);	// 0x0021BD41
		Int rva0021BC2C(Int blingKey) const;	// 0x0021BC2C: count of the bling group
		Int GetDefaultBlingId(Int blingKey) const;	// 0x0021BC53
		void *GetBlingGroupNameKey(UnsignedInt index);	// 0x0021AD5D
		Int GetStatNameKey(UnsignedInt index) const;	// 0x00219B80
		~CreateAHeroSubClass();	// 0x0021E50F
	};
	typedef char CreateAHeroSubClassSize[sizeof(CreateAHeroSubClass) == 0xD8 ? 1 : -1];
	// One create-a-hero class: its sub-class array (0xD8-byte records) and the getters that read it.
	class CreateAHeroClass
	{
	public:
		const CreateAHeroSubClass *findSubClassByUpgradeName(const AsciiString &upgradeName);	// 0x0021AF1C: the last sub-class whose upgrade name equals this one (ignoring case), or null
		const CreateAHeroSubClass *rva00219B9E(UnsignedInt index) const;	// 0x00219B9E (private const): the sub-class at index, or null
		Int rva00219BCA(UnsignedInt index);							// 0x00219BCA
		Int GetStatCount(UnsignedInt index);						// 0x00219BE1
		Int GetAwardCount(UnsignedInt index);						// 0x00219C1F
		Int GetPreferedFaction(UnsignedInt index);					// 0x00219C93
		Int GetSubClassSpendableAttributePoints(UnsignedInt index);	// 0x00219D24
		const AsciiString &GetButtonImageName(UnsignedInt index);	// 0x00219CAB
		const AsciiString &GetSubClassUpgradeName(UnsignedInt index);	// 0x00219CC5
		const AsciiString &GetSubClassNameTag(UnsignedInt index);	// 0x0021ADF8
		const AsciiString &GetSubClassImageName(UnsignedInt index);	// 0x0021AE56
		const AsciiString &GetSubClassDescTag(UnsignedInt index);	// 0x0021AEB9
		Int rva00219CDF(UnsignedInt index);	// 0x00219CDF: sub-class +0x10 or 0
		Int rva00219CF6(UnsignedInt index);	// 0x00219CF6: sub-class +0x14 or 0
		Int rva00219D0D(UnsignedInt index);	// 0x00219D0D: sub-class +0x18 or 0
		Int rva00219D3B(UnsignedInt index);	// 0x00219D3B: sub-class +0x58 or 0
		Int GetAttributeMinValue(UnsignedInt attribute, UnsignedInt subClassIndex);	// 0x0021BE42
		Int GetAttributeMaxValue(UnsignedInt attribute, UnsignedInt subClassIndex);	// 0x0021BE62
		Int GetAttributeDefaultValue(UnsignedInt attribute, UnsignedInt subClassIndex);	// 0x0021BE82
		Int GetBlingCount(Int blingKey, UnsignedInt index) const;	// 0x0021BE23
		void *GetFactionMaskType(UnsignedInt index);	// 0x00219C5D: the sub-class's faction mask (+0x68)
		Int *rva0021BEC1(Int attribute, Int subClassIndex) const;	// 0x0021BEC1: the sub-class's attribute lookup, or null
		void *rva0021ADD9(UnsignedInt index, UnsignedInt subClassIndex);	// 0x0021ADD9: the sub-class's bling group name key, or null
		Int GetSubClassDefaultBlingId(Int blingKey, UnsignedInt subClassIndex) const;	// 0x0021BEA2
		Int GetStatNameKey(UnsignedInt index, UnsignedInt stat);	// 0x00219C00
		Int GetAwardNameKey(UnsignedInt index, UnsignedInt award);	// 0x00219C3E
		CreateAHeroClass(const AsciiString &upgradeName, const AsciiString &nameTag, const AsciiString &descTag, const AsciiString &powersTag, const AsciiString &imageName);	// 0x0021F804
		~CreateAHeroClass();	// 0x0021F8F3

		AsciiString m_nameTag;			// +0x00 (GetClassNameTag)
		AsciiString m_descTag;			// +0x04 (GetClassDescTag)
		AsciiString m_powersTag;		// +0x08 (GetClassPowersTag)
		AsciiString m_imageName;			// +0x0C (GetClassImageName)
		AsciiString m_upgradeName;			// +0x10 (GetClassUpgradeName)
		std::vector<CreateAHeroSubClass> m_subClasses;	// +0x14 (records are 0x20 bytes)
	};
	typedef char CreateAHeroClassSize[sizeof(CreateAHeroClass) == 0x20 ? 1 : -1];

	// Manager arrays: classes (0x20-byte records), bling (0x10) and binders (0x14). Offsets from the
	// getters that index them (0x00219D85, 0x00219E74, 0x0021A106).
	// Vector-layout arrays the manager keeps: first element and one past the last.
	class ClassArray
	{
	public:
		CreateAHeroClass *first;
		CreateAHeroClass *last;
		UnsignedInt size() const { return (UnsignedInt)(last - first); }
	};
	class BlingArray
	{
	public:
		CreateAHeroBlingEntry *first;
		CreateAHeroBlingEntry *last;
		UnsignedInt size() const { return (UnsignedInt)(last - first); }
	};
	class BinderArray
	{
	public:
		CreateAHeroBinderRecord *first;
		CreateAHeroBinderRecord *last;
		UnsignedInt size() const { return (UnsignedInt)(last - first); }
	};

	unsigned char m_pad00[0x14C];		// +0x00..+0x14B, not yet recovered
	ClassArray m_classes;				// +0x14C (first, last)
	unsigned char m_unk0x154[0x08];		// +0x154..+0x15B
	BlingArray m_bling;				// +0x15C
	unsigned char m_unk0x164[0x04];		// +0x164..+0x167
	BinderArray m_binder;				// +0x168
	unsigned char m_unk0x170[0x04];		// +0x170..+0x173 (the binder vector's capacity end)
	Rva0040A3F9 m_heroList;				// +0x174, the heroes the manager knows (GetDefaultHero 0x0021A6C8, constructed by 0x0040A530)
	unsigned char m_unk0x184[0x190 - 0x184];	// +0x184..+0x18F, not yet recovered
	AsciiString m_unbindUpgradeName;		// +0x190, the upgrade UnbindHeroFromObjectAndUpdate (0x00219757) grants the player (destroyed by 0x0021FFEE)
	unsigned char m_unk0x194[0x1C0 - 0x194];	// +0x194..+0x1BF, not yet recovered
	Real m_rollChance;					// +0x1C0, percent
	unsigned char m_unk0x1C4[0x1DC - 0x1C4];	// +0x1C4..+0x1DB, not yet recovered
	AsciiString m_commandSetName;			// +0x1DC, the command set the required buttons come from
};
typedef char CreateAHeroManager_off_m_rollChance[(offsetof(CreateAHeroManager, m_rollChance) == 0x1C0) ? 1 : -1];

extern CreateAHeroManager *TheCreateAHeroManager;
// The out slot the bling lookups in rva0021C970's family store their find result to (retail's global
// at 0x00DFE354, named by the map as g_00DFE354).
extern void *g_00DFE354;

#endif
