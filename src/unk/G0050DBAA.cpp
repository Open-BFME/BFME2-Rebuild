// Generated from the ledger; map snapshot c591f7146f88c8a8248ebb8ae22f0c978c22aed463db64e05ef57ff0dfae99cc
// object=G0050DBAA name-evidence=invented
// Extents and edges are hypotheses; byte verification alone does not establish original TU boundaries.
// Portions derived from Command & Conquer Generals Zero Hour, (c) Electronic Arts Inc., GPL-3.0-or-later; see LICENSE and NOTICE.
#include "GameClient/GameWindow.h"
#include "GameClient/InGameUI.h"
#include "GameClient/KeyDefs.h"
#include "Common/MessageStream.h"

enum { GWM_CHAR = 21 };

// FUNCTION: BFME2 0x0050DBAA size=60 name=?BeaconWindowInput@@YA?AW4WindowMsgHandledType@@PAVGameWindow@@III@Z evidence=N:ledger tier=B hypothesis:mm1-gap-unit
WindowMsgHandledType BeaconWindowInput(GameWindow *window, unsigned int msg, unsigned int mData1, unsigned int mData2)
{
	if (msg == GWM_CHAR && mData1 == KEY_ESC) {
		GameMessage *message = TheMessageStream->appendMessage(GameMessage::MSG_DESTROY_SELECTED_GROUP);
		message->appendBooleanArgument(true);
		TheInGameUI->deselectAllDrawables();
		return MSG_HANDLED;
	}
	return MSG_IGNORED;
}
