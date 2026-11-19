#pragma once
#include "Common/GameMessageTranslator.h"
// Target table7ED668: message translator followed by deleting destructor.
// No added storage; GameClient creates it and attaches at priority100.
class HintSpyTranslator : public GameMessageTranslator {
public:
    virtual GameMessageDisposition translateGameMessage(const GameMessage *);
};
