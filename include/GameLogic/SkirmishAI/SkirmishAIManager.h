#pragma once

#include "Common/GameType.h"
#include "Common/Snapshot.h"
#include "Common/SubsystemInterface.h"
#include "GameLogic/SkirmishAI/SkirmishAI.h"

class GateOpenBehaviorList;
class Object;
class Player;
class Team;
class TeamPrototype;

// The map still spells this class Rva002A8AB1Record.
class SkirmishAI;
typedef SkirmishAI Rva002A8AB1Record;

struct Rva002A8F24Record
{
    // ScoreKeeper::updateFrame (0x0039D1F1) adds this field to the
    // player's frame money after following the +0x0C pointer.
    struct ScoreMoneySource
    {
        unsigned char m_unk0x00[0x14];
        int m_moneyForScore;
    };

    unsigned char m_unk0x00[0xC];
    ScoreMoneySource *m_scoreMoneySource;
};

// Construction puts SubsystemInterface first and Snapshot at +0xC.
// GetSnapshotName returns the subsystem's class name.
class SkirmishAIManager : public SubsystemInterface, public Snapshot
{
public:
    SkirmishAIManager();
    virtual ~SkirmishAIManager();

    virtual void init() {}
    virtual void reset();
    virtual void update();

    void UnRegister(Object *object);
    void rva002A9365(Object *object);
    Rva002A8F24Record *rva002A8F24(Player *player);
    Rva002A8AB1Record *rva002A8AB1(Player *player);

    unsigned char m_unk0x010[0x840];
    float m_aiRevivalCostDiscountLimit;
    float m_aiRevivalTimeDiscountLimit;
    unsigned char m_unk0x858[0x8];
    // Player::setPlayerType also creates AI for non-computer skirmish players when set.
    bool m_forceSkirmishPlayerAI;
    unsigned char m_unk0x861[0x13];
    Real m_teamTimeUntilConsideredIdle;
    unsigned char m_unk0x878[0xC8];
    // Created by the manager and used by GateOpenAndCloseBehavior.
    GateOpenBehaviorList *m_gateList;

protected:
    virtual void loadPostProcess() {}
    virtual const char *GetSnapshotName() const;
    virtual void xfer(Xfer *xfer);
};

// GameEngine allocates this exact size before calling the constructor.
typedef char SkirmishAIManagerSizeCheck[sizeof(SkirmishAIManager) == 0x944 ? 1 : -1];

extern SkirmishAIManager *TheSkirmishAIManager;
