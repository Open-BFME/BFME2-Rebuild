// SIZE 0x8. No base. Retail constructors 0x006DCCC0/0x006DCD20 install
// VA 0x00CEAED0 at +0 and initialize the flags at +4. Scalar subclasses
// start payload at +8 (0x006D84C0/0x006D86A0/0x006D8860).
// Retail's 15-slot table starts with AddRef, Release, ForceDelete; its deleting
// destructor is LAST. CleanAll's virtual calls establish slots 9/10/11.
// Pure-slot names 12/13 have APT PDB name evidence and named retail overrides.
// Old AptValue.h records the table targets; no folded target label is used
// to rename an unrelated virtual. All methods remain out of line.
#pragma once

class EAStringC;
class AptArray;
class AptCIH;
class AptDate;
class AptInteger;
class AptLookup;
class AptNativeFunction;
class AptPrototype;
class AptRegister;
class AptString;
struct AptNativeHash;
#include "Apt/AptVirtualFunctionTable.h"

class AptValue
{
public:
    virtual void AddRef();
    virtual void Release();
    virtual void ForceDelete();
    virtual AptNativeHash *GetNativeHashVirtual();
    virtual bool ContainsNativeHashVirtual() const;
    virtual int getHasClass() const;
    virtual void setHasClass(int);
    virtual AptValue *objectMemberLookup(AptValue *const, const EAStringC *const) const;
    virtual bool objectMemberSet(AptValue *const, const EAStringC *const, AptValue *const);
    virtual void DeleteThis();
    virtual void PreDestroy();
    virtual void DestroyGCPointers();
    virtual bool IsGarbageCollected() const = 0;
    virtual void RegisterReferences() const = 0;

    unsigned int getRefCount() const;
    bool getIsDefined() const;
    void setIsDefined(bool);
    bool getGCMark() const; // 0x006DBB40: bit 1 of the flags word (name is this header's)
    void setGCMark(bool);
    int IsReleaseAtEnd() const;
    void ClearReleaseAtEnd();
    void SetReleaseAtEnd(); // 0x006DBDB0: or [this+4], 4 (the map names it apply@Rva006DBDB0DwordOrSetter)
    AptVirtualFunctionTable_Indices getVtblIndex() const;

    bool isUndefined() const;
    bool isNone() const;
    bool isLookup() const;
    bool isString() const;
    bool isBoolean() const;
    bool isInteger() const;
    bool isRegister() const;
    bool isFloat() const;
    bool isNativeFunction() const;
    bool isScriptFunction() const;
    bool isExtern() const;
    bool isArray() const;
    bool isCIH(bool = false) const;
    // 0x006DCA60 (the map files it as rva006DCA60): a type switch that is true for the string, native function, sound, array, script
    // colour, object, date, movie clip, XML, LoadVars, TextFormat, string-object and script-function type ids. The name is this header's.
    bool canHaveMembers() const;
    bool isObject() const;
    bool isPrototype() const;

    AptLookup *c_lookup() const;
    AptInteger *c_integer() const;
    AptRegister *c_register() const;
    AptString *c_string() const;
    AptNativeFunction *c_nativefunction() const;
    AptCIH *c_cih(bool = false);
    AptCIH *c_cih(bool = false) const;
    AptArray *c_array() const;
    AptPrototype *c_prototype() const;
    AptDate *c_date() const;

    // Rows the map still files under the AptValue placeholder (BfmeAptValue006DCD20); their names and
    // signatures are the map's mangled ones. The type predicates test the id in the flags word against
    // one constant (see AptVirtualFunctionTable.h); the checked casts (0x006DCD50..0x006DD320, 50 bytes
    // each, in type-id order) assert the predicate and return `this`.
    bool isXmlNode() const;        // 0x006DBDE0
    bool isXml() const;            // 0x006DBE50
    bool isXmlAttributes() const;  // 0x006DBEC0
    bool isLoadVars() const;       // 0x006DBF30
    bool isSound() const;          // 0x006DC3F0
    bool isKey() const;            // 0x006DC440
    bool isMath() const;           // 0x006DC4E0
    bool isScriptColour() const;   // 0x006DC530
    bool isDate() const;           // 0x006DC680
    bool isTextFormat() const;     // 0x006DC6D0
    bool isMovieClip() const;      // 0x006DC720
    bool isStage() const;          // 0x006DC770
    bool isCharacterInst() const; // 0x006E0200 (the map says int, but its caller 0x006E44F0 tests al: bool)
    bool isLevelInst() const;     // 0x006E03A0 (the donor calls it IsLevelInst)
    // 0x006E01A0 and 0x006CBEE0 (the map files both under placeholder names): the sprite-instance and animation-instance tests TypeOf uses on a CIH.
    int rva006E01A0(bool bUndefOK) const;
    int rva006CBEE0(bool bUndefOK) const;
    int rva006DCC60(bool bUndefOK) const; // 0x006DCC60 (the map files it under the placeholder name): the button-instance test, true for type 0xE (AptCIH.h line 0xb5 asserts this)
    bool rva006D9780(AptValue *pContext, EAStringC *pKey, AptValue *pValue); // 0x006D9780 (the map files it under the placeholder name): stores pValue at the array index a numeric key names
    // 0x006E02B0: asserts this != NULL (AptCIH.h line 0xc4), then true for a defined value of type 0xF.
    // (isSpriteInstBase 0x006CFCD0 is AptCIH's.)
    bool rva006E02B0() const;
    // 0x006E0260 / 0x006E0300 (the map files them as rva006E0260 / rva006E0300 on AptValue; they return int, retail tests the low byte): the two type
    // predicates rva006E24E0 tries after the sprite-inst-base and text tests.
    int rva006E0260() const;
    int rva006E0300() const;
    // 0x006E04A0 / 0x006E0510 / 0x006E0F40 (the map files them under AptValue; they read the instance pointer of an AptCIH at +0x4C,
    // so a cast to AptCIH is the way in): the member hash of a type 0x13 instance's character instance (null for a defined type 0x13
    // value or a missing instance); whether that hash exists; the type 0xF (text) instance pointer.
    void *rva006E04A0() const;
    int rva006E0350() const; // 0x006E0350 (the map name; O006E01A0.cpp): a defined value of type 0x11 (AptCIH.h line 0xce asserts this != NULL)
    int refreshEventHandlerMask(); // 0x006E0AC0 (the map files it as rva006E0AC0): sets the hash's handler bits for every event name the value defines a child for; returns whether a bit outside the mouse-move group was set
    int rva006E0510() const;
    void *rva006E0F40() const;
    // 0x006E1540 (the map files it as rva006E1540 on AptValue; the value is a CIH): a sprite releases its display list's contents
    // (0x006F76B0); a text instance gives its text buffer back to the host unless it is the shared empty buffer.
    void clearInstanceContents();
    unsigned int getGCRootCount() const;   // 0x006DBB50
    void setGCRootCount(unsigned int);     // 0x006DBC70, asserts (nRoot >= 0) && (nRoot <= MAX_GCROOT)
    void incrementGCRootCount();           // 0x006DBCC0
    void decrementGCRootCount();           // 0x006DBD30
    AptValue *checkedLookup();    // 0x006DCD50
    AptValue *checkedInteger();   // 0x006DCD90
    AptValue *checkedRegister();  // 0x006DCDD0
    AptValue *checkedFloat();     // 0x006DCE10
    AptValue *checkedString();    // 0x006DCE50
    AptValue *rva006DCEA0();      // 0x006DCEA0 (the map files it as rva006DCEA0 on AptValue): a checked conversion like checkedFloat/checkedString; MovieClip.startDrag passes its lock-centre argument through it before toInteger
    AptValue *rva006DD0A0();      // 0x006DD0A0: the checked cast to AptScriptColour (asserts isScriptColour(), AptValue.inl line 0x3fe)
    AptValue *checkedBoolean();
    AptValue *checkedScriptFunction();
    AptValue *checkedNativeFunction();
    AptValue *checkedArray();
    AptValue *checkedSound();
    AptValue *checkedKey();
    AptValue *checkedMath();
    AptValue *checkedScriptColour();
    AptValue *checkedObject();
    AptValue *checkedPrototype();
    AptValue *checkedDate();
    AptValue *checkedTextFormat();
    AptValue *checkedMovieClip();
    AptValue *checkedXmlNode();
    AptValue *checkedXml();
    AptValue *checkedXmlAttributes();
    AptValue *checkedLoadVars();
    AptValue *checkedStage();
    AptValue *checkedCIH(bool bUndefOK);       // 0x006DCF60 (the map still files it as rva006DCF60)
    bool isType3() const;           // 0x006DBFA0
    bool isFrameStack() const;      // 0x006DC350
    bool isMouse() const;           // 0x006DC490

    int toInteger() const;
    float toFloat();
    // 0x006DDF60 (the map name): the url-encoded form of the value's hash (the loadVars / send form): `name=value&` for every entry whose name starts with an underscore except
    // `__proto__` and `_type`, skipping native functions, less the last `&`; empty when the value is not defined or has no hash.
    EAStringC rva006DDF60();
    int inheritsObjectPrototype(); // 0x006DC9E0 (the map files it as rva006DC9E0): whether the value's prototype chain reaches Object.prototype before Function.prototype
    // Retail's AptValue.inl declares two checked casts that no code calls, so retail never emitted them: the assert line numbers of the emitted casts jump from checkedKey (0x395)
    // to checkedMath (0x3E3), 78 lines, where every other cast takes 25 or 26 (two missing bodies of 26 lines), and the assert strings are pooled with no gap between "isKey()" and
    // "isMath()". The first is the Mouse cast: isMouse 0x006DC490 sits between isKey and isMath in the predicate order and AptVFT_Mouse exists. They are declared only (a body would
    // be an extra emitted symbol); AptScriptFunction2::SetupBeforeExecution 0x0070A040 loses its match by one lea operand order unless the class declares them.
    AptValue *checkedMouse();
    AptValue *rva006DD1A0(); // 0x006DD1A0: the map's spelling of checkedTextFormat (declared above); still stands in for the second unemitted cast (its type is not proven)
    bool toBool() const;
    void SetString(const char *);
    void rva006DE870(char *buffer) const; // 0x006DE870: formats the value as text into buffer (map name kept)
    void toString(EAStringC &) const;
    // 0x006DDDE0 (the map files it as rva006ddde0): the object's variables as a URL-encoded query string, returned by value.
    EAStringC toObjectString();
    AptValue *findChild(const EAStringC *, AptValue *);

protected:
    AptValue(AptVirtualFunctionTable_Indices);
    AptValue(AptVirtualFunctionTable_Indices, unsigned int); // 0x006DCD20: the second argument is not used
    virtual ~AptValue();
    // The flags word at +4, as bitfields. The retail assert text in the Apt pool allocator
    // (`pValue->mValueBitfield.mbIsAllocated == false`) names the member and bit 0; the other
    // widths are fixed by the accessors 0x006DBB20..0x006DBDD0 and the xor/and/xor stores in
    // AddRef 0x006DCAC0 and setGCRootCount 0x006DBC70 (their names here are not evidenced).
    struct ValueBitfield
    {
        unsigned int mbIsAllocated : 1; // bit 0: the pool allocator's mark (0x006D29E0)
        unsigned int mGCMark : 1;       // bit 1: setGCMark 0x006DBC50, read at 0x006DBB40
        unsigned int mReleaseAtEnd : 1; // bit 2: IsReleaseAtEnd 0x006DBDD0, ClearReleaseAtEnd 0x006DBDC0
        unsigned int mDestroyedGC : 1;  // bit 3: SetDestroyedGC 0x006DBD80
        unsigned int mIsDefined : 1;    // bit 4: getIsDefined 0x006DBB60, setIsDefined 0x006DBC30
        unsigned int mFlag5 : 1;        // bit 5: set by the (type) constructor 0x006DCCC0, read by Release 0x006DCB20
        unsigned int mRefCount : 12;    // bits 6..17: getRefCount 0x006DBB20 (AddRef clamps at 0xFFF)
        unsigned int mGCRootCount : 7;  // bits 18..24: getGCRootCount 0x006DBB50, MAX_GCROOT 0x7F (assert text)
        AptVirtualFunctionTable_Indices mType : 7; // bits 25..31 (an enum bit-field, signed: the type accessors compile to sar): the AptVirtualFunctionTable_Indices id
    };
    ValueBitfield mValueBitfield;

private:
    friend class AptGC;
    friend class AptValueWithHash;
    friend class AptCIH;
    void SetDestroyedGC(); // 0x006DBD80, out of line (AptValue.cpp): AptValueWithHash's destructor expands it in place, AptCIH::DestroyGCPointers (0x006E0460) and the thunk 0x006CBD00 jump to it
    void setRefCount(unsigned int);
    void setVtblIndex(AptVirtualFunctionTable_Indices); // 0x006DBBC0, asserts 0 < n < AptVFT_NumVFTs
};

typedef char AptValueSize[(sizeof(AptValue) == 8) ? 1 : -1];
