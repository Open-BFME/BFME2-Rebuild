// SubsystemInterface was promoted to Common/SubsystemInterface.h (h03).
// This forward keeps headers drafted before the promotion compiling; include
// Common/SubsystemInterface.h directly in new code.
// Keep this file: deleting it while headers that include it are queued for
// landing breaks them (it refused the promotion five times). Remove it only
// once nothing on main or in flight includes it.
#pragma once
#include "Common/SubsystemInterface.h"
