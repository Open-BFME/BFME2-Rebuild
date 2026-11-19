#pragma once
#include <stddef.h>
#include "GameClient/GameText.h"
#include "Common/FileInfo.h"
// Native GameText arrays: eight-byte label/text values and eight-byte lookup
// pairs. Table loading points each lookup at its StringInfo and label.
struct StringInfo {
    AsciiString label;
    UnicodeString text;
};
struct StringLookUp {
    AsciiString *label;
    StringInfo *info;
};
// Missing-string entries are an eight-byte singly linked list (fetchPtr2E65B1).
struct NoString {
    NoString *next;
    UnicodeString text;
};
// Retail table destructor2E6158 frees the two arrays; ctor clears three words.
class GameTextTable {
public:
    GameTextTable() : m_count(0),m_info(0),m_lookup(0) {}
    ~GameTextTable();
    bool load(const char *,const char *,bool *);
    bool readSTR(const char *);
    bool readCSF(const char *);
    unsigned int m_count;
    StringInfo *m_info;
    StringLookUp *m_lookup;
};
// Allocation2E651F requests44. Ctor2E63AA and parser/reload bodies prove
// these member offsets. Table804FA8 adds deinit at slot22 to the interface.
class GameTextManager : public GameTextInterface {
public:
    GameTextManager();
    virtual ~GameTextManager();
    virtual void init();
    virtual Bool attemptReload(Bool *needsRestart); // SubsystemInterface slot 4; override 0x002E5D62 (map: vslot04)
    virtual void reset();
    virtual void update() {}
    virtual UnicodeString fetch(const char *,bool *);
    virtual UnicodeString fetch(const AsciiString &,bool *);
    virtual UnicodeString *fetchPtr(const char *,bool *);
    virtual UnicodeString *fetchPtr(const AsciiString &,bool *);
    virtual _STL::vector<AsciiString> &getStringsWithLabelPrefix(const AsciiString &);
    virtual void initMapStringFile(const AsciiString &);
    virtual void setTranslateMode(int);
    virtual int getTranslateMode() { return m_translateMode; }
    virtual void deinit();
    FileInfo m_fileInfo; // C
    bool m_usedCSF, m_initialized; // 1C/1D
    UnicodeString m_failed; // 20
    NoString *m_noStringList; // 24
    int m_unknown28;
    GameTextTable *m_textTable, *m_mapTable; // 2C/30
    _STL::vector<AsciiString> m_asciiStringVec; // 34
    int m_translateMode; // 40
};
typedef char GameTextManagerSize[sizeof(GameTextManager)==0x44 ? 1 : -1];
typedef char GameTextManagerModeOffset[offsetof(GameTextManager,m_translateMode)==0x40 ? 1 : -1];
