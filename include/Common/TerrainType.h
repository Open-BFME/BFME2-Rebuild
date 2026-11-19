#pragma once
#include "Common/AsciiString.h"
struct FieldParse;
class TerrainTypeCollection;
// Target allocation is 0x30. Constructor 0x317F6F and collection traversal
// establish the name +4, texture +8 and intrusive next link +0x20.
// The two 12-byte container regions retain their unresolved member storage.
class TerrainType
{
    friend class TerrainTypeCollection;
public:
    TerrainType();
    virtual ~TerrainType();
    TerrainType &operator=(const TerrainType &that);
    void friend_setName(AsciiString name);
    static const FieldParse m_terrainTypeFieldParseTable[];
    const FieldParse *getFieldParse() const { return m_terrainTypeFieldParseTable; }
private:
    AsciiString m_name; // 0x04
    AsciiString m_texture; // 0x08
    bool m_flag0C;
    unsigned char m_unk0x0D[3];
    unsigned char m_unk0x10[12];
    bool m_flag1C;
    unsigned char m_unk0x1D[3];
    TerrainType *m_next; // 0x20
    unsigned char m_unk0x24[12];
};
typedef char check_TerrainType_size[sizeof(TerrainType) == 0x30 ? 1 : -1];
