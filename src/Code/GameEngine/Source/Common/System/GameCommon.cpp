// Tier A; object=O00238954 evidence=W flags=align=U
// Portions derived from Command & Conquer Generals Zero Hour, (c) Electronic Arts Inc., GPL-3.0-or-later; see LICENSE and NOTICE.
#include <stddef.h>
#include "Common/Veterancy.h"
#include "Common/GameType.h"

// The level and relationship names (retail VA 0x00DBA4C0 and 0x00DBA4D4, in this object's .data).
const char *TheVeterancyNames[] =
{
	"REGULAR",
	"VETERAN",
	"ELITE",
	"HEROIC",
	NULL
};

const char *TheRelationshipNames[] =
{
	"ENEMIES",
	"NEUTRAL",
	"ALLIES",
	NULL
};

// The logic and client frame-rate globals (retail VA 0x00DBA4E4..0x00DBA508, after the name tables in this object's .data).
// Retail keeps them writable (the rates are set at start-up), not const as Zero Hour's constants.
Int BfmeLogicFramesPerSecond = 5;
Int BfmeClientFramesPerSecond = 30;
Real g_parseDurationMsecScale = 0.005f;			// logic frames per millisecond
Real MSEC_PER_LOGICFRAME_REAL = 200.0f;
Real LOGICFRAMES_PER_SECONDS_REAL = 5.0f;
Real g_secondsPerLogicFrame = 0.2f;
Real MSEC_PER_CLIENTFRAME_REAL = 1000.0f / 30.0f;
Real BfmeClientFramesPerMillisecond = 0.03f;
Real CLIENTFRAMES_PER_SECOND_REAL = 30.0f;
Real SECONDS_PER_CLIENTFRAME_REAL = 1.0f / 30.0f;

// normalizeAngle: wrap an angle into minus PI..PI. Retail compares with SSE
// comiss and returns the spilled value through x87.
// FUNCTION: BFME2 0x00238954 size=75 name=?normalizeAngle@@YAMM@Z evidence=N:ledger

float normalizeAngle(float a)
{
	while (a > 3.14159274f)
		a -= 6.28318548f;
	while (a <= -3.14159274f)
		a += 6.28318548f;
	return a;
}
