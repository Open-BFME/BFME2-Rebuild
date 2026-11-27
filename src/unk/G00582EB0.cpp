// Generated from the ledger; map snapshot c591f7146f88c8a8248ebb8ae22f0c978c22aed463db64e05ef57ff0dfae99cc
// object=G00582EB0 name-evidence=invented
// Extents and edges are hypotheses; byte verification alone does not establish original TU boundaries.
#include "GameNetwork/LANAPI.h"
#include "GameNetwork/LANGameInfo.h"

AsciiString GenerateGameOptionsString();	// 0x00447C46

// FUNCTION: BFME2 0x00582EB0 size=273 name=?handleGameStart@LANAPI@@IAEXPAULANMessage@@PBUBfmeNetAddress@@_N@Z evidence=N:ledger tier=B hypothesis:mm1-gap-unit
void LANAPI::handleGameStart(LANMessage *message, const BfmeNetAddress *address, Bool wotr)
{
	if (!m_inLobby)
	{
		LANGameInfo *game = m_currentGame;
		if (game != 0 && game->getHostAddress()->Rva00248CBF(address) && !game->isGameInProgress())
		{
			AsciiString options = GenerateGameOptionsString();
			if (OnGameOptionsPacked(*address, 0, message->GameOptions.options, LAN_MAX_OPTIONS_LENGTH))
			{
				options.compare(GenerateGameOptionsString());
				rva00248E2F(false);
				if (wotr)
					OnWOTRGameStart();
				else
					OnGameStart();
			}
			else
			{
				OnPlayerLeave(m_name);
				removeGame(m_currentGame);
				::delete m_currentGame;
				m_currentGame = 0;
				m_inLobby = true;
			}
		}
	}
}
