#pragma once
// The type id every AptValue stores in bits 25..31 of its flags word at +4.
// Evidence (retail RVAs): the protected AptValue constructor 0x006DCCC0
// installs it through the setter 0x006DBBC0, which asserts 0 < id < 0x2F;
// the getter 0x006DBB30 returns flags >> 25; the type predicates
// 0x006DBDE0..0x006DC770 compare the masked word with id << 25.
// Only ids a predicate or a constructor proves are named; the others exist
// (every id from 1 to 0x2E can occur) but nothing recovered so far names them.
// Enumerator names follow the predicate's name in the map (isArray, isDate, ...).
enum AptVirtualFunctionTable_Indices
{
    AptVFT_xxx = 0,              // retail assert text `n > AptVFT_xxx` names the zero enumerator
    AptVFT_String = 1,           // isString 0x006DC0D0 accepts 1 and 0x2A
    AptVFT_Register = 4,         // isRegister 0x006DC1C0
    AptVFT_Boolean = 5,          // isBoolean 0x006DC120; AptBoolean ctor 0x006D8860 passes 5
    AptVFT_Float = 6,            // isFloat 0x006DC210
    AptVFT_Integer = 7,          // isInteger 0x006DC170
    AptVFT_Lookup = 8,           // isLookup 0x006DC080
    AptVFT_NativeFunction = 9,   // isNativeFunction 0x006DC260
    AptVFT_CIHFirst = 12,        // isCIH 0x006DC580 accepts 0x0C..0x13
    AptVFT_CIHLast = 19,
    AptVFT_None = 11,            // 0x006DE390 builds the singleton of this type (AddRef/Release are no-ops); predicate 0x006DC300
    AptVFT_FrameStack = 20,      // AptFrameStack: constructors 0x006FBAA0/0x006FBB10 pass 0x14; predicate 0x006DC350
    AptVFT_Sound = 21,           // isSound 0x006DC3F0
    AptVFT_Array = 22,           // isArray 0x006DC3A0
    AptVFT_Math = 23,            // isMath 0x006DC4E0
    AptVFT_Key = 24,             // isKey 0x006DC440
    AptVFT_Global = 25,          // the built-in global object: constructor 0x006DE630 (flags 0x32000034), lookup AptGlobal::objectMemberLookup
    AptVFT_ScriptColour = 26,    // isScriptColour 0x006DC530
    AptVFT_Object = 27,          // isObject 0x006DC5E0
    AptVFT_Prototype = 28,       // isPrototype 0x006DC630; no refcount registration in 0x006DCCC0
    AptVFT_Date = 29,            // isDate 0x006DC680
    AptVFT_MovieClip = 30,       // isMovieClip 0x006DC720
    AptVFT_Mouse = 31,           // the Mouse object: constructor 0x006DE5A0 (flags 0x3E000034), lookup AptMouse::objectMemberLookup; predicate 0x006DC490
    AptVFT_XmlNode = 32,         // 0x006DBDE0
    AptVFT_Xml = 33,             // 0x006DBE50
    AptVFT_XmlAttributes = 34,   // 0x006DBEC0
    AptVFT_LoadVars = 35,        // 0x006DBF30
    AptVFT_TextFormat = 36,      // isTextFormat 0x006DC6D0
    AptVFT_Stage = 39,           // isStage 0x006DC770
    AptVFT_StringObject = 42,    // isString accepts it too
    AptVFT_ScriptFunction1 = 43, // AptScriptFunction1 ctor 0x00709CA0 passes 0x2B
    AptVFT_ScriptFunction2 = 44, // AptScriptFunction2 ctors 0x00709CD0/0x00709D50 pass 0x2C
    AptVFT_ScriptFunctionByteCodeBlock = 45, // isScriptFunction accepts 0x2B..0x2D
    AptVFT_NumVFTs = 47          // retail assert text: `n < AptVFT_NumVFTs`, `getVtblIndex() < AptVFT_NumVFTs`
};
