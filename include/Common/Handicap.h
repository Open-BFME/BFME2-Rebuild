// Handicap.h: a player's build-cost and build-time handicaps. SIZE 0x10.
// Player embeds one at +0x3C (Player ctor 0x002B0F3C builds it with 0x003B0E83).
#pragma once
#ifndef BFME2_HANDICAP_H
#define BFME2_HANDICAP_H
class ThingTemplate;
class Dict;
// Matched getHandicap 0x003B0EA2 and readFromDict 0x003B0EBD prove 2x2 floats.
// Labels follow the BFME1 donor and literal HANDICAP key names in retail.
class Handicap {
public:
    enum HandicapType { BUILDCOST, BUILDTIME };
    float getHandicap(HandicapType,const ThingTemplate *) const;
    void readFromDict(const Dict *);
private:
    enum ThingType { GENERIC, BUILDINGS };
    float m_handicaps[2][2];
};
// SIZE 0x10
typedef char HandicapSize[(sizeof(Handicap) == 0x10) ? 1 : -1];

#endif
