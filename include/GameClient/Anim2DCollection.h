#pragma once
#include "Common/SubsystemInterface.h"
class AsciiString;
class Anim2D;
class Anim2DTemplate;
// Native ctor2D6C02 calls SubsystemInterface and clears template listC and
// instance list10. GameClient::init allocates14; tableC0331C has14 slots.
// ZH supplies API names; native list lookup2D752D and next2D6C79 confirm
// the template list. No separate allocation base is evidenced.
class Anim2DCollection : public SubsystemInterface {
public:
    Anim2DCollection();
    virtual ~Anim2DCollection();
    virtual void init();
    virtual void reset() {}
    virtual void update();
    Anim2DTemplate *findTemplate(const AsciiString &);
    Anim2DTemplate *newTemplate(const AsciiString &);
    Anim2DTemplate *getNextTemplate(Anim2DTemplate *) const;
    void registerAnimation(Anim2D *);
    void unRegisterAnimation(Anim2D *);
private:
    Anim2DTemplate *m_templateList;
    Anim2D *m_instanceList;
};
typedef char Anim2DCollectionSize[sizeof(Anim2DCollection)==0x14 ? 1 : -1];
extern Anim2DCollection *TheAnim2DCollection;
