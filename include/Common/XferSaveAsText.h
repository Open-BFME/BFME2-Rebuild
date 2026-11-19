#pragma once
// SIZE 0x18. Retail table RVA 0x0087B388 has Xfer's 39 slots.
// Constructor 0x0060DEB1 and Print 0x0060D5F2 establish label flag +4,
// File* +8 and vector<string> +0xC. Print indexes that stack with stride 12.
// Names/types: WorldBuilder xfer_debug.cpp and old XferSaveAsText.cpp;
// virtual names/order follow Xfer's hierarchy, not folded vtable target labels.
#include <string>
#include <vector>
#include <stddef.h>
#include "Common/Xfer.h"
class File;
class XferSaveAsText : public Xfer
{
public:
    XferSaveAsText();
    virtual ~XferSaveAsText(); // slot 0
    virtual bool IsLoading() const; // slot 1
    virtual bool IsStoring() const; // slot 2
    virtual bool IsCRC() const; // slot 3
    virtual bool IsLightCRC() const; // slot 4
    virtual int BeginBlock(const char *name); // slot 5
    virtual void EndBlock(); // slot 6
    virtual void SkipBlock(const char *name); // slot 7
    // slot 8: inherited SkipBadBlock
    virtual Xfer &XferRawBytes(void *data, unsigned int size); // slot 9
    // Overrides retain inherited slots. VC7.1 reverses introducing overloads.
    virtual Xfer &operator==(bool &value);
    virtual Xfer &operator==(char &value);
    virtual Xfer &operator==(unsigned char &value);
    virtual Xfer &operator==(short &value);
    virtual Xfer &operator==(unsigned short &value);
    virtual Xfer &operator==(int &value);
    virtual Xfer &operator==(unsigned int &value);
    virtual Xfer &operator==(__int64 &value);
    virtual Xfer &operator==(float &value);
    virtual Xfer &operator==(AsciiString &value);
    virtual Xfer &operator==(UnicodeString &value);
    virtual Xfer &operator==(PooledString &value);
    virtual Xfer &operator==(Coord3DBase &value);
    virtual Xfer &operator==(ICoord3D &value);
    virtual Xfer &operator==(Region3D &value);
    virtual Xfer &operator==(IRegion3D &value);
    virtual Xfer &operator==(Coord2D &value);
    virtual Xfer &operator==(ICoord2D &value);
    virtual Xfer &operator==(Region2D &value);
    virtual Xfer &operator==(IRegion2D &value);
    virtual Xfer &operator==(RealRange &value);
    virtual Xfer &operator==(RGBColor &value);
    virtual Xfer &operator==(RGBAColorReal &value);
    virtual Xfer &operator==(RGBAColorInt &value);
    virtual Xfer &operator==(Snapshot &value);
    virtual Xfer &operator==(const char *const &value);
    virtual Xfer &operator==(Version &value);
    virtual Xfer &XferEnum(const char *name, void *data, unsigned int size); // slot 37
    static void __cdecl Print(XferSaveAsText *self, const char *format, ...);
protected:
    virtual void XferData(unsigned int type, void *data, unsigned int size); // slot 38
private:
    bool m_pendingLabel; // 0x4
    File *m_file; // 0x8
    std::vector<std::string> m_blocks; // 0xC
};
typedef char XferSaveAsTextSize[(sizeof(XferSaveAsText) == 0x18) ? 1 : -1];
