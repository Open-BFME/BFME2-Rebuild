#pragma once
#include "Common/GameType.h"
#include "Common/Coord3D.h"
#include "Common/PixelRegion.h"
// Retail appends at 0x0030F949..0x0030FA44 prove payload at +8,
// type at +0x18 and the tags below. allocArg proves total size 0x1C.
// location is Coord3D's three-float base (k02): Coord3D has user-declared constructors,
// which a union member may not have. Readers build a Coord3D from it through the inline
// ??0Coord3D@@QAE@ABUCoord3DBase@@@Z (the old decomp's GameLogicDispatch and CommandXlat
// bodies copy it member by member from &arg->location).
union GameMessageArgumentType {
 int integer; float real; bool boolean; ObjectID objectID; DrawableID drawableID;
 unsigned int teamID; Coord3DBase location; ICoord2D pixel; IRegion2D pixelRegion;
 unsigned int timestamp; unsigned short wChar;
};
enum GameMessageArgumentDataType {
 ARGUMENTDATATYPE_INTEGER, ARGUMENTDATATYPE_REAL, ARGUMENTDATATYPE_BOOLEAN,
 ARGUMENTDATATYPE_OBJECTID, ARGUMENTDATATYPE_DRAWABLEID, ARGUMENTDATATYPE_TEAMID,
 ARGUMENTDATATYPE_LOCATION, ARGUMENTDATATYPE_PIXEL, ARGUMENTDATATYPE_PIXELREGION,
 ARGUMENTDATATYPE_TIMESTAMP, ARGUMENTDATATYPE_WIDECHAR, ARGUMENTDATATYPE_UNKNOWN
};
class GameMessageArgument {
public:
 virtual ~GameMessageArgument();
 GameMessageArgument *m_next;
 GameMessageArgumentType m_data;
 GameMessageArgumentDataType m_type;
};
