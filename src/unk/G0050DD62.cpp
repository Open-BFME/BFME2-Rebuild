// Generated from the ledger; map snapshot c591f7146f88c8a8248ebb8ae22f0c978c22aed463db64e05ef57ff0dfae99cc
// object=G0050DD62 name-evidence=invented
// Extents and edges are hypotheses; byte verification alone does not establish original TU boundaries.
#include "GameClient/ControlBar.h"
#include "GameClient/GameWindow.h"
#include "GameClient/InGameUI.h"
#include "GameClient/KeyDefs.h"

// FUNCTION: BFME2 0x0050DD62 size=61 name=?GeneralsExpPointsInput@@YA?AW4WindowMsgHandledType@@PAVGameWindow@@III@Z evidence=N:ledger tier=B hypothesis:mm1-gap-unit
WindowMsgHandledType GeneralsExpPointsInput(GameWindow *window, unsigned int msg, unsigned int mData1, unsigned int mData2)
{
	switch (msg) {
	case 0x11:
		if (TheInGameUI)
			TheInGameUI->placeBuildAvailable(0, 0);
		break;
	case 0x15: {
		UnsignedByte key = mData1;
		switch (key) {
		case KEY_ESC:
			TheControlBar->hidePurchaseScience();
			return MSG_HANDLED;
		}
		return MSG_HANDLED;
	}
	}
	return MSG_HANDLED;
}

// FUNCTION: BFME2 0x0050DD9F size=154 name=?GeneralsExpPointsSystem@@YA?AW4WindowMsgHandledType@@PAVGameWindow@@III@Z evidence=N:ledger tier=B hypothesis:mm1-gap-unit
