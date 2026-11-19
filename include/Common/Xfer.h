// Xfer.h: SAGE save/load/CRC transfer interface. SIZE 0x4 (vtable only in this view).
// Target facts: retail spells the typed transfers as operator== overloads. Xfer's own
// table is RVA 0x007BB910, 39 slots, 0x9C bytes (the float constant 0xBFC00000 follows
// it); XferLoad's is 0x0087AF18. Slots 10..36 hold, in this order: Version, const char *
// const (pure), Snapshot, RGBAColorInt, RGBAColorReal, RGBColor, RealRange, IRegion2D,
// Region2D, ICoord2D, Coord2D, IRegion3D, Region3D, ICoord3D, Coord3DBase, PooledString,
// UnicodeString, AsciiString, float, __int64, unsigned int, int, unsigned short, short,
// unsigned char, char, bool. Each body pushes its tag ("real", "i64", "uint", "int",
// "usht", "shrt", "ubyt", "byte", "bool") and dispatches through XferData at [eax+0x98]
// (slot 38, 0x0060BBD5 is XferEnum at slot 37).
// The declarations below are deliberately the REVERSE of that table order: VC7.1 lays
// out same-name overloads in reverse declaration order. Verified on the retail image:
// this order emits Version at vtable 0x28, int at 0x7C and bool at 0x90, which is what
// retail's own call sites use (FloodUpdate::xfer 0x0048E4E2). Do not reorder them to
// follow the table: that compiles and calls the wrong slots.
// Version1 is 0x000053EE. Mangled names pin the value types: RealRange, RGBColor,
// RGBAColorReal, RGBAColorInt and the Coord/Region structs are structs (AAU), while
// Coord2D, Snapshot and the three string classes are classes (the map's slot-20
// name is ??8Xfer@@UAEAAV0@AAVCoord2D@@@Z).
// Confirmed slot by slot against table 0x007BB910 (k01, round 6): 0 deleting
// dtor 0x00005422; 1-4 the shared `xor al,al; ret` 0x0047A699; 5-7 __purecall;
// 8 SkipBadBlock 0x0050B238; 9 XferRawBytes 0x0060B886; 10 Version 0x0060B8D9;
// 11 __purecall; 12-36 the typed transfers in the order above; 37 XferEnum
// 0x0060BBD5; 38 __purecall (XferData).
#pragma once
#ifndef BFME2_XFER_H
#define BFME2_XFER_H

#include "Common/GameType.h"

class AsciiString;
class UnicodeString;
class PooledString;
class Snapshot;
struct Coord3DBase;
struct ICoord3D;
struct Region3D;
struct IRegion3D;
class Coord2D;
struct ICoord2D;
struct Region2D;
struct IRegion2D;
struct RealRange;
struct RGBColor;
struct RGBAColorReal;
struct RGBAColorInt;

class Xfer
{
public:
	// The save-game version pair; *xfer == version transfers it.
	class Version
	{
	public:
		Version(UnsignedByte current, UnsignedByte minimum) : m_current(current), m_minimum(minimum) {}
		UnsignedByte m_current;	// 0x0
		UnsignedByte m_minimum;	// 0x1
	};

	Xfer() {}
	virtual ~Xfer() {}										// slot 0

	void Version1();

	virtual Bool IsLoading() const;							// slot 1
	virtual Bool IsStoring() const;							// slot 2
	virtual Bool IsCRC() const;								// slot 3
	virtual Bool IsLightCRC() const;						// slot 4

	virtual Int BeginBlock(const char *name) = 0;			// slot 5
	virtual void EndBlock() = 0;							// slot 6
	virtual void SkipBlock(const char *name) = 0;			// slot 7

	virtual void SkipBadBlock(Snapshot &snapshot, UnsignedInt size);	// slot 8
	virtual Xfer &XferRawBytes(void *data, UnsignedInt size);		// slot 9
	virtual Xfer &operator==(bool &value);					// table slot 36
	virtual Xfer &operator==(char &value);					// table slot 35
	virtual Xfer &operator==(unsigned char &value);			// table slot 34
	virtual Xfer &operator==(short &value);					// table slot 33
	virtual Xfer &operator==(unsigned short &value);		// table slot 32
	virtual Xfer &operator==(int &value);					// table slot 31
	virtual Xfer &operator==(unsigned int &value);			// table slot 30
	virtual Xfer &operator==(__int64 &value);				// table slot 29
	virtual Xfer &operator==(float &value);					// table slot 28
	virtual Xfer &operator==(AsciiString &value);			// table slot 27
	virtual Xfer &operator==(UnicodeString &value);			// table slot 26
	virtual Xfer &operator==(PooledString &value);			// table slot 25
	virtual Xfer &operator==(Coord3DBase &value);			// table slot 24
	virtual Xfer &operator==(ICoord3D &value);				// table slot 23
	virtual Xfer &operator==(Region3D &value);				// table slot 22
	virtual Xfer &operator==(IRegion3D &value);				// table slot 21
	virtual Xfer &operator==(Coord2D &value);				// table slot 20
	virtual Xfer &operator==(ICoord2D &value);				// table slot 19
	virtual Xfer &operator==(Region2D &value);				// table slot 18
	virtual Xfer &operator==(IRegion2D &value);				// table slot 17
	virtual Xfer &operator==(RealRange &value);				// table slot 16
	virtual Xfer &operator==(RGBColor &value);				// table slot 15
	virtual Xfer &operator==(RGBAColorReal &value);			// table slot 14
	virtual Xfer &operator==(RGBAColorInt &value);			// table slot 13
	virtual Xfer &operator==(Snapshot &value);				// table slot 12
	virtual Xfer &operator==(const char *const &value) = 0;	// table slot 11
	virtual Xfer &operator==(Version &value);				// table slot 10

	virtual Xfer &XferEnum(const char *name, void *data, UnsignedInt size);	// slot 37

protected:
	virtual void XferData(UnsignedInt type, void *data, UnsignedInt size) = 0;	// slot 38
};

// Free helper (old repo XferObjectID): transfers an ObjectID through the int overload.
Xfer &XferObjectID(Xfer *xfer, ObjectID *objectID);
// Free helper 0x00305C32: transfers a Relationship held in an int (PlayerRelationMap::xfer 0x002ADEBE and TeamRelationMap::xfer 0x003A2AA6 call it for each entry).
void XferRelationship(Xfer *xfer, Int *relationship);

// ID helpers: the particle-system transfer is 0x0030600A and the drawable one
// 0x003060CA; both take the Xfer first and the ID's storage second.
void XferParticleSystemID(Xfer *xfer, int *value);
void XferDrawableID(Xfer *xfer, int *value);

// Labelled-enum helpers ActiveBody::xfer (0x004BF1E8) calls: the label strings are "BodyDamageType",
// "BodySideDestroyedType" and the DamageFXType one; each is a 24-byte XferEnum(label, value, 4).
void XferBodyDamageType(Xfer *xfer, int *value);		// 0x00305E12
void XferBodySideDestroyedType(Xfer *xfer, int *value);	// 0x00305E2A
void XferDamageFXType(Xfer *xfer, int *value);			// 0x00305EBA

#endif
