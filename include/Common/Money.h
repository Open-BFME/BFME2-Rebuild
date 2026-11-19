// Money.h: a player's currency balance. SIZE 0xC; Snapshot is its only base.
// TableBE1AA0 has deleting dtor, loadPostProcess, name1FD2AE (literal Money),
// xfer3B0E38 (uint+4). Ctors1FD293/1FD2B4 and Player+90 prove the extent.
// WB AD6820 asserts Money::withdraw in Common/RTS/Money.cpp; game3B0CB3
// and deposit3B0D7C use balance+4 and player index+8. Old names Rva003B0D7C,
// S3Money and Rva003B0E38 are partial views, not additional real classes.
#pragma once
#ifndef BFME2_MONEY_H
#define BFME2_MONEY_H
#include "Common/Snapshot.h"
class ScoreKeeper;
class Money : public Snapshot
{
public:
    Money():m_money(0),m_playerIndex(0) {}
    virtual ~Money() {}
    void deposit(int amount,ScoreKeeper *tracker,bool playSound);
    unsigned int withdraw(unsigned int amount,ScoreKeeper *tracker,bool playSound);
    unsigned int countMoney() const { return m_money; }
    void setPlayerIndex(int index) { m_playerIndex=index; }
protected:
    virtual void loadPostProcess() {}
    virtual const char *GetSnapshotName() const { return "Money"; }
    virtual void xfer(Xfer *);
public:
    unsigned int m_money; //04
    int m_playerIndex; //08
};
typedef char MoneySize[sizeof(Money)==0xC?1:-1];
#endif
