#pragma once
#include "Common/AsciiString.h"
#include "Common/RGBColor.h"
#include "Common/AudioEventRef.h"
class INI;
struct FieldParse;
enum BridgeTowerType;
// Target newRoad2DB57F/newBridge2DB64E allocate14C; ctor2DB1CA and dtor2DB311
// establish no base and the sole destructor slot in table803E24. Field tables
// 8039E8/803A28 supply the named offsets; ZH TerrainRoads.h supplies names.
// Sound handles are the counted AudioEventInfoRef, not an unrelated opaque
// word: ctor51914/dtor10F149 and setter2DB44F match the shared handle ABI.
class TerrainRoadType {
public:
    TerrainRoadType();
    virtual ~TerrainRoadType();
    const AsciiString &getName() const { return m_name; }
    bool isBridge() const { return m_isBridge; }
    const AsciiString &getTexture() const { return m_texture; }
    float getRoadWidth() { return m_roadWidth; }
    float getRoadWidthInTexture() { return m_roadWidthInTexture; }
    float getBridgeScale() const { return m_bridgeScale; }
    RGBColor getRadarColor() { return m_radarColor; }
    AsciiString getBridgeModel();
    AsciiString getBridgeModelNameDamaged();
    AsciiString getBridgeModelNameBroken();
    AsciiString getTextureReallyDamaged();
    AsciiString getTowerObjectName(BridgeTowerType);
    const AudioEventInfoRef &getDamageToSoundString(BodyDamageType state) const { return m_damageToSoundString[state]; }
    const AudioEventInfoRef &getRepairedToSoundString(BodyDamageType state) const { return m_repairedToSoundString[state]; }
    AsciiString getDamageToOCLString(BodyDamageType,int);
    AsciiString getDamageToFXString(BodyDamageType,int);	// 0x00456725 (BridgeBehavior.cpp)
    AsciiString getRepairedToOCLString(BodyDamageType,int);	// 0x0045674D
    AsciiString getRepairedToFXString(BodyDamageType,int);	// 0x00456774
    float getTransitionEffectsHeight() const { return m_transitionEffectsHeight; }
    int getNumFXPerType() const { return m_numFXPerType; }
    TerrainRoadType *friend_getNext() { return m_next; }
    void friend_setNext(TerrainRoadType *next) { m_next=next; }
    void friend_setID(unsigned int id) { m_id=id; }
    void friend_setBridge(bool isBridge) { m_isBridge=isBridge; }
    void friend_setRoadWidth(float width) { m_roadWidth=width; }
    void friend_setRoadWidthInTexture(float width) { m_roadWidthInTexture=width; }
    void friend_setName(AsciiString);
    void friend_setTexture(AsciiString);
    void friend_setBridgeModelName(AsciiString);
    void friend_setBridgeModelNameDamaged(AsciiString);
    void friend_setBridgeModelNameReallyDamaged(AsciiString);
    void friend_setBridgeModelNameBroken(AsciiString);
    void friend_setTextureDamaged(AsciiString);
    void friend_setTextureReallyDamaged(AsciiString);
    void friend_setTextureBroken(AsciiString);
    void friend_setDamageToSoundString(BodyDamageType,const AudioEventInfoRef &);
    void friend_setRepairedToSoundString(BodyDamageType,const AudioEventInfoRef &);
    void friend_setDamageToOCLString(BodyDamageType,int,AsciiString);
    void friend_setDamageToFXString(BodyDamageType,int,AsciiString);
    void friend_setRepairedToOCLString(BodyDamageType,int,AsciiString);
    void friend_setRepairedToFXString(BodyDamageType,int,AsciiString);
    static const FieldParse m_terrainRoadFieldParseTable[];
    static const FieldParse m_terrainBridgeFieldParseTable[];
    static void parseTransitionToOCL(INI *,void *,void *,const void *);
    static void parseTransitionToFX(INI *,void *,void *,const void *);
    AsciiString m_name; //4
    bool m_isBridge; //8
    unsigned int m_id; //C
    TerrainRoadType *m_next; //10
    float m_roadWidth,m_roadWidthInTexture,m_bridgeScale; //14..1C
    AsciiString m_scaffoldObjectName,m_scaffoldSupportObjectName; //20/24
    RGBColor m_radarColor; //28
    AsciiString m_bridgeModelName,m_texture; //34/38
    AsciiString m_bridgeModelNameDamaged,m_textureDamaged; //3C/40
    AsciiString m_bridgeModelNameReallyDamaged,m_textureReallyDamaged; //44/48
    AsciiString m_bridgeModelNameBroken,m_textureBroken; //4C/50
    AsciiString m_towerObjectName[4]; //54
    AudioEventInfoRef m_damageToSoundString[4]; //64
    AsciiString m_damageToOCLString[4][3],m_damageToFXString[4][3]; //74/A4
    AudioEventInfoRef m_repairedToSoundString[4]; //D4
    AsciiString m_repairedToOCLString[4][3],m_repairedToFXString[4][3]; //E4/114
    float m_transitionEffectsHeight; //144
    int m_numFXPerType; //148
};
typedef char TerrainRoadTypeSizeCheck[sizeof(TerrainRoadType)==0x14C ? 1 : -1];