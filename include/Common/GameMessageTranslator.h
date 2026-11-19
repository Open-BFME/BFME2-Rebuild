#pragma once
#include "Common/MessageStream.h"
// Retail propagation calls the translator at slot 0; TranslatorData teardown
// destroys it through slot 1. Donor MessageStream.h names both operations.
enum GameMessageDisposition { KEEP_MESSAGE, DESTROY_MESSAGE };
class GameMessageTranslator {
public:
    virtual GameMessageDisposition translateGameMessage(const GameMessage *message) = 0;
    virtual ~GameMessageTranslator() {}
};
