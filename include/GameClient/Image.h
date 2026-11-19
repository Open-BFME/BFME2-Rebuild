// Portions derived from Command & Conquer Generals Zero Hour, (c) Electronic Arts Inc., GPL-3.0-or-later; see LICENSE and NOTICE.
#pragma once
#include <stddef.h>
#include <map>
#include "Common/AsciiString.h"
#include "Common/PixelRegion.h"
#include "Common/Region2D.h"
#include "Common/SubsystemInterface.h"

// The Status bit names (retail VA 0x00DBCEDC, in Image's object). Zero Hour's name and guard.
#ifdef DEFINE_IMAGE_STATUS_NAMES
static const char *imageStatusNames[] =
{
	"ROTATED_90_CLOCKWISE", "RAW_TEXTURE",
	NULL
};
#endif
class INI;
class TextureClass;
struct FieldParse;
extern const FieldParse m_imageFieldParseTable[];
// Target ctor2D8FE4, allocations4E8C3/57CE45: size34, no observed base.
// Table8037B8 contains only the deleting destructor2D9262. INI table8036E8
// proves filename, dimensions, UVs and status. Names: donor (ZH).
class Image {
public:
    Image();
    virtual ~Image();
    void setName(const AsciiString &name) { m_name=name; }
    const AsciiString &getName() const { return m_name; }
    void setFilename(const AsciiString &name) { m_filename=name; }
    const AsciiString &getFilename() const { return m_filename; }
    void setUV(Region2D *uv) { if (uv) m_UVCoords=*uv; }
    const Region2D *getUV() const { return &m_UVCoords; }
    void setTextureWidth(int width) { m_textureSize.x=width; }
    void setTextureHeight(int height) { m_textureSize.y=height; }
    const ICoord2D *getTextureSize() const { return &m_textureSize; }
    void setImageSize(ICoord2D *size) { if (size) m_imageSize = *size; }
    const ICoord2D *getImageSize() const { return &m_imageSize; }
    int getImageWidth() const { return m_imageSize.x; }
    int getImageHeight() const { return m_imageSize.y; }
    unsigned int setStatus(unsigned int);
    unsigned int clearStatus(unsigned int);
    unsigned int getStatus() const { return m_status; }
    const FieldParse *getFieldParse() const { return m_imageFieldParseTable; }
    static void parseImageCoords(INI *,void *,void *,const void *);
    static void parseImageStatus(INI *,void *,void *,const void *);
    AsciiString m_name, m_filename; // 4/8
    ICoord2D m_textureSize; // C
    Region2D m_UVCoords; // 14
    ICoord2D m_imageSize; // 24
    // 2C: the ctor stores 0; the destructor (0x002D91CC) takes the pointer, and if it is
    // set, releases the TextureClass pointer held in the first word of the 4-byte block
    // (call 0x0061ED10, the ICF-folded TextureBaseClass/TextureClass::Release_Ref) and frees
    // the block. So the Image owns a 4-byte holder of one TextureClass pointer.
    TextureClass **m_texture;
    unsigned int m_status; // 30
};
typedef char ImageSizeCheck[sizeof(Image)==0x34 ? 1 : -1];
typedef char ImageUVOffset[offsetof(Image,m_UVCoords)==0x14 ? 1 : -1];
// TheMappedImageCollection is an ImageCollection. Target ctor2D932B builds
// SubsystemInterface and STLport map+C; allocation23A2D9 requests18.
// Table803878 has14 slots, including empty init/reset/update overrides.
class ImageCollection : public SubsystemInterface {
public:
    ImageCollection();
    virtual ~ImageCollection();
    virtual void init() {}
    virtual void reset() {}
    virtual void update() {}
    void load(int textureSize);
    const Image *findImageByName(const AsciiString &);
    void addImage(Image *);
    _STL::map<unsigned int,Image *> m_imageMap;
};
typedef char ImageCollectionSizeCheck[sizeof(ImageCollection)==0x18 ? 1 : -1];
extern ImageCollection *TheMappedImageCollection;
