#pragma once
#include "Common/AsciiString.h"
// Native ctor6024FD/dtor6025CE: no base; name4, access8, openC,
// delete-on-closeD, mutex10; MemoryReadFile starts its fields at14.
// Table87A808 has17 slots. Pure-slot signatures are corroborated by the
// matched File subclasses; names follow the ZH File API.
class File {
public:
    enum access { NONE=0, READ=1, WRITE=2, APPEND=4, CREATE=8,
        TRUNCATE=0x10, TEXT=0x20, BINARY=0x40, READWRITE=3,
        ONLYNEW=0x80, STREAMING=0x100 };
    enum seekMode { START, CURRENT, END };
    File();
    virtual ~File();
    virtual bool open(const char *, int = 0);
    virtual void close();
    virtual int read(void *, int) = 0;
    virtual int write(const void *, int) = 0;
    virtual int seek(int, seekMode = CURRENT) = 0;
    virtual void nextLine(char * = 0, int = 0) = 0;
    virtual bool scanInt(int &) = 0;
    virtual bool scanReal(float &) = 0;
    virtual bool scanString(AsciiString &) = 0;
    virtual bool __cdecl print(const char *, ...);
    virtual int size();
    virtual int position();
    virtual char *readEntireAndClose() = 0;
    virtual File *convertToRAMFile() = 0;
    virtual void lock();
    virtual void unlock();
    bool eof();
    const char *getName() const { return m_name.str(); }
    void setName(const char *name) { m_name.set(name); }	// FileSystem::openFile 0x00600C34: the language-path open renames the file to the requested path (StringBase<char>::set on +4)
    int getAccess() const { return m_access; }
    void deleteOnClose() { m_deleteOnClose = true; }
protected:
    AsciiString m_name;
    int m_access;
    bool m_open, m_deleteOnClose;
    void *m_mutex;
};
typedef char FileSize[sizeof(File)==0x14 ? 1 : -1];

File *createMemoryReadFile(char *buffer, int size);	// 0x006022CF
