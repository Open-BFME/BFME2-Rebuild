#pragma once
#include "Common/SubsystemInterface.h"
#include "Common/UnicodeString.h"
#include "Common/Vector.h"
// SIZE 0xC. Target ctor0x002E55FF calls SubsystemInterface then installs
// table RVA0x00804E90: inherited slots0..13 and eight pure slots14..21.
// Manager table0x00804FA8 identifies the fetch overloads and slots18..20.
// MSVC reverses introducing overload groups. Names: donor (ZH), except
// getTranslateMode is role-inferred from the +0x40 getter/setter pair.
class GameTextInterface : public SubsystemInterface {
public:
    // Manager ctor2E63AA directly calls SubsystemInterface; dtor2E66F2
    // restores this interface's table inline. Standalone copies exist.
    GameTextInterface() {}
    virtual ~GameTextInterface() {}
    virtual UnicodeString fetch(const char *,bool *exists=0)=0; // 15
    virtual UnicodeString fetch(const AsciiString &,bool *exists=0)=0; // 14
    virtual UnicodeString *fetchPtr(const char *,bool *exists=0)=0; // 17
    virtual UnicodeString *fetchPtr(const AsciiString &,bool *exists=0)=0; // 16
    virtual _STL::vector<AsciiString> &getStringsWithLabelPrefix(const AsciiString &)=0;
    virtual void initMapStringFile(const AsciiString &)=0;
    virtual void setTranslateMode(int)=0;
    virtual int getTranslateMode()=0;
};
typedef char GameTextInterfaceSizeCheck[sizeof(GameTextInterface)==0xC ? 1 : -1];
extern GameTextInterface *TheGameText;
GameTextInterface *CreateGameTextInterface();
